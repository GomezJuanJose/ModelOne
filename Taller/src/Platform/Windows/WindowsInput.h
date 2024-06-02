#pragma once

#include "Taller/Core/Input.h"

namespace Taller {
	class WindowsInput : public Input {
	protected:
		virtual bool IsKeyPressedImplementation(int keycode) override;

		virtual bool IsMouseButtonImplementation(int button) override;
		virtual std::pair<float, float> GetMousePositionImplementation() override;
		virtual float GetMouseXImplementation() override;
		virtual float GetMouseYImplementation() override;
	};
}