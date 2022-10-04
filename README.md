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

## 5 - Vertex Buffers
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Vertex Buffer é um buffer alocado na GPU (VRAM)<br>
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Shader é um código que irá ser executado na GPU, nele informo como ele precisa ler os Vertex Buffers e desenhar na tela o que eu quero.<br>
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;OpenGL funciona como uma máquina de estado. Ele irá desenhar a partir do Vertex Buffer e Shader que eu selecionar, não sendo necessário eu falar o que precisa fazer.<br>
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Como o OpenGL é executado como máquina de estado, tudo tem um ID (Incluindo os Buffers), para ele poder se localizar
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;GL_ARRAY_BUFFER indica que tipo (target) é a estrutura que eu estou utilizando, neste caso é um Buffer genérico para armazenar Vertex
  - &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Vertex Attributes -> glVertexAttribPointer() Função utilizada para definir atributos dos vertices, só pode ser chamada após a glBindBuffer(). Vou chamar ela toda vez que for definir um atributo.
  - &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**IMPORTANTE** É necessário habilitar a função glVertexAttribPointer(), para fazer isso é necessário acionar a função glEnableVertexAttribArray() (Funciona como máquina de estado, não precisa ser necessariamente antes)

## 6 - Shaders
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Blocos de código para serem executados na GPU<br>
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Vertex Shaders e Fragment (Pixel) Shaders -> Shaders mais focados nos estudos. Existem outros (Ex.: Compute Shaders)<br>
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Quando a função DrawCall é chamada, os Vertex Shaders são executados, depois os Fragments Shaders e por fim vemos o resultado na tela. (Forma Simplificada)<br>
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;O código presente no Vertex Shader é chamado para todo o Vertex que tivermos nos nossos dados que serão renderizados na tela.<br>
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Os Fragment Shaders são chamados para preencher o que está dentro dos Vertex, informando cor por exemplo, eles serão chamados um número maior de vezes, pois um triângulo de 3 vertices pode ser enorme e ter muita informação dentro.<br>
  - &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**IMPORTANTE** Por este motivo se eu quiser otimizar meus cálculos, pode valer a pena rodar em um Vertex Shader e passar o resultado para o Fragment Shader, pois faço a conta menos vezes.<br>
  - &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**IMPORTANTE** Fragment Shaders são utilizados por pixel, pois eles são utilizados para dizer qual cor o pixel deve ser (Após verificar luz, material, ambiente, etc).
  - &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**IMPORTANTE** Vertex Shaders são utilizados por vertex, pois eles definem a posição na tela do pixel.

## 7 - Escrevendo o Shader
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;É possível escrever o Shader direto no código (Pouco recomendado), em C++ tem as RAW Strings, utilizando R"nome(CODIGO)nome";
```
std::string vertexShader = R"vertexShader(
        #version 330 core

        layout(location = 0) in vec4 position;

        void main()
        {
            gl_Position = position;
        }
    )vertexShader";
```

## 8 - Index Buffer
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Utilizo para salvar memória na criação dos vertices.
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Um quadrado são dois triangulos com dois vertices em comum, por isso crio 4 vertices e 1 Index Buffer com 6 posicoes, nele indico como ler as informações.
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Substituo o glDrawArrays por glDrawElements.
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Todos os Index Buffer precisam ser ```unsigned int```

## 9 - Dealing with Errors
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;A função glGetError() retorna os erros que estão na pilha de erros, porém é necessário chamar ela para cada error (Uma função pode gerar mais de um erro)
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Uma forma de melhorar o código é criar macros (define) para limpar todos erros através de um loop e depois verificar o erro para a minha função específica

## 10 - Uniforms
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Forma de utilizar variáveis entre o shader e minha aplicação, assim poderei passar data em tempo de execução
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Ele é executado sempre antes de cada chamada de draw

docs.gl = Link com uma boa documentação
https://www.khronos.org/opengl/

<!-- - &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; -->
