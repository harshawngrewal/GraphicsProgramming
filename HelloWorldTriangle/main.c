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
    //vertices for simple triangle
  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };

  //This shader is dynamically compiled at run-time from its source code
  //gl_position is the output of the vertex shader, it a 3d coordinate but also has a perspective division parameter
  // This shader configure the position vertex attribute(location = 0)
  const char *vertexShaderSource = "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
      "}\0";
  
  unsigned vertexShader;
  unsigned int VBO; // vertex buffer object. 
  unsigned int VAO; // vertex array object
  int shaderCompileStatus = -1;
  
  //First step is compiling vertex shader
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderCompileStatus);

  if(!shaderCompileStatus)
  {
    printf("Failed to compile shader\n");
  }

  //second step is compiling the fragment shader
  // colour red
  const char* fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
  "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\n";
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shaderCompileStatus);
  
  if(!shaderCompileStatus)
  {
    printf("Failed to compile fragment shader\n");
  }

  //Last step is linking the two shaders together for final shader
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &shaderCompileStatus);
  
  if(!shaderCompileStatus)
  {
    printf("Failed to compile final shader\n");
  }

  //delete the individual shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader); 
  
  glGenVertexArrays(1, &VAO);  
  glBindVertexArray(VAO);
  glGenBuffers(1, &VBO);
  
  //here we are binding the VBO to the GL_ARRAY_BUFFER target, so any calls to the buffer type will be for VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO); 
  //copy vertex data into the buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Here we are describing the format of the buffer of vertices to OpenGL. 
  // based off this, OpenGL will run the shader with the proper input
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);  // the attribute of the vertex we are configuring is 'position'


  //render loop
  while(!glfwWindowShouldClose(window))
  {
    processInput(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //specify the shader(fragment and vertex) to use
    //bind the VAO which points to the vertex attribute calls and vertex object to pass into shader
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwPollEvents();
    glfwSwapBuffers(window); //swaps the back buffer with front buffer(double buffer technique??), to actually display to the buffer to screen
  }

  glfwTerminate(); //cleanup allo glfw's resources including the opengl context that was allocated
  return 0;
}