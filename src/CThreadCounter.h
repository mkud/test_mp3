/*
 * CThreadCounter.h
 *
 *  Created on: 2 nov 2019 г.
 *      Author: maxx
 */

#ifndef SRC_CTHREADCOUNTER_H_
#define SRC_CTHREADCOUNTER_H_

#include <pthread.h>

class CThreadCounter {
	int m_iThreadCount;
	pthread_cond_t m_condVar;
	pthread_mutex_t m_mutexLock;
public:
	CThreadCounter();
	virtual ~CThreadCounter();

	void DecrementCount();
	void IncrementCount();

	// releases when all threads have been completed
	void WaitFinish();
};

#endif /* SRC_CTHREADCOUNTER_H_ */
