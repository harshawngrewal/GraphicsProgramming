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
  //INSTATIATE A GLFW WINDOWS for opengl 3.3.  
  //Looks like I am running older drivers with version 3.2
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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
  // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  // glClear(GL_COLOR_BUFFER_BIT); //before display a frame, we wan't to clear the previous one

  //render loop
  while(!glfwWindowShouldClose(window))
  {
    processInput(window);
    glfwPollEvents();
    glfwSwapBuffers(window); //swaps the back buffer with front buffer(double buffer technique??)
  }

  glfwTerminate(); //cleanup allo glfw's resources including the opengl context that was allocated
  return 0;
}