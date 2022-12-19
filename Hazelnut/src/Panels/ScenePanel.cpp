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
				ImGuiTreeNodeFlags flags = ((m_SelectedContext == entity) ? ImGuiTreeNodeFlags_Selected : 0)| ImGuiTreeNodeFlags_OpenOnArrow;
				bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entityID, flags, tag.c_str());
				if (ImGui::IsItemClicked())
				{
					m_SelectedContext = entity;	
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
		DrawComponents();
	}

	void ScenePanel::DrawComponents()
	{
		ImGui::Begin("Properties");
		if (m_SelectedContext)
		{
			if (m_SelectedContext.HasComponent<TagComponent>())
			{
				auto& tag = m_SelectedContext.GetComponent<TagComponent>().Tag;
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), tag.c_str());
				if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
				{
					tag = std::string(buffer);
				}
			}
			if (m_SelectedContext.HasComponent<TransformComponent>())
			{
				if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
				{
					auto& transform = m_SelectedContext.GetComponent<TransformComponent>().Transform;
					ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

					ImGui::TreePop();
				}
				if (m_SelectedContext.HasComponent<CameraComponent>())
				{
					if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
					{
						auto& cameraComponent = m_SelectedContext.GetComponent<CameraComponent>();
						SceneCamera& camera = cameraComponent.Camera;

						const char* cameraTypeStrings[] = { "Perspective", "Orthographic" };
						const char* currentCameraTypeString = cameraTypeStrings[(int)camera.GetProjectionType()];

						ImGui::Checkbox("Primary", &cameraComponent.Primary);

						if (ImGui::BeginCombo("Projection", currentCameraTypeString))
						{

							for (uint32_t i = 0; i < 2; i++)
							{
								bool isSelected = currentCameraTypeString == cameraTypeStrings[i];
								if (ImGui::Selectable(cameraTypeStrings[i], isSelected))
								{
									currentCameraTypeString = cameraTypeStrings[i];
									camera.SetProjectionType((SceneCamera::ProjectionType)i);
								}
								if (isSelected)
									ImGui::SetItemDefaultFocus();
							}

							ImGui::EndCombo();
						}
						if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
						{
							float orthoSize = camera.GetOrthographicSize();
							if (ImGui::DragFloat("OrthoSize", &orthoSize))
								camera.SetOrthographicSize(orthoSize);
							float orthoNear = camera.GetOrthographicNear();
							if (ImGui::DragFloat("OrthoNear", &orthoNear))
								camera.SetOrthographicSize(orthoSize);
							float orthoFar = camera.GetOrthographicFar();
							if (ImGui::DragFloat("OrthoFar", &orthoFar))
								camera.SetOrthographicSize(orthoFar);

							ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);
						}

						if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
						{
							float perspectiveFov = glm::degrees(camera.GetPerspectiveFov());
							if (ImGui::DragFloat("PerspectiveFov", &perspectiveFov))
								camera.SetPerspectiveFov(glm::radians(perspectiveFov));
							float perspectiveNear = camera.GetPerspectiveNear();
							if (ImGui::DragFloat("PerspectiveNear", &perspectiveNear))
								camera.SetPerspectiveFov(perspectiveNear);
							float perspectiveFar = camera.GetPerspectiveFar();
							if (ImGui::DragFloat("PerspectiveFar", &perspectiveFar))
								camera.SetPerspectiveFov(perspectiveFar);
						}
						SceneCamera::ProjectionType cameraType = camera.GetProjectionType();
						ImGui::TreePop();
					}
				}
			}
		}
		ImGui::End();
	}

	
}