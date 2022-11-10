#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map> //Hash Map

#include "GLErrorCheck.h"

struct ShaderProgramSource {
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader {
private:
	unsigned int m_RendererID;
	std::string m_Filepath;
	std::unordered_map<std::string, int> m_UniformLocationCache; // Vou usar para nao ficar sempre buscando com getUniformLocation
public:
	Shader(const std::string &filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set Uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
private:
	int GetUniformLocation(const std::string &name);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderProgramSource ParseShader(const std::string& filepath);
};