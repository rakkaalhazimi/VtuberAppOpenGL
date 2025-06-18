#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H


#include<filesystem>
#include<vector>

#include<glad/glad.h>
#include<stb/stb_image.h>

#include "shader.h"

class Texture
{
  public:
    GLuint ID;
    const char* type;
    GLuint unit;
    
    Texture(std::string image, const char* texType, GLuint slot);
    void texUnit(Shader& shader, const char* uniform, GLuint unit);
    void Bind();
    void Unbind();
    void Delete();
};
#endif