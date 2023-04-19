#include <GL/glew.h> // Biblioteca do GLEW, responsável pro recuperar as chamadas do OpenGL atual

#include <GLFW/glfw3.h>
#include <iostream>
//#include <fstream> // Ler Arquivos
#include <string> // getline
#include <sstream> // stringStream

#include "Renderer.h"
#include "GLErrorCheck.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
//#include "tests/TestClearColor.h"
#include "tests/TestFramework.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
#include "vendor/imgui/imgui_impl_glfw.h"

/*struct ShaderProgramSource {
	std::string vertexSource;
	std::string fragmentSource;
};*/

/*static ShaderProgramSource ParseShader(const std::string & filepath) {
	std::ifstream stream(filepath); // Abre o arquivo

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2]; // 2 streams, um para vertex e outro para fragment
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		// npos = Posicao invalida na string
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			} else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		} else {
			if (type != ShaderType::NONE) {
				ss[(int)type] << line << '\n';
			}
		}
	}
	//ShaderProgramSource shaders;
	//shaders.vertexSource = ss[0].str();
	//shaders.fragmentSource = ss[1].str();
	//return shaders;
	return { ss[0].str(), ss[1].str() };
}*/

// Função para compilar os Shaders
/*static unsigned int CompileShader(unsigned int type, const std::string& source) {
	// Crio um Shader do tipo especificado
	unsigned int id = glCreateShader(type);
	// Pego o código do meu shader (Posso puxar um arquivo, internet, etc...) e pego o endereco da primeira posicao
	const char* src = source.c_str();
	// Aplica o texto (src) no meu shader (id), faço isso 1 vez e o nullptr significa que a minha string terminal com null (Não preciso passar o length dela)
	glShaderSource(id, 1, &src, nullptr);
	// Compilo o Shader efetivamente, traduzindo o código para o mesmo
	glCompileShader(id);

	// TODO: Error Handling
	int result;
	// Verifico o status de compilação do Shader específico e armazeno em result
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		// Caso tenha dado erro
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		// Erro para declarar na stack pois length é uma variável que pode ter qualquer tamanho
		//char message[length];
		// alloca -> Faz alocação de memória na stack
		char* message = (char*)(alloca(sizeof(char) * length));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Falha na Compilacao do Shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}*/

// Função para receber o texto dos Shaders, então utilizar o OpenGL para compilar e utilizar
/*static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// Adiciono os Shaders no Program
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	// TODO: Ver estas funções, porém eu não fico alterando muito este comportamento
	glLinkProgram(program);
	glValidateProgram(program);

	// Após eu já usar os Shaders no meu programa eu posso excluí-los (Eles servem como arquivo temporário
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}*/

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit()) {
		std::cout << "Erro na inicialização do GLFW" << std::endl;
		return -1;
	}

	// Estou falando para usar a versao 3.3 do OpenGL e o Core Profile (Default = Compatibility)
	// O Core Profile não utiliza um VAO (Vertex Array Object) por padrão, então eu preciso criar um manualmente
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	//window = glfwCreateWindow(1366, 768, "Hello World", NULL, NULL);
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// Ativa o vsync
	glfwSwapInterval(1);

	// Precisa inicializar após a criação do Context
	GLenum resultGlewInit = glewInit();
	if (GLEW_OK != resultGlewInit) { // Verifica se conseguir inicializar o GLEW
		std::cout << "Erro na inicialização do GLEW" << std::endl;
		return -1;
	}

	// Retornar versão do OpenGL
	std::cout << glGetString(GL_VERSION) << std::endl;
	{

		Renderer renderer;

		// Inclusão do ImGUI
		ImGui::CreateContext();
		
		ImGui::StyleColorsDark();

		//ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;

		testMenu->RegisterTest<test::TestClearColor>("Clear Color");
		testMenu->RegisterTest<test::TestTexture2D>("Texture 2D");
		testMenu->RegisterTest<test::TestBatchRenderingTexture2D>("Batch Rendering - Texture 2D");
		testMenu->RegisterTest<test::TestBatchRenderingDifferentTexture2D>("Batch Rendering - Different Texture 2D");
		testMenu->RegisterTest<test::TestBatchRenderingSameColor2D>("Batch Rendering - Same Color 2D");
		testMenu->RegisterTest<test::TestBatchRenderingDifferentColor2D>("Batch Rendering - Different Color 2D");

		/*bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);*/

		/*
		float red = 0.0f;
		float increment = 0.05f;*/
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			//GLCall(glClear(GL_COLOR_BUFFER_BIT));
			renderer.Clear();
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			if (currentTest) {
				currentTest->OnUpdate(0.0f);
				currentTest->OnRender();
				ImGui::Begin("Teste");
				if (currentTest != testMenu && ImGui::Button("<-")) {
					// Botao de voltar para o menu, só entro no if se clicar no botão de voltar
					delete currentTest;
					currentTest = testMenu;
				}
				currentTest->OnImGUIRender();
				ImGui::End();
			}


			// OpenGL Legacy
			/*glBegin(GL_TRIANGLES);
			glVertex2f(-0.5f, -0.5f);
			glVertex2f(0.0f, 0.5f);
			glVertex2f(0.5f, -0.5f);
			glEnd();*/

			// Chamada para desenhar na tela um Array (Meu buffer é deste tipo), nele passo qual será o formato da minha imagem, onde começo a ler os valores e quantos valores são (3 vertices)
			//glDrawArrays(GL_TRIANGLES, 0, 3);

			/*GLClearError();
			// Mode, quantidade de indices, tipo de dado que vou usar, ja fiz o bind do buffer na linha 165, por isso pode ser nullptr
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
			ASSERT(GLLogCall());*/
			//va.Bind();
			//ib.Bind();
			//shader.Bind();
			// Alterar a cor vermelha
			//GLCall(glUniform4f(uniform_location, red, 0.2f, 0.0f, 1.0f));
			//shader.SetUniform4f("u_Color", red, 0.2f, 0.0f, 1.0f);

			/*if ((red > 1.0f) || (red < 0.0f)) {
				increment *= -1;
			}
			red += increment;*/
			/*
			
			// Para não ter que ficar limpando log, utilizo essa macro para fazer isso sempre
			//GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			*/
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
		delete currentTest;
		if (currentTest != testMenu) {
			delete testMenu;
		}
	}

	// Limpo tudo da memória
	//GLCall(glDeleteProgram(shader));
	//GLCall(glDeleteProgram(shaderFile));

	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}