#pragma once

namespace Taller {

	class RenderContext {
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

	protected:
		RenderContext() = default;
	};
}