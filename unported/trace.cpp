
/*
 *
 * For source engine tracing
 *
 */

#include "../../../framework/trace.hpp"
#include "../../../framework/entitys.hpp"
#include "../hacks/ifaces.hpp"
#include "../util/classes.hpp"
#include "../util/mathlib.hpp"

namespace modules::source::trace {

// We do this similer to the ssdk
class TraceFilter : public ITraceFilter {
   public:
    TraceFilter(
        IClientEntity* _ignore_target = iface::entity_list->GetClientEntity(
            iface::engine->GetLocalPlayer()))
        : ignore_target(_ignore_target) {}
    TraceType_t GetTraceType() const { return TRACE_EVERYTHING; }
    virtual bool ShouldHitEntity(IHandleEntity* handle, int mask) {
        IClientEntity* entity = reinterpret_cast<IClientEntity*>(handle);
        if (!entity || entity == ignore_target) return false;

#if defined(NEKO_TF2)
        // Ignore invisible entities that we don't wanna hit
        switch (entity->GetClientClass()->id) {
            case 64:   // CFuncRespawnRoomVisualizer
            case 230:  // CTFMedigunShield
            case 55:   // CFuncAreaPortalWindow
                return false;
        }
#endif

        return true;
    }

   private:
    IClientEntity* ignore_target;
};

// Trace line, returns end of trace
trace_t Terrain(const Vector& src, const Vector& dest) {
    Ray_t ray;
    ray.Init(src, dest);

    TraceFilter filter(nullptr);  // TODO, replace with a proper terrain filter
    trace_t trace_result;
    iface::trace->TraceRay(ray, 0x46004003, &filter, &trace_result);

    return trace_result;
}

// Trace a line, returns entity hit or end of trace
trace_t Shot(const Vector& src, const Vector& dest, IClientEntity* ignore) {
    Ray_t ray;
    ray.Init(src, dest);

    TraceFilter filter(ignore);
    trace_t trace_result;
    iface::trace->TraceRay(ray, 0x46004003, &filter, &trace_result);

    return trace_result;
}

void Init() {
    // Trace line to get entity
    ::trace::Terrain = [](const CatVector& src,
                          const CatVector& dest) -> CatVector {
        trace_t trace_result = Terrain(ToVector(src), ToVector(dest));
        return ToCatVector(trace_result.endpos);
    };

    // Trace a line, returns the end vector and entity if hit
    ::trace::Shot = [](const CatVector& src, const CatVector& dest,
                       CatEntity* ignore) -> std::pair<CatEntity*, CatVector> {
        trace_t trace_result = Shot(ToVector(src), ToVector(dest),
                                    reinterpret_cast<IClientEntity*>(ignore));
        return {reinterpret_cast<CatEntity*>(trace_result.m_pEnt),
                ToCatVector(trace_result.endpos)};
    };
}

}  // namespace modules::source::trace
