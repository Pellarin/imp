/**
 *  \file CloseBipartitePairContainer.h
 *  \brief Return all pairs from a SingletonContainer
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2013 IMP Inventors. Close rights reserved.
 */

#ifndef IMPCORE_INTERNAL_CORE_CLOSE_BIPARTITE_PAIR_CONTAINER_H
#define IMPCORE_INTERNAL_CORE_CLOSE_BIPARTITE_PAIR_CONTAINER_H

#include <IMP/core/core_config.h>
#include "../ClosePairsFinder.h"
#include "MovedSingletonContainer.h"
#include <IMP/PairContainer.h>
#include <IMP/PairPredicate.h>
#include <IMP/SingletonContainer.h>
#include <IMP/internal/ListLikePairContainer.h>

IMPCORE_BEGIN_INTERNAL_NAMESPACE

/** \brief Return all close unordered pairs of particles taken from
    the SingletonContainer

    See ClosePairContainer for a more detailed description. This
    container lists all close pairs of particles where one particle is
    taken from each of the input sets.

    \note Any passed ClosePairsFinder is ignored.

    \usesconstraint
 */
class IMPCOREEXPORT CoreCloseBipartitePairContainer
    : public IMP::internal::ListLikePairContainer {
  typedef IMP::internal::ListLikePairContainer P;
  IMP::base::OwnerPointer<SingletonContainer> sc_[2];
  bool were_close_, reset_;
  ObjectKey key_;
  // moved stuff
  ParticleIndexes rbs_[2];
  ParticleIndexes xyzrs_[2];
  IMP::base::map<ParticleIndex, ParticleIndexes> constituents_;
  double slack_, distance_;
  algebra::Transformation3Ds rbs_backup_[2];
  algebra::Vector3Ds xyzrs_backup_[2];
  ParticleIndex covers_[2];
  void initialize(SingletonContainer *a, SingletonContainer *b,
                  ParticleIndex cover_a, ParticleIndex cover_b, double distance,
                  double slack, ObjectKey key);

 public:
  //! Get the individual particles from the passed SingletonContainer
  CoreCloseBipartitePairContainer(SingletonContainer *a, SingletonContainer *b,
                                  double distance, double slack = 1);

  //! make sure you know what you are doing
  CoreCloseBipartitePairContainer(SingletonContainer *a, SingletonContainer *b,
                                  ParticleIndex cover_a, ParticleIndex cover_b,
                                  ObjectKey key, double distance,
                                  double slack = 1);

  /** @name Methods to control the set of filters

     PairContainer objects can be used as filters to prevent
     the addition of pairs to the containeroutput list. Pairs
     which are contained in any container added to this list
     will be excluded from the close pairs list.
  */
  /**@{*/
  IMP_LIST_ACTION(public, PairFilter, PairFilters, pair_filter, pair_filters,
                  PairPredicate *, PairPredicates, obj->set_was_used(true);
                  , , );
  /**@}*/
  void clear_caches() { reset_ = true; }
  IMP_LISTLIKE_PAIR_CONTAINER_2(CoreCloseBipartitePairContainer);
};

IMP_OBJECTS(CoreCloseBipartitePairContainer, CoreCloseBipartitePairContainers);

IMPCORE_END_INTERNAL_NAMESPACE

#endif /* IMPCORE_INTERNAL_CORE_CLOSE_BIPARTITE_PAIR_CONTAINER_H */
