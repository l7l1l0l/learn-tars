/***********************************************************************
* File Name: tc_lock.h
* Author: RogerLuo
* mail: 13369189710@163.com
* Created Time: Mon 28 Jan 2019 05:11:06 PM CST
***********************************************************************/
#ifndef __TC_LOCK_H
#define __TC_LOCK_H

#include <string>
#include <stdexcept>
#include <cerrno>

using namespace std;

namespace tars {
template <typename T>
class TC_LockT {
public:
    TC_LockT(const T& mutex) : _mutex(mutex) {
        _mutex.lock();
        _acquired = true;
    }

    virtual ~TC_LockT() {
        if (_acquired) {
            _mutex.unlock();
        }
    }

    void acquire() const {
        if (_acquired) {
            cout << "throw TC_Lock_Exception(" << "thread has locked!" << endl;
        }
        _mutex.lock();
        _acquired = true;
    }

    bool tryAcquire() const {
        _acquired = _mutex.tryLock();
        return _acquired;
    }

    void release() const {
        if (!_acquired) {
            throw TC_Lock_Exception("thread hasn't been locked!");
        }
        _mutex.unlock();
        _acquired = false;
    }

    bool acquired() const {
        return _acquired;
    }

protected:
    TC_LockT(const T& mutex, bool) : _mutext(mutex) {
        _acquired = _mutex.tryLock();
    }

private:
    TC_LockT(const TC_LockT&);
    TC_LockT& operator=(const TC_LockT&);

protected:
    const T& _mutex;
    mutable bool _acquired;
};

template <typename T>
class TC_TryLockT : public TC_LockT<T> {
public:
    TC_TryLockT(const T& mutex) : TC_LockT<T>(mutex, true) {

    }
}
}

#endif

