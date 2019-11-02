/*
 * CThreadCounter.cpp
 *
 *  Created on: 2 нояб. 2019 г.
 *      Author: maxx
 */

#include "CThreadCounter.h"

CThreadCounter::CThreadCounter() :
		threadCount(0), m_mutexLock(PTHREAD_MUTEX_INITIALIZER) {
	pthread_cond_init(&m_condVar, NULL);

}

void CThreadCounter::IncrementCount(){
	pthread_mutex_lock(&m_mutexLock);
	threadCount++;
	pthread_mutex_unlock(&m_mutexLock);
}

void CThreadCounter::DecrementCount(){
	pthread_mutex_lock(&m_mutexLock);
	threadCount--;
	if (!threadCount)
		pthread_cond_signal(&m_condVar);
	pthread_mutex_unlock(&m_mutexLock);
}

void CThreadCounter::WaitFinish(){
	pthread_mutex_lock(&m_mutexLock);
	while(threadCount)
		pthread_cond_wait(&m_condVar, &m_mutexLock);
	pthread_mutex_unlock(&m_mutexLock);
}

CThreadCounter::~CThreadCounter() {
	pthread_mutex_destroy(&m_mutexLock);
}

