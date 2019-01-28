/***********************************************************************
* File Name: tc_thread_mutex.h
* Author: RogerLuo
* mail: 13369189710@163.com
* Created Time: Mon 28 Jan 2019 05:22:55 PM CST
***********************************************************************/
#ifndef __TC_THREAD_MUTEX_H
#define __TC_THREAD_MUTEX_H


#include <iostream>
#include <pthread.h>

using namespace std;

namespace tars {
class TC_ThreadCond;
class TC_ThreadMutex {
public:
    TC_ThreadMutex();
    virtual ~TC_ThreadMutex();

    void lock() const;

    void tryLock() const;

    void unlock() const;

    bool willUnlock() const {
        return true;
    }

protected:
    TC_ThreadMutex(const TC_ThreadMutex&);
    void operator=(const TC_ThreadMutex&);

    int count() const;

    void count(int c) const;

    friend class TC_ThreadCond;

    mutable pthread_mutex_t _mutex;
};
}

#endif
