
#include "resource_image.hpp"
#include "kon/core/directory.hpp"
#include "kon/debug/log.hpp"
#include "kon/resource/resource.hpp"
#include "modules/graphics/graphics_module.hpp"

#include <../vendor/stb_image.h>
#include <vulkan/vulkan_core.h>

namespace kon {

ResourceImage::ResourceImage(Allocator *allocator, Engine *engine,
			 Directory path, ShortString name, UUID groupID)
	: Resource(allocator, engine, path, name, groupID), m_metadata({"", ColorFormat_R8, {0,0}, {allocator}}),
	  m_graphicsImage(engine->get_modules().get<GraphicsModule>()->get_graphics_context()) {}

ResourceImage::~ResourceImage() = default;

void ResourceImage::load_resource(ResourceLoadError &error) {
	load_metadata(error);

	int x;
	int y;

	m_image = stbi_load(m_path.c_str(),
			  	&x, &y,
			 	(int*)(&m_channels), 4);

	if(m_image == nullptr) {
		KN_CORE_ERROR("uuhh your image is cooked {}", m_path.c_str());
	}

	m_graphicsImage.create(m_image, VkExtent3D{(u32)x,(u32)y,1},
			VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL);

	Resource::load_resource(error);
}

void ResourceImage::load_metadata(ResourceLoadError &error) {
	
	int n; // ??????
	u32 ok = stbi_info(m_path.get_string().c_str(),
			&m_metadata.size.x, &m_metadata.size.y,
			&n);

	if(ok == 0) {
		error = ResourceLoadError_APIError;
		KN_CORE_ERROR("ResourceImage {}", m_path.c_str());
		return;
	}

	/*
	// make some default metadata
	m_metadata.image = m_path.get_string().c_str();
	m_metadata.frames.add({
		Rect{0, 0, m_metadata.size.x, m_metadata.size.y},
		false,
		false,
		Rect{0, 0, m_metadata.size.x, m_metadata.size.y},
		0
	});
	*/

	Resource::load_metadata(error);
}

void ResourceImage::unload_resource() {
	if(m_loadState != ResourceLoadState_FullyLoaded)
		return;

	stbi_image_free(m_image);

	Resource::unload_resource();
}

}

