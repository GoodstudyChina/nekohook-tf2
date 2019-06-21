
#include <cstddef>

namespace sourcesdk {

template<typename Func, class This>
Func GetVirtual(This* _this, int offset) {
    void** table = *reinterpret_cast<void***>(_this);
    return reinterpret_cast<Func>(table[offset]);
}

}
