# Copyright Pedro Ferreira 2005. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

import bjam
import json
import os
import sys

# To simplify implementation of tools level, we'll
# have a global variable keeping the current manager.
the_manager = None
def get_manager():
    return the_manager

class Manager:
    """ This class is a facade to the Boost.Build system.
        It serves as the root to access all data structures in use.
    """
        
    def __init__ (self, engine, global_build_dir):
        """ Constructor.
            engine: the build engine that will actually construct the targets.
        """
        from build.virtual_target import VirtualTargetRegistry
        from build.targets import TargetRegistry
        from build.project import ProjectRegistry
        from build.scanner import ScannerRegistry
        from build.errors import Errors
        from b2.util.logger import NullLogger
        from build import build_request, property_set, feature
        
        self.engine_ = engine
        self.virtual_targets_ = VirtualTargetRegistry (self)
        self.projects_ = ProjectRegistry (self, global_build_dir)
        self.targets_ = TargetRegistry ()
        self.logger_ = NullLogger ()
        self.scanners_ = ScannerRegistry (self)
        self.argv_ = bjam.variable("ARGV")
        self.boost_build_path_ = bjam.variable("BOOST_BUILD_PATH")
        self.errors_ = Errors()
        self.command_line_free_features_ = property_set.empty()

        self.events_enabled = False
        self.next_event_token = 0
        self.message_context_ = []
        
        global the_manager
        the_manager = self
        
    def scanners (self):
        return self.scanners_

    def engine (self):
        return self.engine_
        
    def virtual_targets (self):
        return self.virtual_targets_

    def targets (self):
        return self.targets_

    def projects (self):
        return self.projects_

    def argv (self):
        return self.argv_
    
    def logger (self):
        return self.logger_

    def set_logger (self, logger):
        self.logger_ = logger

    def errors (self):
        return self.errors_

    def getenv(self, name):
        return bjam.variable(name)

    def boost_build_path(self):
        return self.boost_build_path_

    def command_line_free_features(self):
        return self.command_line_free_features_

    def set_command_line_free_features(self, v):
        self.command_line_free_features_ = v

    def construct (self, properties = [], targets = []):
        """ Constructs the dependency graph.
            properties:  the build properties.
            targets:     the targets to consider. If none is specified, uses all.
        """
        if not targets:
            for name, project in self.projects ().projects ():
                targets.append (project.target ())
            
        property_groups = build_request.expand_no_defaults (properties)

        virtual_targets = []
        build_prop_sets = []
        for p in property_groups:
            build_prop_sets.append (property_set.create (feature.split (p)))

        if not build_prop_sets:
            build_prop_sets = [property_set.empty ()]

        for build_properties in build_prop_sets:
            for target in targets:
                result = target.generate (build_properties)
                virtual_targets.extend (result.targets ())

        actual_targets = []
        for virtual_target in virtual_targets:
            actual_targets.extend (virtual_target.actualize ())

    def enable_events(self, value=True, file = None):
        self.events_enabled = value
        self.events_output = file if file else sys.stdout

    def get_token(self):
        r = "e" + str(self.next_event_token)
        self.next_event_token = self.next_event_token + 1
        return r

    def output_event(self, str):

        if not str:
            return

        if str[-1] == os.linesep:
            self.events_output.write(str)
        else:
            self.events_output.write(str)
            self.events_output.write(os.linesep)

    def push_message_context(self, token):
        self.message_context_.append(token)

    def pop_message_context(self):
        self.message_context_.pop()

    def message_context(self):
        return self.message_context_[-1] if len(self.message_context_) else None

    def message(self, message, kind='information', submessages=[], parent = None):
        if not self.events_enabled:
            return

        t = self.get_token()
        j = {
            'token': t,
            'type': 'event',
            'event': 'message',
            'kind': kind,
            'message':  message
        }
        if not parent:
            parent = self.message_context()
        if  parent:
            j['parent'] = parent
        self.output_event(json.dumps(j))
        for s in submessages:
            self.output_event(json.dumps({
                'parent': t,
                'type': 'event',
                'event': 'message',
                'message': s
            }))
        self.events_output.flush()

        return t

    def build_started(self, action, commands):
        if not self.events_enabled:
            return

        j = {
            'token': id(action),
            'type': 'event',
            'event': 'build-action-started',
            'action-name': action.action_name(),
            # FIXME: need to filter out non-file target.
            'targets': [t.full_name() for t in action.targets()],
            'sources': [t.full_name() for t in action.sources()],
            'commands': commands,
            'properties': action.properties().json()
        }
        parent = self.message_context()
        if parent:
            j['parent'] = parent

        self.output_event(json.dumps(j))

    def build_output(self, action, stream, output):
        if not self.events_enabled:
            return
        while output:
            n = output.find(os.linesep)
            if n != -1:
                line = output[:n]
                output = output[n+1:]
            else:
                line = output
                output = None

            j = {
            'parent': id(action),
            'type': 'event',
            'event': 'build-action-output',
            'stream': stream,
            'output': line
            }
            if line.find("error:") != -1:
                j['output-kind'] = 'error'
            elif line.find("warning:") != -1:
                j['output-kind'] = 'warning'

            self.output_event(json.dumps(j))

    def build_finished(self, action, exit_status):
        if not self.events_enabled:
            return
        self.output_event(json.dumps({
            'type': 'event',
            'token': id(action),
            'event': 'build-action-finished',
            'exit-status': exit_status
        }))

