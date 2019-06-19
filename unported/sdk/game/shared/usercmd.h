//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//
//=============================================================================//
#if !defined(USERCMD_H)
#define USERCMD_H
#ifdef _WIN32
#pragma once
#endif

#include "../../public/mathlib/vector.h"
#include "../../public/tier1/checksum_crc.h"
#include "../../public/tier1/utlvector.h"
#include "imovehelper.h"

class bf_read;
class bf_write;

class CEntityGroundContact {
   public:
    int entindex;
    float minheight;
    float maxheight;
};

class CUserCmd {
   public:
    CUserCmd() { this->Reset(); }

    virtual ~CUserCmd(){};

    void Reset() {
        command_number = 0;
        tick_count = 0;
        viewangles.Init();
        forwardmove = 0.0f;
        sidemove = 0.0f;
        upmove = 0.0f;
        buttons = 0;
        impulse = 0;
        weaponselect = 0;
        weaponsubtype = 0;
        random_seed = 0;
        // server_random_seed = 0;
        mousedx = 0;
        mousedy = 0;

        hasbeenpredicted = false;
    }

    // Allow command, but negate gameplay-affecting values
    void MakeInert() {
        viewangles = Vector();
        forwardmove = 0.f;
        sidemove = 0.f;
        upmove = 0.f;
        buttons = 0;
        impulse = 0;
    }

    // For matching server and client commands for debugging
    int command_number;

    // the tick the client created this command
    int tick_count;

    // Player instantaneous view angles.
    Vector viewangles;
    // Intended velocities
    //	forward velocity.
    float forwardmove;
    //  sideways velocity.
    float sidemove;
    //  upward velocity.
    float upmove;
    // Attack button states
    int buttons;
    // Impulse command issued.
    byte impulse;
    // Current weapon id
    int weaponselect;
    int weaponsubtype;

    int random_seed;  // For shared random functions

    short mousedx;  // mouse accum in x from create move
    short mousedy;  // mouse accum in y from create move

    // Client only, tracks whether we've predicted this command at least once
    bool hasbeenpredicted;
};

void ReadUsercmd(bf_read *buf, CUserCmd *move, CUserCmd *from);
void WriteUsercmd(bf_write *buf, const CUserCmd *to, const CUserCmd *from);

#endif  // USERCMD_H
