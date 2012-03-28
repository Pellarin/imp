/**
 *  \file PredicateSingletonsRestraint.h
 *  \brief Apply a SingletonScore to each Singleton in a list.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPCONTAINER_PREDICATE_SINGLETONS_RESTRAINT_H
#define IMPCONTAINER_PREDICATE_SINGLETONS_RESTRAINT_H

#include "container_config.h"

#include "ListSingletonContainer.h"
#include <IMP/compatibility/map.h>
#include <IMP/SingletonPredicate.h>
#include "generic.h"

#include <iostream>

IMPCONTAINER_BEGIN_NAMESPACE

//! Applies a SingletonScore to each Singleton in a list based on a predicate
/** This restraint uses a passed predicate to choose which score to apply
    to each tuple in the input container. The selections are cached, making it
    substantially faster than using a core::TypedPairScore. */
class IMPCONTAINEREXPORT PredicateSingletonsRestraint :
public Restraint
{
  OwnerPointer<SingletonPredicate> predicate_;
  OwnerPointer<SingletonContainer> input_;
  typedef compatibility::map<unsigned int, Pointer<ListSingletonContainer> >
      Map;
  Map containers_;
  Pointer<ListSingletonContainer> unknown_container_;
  Restraints restraints_;
  mutable bool updated_;
  bool error_on_unknown_;
  void update_lists_if_necessary() const;
  void assign_pair(ParticleIndex index) const;
public:
  PredicateSingletonsRestraint(SingletonPredicate *pred,
                      SingletonContainerInput input,
                      std::string name="PredicateSingletonsRestraint %1%");

  /** This version uses the container::create_restraint() function and so
      is more efficient than the non-template version.*/
  template <class Score>
      void set_score(int predicate_value, Score *score) {
    IMP_USAGE_CHECK(get_is_part_of_model(),
                    "You must add this restraint to the model"
                    << " first, sorry, this can be fixed.");
    IMP_NEW(ListSingletonContainer, c, (get_model()));
    restraints_.push_back(container::create_restraint(score, c.get()));
    restraints_.back()->set_model(get_model());
    containers_[predicate_value]=c;
  }
template <class Score>
      void set_unknown_score( Score *score) {
  // make sure it gets cleaned up if it is a temporary
  Pointer<Score> pscore(score);
    IMP_USAGE_CHECK(get_is_part_of_model(),
                    "You must add this restraint to the model"
                    << " first, sorry, this can be fixed.");
    IMP_NEW(ListSingletonContainer, c, (get_model()));
    restraints_.push_back(container::create_restraint(score, c.get()));
    restraints_.back()->set_model(get_model());
    unknown_container_=c;
  }
  /** By default, it is an error if the predicate returns a value that is
      not known. If this is false, then they are silently skipped.
  */
  void set_is_complete(bool tf) {
    error_on_unknown_=tf;
  }

  ParticlesTemp get_particles(int predicate_value) const {
    return containers_.find(predicate_value)->second
      ->get_particles();
  }

  IMP_RESTRAINT(PredicateSingletonsRestraint);
private:
  Restraints do_create_current_decomposition() const;
};

IMPCONTAINER_END_NAMESPACE

#endif  /* IMPCONTAINER_PREDICATE_SINGLETONS_RESTRAINT_H */
