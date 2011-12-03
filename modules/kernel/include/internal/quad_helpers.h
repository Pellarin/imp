/**
 *  \file internal/particle_quad_helpers.h
 *  \brief A container for Quads.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 */

#ifndef IMPKERNEL_INTERNAL_QUAD_HELPERS_H
#define IMPKERNEL_INTERNAL_QUAD_HELPERS_H

#include "../kernel_config.h"
#include "../QuadContainer.h"
#include "../QuadModifier.h"
#include "../QuadScore.h"
#include "container_helpers.h"
#include <algorithm>

IMP_BEGIN_INTERNAL_NAMESPACE
template <class Score>
struct SimpleRestraintParentTraits<Score,
       typename boost::enable_if<
         boost::is_base_of<QuadScore, Score> >::type> {
  typedef IMP::QuadScoreRestraint SimpleRestraint;
  typedef IMP::QuadsScoreRestraint SimplesRestraint;
};
IMP_END_INTERNAL_NAMESPACE


#endif  /* IMPKERNEL_INTERNAL_QUAD_HELPERS_H */
