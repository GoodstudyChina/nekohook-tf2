
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

#pragma once

#include <cassert>
#include <hack/memory.hpp>

#include "ifaces/base_client.hpp"

namespace sourcesdk {
using namespace neko::hack;
class Entity;

namespace netvar {

template<typename T>
class Netvar {
public:
    inline T& Get(Entity* entity) {
        assert(this->offset);
        return *mem::Offset<T>(entity, this->offset);
    }
    void Set(std::ptrdiff_t d) { this->offset = d; }
private:
    std::ptrdiff_t offset = 0;
};

void Init();

static inline Netvar<math::Vector> origin;

}

}
