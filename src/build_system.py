# Status: mostly ported. Missing is --out-xml support, 'configure' integration
# and some FIXME.
# Base revision: 64351

# Copyright 2003, 2005 Dave Abrahams
# Copyright 2006 Rene Rivera
# Copyright 2003, 2004, 2005, 2006, 2007 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)


from b2.build.engine import Engine
from b2.manager import Manager
from b2.util.path import glob
from b2.build import feature, property_set
import b2.build.virtual_target
from b2.build.targets import ProjectTarget
from b2.util.sequence import unique
import b2.build.build_request
from b2.build.errors import ExceptionWithUserContext
import b2.tools.common
from b2.build.toolset import using

import b2.build.project as project
import b2.build.virtual_target as virtual_target
import b2.build.build_request as build_request


import b2.util.regex

from b2.manager import get_manager
from b2.util import cached
from b2.util import option


import bjam

import os
import sys
import re
import json

################################################################################
#
# Module global data.
#
################################################################################

# Flag indicating we should display additional debugging information related to
# locating and loading Boost Build configuration files.
debug_config = False

# The cleaning is tricky. Say, if user says 'bjam --clean foo' where 'foo' is a
# directory, then we want to clean targets which are in 'foo' as well as those
# in any children Jamfiles under foo but not in any unrelated Jamfiles. To
# achieve this we collect a list of projects under which cleaning is allowed.
project_targets = []

# Virtual targets obtained when building main targets references on the command
# line. When running 'bjam --clean main_target' we want to clean only files
# belonging to that main target so we need to record which targets are produced
# for it.
results_of_main_targets = []

# Was an XML dump requested?
out_xml = False

# Default toolset & version to be used in case no other toolset has been used
# explicitly by either the loaded configuration files, the loaded project build
# scripts or an explicit toolset request on the command line. If not specified,
# an arbitrary default will be used based on the current host OS. This value,
# while not strictly necessary, has been added to allow testing Boost-Build's
# default toolset usage functionality.
default_toolset = None
default_toolset_version = None

################################################################################
#
# Public rules.
#
################################################################################

# Returns the property set with the free features from the currently processed
# build request.
#
def command_line_free_features():
    return command_line_free_features

# Sets the default toolset & version to be used in case no other toolset has
# been used explicitly by either the loaded configuration files, the loaded
# project build scripts or an explicit toolset request on the command line. For
# more detailed information see the comment related to used global variables.
#
def set_default_toolset(toolset, version=None):
    default_toolset = toolset
    default_toolset_version = version


pre_build_hook = []

def add_pre_build_hook(callable):
    pre_build_hook.append(callable)

post_build_hook = None

def set_post_build_hook(callable):
    post_build_hook = callable

################################################################################
#
# Local rules.
#
################################################################################


_target_id_split = re.compile("(.*)//(.*)")

# Given a target id, try to find and return the corresponding target. This is
# only invoked when there is no Jamfile in ".". This code somewhat duplicates
# code in project-target.find but we can not reuse that code without a
# project-targets instance.
#
def find_target(target_id):

    projects = get_manager().projects()
    m = _target_id_split.match(target_id)
    if m:
        pm = projects.find(m.group(1), ".")
    else:
        pm = projects.find(target_id, ".")

    if pm:
        result = projects.target(pm)

    if m:
        result = result.find(m.group(2))

    return result

def initialize_config_module(module_name, location=None):

    get_manager().projects().initialize(module_name, location)

# Helper rule used to load configuration files. Loads the first configuration
# file with the given 'filename' at 'path' into module with name 'module-name'.
# Not finding the requested file may or may not be treated as an error depending
# on the must-find parameter. Returns a normalized path to the loaded
# configuration file or nothing if no file was loaded.
#
def load_config(module_name, filename, paths, must_find=False):

    if debug_config:
        print "notice: Searching  '%s' for '%s' configuration file '%s." \
              % (paths, module_name, filename)

    where = None
    for path in paths:
        t = os.path.join(path, filename)
        if os.path.exists(t):
            where = t
            break

    if where:
        where = os.path.realpath(where)

        if debug_config:
            print "notice: Loading '%s' configuration file '%s' from '%s'." \
                  % (module_name, filename, where)

        # Set source location so that path-constant in config files
        # with relative paths work. This is of most importance
        # for project-config.jam, but may be used in other
        # config files as well.
        attributes = get_manager().projects().attributes(module_name) ;
        attributes.set('source-location', os.path.dirname(where), True)
        get_manager().projects().load_standalone(module_name, where)

    else:
        msg = "Configuration file '%s' not found in '%s'." % (filename, path)
        if must_find:
            get_manager().errors()(msg)

        elif debug_config:
            print msg

    return where

