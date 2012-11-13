/**
 *  \file IMP/LCCLASSNAME_macros.h
 *  \brief Macros for various classes.
 *
 *  This file is generated by a script (core/tools/make-containers).
 *  Do not edit directly.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 */

#ifndef IMPKERNEL_HEADERNAME_MACROS_H
#define IMPKERNEL_HEADERNAME_MACROS_H

#include "internal/TupleRestraint.h"
#include "internal/functors.h"
#include "container_macros.h"
#include "input_output_macros.h"
#include <algorithm>


//! Declare the functions needed for a CLASSNAMEScore
/** In addition to the methods done by IMP_INTERACTON, it declares
    - IMP::CLASSNAMEScore::evaluate(IMP::Particle*,
    IMP::DerivativeAccumulator*)
    - IMP::CLASSNAMEScore::get_input_particles()
    - IMP::CLASSNAMEScore::get_output_particles()

    See IMP_SIMPLE_HEADERNAME_SCORE() for a way of providing an
    implementation of that method.
*/
#define IMP_HEADERNAME_SCORE(Name)                                      \
  IMP_IMPLEMENT(double evaluate(ARGUMENTTYPE p,\
                                DerivativeAccumulator *da) const);      \
  IMP_IMPLEMENT_INLINE(double evaluate_index(Model *m,                  \
                                PASSINDEXTYPE p,           \
                                     DerivativeAccumulator *da) const, { \
    return evaluate(IMP::internal::get_particle(m,p), da);              \
                                    });                                 \
  IMP_IMPLEMENT_INLINE(double evaluate_if_good_index(Model *m,          \
                          PASSINDEXTYPE p,                       \
                          DerivativeAccumulator *da,                    \
                                                     double max) const, { \
    IMP_UNUSED(max);                                                    \
    return evaluate_index(m, p, da);                                    \
                       });                                              \
  IMP_BACKWARDS_MACRO_INPUTS;                                           \
  IMP_OBJECT(Name)

//! Declare the functions needed for a CLASSNAMEScore
/** In addition to the methods declared and defined by IMP_HEADERNAME_SCORE,
    the macro provides an implementation of
    - IMP::CLASSNAMEScore::get_input_particles()
    - IMP::CLASSNAMEScore::get_input_containers()
    which assume that only the passed particle serves as input to the
    score.
*/
#define IMP_SIMPLE_HEADERNAME_SCORE(Name)                               \
  IMP_IMPLEMENT(double evaluate(ARGUMENTTYPE p,    \
                                DerivativeAccumulator *da) const);      \
  IMP_IMPLEMENT_INLINE(ModelObjectsTemp                                 \
                       do_get_inputs(Model *m,                          \
                                     const ParticleIndexes &pis) const, { \
                         ModelObjectsTemp ret;                          \
                         ret+=IMP::get_particles(m, pis);               \
                         return ret;                                    \
                       });                                              \
  IMP_IMPLEMENT_INLINE(Restraints create_current_decomposition          \
                       (Model *m,                                       \
                        PASSINDEXTYPE vt) const, {           \
      return  IMP::internal::create_score_current_decomposition(this, m, vt); \
                       });                                        \
  IMP_OBJECT(Name)



//! Declare the functions needed for a complex CLASSNAMEScore
/** In addition to the methods done by IMP_OBJECT(), it declares
    - IMP::CLASSNAMEScore::evaluate()
    - IMP::CLASSNAMEScore::get_input_particles()
    - IMP::CLASSNAMEScore::get_output_particles()
    - IMP::CLASSNAMEScore::evaluate_if_good
*/
#define IMP_COMPOSITE_HEADERNAME_SCORE(Name)                            \
  IMP_IMPLEMENT_INLINE(double evaluate(ARGUMENTTYPE p,     \
                                       DerivativeAccumulator *da) const, { \
    return evaluate_index(IMP::internal::get_model(p),                  \
                  IMP::internal::get_index(p), da);                     \
                       });                                              \
  IMP_IMPLEMENT(double evaluate_index(Model *m, PASSINDEXTYPE p,\
                                      DerivativeAccumulator *da) const); \
  IMP_IMPLEMENT(double evaluate_if_good_index(Model *m,                 \
                          PASSINDEXTYPE p,                       \
                          DerivativeAccumulator *da,                    \
                                              double max) const);       \
  IMP_IMPLEMENT_INLINE(double evaluate_indexes(Model *m,                \
                                        const PLURALINDEXTYPE &p,       \
                                        DerivativeAccumulator *da,      \
                                        unsigned int lower_bound,       \
                                               unsigned int upper_bound) const,\
  {                                                                     \
    double ret=0;                                                       \
    for (unsigned int i=lower_bound; i < upper_bound; ++i) {            \
      ret+= evaluate_index(m, p[i], da);                                \
    }                                                                   \
    return ret;                                                         \
  });                                                                   \
  IMP_IMPLEMENT_INLINE(double                                           \
  evaluate_if_good_index(Model *m,                                      \
                         const PLURALINDEXTYPE &p,                      \
                         DerivativeAccumulator *da,                     \
                         double max,                                    \
                         unsigned int lower_bound,                      \
                         unsigned int upper_bound) const, {             \
    double ret=0;                                                       \
    for (unsigned int i=lower_bound; i < upper_bound; ++i) {            \
      ret+= evaluate_index(m, p[i], da);                                \
      if (ret>max) return std::numeric_limits<double>::max();           \
    }                                                                   \
    return ret;                                                         \
                       });                                              \
  IMP_BACKWARDS_MACRO_INPUTS;                                           \
  IMP_OBJECT(Name)

