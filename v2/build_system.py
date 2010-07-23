# Status: being ported by Vladimir Prus.

# Copyright 2003, 2005 Dave Abrahams 
# Copyright 2006 Rene Rivera 
# Copyright 2003, 2004, 2005, 2006, 2007 Vladimir Prus 
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

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

import bjam

import os
import sys

# FIXME:
# Returns the location of the build system. The primary use case
# is building Boost, where it's sometimes needed to get location
# of other components (like BoostBook files), and it's convenient
# to use location relatively to Boost.Build path.
#rule location ( )
#{
#    local r = [ modules.binding build-system ] ;
#    return $(r:P) ;
#}

# FIXME:

def get_boolean_option(name):
    match = "--" + name
    if match in argv:
        return 1
    else:
        return 0

def get_string_option(name):
    match = "--" + name + "="
    
    for arg in argv:
        if arg.startswith(match):
            return arg[len(match):]
    return None

def home_directories():
    if os.name == "nt":
        result = set()
        try:
            result.add(os.environ['HOMEDRIVE'] + os.environ['HOMEPATH'])
            result.add(os.environ['HOME'])
            result.add(os.environ['USERPROFILE'])
        except KeyError:
            pass
        return list(result)
    else:
        return [os.environ['HOME']]

ignore_config = 0
debug_config = 0

def load_config(manager, basename, path):
    """Unless ignore-config is set, search configuration
    basename.jam in path and loads it.  The jamfile module
    for that file will be loaded 'basename'."""

    if not ignore_config:
        found = glob(path, [basename + ".jam"])
        if found:
            found = found[0]
        if debug_config:
            print "notice: searching '%s' for '%s.jam'" % (path, basename)
            if found:
                print "notice: loading %s.jam from %s" % (basename, found)

        manager.projects().load_standalone(basename, found)

def main():

    global argv
    argv = bjam.variable("ARGV")

    # FIXME: document this option.
    if "--profiling" in argv:
        import cProfile
        import pstats
        cProfile.runctx('main_real()', globals(), locals(), "stones.prof")
        
        stats = pstats.Stats("stones.prof")
        stats.strip_dirs()
        stats.sort_stats('time', 'calls')
        stats.print_callers(20)
    else:
        main_real()

def main_real():

    global ignore_config
    global debug_config
    
    boost_build_path = bjam.variable("BOOST_BUILD_PATH")

    engine = Engine()

    global_build_dir = get_string_option("build-dir")
    debug_config = get_boolean_option("debug-configuration")
    
    manager = Manager(engine, global_build_dir)

    # This module defines types and generator and what not,
    # and depends on manager's existence
    import b2.tools.builtin


    # Check if we can load 'test-config.jam'. If we can, load it and
    # ignore user configs.
    
    test_config = glob(boost_build_path, ["test-config.jam"])
    if test_config:
        test_config = test_config[0]

    if test_config:
        if debug_config:
            print "notice: loading testing-config.jam from '%s'" % test_config
            print "notice: user-config.jam and site-config.jam will be ignored"

        manager.projects().load_standalone("test-config", test_config)


    ignore_config = test_config or get_boolean_option("ignore-config")
    user_path = home_directories() + boost_build_path

    site_path = ["/etc"] + user_path
    if bjam.variable("OS") in ["NT", "CYGWIN"]:
        site_path = [os.environ("SystemRoot")] + user_path

    load_config(manager, "site-config", site_path)

    user_config_path = get_string_option("user-config")
    if not user_config_path:
        user_config_path = os.environ.get("BOOST_BUILD_USER_CONFIG")

    if user_config_path:
        if debug_config:
            print "Loading explicitly specifier user configuration file:"
            print "    %s" % user_config_path
            
        manager.projects().load_standalone("user-config", user_config_path)

    else:
        load_config(manager, "user-config", user_path)
        