# Loads all the configuration files used by Boost Build in the following order:
#
#   -- test-config --
#   Loaded only if specified on the command-line using the --test-config
# command-line parameter. It is ok for this file not to exist even if
# specified. If this configuration file is loaded, regular site and user
# configuration files will not be. If a relative path is specified, file is
# searched for in the current folder.
#
#   -- site-config --
#   Always named site-config.jam. Will only be found if located on the system
# root path (Windows), /etc (non-Windows), user's home folder or the Boost
# Build path, in that order. Not loaded in case the test-config configuration
# file is loaded or the --ignore-site-config command-line option is specified.
#
#   -- user-config --
#   Named user-config.jam by default or may be named explicitly using the
# --user-config command-line option or the BOOST_BUILD_USER_CONFIG environment
# variable. If named explicitly the file is looked for from the current working
# directory and if the default one is used then it is searched for in the
# user's home directory and the Boost Build path, in that order. Not loaded in
# case either the test-config configuration file is loaded or an empty file
# name is explicitly specified. If the file name has been given explicitly then
# the file must exist.
#
# Test configurations have been added primarily for use by Boost Build's
# internal unit testing system but may be used freely in other places as well.
#
def load_configuration_files():

    # Flag indicating that site configuration should not be loaded.
    ignore_site_config = "--ignore-site-config" in sys.argv

    initialize_config_module("test-config")
    test_config = None
    for a in sys.argv:
        m = re.match("--test-config=(.*)$", a)
        if m:
            test_config = b2.util.unquote(m.group(1))
            break

    if test_config:
        where = load_config("test-config", os.path.basename(test_config), [os.path.dirname(test_config)])
        if where:
            if debug_config:
                print "notice: Regular site and user configuration files will"
                print "notice: be ignored due to the test configuration being loaded."

    user_path = [os.path.expanduser("~")] + bjam.variable("BOOST_BUILD_PATH")
    site_path = ["/etc"] + user_path
    if os.name in ["nt"]:
        site_path = [os.getenv("SystemRoot")] + user_path

    if debug_config and not test_config and ignore_site_config:
        print "notice: Site configuration files will be ignored due to the"
        print "notice: --ignore-site-config command-line option."

    initialize_config_module("site-config")
    if not test_config and not ignore_site_config:
        load_config('site-config', 'site-config.jam', site_path)

    initialize_config_module('user-config')
    if not test_config:

        # Here, user_config has value of None if nothing is explicitly
        # specified, and value of '' if user explicitly does not want
        # to load any user config.
        user_config = None
        for a in sys.argv:
            m = re.match("--user-config=(.*)$", a)
            if m:
                user_config = m.group(1)
                break

        if user_config is None:
            user_config = os.getenv("BOOST_BUILD_USER_CONFIG")

        # Special handling for the case when the OS does not strip the quotes
        # around the file name, as is the case when using Cygwin bash.
        user_config = b2.util.unquote(user_config)
        explicitly_requested = user_config

        if user_config is None:
            user_config = "user-config.jam"

        if user_config:
            if explicitly_requested:

                user_config = os.path.abspath(user_config)

                if debug_config:
                    print "notice: Loading explicitly specified user configuration file:"
                    print "    " + user_config

                    load_config('user-config', os.path.basename(user_config), [os.path.dirname(user_config)], True)
            else:
                load_config('user-config', os.path.basename(user_config), user_path)
        else:
            if debug_config:
                print "notice: User configuration file loading explicitly disabled."

    # We look for project-config.jam from "." upward. I am not sure this is
    # 100% right decision, we might as well check for it only alongside the
    # Jamroot file. However:
    # - We need to load project-config.jam before Jamroot
    # - We probably need to load project-config.jam even if there is no Jamroot
    #   - e.g. to implement automake-style out-of-tree builds.
    if os.path.exists("project-config.jam"):
        file = ["project-config.jam"]
    else:
        file = b2.util.path.glob_in_parents(".", ["project-config.jam"])

    if file:
        initialize_config_module('project-config', os.path.dirname(file[0]))
        load_config('project-config', "project-config.jam", [os.path.dirname(file[0])], True)


