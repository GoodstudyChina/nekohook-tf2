
/*
 *
 *	This is a interface for the entity manager to the tf2 module.
 *
 *
 */

#include "../../../framework/entitys.hpp"  // Our entity manager we want to fill
#include "../../../framework/game.hpp"     // Our entity manager we want to fill
#include "../../../util/logging.hpp"
#include "../hacks/hooks.hpp"   // Has interfaces to the game that we need
#include "../hacks/ifaces.hpp"  // Has interfaces to the game that we need
#include "../util/classes.hpp"  // Enables us to use class num gen
#include "../util/mathlib.hpp"  // To convert between catvectors and source vectors
#include "../util/netvars.hpp"         // Contains definition for use of netvars
#include "../util/playerresource.hpp"  // To retrive player information

#include "entitys.hpp"

namespace modules::source::framework::entitys {

bool GetDormant(CatEntity* this_ptr) {
    return ((IClientEntity*)this_ptr)->IsDormant();
}
bool GetAlive(CatEntity* this_ptr) {
    return !netvars::m_lifeState.Get(this_ptr);
}
ent::Type GetType(CatEntity* this_ptr) {
    // World
    if (((IClientEntity*)this_ptr)->entindex() == 0) return ent::Type::kGeneric;

    // Players
    int cl = ((IClientEntity*)this_ptr)->GetClientClass()->id;
    if (cl == classes::CPlayer) return ent::Type::kPlayer;
#if defined(NEKO_TF2)
    // Buildings
    else if (cl == classes::CObjectTeleporter ||
             cl == classes::CObjectSentrygun || cl == classes::CObjectDispenser)
        return ent::Type::kOtherHostile;
    // Projectiles
    else if (cl == classes::CTFProjectile_Arrow ||
             cl == classes::CTFProjectile_EnergyBall ||
             cl == classes::CTFProjectile_EnergyRing ||
             cl == classes::CTFProjectile_GrapplingHook ||
             cl == classes::CTFProjectile_HealingBolt ||
             cl == classes::CTFProjectile_Rocket ||
             cl == classes::CTFProjectile_SentryRocket ||
             cl == classes::CTFProjectile_Flare ||
             cl == classes::CTFGrenadePipebombProjectile ||
             cl == classes::CTFProjectile_Cleaver ||
             cl == classes::CTFProjectile_Jar ||
             cl == classes::CTFProjectile_JarMilk)
        return ent::Type::kProjectile;
#endif
    // Unknown
    return ent::Type::kGeneric;
}
int GetHealth(CatEntity* this_ptr) {
    auto type = GetType(this_ptr);
    if (type == ent::Type::kPlayer) return netvars::m_iHealth.Get(this_ptr);
#if defined(NEKO_TF2)
    if (type == ent::Type::kOtherHostile)
        return netvars::m_iBuildingHealth.Get(this_ptr);
#endif
    return 100;
}
int GetMaxHealth(CatEntity* this_ptr) {
#if defined(NEKO_TF2)
    auto type = GetType(this_ptr);
    if (type == ent::Type::kPlayer) return PRGetMaxHealth(this_ptr);

    if (type == ent::Type::kOtherHostile)
        return netvars::m_iBuildingMaxHealth.Get(this_ptr);
#endif
    return 100;
}
ent::Team GetTeam(CatEntity* this_ptr) {
    // Get team info
    switch (netvars::m_iTeamNum.Get(this_ptr)) {
#if defined(NEKO_FOF)
        case 0:  // Probs deathmatch, everyone is enemys
            return ent::Team::kEnemy;
        case 2:  // Blue
            return ent::Team::kBlue;
        case 3:  // Red
            return ent::Team::kRed;
        case 4:  // Yellow
            return ent::Team::kYellow;
        case 5:  // Green
            return ent::Team::kGreen;
#else  // Generic class
        case 1:  // Unknown
            return ent::Team::kUnknown;
        case 2:  // Red
            return ent::Team::kRed;
        case 3:  // Blue
            return ent::Team::kBlue;
#endif
    }
    return ent::Team::kEnemy;
}

const char* GetName(CatEntity* this_ptr) {
    auto type = GetType(this_ptr);
    if (type == ent::Type::kPlayer) {
        static player_info_s info;
        if (iface::engine->GetPlayerInfo(((IClientEntity*)this_ptr)->entindex(),
                                         &info)) {
            return info.name;
        }
    }
#if defined(NEKO_TF2)
    if (type == ent::Type::kOtherHostile) {
        auto cl = ((IClientEntity*)this_ptr)->GetClientClass()->id;
        if (cl == classes::CObjectSentrygun)
            return "Sentrygun";
        else if (cl == classes::CObjectDispenser)
            return "Dispenser";
        else if (cl == classes::CObjectTeleporter)
            return "Teleporter";
    }
#endif
    return "unknown";
}
CatVector GetOrigin(CatEntity* this_ptr) {
    return ToCatVector(((IClientEntity*)this_ptr)->GetAbsOrigin());
}
CatBox GetCollision(CatEntity* this_ptr) {
    auto collidable = ((IClientEntity*)this_ptr)->GetCollideable();
    auto collision_origin = collidable->GetCollisionOrigin();
    return CatBox(ToCatVector(collidable->OBBMins() + collision_origin),
                  ToCatVector(collidable->OBBMaxs() + collision_origin));
};

bool GetBone(CatEntity* this_ptr, ent::Bone bone, CatBox& input) {
#if defined(NEKO_FOF)
    // TODO, force setup bones
    // TODO, 55 89 E5 53 83 EC 24 A1 ? ? ? ? 8B 5D 08 F3 0F 10 45
    // we can always use force setup bones
    return false;
#endif

    // Test type
    if (GetType(this_ptr) != ent::Type::kPlayer) return false;

    // Map used to map source bones to. Position in array is source bone and
    // value is bone it is to the framework NOTE: yes f1ssi0n, this is slow but
    // its for modularity, if you want fast then go enjoy your game specific
    // cheat >_<
    enum hitbox_t {
        head,
        pelvis,
        spine_0,
        spine_1,
        spine_2,
        spine_3,
        upperArm_L,
        lowerArm_L,
        hand_L,
        upperArm_R,
        lowerArm_R,
        hand_R,
        hip_L,
        knee_L,
        foot_L,
        hip_R,
        knee_R,
        foot_R
    };
    const static std::initializer_list<std::pair<ent::Bone, int>> bone_map{
        // Generic maps for most games
        {ent::Bone::kHead, hitbox_t::head},
        {ent::Bone::kSpineBottom, hitbox_t::pelvis},
        {ent::Bone::kSpineLower, hitbox_t::spine_0},
        {ent::Bone::kSpineMiddle, hitbox_t::spine_1},
        {ent::Bone::kSpineUpper, hitbox_t::spine_2},
        {ent::Bone::kSpineTop, hitbox_t::spine_3},  // Center
        {ent::Bone::kLUpperArm, hitbox_t::upperArm_L},
        {ent::Bone::kLMiddleArm, hitbox_t::lowerArm_L},
        {ent::Bone::kLLowerArm, hitbox_t::hand_L},
        {ent::Bone::kRUpperArm, hitbox_t::upperArm_R},
        {ent::Bone::kRMiddleArm, hitbox_t::lowerArm_R},
        {ent::Bone::kRLowerArm, hitbox_t::hand_R},
        {ent::Bone::kLUpperLeg, hitbox_t::hip_L},
        {ent::Bone::kLMiddleLeg, hitbox_t::knee_L},
        {ent::Bone::kLLowerLeg, hitbox_t::foot_L},
        {ent::Bone::kRUpperLeg, hitbox_t::hip_R},
        {ent::Bone::kRMiddleLeg, hitbox_t::knee_R},
        {ent::Bone::kRLowerLeg, hitbox_t::foot_R}};
    // Get a bone
    const auto use_bone = std::find_if(bone_map.begin(), bone_map.end(),
                                       [&](auto i) { return i.first == bone; });
    if (use_bone == bone_map.end()) return false;

    // Try to get a model
    model_t* model = (model_t*)((IClientEntity*)this_ptr)->GetModel();
    if (!model) return false;

    // Use the studio model to get a hitbox set
    mstudiohitboxset_t* hitbox_set =
        iface::model_info->GetStudiomodel(model)->pHitboxSet(
            netvars::m_nHitboxSet.Get(this_ptr));
    if (!hitbox_set) return false;

    // Create a storage vector for bones and setup them
    matrix3x4_t bones[128];
    // TODO, curtime
    //#if defined(CATHOOK_FOF)
    //  if (!((IClientEntity*)this_ptr)->ForceSetupBonesAtTime(bones,
    //  NET_VAR(this_ptr, netvars::m_flSimulationTime, float))) return false;
    //#else
    if (!((IClientEntity*)this_ptr)
             ->SetupBones(bones, 128, 0x100, iface::global_vars->curtime))
        return false;
    //#endif

    // Get our hitbox
    mstudiobbox_t* bbox = hitbox_set->pHitbox(use_bone->second);
    if (!bbox) return false;

    auto HitBoxTransform = [](Vector in1, matrix3x4_t in2) -> auto {
        return CatVector(
            (in1.x * in2[0][0] + in1.y * in2[0][1] + in1.z * in2[0][2]) +
                in2[0][3],
            (in1.x * in2[1][0] + in1.y * in2[1][1] + in1.z * in2[1][2]) +
                in2[1][3],
            (in1.x * in2[2][0] + in1.y * in2[2][1] + in1.z * in2[2][2]) +
                in2[2][3]);
    };
    // Get and return hitbox
    input = CatBox(HitBoxTransform(bbox->bbmin, bones[bbox->bone]),
                   HitBoxTransform(bbox->bbmax, bones[bbox->bone]));

    return true;
}

CatVector GetCamera(CatEntity* this_ptr) {
    if (((IClientEntity*)this_ptr)->IsDormant()) return CatVector();
    return GetOrigin((CatEntity*)this_ptr) +
           netvars::m_vecViewOffset.Get(this_ptr);
}
CatVector GetCameraAngle(CatEntity* this_ptr) {
    Vector angle;
    iface::engine->GetViewAngles(angle);
    return ToCatVector(angle);
}
void SetCameraAngle(CatEntity* this_ptr, const CatVector& in) {
    *(CatVector*)&hooks::ui_cmd->viewangles = in;
    iface::engine->SetViewAngles(hooks::ui_cmd->viewangles);
    hooks::using_silent = false;
}
void SetSilentCameraAngle(CatEntity* this_ptr, const CatVector& in) {
    *(CatVector*)&hooks::ui_cmd->viewangles = in;
    hooks::using_silent = true;
}
void Attack(CatEntity* this_ptr) { hooks::ui_cmd->buttons |= IN_ATTACK; }

void Jump(CatEntity* this_ptr) { hooks::ui_cmd->buttons |= IN_JUMP; }

void WalkTo(CatEntity* this_ptr, CatVector dest) {
    auto origin = GetOrigin(this_ptr);
    CatVector delta = dest - origin;
    delta.z = 0;
    float yaw = DEG2RAD(VectorAngles(delta).y - hooks::ui_cmd->viewangles.y);
    hooks::ui_cmd->forwardmove = cos(yaw) * 450;
    hooks::ui_cmd->sidemove = -sin(yaw) * 450;
}

CatEntity* GetEntity(int idx) {
    return reinterpret_cast<CatEntity*>(
        iface::entity_list->GetClientEntity(idx));
}
CatEntity* GetLocalPlayer() {
    return reinterpret_cast<CatEntity*>(
        iface::entity_list->GetClientEntity(iface::engine->GetLocalPlayer()));
}
int GetEntityCount() { return iface::entity_list->GetHighestEntityIndex(); }

void Init() {
    ::GetDormant = GetDormant;
    ::GetAlive = GetAlive;
    ::GetHealth = GetHealth;
    ::GetMaxHealth = GetMaxHealth;
    ::GetTeam = GetTeam;
    ::GetType = GetType;
    ::GetName = GetName;
    ::GetOrigin = GetOrigin;
    ::GetCollision = GetCollision;
    ::GetBone = GetBone;
    // Local ent
    ::GetCamera = GetCamera;
    ::GetCameraAngle = GetCameraAngle;
    ::SetCameraAngle = SetCameraAngle;
    ::SetSilentCameraAngle = SetSilentCameraAngle;
    ::Attack = Attack;
    ::Jump = Jump;
    ::WalkTo = WalkTo;
    // Major ent stuff
    ::GetEntity = GetEntity;
    ::GetLocalPlayer = GetLocalPlayer;
    ::GetEntityCount = GetEntityCount;
    game::GetInGame = []() { return iface::engine->IsInGame(); };
}

}  // namespace modules::source::framework::entitys