//! Declare the functions needed for a complex CLASSNAMEScore
/** In addition to the methods done by IMP_OBJECT(), it declares
    - IMP::CLASSNAMEScore::evaluate()
    - IMP::CLASSNAMEScore::get_input_particles()
    - IMP::CLASSNAMEScore::get_output_particles()
    - IMP::CLASSNAMEScore::evaluate_if_good
*/
#define IMP_INDEX_HEADERNAME_SCORE(Name)                                \
  IMP_IMPLEMENT_INLINE(double evaluate(ARGUMENTTYPE p,\
                                        DerivativeAccumulator *da) const, { \
    return evaluate_index(IMP::internal::get_model(p),                  \
                  IMP::internal::get_index(p),                          \
                  da);                                                  \
                        });                                             \
  IMP_IMPLEMENT(double evaluate_index(Model *m, PASSINDEXTYPE p,\
                                      DerivativeAccumulator *da) \
                const IMP_OVERRIDE IMP_FINAL);                         \
  IMP_IMPLEMENT_INLINE(double evaluate_if_good_index(Model *m,         \
                          PASSINDEXTYPE p,                      \
                          DerivativeAccumulator *da,                    \
                                                      double max) const, { \
    IMP_UNUSED(max);                                                    \
    return evaluate_index(m, p, da);                                    \
                       });                                              \
  IMP_IMPLEMENT_INLINE(double                                           \
  evaluate_indexes(Model *m,                                            \
                   const PLURALINDEXTYPE &p,                            \
                   DerivativeAccumulator *da,                           \
                   unsigned int lower_bound,                            \
                   unsigned int upper_bound) const IMP_OVERRIDE IMP_FINAL, \
  {                                                                     \
    double ret=0;                                                       \
    for (unsigned int i=lower_bound; i < upper_bound; ++i) {            \
      ret+= evaluate_index(m, p[i], da);                                \
    }                                                                   \
    return ret;                                                         \
  });                                                                   \
  IMP_IMPLEMENT_INLINE(double                                           \
  evaluate_if_good_index(Model *m,                                      \
                         const PLURALINDEXTYPE &p,                      \
                         DerivativeAccumulator *da,                     \
                         double max,                                    \
                         unsigned int lower_bound,                      \
                         unsigned int upper_bound) const, {             \
    double ret=0;                                                       \
    for (unsigned int i=lower_bound; i < upper_bound; ++i) {            \
      ret+= evaluate_if_good_index(m, p[i], da, max-ret);               \
      if (ret>max) return std::numeric_limits<double>::max();           \
    }                                                                   \
    return ret;                                                         \
                       });                                              \
  IMP_IMPLEMENT(ModelObjectsTemp                                        \
  do_get_inputs(Model *m,                                               \
                const ParticleIndexes &pis) const IMP_OVERRIDE);        \
  IMP_OBJECT(Name)



//! Declare the functions needed for a CLASSNAMEPredicate
/** In addition to the methods done by IMP_OBJECT, it declares
    - IMP::CLASSNAMEPredicate::get_value()
    - IMP::CLASSNAMEPredicate::get_input_particles()
    - IMP::CLASSNAMEPredicate::get_output_particles()
*/
#define IMP_HEADERNAME_PREDICATE(Name)                                   \
  IMP_IMPLEMENT(int get_value(ARGUMENTTYPE a) const);   \
  IMP_IMPLEMENT_INLINE(Ints get_value(const                             \
                              PLURALVARIABLETYPE &o) const, {   \
    Ints ret(o.size());                                                 \
    for (unsigned int i=0; i< o.size(); ++i) {                          \
      ret[i]+= Name::get_value(o[i]);                                   \
    }                                                                   \
    return ret;                                                         \
    });                                                                 \
  IMP_IMPLEMENT_INLINE(int get_value_index(Model *m,                    \
                                           PASSINDEXTYPE vt)\
                       const, {                                         \
        return Name::get_value(IMP::internal::get_particle(m, vt)); \
                       });                                              \
  IMP_IMPLEMENT_INLINE(Ints get_value_index(Model *m,                   \
                                     const PLURALINDEXTYPE &o) const, { \
   Ints ret(o.size());                                                  \
   for (unsigned int i=0; i< o.size(); ++i) {                           \
     ret[i]+= Name::get_value_index(m, o[i]);                           \
   }                                                                    \
   return ret;                                                          \
                       });                                              \
  IMP_BACKWARDS_MACRO_INPUTS;                                           \
  IMP_OBJECT(Name)


