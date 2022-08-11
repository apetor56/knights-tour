#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

#include "Renderer.hpp"
#include "Texture2D.hpp"

const char *vertexPath_chess = "../shaders/vertex.glsl";
const char *fragmentPath_chess = "../shaders/fragment.glsl";
const char *vertexPath_knight = "../shaders/vertex_knight.glsl";
const char *fragmentPath_knight = "../shaders/fragment_knight.glsl";
const char *texturePath = "../tex/knight.png";

Renderer::Renderer() 
:   _vertexChess(GL_VERTEX_SHADER, vertexPath_chess),
    _fragmentChess(GL_FRAGMENT_SHADER, fragmentPath_chess),
    _vertexKnight(GL_VERTEX_SHADER, vertexPath_knight),
    _fragmentKnight(GL_FRAGMENT_SHADER, fragmentPath_knight)
{}

// set VAO, VBO, EBO and shaders for generated data 
void Renderer::setChessBoard(int size, int startPos) {
    _size = size;
    _startPos = startPos;
    generateData(_size);
    _vaoChess.bind();

    _vboChess.bind();
    _vboChess.setData(_chessData, sizeof(float) * _size * _size * 12);
    _eboChess.bind();
    _eboChess.setData(_indexes, _size * size * 6 * sizeof(unsigned int));

    AttributeLayout<float> layout(POSITION | COLOR);
    layout.setAttribSize(Position, 2);
    layout.setAttribSize(Color, 1);

    // Shader::enableAttrib(Position);
    Shader::vertexAttrib(Position, layout);
    // Shader::enableAttrib(Color);
    Shader::vertexAttrib(Color, layout);

    unsigned int program = Shader::createProgramShader(_vertexChess.getShader(), _fragmentChess.getShader());
    _programChess = program;
}

void Renderer::drawChessBoard() const {
    _vaoChess.bind();
    Shader::useProgram(_programChess);
    glDrawElements(GL_TRIANGLES, _size * _size * 6, GL_UNSIGNED_INT, (void*)0);
}

// calculates square's position depending on given coordinates
// (x,y) -> lefl upper corner
void Renderer::setSquare(float& x, float& y, float colorFlag) const {
    // side's length
    static float length = 2 * y / _size;

    // this values tell us, how to move given x and y to calculete square's coordinates
    // 1 -> upper left corner, 2 -> upper rigth corner,
    // 3 -> bottom rigth corner, 4 -> bottom left corner
    static float translate_x[] {
        0, length, length, 0
    };
    static float translate_y[] {
        0, 0, -length, -length
    };

    // calculate vertex attributes
    static int i = 0;
    for(int n = 0; n < 4; n++) {
        _chessData[i] = x + translate_x[n];
        _chessData[i + 1] = y + translate_y[n];
        _chessData[i + 2] = colorFlag;

        i += 3;
    }
}

void Renderer::generateData(int n) {
    // each square on the board has 4 vertices
    // and each vertex has 3 attributes:
    // x - pos, y - pos and color flag: 1 for white, 0 - black
    const int vertexSize = 12 * n * n;
    _chessData = new float[vertexSize]();

    // we have to defined 6 vertices per one square
    const int indicesSize = 6 * n * n;
    _indexes = new unsigned int[indicesSize]();

    // defined start points (upper left corner) and length
    // (you could change 'start_x' to scale chessboard)
    float start_x = -0.8f;
    float start_y = -start_x;
    float length = 2 * start_y / n;

    // 
    setKnightData(start_x, start_y, length);

    // variables needed to calculate new positions
    float x = 0.0f;
    float y = 0.0f;

    // calculate positions of squares on main diagonal (white color)
    for(int i = 0; i < n; i++) {
        x = start_x + length * i;
        y = start_y - length * i;
        setSquare(x, y, 1.0f);
    }

    // other diagonalns
    for(int i = 1; i < n; i++) {
        for(int j = 0; j < n - i; j++) {
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

    setIndexes();
}

// set data for knight vertices and send it
// immediately to buffer
void Renderer::setKnightData(float start_x, float start_y, float length) {
    start_x = start_x + (_startPos % _size) * length;
    start_y = start_y - (_startPos / _size) * length;

    float knightData[] {
        start_x, start_y - length, 0.0f, 0.0f,
        start_x, start_y, 0.0f, 1.0f,
        start_x + length, start_y, 1.0f, 1.0f,
        start_x + length, start_y - length, 1.0f, 0.0f
    };
    unsigned int knightIndexes[] {
        0, 1, 2, 2, 3, 0
    };

    _vaoKnight.bind();

    _vboKnight.bind();
    _vboKnight.setData(knightData, sizeof(knightData));

    _eboKnight.bind();
    _eboKnight.setData(knightIndexes, sizeof(knightIndexes));

    AttributeLayout<float> layout(POSITION | TEXTURE);
    layout.setAttribSize(Position, 2);
    Shader::vertexAttrib(Position, layout);
    Shader::vertexAttrib(Texture, layout);

    _programKnight = Shader::createProgramShader(_vertexKnight.getShader(), _fragmentKnight.getShader());
    Shader::useProgram(_programKnight);

    Texture2D texture;
    texture.activeTex(GL_TEXTURE0);
    texture.bind();
    texture.setData(texturePath, GL_RGBA);
    texture.setUniformSampler(_programKnight, "knightTex", GL_TEXTURE0);
}

void Renderer::drawKnight(int move_x, int move_y) {
    _vaoKnight.bind();
    Shader::useProgram(_programKnight);

    static float length = 1.6f / _size;
    static float x = 0.0f;
    static float y = 0.0f;
    x += move_x * length;
    y += move_y * length;

    glm::mat4 transform(1.0f);
    transform = glm::translate(transform, glm::vec3(x, y, 0.0f));
    unsigned int pos = glGetUniformLocation(_programKnight, "transformMatrix");
    glUniformMatrix4fv(pos, 1, GL_FALSE, glm::value_ptr(transform));
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}

// calculate indices required by element buffer object
void Renderer::setIndexes() {
    // squareVertexID means square ID:
    // _indexes[squareVertexID + x] (x = 0,1,...5) -> indexes of vertices that belog to specific square
    // vertexID + x (x = 0,1,2,3) -> four vertices that belong to one square
    int squareVertexID = 0;
    int vertexID = 0;

    // _indexes[] array shoud look like:
    // 0, 1, 2, 2, 3, 0     (first square)
    // 4, 5, 6, 6, 7, 0     (second square)
    // 8, 9, 10, 10, 11, 8  (third square)

    for(int i = 0; i < _size * _size; i++) {
        squareVertexID = i * 6;
        vertexID = i * 3;

        _indexes[squareVertexID] = i + vertexID;
        _indexes[squareVertexID + 1] = i + 1 + vertexID;
        _indexes[squareVertexID + 2] = i + 2 + vertexID;
        _indexes[squareVertexID + 3] = i + 2 + vertexID;
        _indexes[squareVertexID + 4] = i + 3 + vertexID;
        _indexes[squareVertexID + 5] = i + vertexID;
    }
}