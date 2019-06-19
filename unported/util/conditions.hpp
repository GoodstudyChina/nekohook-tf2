
/*
 *
 * Used to get conditions of entitys
 *
 */

#pragma once

#include <bitset>

#include "../../../framework/entitys.hpp"
#include "../util/netvars.hpp"

namespace modules::source {

enum TfCond {
    kSlowed = 0,
    kZoomed,
    kDisguising,
    kDisguised,
    kCloaked,
    kUbercharged,
    kTeleportedGlow,
    kTaunting,
    kUberchargeFading,
    kUnknown1,  // 9
    kCloakFlicker = 9,
    kTeleporting,
    kKritzkrieged,
    kUnknown2,  // 12
    kTmpDamageBonus = 12,
    kDeadRingered,
    kBonked,
    kDazed,
    kBuffed,
    kCharging,
    kDemoBuff,
    kCritCola,
    kInHealRadius,
    kHealing,
    kOnFire,
    kOverhealed,
    kJarated,  // 24?
    kBleeding,
    kDefenseBuffed,
    kMilked,
    kMegaHeal,
    kRegenBuffed,
    kMarkedForDeath,
    kNoHealingDamageBuff,
    kSpeedBuffAlly,  // 32
    kHalloweenCritCandy,
    kCritCanteen,
    kCritDemoCharge,
    kCritHype,
    kCritOnFirstBlood,
    kCritOnWin,
    kCritOnFlagCapture,
    kCritOnKill,
    kRestrictToMelee,
    kDefenseBuffNoCritBlock,
    kReprogrammed,
    kCritMmmph,
    kDefenseBuffMmmph,
    kFocusBuff,
    kDisguiseRemoved,
    kMarkedForDeathSilent,
    kDisguisedAsDispenser,
    kSapped,
    kUberchargedHidden,
    kUberchargedCanteen,
    kHalloweenBombHead,
    kHalloweenThriller,
    kRadiusHealOnDamage,
    kCritOnDamage,
    kUberchargedOnTakeDamage,
    kUberBulletResist,
    kUberBlastResist,
    kUberFireResist,
    kSmallBulletResist,
    kSmallBlastResist,
    kSmallFireResist,
    kStealthed,  // 64
    kMedigunDebuff,
    kStealthedUserBuffFade,
    kBulletImmune,
    kBlastImmune,
    kFireImmune,
    kPreventDeath,
    kMVMBotRadiowave,
    kHalloweenSpeedBoost,
    kHalloweenQuickHeal,
    kHalloweenGiant,
    kHalloweenTiny,
    kHalloweenInHell,
    kHalloweenGhostMode,
    kMiniCritOnKill,
    kDodgeChance,  // 79
    kObscuredSmoke = 79,
    kParachute,
    kBlastJumping,
    kHalloweenKart,
    kHalloweenKartDash,
    kBalloonHead,
    kMeleeOnly,
    kSwimmingCurse,
    kHalloweenKartNoTurn,  // 87
    kFreezeInput = 87,
    kHalloweenKartCage,
    kHasRune,
    kRuneStrength,
    kRuneHaste,
    kRuneRegen,
    kRuneResist,
    kRuneVampire,
    kRuneWarlock,
    kRunePrecision,  // 96
    kRuneAgility,
    kGrapplingHook,
    kGrapplingHookSafeFall,
    kGrapplingHookLatched,
    kGrapplingHookBleeding,
    kAfterburnImmune,
    kRuneKnockout,
    kRuneImbalance,
    kCritRuneTemp,
    kPasstimeInterception,
    kSwimmingNoEffects,
    kEyeaductUnderworld,
    kKingRune,
    kPlagueRune,
    kSupernovaRune,
    kPlague,
    kKingAura,
    kSpawnOutline,  // 114
    kKnockedIntoAir,
    kCompetitiveWinner,
    kCompetitiveLoser,
    kNoTaunting
};

// Preset groups
const std::initializer_list<const modules::source::TfCond>
    TfCond_Invulnerability = {TfCond::kUbercharged,
                              TfCond::kUberchargedCanteen,
                              TfCond::kUberchargedHidden,
                              TfCond::kUberchargedOnTakeDamage,
                              TfCond::kBonked,
                              TfCond::kDefenseBuffMmmph};
const std::initializer_list<const modules::source::TfCond> TfCond_CritBoost = {
    TfCond::kKritzkrieged,      TfCond::kCritRuneTemp, TfCond::kCritCanteen,
    TfCond::kCritMmmph,         TfCond::kCritOnKill,   TfCond::kCritOnDamage,
    TfCond::kCritOnFirstBlood,  TfCond::kCritOnWin,    TfCond::kCritRuneTemp,
    TfCond::kHalloweenCritCandy};

// Checking for conditions
template <typename Entity>
inline bool HasCondition(Entity entity, TfCond cond) {
    return netvars::m_nPlayerCond.Get(entity)[cond];
}

template <typename Entity>
inline bool HasCondition(Entity entity,
                         const std::initializer_list<const TfCond> cond_group) {
    for (const auto& cond : cond_group)
        if (HasCondition(entity, cond)) return true;
    return false;
}

// Setting of conditions
template <typename Entity>
inline bool SetCondition(Entity entity, TfCond cond, bool value) {
    netvars::m_nPlayerCond.Get(entity)[cond] = value;
    return value;
}

template <typename Entity>
inline bool SetCondition(Entity entity,
                         const std::initializer_list<const TfCond> cond_group,
                         bool value) {
    for (const auto& cond : cond_group) SetCondition(entity, cond, value);
    return value;
}

// Conditions not set in bits

bool CanHeadshot();

namespace conditions {

void Init();

}

}  // namespace modules::source
