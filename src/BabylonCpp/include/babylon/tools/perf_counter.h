#ifndef BABYLON_TOOLS_PERF_COUNTER_H
#define BABYLON_TOOLS_PERF_COUNTER_H

#include <babylon/babylon_global.h>
#include <babylon/core/time.h>

namespace BABYLON {

/**
 * @brief This class is used to track a performance counter which is number
 * based.
 * The user has access to many properties which give statistics of different
 * nature
 *
 * The implementer can track two kinds of Performance Counter: time and count
 * For time you can optionally call fetchNewFrame() to notify the start of a new
 * frame to monitor, then call beginMonitoring() to start and endMonitoring() to
 * record the lapsed time. endMonitoring takes a newFrame parameter for you to
 * specify if the monitored time should be set for a new frame or accumulated to
 * the current frame being monitored.
 * For count you first have to call fetchNewFrame() to notify the start of a new
 * frame to monitor, then call addCount() how many time required to increment
 * the count value you monitor.
 */

class BABYLON_SHARED_EXPORT PerfCounter {

public:
  PerfCounter()
      : _startMonitoringTime{Time::highresTimepointNow()}
      , _min{0}
      , _max{0}
      , _average{0.0}
      , _current{0}
      , _totalValueCount{0}
      , _totalAccumulated{0}
      , _lastSecAverage{0}
      , _lastSecAccumulated{0}
      , _lastSecTime{Time::highresTimepointNow()}
      , _lastSecValueCount{0}
  {
  }

  PerfCounter(const PerfCounter& other)
      : _startMonitoringTime{other._startMonitoringTime}
      , _min{other._min}
      , _max{other._max}
      , _average{other._average}
      , _current{other._current}
      , _totalValueCount{other._totalValueCount}
      , _totalAccumulated{other._totalAccumulated}
      , _lastSecAverage{other._lastSecAverage}
      , _lastSecAccumulated{other._lastSecAccumulated}
      , _lastSecTime{other._lastSecTime}
      , _lastSecValueCount{other._lastSecValueCount}
  {
  }

  PerfCounter(PerfCounter&& other)
      : _startMonitoringTime{std::move(other._startMonitoringTime)}
      , _min{std::move(other._min)}
      , _max{std::move(other._max)}
      , _average{std::move(other._average)}
      , _current{std::move(other._current)}
      , _totalValueCount{std::move(other._totalValueCount)}
      , _totalAccumulated{std::move(other._totalAccumulated)}
      , _lastSecAverage{std::move(other._lastSecAverage)}
      , _lastSecAccumulated{std::move(other._lastSecAccumulated)}
      , _lastSecTime{std::move(other._lastSecTime)}
      , _lastSecValueCount{std::move(other._lastSecValueCount)}
  {
  }

  PerfCounter& operator=(const PerfCounter& other)
  {
    if (&other != this) {
      _startMonitoringTime = other._startMonitoringTime;
      _min                 = other._min;
      _max                 = other._max;
      _average             = other._average;
      _current             = other._current;
      _totalValueCount     = other._totalValueCount;
      _totalAccumulated    = other._totalAccumulated;
      _lastSecAverage      = other._lastSecAverage;
      _lastSecAccumulated  = other._lastSecAccumulated;
      _lastSecTime         = other._lastSecTime;
      _lastSecValueCount   = other._lastSecValueCount;
    }

    return *this;
  }

  PerfCounter& operator=(PerfCounter&& other)
  {
    if (&other != this) {
      std::swap(_startMonitoringTime, other._startMonitoringTime);
      std::swap(_min, other._min);
      std::swap(_max, other._max);
      std::swap(_average, other._average);
      std::swap(_current, other._current);
      std::swap(_totalValueCount, other._totalValueCount);
      std::swap(_totalAccumulated, other._totalAccumulated);
      std::swap(_lastSecAverage, other._lastSecAverage);
      std::swap(_lastSecAccumulated, other._lastSecAccumulated);
      std::swap(_lastSecTime, other._lastSecTime);
      std::swap(_lastSecValueCount, other._lastSecValueCount);
    }

    return *this;
  }

  ~PerfCounter()
  {
  }

  /**
   * Returns the smallest value ever
   */
  size_t min() const
  {
    return _min;
  }

  /**
   * Returns the biggest value ever
   */
  size_t max() const
  {
    return _max;
  }

  /**
   * Returns the average value since the performance counter is running
   */
  double average() const
  {
    return _average;
  }

  /**
   * Returns the average value of the last second the counter was monitored
   */
  double lastSecAverage() const
  {
    return _lastSecAverage;
  }

  /**
   * Returns the current value
   */
  size_t current() const
  {
    return _current;
  }

  size_t total() const
  {
    return _totalAccumulated;
  }

  /**
   * Call this method to start monitoring a new frame.
   * This scenario is typically used when you accumulate monitoring time many
   * times for a single frame, you call this method at the start of the frame,
   * then beginMonitoring to start recording and endMonitoring(false) to
   * accumulated the recorded time to the PerfCounter or addCount() to
   * accumulate a monitored count.
   */
  void fetchNewFrame()
  {
    ++_totalValueCount;
    _current = 0;
    ++_lastSecValueCount;
  }

  /**
   * Call this method to monitor a count of something (e.g. mesh drawn in
   * viewport count)
   * @param newCount the count value to add to the monitored count
   * @param fetchResult true when it's the last time in the frame you add to the
   * counter and you wish to update the statistics properties (min/max/average),
   * false if you only want to update statistics.
   */
  void addCount(size_t newCount, bool fetchResult)
  {
    _current += newCount;
    if (fetchResult) {
      _fetchResult();
    }
  }

  /**
   * Start monitoring this performance counter
   */
  void beginMonitoring()
  {
    _startMonitoringTime = Time::highresTimepointNow();
  }

  /**
   * Compute the time lapsed since the previous beginMonitoring() call.
   * @param newFrame true by default to fetch the result and monitor a new
   * frame, if false the time monitored will be added to the current frame
   * counter
   */
  void endMonitoring(bool newFrame = true)
  {
    if (newFrame) {
      fetchNewFrame();
    }

    auto currentTime = Time::highresTimepointNow();
    _current
      = Time::fpTimeDiff<size_t, std::micro>(_startMonitoringTime, currentTime);

    if (newFrame) {
      _fetchResult();
    }
  }

private:
  void _fetchResult()
  {
    _totalAccumulated += _current;
    _lastSecAccumulated += _current;

    // Min/Max update
    _min     = std::min(_min, _current);
    _max     = std::max(_max, _current);
    _average = static_cast<double>(_totalAccumulated)
               / static_cast<double>(_totalValueCount);

    // Reset last sec?
    auto now = Time::highresTimepointNow();
    if (Time::fpTimeDiff<double, std::milli>(_lastSecTime, now) > 1000.0) {
      _lastSecAverage = static_cast<double>(_lastSecAccumulated)
                        / static_cast<double>(_lastSecValueCount);
      _lastSecTime        = now;
      _lastSecAccumulated = 0;
      _lastSecValueCount  = 0;
    }
  }

private:
  high_res_time_point_t _startMonitoringTime;
  size_t _min;
  size_t _max;
  double _average;
  size_t _current;
  size_t _totalValueCount;
  size_t _totalAccumulated;
  double _lastSecAverage;
  size_t _lastSecAccumulated;
  high_res_time_point_t _lastSecTime;
  double _lastSecValueCount;

}; // end of class PerfCounter

} // end of namespace BABYLON

#endif // end of BABYLON_TOOLS_PERF_COUNTER_H
