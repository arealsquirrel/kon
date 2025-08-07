
#include "debug_imgui.hpp"
#include "kon/core/allocator.hpp"
#include "kon/resource/resource.hpp"
#include "kon/resource/resource_cache.hpp"
#include <imgui.h>

namespace kon {


template<>
void DebugImgui::render_class_debug(Object *object) {
	if(ImGui::TreeNode("Object Info")) {
		ImGui::Text("[%p] %s", (void*)object, object->get_typeinfo()->name.c_str());
		ImGui::Text("Type ID: %llu", object->get_typeinfo()->typeID.uuid);
		ImGui::Text("Instance ID: %llu", object->get_instance_id().uuid);
		ImGui::Separator();
		ImGui::TreePop();
	}
}

template<>
void DebugImgui::render_class_debug(ResourceCache *cache) {
	render_class_debug<Object>(cache->cast<Object>());
	const auto &map = cache->get_resource_map();

	if (ImGui::BeginTable("ResourceCacheTable", 3,
				ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
				ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollX)) {
		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("LoadState");
		ImGui::TableSetupColumn("Group");
		ImGui::TableHeadersRow();

		map.for_each([](const Pair<const ShortString, Resource*> &p){
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%s", p.first.c_str());

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%s", load_state_to_string(p.second->get_load_state()));

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%llu", p.second->get_group_uuid().uuid);
		});

		ImGui::EndTable();
	}
}

static void draw_freelist_allocator(FreeListAllocator *allocator) {
	static bool viewFree = false;
	static bool viewAllocated = false;
	
	ImGui::Checkbox("View Free", &viewFree);
	ImGui::Checkbox("View Allocated", &viewAllocated);

	if (ImGui::BeginTable("table1", 2)) {
		ImGui::TableSetupColumn("Address");
		ImGui::TableSetupColumn("Amount");
		ImGui::TableHeadersRow();

		FreeListAllocator::Header *node = allocator->get_start();
		while(node != nullptr) {
			if((node->free || viewAllocated) && (node->free==false || viewFree)) {
				ImGui::TableNextRow();
				ImVec4 col = (node->free == true) ? 
					ImGui::GetStyle().Colors[ImGuiCol_TextDisabled] : ImGui::GetStyle().Colors[ImGuiCol_CheckMark];

				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%p", (void*)node);
				
				ImGui::TableSetColumnIndex(1);
				ImGui::TextColored(col, "%u", node->size);
			}

			node = node->next;
		}
		
		ImGui::EndTable();
	}
}

void DebugImgui::draw_engine_config(Engine *engine) {
	ImGui::Begin("Engine Control Panel");

	render_class_debug<Object>(engine);

	if(ImGui::TreeNode("Memory")) {
		ImGui::TextWrapped("This details the allocators used by the engine. Green means that the node is being used, and red means the node is free.");

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

	if(ImGui::TreeNode("Resource Cache")) {
		render_class_debug(&engine->get_resource_cache());
		ImGui::TreePop();
	}

	ImGui::End();
}

}

