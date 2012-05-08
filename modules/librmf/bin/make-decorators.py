#!/usr/bin/python
def get_string(type, name, const, per_frame=False):
    if per_frame:
        pfs="true"
    else:
        pfs="false"
    if const:
        return """(fh.get_has_key<%(type)sTraits>
                   (cat, \"%(name)s\")?
                   fh.get_key<%(type)sTraits>(cat, \"%(name)s\")
                              :%(type)sKey())"""%{ "name":name,
                                                                                     "type": type,
                                                                                     "pfs":pfs}
    else:
        return """get_key_always<%(type)sTraits>(fh, cat,
                               \"%(name)s\",
                               %(pfs)s)"""%{ "name":name,
                                                  "type": type,
                                                  "pfs":pfs}


class Children:
    def __init__(self, nice_name):
        self.nice_name=nice_name
    def get_key_members(self, const):
        if (const):
            return ["StaticAliasConstFactory "+self.nice_name+"_;"]
        else:
            return ["StaticAliasFactory "+self.nice_name+"_;"]
    def get_methods(self, const):
        ret=[]
        if const:
            nht="NodeConstHandle"
        else:
            nht="NodeHandle"
        ret.append(nht+"s get_"+self.nice_name+"() const {")
        ret.append("  "+nht+"s typed=nh_.get_children();")
        ret.append("  "+nht+"s ret;")
        ret.append("  for (unsigned int i=0; i< typed.size(); ++i) {")
        ret.append("     if ("+self.nice_name+"_.get_is(typed[i])) {")
        ret.append("        "+self.nice_name+"_.get(typed[i]).get_aliased();")
        ret.append("     }");
        ret.append("  }");
        ret.append("  return ret;")
        ret.append("}")
        if not const:
            ret.append("void set_"+self.nice_name+"(NodeConstHandles v) {")
            ret.append("   for (unsigned int i=0; i< v.size(); ++i) {")
            ret.append("       add_child_alias(nh_, v[i]);")
            ret.append("   }")
            ret.append("}")
            ret.append("void set_"+self.nice_name+"(NodeHandles v) {")
            ret.append("   for (unsigned int i=0; i< v.size(); ++i) {")
            ret.append("       add_child_alias(nh_, v[i]);")
            ret.append("   }")
            ret.append("}")
        return ret
    def get_key_arguments(self, const):
        if (const):
            return ["StaticAliasConstFactory "+self.nice_name+""]
        else:
            return ["StaticAliasFactory "+self.nice_name+""]
    def get_key_pass(self, const):
        return [self.nice_name+"_"]
    def get_key_saves(self, const):
        return [self.nice_name+"_("+self.nice_name+")"]
    def get_initialize(self, const):
        if (const):
            return [self.nice_name+"_(fh)"]
        else:
            return [self.nice_name+"_(fh)"]
    def get_construct(self, const):
        return []
    def get_check(self, const):
        return []


class Attribute:
    def __init__(self, type, nice_name, attribute_name, per_frame=False):
        self.type=type
        self.per_frame=per_frame
        self.nice_name=nice_name
        self.attribute_name=attribute_name
    def get_key_members(self, const):
        return [self.type+"Key "+self.nice_name+"_;"]
    def get_methods(self, const):
        ret=[]
        ret.append(self.type+" get_"+self.nice_name+"() const {")
        ret.append("  return nh_.get_value("+self.nice_name+"_, frame_);")
        ret.append("}")
        if not const:
            ret.append("void set_"+self.nice_name+"("+self.type+" v) {")
            ret.append("   nh_.set_value("+self.nice_name+"_, v, frame_);")
            ret.append("}")
        return ret
    def get_key_arguments(self, const):
        return [self.type+"Key "+self.nice_name]
    def get_key_pass(self, const):
        return [self.nice_name+"_"]
    def get_key_saves(self, const):
        return [self.nice_name+"_("+self.nice_name+")"]
    def get_construct(self, const):
        return [self.nice_name+"_="+get_string(self.type, self.attribute_name, const, self.per_frame)+";"]
    def get_initialize(self, const):
        return []
    def get_check(self, const):
        return ["nh.get_has_value("+self.nice_name+"_, frame)"]


