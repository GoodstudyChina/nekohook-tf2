
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

#include <neko/hook/command.hpp>
#include <neko/hook/settings/var.hpp>

#include "sdk/convar.hpp"

#include "iface.hpp"

namespace neko::console {

class CatConvar : public sourcesdk::Convar {
public:
    CatConvar(settings::Var* _var) : sourcesdk::Convar(_var->command_name.c_str(), "Refer to the nekohook wiki"), var(_var) {}
    void Set(const char* in) override {
        var->SetString(in);
        this->sourcesdk::Convar::Set(var->GetString().c_str());
    }
private:
    settings::Var* var;
};

void Init() {
    // Register Commands and vars
    //for (const neko::Command* tmp : neko::Command::GetList())
    //    iface::cvar->RegisterConCommand(new CatCommand(tmp));
    for (settings::Var* tmp : settings::Var::GetList()) {
        auto* catvar = new CatConvar(tmp);
        iface::cvar->Register(catvar);
    }
}

}
