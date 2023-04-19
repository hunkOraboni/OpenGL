#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

out vec4 v_Color;

uniform mat4 u_MVP; // Ir� receber a matriz que ir� realizar o calculo para colocar minha imagem em 4:3

void main()
{
    // Ordem importa na multiplica��o de matriz
    gl_Position = u_MVP * position;
    v_Color = color;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;

void main()
{
    color = v_Color;
}