class NodeAttribute(Attribute):
    def __init__(self, *args):
        Attribute.__init__(self, *args)
    def get_methods(self, const):
        ret=[]
        if const:
            nht= "NodeConstHandle"
        else:
            nht= "NodeHandle"
        ret.append(nht+" get_"+self.nice_name+"() const {")
        ret.append("  NodeID ni= nh_.get_value("+self.nice_name+"_, frame_);")
        ret.append("  return nh_.get_file().get_node_from_id(ni);")
        ret.append("}")
        if not const:
            ret.append("void set_"+self.nice_name+"(NodeConstHandle v) {")
            ret.append("   nh_.set_value("+self.nice_name+"_, v.get_id(), frame_);")
            ret.append("}")
        return ret

class SingletonRangeAttribute:
    def __init__(self, type, nice_name, attribute_name_begin, attribute_name_end,
                 per_frame=False):
        self.type=type
        self.nice_name=nice_name
        self.per_frame=per_frame
        self.attribute_name_begin=attribute_name_begin
        self.attribute_name_end=attribute_name_end
    def get_key_members(self, const):
        return ["boost::array<"+self.type+"Key,2> "+self.nice_name+"_;"]
    def get_methods(self, const):
        ret=[]
        ret.append(self.type+" get_"+self.nice_name+"() const {")
        ret.append("  return nh_.get_value("+self.nice_name+"_[0], frame_);")
        ret.append("}")
        if not const:
            ret.append("void set_"+self.nice_name+"("+self.type+" v) {")
            ret.append("   nh_.set_value("+self.nice_name+"_[0], v, frame_);")
            ret.append("   nh_.set_value("+self.nice_name+"_[1], v, frame_);")
            ret.append("}")
        return ret
    def get_key_arguments(self, const):
        return ["boost::array<"+self.type+"Key, 2> "+self.nice_name]
    def get_key_pass(self, const):
        return [self.nice_name+"_"]
    def get_key_saves(self, const):
        return [self.nice_name+"_("+self.nice_name+")"]
    def get_initialize(self, const):
        return []
    def get_construct(self, const):
        return [self.nice_name+"_[0]="+get_string(self.type, self.attribute_name_begin, const, self.per_frame)+\
            ";\n"+self.nice_name+"_[1]="+get_string(self.type, self.attribute_name_end, const, self.per_frame)+";"]
    def get_check(self, const):
        return ["nh.get_has_value("+self.nice_name+"_[0], frame)"+\
            "\n  && nh.get_has_value("+self.nice_name+"_[1], frame)"+\
            "\n  && nh.get_value("+self.nice_name+"_[0], frame)"\
            "\n   ==nh.get_value("+self.nice_name+"_[1], frame)"]


