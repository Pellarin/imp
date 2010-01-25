/**
 *  \file MaximumTripletScore.cpp  \brief Define TripletScore
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 Sali Lab. All rights reserved.
 */

#include <IMP/core/MaximumTripletScore.h>
#include "IMP/core/internal/MinimalSet.h"
#include "IMP/internal/utility.h"

IMPCORE_BEGIN_NAMESPACE

namespace {
  unsigned int next_index=0;
}

MaximumTripletScore::MaximumTripletScore(const TripletScoresTemp &scores,
                                           unsigned int n,
                                           std::string name):
  TripletScore(IMP::internal::make_object_name(name, next_index++)),
  scores_(scores),
  n_(n)
{
}


namespace {
  typedef internal::MinimalSet<double,
          TripletScore*, std::greater<double> > MS;
  template <class It>
  MS find_minimal_set(It b, It e, const ParticleTriplet& v, unsigned int n) {
    IMP_LOG(TERSE, "Finding Maximum " << n << " of "
            << std::distance(b,e) << std::endl);
    MS bestn(n);
    for (It it= b; it != e; ++it) {
      double score= (*it)->evaluate(v, NULL);

      if (bestn.can_insert(score)) {
        bestn.insert(score, *it);
      }
    }
    return bestn;
  }
}

double MaximumTripletScore::evaluate(const ParticleTriplet& v,
                                      DerivativeAccumulator *da) const {
  MS bestn= find_minimal_set(scores_.begin(),
                             scores_.end(), v, n_);

  double score=0;
  for (unsigned int i=0; i< bestn.size(); ++i) {
    if (da) {
      bestn[i].second->evaluate(v, da);
    }
    score+= bestn[i].first;
  }
  return score;
}

bool MaximumTripletScore::get_is_changed(const ParticleTriplet& v) const {
  for (unsigned int i=0; i< scores_.size(); ++i) {
    if (scores_[i]->get_is_changed(v)) return true;
  }
  return false;
}


ParticlesList MaximumTripletScore
::get_interacting_particles(const ParticleTriplet& p) const {
  ParticlesList ret;
  for (unsigned int i=0; i< scores_.size(); ++i) {
    ParticlesList c= scores_[i]->get_interacting_particles(p);
    ret.insert(ret.end(), c.begin(), c.end());
  }
  return ret;
}

ParticlesTemp MaximumTripletScore
::get_input_particles(const ParticleTriplet& p) const {
  ParticlesTemp ret;
  for (unsigned int i=0; i< scores_.size(); ++i) {
    ParticlesTemp c= scores_[i]->get_input_particles(p);
    ret.insert(ret.end(), c.begin(), c.end());
  }
  return ret;
}

ContainersTemp MaximumTripletScore
::get_input_containers(const ParticleTriplet& p) const {
  ContainersTemp ret;
  for (unsigned int i=0; i< scores_.size(); ++i) {
    ContainersTemp c= scores_[i]->get_input_containers(p);
    ret.insert(ret.end(), c.begin(), c.end());
  }
  return ret;
}


void MaximumTripletScore::show(std::ostream &out) const
{
  out << "MaximumTripletScore with " << scores_.size() << " scores."
      << std::endl;
}

IMPCORE_END_NAMESPACE
