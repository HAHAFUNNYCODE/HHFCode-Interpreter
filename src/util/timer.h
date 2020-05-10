#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <vector>

namespace Util{

/**
 * Allows for timing durations between starting and stopping.
 * 
 * A timer starts when Timer::start() is called and ends when
 * Timer::stop() is called.
*/
class Timer{
    public:
    ///Resolutions at which the timer is able to measure time.
    enum TimerResolution{
        SECONDS,
        MILLI,
        MICRO,
        NANO
    };

    class TimerRunningException; //See timer.cpp for documentaion
    class TimerFinishedException; //See timer.cpp for documentaion

    //Member variables
    private:
    std::chrono::high_resolution_clock::time_point startpoint, end; //The start of the timer, end of timer;
    std::vector< std::chrono::high_resolution_clock::time_point> laps; //Times when the timer was lapped
    bool running;
    TimerResolution resolution;

    public:
    ///Default constructor for a Timer. Defaults resolution to Milliseconds.
    Timer():
        running(false),resolution(MILLI){}

    /** A constructor.
     * Constructor for a Timer with set resolution.
     * @param res The resolution that the timer will have. */
    Timer(TimerResolution res):
        running(false), resolution(res){}

    void start();
    void stop();
    void lap(); //Adds to lap counter

    double getDuration(TimerResolution);
    inline double getDuration(){return getDuration(resolution);}
    
    std::vector<double> getLapTimes(TimerResolution);
    inline std::vector<double> getLapTimes(){return getLapTimes(resolution);}

    /**
     * A setter. Sets which resolution the timer stores.
     * @param t The resolution to give the timer. */
    inline void setResolution(TimerResolution t){resolution = t;}
};

} //Namespace Util end

#endif //TIMER_H