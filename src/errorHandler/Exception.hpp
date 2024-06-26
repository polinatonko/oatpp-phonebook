#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <string>
#include <string.h>

/**
 * Custom base exception class.
 */
class Exception : public std::exception {
protected:
    const char* errStr;

public:
    /**
     * Constructor with error message.
     * @param err - detailed error message.
     */
    Exception(const char* err)
      : errStr(err)
    {}

    virtual const char* what() const noexcept override { return errStr; }
};

/**
 * Dto validation exception.
 */
class ValidationException : public Exception {
public:
    ValidationException(const char* err)
      : Exception(err)
    {}
};

/**
 * Database exception.
 */
class DbException : public Exception {
public:
    DbException(const char* err)
      : Exception(err)
    {}
};

#endif /* EXCEPTION_HPP */