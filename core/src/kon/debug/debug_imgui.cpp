
#include "debug_imgui.hpp"
#include "kon/core/allocator.hpp"
#include <imgui.h>

namespace kon {

static void draw_freelist_allocator(FreeListAllocator *allocator) {
	FreeListAllocator::Header *node = allocator->get_start();
	while(node != nullptr) {
		ImVec4 col = (node->free == true) ? ImVec4{1,0,0,1} : ImVec4{0,1,0,1};

		ImGui::TextColored(col, "[%p] %u", (void*)node, node->size);
		// KN_CORE_TRACE("\theader at {}:[{}] size {}", (void*)node, node->free, node->size);
		node = node->next;
	}
}

void DebugImgui::draw_engine_config(Engine *engine) {
	ImGui::Begin("Engine Control Panel");

	if(ImGui::TreeNode("Memory")) {
		ImGui::Text("This details the allocators used by the engine.\n Green means that the node is being used, and red means the node is free.");

		if(ImGui::TreeNode("Persistent")) {
			draw_freelist_allocator(dynamic_cast<FreeListAllocator*>(engine->get_allocator_persistent()));
			ImGui::TreePop();
		}

		if(ImGui::TreeNode("Dynamic")) {
			draw_freelist_allocator(dynamic_cast<FreeListAllocator*>(engine->get_allocator_dynamic()));
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

	ImGui::End();
}

}

