#pragma once
#include "ModelOne/Core/Layer.h"
#include "ModelOne/Core/Timestep.h"
#include "ModelOne/ECS/ECS.h"
#include "ModelOne/Core/Application.h"

namespace ModelOne {

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
