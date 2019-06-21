
#include <cstddef>

namespace sourcesdk {

template<typename Func, class This>
Func GetVirtual(This* _this, std::ptrdiff_t offset) {
    using VFT = void**;
    VFT table = *reinterpret_cast<VFT*>(_this);
    return reinterpret_cast<Func>(table + offset);
}

}
