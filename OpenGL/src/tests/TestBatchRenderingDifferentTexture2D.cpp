#include "TestBatchRenderingDifferentTexture2D.h"

namespace test {
	TestBatchRenderingDifferentTexture2D::TestBatchRenderingDifferentTexture2D()
		: m_TranslationA(200, 200, 0), m_TranslationB(200, 400, 0)
	{
		float triangle_position[] = {
			// X, Y, textureCoord_X, textureCoord_Y, Index
			-200.0f, -100.0f, 0.0f, 0.0f, 0.0f, // 0
			 200.0f, -100.0f, 1.0f, 0.0f, 0.0f, // 1
			 200.0f,  100.0f, 1.0f, 1.0f, 0.0f, // 2
			-200.0f,  100.0f, 0.0f, 1.0f, 0.0f, // 3

			 100.0f,  100.0f, 0.0f, 0.0f, 1.0f, // 4
			 500.0f,  100.0f, 1.0f, 0.0f, 1.0f, // 5
			 500.0f,  300.0f, 1.0f, 1.0f, 1.0f, // 6
			 100.0f,  300.0f, 0.0f, 1.0f, 1.0f // 7
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // Como OpenGL irá fazer o blend de Alpha Pixels

		// Criação como Ponteiros pois exportei para uma classe de teste
		m_Shader = std::make_unique<Shader>("res/shaders/BasicDifferentTexture.shader");
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);
		m_VAO = std::make_unique<VertexArray>();

		//VertexBuffer vb(triangle_position, 4 * 4 * sizeof(float)); // X e Y do retangulo + Texture Coordinate
		m_VertexBuffer = std::make_unique<VertexBuffer>(triangle_position, 2 * 4 * 5 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2); // Adicionei o x e y que vou adicionar nas posicoes // layout(position = 0) no shader
		layout.Push<float>(2); // Adicionei a informação sobre o x e y do Texture Coordinate // layout(position = 1) no shader
		layout.Push<float>(1); // layout(location = 2) in float index;
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_Shader->Bind();
		
		m_Texture_SonarQube = std::make_unique<Texture>("res/textures/sonarqube_logo.png");
		m_Texture_SonarQube->Bind(0); // Estou adicionando no Slot 0

		m_Texture_Background = std::make_unique<Texture>("res/textures/background.png");
		m_Texture_Background->Bind(1); // Estou adicionando no Slot 1

		int samplers[2] = { 0, 1 };

		m_Shader->SetUniform1iv("u_Textures", 2, samplers); // o 0 é do Slot da texture passado em Bind
		
		m_VAO->Unbind();
		m_VertexBuffer->Unbind();
		m_IndexBuffer->Unbind();
		m_Shader->Unbind();
	}

	TestBatchRenderingDifferentTexture2D::~TestBatchRenderingDifferentTexture2D()
	{
	}

	void TestBatchRenderingDifferentTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestBatchRenderingDifferentTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		
		m_Shader->Bind();
		// A ordem dos binds das texturas importa no resultado final, dou o bind primeiro na ultima textura adicionada ou coloco o ID das mesmas
		if (!m_ButtonClicked) {
			m_Texture_SonarQube->Bind(0);
			m_Texture_Background->Bind(1);
		} else {
			m_Texture_SonarQube->Bind(1);
			m_Texture_Background->Bind(0);
		}
		
		m_VAO->Bind();
		m_VertexBuffer->Bind();
		m_IndexBuffer->Bind();
		
		Renderer renderer;
		{
			glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
			glm::mat4 view(1.0f);
			view = glm::translate(view, glm::vec3(0, 0, 0)); // Movi a câmera para a direita (Negativo)
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = proj * view * model;

			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}
	void TestBatchRenderingDifferentTexture2D::OnImGUIRender()
	{
		{
			ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 960.0f
			ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
			bool isPressed = ImGui::Button("Change Texture", {0,0});
			if (isPressed) {
				m_ButtonClicked = !m_ButtonClicked;
			}
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
	}
}