//! Declare the functions needed for a CLASSNAMEPredicate
/** In addition to the methods done by IMP_OBJECT, it defines
    - IMP::CLASSNAMEPredicate::get_value_index() based on the return_value
    parameter
    - IMP::CLASSNAMEPredicate::do_get_inputs() based on the return_inputs
    parameter
*/
#define IMP_INDEX_HEADERNAME_PREDICATE(Name, return_value, return_inputs) \
  IMP_IMPLEMENT_INLINE(int get_value(ARGUMENTTYPE a) const, {    \
    return get_value_index(IMP::internal::get_model(a),                 \
                     IMP::internal::get_index(a));                      \
    });                                                                 \
  IMP_IMPLEMENT_INLINE(Ints get_value(const                             \
                                      PLURALVARIABLETYPE &o) const, {   \
    Ints ret(o.size());                                                 \
    for (unsigned int i=0; i< o.size(); ++i) {                          \
      ret[i]+= Name::get_value(o[i]);                                   \
    }                                                                   \
    return ret;                                                         \
                       })                                               \
  IMP_IMPLEMENT_INLINE(int get_value_index(Model *m,                    \
                                           PASSINDEXTYPE pi)\
                       const, {                                         \
                         return_value;                                  \
                       })                                               \
  IMP_IMPLEMENT_INLINE(Ints get_value_index(Model *m,                   \
                                const PLURALINDEXTYPE &o) const, { \
   Ints ret(o.size());                                                  \
   for (unsigned int i=0; i< o.size(); ++i) {                           \
     ret[i]+= Name::get_value_index(m, o[i]);                           \
   }                                                                    \
   return ret;                                                          \
                       });                                              \
  IMP_IMPLEMENT_INLINE_NO_SWIG(void remove_if_equal(Model *m,           \
                                            PLURALINDEXTYPE& ps,        \
                                            int value) const, {         \
      ps.erase(std::remove_if(ps.begin(), ps.end(),                     \
                              IMP::internal::PredicateEquals<Name, true>(this, \
                                                              m, value)), \
               ps.end());                                               \
                       });                                              \
  IMP_IMPLEMENT_INLINE_NO_SWIG(void remove_if_not_equal(Model *m,       \
                                            PLURALINDEXTYPE& ps,        \
                                            int value) const, {         \
      ps.erase(std::remove_if(ps.begin(), ps.end(),                     \
                          IMP::internal::PredicateEquals<Name, false>(this, \
                                                                 m, value)), \
               ps.end());                                               \
                       });                                              \
  IMP_IMPLEMENT_INLINE(ModelObjectsTemp                                 \
  do_get_inputs(Model *m,                                               \
                const ParticleIndexes &pi) const, {                     \
    return_inputs;                                                      \
                       });                                              \
  IMP_OBJECT_INLINE(Name,IMP_UNUSED(out),)


//! Declare the functions needed for a CLASSNAMEModifier
/** In addition to the methods done by IMP_OBJECT, it declares
    - IMP::CLASSNAMEModifier::apply(IMP::Particle*)
    - IMP::CLASSNAMEModifier::get_input_particles()
    - IMP::CLASSNAMEModifier::get_output_particles()
*/
#define IMP_HEADERNAME_MODIFIER(Name)                                   \
  IMP_IMPLEMENT(void apply(ARGUMENTTYPE a) const); \
  IMP_IMPLEMENT_INLINE(void apply_index(Model *m, \
                                        PASSINDEXTYPE a) const, {\
    return Name::apply(IMP::internal::get_particle(m,a));               \
    })                                                                  \
  IMP_BACKWARDS_MACRO_INPUTS;                                                 \
  IMP_BACKWARDS_MACRO_OUTPUTS;                                                \
  IMP_OBJECT(Name)

//! Use IMP_HEADERNAME_MODIFIER() instead
#define IMP_HEADERNAME_DERIVATIVE_MODIFIER(Name)                        \
  IMP_HEADERNAME_MODIFIER(Name)


