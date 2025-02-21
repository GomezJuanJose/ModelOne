#pragma once
#include "Primitivo/Core/Layer.h"
#include "Primitivo/Core/Timestep.h"
#include "Primitivo/ECS/ECS.h"
#include "Primitivo/Core/Application.h"

namespace Primitivo {

	class ShaderLibrary;
	class Texture2DLibrary;
	class MeshLibrary;

	class FrameBuffer;

	struct CameraComponent;

	class RenderLayer : public Layer{
	public:
		RenderLayer();
		~RenderLayer() = default;



		virtual void OnUpdate(Timestep timestep) override;

	private:


		CameraComponent& m_CameraComponent;

		Coordinator& m_Coordinator = Application::Get().GetCoordinator();
		ShaderLibrary& m_ShaderLibrary = Application::Get().GetShaderLibrary();
		MeshLibrary& m_MeshLibrary = Application::Get().GetMeshLibrary();
		Texture2DLibrary& m_Texture2DLibrary = Application::Get().GetTextureLibrary();

		Signature m_MeshSignature;
		Signature m_CameraSignature;
		Signature m_LightSignature;

		AssetRef<FrameBuffer> m_ShadowMap;
	};
}
