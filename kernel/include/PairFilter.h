/**
 *  \file PairFilter.h    \brief A filter for Pairs.
 *
 *  This file is generated by a script (core/tools/make-containers).
 *  Do not edit directly.
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 */

#ifndef IMP_PAIR_FILTER_H
#define IMP_PAIR_FILTER_H

#include "kernel_config.h"
#include "base_types.h"
#include "macros.h"
#include "VersionInfo.h"
#include "ParticleTuple.h"
#include "internal/container_helpers.h"

IMP_BEGIN_NAMESPACE


//! A shared filter for Pairs
/** Stores a searchable shared collection of Pairs.
    \ingroup restraints

    Implementors should see IMP_PAIR_FILTER().
 */
class IMPEXPORT PairFilter : public Object
{
public:
  PairFilter(std::string name="PairFilter %1%");

  /** \note This function may be linear. Be aware of the complexity
      bounds of your particular filter.
   */
  virtual bool get_contains(const ParticlePair& p) const =0;

  /** \note This function may be linear. Be aware of the complexity
      bounds of your particular filter.
   */
  virtual bool get_contains(Model *m,
                            const ParticleIndexPair& p) const {
    return get_contains(internal::get_particle(m,p));
  }

  /** \name Interactions
      Return the set of particles used when applied to the passed
      list.
      @{
  */
  virtual ParticlesTemp get_input_particles(Particle* p) const=0;
  virtual ContainersTemp get_input_containers(Particle* p) const=0;
  /** @} */

#if !defined(IMP_DOXYGEN) && !defined(SWIG)
  virtual void filter_in_place(ParticlePairsTemp& ps) const;
#endif

  IMP_REF_COUNTED_DESTRUCTOR(PairFilter);
};

IMP_OUTPUT_OPERATOR(PairFilter);

IMP_OBJECTS(PairFilter,PairFilters);

IMP_END_NAMESPACE

#endif  /* IMP_PAIR_FILTER_H */
