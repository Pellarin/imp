/**
 *  \file base/log.h
 *  \brief Logging and error reporting support.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPBASE_LOG_H
#define IMPBASE_LOG_H

#include "base_config.h"
#include "base_macros.h"
#include "types.h"
#include "WarningContext.h"
#include "internal/log.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>

IMPBASE_BEGIN_NAMESPACE

/** \name Logging
    \anchor log
    \imp provides tools for controlling the amount of log output produced
    and directing it to the terminal or a file. Only log messages tagged
    with a lower level than the current LogLevel are emitted. In addition
    to a global log level (get_log_level(), set_log_level()), each
    IMP::Object has an internal log level (IMP::Object::get_log_level(),
    IMP::Object::set_log_level()) which is used when executing code on
    that object.

    Logging is provided by IMP/log.h.

    People implementing IMP::Object classes should also see IMP_OBJECT_LOG()
    and IMP::SetLogState.

    All logging is disabled when \imp is built using \c build='fast'.
    @{
 */

//! Push a new log context onto the stack
/** A log context is, eg, a function name.
 */
IMPBASEEXPORT void push_log_context(const char *functionname,
                                    const void *object);

//! pop the top log context
IMPBASEEXPORT void pop_log_context();

//! Write a string to the log
IMPBASEEXPORT void add_to_log(std::string to_write);


//! Set the current global log level
/** Note that this should not, currently, be used directly
    during Model::evaluate() calls. */
IMPBASEEXPORT void set_log_level(LogLevel l);

//! Set whether log messages are tagged with the current log time
IMPBASEEXPORT void set_log_timer(bool tb);

//! Reset the log timer
IMPBASEEXPORT void reset_log_timer();


//! Get the currently active log level
/** This may not always match the value passed to set_log_level()
    as objects can temporarily override the global level
    while they are evaluating.
 */
inline LogLevel get_log_level()
{
  return internal::log_level;
}

#if !defined(IMP_DOXYGEN) && !defined(SWIG)
inline bool get_is_log_output(LogLevel l)
{
  return l <= get_log_level();
}
#endif

#ifdef IMP_DOXYGEN
//! Execute the code block if a certain level of logging is on
/**
   The next code block (delimited by { }) is executed if
   get_log_level() >= level.

   \code
   IMP_IF_LOG(VERBOSE) {
     Floats testp(input.begin(), input.end());
     std::sort(testp.begin(), testp.end());
     IMP_LOG(VERBOSE, "Sorted order is ");
     IMP_LOG_WRITE(VERBOSE, std::copy(testp.begin(), testp.end(),
                   std::ostream_iterator<double>(IMP_STREAM, " ")));
   }
   \endcode
 */
#define IMP_IF_LOG(level)                               \
  if (IMP::base::level <= ::IMP::base::get_log_level())


//! Write an entry to a log.
/** \param[in] level The IMP::Log_Level for the message
    \param[in] expr A stream expression to be sent to the output stream

    Usage:
    \code
    IMP_LOG(VERBOSE, "Hi there, I'm very talkative. My favorite numbers are "
                     << 1 << " " << 2 << " " << 3);
    \endcode
 */
#define IMP_LOG(level, expr)                                \
  if (IMP::get_is_log_output(IMP::base::level))             \
    { std::ostringstream oss;                               \
      using std::operator<<;                                \
      oss<< expr << std::flush;                             \
      IMP::base::add_to_log(oss.str());                     \
    };

//! Write an entry to a log. This is to be used for objects with no operator<<.
/** \param[in] level The IMP::Log_Level for the message
    \param[in] expr An expression which writes something to IMP_STREAM

    \code
    IMP_LOG_WRITE(VERBOSE, IMP::atom::write_pdb(h, IMP_STREAM));
    \endcode
 */
#define IMP_LOG_WRITE(level, expr)                                \
  {                                                               \
    using IMP::base::VERBOSE; using IMP::base::TERSE;             \
    if (IMP::base::get_is_log_output(level))                      \
      {                                                           \
        std::ostringstream IMP_STREAM;                            \
        expr;                                                     \
        IMP::base::add_to_log(IMP_STREAM.str());                  \
      }                                                           \
  }
#else // IMP_DOXYGEN

#if IMP_BUILD < IMP_FAST

#define IMP_IF_LOG(level)                       \
  if (IMP::base::level <= ::IMP::base::get_log_level())

#define IMP_LOG(level, expr)                                            \
  {                                                                     \
    using IMP::base::VERBOSE; using IMP::base::TERSE;                   \
    if (IMP::base::get_is_log_output(level)){                           \
      std::ostringstream oss;                                           \
      oss<< expr << std::flush;                                         \
      IMP::base::add_to_log(oss.str());                                 \
    }                                                                   \
  }

#define IMP_LOG_WRITE(level, expr)                                      \
  if (IMP::base::get_is_log_output(IMP::base::level))                   \
  {std::ostringstream IMP_STREAM;                                       \
    expr;                                                               \
    IMP::base::add_to_log(IMP_STREAM.str());                            \
  }

#else
#define IMP_LOG(l,e)
#define IMP_LOG_WRITE(l,e)
#define IMP_IF_LOG(l) if (false)
#endif
#endif // else on IMP_DXOYGEN


