# Estudos em OpenGL
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;https://www.youtube.com/watch?v=OR4fNpBjmq8&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&index=1

## 1 - Download GLFW<br>
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Framework que pode interagir com os inputs conectados<br>
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;API simples para trabalhar com janelas (Independente do OS), contextos e superfícies<br>
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Suporta OpenGL, OpenGL ES e Vulkan<br>
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Suporta multi-monitores<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Tutorial: https://www.glfw.org/docs/latest/quick.html<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Documentação: https://www.glfw.org/docs/latest/<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;URL: https://www.glfw.org/<br>

## 2 - Criação da Pasta Lib<br>
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Incluir os arquivos da pasta include<br>
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Incluir o arquivo glfw3.lib<br>

## 3 - Criação do Projeto no Visual Studio 2019<br>
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Criação de um projeto vazio<br>
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Adição da pasta $(SolutionDir)lib\GLFW\include para utilizar o GLFW no momento de compilação (Remove os erros apresentados na IDE)<br>
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Adição da pasta $(SolutionDir)lib\GLFW\lib-vc2019 para o linker utilizar, irá buscar a glfw3.lib após a compilação para utilização no código.<br>
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Após adicionar a pasta, é necessário especificar quais .libs irei utilizar. O GLFW utiliza a glfw3.lib; O OpenGL utiliza a opengl32.lib; Para as demais construções no Windows é necessário utilizar as seguintes libs User32.lib;Gdi32.lib;Shell32.lib;<br>

## 4 - GLEW
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;OpenGL é implementado pelo driver da placa de vídeo, para isso preciso das assinaturas das funções implementadas<br>
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Após o Download, adicionar a pasta `$(SolutionDir)lib\GLEW\glew-2.1.0\include` no Include e a pasta `$(SolutionDir)lib\GLEW\glew-2.1.0\lib\Release\Win32` no Linker<br>
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Adicionar a lib que quero em Entrada no Linker glew32s.lib (Há duas libs na pasta, porém a com s no fim é para link estático sem a necessidade do .dll)<br>
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;A biblioteca GL/glew.h precisa ser incluída antes que qualquer outra biblioteca OpenGL no programa
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Ao utilizar static link, irei ter erro de linker na hora de usar a função glewInit, pois ele esperar que seja linkado através de uma DLL com GLEW_BUILD.<br>
  - &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Para resolver este problema, preciso definir GLEW_STATIC<br>
  - &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**IMPORTANTE** Como fazer: Definir como preprocessor GLEW_STATIC (No Visual Studio é em Propriedades->C/C++->Pré-Processador)<br>
  - &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**IMPORTANTE** glewInit() não pode ser invocado antes de criar um contexto válido na aplicação (glfwMakeContextCurrent(window);)<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;URL: http://glew.sourceforge.net/

<<<<<<< HEAD
## 5 - Vertex Buffers e Shaders
Vertex Buffer é um buffer alocado na GPU (VRAM)
Shader é um código que irá ser executado na GPU, nele informo como ele precisa ler os Vertex Buffers e desenhar na tela o que eu quero
OpenGL funciona como uma máquina de estado. Ele irá desenhar a partir do Vertex Buffer e Shader que eu selecionar, não sendo necessário eu falar o que precisa fazer.

<!-- - &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; -->
=======
<!-- - &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; -->
>>>>>>> cd3d6a42bcae4639dd0b852c51fce9026d55064c
