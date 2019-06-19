
/*
 *
 *	This is the modules player resource area for getting things like Names,
 *team, and etc.
 *
 */

#include "../hacks/ifaces.hpp"  // The games entitys
#include "classes.hpp"          // Classnames
#include "netvars.hpp"          // Netvar offsets

#include "playerresource.hpp"

namespace modules::source {

#if defined(NEKO_TF2)

// Used to store our player resource ent. We dont use a entity pointer as that
// might become invalid
static int player_resource_ent = 0;

// Cacheing the player resource entity is kinda needed
static IClientEntity* GetPlayerResource() {
    // Try to get our current player resource entity
    IClientEntity* entity =
        iface::entity_list->GetClientEntity(player_resource_ent);

    // Check if we have a player resource
    if (entity && !entity->IsDormant() &&
        entity->GetClientClass()->id == classes::CPlayerResource)
        return entity;

    // If we dont already have our player resource, we need to find it.
    player_resource_ent = 0;
    int ent_count = GetEntityCount();
    for (int i = 0; i < ent_count; i++) {
        entity = iface::entity_list->GetClientEntity(i);
        if (!entity || entity->IsDormant() ||
            entity->GetClientClass()->id != classes::CPlayerResource)
            continue;
        player_resource_ent = i;
        return entity;
    }

    // We dont have our resource...
    return nullptr;
}

// This is tf2 only stuff
int PRGetMaxHealth(int idx) {
    IClientEntity* resource = GetPlayerResource();
    if (!resource) return 100;

    return mem::Offset<int>(resource,
                            netvars::res_iMaxHealth.GetOffset() + 4 * idx);
}

tf::Class PRGetClass(int idx) {
    IClientEntity* resource = GetPlayerResource();
    if (!resource) return static_cast<tf::Class>(0);

    return mem::Offset<tf::Class>(
        resource, netvars::res_iPlayerClass.GetOffset() + 4 * idx);
}
#endif

}  // namespace modules::source
