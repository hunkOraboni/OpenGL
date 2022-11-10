#include "Shader.h"

Shader::Shader(const std::string& filepath)
	: m_Filepath(filepath), m_RendererID(0)
{
	//Shader::CompileShader();
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

//bool Shader::CompileShader() {
//	return true;
//}

// Função para compilar os Shaders
unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    // Crio um Shader do tipo especificado
    unsigned int id = glCreateShader(type);
    // Pego o código do meu shader (Posso puxar um arquivo, internet, etc...) e pego o endereco da primeira posicao
    const char* src = source.c_str();
    // Aplica o texto (src) no meu shader (id), faço isso 1 vez e o nullptr significa que a minha string terminal com null (Não preciso passar o length dela)
    glShaderSource(id, 1, &src, nullptr);
    // Compilo o Shader efetivamente, traduzindo o código para o mesmo
    glCompileShader(id);

    // TODO: Error Handling
    int result;
    // Verifico o status de compilação do Shader específico e armazeno em result
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        // Caso tenha dado erro
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        // Erro para declarar na stack pois length é uma variável que pode ter qualquer tamanho
        //char message[length];
        // alloca -> Faz alocação de memória na stack
        char* message = (char*)(alloca(sizeof(char) * length));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Falha na Compilacao do Shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

// Função para receber o texto dos Shaders, então utilizar o OpenGL para compilar e utilizar
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = Shader::CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = Shader::CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Adiciono os Shaders no Program
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    // TODO: Ver estas funções, porém eu não fico alterando muito este comportamento
    glLinkProgram(program);
    glValidateProgram(program);

    // Após eu já usar os Shaders no meu programa eu posso excluí-los (Eles servem como arquivo temporário
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath); // Abre o arquivo

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2]; // 2 streams, um para vertex e outro para fragment
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        // npos = Posicao invalida na string
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            if (type != ShaderType::NONE) {
                ss[(int)type] << line << '\n';
            }
        }
    }
    /*ShaderProgramSource shaders;
    shaders.vertexSource = ss[0].str();
    shaders.fragmentSource = ss[1].str();
    return shaders;*/
    return { ss[0].str(), ss[1].str() };
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string &name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }
    GLCall(int uniform_location = glGetUniformLocation(m_RendererID, name.c_str())); // Se retornar -1 é porque não encontrou o uniform
    if (uniform_location == -1) {
        std::cout << "Warning: Uniform '" << name << "' não existe" << std::endl;
    }
    m_UniformLocationCache[name] = uniform_location;
    return uniform_location;
}
