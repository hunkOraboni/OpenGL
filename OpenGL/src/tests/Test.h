#pragma once

#include "../Renderer.h"
#include "imgui/imgui.h"

#include <functional>
#include <vector>
#include <string>

namespace test {
	class Test {
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGUIRender() {}
	};

	class TestMenu : public Test {
	public:
		TestMenu(Test*& currentTestPointer);
		~TestMenu();

		void OnImGUIRender() override;
	private:
		Test*& m_CurrentTest;
		std::vector < std::pair<std::string, std::function<Test* ()>>> m_Tests;
	};
}