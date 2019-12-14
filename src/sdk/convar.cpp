/*
Convar::IsCommand() const
Convar::IsFlagSet(int) const
Convar::AddFlags(int)

Convar::Set(char const*)
*/

#include <cstring>

#include "convar.hpp"

namespace sourcesdk {

int CommandBase::GetDLLIdentifier() const {
    return 0;
}
void CommandBase::Create(char const* _name, char const*_help, int _flags) {
	this->name = _name;
	this->help_string = _help;
	this->flags = _flags;
}
Convar::Convar(char const* _name, char const* _help, int _flags) {
    this->Create(_name, _help, _flags);
}
void Convar::Create(char const* _name, char const* _defaults, int _flags, char const* _help, 
               bool _has_min, float _min, bool _has_max, float _max, void* _callback) {
    this->parent = this;

	this->defaults = _defaults;
	this->value_string_length = std::strlen(this->value_string) + 1;
	this->value_string = new char[this->value_string_length];
	std::memcpy(this->value_string, this->defaults, this->value_string_length);

	this->has_min = _has_min;
	this->min = _min;
	this->has_max = _has_max;
	this->max = _max;
	this->value_float = std::atof(this->value_string);
	this->value_int = std::atoi(this->value_string);
	this->CommandBase::Create(_name, _help, _flags);
}

bool Convar::Clamp(float& in) {
	if (this->has_min && in < this->min) {
		in = this->min;
		return true;
	}
	if (this->has_max && in > this->max){
		in = this->min;
		return true;
	}
	return false;
}

const char* Convar::GetName() const {
	return this->name;
}
const char* Convar::GetHelp() const {
	return this->help_string;
}

bool Convar::IsRegistered() const {
	return this->registered;
}

void Convar::Set(float in) {
	this->InternalSet(in);
}
void Convar::Set(int in) {
	this->InternalSet(in);
}

void Convar::InternalSet(const char* in) {
	if (!in)
		return this->InternalSet(0);
	

	if (!!strcmp(in, this->value_string)) {
		float old = this->value_float;
		this->value_float = std::atof(in);
		this->value_int = std::atoi(in);
		this->Clamp(this->value_float);
		this->Change(in, old);
	}
}
void Convar::InternalSet(float in) {
	this->Clamp(in);

    auto old = this->value_float;
    this->value_float = in;
    this->value_int = static_cast<int>(in);

    char temp_value[32];
    std::snprintf(temp_value, sizeof(temp_value), "%f", in);
    this->Change(temp_value, old);
}
void Convar::InternalSet(int in) {
	this->InternalSet(static_cast<float>(in));
}

void Convar::Change(const char* new_value, float) {

	if (this->value_string) {
        delete[] this->value_string;
        this->value_string = nullptr;
    }

    this->value_string_length = std::strlen(new_value) + 1;
    value_string = new char[this->value_string_length];
    std::strncpy(value_string, new_value, this->value_string_length);
}

}