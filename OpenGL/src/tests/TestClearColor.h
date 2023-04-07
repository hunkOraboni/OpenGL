#pragma once
#include "Test.h"
//#include "../Renderer.h"
//#include "imgui/imgui.h"

namespace test {
	class TestClearColor : public Test {
	public:
		TestClearColor();
		~TestClearColor();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGUIRender() override;
	private:
		float m_ClearColor[4];
	};
}