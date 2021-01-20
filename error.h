//
// Created by Libro on 2021/1/20.
//

#ifndef SRC_ERROR_H
#define SRC_ERROR_H


#include <string>
#include <exception>

class ErrorException : public std::exception {
public:
    ErrorException(const std::string& msg="");
    virtual ~ErrorException() throw ();
    virtual std::string getMessage() const;
    virtual const char *what() const throw ();

private:
    std::string msg;
};


void error(const std::string& msg="");


#endif //SRC_ERROR_H
