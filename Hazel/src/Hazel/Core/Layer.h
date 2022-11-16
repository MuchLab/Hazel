#pragma once
#include "Core.h"
#include "Hazel/Event/Event.h"
#include "Hazel/Core/Timestep.h"

namespace Hazel {
	class HAZEL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline std::string GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}

