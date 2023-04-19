#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord; // Estou definindo que no meu layout, na posicao 1, vou ter um vec2 que serao minhas coordenadas
layout(location = 2) in float index;

out vec2 v_TexCoord;// Vou mandar os dados do VertexShader para o FragmentShader
out float v_Index;

uniform mat4 u_MVP; // Irá receber a matriz que irá realizar o calculo para colocar minha imagem em 4:3

void main()
{
    // Ordem importa na multiplicação de matriz
    gl_Position = u_MVP * position;
    v_TexCoord = texCoord;
    v_Index = index;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D u_Textures[2]; // Irá armazenar as informações de coordenada da minha textura

in vec2 v_TexCoord;
in float v_Index;

void main()
{
    int index = int(v_Index);
    vec4 texColor = texture(u_Textures[index], v_TexCoord); // O fragment Shader é executado por cada pixel, entao ele interpola as coordenadas vinda dos Vertex para pegar a posicao certa da textura
    color = texColor; // Pegando a cor da textura
}