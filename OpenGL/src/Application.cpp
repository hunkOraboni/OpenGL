#include <GL/glew.h> // Biblioteca do GLEW, responsável pro recuperar as chamadas do OpenGL atual

#include <GLFW/glfw3.h>
#include <iostream>

// Função para compilar os Shaders
static unsigned int CompileShader(unsigned int type, const std::string &source) {    
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
}

// Função para receber o texto dos Shaders, então utilizar o OpenGL para compilar e utilizar
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
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
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "Erro na inicialização do GLFW" << std::endl;
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

    // Precisa inicializar após a criação do Context
    GLenum resultGlewInit = glewInit();
    if (GLEW_OK != resultGlewInit) { // Verifica se conseguir inicializar o GLEW
        std::cout << "Erro na inicialização do GLEW" << std::endl;
        return -1;
    }

    // Retornar versão do OpenGL
    std::cout << glGetString(GL_VERSION) << std::endl;

    float triangle_position[6] = {
        -0.5f, -0.5f,
         0.0f, 0.5f,
         0.5f, -0.5f
    };

    // Quantidade Vertex Buffers que irei criar e onde eu vou armazenar a referencia (ID do buffer gerado)
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    // Para usar o buffer eu preciso passar o tipo final dele e ID dele
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // Após selecionar, preciso colocar dados dentro do Buffer
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), triangle_position, GL_STATIC_DRAW);
    // Definição de atributos (Posicao)
    // 0 é o indice (primeiro atributo)
    // 2 é o tamanho que representa cada vertex no vetor, X e Y
    // GL_FLOAT é o tipo da variável que eu tenho
    // GL_FALSE é se quero normalizar ou não, o float já está normalizado (Valor entre 0 e 1)
    // stride é a quantidade de bytes que temos separando cada vertex (no caso X e Y, cada um 4 bytes, sendo 8 no total)
    // 0 é a posição (ponteiro) onde irá começar a analisar o vetor para poder ler os valores (Já defini em glBufferData qual o vetor)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    // A função vai utilizar o index 0 para habilitar a leitura do array e o OpenGL saber como interpretar os dados fornecidos
    glEnableVertexAttribArray(0); // Como funciona como máquina de estado, eu não preciso necessariamente habilitar antes de glVertexAttribPointer

    // Shader
    std::string vertexShader =
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
        ;
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

        // Chamada para desenhar na tela um Array (Meu buffer é deste tipo), nele passo qual será o formato da minha imagem, onde começo a ler os valores e quantos valores são (3 vertices)
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Limpo tudo da memória
    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}