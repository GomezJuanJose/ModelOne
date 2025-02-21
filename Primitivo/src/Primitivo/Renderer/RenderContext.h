#pragma once

namespace Primitivo {

	class RenderContext {
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

	protected:
		RenderContext() = default;
	};
}