# Autoconfigure toolsets based on any instances of --toolset=xx,yy,...zz or
# toolset=xx,yy,...zz in the command line. May return additional properties to
# be processed as if they had been specified by the user.
#
def process_explicit_toolset_requests():

    extra_properties = []

    option_toolsets = [e for option in b2.util.regex.transform(sys.argv, "^--toolset=(.*)$")
                       for e in option.split(',')]
    feature_toolsets = [e for option in b2.util.regex.transform(sys.argv, "^toolset=(.*)$")
                       for e in option.split(',')]

    for t in option_toolsets + feature_toolsets:

        # Parse toolset-version/properties.
        (toolset_version, toolset, version) = re.match("(([^-/]+)-?([^/]+)?)/?.*", t).groups()

        if debug_config:
            print "notice: [cmdline-cfg] Detected command-line request for '%s': toolset= %s version=%s" \
            % (toolset_version, toolset, version)

        # If the toolset is not known, configure it now.
        known = False
        if toolset in feature.values("toolset"):
            known = True

        if known and version and not feature.is_subvalue("toolset", toolset, "version", version):
            known = False
        # TODO: we should do 'using $(toolset)' in case no version has been
        # specified and there are no versions defined for the given toolset to
        # allow the toolset to configure its default version. For this we need
        # to know how to detect whether a given toolset has any versions
        # defined. An alternative would be to do this whenever version is not
        # specified but that would require that toolsets correctly handle the
        # case when their default version is configured multiple times which
        # should be checked for all existing toolsets first.

        if not known:

            if debug_config:
                print "notice: [cmdline-cfg] toolset '%s' not previously configured; attempting to auto-configure now" % toolset_version
            if version is not None:
               using(toolset, version)
            else:
               using(toolset)

        else:

            if debug_config:

                print "notice: [cmdline-cfg] toolset '%s' already configured" % toolset_version

        # Make sure we get an appropriate property into the build request in
        # case toolset has been specified using the "--toolset=..." command-line
        # option form.
        if not t in sys.argv and not t in feature_toolsets:

            if debug_config:
                print "notice: [cmdline-cfg] adding toolset=%s) to the build request." % t ;
            extra_properties += "toolset=%s" % t

    return extra_properties

