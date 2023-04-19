#include "TestTexture2D.h"

namespace test {
	TestTexture2D::TestTexture2D()
		: m_TranslationA(200, 200, 0), m_TranslationB(200, 400, 0)
	{
		float triangle_position[] = {
			-200.0f, -100.0f, 0.0f, 0.0f, // 0
			 200.0f, -100.0f, 1.0f, 0.0f, // 1
			 200.0f,  100.0f, 1.0f, 1.0f, // 2
			-200.0f,  100.0f, 0.0f, 1.0f  // 3
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // Como OpenGL ir� fazer o blend de Alpha Pixels

		// Cria��o como Ponteiros pois exportei para uma classe de teste
		m_Shader = std::make_unique<Shader>("res/shaders/BasicTexture.shader");
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);
		m_VAO = std::make_unique<VertexArray>();

		//VertexBuffer vb(triangle_position, 4 * 4 * sizeof(float)); // X e Y do retangulo + Texture Coordinate
		m_VertexBuffer = std::make_unique<VertexBuffer>(triangle_position, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2); // Adicionei o x e y que vou adicionar nas posicoes // layout(position = 0) no shader
		layout.Push<float>(2); // Adicionei a informa��o sobre o x e y do Texture Coordinate // layout(position = 1) no shader
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_Shader->Bind();
		
		m_Texture = std::make_unique<Texture>("res/textures/sonarqube_logo.png");
		m_Texture->Bind();
		m_Shader->SetUniform1i("u_Texture", 0); // o 0 � do Slot da texture passado em Bind
		
		m_VAO->Unbind();
		m_VertexBuffer->Unbind();
		m_IndexBuffer->Unbind();
		m_Shader->Unbind();
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		
		m_Shader->Bind();
		m_Texture->Bind();
		m_VAO->Bind();
		m_VertexBuffer->Bind();
		m_IndexBuffer->Bind();
		
		Renderer renderer;
		{
			glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
			glm::mat4 view(1.0f);
			view = glm::translate(view, glm::vec3(0, 0, 0)); // Movi a c�mera para a direita (Negativo)
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			//glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
			//glm::mat4 mvp = proj * view * model;
			glm::mat4 mvp = proj * view * model;

			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

		// Desenhando um segundo objeto
		{
			glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
			glm::mat4 view(1.0f);
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = proj * view * model;
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

	}
	void TestTexture2D::OnImGUIRender()
	{
		{
			ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 960.0f
			ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
	}
}