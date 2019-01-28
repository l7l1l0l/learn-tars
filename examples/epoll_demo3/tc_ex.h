/***********************************************************************
* File Name: tc_ex.h
* Author: RogerLuo
* mail: 13369189710@163.com
* Created Time: Mon 28 Jan 2019 05:02:37 PM CST
***********************************************************************/
#ifndef __TC_EX_H
#define __TC_EX_H

#include <stdexcept>

using namespace std;

namespace tars {

class TC_Exception : public exception {
public:
    explicit TC_Exception(const string& buffer);
    TC_Exception(const string& buffer, int err);

    virtual ~TC_Exception() throw();

    virtual const char* what() const throw();

    int getErrCode() {
        return _code;
    }
private:
    void getBacktrace();

private:
    string _buffer;
    int _code;
};
}

#endif

