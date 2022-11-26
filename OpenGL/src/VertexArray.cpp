#include <GLAD/glad.h>
#include <stdexcept>
#include <iostream>

#include "VertexArray.hpp"

//-------------------------------------------------------------//
//-------------- VertexArray class implementation -------------//
//-------------------------------------------------------------//

VertexArray::VertexArray() {
    glGenVertexArrays(1, &_vertexArrayID);
    glBindVertexArray(_vertexArrayID);
}

void VertexArray::bind() const {
    glBindVertexArray(_vertexArrayID);
}

void VertexArray::unbind() const noexcept {
    glBindVertexArray(0);
}

unsigned int VertexArray::getID() const {
    return _vertexArrayID;
}