//
// Created by Libro on 2021/1/20.
//

#include "error.h"
using namespace std;

/* Definitions for the ErrorException class */

ErrorException::ErrorException(const string& msg) {
    this->msg = msg;
}

ErrorException::~ErrorException() throw () {
    /* Empty */
}

string ErrorException::getMessage() const {
    return msg;
}

const char *ErrorException::what() const throw () {
    return ("Error: " + msg).c_str();
}

void error(const string& msg) {
    throw ErrorException(msg);
}