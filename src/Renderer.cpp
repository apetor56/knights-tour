#include "Renderer.hpp"

const char *vertexPath = "../shaders/vertex.glsl";
const char *fragmentPath = "../shaders/fragment.glsl";

Renderer::Renderer() 
:   _vertex(GL_VERTEX_SHADER, vertexPath),
    _fragment(GL_FRAGMENT_SHADER, fragmentPath)
{}

// set VAO, VBO, EBO and shaders for generated data 
void Renderer::setChessBoard(int size) {
    _size = size;
    generateData(_size);
    _vbo.setData(_data, sizeof(float) * _size * _size * 12);

    AttributeLayout<float> layout(POSITION | COLOR);
    layout.setAttribSize(Position, 2);
    layout.setAttribSize(Color, 1);

    _ebo.setData(_indexes, sizeof(unsigned int) * size * size * 6);
    Buffer<GL_ELEMENT_ARRAY_BUFFER, unsigned int> a;
    a.setData(_indexes, _size * _size * 6 * sizeof(unsigned int));

    Shader::enableAttrib(Position);
    Shader::vertexAttrib(Position, layout);
    Shader::enableAttrib(Color);
    Shader::vertexAttrib(Color, layout);

    Shader vertex(GL_VERTEX_SHADER, vertexPath);
    Shader fragment(GL_FRAGMENT_SHADER, fragmentPath);
    unsigned int program = Shader::createProgramShader(vertex, fragment);
    Shader::useProgram(program);
}

void Renderer::drawChessBoard() {
    glDrawElements(GL_TRIANGLES, _size * _size * 6, GL_UNSIGNED_INT, (void*)0);
}

// calculates square's position depending on given coordinates
// (x,y) -> lefl upper corner
void Renderer::setSquare(float& x, float& y, float colorFlag) const {
    // side's length
    static float length = 2 * y / _size;

    // this value tell us, how to move given x and y to calculete square's coordinates
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
        _data[i] = x + translate_x[n];
        _data[i + 1] = y + translate_y[n];
        _data[i + 2] = colorFlag;

        i += 3;
    }
}

void Renderer::generateData(int n) {
    // each square on the board has 4 vertices
    // and each vertex has 3 attributes:
    // x - pos, y - pos and color flag: 1 for white, 0 - black
    const int vertexSize = 12 * n * n;
    _data = new float[vertexSize]();

    // we have to defined 6 vertices per one square
    const int indicesSize = 6 * n * n;
    _indexes = new unsigned int[indicesSize]();

    // defined start points (upper left corner) and length
    // (you could change 'start_x' to scale chessboard)
    float start_x = -0.8f;
    float start_y = -start_x;
    float length = 2 * start_y / n;

    // variables needed to calculate new positions
    float x = 0.0f;
    float y = 0.0f;

    // calculate positions of squares on main diagonal (while color)
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