#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <glad.h>
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

    // manage attributes information on GPU side
    void vertexAttrib(Attributes attrib, AttributeLayout<T>& layout) {
        int stride = layout.getStride();
        void *offset = layout.getOffset(attrib);
        const int& attribSize = vec[attrib];

        glEnableVertexAttribArray(attrib);
        glVertexAttribPointer(attrib, attribSize, dataType(), GL_FALSE, stride, offset);
    }

    void enableAttrib(int index) {
        glEnableVertexAttribArray(index);
    }

    void disableAttrib(int index) {
        glDisableVertexAttribArray(index);
    }

private:
    unsigned int _bufferID[size];
    unsigned int _currentBound;

    // returns gl macro depended on data
    // type store in buffer
    // float -> GL_FLOAT
    // int -> GL_INT ...
    constexpr int dataType() {
        const std::type_info& type = typeid(T);
        
        if(type == typeid(float)) {
            return GL_FLOAT;
        }
        if(type == typeid(unsigned int)) {
            return GL_UNSIGNED_INT;
        }
        
        return -1;
    }
};

#endif // BUFFER_HPP