# FIXME:
## #
## # Autoconfigure toolsets based on any instances of --toolset=xx,yy,...zz or
## # toolset=xx,yy,...zz in the command line
## #
## local option-toolsets = [ regex.split-list [ MATCH ^--toolset=(.*) : $(argv) ] : "," ] ;
## local feature-toolsets = [ regex.split-list [ MATCH ^toolset=(.*) : $(argv) ] : "," ] ;

## # if the user specified --toolset=..., we need to add toolset=... to
## # the build request
## local extra-build-request ;

    extra_build_request = []

## if ! $(ignore-config)
## {
##     for local t in $(option-toolsets) $(feature-toolsets)
##     {
##         # Parse toolset-version/properties
##         local (t-v,t,v) = [ MATCH (([^-/]+)-?([^/]+)?)/?.* : $(t) ] ;
##         local toolset-version = $((t-v,t,v)[1]) ;
##         local toolset = $((t-v,t,v)[2]) ;
##         local version = $((t-v,t,v)[3]) ;

##         if $(debug-config)
##         {
##             ECHO notice: [cmdline-cfg] Detected command-line request for 
##               $(toolset-version): toolset= \"$(toolset)\" "version= \""$(version)\" ;
##         }

##         local known ;

##         # if the toolset isn't known, configure it now.
##         if $(toolset) in [ feature.values <toolset>  ]
##         {
##             known = true ;
##         }

##         if $(known) && $(version) 
##           && ! [ feature.is-subvalue toolset : $(toolset) : version : $(version) ]
##         {
##             known = ;
##         }

##         if ! $(known)
##         {
##             if $(debug-config)
##             {
##                 ECHO notice: [cmdline-cfg] toolset $(toolset-version) 
##                   not previously configured; configuring now ; 
##             }
##             toolset.using $(toolset) : $(version) ;
##         }
##         else
##         {
##             if $(debug-config)
##             {
##                 ECHO notice: [cmdline-cfg] toolset $(toolset-version) already configured ;
##             }
##         }

##         # make sure we get an appropriate property into the build request in
##         # case the user used the "--toolset=..." form
##         if ! $(t) in $(argv)
##             && ! $(t) in $(feature-toolsets) 
##         {
##             if $(debug-config)
##             {
##                 ECHO notice: [cmdline-cfg] adding toolset=$(t) "to build request." ;
##             }
##             extra-build-request += toolset=$(t) ;
##         }
##     }
## }


