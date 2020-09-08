#include "glhelper.h"

namespace parkour {
namespace glhelper {

    uint32_t convertArgbToRgba(uint32_t argb) {
        return ((argb << 8) & 0xffffff00) | ((argb >> 24) & 0x000000ff);
    }

}
}
