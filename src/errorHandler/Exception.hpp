#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <string>
#include <string.h>

class Exception : public std::exception {
protected:

    const char* errStr;

public:

    Exception(const char* err)
    : errStr(err) {}

    virtual const char* what() const noexcept override { return errStr; }
};


class ValidationException : public Exception {
public:

    ValidationException(const char* err)
    : Exception(err) {}
};

class DbException : public Exception {
public:

    DbException(const char* err)
    : Exception(err) {}
};

#endif /* EXCEPTION_HPP */