#include "graphics/Shader.h"

Shader::Shader():
    vertex_shader_id(0), fragment_shader_id(0)
{
    this->shader_id = glCreateProgram();
}

Shader::Shader(const char* vertex_shader_path, const char* fragment_shader_path):
    vertex_shader_id(0), fragment_shader_id(0)
{
    this->shader_id = glCreateProgram();
    this->setVertexShaderFile(vertex_shader_path);
    this->setFragmentShaderFile(fragment_shader_path);
}
Shader::~Shader()
{
    if( this->fragment_shader_id > 0 )
    {
        glDeleteShader(this->fragment_shader_id);
    }
    if( this->vertex_shader_id > 0 )
    {
        glDeleteShader(this->vertex_shader_id);
    }
    if( this->shader_id > 0 )
    {
        glDeleteProgram(this->shader_id);
    }
}
GLuint Shader::getProgramId()
{
    return this->shader_id;
}

bool Shader::setVertexShaderFile(const char* vertex_shader_path)
{
    this->vertex_shader_id = this->compileShader(
            glCreateShader(GL_VERTEX_SHADER),
            this->loadShaderCode(vertex_shader_path));
    if( !this->verifyShader(this->vertex_shader_id))
    {
        fprintf(stderr, "An error in the vertex shader was found. \n");
        this->vertex_shader_id = 0;
        return false;
    }
    glAttachShader(this->shader_id, this->vertex_shader_id);
    return true;
}

bool Shader::setFragmentShaderFile(const char* fragment_shader_path)
{
    this->fragment_shader_id = this->compileShader(
            glCreateShader(GL_FRAGMENT_SHADER),
            this->loadShaderCode(fragment_shader_path));
    if( !this->verifyShader(this->fragment_shader_id))
    {
        fprintf(stderr, "An error in the fragment shader was found. \n");
        this->fragment_shader_id = 0;
        return false;
    }
    glAttachShader(this->shader_id, this->fragment_shader_id);
    return true;
}

std::string Shader::loadShaderCode(const char* file_path)
{
    std::string shader_code;
    std::ifstream shader_code_stream(file_path, std::ios::in);
    if( shader_code_stream.is_open() )
    {
        std::string line = "";
        while( std::getline(shader_code_stream, line) )
        {
            shader_code += "\n" + line;
        }
        shader_code_stream.close();
    }
    else
    {
        fprintf(stderr, "Failed to open shader file: %s.\n", file_path);
        return "";
    }
    return shader_code;
}

GLuint Shader::compileShader(GLuint shader_id, std::string shader_code)
{
    /* GLuint shader_id = 0; */
    printf("Compiling shader\n");

    // Cast the std::string to a c-string
    const char* shader_code_str = shader_code.c_str();

    // Compile
    glShaderSource(shader_id, 1, &shader_code_str, NULL);
    glCompileShader(shader_id);
    return shader_id;
}

bool Shader::verifyShader(GLuint shader_id)
{
    GLint compile_result = GL_FALSE;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_result);
    if( compile_result == GL_TRUE )
    {
        // All is good!
        return true;
    }
    fprintf(stderr, "Shader got error..\r");
    int log_string_length;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_string_length);
    if( log_string_length > 0 )
    {
        // Print the shader error message
        char* error_message = (char*)malloc(log_string_length);
        glGetShaderInfoLog(shader_id, log_string_length, NULL, error_message);
        fprintf(stderr, "Shader error: %s\n", error_message);
        free(error_message);
    }
    return false;
}

bool Shader::link()
{
    printf("Linking shader program\n");

    glLinkProgram(this->shader_id);

    // Check the program
    GLint linking_status = GL_FALSE;
    glGetProgramiv(this->shader_id, GL_LINK_STATUS, &linking_status);
    glDeleteShader(this->vertex_shader_id);
    glDeleteShader(this->fragment_shader_id);
    if( linking_status == GL_TRUE )
    {
        // All is good!
        return true;
    }
    // TODO
    /* glGetProgramiv(this->shader_id, GL_INFO_LOG_LENGTH, &InfoLogLength); */
    /* if (InfoLogLength > 0) */
    /* { */
    /*     std::vector<char> ProgramErrorMessage(InfoLogLength + 1); */
    /*     glGetProgramInfoLog(this->shader_id, InfoLogLength, NULL, &ProgramErrorMessage[0]); */
    /*     printf("%s\n", &ProgramErrorMessage[0]); */
    /* } */

    return false;
}
