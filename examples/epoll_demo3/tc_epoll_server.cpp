/***********************************************************************
* File Name: tc_epoll_server.cpp
* Author: RogerLuo
* mail: 13369189710@163.com
* Created Time: Fri 25 Jan 2019 04:36:16 PM CST
***********************************************************************/
#include "tc_epoll_server.h"

#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cassert>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>


namespace tars{
#define H64(x) (((uint64_t)x) << 32)

NetThread::NetThread() {
#if 0
    int iSocketType = SOCK_STREAM;
    int iDomain = AF_INET;

    _shutdown_sock = socket(iDomain, iSocketType, 0);
    _notify_sock = socket(iDomain, iSocketType, 0);

    if (_shutdown_sock < 0) {
        cout << "_shutdown_sock invaid" << endl;
    }

    if (_notify_sock < 0) {
        cout << "_notify_sock invaid" << endl;
    }
#endif
    _shutdown_sock.createSocket();
    _notify_sock.createSocket();

    _response.response = "";
    _response.uid = 0;
}

NetThread::~NetThread() {

}

int NetThread::bind(const string& ip, int port) {
#if 0
    int iSocketType = SOCK_STREAM;
    int iDomain = AF_INET;

    _sock = socket(iDomain, iSocketType, 0);

    if (_sock < 0) {
        cout << "bind _sock invalid" << endl;
    }

    struct sockaddr_in bindAddr;
    bzero(&bindAddr, sizeof(bindAddr));

    bindAddr.sin_family = iDomain;
    bindAddr.sin_port = htons(port);

    parseAddr(ip, bindAddr.sin_addr);

    int iReuseAddr = 1;

    setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, (const void*)&iReuseAddr, sizeof(int));

    if(::bind(_sock, (struct sockaddr*)(&bindAddr), sizeof(bindAddr)) < 0) {
        cout << "bind error" << endl;
    }

    cout << "server already bind fd is " << _sock << endl;

    int iConnBackLog = 1024;
    if (::listen(_sock, iConnBackLog) < 0) {
        cout << "listen error" << endl;
    }

    cout << "server already listen fd is " << _sock << endl;

    int flag = 1;
    if (setsockopt(_sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&flag, int(sizeof(int))) == -1) {
        cout << "setkeepalive error" << endl;
    }

    flag = 1;
    if (setsockopt(_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, int(sizeof(int))) == -1) {
        cout << "[TC_Socket::setTcpNoDelay] error" << endl;
    }

    linger stLinger;
    stLinger.l_onoff = 1;
    stLinger.l_linger = 0;
    
    if (setsockopt(_sock, SOL_SOCKET, SO_LINGER, (const void*)&stLinger, sizeof(linger)) == -1) {
        cout << "[TC_Socket::setNoCloseWait] error" << endl;
    }

    int val = 0;
    bool bBlock = false;

    if ((val == fcntl(_sock, F_GETFL, 0)) == -1) {
        cout << "[TC_Socker::setblock] fcntl [F_GETFL] error" << endl;
    }

    if (!bBlock) {
        val |= O_NONBLOCK;
    } else {
        val &= ~O_NONBLOCK;
    }

    if (fcntl(_sock, F_SETFL, val) == -1) {
        cout << "fcntl nonblock error" << endl;
    }

    return _sock;
#endif
    int type = AF_INET;
    _sock.createSocket(SOCK_STREAM, type);
    _sock.bind(ip, port);
    _sock.listen(1024);

    cout << "server already listen fd is " << _sock.getfd() << endl;

    _sock.setKeepAlive();
    _sock.setTcpNoDelay();
    _sock.setNoCloseWait();
    _sock.setblock(false);

    return _sock.getfd();
}

#if 0
void NetThread::parseAddr(const std::string& addr, struct in_addr &stAddr) {
    int iRet = inet_pton(AF_INET, addr.c_str(), &stAddr);
    if (iRet < 0) {
        cout << "parseAddr iRet error" << endl;
    } else if (iRet == 0) {
        struct hostent stHostent;
        struct hostent* pstHostent;
        char buf[2048] = { 0 };
        int iError = 0;
        gethostbyname_r(addr.c_str(), &stHostent, buf, sizeof(buf), &pstHostent, &iError);
        if (pstHostent == NULL) {
            cout << "gethostbyname_r error!" << endl;
        } else {
            stAddr = *(struct in_addr*)pstHostent->h_addr;
        }
    }

}
#endif

void NetThread::createEpoll(uint32_t iIndex) {
    int total = 200000;
    _epoller.create(10240);
    _epoller.add(_shutdown_sock.getfd(), H64(ET_CLOSE), EPOLLIN);
    _epoller.add(_notify_sock.getfd(), H64(ET_NOTIFY), EPOLLIN);
    _epoller.add(_sock.getfd(), H64(ET_LISTEN)|_sock.getfd(), EPOLLIN);

    for (uint32_t i = 1; i <= total; i++) {
        _free.push_back(i);
        ++_free_size;
    }
    cout << "epoll create successsful" << endl;
}

