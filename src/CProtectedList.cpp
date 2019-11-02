/*
 * CProtectedList.cpp
 *
 *  Created on: 1 нояб. 2019 г.
 *      Author: maxx
 */

#include "CProtectedList.h"

CProtectedList::CProtectedList(string szDirectory) :
		m_mutexLock(PTHREAD_MUTEX_INITIALIZER), m_listOfStrings(), m_szDirectory(szDirectory) {

}

CProtectedList::~CProtectedList() {
	pthread_mutex_destroy(&m_mutexLock);
}

void CProtectedList::AppendValueUnsafe(string szValue) {
	m_listOfStrings.push_back(szValue);
}

int CProtectedList::GetNextValueSafe(string &retVal) {
	pthread_mutex_lock(&m_mutexLock);
	if (m_listOfStrings.empty()) {
		pthread_mutex_unlock(&m_mutexLock);
		return 0;
	}
	retVal = m_listOfStrings.front();
	m_listOfStrings.pop_front();
	pthread_mutex_unlock(&m_mutexLock);
	return 1;
}
