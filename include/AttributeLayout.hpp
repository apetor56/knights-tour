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
                _stride += _attribSize[attrib];
            }
        }
    }

    AttributeLayout(const AttributeLayout& other) = delete;

    AttributeLayout& operator=(const AttributeLayout& other) = delete;

    // changes given attrib with new size
    // (do this before sending data to vertex buffer!!!)
    void setAttribSize(Attributes attrib, int size) {
        if(size > 0 && size <= 4) {
            _attribSize[attrib] = size;
        }
        else {
            std::cerr << "Passed wrong attribute size: " << size
            << ". You could pass only values: 1, 2, 3 or 4\n";
        }

        refreshInfo();
    }

    int getAttribSize(Attributes attrib) const {
        return _attribSize[attrib];
    }

    // calculates stride and offsets again due to
    // changes in specific attribute's size (_attribSize[])
    // do the same what in constructor
    void refreshInfo() {
        _stride = 0;
        for(int attrib = Position; attrib != End; attrib++) {
            if(isUsed(attrib)) {
                _offsets[attrib] = _stride; 
                _stride += _attribSize[attrib];
            }
        }
    }

    // checks if given attribute is used
    bool isUsed(int attrib) const {
        return _usedAttrib[attrib];
    }

    // return stride in bytes
    int getStride() const noexcept {
        return _stride * sizeof(T);
    }

    // return offset of given attribute (cast to void*)
    void *getOffset(Attributes attrib) const {
        return reinterpret_cast<void*>(_offsets[attrib] * sizeof(T));
    }

    int getOffseti(Attributes attrib) const {
        return _offsets[attrib];
    }

private:
    int _stride;
    int _offsets[4];
    std::bitset<4> _usedAttrib;
    int _attribSize[4] = {3, 3, 2, 0};
};

#endif // ATTRIBUTE_LAYOUT_HPP