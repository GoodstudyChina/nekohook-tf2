
/*
 *
 * Aimbot stuff
 *
 *
 */

#include "../../../features/aimbot.hpp"
#include "../../../framework/game.hpp"
#include "../../../util/catvars.hpp"
#include "../hacks/ifaces.hpp"
#include "../util/classes.hpp"
#include "../util/netvars.hpp"
#if defined(NEKO_TF2)
#include "../util/conditions.hpp"
#include "../util/playerresource.hpp"
#endif

#include "aimbot.hpp"

namespace modules::source::features::aimbot {

static CatVector AutoHitbox(CatEntity* entity) {
// Team fortress 2 check
#if defined(NEKO_TF2)
    // Get our active weapon
    auto local_ent =
        iface::entity_list->GetClientEntity(iface::engine->GetLocalPlayer());
    if (!local_ent || local_ent->IsDormant())
        return ::features::aimbot::RetrieveAimpoint(entity, 4);
    auto weapon = iface::entity_list->GetClientEntity(
        netvars::m_hActiveWeapon.Get(local_ent) & 0xFFF);
    if (!weapon || weapon->IsDormant())
        return ::features::aimbot::RetrieveAimpoint(entity, 4);
    int wep_id = weapon->GetClientClass()->id;

    // Sniper rifle
    if (wep_id == classes::CTFSniperRifle ||
        wep_id == classes::CTFSniperRifleDecap) {
        // Check if can headshot
        if (HasCondition(local_ent, TfCond::kZoomed) && CanHeadshot()) {
            // Bodyshot handler
            float bdmg = netvars::m_flChargedDamage.Get(weapon);
            // Vaccinator damage correction, protects against 75% of damage
            if (HasCondition(entity, TfCond::kUberBulletResist)) bdmg *= 0.25f;
            // Passive bullet resist protects against 10% of damage
            else if (HasCondition(entity, TfCond::kSmallBulletResist))
                bdmg *= 0.90f;
            // Invis damage correction, Invis spies get protection from 10% of
            // damage
            if (HasCondition(entity, TfCond::kCloaked)) bdmg *= 0.90f;
            // If we cant kill with a bodyshot, use a headshot
            auto health = GetHealth(entity);
            if (bdmg < health && !HasCondition(local_ent, TfCond_CritBoost))
                return ::features::aimbot::RetrieveAimpoint(entity, 3);
        }
        return ::features::aimbot::RetrieveAimpoint(entity, 4);
    }
    // Hunstman
    if (wep_id == classes::CTFCompoundBow)
        return ::features::aimbot::RetrieveAimpoint(entity, 3);
    // Ambassador
    auto wep_IDX = netvars::m_iItemDefinitionIndex.Get(weapon);
    if (wep_IDX == 61 || wep_IDX == 1006) {
        if (GetHealth(entity) <= 18 ||
            HasCondition(local_ent, TfCond_CritBoost) ||
            iface::global_vars->curtime -
                    netvars::m_flLastFireTime.Get(weapon) <=
                1.0) {
            return ::features::aimbot::RetrieveAimpoint(entity, 4);
        }
        return ::features::aimbot::RetrieveAimpoint(entity, 3);
    }
#elif defined(NEKO_FOF)
    // Head
    ::features::aimbot::RetrieveAimpoint(entity, 3);
#endif
    // Nothing to do, return center
    return ::features::aimbot::RetrieveAimpoint(entity, 4);
}

#if defined(NEKO_TF2)
static CatVarBool waitforcharge(::features::aimbot::aimbot_menu,
                                "ab_waitforcharge", false, "Wait for charge");
static CatVarBool ignore_taunting(::features::aimbot::aimbot_menu,
                                  "ab_ignoretaunt", false, "Ignore taunting");
static CatVarBool ignore_cloak(::features::aimbot::aimbot_menu,
                               "ab_ignorecloak", false, "Ignore Cloaked");
static CatVarBool ignore_ringer(::features::aimbot::aimbot_menu,
                                "ab_ignoreringer", false, "Ignore Deadringer");
#endif

static bool TargetCheck(CatEntity* entity) {
#if defined(NEKO_TF2)
    if (waitforcharge) {
        // We need the weapon so we use this mess
        auto local_ent = iface::entity_list->GetClientEntity(
            iface::engine->GetLocalPlayer());
        if (!local_ent || local_ent->IsDormant()) return true;
        auto weapon = iface::entity_list->GetClientEntity(
            netvars::m_hActiveWeapon.Get(local_ent) & 0xFFF);
        if (!weapon || weapon->IsDormant()) return true;
        int wep_id = weapon->GetClientClass()->id;
        // check if snipar wep
        if (wep_id != classes::CTFSniperRifle &&
            wep_id != classes::CTFSniperRifleDecap)
            return true;
        // Check if zoom
        if (!HasCondition(local_ent, TfCond::kZoomed)) return true;

        // Get charge damage
        float cdmg = netvars::m_flChargedDamage.Get(weapon) * 3;
        // Vaccinator damage correction, protects against 75% of damage
        if (HasCondition(entity, TfCond::kUberBulletResist)) cdmg *= 0.25f;
        // Passive bullet resist protects against 10% of damage
        else if (HasCondition(entity, TfCond::kSmallBulletResist))
            cdmg *= 0.90f;
        // Invis damage correction, Invis spies get protection from 10% of
        // damage
        if (HasCondition(entity, TfCond::kCloaked)) cdmg *= 0.90f;

        // uhh
        auto health = GetHealth(entity);
        if (health <= 150) return true;
        if ((cdmg < health && !HasCondition(local_ent, TfCond_CritBoost)) ||
            cdmg == 450)
            return false;
    }
    // Player only
    if (((IClientEntity*)entity)->GetClientClass()->id == classes::CPlayer) {
        if (ignore_taunting && HasCondition(entity, TfCond::kTaunting))
            return false;

        if (ignore_cloak || ignore_ringer)
            if (HasCondition(entity, TfCond::kCloaked)) return false;
        // if (HasWeapon(entity, 59)) // Item id for deadringer is 59 as of time
        // of creation
        //  return !ignore_deadringer;
        // else
        // Invincible check
        if (HasCondition(entity, TfCond_Invulnerability)) return false;
    }
#endif
    return true;
}

static CatVarBool only_zoomed(::features::aimbot::aimbot_menu, "ab_zoomed_only",
                              false, "Zoomed only");
bool ShouldAimModule(CatEntity* local_ent) {
    tf::Class tf_class = PRGetClass(local_ent);
    if (tf_class == tf::Class::kSniper) {
        if (HasCondition(local_ent, TfCond::kZoomed)) {
            if (!CanHeadshot()) return false;
        } else if (only_zoomed)
            return false;
    }
    return true;
}

void Init() {
    ::features::aimbot::TargetSelectionModule = TargetCheck;
    ::features::aimbot::GetAutoHitbox = AutoHitbox;
    ::features::aimbot::ShouldAimModule = ShouldAimModule;
}

}  // namespace modules::source::features::aimbot
