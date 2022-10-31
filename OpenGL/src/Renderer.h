#pragma once

#include "GLErrorCheck.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer {
	// Pode ser uma boa abordagem ser uma classe Singleton
public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};