class RangeAttribute:
    def __init__(self, type, nice_name, attribute_name_begin,
                 attribute_name_end, per_frame=False):
        self.type=type
        self.nice_name=nice_name
        self.per_frame=per_frame
        self.attribute_name_begin=attribute_name_begin
        self.attribute_name_end=attribute_name_end
    def get_key_members(self, const):
        return ["boost::array<"+self.type+"Key,2> "+self.nice_name+"_;"]
    def get_methods(self, const):
        ret=[]
        ret.append(self.type+"Range get_"+self.nice_name+"() const {")
        ret.append("  return std::make_pair(nh_.get_value("+self.nice_name+"_[0], frame_),")
        ret.append("                        nh_.get_value("+self.nice_name+"_[1], frame_));")
        ret.append("}")
        if not const:
            ret.append("void set_"+self.nice_name+"("+self.type+" v0, "+self.type+" v1) {")
            ret.append("   nh_.set_value("+self.nice_name+"_[0], v0, frame_);")
            ret.append("   nh_.set_value("+self.nice_name+"_[1], v1, frame_);")
            ret.append("}")

        return ret
    def get_key_arguments(self, const):
        return ["boost::array<"+self.type+"Key, 2> "+self.nice_name]
    def get_key_pass(self, const):
        return [self.nice_name+"_"]
    def get_key_saves(self, const):
        return [self.nice_name+"_("+self.nice_name+")"]
    def get_initialize(self, const):
        return []
    def get_construct(self, const):
        return [self.nice_name+"_[0]="+get_string(self.type, self.attribute_name_begin, const, self.per_frame)+\
            ";\n"+self.nice_name+"_[1]="+get_string(self.type, self.attribute_name_end, const, self.per_frame)+";"]
    def get_check(self, const):
        return ["nh.get_has_value("+self.nice_name+"_[0], frame)"+\
            "\n  && nh.get_has_value("+self.nice_name+"_[1], frame)"+\
            "\n  && nh.get_value("+self.nice_name+"_[0], frame)"\
            "\n   <nh.get_value("+self.nice_name+"_[1], frame)"]


class Attributes:
    def __init__(self, type, ptype, nice_name, attribute_names, per_frame=False):
        self.type=type
        self.nice_name=nice_name
        self.ptype=ptype
        self.attribute_names=attribute_names
        self.per_frame=per_frame
    def get_key_members(self, const):
        return [self.type+"Keys "+self.nice_name+"_;"]
    def get_methods(self, const):
        ret=[]
        ret.append("""%(ptype)s get_%(name)s() const {
       %(ptype)s ret;
       for (unsigned int i=0; i< %(len)s; ++i) {
          ret.push_back(nh_.get_value(%(key)s[i], frame_));
       }
       return ret;
    }"""%{"type":self.type,
          "ptype":self.ptype,
          "name":self.nice_name,
          "len":len(self.attribute_names),
          "key":self.nice_name+"_"})
        if not const:
            ret.append("""void set_%(name)s(const %(ptype)s &v) {
         for (unsigned int i=0; i< %(len)s; ++i) {
            nh_.set_value(%(key)s[i], v[i], frame_);
         }
      }"""%{"type":self.type,
            "ptype":self.ptype,
            "name":self.nice_name,
            "len":len(self.attribute_names),
            "key":self.nice_name+"_"})
        return ret
    def get_key_arguments(self, const):
        return [self.type+"Keys "+self.nice_name]
    def get_key_pass(self, const):
        return [self.nice_name+"_"]
    def get_key_saves(self, const):
        return [self.nice_name+"_("+self.nice_name+")"]
    def get_initialize(self, const):
        return []
    def get_construct(self, const):
        ret=[]
        for n in self.attribute_names:
            ret.append("""      %(name)s_.push_back(%(get)s);"""%{"type":self.type,
                                                            "name":self.nice_name,
                                                            "len":len(self.attribute_names),
                                                            "key":self.nice_name+"_",
                                                            "get":get_string(self.type, n, const, self.per_frame)
                                                            })
        return ret
    def get_check(self, const):
        return ["nh.get_has_value("+self.nice_name+"_[0], frame)"]


class DecoratorCategory:
    def __init__(self, category, arity,
                 attributes, internal_attributes=[]):
        self.category=category
        self.arity=arity
        self.attributes=attributes
        self.internal_attributes=internal_attributes

