#ifndef RAY_CASTER_H_CLASS
#define RAY_CASTER_H_CLASS


#include<glad/glad.h>
#include<glfw/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "shader.h"


class RayCaster
{
  public:
    GLuint VAO;
    GLuint VBO;
    glm::vec3 rayOrigin;
    glm::vec3 rayDirection;
    glm::vec3 vertices[2];
    
    RayCaster();
    void DrawLine(GLFWwindow* window, Shader& shader, Camera& camera);
    void Intersect();
};


#endif
