/*
 * Timer.h
 *
 *  Created on: 28-07-2013
 *      Author: pawel
 */

#ifndef TIMER_H_
#define TIMER_H_

class Timer {
public:
	Timer(int timeLimit);
	Timer(const Timer& timer);
	Timer();
	void startTimer();
	void stopTimer();
	bool tle();
	int elapsedTime();

	int timeLimit;
private:

	bool late;
	bool stoped;
	int stime;
};

#endif /* TIMER_H_ */
