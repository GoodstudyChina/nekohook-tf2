
/*
 *
 *	We do hooking here!
 *	Eventually you will be able to pass your function into here to do stuff
 *with the hooked functions
 *
 *
 */

#include <cstdlib>  // Needed to get client mode
#include <thread>

#include "../../../framework/game.hpp"
#include "../../../hacks/hooking.hpp"
#include "../../../hacks/signature.hpp"
#include "../../../util/catvars.hpp"
#include "../../../util/chrono.hpp"
#include "../../../util/logging.hpp"
#include "../../../util/platform.hpp"
#include "../util/mathlib.hpp"
#include "../util/netvars.hpp"
#include "../util/worldtoscreen.hpp"
#if defined(NEKO_TF2)
#include "../util/conditions.hpp"
#endif
#include "ifaces.hpp"

#include "hooks.hpp"

namespace offsets {

constexpr int CreateMove = Platform(22, 21);
constexpr int OverrideView = 17;
constexpr int PaintTraverse = Platform(42, 41);
constexpr int FrameStageNotify = 35;
constexpr int SendNetMsg = 41;

}  // namespace offsets

namespace modules::source::hooks {

// Here we store our main hooks
static VMTHook clientmode;
static VMTHook client;
static VMTHook panel;
static VMTHook netmsg;

// Here we have our hooked functions
CUserCmd* ui_cmd =
    nullptr;  // For functions not directly in cm but ones that use the cmd
bool using_silent = false;
Vector LastAngles;  // We save angles from CreateMove to use down in
                    // FrameStageNotify
#if defined(_WIN32)
static bool __fastcall CreateMove_hook(void* thisptr, void* edx,
                                       float inputSample, CUserCmd* cmd) {
    using CreateMove_t = bool(__thiscall*)(void*, float, CUserCmd*);
#else
static bool CreateMove_hook(void* thisptr, float inputSample, CUserCmd* cmd) {
    using CreateMove_t = bool (*)(void*, float, CUserCmd*);
#endif
    const CreateMove_t original = clientmode.GetMethod<CreateMove_t>(
        offsets::CreateMove);  // We call this on return so that we can process
                               // our stuff before create move
    if (!cmd)
        return original(thisptr, inputSample,
                        cmd);  // Cant do anything without cmd
    ui_cmd = cmd;              // Update the cmd

    // Update ingame
    if (!game::GetInGame()) return original(thisptr, inputSample, cmd);

    // Run the frameworks worldtick
    events::world.Emit();

    // Run the original to get result
    // I do this so we can make the game think we are at the normal pos and
    // angles while we send the silent angles to the server Unsure if this is
    // the case tho but it works
    bool ret = original(thisptr, inputSample, cmd);

    // Save this so we can use it in FrameStageNotify to show angles in
    // thirdperson
    LastAngles = cmd->viewangles;

    // If we are using silent aim, we return false
    if (using_silent) {
        // To use silent, we just dont return true, but we need to still correct
        // movement so we do that too
        Vector camera_angle;
        iface::engine->GetViewAngles(camera_angle);

        CatVector ang = VectorAngles(
            CatVector(cmd->forwardmove, cmd->sidemove, cmd->upmove));
        float yaw = DEG2RAD(ang.y - camera_angle.y + cmd->viewangles.y);
        float speed = sqrt(cmd->forwardmove * cmd->forwardmove +
                           cmd->sidemove * cmd->sidemove);

        cmd->forwardmove = cos(yaw) * speed;
        cmd->sidemove = sin(yaw) * speed;

        using_silent = false;
        return false;
    }
    return ret;
}

#if defined(NEKO_TF2)
static CatVarBool no_zoom({"Visuals", "Misc"}, "no_zoom", false, "No Zoom",
                          "Disables scope overlay and zoom");
#endif
#if defined(_WIN32)
static void __fastcall PaintTraverse_hook(IPanel* _this, void* edx,
                                          unsigned int vp, bool fr, bool ar) {
    using PaintTraverse_t =
        void(__thiscall*)(IPanel*, unsigned int, bool, bool);
#else
static void PaintTraverse_hook(IPanel* _this, unsigned int vp, bool fr,
                               bool ar) {
    using PaintTraverse_t = void (*)(IPanel*, unsigned int, bool, bool);
#endif
    const PaintTraverse_t original =
        panel.GetMethod<PaintTraverse_t>(offsets::PaintTraverse);

    static unsigned long panel_top = 0;
    if (!panel_top && !strcmp("MatSystemTopPanel", _this->GetName(vp)))
        panel_top = vp;

    static unsigned long panel_focus = 0;
    if (!panel_focus && !strcmp("FocusOverlayPanel", _this->GetName(vp)))
        panel_focus = vp;

// No Zoom
#if defined(NEKO_TF2)
    static unsigned long panel_scope = 0;
    if (!panel_scope && !strcmp("HudScope", _this->GetName(vp)))
        panel_scope = vp;
    if (no_zoom && vp == panel_scope) return;
#endif

    original(_this, vp, fr, ar);  // Call the original function before we draw

    // Logic to determine whether we should draw, no idea how it works
    static bool draw_flag = false;
    if (vp == panel_top) draw_flag = true;
    if (vp != panel_focus) return;
    _this->SetTopmostPopup(panel_focus, true);
    if (!draw_flag) return;
    draw_flag = false;

    // TODO: find the id of the one we print in
    // debug_log.Puts(_this->GetName(vp));

    events::draw.Emit();  // Call the managers draw tick
}

static CatVarInt fov_override({"Visuals", "Misc"}, "fov", 0, "Fov Override",
                              "Change fov");
static void OverrideView_hook(void* _this, CViewSetup* setup) {
    using OverrideView_t = void (*)(void*, CViewSetup*);
    const OverrideView_t original =
        clientmode.GetMethod<OverrideView_t>(offsets::OverrideView);
    original(_this, setup);

    // Set this for wts
    wts::wts_viewsetup = setup;

    // No-Zoom
#if defined(NEKO_TF2)
    static ConVar* fov_desired = iface::cvar->FindVar("fov_desired");
    auto local_ent =
        iface::entity_list->GetClientEntity(iface::engine->GetLocalPlayer());
    if (no_zoom) {
        // IF we are zoomed
        if (local_ent && !local_ent->IsDormant() &&
            HasCondition(local_ent, TfCond::kZoomed)) {
            if (fov_override)
                setup->fov = fov_override;
            else
                setup->fov = fov_desired->GetFloat();
        }
    } else if (!local_ent || !HasCondition(local_ent, TfCond::kZoomed))
#endif
        if (fov_override) setup->fov = fov_override;
}

static CatVarBool force_thirdperson(
    {"Visuals", "Misc"}, "thirdperson", false, "Force Thirdperson",
    "Forces thirdperson mode 99.9% of the time");
static void FrameStageNotify_hook(IClient* _this, int stage) {
    using FrameStageNotify_t = void (*)(IClient*, int);
    const FrameStageNotify_t original =
        client.GetMethod<FrameStageNotify_t>(offsets::FrameStageNotify);

    if (stage == FRAME_RENDER_START) {
        IClientEntity* local_ent = iface::entity_list->GetClientEntity(
            iface::engine->GetLocalPlayer());
        if (local_ent && !local_ent->IsDormant() &&
            !netvars::m_lifeState.Get(local_ent)) {
#if defined(NEKO_TF2)
            // Force Thirdperson
            netvars::m_nForceTauntCam.Get(local_ent) = force_thirdperson;

            // To show antiaim in thirdperson
            if (force_thirdperson &&
                stage == 5) {  //&& g_IInput && g_IInput->CAM_IsThirdPerson()) {
                *reinterpret_cast<float*>(
                    reinterpret_cast<uintptr_t>(local_ent) +
                    netvars::deadflag.GetOffset() + 4) = LastAngles.x;
                *reinterpret_cast<float*>(
                    reinterpret_cast<uintptr_t>(local_ent) +
                    netvars::deadflag.GetOffset() + 8) = LastAngles.y;
            }
#endif
        }
    }
    original(_this, stage);
}

// MrSteyk and Raspbian i think wanted be to try a few things.... Still wont
// have a working sig finder....

/*typedef void(__cdecl *ReadUserCmdtd)(bf_read *buf, CUserCmd *move, CUserCmd
*from); ReadUserCmdtd ReadUserCmdFN = nullptr; using ReadUsercmd_t =
void(*)(bf_read* buf, CUserCmd* move, CUserCmd* from); static ReadUsercmd_t
ReadUsercmd = nullptr;

typedef void(__cdecl *WriteUserCmdtd)(bf_write *buf, CUserCmd *to, CUserCmd
*from); WriteUserCmdtd WriteUserCmdFN = nullptr;*/

static CatVarKey timeshift({"Misc"}, "timeshift", CatKey::kNone, "Timeshift");
static bool SendNetMsg_hook(INetChannel* _this, INetMessage& msg,
                            bool force_reliable, bool voice) {
    using SendNetMsg_t = bool (*)(INetChannel*, INetMessage&, bool, bool);
    const SendNetMsg_t original =
        netmsg.GetMethod<SendNetMsg_t>(offsets::SendNetMsg);

    return original(_this, msg, force_reliable, voice);
}
/*if (!strcmp("clc_Move", msg.GetName()) && timeshift && timeshift.Depressed())
{

        if (!ReadUserCmdFN) {
                ReadUserCmdFN = (ReadUserCmdtd)0xCALLME;
        }
        if (!WriteUserCmdFN) {
                WriteUserCmdFN = (WriteUserCmdtd)0xCALLME;
        }
        auto Cl_Move = (CLC_Move&)msg;
        int totalcmds = Cl_Move.m_nNewCommands;//+ Cl_Move.m_nBackupCommands
        CUserCmd		*from, *to;
        CUserCmd cmds[CMD_MAXBACKUP] = {};

        CUserCmd		cmdNull;


        cmdNull.Reset();
        save_pos = Cl_Move.m_DataOut.m_iCurBit;
        Cl_Move.m_DataOut.Reset();
        from = &cmdNull;
        for (i = 0; i <= totalcmds; i++){
                to = &cmds[i];
                ReadUserCmdFN((bf_read*)&Cl_Move.m_DataOut, to, from);
                from = to;
        }
        Cl_Move.m_DataOut.m_iCurBit = save_pos;
        real_commands = totalcmds;
        last_command_number = cmds[totalcmds].command_number;
        last_tick_count = cmds[totalcmds].tick_count + 200;
        while (totalcmds <= MAX_USERCMD) {
                to = &cmds[rand() % real_commands];//b1g random
                to->command_number = last_command_number + totalcmds -
real_commands + 1; to->tick_count = last_tick_count + totalcmds - real_commands
+ 1; WriteUserCmdFN(&Cl_Move.m_DataOut, to, from); if (totalcmds >
real_commands) { cmds[totalcmds] = *to;
                }
                from = to;
                totalcmds++;
        }
        last_command_number = cmds[totalcmds].command_number;
        last_tick_count = cmds[totalcmds].tick_count;
        Cl_Move.m_nNewCommands = totalcmds - Cl_Move.m_nBackupCommands;

        netchan->m_nOutSequenceNr += 10;


        cGMOD_hooks.NetChannelHook.GetOriginalMethod<NET_CHANNEL_tdSendNetMsg>(40)(netchan,
th, msg, bForceReliable, bVoice); netchan->Transmit();
        netchan->m_nOutSequenceNrAck = netchan->m_nOutSequenceNr;
        //////////////////////////////////////////////////////////

        for (int iter = 0; iter <= 228; iter++) {

                moveMsg.m_DataOut.StartWriting(data, sizeof(data));

                moveMsg.m_nBackupCommands = 0;
                moveMsg.m_nNewCommands = totalcmds;
                for (int iter = 0; iter <= totalcmds; iter++) {
                        to = &cmds[rand() % totalcmds];
                        to->command_number = last_command_number + iter + 1;
                        to->tick_count = last_tick_count + iter + 1;
                        WriteUserCmdFN(&moveMsg.m_DataOut, to, from);
                        from = to;
                }
                last_command_number += totalcmds;
                last_tick_count += totalcmds;
                msg = moveMsg;
                cGMOD_hooks.NetChannelHook.GetOriginalMethod<NET_CHANNEL_tdSendNetMsg>(40)(netchan,
th, msg, bForceReliable, bVoice); netchan->Transmit();
                netchan->m_nOutSequenceNrAck = netchan->m_nOutSequenceNr;
        }
        return true;
}

return
cGMOD_hooks.NetChannelHook.GetOriginalMethod<NET_CHANNEL_tdSendNetMsg>(40)(netchan,
th, msg, bForceReliable, bVoice);*/
//}

// static bool SendNetMsg_hook(void *_this, INetMessage &msg, bool
// bForceReliable = false, bool bVoice = false) {

// Use this to init our hooks
void Init() {
    // Because getting this to work is slow, why not let it run in another
    // thread
    auto HookClientmode = []() {
        // Client mode
        uintptr_t* tmp;
#if !defined(_WIN32)
        while (!(
            tmp = **(uintptr_t***)(((*(uintptr_t**)iface::client)[10]) + 1))) {
        }
#else
        while (!(
            tmp = **(uintptr_t***)(((*(uintptr_t**)iface::client)[10]) + 5))) {
        }  // Thanks rud
#endif
        clientmode.Set(static_cast<void*>(tmp));
        clientmode.HookMethod((void*)CreateMove_hook,
                              offsets::CreateMove);  // World Tick
        clientmode.HookMethod((void*)OverrideView_hook,
                              offsets::OverrideView);  // Override View
        clientmode.Apply();
    };
    std::thread client_mode_finder(HookClientmode);
    client_mode_finder.detach();

    // Panel
    panel.Set(iface::panel);
    panel.HookMethod((void*)PaintTraverse_hook,
                     offsets::PaintTraverse);  // Draw Tick
    panel.Apply();

    // Client
    client.Set(iface::client);
    client.HookMethod((void*)FrameStageNotify_hook, offsets::FrameStageNotify);
    client.Apply();

    /*events::world.Listen([](){
        // As to try to prevent lag we do a check ever sec
        static CatTimer do_stuff;
        if (do_stuff.ResetAfter(std::chrono::seconds(1))) {
            // We check if we arent hooked anymore because source does that
            INetChannel* channel =
    reinterpret_cast<INetChannel*>(iface::engine->GetNetChannelInfo()); if
    (channel && *reinterpret_cast<void**>(channel) !=
    netmsg.vtable_replacement.get()) { netmsg.Set(channel);
                netmsg.HookMethod((void*)SendNetMsg_hook, offsets::SendNetMsg);
                netmsg.Apply();
            }
        }
    });*/

    // ReadUsercmd = GetSignature<ReadUsercmd_t>(sharedobj::client, "55 89 E5 57
    // 56 53 83 EC 1C 8B 7D 0C 8B 75 10 8B 5D 08 39 F7 0F");
    // Should we do this... i dont really know lel
    // client_mode_finder.join();
}

}  // namespace modules::source::hooks
