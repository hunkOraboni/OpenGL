#pragma once
class VertexBuffer {
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size, unsigned int usage = GL_STATIC_DRAW);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	// Dynamic Geometry (Passo os dados do meu VertexBuffer em tempo de execução
	void SetBufferSubData(void* data, unsigned int size);
};