/**
 *  \file FilteredListPairContainer.h
 *  \brief Store a list of ParticlePairs filtered based on another list
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-9 Sali Lab. All rights reserved.
 */

#ifndef IMPCORE_FILTERED_LIST_PAIR_CONTAINER_H
#define IMPCORE_FILTERED_LIST_PAIR_CONTAINER_H

#include "config.h"
#include "internal/version_info.h"
#include <IMP/PairContainer.h>
#include <IMP/container_macros.h>

IMP_BEGIN_NAMESPACE

typedef PairContainers PairFilters;

IMP_END_NAMESPACE

IMPCORE_BEGIN_NAMESPACE

//! Store a list of ParticlePairs filtered by other lists
/** This class stores a list of ParticlePairs and a list of
    PairContainers with the invariant that none of the
    PairContainers contain any of the ParticlePairs stored.

    \note Currently the filter is only applied upon addition
    of a ParticlePair to the container. So adding more sets to the
    filter afterwards won't remove objects. Nor will changing
    the filtering sets.

    \note The indexes can change when particles are inserted
    as the list is maintained in sorted order.

    \see ListPairContainer
 */
class IMPCOREEXPORT FilteredListPairContainer
  : public PairContainer
{
  bool sorted_;
  mutable std::vector<ParticlePair> data_;
public:
  //! cannot pass a Pairs on construction
  FilteredListPairContainer();

  IMP_PAIR_CONTAINER(FilteredListPairContainer,
                          internal::version_info);

  //! Add vt if none of the referenced containers already contains it
  void add_particle_pair(ParticlePair vt);

  void clear_particle_pairs() {
    data_.clear();
    sorted_=true;
  }
  void reserve_particle_pairs(unsigned int sz) {
    data_.reserve(sz);
  }

  /** @name Faster editing

      The container keeps it list of elements in a sorted order.
      As this can make for slow insertions, the user has the option
      of disabling the sorting while inserting many objects. To do this,
      call
      \code
      set_is_editing(true);
      // do stuff
      set_is_editing(false);
      \endcode
      \see FilteredListPairContainerEditor

      @{
   */
  void set_is_editing( bool tf);

  bool get_is_editing() const {
    return !sorted_;
  }
  /** @}*/

 /** @name Methods to control the set of filters

     PairContainer objects can be used as filters to prevent
     the addition of ParticlePair objects to the container. Every
     time someone requests to add a ParticlePair object, the container
     checks each PairContainer in the PairFilter list to
     see of any of the filters contain an identical object using the
     PairContainer::get_contains_particle_pair() call. If true is
     returned, the object is not added to this container.
  */
  /**@{*/
  IMP_LIST(public, PairFilter, pair_filter,
           PairContainer*, PairContainers);
   /**@}*/
};

IMPCORE_END_NAMESPACE

#endif  /* IMPCORE_FILTERED_LIST_PAIR_CONTAINER_H */
