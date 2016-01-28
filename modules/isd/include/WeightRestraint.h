/**
 *  \file IMP/isd/WeightRestraint.h
 *  \brief Put description here
 *
 *  Copyright 2007-2016 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPISD_WEIGHT_RESTRAINT_H
#define IMPISD_WEIGHT_RESTRAINT_H
#include <IMP/isd/Scale.h>
#include "isd_config.h"
#include <IMP/Restraint.h>

IMPISD_BEGIN_NAMESPACE
/** A restraint for in-vivo ensemble FRET data
 */

class IMPISDEXPORT WeightRestraint : public IMP::Restraint {
  Particle *w_;
  Float wmin_;
  Float wmax_;
  Float kappa_;

 public:
  //! Create the restraint.
  /** Restraints should store the particles they are to act on,
      preferably in a Singleton or PairContainer as appropriate.
   */

  WeightRestraint(Particle *w, Float wmin, Float wmax, Float kappa);

  /** This macro declares the basic needed methods: evaluate and show
   */
  virtual double unprotected_evaluate(IMP::DerivativeAccumulator *accum)
      const IMP_OVERRIDE;
  virtual IMP::ModelObjectsTemp do_get_inputs() const IMP_OVERRIDE;
  IMP_OBJECT_METHODS(WeightRestraint);
};

IMPISD_END_NAMESPACE

#endif /* IMPISD_WEIGHT_RESTRAINT_H */
