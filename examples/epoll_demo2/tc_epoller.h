/***********************************************************************
* File Name: tc_epoller.h
* Author: RogerLuo
* mail: 13369189710@163.com
* Created Time: Fri 25 Jan 2019 04:10:12 PM CST
***********************************************************************/
#ifndef __TC_EPOLLER_H_
#define  __TC_EPOLLER_H_

#include <sys/epoll.h>
#include <cassert>

namespace tars {

class TC_Epoller {
public:
    TC_Epoller(bool bEt = true);

    ~TC_Epoller();

    void create(int max_connections);

    void add(int fd, long long data, __uint32_t event);

    void mod(int fd, long long data, __uint32_t event);

    void del(int fd, long long data, __uint32_t event);

    int wait(int millsecond);

    struct epoll_event& get(int i) {
        assert(_pevs != 0);
        return _pevs[i];
    }

protected:
    void ctrl(int fd, long long data, __uint32_t events, int op);

protected:
    int _iEpollfd;
    int _max_connections;
    struct epoll_event* _pevs;

    bool _et;
};
}

#endif
