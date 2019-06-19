
// NOTE: I am recreating everything needed for swingtrace here...
// Its exactly how the game does it so it should be accurate!
// NOTE: also do a check after wards that if u look directly at the player if a
// backstab would happen to make it an aimbot kinda too, would be cool to have
// it be more than a triggerbot

// NOTE: Uncomment things from the top down and u can see what makes sence and
// what doesnt

// bool enabled = false; // Tester var, needs a catvar

// Vector Weapon_ShootPosition(IClientEntity* entity) {
//    return entity->GetAbsOrigin(); // is fucked
/*Vector forward, right, up;
AngleVectors(entity->GetAbsAngles(), &forward, &right, &up);

return entity->GetAbsOrigin()
           + forward * m_HackedGunPos.y
           + right * m_HackedGunPos.x
           + up * m_HackedGunPos.z;*/
//}

/*class CTraceFilterSimple : public ITraceFilter {
public:
        CTraceFilterSimple(const IHandleEntity* pass_entity);
        virtual bool ShouldHitEntity(IHandleEntity* handle, int contents_mask) {
        IClientEntity* entity = reinterpret_cast<IClientEntity*>(handle);*/
// Commented our as its kinda iffy and it seems unneeded for the majority of
// cases
/*auto StandardFilterRules = [&]() -> bool {
    // Static prop case...
        if (!entity)
                return true;

        SolidType_t solid = pCollide->GetSolid();
        const model_t *pModel = pCollide->GetModel();

        if ((modelinfo->GetModelType(pModel) != mod_brush) || (solid !=
SOLID_BSP && solid != SOLID_VPHYSICS)) { if ((fContentsMask & CONTENTS_MONSTER)
== 0) return false;
        }

        // This code is used to cull out tests against see-thru entities
        if ( !(fContentsMask & CONTENTS_WINDOW) && pCollide->IsTransparent() )
                return false;

        // FIXME: this is to skip BSP models that are entities that can be
        // potentially moved/deleted, similar to a monster but doors don't seem
to
        // be flagged as monsters
        // FIXME: the FL_WORLDBRUSH looked promising, but it needs to be set on
        // everything that's actually a worldbrush and it currently isn't
        if ( !(fContentsMask & CONTENTS_MOVEABLE) && (pCollide->GetMoveType() ==
MOVETYPE_PUSH))// !(touch->flags & FL_WORLDBRUSH) ) return false;

        return true;
}
if (!StandardFilterRules(handle, contentsMask))
                return false;*/

// Don't test if the game code tells us we should ignore this collision...
/*if (!entity && entity == pass_entity)
        return false;

return true;
}
virtual void SetPassEntity(const IHandleEntity *pPassEntity ) { pass_ent =
pPassEntity; } virtual void SetCollisionGroup( int iCollisionGroup ) {
m_collisionGroup = iCollisionGroup; } private: const IHandleEntity* pass_entity;
};*/

