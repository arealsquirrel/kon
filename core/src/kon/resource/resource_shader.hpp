#ifndef KN_RESOURCE_SHADER
#define KN_RESOURCE_SHADER

#include "kon/core/directory.hpp"
#include "kon/core/object.hpp"
#include "kon/engine/engine.hpp"
#include "kon/resource/resource.hpp"
#include <kon/core/core.hpp>

namespace kon {

enum ShaderType {
	ShaderType_Fragment,
	ShaderType_Vertex,
	ShaderType_Compute
};

class ResourceShader : public Resource {
KN_OBJECT(ResourceShader, Resource)

public:
	ResourceShader(Allocator *allocator, Engine *engine,
			 Directory path, ShortString name, UUID groupID=UUID(0));
	~ResourceShader();

	void load_resource(ResourceLoadError &error) override;
	void load_metadata(ResourceLoadError &error) override;
	void unload_resource() override;

public:
	u32 get_size() const { return m_size; }
	const char *get_shader_code() const { return m_code; }

private:
	char *m_code;
	u32 m_size;
};

}

#endif
