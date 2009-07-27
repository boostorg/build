# Copyright Pedro Ferreira 2005. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

import bjam

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
        
        # Object Map.
        # TODO: This is a kludge: maps object names to the actual instances.
        # Sometimes, objects are stored in properties, along with some grist.
        # This map is used to store the value and return an id, which can be later on used to retriev it back.
        self.object_map_ = {}

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

    def register_object (self, value):
        """ Stores an object in a map and returns a key that can be used to retrieve it.
        """
        key = 'object_registry_' + str (value)
        self.object_map_ [key] = value
        return key
    
    def get_object (self, key):
        """ Returns a previously registered object.
        """
        if not isinstance (key, str):
            # Probably it's the object itself.
            return key
            
        return self.object_map_ [key]

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
    
