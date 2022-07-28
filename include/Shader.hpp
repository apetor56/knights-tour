#ifndef SHADER_HPP
#define SHADER_HPP

#include <optional>
#include "Exception.hpp"

class Shader {
public:
    explicit Shader(unsigned int type, const char *source);

    Shader(const Shader& other) = delete;

    ~Shader();

    operator unsigned int();

    static unsigned int createProgramShader(unsigned int s1, unsigned int s2);

    static void useProgram(unsigned int program);

    unsigned int getShader() const noexcept;

private:
    unsigned int _shaderID;

    static std::optional<std::string> getShaderSourceCode(const char *name);
};

class ShaderException : public Exception {
public:
    using Exception::Exception;

    ShaderException(int line, const char *file, const char *infoLog);

    const char *getType() const override;

    const char *what() const noexcept override;
};

#endif // SHADER_HPP