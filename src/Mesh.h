#ifndef MESH_CLASS_H
#define MESH_CLASS_H


#include<vector>

#include<glad/glad.h>
#include<glm/glm.hpp>

#include "shader.h"


struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;
};


class Mesh
{
    public:
      GLuint VAO, VBO, EBO;
      
      std::vector<glm::vec3> positions;
      std::vector<GLuint> indices;
      
      Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
      void Draw(Shader& shader);
      void Delete();
    
};

#endif