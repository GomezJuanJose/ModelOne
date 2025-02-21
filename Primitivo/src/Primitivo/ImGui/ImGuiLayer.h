#pragma once

#include "Primitivo/Core/Layer.h"

namespace Primitivo {
	class ImGuiLayer : public Layer{
	public:
		ImGuiLayer();
		~ImGuiLayer();


		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;



		void BeginImGuiRender();
		void EndImGuiRender();

	private:
		float m_Time = 0.0f;
	};
}