/*inline void UTIL_TraceHull( const Vector &vecAbsStart, const Vector
&vecAbsEnd, const Vector &hullMin,  const Vector &hullMax,	unsigned int
mask, const IHandleEntity *ignore,  int collisionGroup, trace_t *ptr ) { Ray_t
ray; ray.Init( vecAbsStart, vecAbsEnd, hullMin, hullMax ); CTraceFilterSimple
traceFilter( ignore, collisionGroup );

        enginetrace->TraceRay( ray, mask, &traceFilter, ptr );
}

inline void UTIL_TraceLine( const Vector& vecAbsStart, const Vector& vecAbsEnd,
unsigned int mask, ITraceFilter *pFilter, trace_t *ptr ) { Ray_t ray; ray.Init(
vecAbsStart, vecAbsEnd );

        enginetrace->TraceRay( ray, mask, pFilter, ptr );
}
//https://github.com/VSES/SourceEngine2007/blob/43a5c90a5ada1e69ca044595383be67f40b33c61/se2007/game/shared/tf/tf_weaponbase_melee.cpp
bool DoSwingTrace(trace_t &trace) {
        // Setup a volume for the melee weapon to be swung - approx size, so all
melee behave the same. static Vector swing_mins( -18, -18, -18 ); static Vector
swing_maxs( 18, 18, 18 );

        // Get the current player.
        CTFPlayer* local_ent =
iface::entity_list->GetClientEntity(iface::engine->GetLocalPlayer()); if
(!local_ent) return false;

    Vector camera_angle;
        iface::engine->GetViewAngles(camera_angle);

        // Setup the swing range.
        Vector forward;
        AngleVectors(camera_angle, &forward);
        Vector swing_start = Weapon_ShootPosition();
        Vector swing_end = swing_start + forward * 48;

        // See if we hit anything.
        UTIL_TraceLine(swing_start, swing_end, MASK_SOLID, pPlayer,
COLLISION_GROUP_NONE, &trace); if ( trace.fraction >= 1.0 )
        {
                UTIL_TraceHull( vecSwingStart, vecSwingEnd, vecSwingMins,
vecSwingMaxs, MASK_SOLID, pPlayer, COLLISION_GROUP_NONE, &trace ); if (
trace.fraction < 1.0 )
                {
                        // Calculate the point of intersection of the line (or
hull) and the object we hit
                        // This is and approximation of the "best" intersection
                        CBaseEntity *pHit = trace.m_pEnt;
                        if ( !pHit || pHit->IsBSPModel() )
                        {
                                // Why duck hull min/max?
                                FindHullIntersection( vecSwingStart, trace,
VEC_DUCK_HULL_MIN, VEC_DUCK_HULL_MAX, pPlayer );
                        }

                        // This is the point on the actual surface (the hull
could have hit space) vecSwingEnd = trace.endpos;
                }
        }

        return ( trace.fraction < 1.0f );
}*/

/*void WorldTick() {

    // Swing trace
   https://github.com/VSES/SourceEngine2007/blob/43a5c90a5ada1e69ca044595383be67f40b33c61/se2007/game/shared/tf/tf_weaponbase_melee.cpp
    static Vector vecSwingMins( -18, -18, -18 );
    static Vector vecSwingMaxs( 18, 18, 18 );

    IClientEntity* local_ent =
   iface::entity_list->GetClientEntity(iface::engine->GetLocalPlayer()); if
   (!local_ent) return;

    // Setup the swing range.
    Vector forward;
    AngleVectors(local_ent->EyeAngles(), &forward);
    Vector vecSwingStart = pPlayer->Weapon_ShootPosition();
    Vector vecSwingEnd = vecSwingStart + vecForward * 48;

    Vector wsc = local_ent->WorldSpaceCenter();*/

/*int ent_count = iface::engine->GetMaxClients();
for(int i = 0; i < ent_count; i++) {
    IClientEntity* entity = iface::entity_list->GetClientEntity(i);
    if (!entity || entity->IsDormant() || netvars::m_lifeState.Get(entity) ||
local_ent == entity) continue;*/

// Ripped from
// https://github.com/VSES/SourceEngine2007/blob/43a5c90a5ada1e69ca044595383be67f40b33c61/se2007/game/shared/tf/tf_weapon_knife.cpp#L132
// Get the forward view vector of the target, ignore Z
/*Vector forward;
AngleVectors(entity->EyeAngles(), &forward, NULL, NULL );
forward.z = 0.0f;
forward.NormalizeInPlace();

// Get a vector from my origin to my targets origin
Vector target_vec = entity->WorldSpaceCenter() - wsc;
target_vec.z = 0.0f;
target_vec.NormalizeInPlace();

if (DotProduct(forward, target_vec) > -0.1) {
    hooks::ui_cmd->buttons |= IN_ATTACK;
}
}
}

void Init() {

}*/
