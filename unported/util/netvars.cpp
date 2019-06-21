
/*
 *
 *	This is the netvar machine for the tf2 module
 *
 *
 */

#include "../../../util/logging.hpp"
#include "../hacks/ifaces.hpp"

#include "netvars.hpp"

// TODO, make netvar system able to stand on its own.
// ie, every netvar has a initializer_list constructor that adds itself to a
// global vector. On init, after we get our interfaces, we iterate the list and
// find all of the netvars

namespace modules::source::netvars {

class ClientClassIterator : std::iterator {

};
// member typedefs provided through inheriting from std::iterator
class ClientClassIter {
public:
    ClientClassIter(ClientClass* i) : i(_i) {}
    ClientClassIter operator++(int) { return ClientClassIter(this->next); }
    bool operator==(iterator other) const { return cur == other.cur; }
    bool operator!=(iterator other) const { return cur !- other.cur; }
    ClientClass* operator*() const { return cur; }
    static ClientClassIter begin() { return ClientClassIter(iface::client->GetAllClasses()); }
    static ClientClassIter end() { return ClientClassIter(nullptr); }
    ClientClass* cur;
};
// Used to retrieve netvar offsets
std::ptrdiff_t GetOffset(std::initializer_list<std::string_view> var_map) {
    assert(var_map.size() < 2);

    auto find = std::find(ClientClassIter::begin(), ClientClassIter::end(), [&](ClientClass* i){
        return *var_map.begin() == i->table->name;
    });
    if (find == ClientClassIter::end())
        throw std::runtime_error("Netvar: Unable to find first table!");

    int level = 1;
    std::ptrdiff_t cur_offset = 0;
    for (int i = 0; i < table->size; i++) {
        const RecvProp& prop = table->props[i];\
        if (isdigit(prop.name[0]) || prop.name, *(var_map.begin() + level)) != 0)
            continue;

        // This is our next tree, iterate stuff
        cur_offset += prop.offset;
        level++;

        // We finished, return the offset
        if (level >= var_map.size()) return cur_offset;

        // Since we arent finished, we setup for another recursion
        table = prop.table;
        i = -1;
    }
    // Error
    std::stringstream err << "Netvar got to level " << level << " and couldnt find through: ";
    for (std::string_view i : var_map)
        err << i;
    throw std::runtime_error(err.str());

    return 0;
}

// Generic stuff
NetVar<int> m_iTeamNum;
NetVar<int> m_iHealth;
NetVar<unsigned char> m_lifeState;
NetVar<CatVector> m_vecViewOffset;
// Aimbot
NetVar<int> m_nHitboxSet;
NetVar<float> m_flSimulationTime;
NetVar<int> m_hActiveWeapon;
NetVar<float> m_flLastFireTime;
NetVar<int> deadflag;
NetVar<int> m_iClip1;
// Team fortress 2
#if defined(NEKO_TF2)
NetVar<float> m_flNextPrimaryAttack;
NetVar<int> m_iBuildingHealth;
NetVar<int> m_iBuildingMaxHealth;
NetVar<int> m_iUpgradeLevel;
NetVar<int> m_iClass;
NetVar<int> res_iMaxHealth;
NetVar<int> res_iPlayerClass;
NetVar<std::bitset<128>> m_nPlayerCond;
NetVar<float> m_flChargedDamage;
NetVar<int> m_iItemDefinitionIndex;
NetVar<int> m_nForceTauntCam;
#endif

void Init() {
    // Generic
    m_iTeamNum.Set(GetOffset({"DT_BaseEntity", "m_iTeamNum"}));
    m_iHealth.Set(GetOffset({"DT_BasePlayer", "m_iHealth"}));
    m_lifeState.Set(GetOffset({"DT_BasePlayer", "m_lifeState"}));
    m_vecViewOffset.Set(
        GetOffset({"DT_BasePlayer", "localdata", "m_vecViewOffset[0]"}));
    // Aimbot related
    m_nHitboxSet.Set(GetOffset({"DT_BaseAnimating", "m_nHitboxSet"}));
    m_flSimulationTime.Set(GetOffset({"DT_BaseEntity", "m_flSimulationTime"}));
    m_hActiveWeapon.Set(
        GetOffset({"DT_BaseCombatCharacter", "m_hActiveWeapon"}));
    deadflag.Set(GetOffset({"DT_BasePlayer", "pl", "deadflag"}));
    m_iClip1.Set(
        GetOffset({"DT_BaseCombatWeapon", "LocalWeaponData", "m_iClip1"}));
    // Team fortress specific
#if defined(NEKO_TF2)
    m_flNextPrimaryAttack.Set(
        GetOffset({"DT_BaseCombatWeapon", "LocalActiveWeaponData",
                   "m_flNextPrimaryAttack"}));
    m_iBuildingHealth.Set(GetOffset({"DT_BaseObject", "m_iHealth"}));
    m_iBuildingMaxHealth.Set(GetOffset({"DT_BaseObject", "m_iMaxHealth"}));
    m_iUpgradeLevel.Set(GetOffset({"DT_BaseObject", "m_iUpgradeLevel"}));
    m_iClass.Set(GetOffset({"DT_TFPlayer", "m_PlayerClass", "m_iClass"}));
    res_iMaxHealth.Set(GetOffset({"DT_TFPlayerResource", "m_iMaxHealth"}));
    res_iPlayerClass.Set(GetOffset({"DT_TFPlayerResource", "m_iPlayerClass"}));
    m_nPlayerCond.Set(GetOffset({"DT_TFPlayer", "m_Shared", "m_nPlayerCond"}));
    m_flChargedDamage.Set(GetOffset(
        {"DT_TFSniperRifle", "SniperRifleLocalData", "m_flChargedDamage"}));
    m_iItemDefinitionIndex.Set(GetOffset({"DT_EconEntity", "m_AttributeManager",
                                          "m_Item", "m_iItemDefinitionIndex"}));
    m_flLastFireTime.Set(GetOffset(
        {"DT_TFWeaponBase", "LocalActiveTFWeaponData", "m_flLastFireTime"}));
    m_nForceTauntCam.Set(GetOffset({"DT_TFPlayer", "m_nForceTauntCam"}));
#endif
}

}  // namespace modules::source::netvars
