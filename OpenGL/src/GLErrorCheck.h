#include <GL/glew.h>
#pragma once

// Fun��o para verificar caso eu tenha algum erro e quebrar o c�digo no momento (linha) que isto ocorrer
#define ASSERT(x) if (!(x)) __debugbreak();
// Macro para n�o ter que executar a limpeza dos erros e validar se tenho algum erro
// #x = Transforma x em um stream (texto)
// __FILE__ = Arquivo que gerou o erro
// __LINE__ = Linha que gerou o erro
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);