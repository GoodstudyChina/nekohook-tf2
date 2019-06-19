
/*
 *
 *	The resource files header.
 *
 *
 */

#include "../../../framework/entitys.hpp"  // So we can use CatEntitys
#include "../sdk/sdk.hpp"

namespace modules::source {

#if defined(NEKO_TF2)

namespace tf {
enum Class {
    kScout = 1,
    kSniper,
    kSoldier,
    kDemoman,
    kMedic,
    kHeavy,
    kPyro,
    kSpy,
    kEngineer
};
}

int PRGetMaxHealth(int idx);
tf::Class PRGetClass(int idx);

inline int PRGetMaxHealth(IClientEntity* player) {
    return PRGetMaxHealth(player->entindex());
}

inline tf::Class PRGetClass(IClientEntity* player) {
    return PRGetClass(player->entindex());
}

inline int PRGetMaxHealth(CatEntity* player) {
    return PRGetMaxHealth((IClientEntity*)player);
}
inline tf::Class PRGetClass(CatEntity* player) {
    return PRGetClass((IClientEntity*)player);
}

#endif

}  // namespace modules::source
