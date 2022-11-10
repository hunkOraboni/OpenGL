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
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
		float triangle_position[] = {
			// x, y, texture coordinate x e y
			-0.5f, -0.5f, 0.0f, 0.0f, // 0
			 0.5f, -0.5f, 1.0f, 0.0f, // 1
			 0.5f,  0.5f, 1.0f, 1.0f, // 2
			-0.5f,  0.5f, 0.0f, 1.0f  // 3
		};

		// Para desenhar um quadrado, preciso de 2 triangulos, mas os vertices se conectam, para salvar memória, irei utilizar o ponto já definido.
		// IndexBuffer é simplesmente isso
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		// Irá pegar SRC_ALPHA vindo do Fragment Shader e vai fazer (1 - ele), para ter a real cor do fragmento // Utilizado para exibir pixels transparentes
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // Como OpenGL irá fazer o blend de Alpha Pixels

		// Criar VAO
		// Posso ter mais de um VAO, um para cada objeto que quero renderizar, assim eu não preciso ficar chamando as funções glVertexAttribPointer e tendo que reconfigurar tudo sempre
		/*unsigned int vao;
		GLCall(glGenVertexArrays(1,&vao));
		GLCall(glBindVertexArray(vao));*/

		VertexArray va;

		// Esta função faz o mesmo que as linhas abaixo
		//VertexBuffer vb(triangle_position, 4 * 2 * sizeof(float)); // Somente X e Y do retangulo
		VertexBuffer vb(triangle_position, 4 * 4 * sizeof(float)); // X e Y do retangulo + Texture Coordinate

		VertexBufferLayout layout;
		layout.Push<float>(2); // Adicionei o x e y que vou adicionar nas posicoes // layout(position = 0) no shader
		layout.Push<float>(2); // Adicionei a informação sobre o x e y do Texture Coordinate // layout(position = 1) no shader
		va.AddBuffer(vb, layout);

		// Quantidade Vertex Buffers que irei criar e onde eu vou armazenar a referencia (ID do buffer gerado)
		/*unsigned int buffer;
		GLCall(glGenBuffers(1, &buffer));
		// Para usar o buffer eu preciso passar o tipo final dele e ID dele
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
		// Após selecionar, preciso colocar dados dentro do Buffer
		GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), triangle_position, GL_STATIC_DRAW));*/
		// Definição de atributos (Posicao)
		// 0 é o indice (primeiro atributo)
		// 2 é o tamanho que representa cada vertex no vetor, X e Y
		// GL_FLOAT é o tipo da variável que eu tenho
		// GL_FALSE é se quero normalizar ou não, o float já está normalizado (Valor entre 0 e 1)
		// stride é a quantidade de bytes que temos separando cada vertex (no caso X e Y, cada um 4 bytes, sendo 8 no total)
		// 0 é a posição (ponteiro) onde irá começar a analisar o vetor para poder ler os valores (Já defini em glBufferData qual o vetor)
		//GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
		// A função vai utilizar o index 0 para habilitar a leitura do array e o OpenGL saber como interpretar os dados fornecidos
		//GLCall(glEnableVertexAttribArray(0)); // Como funciona como máquina de estado, eu não preciso necessariamente habilitar antes de glVertexAttribPointer
		// ** Vertex Array Objects ** -> O indice 0 das funções acima estão relacionadas ao VAO, no Core_Profile eu preciso instanciar um VAO antes para poder utilizar


		IndexBuffer ib(indices, 6);
		// Utilização de um IndexBuffer
		/*unsigned int ibo; // ID do meu IndexBuffer
		GLCall(glGenBuffers(1, &ibo));
		// Uso outro tipo de buffer, o GL_ELEMENT_ARRAY_BUFFER
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
		// Após selecionar, preciso colocar dados dentro do Buffer (Sao 6 indices)
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));*/

		// Shader
		std::string vertexShader = R"vertexShader(
        #version 330 core

        layout(location = 0) in vec4 position;

        void main()
        {
            gl_Position = position;
        }
    )vertexShader";
		/*std::string vertexShader =
			// https://en.wikipedia.org/wiki/OpenGL_Shading_Language
			// https://www.khronos.org/opengl/wiki/Core_Language_(GLSL)
			"#version 330 core\n"
			"\n"
			// layout(location = 0) => É o meu vertexPointer na posição 0, definido na função glVertexAttribPointer
			// vec4 = gl_Position é um vec4, mesmo eu tendo um triangulo vec2
			"layout(location = 0) in vec4 position;\n"
			"\n"
			"void main()\n"
			"{\n"
				// Vai pegar ambos x e y que tenho definido e fazer o cast para vec4
			"   gl_Position = position;\n"
			"}\n"
			;*/
		std::string fragmentShader =
			"#version 330 core\n"
			"\n"
			"layout(location = 0) out vec4 color;\n"
			"\n"
			"void main()\n"
			"{\n"
			// Vou adicionar a cor vermelha em todos os pixels do meu triangulo
			"   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
			"}\n"
			;
		//unsigned int shader = CreateShader(vertexShader, fragmentShader);
		//GLCall(glUseProgram(shader));

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		/*ShaderProgramSource source = Shader::ParseShader("res/shaders/Basic.shader");
		unsigned int shaderFile = Shader::CreateShader(source.vertexSource, source.fragmentSource);*/
		//GLCall(glUseProgram(shader));

		//shader.SetUniform4f("u_Color", 0.3f, 0.2f, 0.0f, 1.0f);
		//GLCall(int uniform_location = glGetUniformLocation(shaderFile, "u_Color")); // Se retornar -1 é porque não encontrou o uniform
		//ASSERT(uniform_location != -1);
		// Eu preciso executar esta função depois de escolher um shader, pois nele estará meu uniform
		//GLCall(glUniform4f(uniform_location, 0.3f, 0.2f, 0.0f, 1.0f));

		Texture texture("res/textures/sonarqube_logo.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0); // o 0 é do Slot da texture passado em Bind

		va.Unbind();
		shader.Unbind();
		vb.Unbind();
		ib.Unbind();
		//GLCall(glUseProgram(0));

		Renderer renderer;

		float red = 0.0f;
		float increment = 0.05f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			//GLCall(glClear(GL_COLOR_BUFFER_BIT));
			renderer.Clear();

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
			shader.Bind();
			// Alterar a cor vermelha
			//GLCall(glUniform4f(uniform_location, red, 0.2f, 0.0f, 1.0f));
			//shader.SetUniform4f("u_Color", red, 0.2f, 0.0f, 1.0f);

			if ((red > 1.0f) || (red < 0.0f)) {
				increment *= -1;
			}
			red += increment;

			renderer.Draw(va, ib, shader);

			// Para não ter que ficar limpando log, utilizo essa macro para fazer isso sempre
			//GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));


			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	// Limpo tudo da memória
	//GLCall(glDeleteProgram(shader));
	//GLCall(glDeleteProgram(shaderFile));

	glfwTerminate();
	return 0;
}