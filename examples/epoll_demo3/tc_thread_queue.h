/***********************************************************************
* File Name: tc_thread_queue.h
* Author: RogerLuo
* mail: 13369189710@163.com
* Created Time: Mon 28 Jan 2019 06:27:51 PM CST
***********************************************************************/
#ifndef __TC_THREAD_QUEUE_H_
#define __TC_THREAD_QUEUE_H_

#include <deque>
#include <vector>
#include <cassert>
#include "tc_monitor.h"
#include "tc_lock.h"

namespace tars {
template<typename T, typename D = deque<T> >
class TC_ThreadQueue : protected TC_ThreadLock {
public:
    TC_ThreadQueue() : _size(0) {

    }

public:
    typedef D queue_type;
    bool pop_front(T& t, size_t millsecond = 0);

    void notifyT();
    void push_back(const T& t);
    void push_front(const T& t);
    void push_front(const queue_type& qt);

    bool swap(queue_type& q, size_t millsecond = 0);

    size_t size() const;

    void clear();

    bool empty() const;

protected:
    queue_type _queue;
    size_t _size;
};

template<typename T, typename D> bool TC_ThreadQueue<T, D>::pop_front(T& t, size_t millsecond) {
    Lock lock(*this);

    if (_queue.empty()) {
        if (millsecond == 0) {
            return false;
        }
        if (millsecond == (size_t)-1) {
            cout << "pop_front wait begin" << endl;
            wait();
            cout << "pop_front wait end" << endl;
        } else {
            cout << "pop_front timedwait begin" << endl;
            if (!timedWait(millsecond)) {
                return false;
            }
        }
    }

    if (_queue.empty()) {
        return false;
    }

    t = _queue.front();
    _queue.pop_front();
    assert(_size > 0);
    --_size;
    return true;
}

template<typename T, typename D> void TC_ThreadQueue<T, D>::notify() {
    Lock lock(*this);
    notifyAll();
}

template<typename T, typename D> void TC_ThreadQueue<T, D>::push_back(const T& t) {
    Lock lock(*this);

    notify();

    _queue.push_back(t);

    ++_size();
}

template<typename T, typename D> void TC_ThreadQueue<T, D>::push_back(const queue_type& qt) {
    Lock lock(*this);

    typename queue_type::const_iterator it = qt.begin();
    typename queue_type::const_iterator itEnd = qt.End();

    while (it != itEnd) {
        _queue.push_back(*it);
        ++it;
        ++_size;
        notify();
    }
}

template<typename T, typename D> void TC_ThreadQueue<T, D>::push_front(const T& t) {
    Lock lock(*this);

    notify();

    _queue.push_front(t);

    ++_size;
}

template<typename T, typename D> void TC_ThreadQueue<T, D>::push_front(const queue_type& qt) {
    Lock lock(*this);

    typename queue_type::const_iterator it = qt.begin();
    typename queue_type::const_iterator itEnd = qt.End();

    while (it != itEnd) {
        _queue.push_front(*it);
        ++it;
        ++_size;
        notify();
    }
}

template<typename T, typename D> bool TC_ThreadQueue<T, D>::swap(queue_type& q, size_t millsecond) {
    Lock lock(*this);
    if (_queue.empty()) {
        if (millsecond == 0) {
            return false;
        }
        if (millsecond == (size_t)-1) {
            wait();
        } else {
            if (!timedWait(millsecond)) {
                return false;
            }
        }
    }

    if (_queue.empty()) {
        return false;
    }

    q.swap(_queue);
    _size = _queue.size();
    return true;
}

template<typename T, typename D> size_t TC_ThreadQueue<T, D>::size() const {
    Lock lock(*this);
    return _size;
}

template<typename T, typename D> void TC_ThreadQueue<T, D>::clear() {
    Lock lock(*this);
    _queue.clear();
    _size = 0;
}

template<typename T, typename D> bool TC_ThreadQueue<T, D>::empty() {
    Lock lock(*this);
    return _queue.empty();
}

}


#endif

