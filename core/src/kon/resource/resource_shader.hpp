#ifndef KN_RESOURCE_SHADER
#define KN_RESOURCE_SHADER

#include "kon/core/directory.hpp"
#include "kon/engine/engine.hpp"
#include "kon/resource/resource.hpp"
#include <kon/core/core.hpp>

namespace kon {

class ResourceShader : public Resource {
public:
	ResourceShader(Allocator *allocator, Engine *engine,
			 Directory path, ShortString name, UUID groupID=UUID(0));
	~ResourceShader();

	void load_resource(ResourceLoadError *error) override;
	void load_metadata(ResourceLoadError *error) override;
	void unload_resource() override;
};

}

#endif
