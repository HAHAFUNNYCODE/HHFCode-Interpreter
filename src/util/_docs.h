/**
 * @namespace Util
 * @brief A utility namespace
 * @details Namespace storing various utility classes and functions.
*/

/**
 * @fn Util::Timer::getDuration(TimerResolution t)
 * @brief Measures Timer's Duration
 * 
 * @details Measures the duration between when the timer was started and stopped
 * 
 * @param t The resolution at which the timer should output the duration
 * 
 * @returns The duration that the timer measured
 * 
 * @throw TimerRunningException Thrown if the
 *		timer is still running.
 *
 * @see start()
 * @see stop()
*/

/**
 * @fn Util::Timer::getDuration()
 * @brief Measures Timer's Duration.
 * 
 * @details Measures the duration between when the timer was started and 
 *  stopped based on the resolution stored in the timer.
 * 
 * @returns The duration that the timer measured
 * 
 * @throw TimerRunningException Thrown if the
 *  timer is still running.
 * 
 * @see start()
 * @see stop()
*/

/**
 * @fn Util::Timer::getLapTimes(TimerResolution t)
 * @brief Measures Timer's lap times.
 * 
 * @details Calculates and stores the lap times based on the time points when the timer was lapped.
 *  The output is based on the TimerResolution given
 * 
 * @param t The resolution at which to output 
 * 
 * @returns A vector containing the calculated lap times.
 * 
 * @throw TimerRunningException Thrown if the
 *		timer is still running.
 * 
 * @see lap()
*/

/**
 * @fn Util::Timer::getLapTimes()
 * @brief Measures Timer's lap times.
 * 
 * @details Calculates and stores the lap times based on the time points when the timer was lapped.
 *  The output is based on the TimerResolution stored in the Timer.
 * 
 * @returns A vector containing the calculated lap times.
 * 
 * @throw TimerRunningException Thrown if the
 *		timer is still running.
 *
 * @see lap()
*/