class Builder:

        def __init__(self, current_project, metatarget_ids, manager):

            self.manager = manager

            self.metatargets = []
            self.directly_requested_targets = []
            self.engine_targets = []
            self.config_log = None

            projects = manager.projects()

            for id in metatarget_ids:
                t = None
                if current_project:
                    t = current_project.find(id, no_error=1)
                else:
                    t = find_target(id)

                if not t:
                    manager.errors()("could not find metatarget '%s'" % id)
                else:
                    self.metatargets.append(t)

            if not self.metatargets:
                self.metatargets = [projects.target(projects.module_name("."))]


        def setup_config_log(self):

            # We wish to put config.log in the build directory corresponding
            # to Jamroot, so that the location does not differ depending on
            # directory where we do build.  The amount of indirection necessary
            # here is scary.
            first_project = self.metatargets[0].project()
            first_project_root_location = first_project.get('project-root')
            first_project_root_module = self.manager.projects().load(first_project_root_location)
            first_project_root = self.manager.projects().target(first_project_root_module)
            first_build_build_dir = first_project_root.build_dir()
            path = os.path.join(first_build_build_dir, "config.log")
            if self.config_log and path != self.config_log:
                self.manager.error()("location of configuration log changed");
            self.config_log = path;

            import b2.build.configure as configure
            configure.set_log_file(path)

        def generate(self, properties):

            # Generation of targets might invoke configuration checks,
            # so make sure config.log is setup.
            self.setup_config_log()


            self.manager.set_command_line_free_features(property_set.create(properties.free()))

            targets = []
            for t in self.metatargets:
                try:
                    g = t.generate(properties)
                    if not isinstance(t, ProjectTarget):
                        self.directly_requested_targets.extend(g.targets())
                    targets.extend(g.targets())
                except ExceptionWithUserContext, e:
                    e.report()
                except Exception:
                    raise

            self.engine_targets.extend([t.actualize() for t in targets])

        def build(self):
            bjam.call("DEPENDS", "all", self.engine_targets)
            ok = bjam.call("UPDATE_NOW", "all")
            # Prevent automatic update of the 'all' target, now that
            # we have explicitly updated what we wanted.
            bjam.call("UPDATE")

            return ok

        def clean(self):

            """Instead of building, clean the requested targets

               If a non-project metatarget was requested, we want to clean
               final targets as well as intermediate targets, but not sources
               or roots from other metatargets.

               If a project metatarget was requested, we want to clean all targets
               below that project.
            """

            to_clean = set([t for root in self.directly_requested_targets for t in virtual_target.traverse(root)])

            projects = {m for m in self.metatargets if isinstance(m, b2.build.targets.ProjectTarget)}

            @cached
            def should_clean_project(p):
                if p in projects:
                    return True
                elif p.parent().is_root():
                    return False
                else:
                    return should_clean_project(p.parent())

            to_clean.update([t for t in self.manager.virtual_targets().all_targets()
                                    if t.action() and should_clean_project(t.project())])

            self.manager.engine().set_update_action("common.Clean", "clean",
                                                    [t.actualize() for t in to_clean])
            bjam.call("UPDATE_NOW", "clean")


################################################################################
#
# main()
# ------
#
################################################################################

def main():

    sys.argv = bjam.variable("ARGV")

    # FIXME: document this option.
    if "--profiling" in sys.argv:
        import cProfile
        r = cProfile.runctx('main_real()', globals(), locals(), "stones.prof")

        import pstats
        stats = pstats.Stats("stones.prof")
        stats.strip_dirs()
        stats.sort_stats('time', 'calls')
        stats.print_callers(20)
        return r
    else:
        try:
            return main_real()
        except ExceptionWithUserContext, e:
            e.report()




def command_line_build(current_project, manager, metatarget_ids, properties):
    # Expand properties specified on the command line into multiple property
    # sets consisting of all legal property combinations. Each expanded property
    # set will be used for a single build run. E.g. if multiple toolsets are
    # specified then requested targets will be built with each of them.
    if properties:
        expanded = build_request.expand_no_defaults(properties)
    else:
        expanded = [property_set.empty()]

    # Check that we actually found something to build.
    if not current_project and not metatarget_ids:
        get_manager().errors()("no Jamfile in current directory found, and no target references specified.")
        # FIXME:
        # EXIT
    # Flags indicating that this build system run has been started in order to
    # clean existing instead of create new targets. Note that these are not the
    # final flag values as they may get changed later on due to some special
    # targets being specified on the command line.
    clean = "--clean" in sys.argv
    if "clean" in metatarget_ids:
        clean = True
        metatarget_ids = [t for t in metatarget_ids if t != "clean"]
    cleanall = "--clean-all" in sys.argv
    # List of explicitly requested files to build. Any target references read
    # from the command line parameter not recognized as one of the targets
    # defined in the loaded Jamfiles will be interpreted as an explicitly
    # requested file to build. If any such files are explicitly requested then
    # only those files and the targets they depend on will be built and they
    # will be searched for among targets that would have been built had there
    # been no explicitly requested files.
    explicitly_requested_files = []
    manager.errors().push_user_context("Processing build request")
    # FIXME: put this BACK.
    ## if [ option.get dump-generators : : true ]
    ## {
    ##     generators.dump ;
    ## }
    virtual_targets = []
    builder = Builder(current_project, metatarget_ids, manager)

    j = option.get("jobs")
    if j:
        bjam.call("set-variable", 'PARALLELISM', j)
    k = option.get("keep-going", "true", "true")
    if k in ["on", "yes", "true"]:
        bjam.call("set-variable", "KEEP_GOING", "1")
    elif k in ["off", "no", "false"]:
        bjam.call("set-variable", "KEEP_GOING", "0")
    else:
        print "error: Invalid value for the --keep-going option"
        sys.exit()

        # Now that we have a set of targets to build and a set of property sets to
    # build the targets with, we can start the main build process by using each
    # property set to generate virtual targets from all of our listed targets
    # and any of their dependants.
    for p in expanded:
        builder.generate(p)

    # The 'all' pseudo target is not strictly needed expect in the case when we
    # use it below but people often assume they always have this target
    # available and do not declare it themselves before use which may cause
    # build failures with an error message about not being able to build the
    # 'all' target.
    bjam.call("NOTFILE", "all")
    if cleanall:
        # Just clean totally everything.
        bjam.call("UPDATE", "clean-all")
    elif clean:
        builder.clean()
    else:
        # FIXME:
        # configure.print-configure-checks-summary ;

        if pre_build_hook:
            for h in pre_build_hook:
                h()

        ok = builder.build()

        if post_build_hook:
            post_build_hook(ok)

