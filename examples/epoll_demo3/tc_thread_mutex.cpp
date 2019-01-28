/***********************************************************************
* File Name: tc_thread_mutex.cpp
* Author: RogerLuo
* mail: 13369189710@163.com
* Created Time: Mon 28 Jan 2019 06:55:51 PM CST
***********************************************************************/
#include "tc_thread_mutex.h"

#include <string.h>
#include <iostream>
#include <cassert>
#include <errno.h>

namespace tars {
TC_ThreadMutex::TC_ThreadMutex() {
    int rc;
    pthread_mutexattr_t attr;
    rc = pthread_mutexattr_init(&attr);

    assert(rc == 0);

    rc = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
    assert(rc == 0);

    rc = pthread_mutex_init(&_mutex, &attr);
    assert(rc == 0);

    rc = pthread_mutexattr_destory(&attr);
    assert(rc == 0);

    if (rc != 0) {
        cout << "[TC_ThreadMutex::TC_ThreadMutex] pthread_mutexattr_init error" << endl;
    }
}

TC_ThreadMutex::~TC_ThreadMutex() {
    int rc = 0;
    rc = pthread_mutex_destory(&_mutex);
    if (rc != 0) {
        cout << "[TC_ThreadMutex::~TC_ThreadMutex] pthread_mutex_destory error:" << string(strerror(rc)) << endl;
    }
}

void TC_ThreadMutex::lock() const {
    int rc = pthread_mutex_lock(&_mutex);

    if (rc != 0) {
        if (rc == EDEADLK) {
            cout << "[TC_ThreadMutex::lock] pthread_mutex_lock error: " << rc << endl;
        } else {
            cout << "[TC_ThreadMutex::lock] pthread_mutex_lock error" << endl;
        }
    }
}

bool TC_ThreadMutex::tryLock() const {
    int rc = pthread_mutex_trylock(&_mutex);

    if (rc != 0 && rc != EBUSY) {
        if (rc == EDEADLK) {
            cout << "[TC_ThreadMutex::lock] pthread_mutex_trylock error: " << rc << endl;
        } else {
            cout << "[TC_ThreadMutex::lock] pthread_mutex_trylock error" << endl;
        }
    }
}

void TC_ThreadMutex::unlock() const {
    int rc = pthread_mutex_unlock(&_mutex);

    if (rc != 0) {
        cout << "[TC_ThreadMutex::lock] pthread_mutex_unlock error" << endl;
    }
}

int TC_ThreadMutex::count() const {
    return 0;
}

void TC_ThreadMutex::count(int c) const {

}

} 

