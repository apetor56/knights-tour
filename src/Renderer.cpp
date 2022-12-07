#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

#include "Renderer.hpp"
#include "Texture2D.hpp"
#include "Paths.hpp"

//-------------------------------------------------------------//
//-------------- Renderer class implementation ----------------//
//-------------------------------------------------------------//

/* Takes shaders' source files path and creates shader program. */
Renderer::Graphics::Graphics(const char *vertexPath, const char *fragmentPath)
    : vertexShader(GL_VERTEX_SHADER, vertexPath),
      fragmentShader(GL_FRAGMENT_SHADER, fragmentPath),
      program(Shader::createProgramShader(vertexShader, fragmentShader))
{}

/* Initialize chessboard's size and graphics objects. */
Renderer::Renderer(int size) 
:   _size(size),
    _chessboard(vertexPath_chess, fragmentPath_chess),
    _knight(vertexPath_knight, fragmentPath_knight),
    _line(vertexPath_line, fragmentPath_line)
{}

/* Set VAO, VBO, EBO and shaders for chessboard's data generated for. */
void Renderer::setChessBoard(int startPos) {
    _startPos = startPos;

    /* generates vertex and index data for chessboard */
    generateChessData();

    /* binding objects and sending data to buffers */
    _chessboard.vao.bind();
    _chessboard.vbo.bind();
    _chessboard.vbo.setData(_chessboard.vertices, sizeof(float) * _size * _size * 12);
    _chessboard.ebo.bind();
    _chessboard.ebo.setData(_chessboard.indices, _size * _size * 6 * sizeof(unsigned int));

    /* setting appropriate attributes and vertex attrib pointer */
    AttributeLayout<float> layout(POSITION | COLOR);
    layout.setAttribSize(Position, 2);
    layout.setAttribSize(Color, 1);
    Shader::vertexAttrib(Position, layout);
    Shader::vertexAttrib(Color, layout);
}

/* Draws chessboard. */
void Renderer::drawChessBoard() const {
    const int indicesNum = _size * _size * 6; 
    _chessboard.vao.bind();
    Shader::useProgram(_chessboard.program);
    glDrawElements(GL_TRIANGLES, indicesNum, GL_UNSIGNED_INT, (void*)0);
    _chessboard.vao.unbind();
}

/* Calculates square's position depending on given coordinates.
(x,y) in function's signature -> lefl upper corner of square. */
void Renderer::setSquare(const float& x, const float& y, const float& colorFlag) const {
    // side's length
    float length = 2.0f / _size;

    /* this values tell us, how to move given x and y to calculete square's coordinates
    1 -> to upper left corner, 2 -> to upper rigth corner,
    3 -> to bottom rigth corner, 4 -> to bottom left corner */
    float translate_x[] {
        0, length, length, 0
    };
    float translate_y[] {
        0, 0, -length, -length
    };

    /* calculates new chessboard's square
    i     -> x - position
    i + 1 -> y - position
    i + 2 -> color flag */
    static int i = 0;
    for(int n = 0; n < 4; n++) {
        _chessboard.vertices[i] = x + translate_x[n];
        _chessboard.vertices[i + 1] = y + translate_y[n];
        _chessboard.vertices[i + 2] = colorFlag;

        i += 3;
    }

    if(i >= _size * _size * 12) {
        i = 0;
    }
}

/* Generates data for chessboard. */
void Renderer::generateChessData() {
    /* each square has 4 vertices and each vertex has 3 attributes:
    x - pos, y - pos and color flag: 1 for white, 0 - black,
    so vertex data size must be equal _size * _size * 4 * 3 */
    const int vertexSize = 12 * _size * _size;
    _chessboard.vertices = new float[vertexSize]();

    // we have to define 6 vertices per one square
    const int indicesSize = 6 * _size * _size;
    _chessboard.indices = new unsigned int[indicesSize]();

    /* defines start points (upper left corner) and length */
    float start_x = -1.0f;
    float start_y = -start_x;
    float length = 2 * start_y / _size;

    /* calculates data for knight depending on start position and length */
    setKnightData(start_x, start_y, length);

    // variables needed to calculate new positions
    float x = 0.0f;
    float y = 0.0f;

    // calculate squares' position on main diagonal (white color)
    for(int i = 0; i < _size; i++) {
        x = start_x + length * i;
        y = start_y - length * i;
        setSquare(x, y, 1.0f);
    }

    // other diagonalns
    for(int i = 1; i < _size; i++) {
        for(int j = 0; j < _size - i; j++) {
            // superdiagonals
            x = start_x + length * (j + i);
            y = start_y - length * j;
            setSquare(x, y, float((i - 1) % 2));

            // subdiagonals
            x = start_x + length * j;
            y = start_y - length * (j + i);
            setSquare(x, y, float((i - 1) % 2));
        }
    }

    /* generates indices for chessboard */
    setChessIndices();
}

