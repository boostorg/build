#!/usr/bin/python
# Copyright 2006 Rene Rivera
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

'''
Processing of Doxygen generated XML.
'''

import os
import os.path
import sys
import time
import string
import getopt
import glob
import re
import xml.dom.minidom

    
def usage():
    print '''
Usage:
    %s options

Options:
    --xmldir        Directory with the Doxygen xml result files.
    --output        Write the output BoostBook to the given location.
    --id            The ID of the top level BoostBook section.
    --title         The title of the top level BoostBook section.
    --enable-index  Generate additional index sections for classes and
                    types.
''' % ( sys.argv[0] )


def get_args( argv = sys.argv[1:] ):
    spec = [
        'xmldir=',
        'output=',
        'id=',
        'title=',
        'enable-index',
        'help' ]
    options = {
        '--xmldir' : 'xml',
        '--output' : None,
        '--id' : 'dox',
        '--title' : 'Doxygen'
        }
    ( option_pairs, other ) = getopt.getopt( argv, '', spec )
    map( lambda x: options.__setitem__( x[0], x[1] ), option_pairs )
    
    if options.has_key( '--help' ):
        usage()
        sys.exit(1)
    
    return {
        'xmldir' : options['--xmldir'],
        'output' : options['--output'],
        'id' : options['--id'],
        'title' : options['--title'],
        'index' : options.has_key('--enable-index')
        }

def if_attribute(node, attribute, true_value, false_value=None):
    if node.getAttribute(attribute) == 'yes':
        return true_value
    else:
        return false_value

