/***********************************************************************
* File Name: tc_ex.cpp
* Author: RogerLuo
* mail: 13369189710@163.com
* Created Time: Mon 28 Jan 2019 05:06:03 PM CST
***********************************************************************/
#include <execinfo.h>
#include <string.h>
#include <stdlib.h>
#include <cerrno>

#include "tc_ex.h"

namespace tars {
TC_Exception::TC_Exception(const string& buffer) : _buffer(buffer), _code(0) {

} 

TC_Exception::TC_Exception(const string& buffer, int err) : _buffer(buffer), _code(err) {

}

TC_Exception::~TC_Exception() throw() {

}

const char* TC_Exception::what() const throw() {
    return _buffer.c_str();
}

void TC_Exception::getBacktrace() {
    void* array[64];
    int nSize = backtrace(array, 64);
    char** symbols = backtrace_symbols(array, nSize);

    for (int i = 0; i < nSize; i++) {
        _buffer += symbols[i];
        _buffer += "\n";
    }
    free(symbols);
}

}


