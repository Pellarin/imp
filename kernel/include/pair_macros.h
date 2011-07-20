/**
 *  \file pair_macros.h    \brief Macros for various classes.
 *
 *  This file is generated by a script (core/tools/make-containers).
 *  Do not edit directly.
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 */

#ifndef IMP_PAIR_MACROS_H
#define IMP_PAIR_MACROS_H

#ifndef IMP_DOXYGEN

#define IMP_PAIR_SCORE_BASE(Name)                                 \
  double evaluate(const ParticlePair& a,                                  \
                  DerivativeAccumulator *da) const;                     \
  double evaluate(const ParticlePairsTemp &ps,                         \
                  DerivativeAccumulator *da) const {                    \
    double ret=0;                                                       \
    for (unsigned int i=0; i< ps.size(); ++i) {                         \
      ret+=Name::evaluate(ps[i], da);                                   \
    }                                                                   \
    return ret;                                                         \
  }                                                                     \
  double evaluate_if_good(const ParticlePairsTemp &ps,                 \
                          DerivativeAccumulator *da,                    \
                          double max) const {                           \
    double ret=0;                                                       \
    for (unsigned int i=0; i< ps.size(); ++i) {                         \
      double cur=Name::evaluate(ps[i], da);                             \
      max-=cur;                                                         \
      ret+=cur;                                                         \
      if (max <0) break;                                                \
    }                                                                   \
    return ret;                                                         \
  }                                                                     \
  double evaluate_if_good(const ParticlePair& ps,                         \
                          DerivativeAccumulator *da,                    \
                          double ) const {                              \
    return Name::evaluate(ps, da);                                      \
  }                                                                     \
  IMP_OBJECT(Name)

#else
#define IMP_PAIR_SCORE_BASE(Name)
#endif


//! Declare the functions needed for a PairScore
/** In addition to the methods done by IMP_INTERACTON, it declares
    - IMP::PairScore::evaluate(IMP::Particle*,
    IMP::DerivativeAccumulator*)
    - IMP::PairScore::get_input_particles()
    - IMP::PairScore::get_output_particles()

    See IMP_SIMPLE_PAIR_SCORE() for a way of providing an
    implementation of that method.
*/
#define IMP_PAIR_SCORE(Name)                              \
  ParticlesTemp get_input_particles(Particle*) const;           \
  ContainersTemp get_input_containers(Particle *) const;        \
  IMP_PAIR_SCORE_BASE(Name)

//! Declare the functions needed for a PairScore
/** In addition to the methods declared and defined by IMP_PAIR_SCORE,
    the macro provides an implementation of
    - IMP::PairScore::get_input_particles()
    - IMP::PairScore::get_input_containers()
    which assume that only the passed particle serves as input to the
    score.
*/
#define IMP_SIMPLE_PAIR_SCORE(Name)                       \
  ParticlesTemp get_input_particles(Particle*p) const {         \
    return ParticlesTemp(1,p);                                  \
  }                                                             \
  ContainersTemp get_input_containers(Particle *) const {       \
    return ContainersTemp();                                    \
  }                                                             \
  IMP_PAIR_SCORE_BASE(Name)



//! Declare the functions needed for a complex PairScore
/** In addition to the methods done by IMP_OBJECT(), it declares
    - IMP::PairScore::evaluate()
    - IMP::PairScore::get_input_particles()
    - IMP::PairScore::get_output_particles()
    - IMP::PairScore::evaluate_if_good
*/
#define IMP_COMPOSITE_PAIR_SCORE(Name)                            \
  ParticlesTemp get_input_particles(Particle *p) const;                 \
  ContainersTemp get_input_containers(Particle *p) const;               \
  double evaluate(const ParticlePair& p,                                  \
                  DerivativeAccumulator *da) const;                     \
  double evaluate(const ParticlePairsTemp& ps,                         \
                  DerivativeAccumulator *da) const {                    \
    double ret=0;                                                       \
    for (unsigned int i=0; i< ps.size(); ++i) {                         \
      ret+=Name::evaluate(ps[i], da);                                   \
    }                                                                   \
    return ret;                                                         \
  }                                                                     \
  double evaluate_if_good(const ParticlePairsTemp &ps,                 \
                          DerivativeAccumulator *da,                    \
                          double max) const {                           \
    double ret=0;                                                       \
    for (unsigned int i=0; i< ps.size(); ++i) {                         \
      double cur=Name::evaluate_if_good(ps[i], da, max);                \
      max-=cur;                                                         \
      ret+=cur;                                                         \
      if (max <0) break;                                                \
    }                                                                   \
    return ret;                                                         \
  }                                                                     \
  double evaluate_if_good( const ParticlePair& ps,                        \
                          DerivativeAccumulator *da,                    \
                          double ) const;                               \
  IMP_OBJECT(Name)






