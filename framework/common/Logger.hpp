/*
 * Logger.hpp
 *
 * Lightweight compile-time-controlled logging macros for the framework.
 *
 * Log level is controlled by the LOG_LEVEL preprocessor macro:
 *   LOG_LEVEL 0  —  silent (no output at all)
 *   LOG_LEVEL 1  —  errors only
 *   LOG_LEVEL 2  —  errors + info
 *   LOG_LEVEL 3  —  errors + info + debug  (default when NDEBUG is not defined)
 *
 * The level defaults to 3 in debug builds and 2 in release builds (NDEBUG).
 * Override at build time with:
 *   g++ -DLOG_LEVEL=1 ...
 *   or in CMakeLists.txt:
 *     target_compile_definitions(my_target PRIVATE LOG_LEVEL=1)
 *
 * ---------------------------------------------------------------------------
 * Usage
 * ---------------------------------------------------------------------------
 *   #include "framework/common/Logger.hpp"
 *
 *   LOG_DEBUG("entering handler, fd=" << fd);
 *   LOG_INFO("server ready on port " << port);
 *   LOG_ERROR("select failed, errno=" << errno);
 *
 * Output format:
 *   [DEBUG] handleInput(): entering handler, fd=5
 *   [INFO]  runReactorEventLoop(): server ready on port 8080
 *   [ERROR] handleEvents(): select failed, errno=9
 */

#ifndef FRAMEWORK_COMMON_LOGGER_HPP_
#define FRAMEWORK_COMMON_LOGGER_HPP_

#include <iostream>
#include <sstream>
#include <mutex>

namespace Common
{

/**
 * Logger provides a thread-safe, line-atomic write helper used by the
 * LOG_* macros below.  All public methods are static; Logger is never
 * instantiated directly.
 */
class Logger
{
  public:
    /**
     * Returns the process-wide mutex that serialises all log writes.
     * Uses a function-local static so no out-of-order construction is
     * possible and no separate .cpp translation unit is needed.
     */
    static std::mutex& getMutex()
    {
        static std::mutex sMutex;
        return sMutex;
    }

    /**
     * Writes a prefixed, function-tagged log line to stdout.
     * The entire line is flushed as one operation under the mutex to
     * prevent interleaving from concurrent threads.
     *
     * @param prefix   Level tag, e.g. "[DEBUG] " or "[INFO]  "
     * @param func     Value of __FUNCTION__ at the call site
     * @param msg      Already-formatted message string
     */
    static void log( const char* prefix,
                     const std::string& msg )
    {
        const std::lock_guard<std::mutex> lock( getMutex() );
        std::cout << prefix << msg << '\n';
    }

    /**
     * Writes an error log line to stderr.
     *
     * @param func  Value of __FUNCTION__ at the call site
     * @param msg   Already-formatted message string
     */
    static void logError( const char* func, const std::string& msg )
    {
        const std::lock_guard<std::mutex> lock( getMutex() );
        std::cerr << "[ERROR] " << func << "(): " << msg << '\n';
    }

  private:
    Logger()  = delete;
    ~Logger() = delete;
};

} /* namespace Common */

/* ---------------------------------------------------------------------------
 * Compile-time log level selection
 * --------------------------------------------------------------------------*/
#ifndef LOG_LEVEL
#  ifdef NDEBUG
#    define LOG_LEVEL 2   /* release builds : INFO and above  */
#  else
#    define LOG_LEVEL 3   /* debug builds   : everything      */
#  endif
#endif

/* ---------------------------------------------------------------------------
 * LOG_DEBUG  —  compiled out entirely when LOG_LEVEL < 3
 * --------------------------------------------------------------------------*/
#if LOG_LEVEL >= 3
#  define LOG_DEBUG( msg )                                                    \
     do {                                                                     \
         std::ostringstream _log_oss;                                         \
         _log_oss << msg;                                                     \
         Common::Logger::log( "[DEBUG] ", _log_oss.str() );    \
     } while ( 0 )
#else
#  define LOG_DEBUG( msg ) do {} while ( 0 )
#endif

/* ---------------------------------------------------------------------------
 * LOG_INFO  —  compiled out entirely when LOG_LEVEL < 2
 * --------------------------------------------------------------------------*/
#if LOG_LEVEL >= 2
#  define LOG_INFO( msg )                                                     \
     do {                                                                     \
         std::ostringstream _log_oss;                                         \
         _log_oss << msg;                                                     \
         Common::Logger::log( "[INFO]  ", _log_oss.str() );    \
     } while ( 0 )
#else
#  define LOG_INFO( msg ) do {} while ( 0 )
#endif

/* ---------------------------------------------------------------------------
 * LOG_ERROR  —  always active regardless of LOG_LEVEL
 * --------------------------------------------------------------------------*/
#define LOG_ERROR( msg )                                                      \
     do {                                                                     \
         std::ostringstream _log_oss;                                         \
         _log_oss << msg;                                                     \
         Common::Logger::logError( __FUNCTION__, _log_oss.str() );           \
     } while ( 0 )

#endif /* FRAMEWORK_COMMON_LOGGER_HPP_ */