# FIXME:
## if USER_MODULE in [ RULENAMES ]
## {
##     USER_MODULE site-config user-config ;
## }

    if get_boolean_option("version"):
        # FIXME: Move to a separate module. Include bjam
        # verision.
        print "Boost.Build M15 (Python port in development)"
        sys.exit(0)

    b2.tools.common.init(manager)        

    # We always load project in "." so that 'use-project' directives has
    # any chance of been seen. Otherwise, we won't be able to refer to
    # subprojects using target ids.

    current_project = None
    projects = manager.projects()
    if projects.find(".", "."):
        current_project = projects.target(projects.load("."))

    # FIXME: revive this logic, when loading of gcc works
    if not feature.values("<toolset>") and not ignore_config and 0:
        default_toolset = "gcc" ;
        if bjam.variable("OS") == "NT":
            default_toolset = "msvc"
               
        print "warning: No toolsets are configured." ;
        print "warning: Configuring default toolset '%s'" % default_toolset
        print "warning: If the default is wrong, you may not be able to build C++ programs."
        print "warning: Use the \"--toolset=xxxxx\" option to override our guess."
        print "warning: For more configuration options, please consult"
        print "warning: http://boost.org/boost-build2/doc/html/bbv2/advanced/configuration.html"

        projects.project_rules().using([default_toolset])

    (target_ids, properties) = b2.build.build_request.from_command_line(
        argv[1:] + extra_build_request)

    properties = [property_set.create(feature.split(ps)) for ps in properties]

    if properties:
        expanded = b2.build.build_request.expand_no_defaults(properties)
    else:
        expanded = [property_set.empty()]

    targets = []
    
    clean = get_boolean_option("clean")
    clean_all = get_boolean_option("clean-all")
    

    bjam_targets = []

    # Given a target id, try to find and return corresponding target.
    # This is only invoked when there's no Jamfile in "."
    # This code somewhat duplicates code in project-target.find but we can't  reuse
    # that code without project-targets instance.
    def find_target (target_id):
        split = target_id.split("//")
        pm = None
        if len(split) > 1:
            pm = projects.find(split[0], ".")
        else:
            pm = projects.find(target_id, ".")

        result = None
        if pm:
            result = projects.target(pm)

        if len(split) > 1:
            result = result.find(split[1])

    if not current_project and not target_ids:
        print "error: no Jamfile in current directory found, and no target references specified."
        sys.exit(1)

    for id in target_ids:
        if id == "clean":
            clean = 1
        else:
            t = None
            if current_project:
                t = current_project.find(id, no_error=1)
            else:
                t = find_target(id)

            if not t:
                print "notice: could not find main target '%s'" % id
                print "notice: assuming it's a name of file to create " ;
                bjam_targets.append(id)
            else:
                targets.append(t)

    if not targets:
        targets = [projects.target(projects.module_name("."))]
    
    virtual_targets = []

    # Virtual targets obtained when building main targets references on
    # the command line. When running
    #
    #   bjam --clean main_target
    #
    # we want to clean the files that belong only to that main target,
    # so we need to record which targets are produced.
    results_of_main_targets = []

    for p in expanded:
        manager.set_command_line_free_features(property_set.create(p.free()))
        
        for t in targets:
            try:
                g = t.generate(p)
                if not isinstance(t, ProjectTarget):
                    results_of_main_targets.extend(g.targets())
                virtual_targets.extend(g.targets())
            except ExceptionWithUserContext, e:
                e.report()
            except Exception:                
                raise

    # The cleaning is tricky. Say, if
    # user says: 
    #
    #    bjam --clean foo
    #
    # where 'foo' is a directory, then we want to clean targets
    # which are in 'foo' or in any children Jamfiles, but not in any
    # unrelated Jamfiles. So, we collect the list of project under which
    # cleaning is allowed.
    #
    projects_to_clean = []
    targets_to_clean = []
    if clean or clean_all:
        for t in targets:
            if isinstance(t, ProjectTarget):
                projects_to_clean.append(t.project_module())
                
            for t in results_of_main_targets:
                # Don't include roots or sources.                
                targets_to_clean += b2.build.virtual_target.traverse(t)
 
    targets_to_clean = unique(targets_to_clean)

    is_child_cache_ = {}

    # Returns 'true' if 'project' is a child of 'current-project',
    # possibly indirect, or is equal to 'project'.
    # Returns 'false' otherwise.
    def is_child (project):

        r = is_child_cache_.get(project, None)
        if not r:
            if project in projects_to_clean:
                r = 1
            else:
                parent = manager.projects().attribute(project, "parent-module")
                if parent and parent != "user-config":
                    r = is_child(parent)
                else:
                    r = 0

            is_child_cache_[project] = r

        return r

    actual_targets = []
    for t in virtual_targets:
        actual_targets.append(t.actualize())


    bjam.call("NOTFILE", "all")
    bjam.call("DEPENDS", "all", actual_targets)

    if bjam_targets:
        bjam.call("UPDATE", ["<e>%s" % x for x in bjam_targets])
    elif clean_all:
        bjam.call("UPDATE", "clean-all")
    elif clean:
        to_clean = []
        for t in manager.virtual_targets().all_targets():
            p = t.project()

            # Remove only derived targets.
            if t.action() and \
               (t in targets_to_clean or is_child(p.project_module())):
                to_clean.append(t)

        to_clean_actual = [t.actualize() for t in to_clean]
        manager.engine().set_update_action('common.Clean', 'clean',
                                           to_clean_actual, None)

        bjam.call("UPDATE", "clean")

    else:
        bjam.call("UPDATE", "all")        