class Decorator:
    def __init__(self, name, description,
                 decorator_categories,
                 init_function=[],
                 check_function=[]):
        self.name=name
        self.description=description
        self.init_function=init_function
        self.categories= decorator_categories
        self.check_function= check_function
    def _get_key_members(self, const):
        ret=[]
        for cd in self.categories:
            for a in cd.attributes+cd.internal_attributes:
                ret.extend(a.get_key_members(const))
        return "\n".join(ret)
    def _get_methods(self, const):
        ret=[]
        for cd in self.categories:
            for a in cd.attributes:
                ret.extend(a.get_methods(const))
        return "\n".join(ret)
    def _get_key_arguments(self, const):
        ret=[]
        for cd in self.categories:
            for a in cd.attributes+cd.internal_attributes:
                ret.extend(a.get_key_arguments(const))
        return ",\n".join(ret)
    def _get_key_pass(self, const):
        ret=[]
        for cd in self.categories:
            for a in cd.attributes+cd.internal_attributes:
                ret.extend(a.get_key_pass(const))
        return ",\n".join(ret)
    def _get_key_saves(self, const):
        ret=[]
        for cd in self.categories:
            for a in cd.attributes+cd.internal_attributes:
                ret.extend(a.get_key_saves(const))
        return ",\n".join(ret)
    def _get_checks(self, const):
        ret=[]
        for cd in self.categories:
            for a in cd.attributes+cd.internal_attributes:
                ret.extend(a.get_check(const))
        ret.extend(self.check_function)
        return "\n    && ".join(ret)
    def _get_construct(self, const):
        ret=[]
        for cd in self.categories:
            # make handle missing later
            lhs="{\n  CategoryD<"+str(cd.arity)\
                         +"> cat="
            if const:
                rhs="fh.get_category<"+str(cd.arity)+">(\""\
                    +cd.category+"\");"
            else:
                rhs="get_category_always<"+str(cd.arity)+">(fh, \""\
                    +cd.category+"\");"
            ret.append(lhs+rhs)
            for a in cd.attributes+cd.internal_attributes:
                ret.extend(a.get_construct(const))
            ret.append("}")
        return "\n".join(ret)
    def _get_initialize(self, const):
        ret=[]
        for cd in self.categories:
            for a in cd.attributes+cd.internal_attributes:
                ret.extend(a.get_initialize(const))
        if ret==[]:
            return ""
        else:
            return ": " + ", ".join(ret)
    def get(self):
        ret=[]
        classstr="""/** %(description)s

       \see %(name)s%(NOTCONST)s
       \see %(name)s%(CONST)sFactory
     */
    class %(name)s%(CONST)s {
    Node%(CONST)sHandle nh_;
    unsigned int frame_;
    friend class %(name)s%(CONST)sFactory;
    private:
    %(key_members)s
    %(name)s%(CONST)s(Node%(CONST)sHandle nh,
                      unsigned int frame,
                  %(key_arguments)s):
       nh_(nh),
       frame_(frame),
       %(key_saves)s {
    %(init)s;
    }
    public:
    %(methods)s
    IMP_RMF_SHOWABLE(Const%(name)s,
                     "%(name)s%(CONST)s "
                     << nh_.get_name());
    ~%(name)s%(CONST)s() {
    }
    };

    typedef vector<%(name)s%(CONST)s>
            %(name)s%(CONST)ss;
"""
        ret.append(classstr%{"description":self.description,
                             "name":self.name,
                             "key_members": self._get_key_members(True),
                             "methods": self._get_methods(True),
                             "key_arguments": self._get_key_arguments(True),
                             "key_saves": self._get_key_saves(True),
                             "CONST":"Const", "NOTCONST":"",
                             "init":""})
        ret.append(classstr%{"description":self.description,
                             "name":self.name,
                             "key_members": self._get_key_members(False),
                             "methods": self._get_methods(False),
                             "key_arguments": self._get_key_arguments(False),
                             "key_saves": self._get_key_saves(False),
                             "CONST":"", "NOTCONST":"Const",
                             "init":"\n".join(self.init_function)})
        factstr="""/** Create decorators of type %(name)s.

       \see %(name)s%(CONST)s
       \see %(name)s%(NOTCONST)sFactory
    */
    class %(name)s%(CONST)sFactory {
    private:
    %(key_members)s
    public:
    typedef File%(CONST)sHandle File;
    typedef %(name)s%(CONST)s Decorator;
    %(name)s%(CONST)sFactory(File%(CONST)sHandle fh) %(initialize)s{
    %(construct)s;
    }
    %(name)s%(CONST)s get(Node%(CONST)sHandle nh,
                          unsigned int frame=0) const {
      %(create_check)s;
      return %(name)s%(CONST)s(nh, frame, %(key_pass)s);
    }
    bool get_is(Node%(CONST)sHandle nh, unsigned int frame=0) const {
      return %(checks)s;
    }
    IMP_RMF_SHOWABLE(%(name)s%(CONST)sFactory,
                     "%(name)s%(CONST)sFactory");
    };

    typedef vector<%(name)s%(CONST)sFactory>
            %(name)s%(CONST)sFactories;
"""
        ret.append(factstr%{"name":self.name,
                             "key_members": self._get_key_members(False),
                             "key_pass": self._get_key_pass(False),
                             "CONST":"", "NOTCONST":"Const",
                            "create_check":"",
                            "construct": self._get_construct(False),
                            "initialize": self._get_initialize(False),
                            "checks":self._get_checks(False)});
        ret.append(factstr%{"name":self.name,
                             "key_members": self._get_key_members(True),
                             "key_pass": self._get_key_pass(True),
                             "create_check":"IMP_RMF_USAGE_CHECK(get_is(nh, frame), \"Node is not\")",
                             "CONST":"Const", "NOTCONST":"",
                            "construct": self._get_construct(True),
                            "initialize": self._get_initialize(True),
                            "checks":self._get_checks(True)});
        return "\n".join(ret)


