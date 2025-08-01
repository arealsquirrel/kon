
#include "variant.hpp"

namespace kon {

Variant::Variant(VariantType type) 
	: m_type(type) {}

Variant::~Variant() {
	
}

// --------- INT ---------
template<>
int &Variant::get() { return i; }

template<>
void Variant::set(const int &t) { i = t; }

// --------- UNSIGNED INT ---------
template<>
unsigned int &Variant::get() { return ui; }

template<>
void Variant::set(const unsigned int &t) { ui = t; }

// --------- LONG UNSIGNED INT ---------
template<>
long unsigned int &Variant::get() { return lui; }

template<>
void Variant::set(const long unsigned int &t) { lui = t; }

// --------- FLOAT ---------
template<>
float &Variant::get() { return f; }

template<>
void Variant::set(const float &t) { f = t; }

// --------- DOUBLE ---------
template<>
double &Variant::get() { return d; }

template<>
void Variant::set(const double &t) { ui = t; }

// --------- STRING ---------
template<>
String &Variant::get() { return s; }

template<>
void Variant::set(const String &t) { s = t; }

// --------- SHORT STRING ---------
template<>
ShortString &Variant::get() { return ss; }

template<>
void Variant::set(const ShortString &t) { ss = t; }

}

