#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Buffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"

class Renderer {
public:
    Renderer(int size = 5);

    Renderer(const Renderer& other) = delete;

    Renderer& operator=(const Renderer& other) = delete;

    void setChessBoard(int startPos = 0);

    void drawChessBoard() const;

    void drawKnight(int move_x, int move_y);

private:
    int _size;
    int _lineCounter;
    int _startPos;
    struct Graphics {
        Graphics(const char *vertexPath, const char *fragmentPath);
        Buffer<GL_ARRAY_BUFFER, float> vbo;
        Buffer<GL_ELEMENT_ARRAY_BUFFER, unsigned int> ebo;
        VertexArray vao;
        Shader vertexShader;
        Shader fragmentShader;
        unsigned int program;
        float *vertices;
        unsigned int *indices;
    } _chessboard, _knight, _line;


    void generateChessData();

    void setSquare(const float& x, const float& y, const float& colorFlag) const;

    void setKnightData(float start_x, float start_y, const float& length);

    void setChessIndices();

    void initializeLine(float x, float y);

    void updateLine(float x, float y);

    void setLineIndexes();
};

#endif // RENDERER_HPP