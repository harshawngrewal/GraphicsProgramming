#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "stdio.h"

void framebuffer_size_callback(GLFWwindow* window, int new_width, int new_height)
{
  glViewport(0, 0, new_width, new_height);
}

void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, 1);
  }
}

int main()
{
  //INSTATIATE A GLFW WINDOWS for opengl 4.6.  
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow * window = glfwCreateWindow(800,600, "Testing Hello Triangle", NULL, NULL);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //callback set which will resize window
  if(window == NULL)
  {
    printf("Failed to create GLFW windows\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window); //will display the windows on monitor

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      fprintf(stderr, "Failed to initialize GLAD\n");
  }

  //render loop
  while(!glfwWindowShouldClose(window))
  {
    processInput(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwPollEvents();
    glfwSwapBuffers(window); //swaps the back buffer with front buffer(double buffer technique??)
  }

  glfwTerminate(); //cleanup allo glfw's resources including the opengl context that was allocated
  return 0;
}