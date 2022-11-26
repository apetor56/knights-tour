#ifndef Texture2D_HPP
#define Texture2D_HPP

class Texture2D {
public:
    explicit Texture2D();

    Texture2D(const Texture2D& other) = delete;

    Texture2D& operator=(const Texture2D& other) = delete;

    static void activeTex(unsigned int Texture2D);

    static void setData(const char *imagePath, unsigned int colorFormat);

    static void setUniformSampler(unsigned int program, const char *name, int textureUnit);

    void bind() const;

private:
    unsigned int _texID;
};

#endif // Texture2D_HPP