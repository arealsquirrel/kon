#ifndef KN_RESOURCE_IMAGE
#define KN_RESOURCE_IMAGE

#include <kon/core/core.hpp>
#include "kon/container/string.hpp"
#include "kon/core/directory.hpp"
#include "kon/core/object.hpp"
#include "kon/engine/engine.hpp"
#include "kon/math/vector.hpp"
#include "kon/resource/resource.hpp"
#include <kon/math/vector2.hpp>

namespace kon {

class ResourceImage : public Resource {
KN_OBJECT(ResourceImage, Resource)

public:

	/*
	 * these are the png formats we might
	 * find from aseprite
	 */
	enum ColorFormat : u8 {
		ColorFormat_R8 = 1,
		ColorFormat_R8A8 = 2,
		ColorFormat_R8G8B8 = 3,
		ColorFormat_R8G8B8A8 = 4
	};

	/*
	 * these are the channels requested and returned by
	 * stbi image
	 */
	enum ColorChannels : int {
		ColorChannels_Grey = 1,
		ColorChannels_GreyAlpha = 2,
		ColorChannels_RGB = 3,
		ColorChannels_RGBA = 4
	};

	/*
	 * stores an aseprite frame, which can either be
	 * a frame of animation or a frame of a tilemap
	 */
	struct Frame {
		Rect bounds;
		bool rotated;
		bool trimmed;
		Rect sourceSize;
		u32 duration; // used for animation, defines the duration in ms
	};

	struct Meta {
		ShortString image;
		ColorFormat format;
		Vector2Int size;
		ArrayList<Frame> frames;
	};

public:
	ResourceImage(Allocator *allocator, Engine *engine,
			 Directory path, ShortString name, UUID groupID=UUID(0));
	~ResourceImage();

	void load_resource(ResourceLoadError &error) override;
	void load_metadata(ResourceLoadError &error) override;
	void unload_resource() override;

	u8 *get_image() const { return m_image; }
	ColorChannels get_channels() const { return m_channels; }
	const Meta &get_metadata() const { return m_metadata; }

private:
	u8 *m_image;
	ColorChannels m_channels;
	Meta m_metadata;
};

}

#endif
