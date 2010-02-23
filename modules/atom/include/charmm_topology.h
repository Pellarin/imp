/**
 * \file charmm_topology.h \brief Classes for handling CHARMM-style topology.
 *
 * Copyright 2007-2010 Sali Lab. All rights reserved.
 *
 */

#ifndef IMPATOM_CHARMM_TOPOLOGY_H
#define IMPATOM_CHARMM_TOPOLOGY_H

#include "config.h"

#include <string>
#include <vector>

IMPATOM_BEGIN_NAMESPACE

//! A single atom in a CHARMM topology.
/** Each atom has a name (unique to the residue), a CHARMM type (used to
    look up parameters such as radii and bond lengths in the parameter file)
    and an electrostatic charge.
 */
class CHARMMAtom {
  std::string name_;
  std::string charmm_type_;
  double charge_;
public:
  CHARMMAtom(std::string name) : name_(name) {};

  std::string get_name() const { return name_; }
  std::string get_charmm_type() const { return charmm_type_; }
  double get_charge() const { return charge_; }
  void set_charmm_type(std::string charmm_type) { charmm_type_ = charmm_type; }
  void set_charge(double charge) { charge_ = charge; }
};

//! The end of a bond, angle, dihedral, or improper.
/** An endpoint is an atom in a residue; bonds have two endpoints,
    angles three, and dihedrals and impropers, four.

    If residue_ is not NULL, the endpoint is the named atom in the
    pointed-to residue. (This is used for bonds that can span multiple
    residues, perhaps in different chains, and is typically only created
    by applying a two-residue patch such as DISU or LINK).

    If residue_ is NULL, the endpoint is the named atom in the "current"
    residue, unless the atom name has a +, -, 1: or 2: prefix.
    Atoms prefixed with + are found in the next residue in the chain.
    Atoms prefixed with - are found in the previous residue in the chain.
    1: or 2: prefixes are used by two-residue patches to refer to atoms in
    each residue patched by the patch.
 */
class CHARMMBondEndpoint {
  std::string atom_name_;
//CHARMMResidueTopology *residue_;
public:
  std::string get_atom_name() const { return atom_name_; }
  void set_atom_name(std::string atom_name) { atom_name_ = atom_name; }
};

//! A bond, angle, dihedral or improper between some number of endpoints.
template <unsigned int D>
class CHARMMBond
{
  std::vector<CHARMMBondEndpoint> endpoints_;
};

//! Base class for all CHARMM residue-based topology
class IMPATOMEXPORT CHARMMResidueTopologyBase {
protected:
  std::vector<CHARMMAtom> atoms_;
  std::vector<CHARMMBond<2> > bonds_;
  std::vector<CHARMMBond<3> > angles_;
  std::vector<CHARMMBond<4> > dihedrals_;
  std::vector<CHARMMBond<4> > impropers_;
public:
  void add_atom(const CHARMMAtom &atom);
  CHARMMAtom &get_atom(std::string name);
};

//! The ideal topology of a single residue as read from a CHARMM topology file
class IMPATOMEXPORT CHARMMIdealResidueTopology
    : public CHARMMResidueTopologyBase {
  std::string default_first_patch_, default_last_patch_;
public:
  //! Delete the named atom
  /** \todo also remove any bonds/angles referencing this atom
   */
  void delete_atom(std::string name);

  void set_default_first_patch(std::string patch) {
    default_first_patch_ = patch;
  }
  void set_default_last_patch(std::string patch) {
    default_last_patch_ = patch;
  }
  std::string get_default_first_patch() const { return default_first_patch_; }
  std::string get_default_last_patch() const { return default_last_patch_; }
};

IMPATOM_END_NAMESPACE

#endif  /* IMPATOM_CHARMM_TOPOLOGY_H */
