/***********************************************************************
* File Name: tc_thread_cond.cpp
* Author: RogerLuo
* mail: 13369189710@163.com
* Created Time: Mon 28 Jan 2019 06:46:30 PM CST
***********************************************************************/
#include "tc_thread_cond.h"
#include <string.h>
#include <cassert>
#include <iostream>
#include <stdint.h>

using namespace std;

namespace tars {
TC_ThreadCond::TC_ThreadCond() {
    int rc;
    pthread_condattr attr;

    rc = pthread_condattr_init(&attr);

    if (rc != 0) {
        cout << "[TC_ThreadCond::TC_ThreadCond] pthread_condattr_init error" << endl;
    }

    rc = pthread_cond_init(&_cond, &attr);
    if (rc != 0) {
        cout << "[TC_ThreadCond] pthread_conn_init error" << endl;
    }

    rc = pthread_condattr_destroy(&attr);
    if (rc != 0) {
        cout << "[TC_ThreadCond::TC_ThreadCond] pthread_destory error" << endl;
    }
}

TC_ThreadCond::~TC_ThreadCond() {
    int rc = 0;
    rc = pthread_cond_destory(&_cond);
    if (rc != 0) {
        cout << "[TC_ThreadCond::~TC_ThreadCond] pthread_cond_destory error: " << string(strerror(rc) << endl;
    }
}

TC_ThreadCond::signal() {
    int rc = pthread_cond_signal(&_cond);
    if (rc != 0) {
        cout << "[TC_ThreadCond::signal] pthread_cond_signal error" << endl;
    }
}

void TC_ThreadCond::broadcast() {
    int rc = pthread_cond_broadcast(&_cond);
    if (rc != 0) {
        cout << "[TC_ThreadCond::broadcast] pthread_cond_broadcast error" << endl;
    }
}

timespec TC_ThreadCond::abstime(int millsecond) const {
    struct timeval tv;

    gettimeofday(&tv, 0);

    int64_t it = tv.tv_sec * (int64_t)1000000 + tv.tv_usec + (int64_t)millsecond * 1000;

    tv.tv_sec = it / (int64_t)1000000;
    tv.tv_usec = it % (int64_t)1000000;

    timespec ts;
    ts.tv_sec = tv.tv_sec;
    ts.tv_nsec = tv.tv_usec * 1000;
    return ts;
}

}

