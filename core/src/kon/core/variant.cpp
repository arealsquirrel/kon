
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

template<>
VariantType type_to_variant_type<int>() { return VariantType_int; }

// --------- UNSIGNED INT ---------
template<>
unsigned int &Variant::get() { return ui; }

template<>
void Variant::set(const unsigned int &t) { ui = t; }

template<>
VariantType type_to_variant_type<unsigned int>() { return VariantType_uint; }


// --------- LONG UNSIGNED INT ---------
template<>
long unsigned int &Variant::get() { return lui; }

template<>
void Variant::set(const long unsigned int &t) { lui = t; }

template<>
VariantType type_to_variant_type<long unsigned int>() { return VariantType_luint; }

// --------- FLOAT ---------
template<>
float &Variant::get() { return f; }

template<>
void Variant::set(const float &t) { f = t; }

template<>
VariantType type_to_variant_type<float>() { return VariantType_float; }

// --------- DOUBLE ---------
template<>
double &Variant::get() { return d; }

template<>
void Variant::set(const double &t) { ui = t; }

template<>
VariantType type_to_variant_type<double>() { return VariantType_double; }

// --------- STRING ---------
template<>
String &Variant::get() { return s; }

template<>
void Variant::set(const String &t) { s = t.c_str(); }

template<>
VariantType type_to_variant_type<String>() { return VariantType_String; }

// --------- SHORT STRING ---------
template<>
ShortString &Variant::get() { return ss; }

template<>
void Variant::set(const ShortString &t) { ss = t; }

template<>
VariantType type_to_variant_type<ShortString>() { return VariantType_ShortString; }

}

