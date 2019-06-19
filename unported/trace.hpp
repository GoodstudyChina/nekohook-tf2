
#include "../../../framework/entitys.hpp"
#include "../hacks/ifaces.hpp"
#include "../util/mathlib.hpp"

namespace modules::source::trace {

// Trace line, returns end of trace
trace_t Terrain(const Vector&, const Vector&);

// Trace a line, returns entity hit or end of trace
trace_t Shot(const Vector&, const Vector&,
             IClientEntity* ignore = iface::entity_list->GetClientEntity(
                 iface::engine->GetLocalPlayer()));

// Check if line hits entity, this is for convienience
inline bool Entity(IClientEntity* entity, const Vector& src, const Vector& dest,
                   IClientEntity* ignore = iface::entity_list->GetClientEntity(
                       iface::engine->GetLocalPlayer())) {
    trace_t trace_result = Shot(src, dest, ignore);
    return reinterpret_cast<IClientEntity*>(trace_result.m_pEnt) == entity ||
           trace_result.fraction >= 0.99f;
}

// In front of src + angles
inline trace_t Front(
    const Vector& src, const Vector& angles,
    IClientEntity* ignore =
        iface::entity_list->GetClientEntity(iface::engine->GetLocalPlayer())) {
    return Shot(src,
                ToVector(util::ExtendLine(ToCatVector(src), ToCatVector(angles),
                                          8192.0f)),
                ignore);
}

// Whats in front of the player
inline trace_t Front(
    IClientEntity* entity,
    IClientEntity* ignore =
        iface::entity_list->GetClientEntity(iface::engine->GetLocalPlayer())) {
    Vector angle;
    iface::engine->GetViewAngles(angle);
    return Front(ToVector(GetCamera(reinterpret_cast<CatEntity*>(entity))),
                 angle, ignore);
}

// Init for the nekohook framework
void Init();

}  // namespace modules::source::trace
