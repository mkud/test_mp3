/*
 * CThreadCounter.h
 *
 *  Created on: 2 нояб. 2019 г.
 *      Author: maxx
 */

#ifndef SRC_CTHREADCOUNTER_H_
#define SRC_CTHREADCOUNTER_H_

#include <pthread.h>

class CThreadCounter {
	int threadCount;
	pthread_cond_t m_condVar;
	pthread_mutex_t m_mutexLock;
public:
	CThreadCounter();
	virtual ~CThreadCounter();
	void DecrementCount();
	void IncrementCount();
	void WaitFinish();
};

#endif /* SRC_CTHREADCOUNTER_H_ */
