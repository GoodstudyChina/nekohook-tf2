
/*
 *
 *	Classes
 *
 *
 */

#include "../../../framework/game.hpp"  // So we can get ingame state
#include "../../../util/logging.hpp"    // Logging is cool
#include "../hacks/ifaces.hpp"          // for g_IBaseClient and sdk

#include "classes.hpp"

namespace modules::source::classes {

// used for magic
static int GetClassNum(const char* input) {
    // We want to find a class with the name input here
    for (ClientClass* i = iface::client->GetAllClasses(); i != nullptr;
         i = i->next)
        if (!strcmp(i->name, input))
            return i->id;  // We found our class, return
    debug_log.Fmt("ClassGen: Couldnt find %s", input);
    return -1;
}

// Players
int CPlayer = -1;
int CPlayerResource = -1;
#if defined(NEKO_TF2)
// Buildings
int CObjectSentrygun = -1;
int CObjectDispenser = -1;
int CObjectTeleporter = -1;
// Weapons
int CTFCannon = -1;
int CTFPipebombLauncher = -1;
// Projectiles
int CTFProjectile_Arrow = -1;
int CTFProjectile_EnergyBall = -1;
int CTFProjectile_EnergyRing = -1;
int CTFProjectile_GrapplingHook = -1;
int CTFProjectile_HealingBolt = -1;
int CTFProjectile_Rocket = -1;
int CTFProjectile_SentryRocket = -1;
int CTFProjectile_Flare = -1;
int CTFGrenadePipebombProjectile = -1;
int CTFProjectile_Cleaver = -1;
int CTFProjectile_Jar = -1;
int CTFProjectile_JarMilk = -1;
int CTFSniperRifle = -1;
int CTFSniperRifleDecap = -1;
int CTFCompoundBow = -1;
// slender
int CTFBaseBoss = -1;
#endif

#define InitClass(x) \
    { x = GetClassNum(#x); }

void Init() {
    //  This needs to be done ingame
    if (!iface::engine->IsInGame()) return;
    events::world.Remove(Init);

    // Players
#if defined(NEKO_TF2)
    CPlayer = GetClassNum("CTFPlayer");
    CPlayerResource = GetClassNum("CTFPlayerResource");
    // Buildings
    InitClass(CObjectSentrygun);
    InitClass(CObjectDispenser);
    InitClass(CObjectTeleporter);
    // Weapons
    InitClass(CTFCannon);
    InitClass(CTFPipebombLauncher);
    // Projectiles
    InitClass(CTFProjectile_Arrow);
    InitClass(CTFProjectile_EnergyBall);
    InitClass(CTFProjectile_EnergyRing);
    InitClass(CTFProjectile_GrapplingHook);
    InitClass(CTFProjectile_HealingBolt);
    InitClass(CTFProjectile_Rocket);
    InitClass(CTFProjectile_SentryRocket);
    InitClass(CTFProjectile_Flare);
    InitClass(CTFGrenadePipebombProjectile);
    InitClass(CTFProjectile_Cleaver);
    InitClass(CTFProjectile_Jar);
    InitClass(CTFProjectile_JarMilk);
    // Weapons
    InitClass(CTFSniperRifle);
    InitClass(CTFSniperRifleDecap);
    InitClass(CTFCompoundBow);
    InitClass(CTFBaseBoss);
#elif defined(NEKO_FOF)
    CPlayer = GetClassNum("CFoF_Player");
#endif
}

}  // namespace modules::source::classes
