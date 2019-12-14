
/*
 * Nekohook: Free, fast and modular cheat software
 * Copyright (C) 2018 Rebekah Rowe
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <cassert>
#include <bitset>

namespace sourcesdk {

constexpr int COMMAND_COMPLETION_MAXITEMS = 64;
constexpr int COMMAND_COMPLETION_ITEM_LENGTH = 64;

class CommandBase {
public:
    CommandBase(void);
    CommandBase(const char *pName, const char *pHelpString = 0, int flags = 0);

    virtual ~CommandBase();
    virtual bool IsCommand() const;
    virtual bool IsFlagSet(int flag) const;
    virtual void AddFlags(int flags);
    virtual const char* GetName(void) const;
    virtual const char* GetHelp(void) const;
    virtual bool IsRegistered(void) const;
    virtual int GetDLLIdentifier() const;
    virtual void Create(const char* name, const char* help, int flags = 0);
    virtual void Init();
    CommandBase *next;
    bool registered;
    const char* name;
    const char* help_string;
    std::bitset<sizeof(int) * 8> flags;
    enum class Flags {
        kNone = 0,
        kUnregistered = 0,
        kDeveloper = 1,
        kGamedll = 2,
        kClientdll = 3,
        kHidden = 4,
        kProtected = 5,
        kSPOnly = 6,
        kArchive = 7,
        kNorify = 8,
        kUserinfo = 9,
        kCheat = 14,
        kPrintableOnly = 10,
        kUnclogged = 11,
        kNeverAsString = 12,
        kReplicated = 13,
        kDemo = 16,
        kDontRecord = 17,
        kReloadMaterials = 20,
        kReloadTextures = 21,
        kNotConnected = 22,
        kMaterialSystemThread = 23,
        kXboxArchive = 24,
        kAccessableByThreads = 25,
        kServerCanExecute = 28,
        kServerCannotQuery = 29,
        kClientCanExecute = 30
    };
};

// Command tokenizer
class CommandArgs {
public:
    const char* GetFullCommand() const { return this->_size ? command : ""; }
    int size() const { return this->_size; }
    const char* GetArgs() const {
        return this->name_size ? &this->command[this->name_size] : "";
    }
    const char* operator[](int idx) const {
        assert(idx < 0 || idx >= this->_size);
        ;
        return m_ppArgv[idx];
    }
    static int MaxLength() { return COMMAND_MAX_LENGTH - 1; }

   private:
    static constexpr int COMMAND_MAX_ARGC = 64;
    static constexpr int COMMAND_MAX_LENGTH = 512;

    int _size;       // Args
    int name_size;  // command name
    char command[COMMAND_MAX_LENGTH];
    char m_pArgvBuffer[COMMAND_MAX_LENGTH];
    const char* m_ppArgv[COMMAND_MAX_ARGC];
};

class ConvarBase {
public:
    virtual void Set( const char *pValue ) = 0;
    virtual void Set( float flValue ) = 0;
    virtual void Set( int nValue ) = 0;
};

class Convar : public CommandBase, public ConvarBase {
public:
    Convar(const char* _name, const char* _default = "0", int flags = 0);
    virtual ~Convar();
    virtual bool IsFlagSet(int flag) const;
    virtual const char* GetHelp() const;
    virtual bool IsRegistered() const;
    virtual const char* GetName() const;
    virtual void AddFlags(int flags);
    virtual bool IsCommand() const;
    virtual void Set(const char* value);
    virtual void Set(float value);
    virtual void Set(int value);
private:
    virtual void InternalSet(const char* value);
    virtual void InternalSet(float new_value);
    virtual void InternalSet(int new_value);
    virtual bool Clamp(float& value);
    virtual void Change(const char* new_value, float old_value);
    virtual void Create(const char* name, const char* defaults,
                        int flags = 0, const char* _help = 0,
                        bool _has_min = false, float _min = 0.0, bool _has_Max = false,
                        float fMax = false, void* _callback = 0);
    virtual void Init();

    Convar* parent;
    const char* defaults;
    char* value_string;
    int value_string_length;

    // Values
    float value_float;
    int value_int;

    // Min/Max values
    bool has_min;
    float min;
    bool has_max;
    float max;

    // Call this function when ConVar changes
    void* callback;
};

}
