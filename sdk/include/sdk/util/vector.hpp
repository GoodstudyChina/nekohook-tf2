
/*
 * Nekohook: Free, fast and modular cheat software
 * Copyright (C) 2018 Rebekah Rowe
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
 
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
