
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

namespace sourcesdk {

enum CVarFlags {
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
}

constexpr int COMMAND_COMPLETION_MAXITEMS = 64;
constexpr int COMMAND_COMPLETION_ITEM_LENGTH = 64;

// Purpose: The base console invoked command/cvar interface
class CommandBase {
   public:
    using FlagT = std::bitset<sizeof(int)>;
    CommandBase() {}
    CommandBase(const char* pName, const char* pHelpString = "",
                FlagT flags = 0) {
        CreateBase(pName, pHelpString, flags);
    }
    virtual ~CommandBase() {}
    virtual bool IsCommand() const { return true; }
    virtual bool IsFlagSet(int flag) const { return this->flags[flag]; }
    virtual void AddFlags(FlagT in_flags) { this->flags |= in_flags; };
    virtual const char* GetName() const { return this->name; }
    virtual const char* GetHelpText() const { return this->help_string; }
    virtual bool IsRegistered(void) const { return this->registered; };
    virtual void* GetDLLIdentifier() const { return nullptr; };
    virtual void CreateBase(const char* _name, const char* _help_string = "",
                            int _flags = 0) {
        this->name = pName;
        this->help = _help_string;
        this->flags = _flags;
    }
    virtual void Init(){};

    ConCommandBase* next = nullptr;  // linked list
    bool registered = false;
    std::string name = "";
    std::string help = "";
    std::bitset<sizeof(int)> flags;
};

class Command : public CommandBase {
   public:
    Command(const char* pName, FnCommandCallbackVoid_t callback,
            const char* pHelpString = "", FlagT flags = 0);

    virtual ~ConCommand() {}
    virtual int AutoCompleteSuggest(const char* partial,
                                    utl::Vector<utl::String>& commands);
    virtual bool CanAutoComplete() { return false; }
    virtual void Dispatch(const CCommand& command);
};

// Command tokenizer
class CommandArgs {
   public:
    const char* GetFullCommand() const { return this->size ? command : ""; }
    int size() const { return this->size; }
    const char* GetArgs() const {
        return this->name_size ? &this->command[this->name_size] : "";
    }
    const char* operator[](int idx) const {
        assert(idx < 0 || idx >= this->size);
        ;
        return m_ppArgv[idx];
    }
    static int MaxLength() { return COMMAND_MAX_LENGTH - 1; }

   private:
    static constexpr int COMMAND_MAX_ARGC = 64;
    static constexpr int COMMAND_MAX_LENGTH = 512;

    int size;       // Args
    int name_size;  // command name
    char command[COMMAND_MAX_LENGTH];
    char m_pArgvBuffer[COMMAND_MAX_LENGTH];
    const char* m_ppArgv[COMMAND_MAX_ARGC];
};

class ConVarBase {
   public:
    // Value set
    virtual void SetValue(const char* pValue) = 0;
    virtual void SetValue(float flValue) = 0;
    virtual void SetValue(int nValue) = 0;

    // Return name of command
    virtual const char* GetName(void) const = 0;

    // Accessors.. not as efficient as using GetState()/GetInfo()
    // if you call these methods multiple times on the same IConVar
    virtual bool IsFlagSet(int nFlag) const = 0;
};