/* Sets data for knight vertices and sends it to buffer.
(start_x,start_y) in function's signature -> left upper corner of start square. */
void Renderer::setKnightData(float start_x, float start_y, const float& length) {
    /* moves start_x to the right if _startPos != 0 */
    start_x = start_x + (_startPos % _size) * length;

    /* moves start_y down if _startPos != 0 */
    start_y = start_y - (_startPos / _size) * length;

    /* initializes line at the same position like knight,
    but on the middle of square */
    initializeLine(start_x + length / 2, start_y - length / 2);

    /* knight's vertex data */
    float knightData[] {
        /*      POSITION                TEXTURE COORDINATE */
        start_x, start_y - length,          0.0f, 0.0f,              // bottom left corner
        start_x, start_y,                   0.0f, 1.0f,              // upper left corner
        start_x + length, start_y,          1.0f, 1.0f,              // upper right corner
        start_x + length, start_y - length, 1.0f, 0.0f               // bottom right corner
    };
    /* knight's indices data */
    unsigned int knightIndexes[] {
        0, 1, 2, 2, 3, 0
    };

    /* binding objects and sending data to buffers */
    _knight.vao.bind();
    _knight.vbo.bind();
    _knight.vbo.setData(knightData, sizeof(knightData));
    _knight.ebo.bind();
    _knight.ebo.setData(knightIndexes, sizeof(knightIndexes));

    /* setting appropriate attributes and vertex attrib pointer */
    AttributeLayout<float> layout(POSITION | TEXTURE);
    layout.setAttribSize(Position, 2);
    Shader::vertexAttrib(Position, layout);
    Shader::vertexAttrib(Texture, layout);

    /* texture settings stuff
    first texture */
    Shader::useProgram(_knight.program);
    Texture2D texture;
    texture.bind();
    texture.activeTex(GL_TEXTURE0);
    texture.setData(texturePath_knight, GL_RGBA);
    texture.setUniformSampler(_knight.program, "knightTex", GL_TEXTURE0);

    /* second texture */
    texture.activeTex(GL_TEXTURE1);
    texture.setData(texturePath_gachi, GL_RGB);
    texture.setUniformSampler(_knight.program, "gachiTex", GL_TEXTURE1);
}

/* Moves and draws knight.
move_x and move_y mean knight movement on chessboard, e.g
(-2, 1), (1, 2), (2, -1), etc. */
void Renderer::drawKnight(int move_x, int move_y) {
    static float length = 2.0f / _size;

    /* data needed to matrix transformations
    x and y must be static because knight's vertex data isn't changed (once send to buffer),
    so this values have to be continously updated */
    static float x = 0.0f;
    static float y = 0.0f;
    x += move_x * length;
    y += move_y * length;

    /* with kinght's position there's need to update line's vertex buffer object */
    updateLine(move_x * length, move_y * length);

    /* draws line after update */
    _line.vao.bind();
    Shader::useProgram(_line.program);
    glDrawElements(GL_LINES, (_lineCounter - 1) * 2, GL_UNSIGNED_INT, (void*)0);
    
    _knight.vao.bind();
    Shader::useProgram(_knight.program);

    /* translates knight to next position */
    glm::mat4 transform(1.0f);
    transform = glm::translate(transform, glm::vec3(x, y, 0.0f));
    unsigned int pos = glGetUniformLocation(_knight.program, "transformMatrix");
    glUniformMatrix4fv(pos, 1, GL_FALSE, glm::value_ptr(transform));

    /* draws knight */
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}

