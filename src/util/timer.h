#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <vector>

namespace Util{

class Timer{
    public:
    enum TimerResolution{ //Orders of magnitude for time
        SECONDS,
        MILLI,
        MICRO,
        NANO
    };

    class TimerRunningException; //Runs if the timer is running and an invalid operation is performed
    class TimerFinishedException; //Runs if the timer is not running and an invalid operation is performed

    //Member variables
    private:
    std::chrono::high_resolution_clock::time_point startpoint, end; //The start of the timer, end of timer;
    std::vector< std::chrono::high_resolution_clock::time_point> laps; //Times when the timer was lapped
    bool running;
    TimerResolution resolution;

    public:
    Timer():
        running(false),resolution(MILLI){}

    Timer(TimerResolution res):
        running(false), resolution(res){}
        
    void start(); //Can only run start and stop once.
    void stop();
    void lap(); //Adds to lap counter

    double getDuration(TimerResolution);
    inline double getDuration(){return getDuration(resolution);}
    
    std::vector<double> getLapTimes(TimerResolution);
    inline std::vector<double> getLapTimes(){return getLapTimes(resolution);}

    inline void setResolution(TimerResolution t){resolution = t;}
};

} //Namespace Util end

#endif //TIMER_H