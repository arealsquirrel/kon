#ifndef KN_RESOURCE_MODEL
#define KN_RESOURCE_MODEL

#include "kon/core/directory.hpp"
#include "kon/engine/engine.hpp"
#include "kon/resource/resource.hpp"
#include <kon/core/core.hpp>

namespace kon {

class ResourceModel : public Resource {
public:
	ResourceModel(Allocator *allocator, Engine *engine,
			 Directory path, ShortString name, UUID groupID=UUID(0));
	~ResourceModel();

	void load_resource(ResourceLoadError *error) override;
	void load_metadata(ResourceLoadError *error) override;
	void unload_resource() override;
};

}

#endif
