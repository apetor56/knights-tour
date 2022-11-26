#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <iostream>
#include <exception>

class Exception : public std::exception {
public:
    Exception(int line, const char *file) noexcept;

    virtual const char *getType() const = 0;

    const std::string& getFile() const noexcept;

    const int& getLine() const noexcept;

private:
    int _line;
    std::string _file;

protected:
    mutable std::string whatBuffer;
};

#endif // EXCEPTION_HPP