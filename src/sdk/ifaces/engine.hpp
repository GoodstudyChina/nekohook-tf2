
#pragma once

#include <neko/hook/util/math.hpp>

#include "../vft.hpp"


namespace sourcesdk {
using namespace neko;
static constexpr int kMaxNameLength = 32;

struct PlayerInfo {
    char name[32];
    int  user_id;
    char guid[33];
    int  friends_id;
    char friends_name[32];
    bool fake_player;
    bool is_hltv;
    int  custom_files[4];
    int  files_downloaded;
};

class Engine {
   public:
    bool GetPlayerInfo(int ent_num, PlayerInfo& pinfo) { // 8
        using Func = bool (*)(int, PlayerInfo&);
        return GetVirtual<Func>(this, 8)(ent_num, pinfo);
    }

    int GetLocalPlayer() { // 12
        using Func = int (*)();
        return GetVirtual<Func>(this, 12)();
    }
    math::Vector GetViewAngles() {
        math::Vector tmp;
        using Func = void (*)(math::Vector&);
        GetVirtual<Func>(this, 19)(tmp);
        return tmp;
    }
    void SetViewAngles(math::Vector& va) {
        using Func = void (*)(math::Vector&);
        return GetVirtual<Func>(this, 20)(va);
    }

    //int GetMaxClients(void) = 0; // 21

    bool IsInGame() { // 26
        using Func = bool (*)();
        return GetVirtual<Func>(this, 26)();
    }

    // Get access to the world to screen transformation matrix
    //const VMatrix& WorldToScreenMatrix() = 0; // 36
};

}
