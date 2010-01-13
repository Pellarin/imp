/**
 *  \file GroupnameContainerSet.h
 *  \brief Store a set of GroupnameContainers
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 Sali Lab. All rights reserved.
 */

#ifndef IMPCORE_GROUPNAME_CONTAINER_SET_H
#define IMPCORE_GROUPNAME_CONTAINER_SET_H

#include "config.h"
#include <IMP/GroupnameContainer.h>
#include <IMP/container_macros.h>

IMPCORE_BEGIN_NAMESPACE

//! Stores a set of GroupnameContainers
/**
 */
class IMPCOREEXPORT GroupnameContainerSet
  : public GroupnameContainer
{
 // to not have added and removed
 GroupnameContainerSet(bool);
public:
  //! Construct and empty set
  GroupnameContainerSet(std::string name="GroupnameContainerSet %1%");

  GroupnameContainerSet(const GroupnameContainers &in,
                        std::string name="GroupnameContainerSet %1%");

  IMP_GROUPNAME_CONTAINER(GroupnameContainerSet, get_module_version_info());
 /** @name Methods to control the nested container

     This container merges a set of nested containers. To add
     or remove nested containers, use the methods below.
  */
  /**@{*/
  IMP_LIST(public, GroupnameContainer, groupname_container,
           GroupnameContainer*, GroupnameContainers);
  /**@}*/

  static GroupnameContainerSet *create_untracked_container() {
    GroupnameContainerSet *lsc = new GroupnameContainerSet(false);
    return lsc;
  }
};


IMPCORE_END_NAMESPACE

#endif  /* IMPCORE_GROUPNAME_CONTAINER_SET_H */
