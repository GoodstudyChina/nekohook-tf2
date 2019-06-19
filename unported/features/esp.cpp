
/*
 *
 *	Esp additions
 *
 */

#include <bitset>

#include "../../../features/esp.hpp"  // So we can get the esp menu location so we can add to it
#include "../../../framework/entitys.hpp"  // So we can use entitys
#include "../hacks/ifaces.hpp"             // Get client entitys
#include "../util/classes.hpp"             // Slender esp
#include "../util/netvars.hpp"  // Contains definition for use of netvars
#include "../util/playerresource.hpp"
#if defined(NEKO_TF2)
#include "../util/conditions.hpp"
#endif

#include "esp.hpp"

namespace modules::source::features::esp {

static CatVarBool show_class(::features::esp::esp_menu, "esp_class", true,
                             "Show Classes");
static CatVarBool slender(::features::esp::esp_menu, "esp_slender", false,
                          "Show pages", "Shows pages in slender gamemode");
static CatVarBool condition_debug(::features::esp::esp_menu,
                                  "esp_debug_conditions", false,
                                  "Show condition debug");
static CatVarBool show_entity_classnames(::features::esp::esp_menu,
                                         "esp_debug_classname", false,
                                         "Show Entity Class Name",
                                         "Shows entitys classnames");
static CatVarBool show_team_num(::features::esp::esp_menu, "esp_debug_teamnum",
                                false, "Show Entity Team Name",
                                "debug team nums");
void AddEspStrings(std::vector<std::pair<const char*, CatColor>>& str_cache,
                   CatEntity* entity) {
#if defined(NEKO_TF2)
    if (show_class && GetType(entity) == ent::Type::kPlayer) {
        const char* class_name = nullptr;
        switch (PRGetClass(entity)) {
            case tf::Class::kScout:
                class_name = "Scout";
                break;
            case tf::Class::kSniper:
                class_name = "Sniper";
                break;
            case tf::Class::kSoldier:
                class_name = "Soldier";
                break;
            case tf::Class::kDemoman:
                class_name = "Demoman";
                break;
            case tf::Class::kMedic:
                class_name = "Medic";
                break;
            case tf::Class::kHeavy:
                class_name = "Heavy";
                break;
            case tf::Class::kPyro:
                class_name = "Pyro";
                break;
            case tf::Class::kSpy:
                class_name = "Spy";
                break;
            case tf::Class::kEngineer:
                class_name = "Engineer";
                break;
            default:
                class_name = "Unknown";
        }
        str_cache.push_back({class_name, colors::EntityColor(entity)});
    }
    // Slender gamemode LMAOOOOO
    if (slender) {
        // Page esp
        // Here we get the model name if needed
        static const model_t* page_model = nullptr;
        auto ent_model = ((IClientEntity*)entity)->GetModel();
        if (!page_model && ent_model &&
            strstr(iface::model_info->GetModelName(ent_model),
                   "models/slender/sheet.mdl"))
            page_model = ent_model;
        // Check if model is sheet
        if (page_model && ent_model == page_model)
            str_cache.push_back({"-Page-", colors::green});

        // Slenderman esp
        // TODO, we need a way to tell the difference between an actual tf2 boss
        // and a slender one
        else if (classes::CTFBaseBoss ==
                 ((IClientEntity*)entity)->GetClientClass()->id)
            str_cache.push_back({"-Boss-", colors::orange});
    }

    // Condition debug
    if (condition_debug && GetType(entity) == ent::Type::kPlayer) {
        static char uhh[256];
        strcpy(uhh, netvars::m_nPlayerCond.Get(entity).to_string().c_str());
        str_cache.push_back({uhh, colors::white});
    }
#endif

    // Classname esp
    if (show_entity_classnames)
        str_cache.push_back(
            {((IClientEntity*)entity)->GetClientClass()->name, colors::white});

    // Team number
    if (show_team_num && GetType(entity) == ent::Type::kPlayer) {
        static char buf[12];
        sprintf(buf, "Team: %i", netvars::m_iTeamNum.Get(entity));
        str_cache.push_back({buf, colors::white});
        ;
    }
}

// Function to run on init
static CatVarBool show_cloaked(::features::esp::esp_menu, "esp_cloaked", true,
                               "Show cloaked spies");
void Init() {
    ::features::esp::GetEntityStrings = AddEspStrings;
#if defined(NEKO_TF2)
    ::features::esp::ShouldEsp = [](CatEntity* entity) -> bool {
        if (GetType(entity) == ent::Type::kPlayer) {
            if (PRGetClass(entity) == tf::Class::kSpy) {
                if (!show_cloaked && HasCondition(entity, TfCond::kCloaked))
                    return false;
            }
        }
        return true;
    };
#endif
}

}  // namespace modules::source::features::esp
