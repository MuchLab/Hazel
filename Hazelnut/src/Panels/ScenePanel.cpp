#include "ScenePanel.h"
#include <imGui/imgui.h>

namespace Hazel {
	ScenePanel::ScenePanel(Ref<Scene>& scene)
		: m_Scene(scene)
	{
		
	}
	void ScenePanel::OnImGuiRender()
	{
		ImGui::Begin("ScenePanel");
		m_Scene->m_Registry.each([&](auto entityID)
			{
				Entity entity = { entityID, m_Scene.get() };
				auto& tag = entity.GetComponent<TagComponent>().Tag;
				//ImGui::Text(tag.c_str());
				ImGuiTreeNodeFlags flags = ((m_SeletedContext == entity) ? ImGuiTreeNodeFlags_Selected : 0)| ImGuiTreeNodeFlags_OpenOnArrow;
				bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entityID, flags, tag.c_str());
				if (ImGui::IsItemClicked())
				{
					HZ_CORE_WARN("Seleted entity:{0}", (uint32_t)entityID);
					m_SeletedContext = entity;
				}
					
				if (opened)
				{
					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
					bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
					if (opened)
						ImGui::TreePop();
					ImGui::TreePop();
				}
			});
		ImGui::End();
	}
}