#pragma once
#include "Test.h"
#include "../Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	class TestBatchRenderingDifferentTexture2D : public Test {
	public:
		TestBatchRenderingDifferentTexture2D();
		~TestBatchRenderingDifferentTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGUIRender() override;
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture_SonarQube;
		std::unique_ptr<Texture> m_Texture_Background;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		glm::vec3 m_TranslationA;
		glm::vec3 m_TranslationB;
		bool m_ButtonClicked = false;
	};
}