//! Declare the functions needed for a PairModifier
/** In addition to the methods done by IMP_OBJECT, it declares
    - IMP::PairModifier::apply(IMP::Particle*)
    - IMP::PairModifier::get_input_particles()
    - IMP::PairModifier::get_output_particles()
    \see IMP_PAIR_MODIFIER_DA()
*/
#define IMP_PAIR_MODIFIER(Name)                                   \
  void apply(const ParticlePair& a) const;                                \
  void apply(const ParticlePair& a, DerivativeAccumulator&) const{        \
    apply(a);                                                           \
  }                                                                     \
  void apply(const ParticlePairsTemp &ps) const {                      \
    for (unsigned int i=0; i< ps.size(); ++i) {                         \
      Name::apply(ps[i]);                                               \
    }                                                                   \
  }                                                                     \
  void apply(const ParticlePairsTemp &ps, DerivativeAccumulator &) const { \
    for (unsigned int i=0; i< ps.size(); ++i) {                         \
      Name::apply(ps[i]);                                               \
    }                                                                   \
  }                                                                     \
  ParticlesTemp get_input_particles(Particle*) const;                   \
  ParticlesTemp get_output_particles(Particle*) const;                  \
  ContainersTemp get_input_containers(Particle*) const;                 \
  ContainersTemp get_output_containers(Particle*) const;                \
  IMP_OBJECT(Name)



//! Declare the functions needed for a PairModifier
/** In addition to the methods done by IMP_OBJECT, it declares
    - IMP::PairModifier::apply(IMP::Particle*,
    IMP::DerivativeAccumulator&)
    - IMP::PairModifier::get_input_particles()
    - IMP::PairModifier::get_output_particles()

    \see IMP_PAIR_MODIFIER
*/
#define IMP_PAIR_MODIFIER_DA(Name)                                \
  void apply(const ParticlePair& a, DerivativeAccumulator &da) const;     \
  void apply(const ParticlePair&) const{                                  \
    IMP_LOG(VERBOSE, "This modifier requires a derivative accumulator " \
            << *this << std::endl);                                     \
  }                                                                     \
  void apply(const ParticlePairsTemp &) const {                        \
    IMP_LOG(VERBOSE, "This modifier requires a derivative accumulator " \
            << *this << std::endl);                                     \
  }                                                                     \
  void apply(const ParticlePairsTemp &ps,                              \
             DerivativeAccumulator &da) const {                         \
    for (unsigned int i=0; i< ps.size(); ++i) {                         \
      Name::apply(ps[i], da);                                           \
    }                                                                   \
  }                                                                     \
  ParticlesTemp get_input_particles(Particle*) const;                   \
  ParticlesTemp get_output_particles(Particle*) const;                  \
  ContainersTemp get_input_containers(Particle*) const;                 \
  ContainersTemp get_output_containers(Particle*) const;                \
  IMP_OBJECT(Name)





