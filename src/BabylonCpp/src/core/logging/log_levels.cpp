#include <babylon/core/logging/log_levels.h>

namespace BABYLON {

constexpr unsigned int LogLevels::LEVEL_QUIET = 0;
constexpr unsigned int LogLevels::LEVEL_ERROR = 1;
constexpr unsigned int LogLevels::LEVEL_WARN  = 2;
constexpr unsigned int LogLevels::LEVEL_INFO  = 3;
constexpr unsigned int LogLevels::LEVEL_DEBUG = 4;
constexpr unsigned int LogLevels::LEVEL_TRACE = 5;

const std::vector<std::pair<unsigned int, std::string>> LogLevels::Levels
  = {std::make_pair(LogLevels::LEVEL_QUIET, "QUIET"),
     std::make_pair(LogLevels::LEVEL_ERROR, "ERROR"),
     std::make_pair(LogLevels::LEVEL_WARN, "WARN"),
     std::make_pair(LogLevels::LEVEL_INFO, "INFO"),
     std::make_pair(LogLevels::LEVEL_DEBUG, "DEBUG"),
     std::make_pair(LogLevels::LEVEL_TRACE, "TRACE")};

std::string LogLevels::ToReadableLevel(unsigned int level)
{
  switch (level) {
    case LogLevels::LEVEL_QUIET:
      return "QUIET";
    case LogLevels::LEVEL_ERROR:
      return "ERROR";
    case LogLevels::LEVEL_WARN:
      return "WARN";
    case LogLevels::LEVEL_INFO:
      return "INFO";
    case LogLevels::LEVEL_DEBUG:
      return "DEBUG";
    case LogLevels::LEVEL_TRACE:
      return "TRACE";
    default:
      return "QUIET";
  }
}

} // end of namespace BABYLON