void NetThread::run() {
    cout << "NetThread run ET_NET = " << ET_NET << endl;
    while(true) {
        int iEvNum = _epoller.wait(2000);
        for (int i = 0; i < iEvNum; ++i) {
            const epoll_event& ev = _epoller.get(i);

            uint32_t h = ev.data.u64 >> 32;

            switch (h) {
                case ET_LISTEN:
                    {
                        cout << "ET_LISTEN" << endl;
                        if (ev.events & EPOLLIN) {
                            bool ret;
                            do {
                                ret = accept(ev.data.u32);
                            } while (ret);
                        }
                    }
                    break;
                case ET_CLOSE:
                    {
                        cout << "ET_CLOSE" << endl;
                    }
                    break;
                case ET_NOTIFY:
                    {
                        cout << "ET_NOTIFY" << endl;
                        processPipe();
                    }
                    break;
                case ET_NET:
                    {
                        cout << "ET_NET" << endl;
                        processNet(ev);
                    }
                    break;
                default:
                    cout << "error >>>> " << h << endl;
                    assert(true);
            }
        }
    }
}

bool NetThread::accept(int fd) {
    struct sockaddr_in stSockAddr;
    socklen_t iSockAddrSize = sizeof(sockaddr_in);

    TC_Socket cs;
    cs.setOwner(false);

    TC_Socket s;
    s.init(fd, false, AF_INET);

    int iRetCode = s.accept(cs, (struct sockaddr*) &stSockAddr, iSockAddrSize);
    if (iRetCode > 0) {
        string ip;
        uint16_t port;
        char sAddr[INET_ADDRSTRLEN] = { 0 };
        struct sockaddr_in* p = (struct sockaddr_in*)&stSockAddr;
        inet_ntop(AF_INET, &p->sin_addr, sAddr, sizeof(sAddr));

        ip = sAddr;
        port = ntohs(p->sin_port);

        cout << "accept ip is " << ip << " port is " << port << endl;

        cs.setblock(false);
        cs.setKeepAlive();
        cs.setTcpNoDelay();
        cs.setCloseWaitDefault();

        uint32_t uid = _free.front();
        _free.pop_front();
        --_free_size;
        _listen_connect_id[uid] = cs.getfd();
        cout << "server accept successful fd id " << cs.getfd() << endl;
        _epoller.add(cs.getfd(), uid, EPOLLIN | EPOLLOUT);
    } else {
        if (errno == EAGAIN) {
            return false;
        }
    }
    return true;
#if 0
    int fd;
    while ((fd = ::accept(_sock, (struct sockaddr*)&stSockAddr, &iSockAddrSize)) < 0 && errno == EINTR);

    cout << "server accept successful fd is " << fd << endl;

    if (fd > 0) {
        string ip;
        uint64_t port;
        char sAddr[INET_ADDRSTRLEN] = { 0 };
        struct sockaddr_in* p = (struct sockaddr_in*)&stSockAddr;
        inet_ntop(AF_INET, &p->sin_addr, sAddr, sizeof(sAddr));

        ip = sAddr;
        port = ntohs(p->sin_port);

        int val = 0;
        bool bBlock = false;

        if ((val == fcntl(fd, F_GETFL, 0)) == -1) {
            cout << "F_GETFL error" << endl;
        } 

        if (!bBlock) {
            val |= O_NONBLOCK;
        } else {
            val &= ~O_NONBLOCK;
        }

        if (fcntl(fd, F_SETFL, val) == -1) {
            cout << "F_SETFL error" << endl;
        }

        int flag = 1;
        if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char*)&flag, int(sizeof(int))) == -1) {
            cout << "[TC_Socket::setKeepAlive] error" << endl;
        }

        if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, int(sizeof(int))) == -1) {
            cout << "[TC_Socket::setTcpNoDelay] error" << endl;
        }
        
        linger stLinger;
        stLinger.l_onoff = 0;
        stLinger.l_linger = 0;

        if (setsockopt(fd, SOL_SOCKET, SO_LINGER, (const void*)&stLinger, sizeof(linger)) == -1) {
            cout << "[TC_Socket::setCloseWaitDefault] error" << endl;
        }

        uint32_t uid = _free.front();
        _free.pop_front();

        --_free_size;

        _listen_connect_id[uid] = fd;
        _epoller.add(fd, uid, EPOLLIN | EPOLLOUT);
    } else {
        if (errno == EAGAIN) {
            return false;
        }
        return true;
    }
    return true;
#endif
}

void NetThread::processNet(const epoll_event& ev) {
    uint32_t uid = ev.data.u32;
    int fd = _listen_connect_id[uid];

    cout << "processNet uid is " << uid << "fd is " << fd << endl;

    if (ev.events & EPOLLERR || ev.events & EPOLLHUP) {
        cout << "should delete connection" << endl;
        return;
    }

    if (ev.events & EPOLLIN) {
        while (true) {
            char buffer [32 * 1024] = { 0 };
            int iBytes = 0;

            iBytes = ::read(fd, (void*)buffer, sizeof(buffer));
            cout << "server recieve " << iBytes << "bytes buffer is " << buffer << endl;

            if (iBytes < 0) {
                if (errno == EAGAIN) {
                    break;
                } else {
                    cout << "client close" << endl;
                    return;
                }
            } else if (iBytes == 0) {
                cout << "fd " << fd << " client close" << endl;
                return;
            }

            _recvbuffer.append(buffer, iBytes);
            _response.response = "hello";
            _response.uid = uid;

            _epoller.mod(_notify_sock.getfd(), H64(ET_NOTIFY), EPOLLOUT);
        }
    }

    if (ev.events & EPOLLOUT) {
        cout << "need to send data" << endl;
    }
}

void NetThread::processPipe() {
    uint32_t uid = _response.uid;
    int fd = _listen_connect_id[uid];

    cout << "processPipe uid is " << uid << " fd is " << fd << endl;
    int bytes = ::send(fd, _response.response.c_str(), _response.response.size(), 0);
    cout << "send byte is " << bytes << endl;
    cout << "response is " << _response.response << endl;
}
}