#ifndef IMP_DOXYGEN
#define IMP_IMPLEMENT_PAIR_CONTAINER(Name)                        \
  void apply(const PairModifier *sm) {                             \
    template_apply(sm);                                                 \
  }                                                                     \
  void apply(const PairModifier *sm,                               \
             DerivativeAccumulator &da) {                               \
    template_apply(sm, da);                                             \
  }                                                                     \
  double evaluate(const PairScore *s,                              \
                  DerivativeAccumulator *da) const {                    \
    return template_evaluate(s, da);                                    \
  }                                                                     \
  double evaluate_if_good(const PairScore *s,                      \
                          DerivativeAccumulator *da, double max) const { \
    return template_evaluate_if_good(s, da, max);                       \
  }                                                                     \
  ParticlesTemp get_contained_particles() const;                        \
  IMP_OBJECT(Name)
#endif







//! Declare the needed functions for a PairContainer
/** In addition to the methods of IMP_OBJECT, it declares
    - IMP::PairContainer::get_contains_particle_particle_pair()
    - IMP::PairContainer::get_number_of_particle_particle_pairs()
    - IMP::PairContainer::get_particle_particle_pair()
    - IMP::PairContainer::apply()
    - IMP::PairContainer::evaluate()
    - IMP::Interaction::get_input_objects()
*/
#define IMP_PAIR_CONTAINER(Name)                                  \
  PairContainerPair get_added_and_removed_containers() const;      \
  bool get_contains_particle_pair(const ParticlePair& p) const;      \
  unsigned int get_number_of_particle_pairs() const;                     \
  ParticlePair get_particle_pair(unsigned int i) const;                  \
  IMP_IMPLEMENT_PAIR_CONTAINER(Name)


#ifndef SWIG
//! Declare the needed functions for a PairFilter
/** In addition to the methods done by all the macros, it declares
    - IMP::PairFilter::get_contains_particle_LCCLASNAME()
    - IMP::PairFilter::get_input_particles()
*/
#define IMP_PAIR_FILTER(Name)                                     \
public:                                                                 \
 bool get_contains(const ParticlePair& p) const;                   \
 ParticlesTemp get_input_particles(Particle* t) const;                  \
 ContainersTemp get_input_containers(Particle* t) const;                \
 void filter_in_place(ParticlePairsTemp &ps) const {                   \
   ps.erase(std::remove_if(ps.begin(), ps.end(),                        \
                           IMP::internal::GetContains<Name>(this)),     \
            ps.end());                                                  \
 }                                                                      \
 IMP_OBJECT(Name)
#else
#define IMP_PAIR_FILTER(Name)                                     \
  bool get_contains(const ParticlePair& p) const;                    \
  ParticlesTemp get_input_particles(Particle*t) const;                  \
  ContainersTemp get_input_containers(Particle*t) const;                \
  IMP_OBJECT(Name)
#endif


/** These macros avoid various inefficiencies.

    The macros take the name of the sequence and the operation to
    peform. The item in the sequence is called _1, it's index is _2.
    Use it like
    \code
    IMP_FOREACH_PARTICLE(sc, std::cout << "Item " << _2
                         << " is _1->get_name() << std::endl);
    \endcode
*/
#define IMP_FOREACH_PAIR(sequence, operation)                     \
  do {                                                                  \
    if (sequence->get_provides_access()) {                              \
      const ParticlePairsTemp &imp_foreach_access                      \
        =sequence->get_access();                                        \
      for (unsigned int _2=0; _2< imp_foreach_access.size(); ++_2) {    \
        IMP::ParticlePair _1= imp_foreach_access[_2];                   \
        bool imp_foreach_break=false;                                   \
        operation                                                       \
          if (imp_foreach_break) { break;}                              \
      }                                                                 \
    } else {                                                            \
      unsigned int imp_foreach_size                                     \
        = sequence->get_number_of_particle_pairs();                      \
      for (unsigned int _2=0;                                           \
           _2 != imp_foreach_size;                                      \
           ++_2) {                                                      \
        IMP::ParticlePair _1= sequence->get_particle_pair(_2);           \
        bool imp_foreach_break=false;                                   \
        operation                                                       \
          if (imp_foreach_break) break;                                 \
      }                                                                 \
    }                                                                   \
  } while (false)

#endif  /* IMP_PAIR_MACROS_H */
