Estudos em OpenGL

1 - Download GLFW<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- Framework que pode interagir com os inputs conectados<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- API simples para trabalhar com janelas (Independente do OS), contextos e superfícies<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- Suporta OpenGL, OpenGL ES e Vulkan<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- Suporta multi-monitores<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Tutorial: https://www.glfw.org/docs/latest/quick.html<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Documentação: https://www.glfw.org/docs/latest/<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;URL: https://www.glfw.org/<br />
<br /><br />
2 - Criação da Pasta Lib<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- Incluir os arquivos da pasta include<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- Incluir o arquivo glfw3.lib<br />
<br /><br />
3 - Criação do Projeto no Visual Studio 2019<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- Criação de um projeto vazio<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- Adição da pasta $(SolutionDir)lib\GLFW\include para utilizar o GLFW no momento de compilação (Remove os erros apresentados na IDE)<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- Adição da pasta $(SolutionDir)lib\GLFW\lib-vc2019 para o linker utilizar, irá buscar a glfw3.lib após a compilação para utilização no código.<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- Após adicionar a pasta, é necessário especificar quais .libs irei utilizar. O GLFW utiliza a glfw3.lib; O OpenGL utiliza a opengl32.lib; Para as demais construções no Windows é necessário utilizar as seguintes libs User32.lib;Gdi32.lib;Shell32.lib;<br />
<br /><br />