#include "TestBatchRenderingDifferentColor2D.h"

namespace test {
	TestBatchRenderingDifferentColor2D::TestBatchRenderingDifferentColor2D()
		: m_TranslationA(200, 200, 0), m_TranslationB(200, 400, 0)
	{
		float triangle_position[] = {
			// Layout
			// X, Y, R, G, B, Alpha
			-100.0f, -100.0f, 0.18f, 0.6f, 0.96f, 1.0f, // 0
			 -50.0f, -100.0f, 0.18f, 0.6f, 0.96f, 1.0f, // 1
			 -50.0f,  -50.0f, 0.18f, 0.6f, 0.96f, 1.0f, // 2
			-100.0f,  -50.0f, 0.18f, 0.6f, 0.96f, 1.0f, // 3

			  50.0f,  -100.0f, 1.0f, 0.93f, 0.24f, 1.0f, // 4
			 100.0f,  -100.0f, 1.0f, 0.93f, 0.24f, 1.0f, // 5
			 100.0f,   -50.0f, 1.0f, 0.93f, 0.24f, 1.0f, // 6
			  50.0f,   -50.0f, 1.0f, 0.93f, 0.24f, 1.0f  // 7
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
		m_Shader = std::make_unique<Shader>("res/shaders/BasicDifferentColor.shader");
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);
		m_VAO = std::make_unique<VertexArray>();

		// 2 * 6 * 4 => Qt Objetos * Qt Valores * Qt Vertex
		m_VertexBuffer = std::make_unique<VertexBuffer>(triangle_position, 2 * 6 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2); // layout(location = 0) in vec4 position;
		layout.Push<float>(4); // layout(location = 1) in vec4 color;
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_Shader->Bind();
		
		m_VAO->Unbind();
		m_VertexBuffer->Unbind();
		m_IndexBuffer->Unbind();
		m_Shader->Unbind();
	}

	TestBatchRenderingDifferentColor2D::~TestBatchRenderingDifferentColor2D()
	{
	}

	void TestBatchRenderingDifferentColor2D::OnUpdate(float deltaTime)
	{
	}

	void TestBatchRenderingDifferentColor2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		
		m_Shader->Bind();
		m_VAO->Bind();
		m_VertexBuffer->Bind();
		m_IndexBuffer->Bind();
		
		Renderer renderer;
		{
			glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
			glm::mat4 view(1.0f);
			view = glm::translate(view, glm::vec3(0, 0, 0)); // Movi a câmera para a direita (Negativo)
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			//glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
			//glm::mat4 mvp = proj * view * model;
			glm::mat4 mvp = proj * view * model;

			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

	}
	void TestBatchRenderingDifferentColor2D::OnImGUIRender()
	{
		{
			ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 960.0f
			ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
	}
}