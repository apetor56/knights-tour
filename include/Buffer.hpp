#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <glad.h>
#include <iostream>
#include <typeinfo>
#include "AttributeLayout.hpp"

//-------------------------------------------------------------//
//---------------- Buffer class implementation ----------------//
//-------------------------------------------------------------//

template<unsigned int bufferType, typename T, int size = 1>
class Buffer {
public:
    // generates buffers with 'bufferType'
    // and bind first buffer
    Buffer() {
        glGenBuffers(size, _bufferID);
        glBindBuffer(bufferType, _bufferID[0]);
        _currentBound = 0;
    }

    Buffer(const Buffer& other) = delete;

    Buffer& operator=(const Buffer& other) = delete;

    // fill buffer with given data
    void setData(T *data, int dataSize) {
        glBufferData(bufferType, dataSize, data, GL_STATIC_DRAW);
    }

    void bind(unsigned int index) {
        glBindBuffer(bufferType, index);
        _currentBound = index;
    }

    void unbind() {
        glBindBuffer(bufferType, 0);
    }

private:
    unsigned int _bufferID[size];
    unsigned int _currentBound;
};

#endif // BUFFER_HPP