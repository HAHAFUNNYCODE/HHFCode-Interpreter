#include "timer.h"

namespace Util{

/** @brief A Timer exception.
    @details An exception thrown by a Timer when an invalid action 
        is taken while the Timer is running.*/
class Timer::TimerRunningException{
    const char* what () const throw() {
        return "The timer is currently running";
    }
};

/** @brief A Timer exception.
    @details An exception thrown by a Timer when an invalid action 
        is taken while the Timer is not running.*/
class Timer::TimerFinishedException{
    const char* what () const throw() {
        return "The timer is not currently running";
    }
};

/**
 * Starts the timer object
 * 
 * @throw TimerRunningException Thrown if the
 *		timer is already running.
*/
void Timer::start(){
    if(running){ //Starting a running timer is invalid
        throw TimerRunningException();
    }
    running = true;
    startpoint = std::chrono::high_resolution_clock::now();
}

/**
 * Stops the timer object
 * 
 * @throwTimerFinishedException Thrown if the
 *		timer is not running.
*/
void Timer::stop(){
    if(!running){ //Stopping an inactive timer is invalid
        throw TimerFinishedException();
    }
    end = std::chrono::high_resolution_clock::now();
    running = false;
}

/**
 * Marks a lap for the timer
*/
void Timer::lap(){
    laps.push_back(std::chrono::high_resolution_clock::now());
}


double Timer::getDuration(TimerResolution t){
    if(running)
        throw TimerRunningException();

    switch (t)
    {
        case SECONDS:
            return (end - startpoint).count();
        case MILLI:
            return std::chrono::duration<double, std::milli>(end - startpoint).count();
        case MICRO:
            return std::chrono::duration<double, std::micro>(end - startpoint).count();
        case NANO:
            return std::chrono::duration<double, std::nano>(end - startpoint).count();
    }

    return -1;
}

//Returns the lap time durations between time points made from Timer::lap.
std::vector<double> Timer::getLapTimes(TimerResolution t){
    if(running)
        throw TimerRunningException();

    std::vector<double> lapDur;
    for(size_t i = 0; i <= laps.size(); i++){
        auto curLap = (i == laps.size())?end:laps[i];
        auto prevLap = (i == 0)?startpoint:laps[i-1];
        switch (t)
        {
            case SECONDS:
                 lapDur.push_back((curLap - prevLap).count());
                 break;
            case MILLI:
                lapDur.push_back(std::chrono::duration<double, std::milli>(curLap - prevLap).count());
                break;
            case MICRO:
                lapDur.push_back(std::chrono::duration<double, std::micro>(curLap - prevLap).count());
                break;
            case NANO:
                lapDur.push_back(std::chrono::duration<double, std::nano>(curLap - prevLap).count());
                break;
        }
    }

    return lapDur;
}

} //Namespace Util end