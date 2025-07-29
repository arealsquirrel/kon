
#include "object.hpp"

namespace kon {

Object::Object(Engine *engine, Allocator *allocator)
	: m_engine(engine), m_allocator(allocator) {}

Object::~Object() = default;

}

