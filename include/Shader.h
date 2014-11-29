#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <GL/glew.h>


/**
 * Shader object.
 * This handles the loading and cleanup after an GLSL shader object.
 *
 * Usage:
 * <code>
 *  Shader s(VERTEX_PATH, SHADER_PATH);
 *  if( !s.link() )
 *  {
 *      // Failed to load.
 *      ...
 *      return 1;
 *  }
 *  GLuint program_id = s.getProgramId();
 *  ..
 * </code>
 *
 * TODO allow for loading raw shader code as std::string
 *      loadShaderCode(...)
 */
class Shader
{
    public:
        /**
         * Create an empty shader.
         * If using this constructor you will need to call setVertexShaderFile(...) and setFragmentShaderFile(...)
         * manually.
         */
        Shader();
        /**
         * Create a shader with the given vertex and shader programs.
         */
        Shader(const char* vertex_shader_path, const char* fragment_shader_path);

        /**
         * Destructor that handles all the resource cleanup.
         */
        ~Shader();

        /**
         * Set the vertex shader filepath.
         */
        bool setVertexShaderFile(const char* vertex_shader_path);
        /**
         * Set the fragment shader filepath.
         */
        bool setFragmentShaderFile(const char* fragment_shader_path);
        /**
         * Link the shader object.
         */
        bool link();
        /**
         * Get the program ID for this shader.
         * This should only be called once calling link() and getting true as the responce.
         */
        GLuint getProgramId();

    private:
        GLuint shader_id;
        GLuint vertex_shader_id;
        GLuint fragment_shader_id;

        std::string loadShaderCode(const char* file_path);
        GLuint compileShader(GLuint shader_id, std::string shader_code);
        bool verifyShader(GLuint shader_id);
};
