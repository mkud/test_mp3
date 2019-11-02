/*
 * CProtectedList.h
 *
 *  Created on: 1 nov 2019 г.
 *      Author: maxx
 */

#ifndef SRC_CPROTECTEDLIST_H_
#define SRC_CPROTECTEDLIST_H_
#include <list>
#include <string>
#include <pthread.h>

using namespace std;

class CProtectedList {
	list<string> m_listOfStrings;
	pthread_mutex_t m_mutexLock;
	string m_szDirectory;
public:
	CProtectedList(string szDirectory);
	virtual ~CProtectedList();
	void AppendValueUnsafe(string szValue);
	int GetNextValueSafe(string &retVal);

	string GetDirectory() {
		return m_szDirectory;
	}
};

#endif /* SRC_CPROTECTEDLIST_H_ */
