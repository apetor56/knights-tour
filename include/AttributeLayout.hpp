#ifndef ATTRIBUTE_LAYOUT_HPP
#define ATTRIBUTE_LAYOUT_HPP

#include <bitset>

//-------------------------------------------------------------//
//------------- AttributeLayout class implementation ----------//
//-------------------------------------------------------------//

// macros used in Attributes constructor to
// determine values like stride and offset 
#define POSITION 1
#define COLOR (1 << 1)
#define TEXTURE (1 << 2)
#define ALL 3

// values used in glVertexAttribPointer() function
// that represent vector's size in vertex shader (vec1, vec2, vec3, vec4)
const int vec[] {
    3,      // position
    3,      // color
    3,      // texture
    0       // end
};

// values used in glVertexAttribPinter() and
// glEnableVertexArray() functions that represent
// attribute's index (layout int vertex shader)
enum Attributes {
    Position, Color, Texture, End
};

// wrapper for attributes, calculates
// stride, offsets and determine active
// attributes
template<typename T>
class AttributeLayout {
public:
    AttributeLayout(unsigned int usedAttributes)
    :   _stride(0),
        _usedAttrib(usedAttributes)
    {
        // specific attribute offset equals "stride" to that point,
        // where this attribute begins (in integers)
        // stride equals sum of all active attributes' size (in integers)
        for(int attrib = Position; attrib != End; attrib++) {
            if(isUsed(attrib)) {
                _offsets[attrib] = _stride; 
                _stride += vec[attrib];
            }
        }
    }

    AttributeLayout(const AttributeLayout& other) = delete;

    AttributeLayout& operator=(const AttributeLayout& other) = delete;

    // checks if given attribute is used
    bool isUsed(int attrib) {
        return _usedAttrib[attrib];
    }

    // return stride in bytes
    int getStride() const noexcept {
        return _stride * sizeof(T);
    }

    // return offset of given attribute (cast to void*)
    void *getOffset(Attributes attrib) {
        return reinterpret_cast<void*>(_offsets[attrib] * sizeof(T));
    }

private:
    int _stride;
    int _offsets[4];
    std::bitset<4> _usedAttrib; 
};

#endif // ATTRIBUTE_LAYOUT_HPP