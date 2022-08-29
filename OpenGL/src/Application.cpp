#include <GL/glew.h> // Biblioteca do GLEW, responsável pro recuperar as chamadas do OpenGL atual

#include <GLFW/glfw3.h>
#include <iostream>

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

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}