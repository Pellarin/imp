/**
 *  \file SingletonScoreState.cpp
 *  \brief Use a SingletonModifier applied to a SingletonContainer to
 *  maintain an invariant
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-8 Sali Lab. All rights reserved.
 *
 */

#include "IMP/core/SingletonScoreState.h"

IMPCORE_BEGIN_NAMESPACE

SingletonScoreState::SingletonScoreState(SingletonModifier *before,
                                         SingletonModifier *after,
                                         Particle* v):
  v_(v){
    if (before) f_=before;
    if (after) af_=after;
}

SingletonScoreState::~SingletonScoreState(){}


void SingletonScoreState::do_before_evaluate()
{
  if (!f_) return;
  IMP_LOG(TERSE, "Begin SingletonsScoreState::update" << std::endl);
  IMP_CHECK_OBJECT(f_);
  IMP::internal::ContainerTraits<Particle>
    ::apply(f_.get(), v_);
  IMP_LOG(TERSE, "End SingletonsScoreState::update" << std::endl);
}

void SingletonScoreState::do_after_evaluate(DerivativeAccumulator *)
{
  if (!af_) return;
  IMP_LOG(TERSE, "Begin SingletonsScoreState::after_evaluate" << std::endl);
  IMP_CHECK_OBJECT(af_);
  IMP::internal::ContainerTraits<Particle>
    ::apply(af_.get(), v_);
  IMP_LOG(TERSE, "End SingletonsScoreState::after_evaluate" << std::endl);
}

void SingletonScoreState::show(std::ostream &out) const {
  out << "SingletonScoreState" << std::endl;
}

IMPCORE_END_NAMESPACE
