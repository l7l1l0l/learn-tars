/***********************************************************************
* File Name: tc_monitor.h
* Author: RogerLuo
* mail: 13369189710@163.com
* Created Time: Mon 28 Jan 2019 05:21:54 PM CST
***********************************************************************/
#ifndef __TC_MONITOR_H
#define __TC_MONITOR_H

#include "tc_thread_mutex.h"
#include "tc_thread_cond.h"

namespace tars {
template <class T, class P>
class TC_Monitor {
public:
    typedef TC_LockT<TC_Monitor<T, P> > Lock;
    typedef TC_TryLockT<TC_Monitor<T, P> > TryLock;

    TC_Monitor() : _nnotify(0) {

    }

    virtual ~TC_Monitor() {

    }

    void lock() const {
        _mutex.lock();
        _nntoify = 0;
    }

    void unlock() const {
        notifyTmpl(_nnotify);
        _mutex.unlock();
    }

    bool tryLock() const {
        bool result = _mutex.tryLock();
        if (result) {
            _nnotify = 0; 
        }
        return result;
    }

    void wait() const {
        cout << "TC_Monitor wait" << endl;
        notifyImpl(_nnotify);

        try {
            _cond.wait(_mutex);
        } catch(...) {
            _nnotify = 0;
            throw;
        }

        _nnotify = 0;
    }

    bool timedwait(int millsecond) const {
        notifyImpl(_nnotify);

        bool rc;

        try {
            rc = _cond.timedWait(_mutex, millsecond);
        } catch (...) {
            _nnotify = 0;
            throw;
        }

        _nnotify = 0;
        return rc;
    }

    void notify() {
        if (_nnotify != -1) {
            ++_nnotify;
        }
    }

    void notifyAll() {
        _nnotify = -1;
    }

protected:
    void notifyImpl(int nnotify) const {
        if (nnotify != 0) {
            if (nnotify == -1) {
                _conn.broadcast();
                return;
            }
        } else {
            while (nnotify > 0) {
                _cond.signal();
                --nnotify;
            }
        }
    }

private:
    TC_Monitor(const TC_Monitor&);
    void operator=(const TC_Monitor&);

protected:
    mutable int _nnotify;
    mutable P _cond;
    T _mutex;
};
typedef TC_Monitor<TC_ThreadMutex, TC_ThreadCond> TC_ThreadLock;
}

#endif

