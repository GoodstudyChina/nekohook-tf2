
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

#include <bitset>
#include <cstddef>

#include <neko/hook/util/math.hpp>

namespace sourcesdk {
using namespace neko;

enum class Buttons {
    kAttack,
    kJump,
    kDuck,
    kForward,
    kBack,
    kUse,
    kCancel,
    kLeft,
    kRight,
    kMoveLeft,
    kMoveRight,
    kAttack2,
    kRun,
    kReload,
    kAlt1,
    kAlt2,
    kScore,
    kSpeed,
    kWalk,
    kZoom,
    kWeapon1,
    kWeapon2,
    kBullRush,
    kGrenade1,
    kGrenade2,
    kAttack3
};

class UserCmd {
public:
    virtual ~UserCmd(){};
    int command_number;
    int tick_count;
    math::Vector viewangles;
    float forwardmove;
    float sidemove;
    float upmove;
    std::bitset<sizeof(int) * 8> buttons;
    std::byte impulse;
    int weaponselect;
    int weaponsubtype;
    int random_seed;
    short mousedx;
    short mousedy;
    bool hasbeenpredicted;
};

}