class Doxygen2BoostBook:
    
    def __init__( self,
        #~ id=None,
        #~ title='',
        #~ last_revision=None,
        **kwargs ):
        ##
        self.args = kwargs
        self.args.setdefault('id','')
        self.args.setdefault('title','')
        self.args.setdefault('last_revision', time.asctime())
        self.args.setdefault('index', False)
        self.id = '%(id)s.reference' % self.args
        self.args['id'] = self.id
        self.boostbook = xml.dom.minidom.parseString('''<?xml version="1.0" encoding="UTF-8"?>
<section id="%(id)s" name="%(title)s" last-revision="%(last_revision)s">
    <title>%(title)s</title>
    <library-reference id="%(id)s.headers">
        <title>Headers</title>
    </library-reference>
    <index id="%(id)s.classes">
        <title>Classes</title>
    </index>
    <index id="%(id)s.index">
        <title>Index</title>
    </index>
</section>
''' % self.args )
        self.section = {
            'headers' : self._getChild('library-reference',id='%(id)s.headers' % self.args),
            'classes' : self._getChild('index',id='%(id)s.classes' % self.args),
            'index' : self._getChild('index',id='%(id)s.index' % self.args)
            }
        if not self.args['index']:
            self.section['classes'].parentNode.removeChild(self.section['classes'])
            self.section['classes'].unlink()
            del self.section['classes']
            self.section['index'].parentNode.removeChild(self.section['index'])
            self.section['index'].unlink()
            del self.section['index']
        self.symbols = {}
        self.generated = False
        self.idmap = {}
    
    def addDox( self, document ):
        ##
        self._translateNode(document.documentElement)
    
    def tostring( self ):
        self._generate()
        #~ return self.boostbook.toprettyxml('  ')
        return self.boostbook.toxml('utf-8')
    
    def _generate( self ):
        if not self.generated:
            self.generated = True
            symbols = self.symbols.keys()
            symbols.sort()
            for symbol in symbols:
                if self.symbols[symbol]['kind'] in ('header'):
                    self.section['headers'].appendChild(self.symbols[symbol]['dom'])
            for symbol in symbols:
                if self.symbols[symbol]['kind'] not in ('namespace', 'header'):
                    container = self._resolveContainer(self.symbols[symbol],
                        self.symbols[self.symbols[symbol]['header']]['dom'])
                    if container.nodeName != 'namespace':
                        ## The current BoostBook to Docbook translation doesn't
                        ## respect, nor assign, IDs to inner types of any kind.
                        ## So nuke the ID entry so as not create bogus links.
                        del self.idmap[self.symbols[symbol]['id']]
                    container.appendChild(self.symbols[symbol]['dom'])
            self._rewriteIDs(self.boostbook.documentElement)
    
    def _rewriteIDs( self, node ):
        if node.nodeName in ('link'):
            if (self.idmap.has_key(node.getAttribute('linkend'))):
                node.setAttribute('linkend',self.idmap[node.getAttribute('linkend')])
            else:
                node.removeAttribute('linkend')
        elif hasattr(node,'hasAttribute') and node.hasAttribute('id') and self.idmap.has_key(node.getAttribute('id')):
            node.setAttribute('id',self.idmap[node.getAttribute('id')])
        if node.firstChild:
            self._rewriteIDs(node.firstChild)
        if node.nextSibling:
            self._rewriteIDs(node.nextSibling)
    
    def _resolveContainer( self, cpp, root ):
        container = root
        for ns in cpp['namespace']:
            node = self._getChild('namespace',name=ns,root=container)
            if not node:
                node = container.appendChild(
                    self._createNode('namespace',name=ns))
            container = node
        for inner in cpp['name'].split('::'):
            node = self._getChild(name=inner,root=container)
            if not node:
                break
            container = node
        return container
    
    def _setID( self, id, name ):
        self.idmap[id] = name.replace('::','.').replace('/','.')
        #~ print '--| setID:',id,'::',self.idmap[id]
    
    def _translateNode( self, *context, **kwargs ):
        node = None
        name = '_translate'
        for c in context:
            if c:
                if not isinstance(c,xml.dom.Node):
                    name += '_'+c
                else:
                    name += '_'+c.nodeName
                    node = c
        name = name.replace('-','_')
        #~ print '_translateNode:', name
        if node and hasattr(self,name):
            return getattr(self,name)(node,**kwargs)
        else:
            return None
    
    def _translateChildren( self, parent, **kwargs ):
        target = kwargs['target']
        for n in parent.childNodes:
            child = self._translateNode(n,target=target)
            if child:
                target.appendChild(child)
            else:
                child = n.cloneNode(False)
                if hasattr(child,'data'):
                    child.data = child.data.strip()
                target.appendChild(child)
                self._translateChildren(n,target=child)
    
    def _translateDescription( self, node, target=None, tag='description', **kwargs ):
        description = self._getChild(tag,root=target)
        if not description:
            description = target.appendChild(self._createNode(tag))
        self._translateChildren(node,target=description)
        return description
    
    def _translate_doxygen( self, node ):
        #~ print '_translate_doxygen:', node.nodeName
        result = []
        for n in node.childNodes:
            newNode = self._translateNode(n)
            if newNode:
                result.append(newNode)
        return result
    
    def _translate_doxygenindex( self, node ):
        #~ print '_translate_doxygenindex:', node.nodeName
        if self.args['index']:
            entries = []
            classes = []
            for n in node.childNodes:
                if n.nodeName == 'compound':
                    if n.getAttribute('kind') not in ('file','dir','define'):
                        cpp = self._cppName(self._getChildData('name',root=n))
                        entry = {
                            'name' : cpp['name'],
                            'compoundname' : cpp['compoundname'],
                            'id' : n.getAttribute('refid')
                            }
                        if n.getAttribute('kind') in ('class','struct'):
                            classes.append(entry)
                        entries.append(entry)
                        for m in n.childNodes:
                            if m.nodeName == 'member':
                                cpp = self._cppName(self._getChildData('name',root=m))
                                entry = {
                                    'name' : cpp['name'],
                                    'compoundname' : cpp['compoundname'],
                                    'id' : n.getAttribute('refid')
                                    }
                                if hasattr(m,'getAttribute') and m.getAttribute('kind') in ('class','struct'):
                                    classes.append(entry)
                                entries.append(entry)
            entries.sort(lambda x,y: cmp(x['name'].lower(),y['name'].lower()))
            classes.sort(lambda x,y: cmp(x['name'].lower(),y['name'].lower()))
            self._translate_index_(entries,target=self.section['index'])
            self._translate_index_(classes,target=self.section['classes'])
        return None
    
    def _translate_index_(self, entries, target=None, **kwargs ):
        i = 0
        targetID = target.getAttribute('id')
        while i < len(entries):
            dividerKey = entries[i]['name'][0].upper()
            divider = target.appendChild(self._createNode('indexdiv',id=targetID+'.'+dividerKey))
            divider.appendChild(self._createText('title',dividerKey))
            while i < len(entries) and dividerKey == entries[i]['name'][0].upper():
                iename = entries[i]['name']
                ie = divider.appendChild(self._createNode('indexentry'))
                ie = ie.appendChild(self._createText('primaryie',iename))
                while i < len(entries) and entries[i]['name'] == iename:
                    ie.appendChild(self.boostbook.createTextNode(' ('))
                    ie.appendChild(self._createText(
                        'link',entries[i]['compoundname'],linkend=entries[i]['id']))
                    ie.appendChild(self.boostbook.createTextNode(')'))
                    i += 1
    
    def _translate_compounddef( self, node, target=None, **kwargs ):
        return self._translateNode(node,node.getAttribute('kind'))
    
    def _translate_compounddef_namespace( self, node, target=None, **kwargs ):
        #~ print '--| _translate_compounddef_namespace:', node.getAttribute('id')
        namespace = {
            'id' : node.getAttribute('id'),
            'kind' : 'namespace',
            'name' : self._getChildData('compoundname',root=node),
            'brief' : self._getChildData('briefdescription',root=node),
            'detailed' : self._getChildData('detaileddescription',root=node),
            'parsed' : False
            }
        if self.symbols.has_key(namespace['name']):
            if not self.symbols[namespace['name']]['parsed']:
                self.symbols[namespace['name']]['parsed'] = True
                #~ for n in node.childNodes:
                    #~ if hasattr(n,'getAttribute'):
                        #~ self._translateNode(n,n.getAttribute('kind'),target=target,**kwargs)
        else:
            self.symbols[namespace['name']] = namespace
            #~ self._setID(namespace['id'],namespace['name'])
        return None
    
    def _translate_compounddef_class( self, node, target=None, **kwargs ):
        return self._translate_compounddef_struct(node,tag='class',target=target,**kwargs)
    
    def _translate_compounddef_struct( self, node, tag='struct', target=None, **kwargs ):
        #~ print '--| _translate_compounddef_struct:', node.getAttribute('id')
        result = None
        includes = self._getChild('includes',root=node)
        if includes:
            ## Add the header into the output table.
            self._translate_compounddef_includes_(includes,includes,**kwargs)
            ## Compounds are the declared symbols, classes, types, etc.
            ## We add them to the symbol table, along with the partial DOM for them
            ## so that they can be organized into the output later.
            compoundname = self._getChildData('compoundname',root=node)
            compoundname = self._cppName(compoundname)
            self._setID(node.getAttribute('id'),compoundname['compoundname'])
            struct = self._createNode(tag,name=compoundname['name'].split('::')[-1])
            self.symbols[compoundname['compoundname']] = {
                'header' : includes.firstChild.data,
                'namespace' : compoundname['namespace'],
                'id' : node.getAttribute('id'),
                'kind' : tag,
                'name' : compoundname['name'],
                'dom' : struct
                }
            for n in node.childNodes:
                self._translateNode(n,target=struct,scope=compoundname['compoundname'])
            result = struct
        return result
    
    def _translate_compounddef_includes_( self, node, target=None, **kwargs ):
        name = node.firstChild.data
        if not self.symbols.has_key(name):
            self._setID(node.getAttribute('refid'),name)
            self.symbols[name] = {
                'kind' : 'header',
                'id' : node.getAttribute('refid'),
                'dom' : self._createNode('header',
                    id=node.getAttribute('refid'),
                    name=name)
                }
        return None
    
    def _translate_basecompoundref( self, ref, target=None, **kwargs ):
        inherit = target.appendChild(self._createNode('inherit',
            access=ref.getAttribute('prot')))
        self._translateChildren(ref,target=inherit)
        return
    
    def _translate_templateparamlist( self, templateparamlist, target=None, **kwargs ):
        template = target.appendChild(self._createNode('template'))
        for param in templateparamlist.childNodes:
            if param.nodeName == 'param':
                paramKind = None
                if self._getChildData('type',root=param) in (
                    'class','typename'):
                    paramKind = 'template-type-parameter'
                else:
                    paramKind = 'template-nontype-parameter'
                templateParam = template.appendChild(
                    self._createNode(paramKind,
                        name=self._getChildData('declname',root=param)))
                defval = self._getChild('defval',root=param)
                if defval:
                    templateParam.appendChild(self._createText('default',
                        self._getChildData('ref',root=defval.firstChild)))
        return template
    
    def _translate_briefdescription( self, brief, target=None, **kwargs ):
        self._translateDescription(brief,target=target,**kwargs)
        return self._translateDescription(brief,target=target,tag='purpose',**kwargs)
    
    def _translate_detaileddescription( self, detailed, target=None, **kwargs ):
        return self._translateDescription(detailed,target=target,**kwargs)
    
    def _translate_sectiondef( self, sectiondef, target=None, **kwargs ):
        self._translateNode(sectiondef,sectiondef.getAttribute('kind'),target=target,**kwargs)
    
    def _translate_sectiondef_x_( self, sectiondef, target=None, **kwargs ):
        for n in sectiondef.childNodes:
            if hasattr(n,'getAttribute'):
                self._translateNode(n,n.getAttribute('kind'),target=target,**kwargs)
        return None
    
    def _translate_sectiondef_func_( self, sectiondef, name='functions', target=None, **kwargs ):
        members = target.appendChild(self._createNode('method-group',name=name))
        for n in sectiondef.childNodes:
            if hasattr(n,'getAttribute'):
                self._translateNode(n,n.getAttribute('kind'),target=members,**kwargs)
        return members
    
    def _translate_sectiondef_public_type( self, sectiondef, target=None, **kwargs ):
        return self._translate_sectiondef_x_(sectiondef,target=target,**kwargs)
    
    def _translate_sectiondef_public_attrib( self, sectiondef, target=None, **kwargs):
        return self._translate_sectiondef_x_(sectiondef,target=target,**kwargs)
    
    def _translate_sectiondef_public_func( self, sectiondef, target=None, **kwargs ):
        return self._translate_sectiondef_func_(sectiondef,
            name='public member functions',target=target,**kwargs)
    
    def _translate_sectiondef_public_static_func( self, sectiondef, target=None, **kwargs):
        return self._translate_sectiondef_func_(sectiondef,
            name='public static functions',target=target,**kwargs)
    
    def _translate_sectiondef_protected_func( self, sectiondef, target=None, **kwargs ):
        return self._translate_sectiondef_func_(sectiondef,
            name='protected member functions',target=target,**kwargs)
    
    def _translate_sectiondef_private_static_func( self, sectiondef, target=None, **kwargs):
        return self._translate_sectiondef_func_(sectiondef,
            name='private static functions',target=target,**kwargs)
    
    def _translate_memberdef_typedef( self, memberdef, target=None, scope=None, **kwargs ):
        self._setID(memberdef.getAttribute('id'),
            scope+'::'+self._getChildData('name',root=memberdef))
        typedef = target.appendChild(self._createNode('typedef',
            id=memberdef.getAttribute('id'),
            name=self._getChildData('name',root=memberdef)))
        typedef_type = typedef.appendChild(self._createNode('type'))
        self._translateChildren(self._getChild('type',root=memberdef),target=typedef_type)
        return typedef
    
    def _translate_memberdef_function( self, memberdef, target=None, scope=None, **kwargs ):
        ## The current BoostBook to Docbook translator doesn't respect method
        ## Ids. Nor does it assign any useable IDs to the individial methods.
        # self._setID(memberdef.getAttribute('id'),
        #     scope+'::'+self._getChildData('name',root=memberdef))
        ## Hence instead of registering an ID for the method we point it at the
        ## containing class.
        self._setID(memberdef.getAttribute('id'),scope)
        method = target.appendChild(self._createNode('method',
            # id=memberdef.getAttribute('id'),
            name=self._getChildData('name',root=memberdef),
            cv=' '.join([
                if_attribute(memberdef,'const','const','').strip()
                ]),
            specifiers=' '.join([
                if_attribute(memberdef,'static','static',''),
                if_attribute(memberdef,'explicit','explicit',''),
                if_attribute(memberdef,'inline','inline','')
                ]).strip()
            ))
        for n in memberdef.childNodes:
            self._translateNode(memberdef,'function',n,target=method)
        return method
    
    def _translate_memberdef_function_templateparamlist(
        self, templateparamlist, target=None, **kwargs ):
        return self._translate_templateparamlist(templateparamlist,target=target,**kwargs)
    
    def _translate_memberdef_function_type( self, resultType, target=None, **kwargs ):
        methodType = target.appendChild(self._createNode('type'))
        self._translateChildren(resultType,target=methodType)
        return methodType
    
    def _translate_memberdef_function_briefdescription( self, description, target=None, **kwargs ):
        self._translateDescription(description,target=target,**kwargs)
        return self._translateDescription(description,target=target,tag='purpose',**kwargs)
    
    def _translate_memberdef_function_detaileddescription( self, description, target=None, **kwargs ):
        return self._translateDescription(description,target=target,**kwargs)
    
    def _translate_memberdef_function_inbodydescription( self, description, target=None, **kwargs ):
        return self._translateDescription(description,target=target,**kwargs)
    
    def _translate_memberdef_function_param( self, param, target=None, **kwargs ):
        return self._translate_param(param,target=target,**kwargs)
    
    def _translate_memberdef_variable( self, memberdef, target=None, scope=None, **kwargs ):
        self._setID(memberdef.getAttribute('id'),
            scope+'::'+self._getChildData('name',root=memberdef))
        data_member = target.appendChild(self._createNode('data-member',
            id=memberdef.getAttribute('id'),
            name=self._getChildData('name',root=memberdef)))
        data_member_type = data_member.appendChild(self._createNode('type'))
        self._translateChildren(self._getChild('type',root=memberdef),target=data_member_type)
    
    def _translate_memberdef_enum( self, memberdef, target=None, scope=None, **kwargs ):
        self._setID(memberdef.getAttribute('id'),
            scope+'::'+self._getChildData('name',root=memberdef))
        enum = target.appendChild(self._createNode('enum',
            id=memberdef.getAttribute('id'),
            name=self._getChildData('name',root=memberdef)))
        for n in memberdef.childNodes:
            self._translateNode(memberdef,'enum',n,target=enum,scope=scope,**kwargs)
        return enum
    
    def _translate_memberdef_enum_enumvalue( self, enumvalue, target=None, scope=None, **kwargs ):
        self._setID(enumvalue.getAttribute('id'),
            scope+'::'+self._getChildData('name',root=enumvalue))
        value = target.appendChild(self._createNode('enumvalue',
            id=enumvalue.getAttribute('id'),
            name=self._getChildData('name',root=enumvalue)))
        initializer = self._getChild('initializer',root=enumvalue)
        if initializer:
            self._translateChildren(initializer,
                target=target.appendChild(self._createNode('default')))
        return value
    
    def _translate_param( self, param, target=None, **kwargs):
        parameter = target.appendChild(self._createNode('parameter',
            name=self._getChildData('declname',root=param)))
        paramtype = parameter.appendChild(self._createNode('paramtype'))
        self._translateChildren(self._getChild('type',root=param),target=paramtype)
        defval = self._getChild('defval',root=param)
        if defval:
            self._translateChildren(self._getChild('defval',root=param),target=parameter)
        return parameter
    
    def _translate_ref( self, ref, **kwargs ):
        return self._translateNode(ref,ref.getAttribute('kindref'))
    
    def _translate_ref_compound( self, ref, **kwargs ):
        result = self._createNode('link',linkend=ref.getAttribute('refid'))
        classname = result.appendChild(self._createNode('classname'))
        self._translateChildren(ref,target=classname)
        return result
    
    def _translate_ref_member( self, ref, **kwargs ):
        result = self._createNode('link',linkend=ref.getAttribute('refid'))
        self._translateChildren(ref,target=result)
        return result
    
    def _getChild( self, tag = None, id = None, name = None, root = None ):
        if not root:
            root = self.boostbook.documentElement
        for n in root.childNodes:
            found = True
            if tag and found:
                found = found and tag == n.nodeName
            if id and found:
                if n.hasAttribute('id'):
                    found = found and n.getAttribute('id') == id
                else:
                    found = found and n.hasAttribute('id') and n.getAttribute('id') == id
            if name and found:
                found = found and n.hasAttribute('name') and n.getAttribute('name') == name
            if found:
                #~ print '--|', n
                return n
        return None
    
    def _getChildData( self, tag, **kwargs ):
        child = self._getChild(tag,**kwargs)
        if child:
            text = self._getChild('#text',root=child)
            if text:
                return text.data.strip()
        return ''
    
    def _cppName( self, type ):
        parts = re.search('^([^<]+)[<]?(.*)[>]?$',type.strip().strip(':'))
        result = {
            'compoundname' : parts.group(1),
            'namespace' : parts.group(1).split('::')[0:-1],
            'name' : parts.group(1).split('::')[-1],
            'specialization' : parts.group(2)
            }
        if result['namespace'] and len(result['namespace']) > 0:
            namespace = '::'.join(result['namespace'])
            while (
                len(result['namespace']) > 0 and (
                    not self.symbols.has_key(namespace) or
                    self.symbols[namespace]['kind'] != 'namespace')
                ):
                result['name'] = result['namespace'].pop()+'::'+result['name']
                namespace = '::'.join(result['namespace'])
        return result
    
    def _createNode( self, tag, **kwargs ):
        result = self.boostbook.createElement(tag)
        for k in kwargs.keys():
            if k == 'id':
                result.setAttribute('id',kwargs[k])
            else:
                result.setAttribute(k,kwargs[k])
        return result
    
    def _createText( self, tag, data, **kwargs ):
        result = self._createNode(tag,**kwargs)
        data = data.strip()
        if len(data) > 0:
            result.appendChild(self.boostbook.createTextNode(data))
        return result


def main( xmldir=None, output=None, id=None, title=None, index=False ):
    #~ print '--- main: xmldir = %s, output = %s' % (xmldir,output)
    
    input = glob.glob( os.path.abspath( os.path.join( xmldir, "*.xml" ) ) )
    input.sort
    translator = Doxygen2BoostBook(id=id, title=title, index=index)
    #~ Feed in the namespaces first to build up the set of namespaces
    #~ and definitions so that lookup is unambiguous when reading in the definitions.
    namespace_files = filter(
        lambda x:
            os.path.basename(x).startswith('namespace_'),
        input)
    decl_files = filter(
        lambda x:
            not os.path.basename(x).startswith('namespace_') and not os.path.basename(x).startswith('_'),
        input)
    for dox in namespace_files:
        #~ print '--|',os.path.basename(dox)
        translator.addDox(xml.dom.minidom.parse(dox))
    for dox in decl_files:
        #~ print '--|',os.path.basename(dox)
        translator.addDox(xml.dom.minidom.parse(dox))
    
    if output:
        output = open(output,'w')
    else:
        output = sys.stdout
    if output:
        output.write(translator.tostring())


main( **get_args() )