class Server:

    def __init__(self, current_project, manager):
        self.current_project = current_project
        self.manager = manager

    def main_loop(self):

        while True:
            line = sys.stdin.readline()
            if not line:
                break

            j = json.loads(line)

            if j['type'] == 'request':
                r = j['request']
                if r == "build" or r == "clean":
                    metatarget_ids = j.get('metatargets', [])
                    properties = j.get('properties', [])
                    properties = property_set.create(properties)

                    builder = Builder(self.current_project, metatarget_ids, self.manager)
                    builder.generate(properties)
                    ok = builder.build()

                    done = {"type": "event",
                            "event": "build-finished",
                            "success": "true" if ok else "false"
                            }
                    print json.dumps(done)
                    sys.stdout.flush()

                elif r == "get":
                    p = j.get('path')

                    if p == 'properties':
                        pass


            else:
                print "unknown type"

        print "Running main loop"


def main_real():

    global debug_config, out_xml

    debug_config = "--debug-configuration" in sys.argv
    out_xml = any(re.match("^--out-xml=(.*)$", a) for a in sys.argv)

    engine = Engine()

    global_build_dir = option.get("build-dir")
    manager = Manager(engine, global_build_dir)



    if "--version" in sys.argv:
        from b2.build import version
        version.report()
        return

    # This module defines types and generator and what not,
    # and depends on manager's existence
    import b2.tools.builtin

    b2.tools.common.init(manager)

    load_configuration_files()

    # Load explicitly specified toolset modules.
    extra_properties = process_explicit_toolset_requests()

    # Load the actual project build script modules. We always load the project
    # in the current folder so 'use-project' directives have any chance of
    # being seen. Otherwise, we would not be able to refer to subprojects using
    # target ids.
    current_project = None
    projects = get_manager().projects()
    if projects.find(".", "."):
        current_project = projects.target(projects.load("."))

    # Load the default toolset module if no other has already been specified.
    if not feature.values("toolset"):

        dt = default_toolset
        dtv = None
        if default_toolset:
            dtv = default_toolset_version
        else:
            dt = "gcc"
            if os.name == 'nt':
                dt = "msvc"
            # FIXME:
            #else if [ os.name ] = MACOSX
            #{
            #    default-toolset = darwin ;
            #}

        print "warning: No toolsets are configured."
        print "warning: Configuring default toolset '%s'." % dt
        print "warning: If the default is wrong, your build may not work correctly."
        print "warning: Use the \"toolset=xxxxx\" option to override our guess."
        print "warning: For more configuration options, please consult"
        print "warning: http://boost.org/boost-build2/doc/html/bbv2/advanced/configuration.html"

        using(dt, dtv)

    if "--server" in sys.argv:

        if not current_project:
            manager.errors()("Server mode requires starting in project root")

        if not current_project.is_root():
            manager.errors()("Server mode requires starting in project root")

        server = Server(current_project, manager)
        server.main_loop()

    else:
        # Parse command line for targets and properties. Note that this requires
        # that all project files already be loaded.
        (metatarget_ids, properties) = build_request.from_command_line(sys.argv[1:] + extra_properties)

        command_line_build(current_project, manager, metatarget_ids, properties)


        if manager.errors().count() == 0:
            return ["ok"]
        else:
            return []
