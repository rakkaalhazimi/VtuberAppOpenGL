#ifndef PMX_MODEL_H_HEADER_CLASS
#define PMX_MODEL_H_HEADER_CLASS


#include<glad/glad.h>

#include "PMXFile.h"
#include "shader.h"
#include "Texture.h"



struct VertexModel
{
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texUV;
  glm::ivec4 boneIndices;
  glm::vec4 boneWeights;
};

class PMXModel
{
  public:
    GLuint VAO, VBO, EBO;
    std::vector<VertexModel> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    std::vector<PMXMaterial> materials;
    std::vector<PMXBone> bones;
    
    PMXModel(PMXFile &pmxFile);
    void Draw(Shader& shader);
    
  private:
    
};

#endif