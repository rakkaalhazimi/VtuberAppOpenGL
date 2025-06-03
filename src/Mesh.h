#ifndef MESH_CLASS_H
#define MESH_CLASS_H


#include<vector>

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

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
      
      glm::mat4 model = glm::mat4(1.0f);
      
      glm::vec3 rotation = glm::vec3(0.0f);
      glm::vec3 translation = glm::vec3(0.0f);
      float scale = 1.0f;
      
      std::vector<glm::vec3> positions;
      std::vector<glm::vec3> positionsTransformed;
      std::vector<GLuint> indices;
      glm::vec3 centroid;
      
      bool isSelected = false;
      
      Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
      void Draw(Shader& shader);
      std::vector<glm::vec3> getTransformedPosition();
      void RotateZ(float degree);
      void RotateY(float degree);
      void Scale(float size);
      void Delete();
    
    private:
      void TransformPosition(glm::mat4& model);
    
};

#endif