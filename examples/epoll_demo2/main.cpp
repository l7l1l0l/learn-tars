/***********************************************************************
* File Name: main.cpp
* Author: RogerLuo
* mail: 13369189710@163.com
* Created Time: Fri 25 Jan 2019 04:24:52 PM CST
***********************************************************************/
#include "tc_epoll_server.h"

using namespace std;
using namespace tars;

int main () {
    NetThread* vNetThread = new NetThread();
    string ip = "127.0.0.1";
    int port = 9710;
    vNetThread->bind(ip, port);
    vNetThread->createEpoll(1);
    vNetThread->run();

    delete vNetThread;
    return 0;
}

