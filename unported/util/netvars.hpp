
/*
 *
 *	Header file for netvars. Be sure to extern every netvar you put in the
 *main cpp file
 *
 */

#pragma once

#include <cstdint>
#include <initializer_list>

#if defined(NEKO_TF2)
#include <bitset>
#endif

#include "../../../hacks/memory.hpp"
#include "../../../util/mathlib.hpp"

namespace modules::source::netvars {

template <typename Type>
class NetVar {
    uint16_t offset = 0;  // To limit the amout of copying/moving of variables
                          // for a small reach at speed
   public:
    auto GetOffset() { return this->offset; }

    // because things are retarted
    inline void Set(uint16_t in) { this->offset = in; }

    // because CatEntity and IClientEntity, gays
    template <typename Pointer>
    static inline Type& Get(Pointer entity, uint16_t _offset) {
        return mem::Offset<Type>(entity, _offset);
    }

    template <typename Pointer>
    inline Type& Get(Pointer entity) {
        return this->Get(entity, this->offset);
    }
};

// Generic stuff
extern NetVar<int> m_iTeamNum;
extern NetVar<int> m_iHealth;
extern NetVar<unsigned char> m_lifeState;
extern NetVar<CatVector> m_vecViewOffset;
// Aimbot
extern NetVar<int> m_nHitboxSet;
extern NetVar<float> m_flSimulationTime;
extern NetVar<int> m_hActiveWeapon;
// NetVar<float> m_iFOV;
// NetVar<> m_nTickBase;
extern NetVar<float> m_flLastFireTime;
extern NetVar<int> deadflag;
extern NetVar<int> m_iClip1;
// Team fortress 2
#if defined(NEKO_TF2)
extern NetVar<float> m_flNextPrimaryAttack;
extern NetVar<int> m_iBuildingHealth;
extern NetVar<int> m_iBuildingMaxHealth;
extern NetVar<int> m_iUpgradeLevel;
extern NetVar<int> m_iClass;
extern NetVar<int> res_iMaxHealth;
extern NetVar<int> res_iPlayerClass;
extern NetVar<std::bitset<128>> m_nPlayerCond;
extern NetVar<float> m_flChargedDamage;
extern NetVar<int> m_iItemDefinitionIndex;
extern NetVar<int> m_nForceTauntCam;
#endif

void Init();

}  // namespace modules::source::netvars
