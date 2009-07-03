/**
 *  \file GroupnamesRestraint.h
 *  \brief Apply a GroupnameScore to each Classname in a list.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-9 Sali Lab. All rights reserved.
 *
 */

#ifndef IMPCORE_GROUPNAMES_RESTRAINT_H
#define IMPCORE_GROUPNAMES_RESTRAINT_H

#include "config.h"
#include "internal/version_info.h"

#include <IMP/Restraint.h>
#include <IMP/Pointer.h>
#include <IMP/GroupnameScore.h>
#include <IMP/GroupnameContainer.h>

#include <iostream>

IMPCORE_BEGIN_NAMESPACE

//! Applies a GroupnameScore to each Classname in a list.
/** This restraint stores the used particles in a Classnames.
    The container used can be set so that the list can be shared
    with other containers (or a nonbonded list can be used). By default
    a ListGroupnameContainer is used and the
    {add_, set_, clear_}classname{s} methods can be used.

    \see GroupnameRestraint
 */
class IMPCOREEXPORT GroupnamesRestraint : public Restraint
{
  Pointer<GroupnameScore> ss_;
  Pointer<GroupnameContainer> pc_;
public:
  //! Create the restraint.
  /** \param[in] ss The function to apply to each particle.
      \param[in] ps The list of particles to use in the restraint.
   */
  GroupnamesRestraint(GroupnameScore *ss,
                      const Classnames &ps=Classnames());

 //! Create the restraint with a shared container
  /** \param[in] ss The function to apply to each particle.
      \param[in] pc The container containing the stored particles. This
      container is not copied.
   */
  GroupnamesRestraint(GroupnameScore *ss,
                      GroupnameContainer *pc);

  IMP_RESTRAINT(GroupnamesRestraint, internal::version_info);

  /** \note This can only be used if the container is a ListGroupnameContainer
  */
  void add_classnames(const Classnames &ps);
  /** \note This can only be used if the container is a ListGroupnameContainer
   */
  void set_classnames(const Classnames &ps);
  /** \note This can only be used if the container is a ListGroupnameContainer
  */
  void add_classname(Value v);
  /** \note This can only be used if the container is a ListGroupnameContainer
  */
  void clear_classnames();

  //! Get the container used to store Particles
  GroupnameContainer* get_groupname_container() {
    return pc_;
  }
  //! Set the container used to store Particles
  /** \note The passed container is used, not a copy.
   */
  void set_groupname_container(GroupnameContainer *pc) {
    pc_=pc;
  }

  virtual ParticlesList get_interacting_particles() const;

  GroupnameScore* get_groupname_score() const {
    return ss_;
  }

  void set_groupname_score(GroupnameScore* ss) {
    IMP_check(ss, "The GroupnameScore must be non-null",
              IndexException);
    ss_= ss;
  }
};

IMPCORE_END_NAMESPACE

#endif  /* IMPCORE_GROUPNAMES_RESTRAINT_H */
