#pragma once
#include "GLErrorCheck.h"
#include <string>

class Texture {
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP; // BPP = Bits per pixel
public:
	Texture(const std::string &path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; };
	inline int GetHeight() const { return m_Height; };
	inline int GetRendererID() const { return m_RendererID; };
};