/* Calculates indices for chessboard data.
Chessboard's indices array shoud look like:
0, 1, 2, 2, 3, 0 -> (first square)
4, 5, 6, 6, 7, 0 -> (second square)
8, 9, 10, 10, 11, 8 -> (third square) */
void Renderer::setChessIndices() {
    int squareVertexID = 0;
    int vertexID = 0;

    for(int i = 0; i < _size * _size; i++) {
        squareVertexID = i * 6;
        vertexID = i * 3;

        /*  _chessboard.indices[squareVertexID + x] (x = 0,1,...5) -> indexes of vertices that create one square (3 per triangle)
        i + x + vertexID (x = 0,1,2,3) -> four vertices that belong to one square */
        _chessboard.indices[squareVertexID] = i + vertexID;
        _chessboard.indices[squareVertexID + 1] = i + 1 + vertexID;
        _chessboard.indices[squareVertexID + 2] = i + 2 + vertexID;
        _chessboard.indices[squareVertexID + 3] = i + 2 + vertexID;
        _chessboard.indices[squareVertexID + 4] = i + 3 + vertexID;
        _chessboard.indices[squareVertexID + 5] = i + vertexID;
    }
}

/* Sets initial data for line. */
void Renderer::initializeLine(float x, float y) {
    /* sets line width depending on chessboard size */
    glEnable(GL_LINE_SMOOTH);
    // glLineWidth(1.0f);
    glLineWidth(3.0f / _size * 8.0f);

    /* vertex has 2 positions, so line's vertex data size will be equal 2 * _size * _size */
    _line.vertices = new float[2 * _size * _size]();

    /* indices data size will be smaller than vertex
    if we have e.g 5 points, than 4 lines will be drawn (knight path couldn't be closed) */
    _line.indices = new unsigned int[2 * (_size * _size - 1)]();

    /* initial x and y positons */
    _line.vertices[0] = x;
    _line.vertices[1] = y;

    /* sets line indices at one blow */
    setLineIndexes();

    /* binding objects and sending data to buffers */
    _line.vao.bind();
    _line.vbo.bind();
    _line.ebo.bind();
    _line.vbo.setData(_line.vertices, sizeof(float) * 2 * _size * _size);
    _line.ebo.setData(_line.indices, sizeof(unsigned int) * 2 * (_size * _size - 1));

    /* setting appropriate attributes and vertex attrib pointer */
    AttributeLayout<float> layout(POSITION);
    layout.setAttribSize(Position, 2);
    Shader::vertexAttrib(Position, layout);
    _line.vao.unbind();
    _lineCounter = 1;
}

/* Updates line's vertex data with given x and y positions. */
void Renderer::updateLine(float x, float y) {
    /* if knight's path is ended or knight is on initial position, then exit funciton */
    if(!(_lineCounter < _size * _size) || (x == 0.0f && y == 0.0f)) {
        return;
    }

    /* binding objects and updating line's vertex data on CPU side */
    _line.vbo.bind();
    _line.ebo.bind();
    _line.vertices[2 * _lineCounter] = _line.vertices[2 * (_lineCounter - 1)] + x;
    _line.vertices[2 * _lineCounter + 1] = _line.vertices[2 * (_lineCounter - 1) + 1] + y;
    
    /* updates vertex data on GPU side */
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * 2 * _lineCounter, sizeof(float), &_line.vertices[2 * _lineCounter]);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (2 * _lineCounter + 1), sizeof(float), &_line.vertices[2 * _lineCounter + 1]);

    _lineCounter++;
}

/* Fill line's indices with specific data.
Line's indices array should look like:
0 1 -> (from first point to second)
1 2 -> (from 1. to 2.)
...
n-2 n-1 (from n-2. to n-1) */
void Renderer::setLineIndexes() {
    _line.indices[0] = 0u;
    _line.indices[1] = 1u;

    for(unsigned int i = 2u; i < 2 * (_size * _size - 1u); i++) {
        _line.indices[i] = _line.indices[i - 1] + i % 2;
    }
}