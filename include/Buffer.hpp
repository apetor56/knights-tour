#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <glad/glad.h>
#include <iostream>
#include <typeinfo>
#include "AttributeLayout.hpp"

//-------------------------------------------------------------//
//---------------- Buffer class implementation ----------------//
//-------------------------------------------------------------//

template<unsigned int bufferType, typename T>
class Buffer {
public:
    // generates buffer with 'bufferType'
    // and bind it
    Buffer() {
        glGenBuffers(1, &_bufferID);
        glBindBuffer(bufferType, _bufferID);
    }

    Buffer(const Buffer& other) = delete;

    Buffer& operator=(const Buffer& other) = delete;

    // fill currently bound buffer with given data
    static void setData(T *data, int dataSize);

    void bind() {
        glBindBuffer(bufferType, _bufferID);
    }

    void unbind() {
        glBindBuffer(bufferType, 0);
    }

private:
    unsigned int _bufferID;
};

template<unsigned int bufferType, typename T>
void Buffer<bufferType, T>::setData(T *data, int dataSize) {
    glBufferData(bufferType, dataSize, data, GL_STATIC_DRAW);
}

#endif // BUFFER_HPP