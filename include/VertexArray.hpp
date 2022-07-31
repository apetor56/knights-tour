#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include <glad.h>
#include <stdexcept>
#include <iostream>

//-------------------------------------------------------------//
//-------------- VertexArray class implementation -------------//
//-------------------------------------------------------------//

template<int size = 1>
class VertexArray {
public:
    VertexArray() {
        glGenVertexArrays(size, _vertexArrayID);
        glBindVertexArray(_vertexArrayID[0]);
    }

    VertexArray(const VertexArray& other) = delete;

    VertexArray& operator=(const VertexArray& other) = delete;

    void bind(unsigned int index) {
        if(index > static_cast<unsigned int>(size)) {
            throw std::out_of_range("wrong index passed to bind() function");
        }
        glBindVertexArray(_vertexArrayID[0]);
    }

    void unbind() noexcept {
        glBindVertexArray(0);
    }

    unsigned int getID(unsigned int index) const {
        if(index > static_cast<unsigned int>(size)) {
            throw std::out_of_range("wrong index passed to getID() function");
        }

        return _vertexArrayID[index];
    }

private:
    unsigned int _vertexArrayID[size];
};

#endif // VERTEX_ARRAY_HPP