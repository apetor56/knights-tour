#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP
class VertexArray {
public:
    VertexArray();

    VertexArray(const VertexArray& other) = delete;

    VertexArray& operator=(const VertexArray& other) = delete;

    void bind() const;

    void unbind() const noexcept;

    unsigned int getID() const;

private:
    unsigned int _vertexArrayID;
};

#endif // VERTEX_ARRAY_HPP