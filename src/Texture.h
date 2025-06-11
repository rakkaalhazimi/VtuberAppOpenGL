#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H


#include<glad/glad.h>
#include<stb/stb_image.h>

#include "shader.h"

class Texture
{
  public:
    GLuint ID;
    GLenum type;
    GLuint unit;
    
    Texture(const char* image, GLenum slot);
    void texUnit(Shader& shader, const char* uniform);
    void Bind();
    void Unbind();
    void Delete();
};
#endif