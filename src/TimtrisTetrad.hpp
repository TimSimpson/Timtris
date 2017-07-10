#ifndef TimtrisTETRAD_H
#define TimtrisTETRAD_H

#include <lp3/core.hpp>

namespace lp3 { namespace timtris {

class TimtrisTetrad
{
public:
    TimtrisTetrad(int tetradIndex, int rotationIndex);
    /* Functions to "Get" the actual data on how the block maps out. */
    bool Block(int x, int y);
    int Height();
    int Width();
    /* Properties. */
    int GetRotation();
    int GetTetrad();
    void Rotate(bool clockWise);
    void SetRotation(int rotateIndex);
    void SetTetrad(int tetradIndex);
private:
    const char * blocks;
    std::uint32_t height;
    int rotateIndex;
    int tetradIndex;
	std::uint32_t width;
};

}	}

#endif
