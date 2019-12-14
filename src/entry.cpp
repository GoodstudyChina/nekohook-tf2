
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

#include <chrono>
#include <fstream>
#include <thread>

#include "sdk/entity.hpp"

#include "hooks/clientmode.hpp"
#include "iface.hpp"

namespace neko::entry {
using namespace std::chrono_literals;

void __attribute__((constructor)) Init() {
    std::thread t([](){
        try {
            iface::Init();
            std::this_thread::sleep_for(3s);
            sourcesdk::netvar::Init();
            hook::clientmode::Init();
        } catch (std::exception& err) {
            //std::fstream out("/tmp/err", std::ios_base::out | std::ios_base::trunc);
            //out << "Recieved error: " << err.what() << std::endl;
        } catch (...) {
            std::fstream out("/tmp/err", std::ios_base::out | std::ios_base::trunc);
            out << "Recieved unknown error." << std::endl;
        }
    });
    t.detach();
}

}
