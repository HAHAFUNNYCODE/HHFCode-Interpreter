#include "timer.h"

class TimerRunningException{
    const char* what () const throw() {
        return "The timer is currently running";
    }
};

class TimerFinishedException{
    const char* what () const throw() {
        return "The timer is not currently running";
    }
};

void Timer::start(){
    if(running){ //Starting a running timer is invalid
        throw TimerRunningException();
    }
    running = true;
    startpoint = std::chrono::high_resolution_clock::now();
}

void Timer::stop(){
    if(!running){ //Stopping an inactive timer is invalid
        throw TimerFinishedException();
    }
    end = std::chrono::high_resolution_clock::now();
    running = false;
}

void Timer::lap(){
    laps.push_back(std::chrono::high_resolution_clock::now());
}

double Timer::getDuration(TimerResolution t){
    if(running) //Getting duration of running timer is bad
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

std::vector<double> Timer::getLapTimes(TimerResolution t){
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