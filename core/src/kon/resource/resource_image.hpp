#ifndef KN_RESOURCE_IMAGE
#define KN_RESOURCE_IMAGE

#include "kon/core/directory.hpp"
#include "kon/engine/engine.hpp"
#include "kon/resource/resource.hpp"
#include <kon/core/core.hpp>

namespace kon {

class ResourceImage : public Resource {
public:
	ResourceImage(Allocator *allocator, Engine *engine,
			 Directory path, ShortString name, UUID groupID=UUID(0));
	~ResourceImage();

	void load_resource(ResourceLoadError *error) override;
	void load_metadata(ResourceLoadError *error) override;
	void unload_resource() override;
};

}

#endif
