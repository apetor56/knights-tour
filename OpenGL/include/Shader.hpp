#ifndef SHADER_HPP
#define SHADER_HPP

#include <optional>
#include "Exception.hpp"
#include "AttributeLayout.hpp"

class Shader {
public:
    Shader() = default;
    
    explicit Shader(unsigned int type, const char *source);

    Shader(const Shader& other) = delete;

    ~Shader();

    operator unsigned int();

    static unsigned int createProgramShader(unsigned int s1, unsigned int s2);

    static void useProgram(unsigned int program);

    unsigned int getShader() const noexcept;

    static void enableAttrib(int index);

    static void disableAttrib(int index);

    // execute glEnableVertexAttribArray() and glVertexAttribPointer() functions
    // with given attrib and stride and offset given in attribute layout
    template<typename T>
    static void vertexAttrib(Attributes attrib, AttributeLayout<T>& layout) {
        int stride = layout.getStride();
        void *offset = layout.getOffset(attrib);
        const int& attribSize = layout.getAttribSize(attrib);

        glEnableVertexAttribArray(attrib);
        glVertexAttribPointer(attrib, attribSize, dataType<T>(), GL_FALSE, stride, offset);
    }

private:
    unsigned int _shaderID;

    static std::optional<std::string> getShaderSourceCode(const char *name);

    // returns specific GL macro depeding on type od data
    // stored in buffers
    template<typename T>
    static int dataType() {
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

class ShaderException : public Exception {
public:
    using Exception::Exception;

    ShaderException(int line, const char *file, const char *infoLog);

    const char *getType() const override;

    const char *what() const noexcept override;
};

#endif // SHADER_HPP