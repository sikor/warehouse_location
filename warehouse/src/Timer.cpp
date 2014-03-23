/*
 * Timer.cpp
 *
 *  Created on: 28-07-2013
 *      Author: pawel
 */

#include "Timer.h"
#include <time.h>

Timer::Timer(int timeLimit)
{
	this->timeLimit = timeLimit;
	late = false;
	stoped = false;
}




void Timer::startTimer()
{
	stime = time(NULL);
	stoped = false;
}


void Timer::stopTimer()
{
	stoped = true;
	late = tle();
}



Timer::Timer(const Timer & timer)
{
	this->timeLimit = timer.timeLimit;
}

Timer::Timer()
{
	this->timeLimit = 0;
}

bool Timer::tle()
{
	if(stoped){
		return late;
	}else{
		bool tle = time(NULL) - stime > timeLimit;
		return tle;
	}
}

int Timer::elapsedTime() {
	return time(NULL) - stime;
}