#ifdef IMP_DOXYGEN
//! Write a warning to a log.
/** \param[in] expr An expression to be output to the log. It is prefixed
                    by "WARNING"
 */
#define IMP_WARN(expr)


//! Write a warning once per context object
/** Use this macro to, for example, warn on unprocessable fields in a PDB,
    since they tend to come together. The key is what is tested
    for uniqueness, the expr is what is output.

    Warnings are only output when the context object is destroyed.
 */
#define IMP_WARN_ONCE(key, expr, context)

//! Write an entry to a log. This is to be used for objects with no operator<<.
/** \param[in] expr An expression which writes something to IMP_STREAM.
                    It is prefixed by "WARNING"
 */
#define IMP_WARN_WRITE(expr)

//! Write a warning to standard error.
/** \param[in] expr An expression to be output to std::cerr. It is prefixed
                    by "ERROR"
 */
#define IMP_ERROR(expr)

//! Write an entry to standard error; for objects with no operator<<.
/** \param[in] expr An expression which writes something to IMP_STREAM.
                    It is prefixed by "ERROR"
 */
#define IMP_ERROR_WRITE(expr)

#elif IMP_BUILD==IMP_FAST

#define IMP_WARN(expr)


#define IMP_WARN_ONCE(key, expr, context)


#define IMP_WARN_WRITE(expr)

#define IMP_ERROR(expr) std::cerr << "ERROR: " << expr << std::endl;

#define IMP_ERROR_WRITE(expr)



#else
#define IMP_WARN(expr) if (IMP::base::get_is_log_output(IMP::base::WARNING)) \
    { std::ostringstream oss;                                \
      oss << "WARNING  " << expr << std::flush;              \
      IMP::base::add_to_log(oss.str());                      \
    };


#define IMP_WARN_ONCE(key, expr, context) {                  \
    std::ostringstream oss;                                  \
    oss << expr << std::flush;                               \
    context.add_warning(key, oss.str());                     \
  }


#define IMP_WARN_WRITE(expr)                                       \
  if (IMP::base::get_is_log_output(IMP::base::WARNING))            \
    {std::ostringstream IMP_STREAM;                                \
      expr;                                                        \
      IMP::base::add_to_log(IMP_STREAM.str());                     \
    }

#define IMP_ERROR(expr) std::cerr << "ERROR: " << expr << std::endl;

#define IMP_ERROR_WRITE(expr) {         \
  std::ostream &IMP_STREAM = std::cerr; \
  std::cerr<< "ERROR ";                 \
  expr;                                 \
  std::cerr << std::endl;               \
}
#endif



#ifdef IMP_DOXYGEN
//! Set the log level to the object's log level.
/** All non-trivial Object methods should start with this. It creates a
    RAII-style object which sets the log level to the local one,
    if appropriate, until it goes out of scope.
 */
#define IMP_OBJECT_LOG

//! Beginning logging for a non-member function
/**
 */
#define IMP_FUNCTION_LOG

#endif

// recommended by http://gcc.gnu.org/gcc/Function-Names.html
#if defined(_MSC_VER)
#  define __func__ __FUNCTION__
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ < 199901L
# if __GNUC__ >= 2
#  define __func__ __FUNCTION__
# else
#  define __func__ "<unknown>"
# endif
#endif

#if IMP_BUILD < IMP_FAST
#define IMP_OBJECT_LOG \
  IMP::base::SetLogState log_state_guard__(this->get_log_level());      \
  IMP::base::SetCheckState check_state_guard__(this->get_check_level()); \
  IMP_CHECK_OBJECT(this);                                               \
  IMP::base::CreateLogContext log_context__(__func__, this)


#define IMP_FUNCTION_LOG                                                \
  IMP::base::CreateLogContext log_context__(__func__)


#else // fast
#define IMP_OBJECT_LOG
#define IMP_FUNCTION_LOG
#endif // fast


//! Create a new log context
/** The following produces

    \verbatim
    myfunction:
       1
       2
       3
    \endverbatim

    \code
    {
        CreateLogContext ii("myfunction ");
        IMP_LOG(VERBOSE, 1);
        IMP_LOG(VERBOSE, 2);
        IMP_LOG(VERBOSE, 3);
    }
    IMP_LOG(VERBOSE, "Now it is has ended." << std::endl);
    \endcode

    The more interesting use is that you can use it before
    calling a function to ensure that all the output of that
    function is nicely offset.
*/
struct CreateLogContext {
public:
  IMP_RAII(CreateLogContext, (const char *fname, const void* object=NULL),,
           push_log_context(fname, object),
           pop_log_context(),);
};


#ifdef IMP_DOXYGEN
//! Create a new long context from a streamed name
#define IMP_LOG_CONTEXT(name)
#else
#if IMP_BUILD < IMP_FAST
#define IMP_LOG_CONTEXT(name)                                           \
  CreateLogContext imp_log_context(name, NULL)
#else
#define IMP_LOG_CONTEXT(name)
#endif
#endif // IMP_DOXYGEN


/** @} */

IMPBASE_END_NAMESPACE


#endif  /* IMPBASE_LOG_H */
