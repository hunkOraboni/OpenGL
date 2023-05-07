#pragma once
#include "Test.h"
#include "../Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <array>

namespace test {
	class TestBatchRenderingDifferentTexture2DDynamicGeometry : public Test {
	public:
		TestBatchRenderingDifferentTexture2DDynamicGeometry();
		~TestBatchRenderingDifferentTexture2DDynamicGeometry();

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
		
		struct Vertex {
			glm::vec2 Position;
			glm::vec2 TexCoords;
			float TexID;
		};

		//Nesta função vou criar os 4 vertex do meu quad
		std::array<TestBatchRenderingDifferentTexture2DDynamicGeometry::Vertex, 4> CreateQuad(float x, float y, float textureID);
	};
}