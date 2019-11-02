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

	//The directory is used simply to simplify transferring it to the working threads.
	string m_szDirectory;
public:
	CProtectedList(string szDirectory);
	virtual ~CProtectedList();

	//Not need to be safe - used only in the main thread
	void AppendValueUnsafe(string szValue);

	//Concurent part
	int GetNextValueSafe(string &retVal);

	string GetDirectory() {
		return m_szDirectory;
	}
};

#endif /* SRC_CPROTECTEDLIST_H_ */
