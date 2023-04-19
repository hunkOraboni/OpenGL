#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord; // Estou definindo que no meu layout, na posicao 1, vou ter um vec2 que serao minhas coordenadas

out vec2 v_TexCoord;// Vou mandar os dados do VertexShader para o FragmentShader

uniform mat4 u_MVP; // Ir� receber a matriz que ir� realizar o calculo para colocar minha imagem em 4:3

void main()
{
    // Ordem importa na multiplica��o de matriz
    gl_Position = u_MVP * position;
    v_TexCoord = texCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

in vec2 v_TexCoord;

void main()
{
    //color = vec4(0.0, 1.0, 0.0, 1.0);
    color = u_Color; // Definindo as cores manualmente
}