#include <GL/glew.h> // Biblioteca do GLEW, respons�vel pro recuperar as chamadas do OpenGL atual

#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream> // Ler Arquivos
#include <string> // getline
#include <sstream> // stringStream

struct ShaderProgramSource {
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderProgramSource ParseShader(const std::string & filepath) {
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
    /*ShaderProgramSource shaders;
    shaders.vertexSource = ss[0].str();
    shaders.fragmentSource = ss[1].str();
    return shaders;*/
    return { ss[0].str(), ss[1].str() };
}

// Fun��o para compilar os Shaders
static unsigned int CompileShader(unsigned int type, const std::string &source) {    
    // Crio um Shader do tipo especificado
    unsigned int id = glCreateShader(type);
    // Pego o c�digo do meu shader (Posso puxar um arquivo, internet, etc...) e pego o endereco da primeira posicao
    const char* src = source.c_str();
    // Aplica o texto (src) no meu shader (id), fa�o isso 1 vez e o nullptr significa que a minha string terminal com null (N�o preciso passar o length dela)
    glShaderSource(id, 1, &src, nullptr);
    // Compilo o Shader efetivamente, traduzindo o c�digo para o mesmo
    glCompileShader(id);

    // TODO: Error Handling
    int result;
    // Verifico o status de compila��o do Shader espec�fico e armazeno em result
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        // Caso tenha dado erro
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        // Erro para declarar na stack pois length � uma vari�vel que pode ter qualquer tamanho
        //char message[length];
        // alloca -> Faz aloca��o de mem�ria na stack
        char* message = (char*)(alloca(sizeof(char) * length));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Falha na Compilacao do Shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

// Fun��o para receber o texto dos Shaders, ent�o utilizar o OpenGL para compilar e utilizar
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Adiciono os Shaders no Program
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    // TODO: Ver estas fun��es, por�m eu n�o fico alterando muito este comportamento
    glLinkProgram(program);
    glValidateProgram(program);

    // Ap�s eu j� usar os Shaders no meu programa eu posso exclu�-los (Eles servem como arquivo tempor�rio
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "Erro na inicializa��o do GLFW" << std::endl;
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Precisa inicializar ap�s a cria��o do Context
    GLenum resultGlewInit = glewInit();
    if (GLEW_OK != resultGlewInit) { // Verifica se conseguir inicializar o GLEW
        std::cout << "Erro na inicializa��o do GLEW" << std::endl;
        return -1;
    }

    // Retornar vers�o do OpenGL
    std::cout << glGetString(GL_VERSION) << std::endl;

    float triangle_position[] = {
        -0.5f, -0.5f, // 0
         0.5f, -0.5f, // 1
         0.5f,  0.5f, // 2
        -0.5f,  0.5f  // 3
    };

    // Para desenhar um quadrado, preciso de 2 triangulos, mas os vertices se conectam, para salvar mem�ria, irei utilizar o ponto j� definido.
    // IndexBuffer � simplesmente isso
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // Quantidade Vertex Buffers que irei criar e onde eu vou armazenar a referencia (ID do buffer gerado)
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    // Para usar o buffer eu preciso passar o tipo final dele e ID dele
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // Ap�s selecionar, preciso colocar dados dentro do Buffer
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), triangle_position, GL_STATIC_DRAW);
    // Defini��o de atributos (Posicao)
    // 0 � o indice (primeiro atributo)
    // 2 � o tamanho que representa cada vertex no vetor, X e Y
    // GL_FLOAT � o tipo da vari�vel que eu tenho
    // GL_FALSE � se quero normalizar ou n�o, o float j� est� normalizado (Valor entre 0 e 1)
    // stride � a quantidade de bytes que temos separando cada vertex (no caso X e Y, cada um 4 bytes, sendo 8 no total)
    // 0 � a posi��o (ponteiro) onde ir� come�ar a analisar o vetor para poder ler os valores (J� defini em glBufferData qual o vetor)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    // A fun��o vai utilizar o index 0 para habilitar a leitura do array e o OpenGL saber como interpretar os dados fornecidos
    glEnableVertexAttribArray(0); // Como funciona como m�quina de estado, eu n�o preciso necessariamente habilitar antes de glVertexAttribPointer

    // Utiliza��o de um IndexBuffer
    unsigned int ibo; // ID do meu IndexBuffer
    glGenBuffers(1, &ibo);
    // Uso outro tipo de buffer, o GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    // Ap�s selecionar, preciso colocar dados dentro do Buffer (Sao 6 indices)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

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
        // layout(location = 0) => � o meu vertexPointer na posi��o 0, definido na fun��o glVertexAttribPointer
        // vec4 = gl_Position � um vec4, mesmo eu tendo um triangulo vec2
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
    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
    unsigned int shaderFile = CreateShader(source.vertexSource, source.fragmentSource);
    glUseProgram(shaderFile);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // OpenGL Legacy
        /*glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();*/

        // Chamada para desenhar na tela um Array (Meu buffer � deste tipo), nele passo qual ser� o formato da minha imagem, onde come�o a ler os valores e quantos valores s�o (3 vertices)
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        // Mode, quantidade de indices, tipo de dado que vou usar, ja fiz o bind do buffer na linha 165, por isso pode ser nullptr
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Limpo tudo da mem�ria
    glDeleteProgram(shader);
    glDeleteProgram(shaderFile);

    glfwTerminate();
    return 0;
}