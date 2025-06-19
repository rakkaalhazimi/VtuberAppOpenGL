#include "PMXModel.h"


PMXModel::PMXModel(PMXFile &pmxFile)
{
  // Vertices
  // Convert vertices from PMXFile -> PMXModel  
  for (PMXVertex item: pmxFile.vertices)
  {
    
    glm::ivec4 boneIndicesVec(0);
    for (size_t i = 0; i < item.boneIndices.size() && i < 4; ++i) 
    {
        boneIndicesVec[i] = item.boneIndices[i];
    }
    
    glm::vec4 boneWeightsVec(0.0f);
    for (size_t i = 0; i < item.weights.size() && i < 4; ++i) 
    {
        boneWeightsVec[i] = item.weights[i];
    }
    
    vertices.push_back(
      VertexModel
      {
        item.position,
        item.normal,
        glm::vec2(item.uv.x, 1.0f - item.uv.y),
        boneIndicesVec,
        boneWeightsVec
      }
    );
    
  }
  
  // Indices
  // CAVEAT: use the vertex index size (current size is 2 thus we use uint16_t)
  for (uint16_t item: pmxFile.indices)
  {
    indices.push_back(item);
  }
  
  // Textures
  for (int i = 0; i < pmxFile.textures.size(); i++)
  {
    textures.push_back(
      Texture{pmxFile.textures[i].c_str(), "myTexture", (GLuint)i}
    );
  }
  
  // Materials
  materials = pmxFile.materials;
  
  // OpenGL Array Buffer
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  
  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexModel), vertices.data(), GL_STATIC_DRAW);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
  
  // Position (vec3)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexModel), (void*)0);
  glEnableVertexAttribArray(0);
  
  // Normal (vec3)
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexModel), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  
  // UV (vec2)
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexModel), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  
  // Bone Indices (ivec4)
  glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexModel), (void*)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);
  
  // Bone Weights (vec4)
  glVertexAttribPointer(4, 4, GL_INT, GL_FALSE, sizeof(VertexModel), (void*)(8 * sizeof(float) + 4 * sizeof(int)));
  glEnableVertexAttribArray(4);
  
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void PMXModel::Draw(Shader &shader)
{
  shader.Activate();
  glBindVertexArray(VAO);
  
  int indexOffset = 0;
  
  for (int i = 0; i < materials.size(); i++)
  {
    int indexCount = materials[i].faceCount;
    int textureIndex = materials[i].textureIndex;
    
    textures[textureIndex].texUnit(shader, "myTexture", textureIndex);
    textures[textureIndex].Bind();
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)(indexOffset * sizeof(GLuint)));
    
    indexOffset += indexCount;
  }
}