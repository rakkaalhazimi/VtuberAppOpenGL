#include<iomanip>
#include<iostream>
#include<fstream>
#include<sstream>

#include<glad/glad.h>
#include<glfw/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Mesh.h"
#include "RayCaster.h"
#include "Selector.h"
#include "Texture.h"
#include "TextRenderer.h"



const unsigned int width = 800;
const unsigned int height = 800;

// position | normal | color | texture
Vertex vertices[] = 
{
  // Upper left
  Vertex{ glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
  // Upper right
  Vertex{ glm::vec3(0.5f, 0.5, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(2.0f, 1.0f) },
  // Lower left
  Vertex{ glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
  // Lower right
  Vertex{ glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.5f, 0.5f), glm::vec2(2.0f, 0.0f) },
};


GLuint indices[] = {
  0, 1, 2,
  1, 2, 3,
};



int main() {
  // Initialize GLFW
  glfwInit();
  
  // Tell GLFW what version of OpenGL we are using
  // In this case, we are using OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Tell GLFW we are using the CORE profile
  // So that means we only have the modern functions
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  // Create a GLFWwindow object of 800 by 800 pixels, naming it YoutubeOpenGL
  GLFWwindow *window = glfwCreateWindow(width, height, "Vtuber Application", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
  }
  
  // Introduce the windows into the current context
  glfwMakeContextCurrent(window);
  
  // Vsync ON
  glfwSwapInterval(1);
  
  // Load GLAD so it configures OpenGL
  gladLoadGL();
  
  // Specify the viewport of OpenGL in the Window
  // In this case the viewport goes from x=0, y=0, to x=800, y=800
  glViewport(0, 0, width, height);
  
  
  Shader shader("assets/shaders/default.vert", "assets/shaders/default.frag");
  Shader textShader("assets/shaders/text.vert", "assets/shaders/text.frag");
  Shader rayShader("assets/shaders/default.vert", "assets/shaders/ray.frag");
  
  std::vector<Vertex> vert(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
  std::vector<GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
  
  // Mesh
  Mesh mesh(vert, ind);
  Mesh mesh2(vert, ind);
  mesh2.translation.x = 1.5f;
  std::vector<Mesh*> meshes = {&mesh, &mesh2};
  
  // Texture
  Texture myTexture("assets/images/brs.png", 0, GL_RGBA, GL_UNSIGNED_BYTE);
  myTexture.texUnit(shader, "myTexture");
  
  // Raycaster
  RayCaster rayCaster;
  
  
  // UV Scrolling
  const float targetFPS = 60.0f;
  const float scrollSpeed = 0.2f;  // How fast the texture scrolls (units per second)
  const float targetFrameTime = 1.0f / targetFPS;  // Time per frame (1 second / target FPS)

  float lastTime = glfwGetTime();
  float elapsedTime = 0.0f;
  float uOffset = 0.0f;
  
  // Camera
  Camera camera(width, height, glm::vec3(0.0f, 0.0f, -2.0f));
  
  glEnable(GL_DEPTH_TEST);
  // Specify the color of the background
  glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
  // Clean the back buffer and assign the new color to it
  glClear(GL_COLOR_BUFFER_BIT);
  // Swap the back buffer with the front buffer
  glfwSwapBuffers(window);
  
  
  // Freetype
  TextRenderer textRender("assets/fonts/ARIAL.TTF");
  bool locked = false;
  
  glm::mat4 model = glm::mat4(1.0f);
  
  
  // Selector
  Selector selector;
  
  
  // Main while loop
  while (!glfwWindowShouldClose(window))
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    
    // UV Scrolling
    elapsedTime += deltaTime;
    if (elapsedTime >= targetFrameTime)
    {
      shader.Activate();
      uOffset += 0.01f;
      GLuint uOffsetLoc = glGetUniformLocation(shader.ID, "uOffset");
      glUniform1f(uOffsetLoc, uOffset);
      
      elapsedTime = 0.0f;
    }
    
    // Camera
    camera.Inputs(window);
    camera.updateMatrix(45.0f, 0.1f, 100.0f, shader, "camMatrix");
    
    shader.Activate();
    myTexture.Bind();
    selector.Watch(window, rayCaster, meshes);
    mesh.Draw(shader);
    mesh2.Draw(shader);
    
    // Ray Casting
    rayShader.Activate();
    camera.updateMatrix(45.0f, 0.1f, 100.0f, rayShader, "camMatrix");
    rayCaster.Activate(window, rayShader, camera);
    rayCaster.DrawLine();
    const bool hit = rayCaster.Intersect(shader, mesh);
    const bool pressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    const bool unlocked = glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS;
    
    // Get mouse coordinate
	  double xpos, ypos;
    int winWidth, winHeight;
    glfwGetWindowSize(window, &winWidth, &winHeight);
	  glfwGetCursorPos(window, &xpos, &ypos);
    float x = (2.0f * xpos) / winWidth - 1.0f;
    float y = 1.0f - (2.0f * ypos) / winHeight;
    glm::vec2 ndc = glm::vec2(x, y);
    
    std::stringstream mouseLog;
    mouseLog 
      << "X: " << std::setprecision(3) << ndc.x
      << " "
      << "Y: " << std::setprecision(3) << ndc.y
      << " "
      // << "Z: " << std::setprecision(3) << rayCaster.rayDirection.z
      << " "
      << "Inter: " << hit;
      // << "width: " << winWidth << " height: " << winHeight;
    textRender.type(textShader, mouseLog.str(), 400.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
    
    glfwSwapBuffers(window);
    
    // Take care of all GLFW events
    glfwPollEvents();
  }
  
  shader.Delete();
  textShader.Delete();
  rayShader.Delete();
  mesh.Delete();
  myTexture.Delete();
  glfwDestroyWindow(window);
  glfwTerminate();
  
  return 0;
}