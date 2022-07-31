#include <glad.h>
#include <sstream>
#include <fstream>
#include <cstring>
#include "Shader.hpp"
#include "Exception.hpp"

#define SHD_EXC(infoLog) ShaderException(__LINE__, __FILE__, infoLog);

//-------------------------------------------------------------//
//---------------- Shader class implementation ----------------//
//-------------------------------------------------------------//

// create and compile shader from source file
Shader::Shader(unsigned int type, const char *sourceFile) {
    _shaderID = glCreateShader(type);

    std::string s_src = *Shader::getShaderSourceCode(sourceFile);
    const char *src = s_src.c_str();
    
    glShaderSource(_shaderID, 1, &src, nullptr);
    glCompileShader(_shaderID);

    char infoLog[512];
    int success;

    glGetShaderiv(_shaderID, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(_shaderID, 512, nullptr, infoLog);
        throw SHD_EXC(infoLog);
    }
}

Shader::~Shader() {
    glDeleteShader(_shaderID);
}

Shader::operator unsigned int() {
    return _shaderID;
}

// get shader from source file with given path
std::optional<std::string> Shader::getShaderSourceCode(const char *name) {
    std::ostringstream oss;
    std::fstream file(name);

    if(!file) {
        std::cout << name << ": failed to open shader source file\n";
        return {};
    }

    std::string line;
    while(std::getline(file, line)) {
        oss << line << '\n';
    }
    
    return oss.str();
}

// creates and link shader program
unsigned int Shader::createProgramShader(unsigned int s1, unsigned int s2) {
    unsigned int program = glCreateProgram();

    glAttachShader(program, s1);
    glAttachShader(program, s2);

    glLinkProgram(program);

    char infoLog[512];
    int success;

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        throw SHD_EXC(infoLog);
    }

    return program;
}

void Shader::useProgram(unsigned int program) {
    glUseProgram(program);
}

unsigned int Shader::getShader() const noexcept {
   return _shaderID;
}

void Shader::enableAttrib(int index) {
        glEnableVertexAttribArray(index);
}

void Shader::disableAttrib(int index) {
    glDisableVertexAttribArray(index);
}

//-----------------------------------------------------------------//
//---------------- Shader Exception implementation ----------------//
//-----------------------------------------------------------------//

ShaderException::ShaderException(int line, const char *file, const char *infoLog)
:   ShaderException(line, file)
{
    whatBuffer = infoLog;
}

const char *ShaderException::getType() const {
    return "Shader exception";
}

const char *ShaderException::what() const noexcept {
    std::ostringstream oss;

    oss << "[FILE] " << getFile() << ":" << getLine() << '\n'
        << "[LINE] " << getLine() << '\n'
        << "[TYPE] " << getType();

    whatBuffer = oss.str() + '\n' + whatBuffer;
    
    return whatBuffer.c_str();
}