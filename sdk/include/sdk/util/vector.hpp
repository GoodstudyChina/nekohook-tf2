
#include "memory.hpp"

namespace sourcesdk::utl {

template <class T, class Allocator = Memory<T> >
class Vector {
   public:
    inline bool operator[](int idx) const {return memory.mem[idx]} std::size_t
        size() {
        return this->size;
    }

   private:
    Allocator memory;
    int size;
};

}  // namespace sourcesdk
