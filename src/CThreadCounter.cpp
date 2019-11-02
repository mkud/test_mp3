/*
 * CThreadCounter.cpp
 *
 *  Created on: 2 nov 2019 г.
 *      Author: maxx
 */

#include "CThreadCounter.h"

CThreadCounter::CThreadCounter() :
		m_iThreadCount(0), m_mutexLock(PTHREAD_MUTEX_INITIALIZER) {
	pthread_cond_init(&m_condVar, NULL);

}

void CThreadCounter::IncrementCount() {
	pthread_mutex_lock(&m_mutexLock);
	m_iThreadCount++;
	pthread_mutex_unlock(&m_mutexLock);
}

void CThreadCounter::DecrementCount() {
	pthread_mutex_lock(&m_mutexLock);
	m_iThreadCount--;
	if (!m_iThreadCount)
		pthread_cond_signal(&m_condVar);
	pthread_mutex_unlock(&m_mutexLock);
}

void CThreadCounter::WaitFinish() {
	pthread_mutex_lock(&m_mutexLock);
	while (m_iThreadCount)
		pthread_cond_wait(&m_condVar, &m_mutexLock);
	pthread_mutex_unlock(&m_mutexLock);
}

CThreadCounter::~CThreadCounter() {
	pthread_mutex_destroy(&m_mutexLock);
	pthread_cond_destroy(&m_condVar);
}

