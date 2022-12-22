#include "ScenePanel.h"
#include <imGui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Hazel {
	ScenePanel::ScenePanel(Ref<Scene>& scene)
		: m_Scene(scene)
	{
		
	}
	void ScenePanel::OnImGuiRender()
	{
		ImGui::Begin("Scene");
		
		DrawTreeNodes();
		
		ImGui::End();
		ImGui::Begin("Properties");

		DrawComponents();

		ImGui::End();

	}
	void ScenePanel::DrawTreeNodes()
	{
		m_Scene->m_Registry.each([&](auto entityID)
			{
				Entity entity = { entityID, m_Scene.get() };
				auto& tag = entity.GetComponent<TagComponent>().Tag;
				//ImGui::Text(tag.c_str());
				ImGuiTreeNodeFlags flags = ((m_SelectedContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
				bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entityID, flags, tag.c_str());
				if (ImGui::IsItemClicked())
				{
					m_SelectedContext = entity;
				}

				bool entityDeleted = false;
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete Entity"))
						entityDeleted = true;
					ImGui::EndPopup();
				}

				if (opened)
				{
					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
					bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
					if (opened)
						ImGui::TreePop();
					ImGui::TreePop();
				}

				if (entityDeleted)
				{
					m_Scene->DestroyEntity(entity);
					if (m_SelectedContext == entity)
						m_SelectedContext = {};
				}
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectedContext = {};

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				m_Scene->CreateEntity("Empty Entity");
			}
			ImGui::EndPopup();
		}
	}

	void ScenePanel::DrawComponents()
	{
		
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

			ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

			if (m_SelectedContext.HasComponent<TransformComponent>())
			{
				if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
				{
					auto& translation = m_SelectedContext.GetComponent<TransformComponent>().Translation;
					DrawDragFloat3("Translation", translation);
					auto& rotation = m_SelectedContext.GetComponent<TransformComponent>().Rotation;
					glm::vec3 degrees = glm::degrees(rotation);
					DrawDragFloat3("Rotation", degrees);
					rotation = glm::radians(degrees);
					auto& scale = m_SelectedContext.GetComponent<TransformComponent>().Scale;
					DrawDragFloat3("Scale", scale, 1.0f);

					ImGui::TreePop();
				}
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
			if (m_SelectedContext.HasComponent<SpriteRendererComponent>())
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
				bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, "Sprite Renderer");
				ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
				if (ImGui::Button("+", ImVec2{ 20, 20 }))
				{
					ImGui::OpenPopup("ComponentSettings");
				}
				ImGui::PopStyleVar();

				bool removeComponent = false;
				if (ImGui::BeginPopup("ComponentSettings"))
				{
					if (ImGui::MenuItem("Remove component"))
						removeComponent = true;

					ImGui::EndPopup();
				}

				if (open)
				{
					auto& src = m_SelectedContext.GetComponent<SpriteRendererComponent>();
					ImGui::ColorEdit4("Color", glm::value_ptr(src.Color));
					ImGui::TreePop();
				}

				if (removeComponent)
					m_SelectedContext.RemoveComponent<SpriteRendererComponent>();
			}
			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");
			if (ImGui::BeginPopup("AddComponent"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					m_SelectedContext.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Sprite Renderer"))
				{
					m_SelectedContext.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
		
	}

	void ScenePanel::DrawDragFloat3(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
	{
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}
}