colored= Decorator("Colored", "These particles has associated color information.",
                   [DecoratorCategory("shape", 1, [Attributes("Float", "Floats",
                                                              "rgb_color", ["rgb color red",
                                                                            "rgb color green",
                                                                            "rgb color blue"])])],
                   "")
particle= Decorator("Particle", "These particles has associated coordinates and radius information.",
                   [DecoratorCategory("physics", 1, [Attributes("Float", "Floats",
                                                                "coordinates", ["cartesian x",
                                                                                "cartesian y",
                                                                                "cartesian z"], True),
                                                     Attribute("Float", "radius", "radius"),
                                                     Attribute("Float", "mass", "mass")])],
                   "")
iparticle= Decorator("IntermediateParticle", "These particles has associated coordinates and radius information.",
                   [DecoratorCategory("physics", 1, [Attributes("Float", "Floats",
                                                                "coordinates", ["cartesian x",
                                                                                "cartesian y",
                                                                                "cartesian z"], True),
                                                     Attribute("Float", "radius", "radius")])],
                   "")
pparticle= Decorator("RigidParticle", "These particles has associated coordinates and orientation information.",
                   [DecoratorCategory("physics", 1, [Attributes("Float", "Floats",
                                                                "orientation", ["orientation r",
                                                                                "orientation i",
                                                                                "orientation j",
                                                                                "orientation k"], True),
                                                     Attributes("Float", "Floats",
                                                                "coordinates", ["cartesian x",
                                                                                "cartesian y",
                                                                                "cartesian z"], True)])],
                   "")

score= Decorator("Score", "Associate a score with some set of particles.",
                   [DecoratorCategory("feature", 1, [Children("representation"),
                                                     Attribute("Float", "score", "score", True)])],
                   "")

ball= Decorator("Ball", "A geometric ball.",
                   [DecoratorCategory("shape", 1, [Attributes("Float", "Floats",
                                                              "coordinates", ["cartesian x",
                                                                              "cartesian y",
                                                                              "cartesian z"], True),
                                                   Attribute("Float", "radius", "radius")],
                                      internal_attributes=[Attribute("Index", "type", "type")])],
                   ["nh.set_value(type_, 0);"], ["nh.get_value(type_)==0"])
cylinder= Decorator("Cylinder", "A geometric cylinder.",
                   [DecoratorCategory("shape", 1, [Attributes("Floats", "FloatsList",
                                                              "coordinates", ["cartesian xs",
                                                                              "cartesian ys",
                                                                              "cartesian zs"], True),
                                                   Attribute("Float", "radius", "radius")],
                                      internal_attributes=[Attribute("Index", "type", "type")])],
                   ["nh.set_value(type_, 1);"], ["nh.get_value(type_)==1"])