//! Declare the functions needed for a CLASSNAMEModifier
/** In addition to the methods done by IMP_OBJECT, it declares
    - IMP::CLASSNAMEModifier::apply(IMP::Particle*)
    - IMP::CLASSNAMEModifier::get_inputs()
    - IMP::CLASSNAMEModifier::get_outputs()
*/
#define IMP_INDEX_HEADERNAME_MODIFIER(Name)                 \
  IMP_IMPLEMENT_INLINE(void apply(ARGUMENTTYPE a) const, {  \
    apply_index(IMP::internal::get_model(a),                            \
                IMP::internal::get_index(a));                           \
    });                                                                 \
  IMP_IMPLEMENT(void apply_index(Model *m,                              \
                                 PASSINDEXTYPE a)\
                const IMP_OVERRIDE IMP_FINAL);                          \
  IMP_IMPLEMENT_INLINE(void apply_indexes(Model *m,                     \
                                          const PLURALINDEXTYPE &o,     \
                                          unsigned int lower_bound,     \
                                          unsigned int upper_bound)\
                       const IMP_OVERRIDE IMP_FINAL,                    \
  {                                                                     \
    for (unsigned int i=lower_bound; i < upper_bound; ++i) {            \
      apply_index(m, o[i]);                                             \
    }                                                                   \
  });                                                                   \
  IMP_IMPLEMENT(ModelObjectsTemp do_get_inputs(Model *m,                \
                                    const ParticleIndexes &pis) const); \
  IMP_IMPLEMENT(ModelObjectsTemp do_get_outputs(Model *m,               \
                                    const ParticleIndexes &pis) const); \
  IMP_OBJECT(Name)

//! Use IMP_INDEX_HEADERNAME_MODIFIER instead
#define IMP_INDEX_HEADERNAME_DERIVATIVE_MODIFIER(Name)  \
  IMP_INDEX_HEADERNAME_MODIFIER(Name)




#ifndef IMP_DOXYGEN
#define IMP_IMPLEMENT_HEADERNAME_CONTAINER(Name)                        \
  void apply(const CLASSNAMEModifier *sm) const {                       \
    apply_generic(sm);                                                  \
  }                                                                     \
  ParticleIndexes get_all_possible_indexes() const;                     \
  IMP_OBJECT(Name)
#endif





//! Declare the needed functions for a CLASSNAMEContainer
/** In addition to the methods of IMP_OBJECT, it declares
    - IMP::CLASSNAMEContainer::get_number_of_particle_FUNCTIONNAMEs()
    - IMP::CLASSNAMEContainer::get_particle_FUNCTIONNAME()
    - IMP::CLASSNAMEContainer::apply()
    - IMP::CLASSNAMEContainer::evaluate()
    - IMP::Interaction::get_input_objects()

    You need to define a template method with the signature
\code
template <class Functor>
Functor for_each(Functor f);
\endcode
    that applied the functor to each thing in the container.
*/
#define IMP_HEADERNAME_CONTAINER(Name)                                  \
  IMP_IMPLEMENT(bool get_is_changed() const);                           \
  IMP_IMPLEMENT(PLURALINDEXTYPE get_indexes() const);                   \
  IMP_IMPLEMENT(PLURALINDEXTYPE get_range_indexes() const);      \
  IMP_IMPLEMENT(void do_before_evaluate());                             \
  IMP_IMPLEMENT(ParticlesTemp get_input_particles() const);             \
  IMP_IMPLEMENT(ContainersTemp get_input_containers() const);           \
  ModelObjectsTemp do_get_inputs() const {                              \
    ModelObjects ret;                                                   \
    ret+=get_input_containers();                                        \
    ret+=get_input_particles();                                         \
    return ret;                                                         \
  }                                                                     \
  IMP_IMPLEMENT_HEADERNAME_CONTAINER(Name)


//! Declare the needed functions for an active CLASSNAMEContainer
/** In addition to the methods of IMP_HEADERNAME_CONTAINER(), it declares
    - IMP::ScoreState::get_input_particles()
    - IMP::ScoreState::get_input_containers()
    - IMP::ScoreState::do_before_evaluate()
*/
#define IMP_ACTIVE_HEADERNAME_CONTAINER(Name)                           \
  IMP_HEADERNAME_CONTAINER(name)

/** Use IMP_CONTAINER_FOREACH() instead.
*/
#define IMP_FOREACH_HEADERNAME(sequence, operation) do {                \
    IMP::PLURALVARIABLETYPE imp_all=sequence->get();   \
  for (unsigned int _2=0;                                               \
       _2 != imp_all.size();                                            \
       ++_2) {                                                          \
    IMP::VARIABLETYPE _1= imp_all[_2];               \
    bool imp_foreach_break=false;                                       \
    operation                                                           \
      if (imp_foreach_break) break;                                     \
  }                                                                     \
  } while (false)



/** Use IMP_CONTAINER_FOREACH() instead.
*/
#define IMP_FOREACH_HEADERNAME_INDEX(sequence, operation)               \
  IMP_CONTAINER_FOREACH(CLASSNAMEContainer, sequence, operation)

#endif  /* IMPKERNEL_HEADERNAME_MACROS_H */
