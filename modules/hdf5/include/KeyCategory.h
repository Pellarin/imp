/**
 *  \file IMP/hdf5/KeyCategory.h
 *  \brief Handle read/write of Model data from/to files.
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPHDF5_KEY_CATEGORY_H
#define IMPHDF5_KEY_CATEGORY_H

#include "hdf5_config.h"
#include <IMP/macros.h>
#include <IMP/exception.h>
#include <vector>

IMPHDF5_BEGIN_NAMESPACE
/** The category for a key. */
class IMPHDF5EXPORT KeyCategory {
  int i_;
  KeyCategory(unsigned int i): i_(i){}
public:
  KeyCategory(): i_(-1){}
  unsigned int get_index() const {
    IMP_USAGE_CHECK(i_ >=0, "Invalid KeyCategory used");
    return i_;
  }
  static KeyCategory add_key_category(std::string name);
  std::string get_name() const;
  unsigned int get_offset() {
    // int, string, float
    return 3*i_;
  }
  IMP_HASHABLE_INLINE(KeyCategory, return i_);
  IMP_COMPARISONS_1(KeyCategory, i_);
  void show(std::ostream &out) const {
    out << get_name();
  }
};

IMP_VALUES(KeyCategory, KeyCategories);

/** Standard physical keys are
    - "cartesian_x", "cartesian_y", "cartesian_z" for
    cartesian coordinates in angstroms
    - "radius" for radius in angstroms
    - "mass" for mass in daltons
 */
IMPHDF5EXPORT extern const KeyCategory Physics;

/** Standard sequence keys are:
    - "begin residue index", "beyond residue index" for one
    or more residue indexes eg [begin_residue_index, beyond_residue_index)
    - "element" for the element by atomic number
    - "seconard structure" 1 for helix, 2 for strand
    - "sequence" store the sequence as a series of space-deliminted 3-letter
    codes
    - "residue type" store the three letter code for the residue type
*/
IMPHDF5EXPORT extern const KeyCategory Sequence;


/** Float keys are
    - cartesian_x,cartesian_y,cartesian_z,radius
    - rgb_color_red, rgb_color_blue, rgb_color_green

    Int keys are
    - shape which is
        - 1 for sphere described by cartesian_x0,cartesian_y0,cartesian_z0,
        radius
        - 2 for cylinder, described by cartesian_x0,cartesian_y0,cartesian_z0,
        cartesian_x1,cartesian_y1,cartesian_z1, r
 */
IMPHDF5EXPORT extern const KeyCategory Shape;


/** Float keys are
    - score
    Index keys are:
    - representation{i} for the representation nodes involved
*/
IMPHDF5EXPORT extern const KeyCategory Feature;
IMPHDF5_END_NAMESPACE

#endif /* IMPHDF5_KEY_CATEGORY_H */