segment= Decorator("Segment", "A geometric line setgment.",
                   [DecoratorCategory("shape", 1, [Attributes("Floats", "FloatsList",
                                                              "coordinates", ["cartesian xs",
                                                                              "cartesian ys",
                                                                              "cartesian zs"], True)],
                                      internal_attributes=[Attribute("Index", "type", "type")])],
                    ["nh.set_value(type_, 1);"], ["nh.get_value(type_)==1"])

journal= Decorator("JournalArticle", "Information regarding a publication.",
                   [DecoratorCategory("publication", 1, [Attribute("String", "title", "title"),
                                                         Attribute("String", "journal", "journal"),
                                                         Attribute("String", "pubmed_id", "pubmed id"),
                                                         Attribute("Int", "year", "year"),
                                                         Attribute("Strings", "authors", "authors"),])],
                   "")

atom= Decorator("Atom", "Information regarding an atom.",
                   [DecoratorCategory("physics", 1, [Attributes("Float", "Floats",
                                                                "coordinates", ["cartesian x",
                                                                                "cartesian y",
                                                                                "cartesian z"], True),
                                                     Attribute("Float", "radius", "radius"),
                                                     Attribute("Float", "mass", "mass"),
                                                     Attribute("Index", "element", "element")])],
                   "")


residue= Decorator("Residue", "Information regarding a residue.",
                   [DecoratorCategory("sequence", 1, [SingletonRangeAttribute("Int", "index", "first residue index", "last residue index"),
                                                         Attribute("String", "type", "residue type")])],
                   "")

chain= Decorator("Chain", "Information regarding a chain.",
                   [DecoratorCategory("sequence", 1, [Attribute("Index", "chain_id", "chain id")])],
                   "")

fragment= Decorator("Domain", "Information regarding a fragment of a molecule.",
                   [DecoratorCategory("sequence", 1, [RangeAttribute("Int", "indexes", "first residue index", "last residue index")])],
                   "")

copy= Decorator("Copy", "Information regarding a copy of a molecule.",
                   [DecoratorCategory("sequence", 1, [Attribute("Index", "copy_index", "copy index")])],
                   "")
diffuser= Decorator("Diffuser", "Information regarding diffusion coefficients.",
                   [DecoratorCategory("physics", 1, [Attribute("Float", "diffusion_coefficient", "diffusion coefficient")])],
                   "")
typed= Decorator("Typed", "A numeric tag for keeping track of types of molecules.",
                   [DecoratorCategory("sequence", 1, [Attribute("String", "type_name", "type name")])],
                   "")

salias= Decorator("StaticAlias", "Store a static reference to another node.",
                   [DecoratorCategory("alias", 1, [NodeAttribute("NodeID", "aliased", "aliased")])],
                   "")
dalias= Decorator("DynamicAlias", "Store a dynamic reference to another node.",
                   [DecoratorCategory("alias", 1, [Attribute("NodeID", "alias", "dynamic alias", True)])],
                   "")


print """/**
 *  \\file RMF/decorators.h
 *  \\brief Helper functions for manipulating RMF files.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPLIBRMF_DECORATORS_H
#define IMPLIBRMF_DECORATORS_H

#include "RMF_config.h"
#include "infrastructure_macros.h"
#include "NodeHandle.h"
#include "FileHandle.h"
#include "internal/utility.h"
namespace RMF {
"""
print colored.get()
print particle.get()
print iparticle.get()
print pparticle.get()
print ball.get()
print cylinder.get()
print segment.get()
print journal.get()
print residue.get()
print atom.get()
print chain.get()
print fragment.get()
print copy.get()
print diffuser.get()
print typed.get()
print salias.get()
print dalias.get()
print score.get()
print """} /* namespace RMF */

#endif /* IMPLIBRMF_DECORATORS_H */"""
