//
//  FTime.cpp
//  gaspunk
//
//  Created by Frank Collebrusco on 5/31/22.
//

#include "FTime.h"



#include <chrono>
#include <ctime>


std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
auto duration = now.time_since_epoch();
auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
double epoch_to_launch_nanos = nanoseconds.count();

double nanos(){
    now = std::chrono::system_clock::now();
    duration = now.time_since_epoch();
    nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
    return (double)nanoseconds.count() - epoch_to_launch_nanos;
}
double micros(){
    now = std::chrono::system_clock::now();
    duration = now.time_since_epoch();
    nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
    return ((double)nanoseconds.count() - epoch_to_launch_nanos) / 1000;
}
double millis(){
    now = std::chrono::system_clock::now();
    duration = now.time_since_epoch();
    nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
    return ((double)nanoseconds.count() - epoch_to_launch_nanos) / 1000000;
}
double seconds(){
    now = std::chrono::system_clock::now();
    duration = now.time_since_epoch();
    nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
    return ((double)nanoseconds.count() - epoch_to_launch_nanos) / 1000000000;
}





