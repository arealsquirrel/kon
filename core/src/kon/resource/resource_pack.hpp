#ifndef KN_RESOURCE_PACK
#define KN_RESOURCE_PACK

#include "kon/container/string.hpp"
#include "kon/resource/resource.hpp"
#include "nlohmann/json.hpp"
#include <kon/core/core.hpp>

namespace kon {

class ResourcePack : public Resource {
public:
	ResourcePack(Allocator *allocator, Engine *engine,
			 Directory path, ShortString name, UUID groupID=UUID(0));
	~ResourcePack();

	void load_resource(ResourceLoadError &error) override;
	void load_metadata(ResourceLoadError &error) override;
	void unload_resource() override;

	const ShortString &get_shaders_path() const { return m_shaders; }
	const ShortString &get_models_path() const { return m_models; }
	const ShortString &get_textures_path() const { return m_textures; }

private:
	nlohmann::json m_json;

	ShortString m_packname;
	ShortString m_shaders;
	ShortString m_models;
	ShortString m_textures;
};

}

#endif
