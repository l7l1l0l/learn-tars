/***********************************************************************
* File Name: tc_thread_cond.h
* Author: RogerLuo
* mail: 13369189710@163.com
* Created Time: Mon 28 Jan 2019 05:27:33 PM CST
***********************************************************************/
#ifndef __TC_THREAD_COND_H
#define __TC_THREAD_COND_H

#include <sys/time.h>
#include <cerrno>
#include <iostream>
#include <assert.h>
#include <pthread.h>

using namespace std;

namespace tars {

class TC_ThreadMutex;

class TC_ThreadCond {
public:
    TC_ThreadCond();
    ~TC_ThreadCond();

    void signal();

    void broadcast();

    timespec abstime(int millsecond) const;

    template<typename Mutex> void wait(const Mutex& mutex) const {
        int c = mutex.count();
        int rc = pthread_conn_wait(&_cond, &mutex._mutex);
        mutex.count(c);
        if (rc != 0) {
            cout << "[TC_ThreadCond::wait] pthread_conn_wiat error" << endl;
        }
    }

    template<typename Mutex> bool timedWait(const Mutex& mutex, int millsecond) const {
        int c = mutex.count();
        timespec ts = abstime(millsecond);

        int rc = pthread_cond_timedwait(&_cond, &mutex._mutex, &ts);

        mutex.count(c);

        if (rc != 0) {
            if (rc != ETIMEDOUT) {
                cout << "[TC_ThreadCond::timedWait] pthread_cond_timedwait error" << endl;
            }
            return false;
        }
        return true;
    }

protected:
    TC_ThreadCond(const TC_ThreadCond&);
    TC_ThreadCond& operator=(const TC_Thread&);

private:
    mutable pthread_cond_t _cond;
};
}


#endif

