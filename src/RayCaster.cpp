#include "RayCaster.h"


RayCaster::RayCaster()
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  
  // glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(sizeof(glm::vec3)));
  // glEnableVertexAttribArray(2);
  std::cout << "Initiate ray caster" << std::endl;

  glBindVertexArray(0);
}

void RayCaster::DrawLine(GLFWwindow* window, Shader& shader, Camera& camera)
{
  shader.Activate();
  
  // Get mouse coordinate
  double xpos, ypos;
  int winWidth, winHeight;
  glfwGetWindowSize(window, &winWidth, &winHeight);
  glfwGetCursorPos(window, &xpos, &ypos);
  
  float x = (2.0f * xpos) / winWidth - 1.0f;
  float y = 1.0f - (2.0f * ypos) / winHeight;
  glm::vec2 ndc = glm::vec2(x, y);
  
  glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);
  glm::vec4 rayEye = glm::inverse(camera.projection) * rayClip;
  rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f); // Direction in eye space
  
  // The line is pointing to our direction, that is why it seems like following the mouse.
  glm::vec3 rayWorld = glm::normalize(glm::vec3(glm::inverse(camera.view) * rayEye));
  glm::vec3 rayOrigin = camera.Position + glm::vec3(0.0f, 0.0f, -0.1f);
  // glm::vec3 rayOrigin = glm::vec3(0.0f, 0.0f, -2.0f);
  glm::vec3 rayEnd = rayOrigin + rayWorld * 100.0f;
  glm::vec3 point = rayOrigin + rayWorld + 0.01f;
  glm::vec4 mousePoint = glm::inverse(camera.projection * camera.view) * glm::vec4(ndc.x, ndc.y, -1.0f, 1.0f);
  mousePoint /= mousePoint.w;
  
  // Update vertices
  vertices[0] = glm::vec3(mousePoint.x, mousePoint.y, mousePoint.z - 0.01f);
  vertices[1] = vertices[0] + rayWorld * 100.0f;
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
  glBindVertexArray(VAO);
  glDrawArrays(GL_LINES, 0, 2);  
}