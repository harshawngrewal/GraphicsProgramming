#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "stdio.h"
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

enum shape
{
  triangle,
  triangle2,
  rectangle,
  wire_rectangle
};

// we use the same array for all shapes, just 
float vertices[] = {
    0.5f,  0.5f, 0.0f,  // top right
    0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
      -0.5f,   0.0f, 0.0f,    // middle left
    0.5f,   0.0f, 0.0f,    // middle right
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
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";
const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n";


void framebuffer_size_callback(GLFWwindow* window, int new_width, int new_height)
{
  glViewport(0, 0, new_width/3, new_height);
}

void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, 1);
  }
}

void detectTermination(GLFWwindow *window)
{
  while(1)
  {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
      glfwSetWindowShouldClose(window, 1);
      return;
    }
  }
}

int fpsOverlay()
{
  unsigned int VBO; // vertex buffer object.
  unsigned int VAO; // vertext array object.
  unsigned int EBO; // index buffer object. 
  unsigned int vertexShader;
  unsigned int fragmentShader;
  int shaderCompileStatus;
  unsigned int shaderProgram;

  unsigned int indices[] = {  // note that we start from 0!
      0,1,2,3,4,5
  };

  //TODO: this will handle the render logic for the fps overlay
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow * window = glfwCreateWindow(800,600, "FPS OVERLAY", NULL, NULL);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //callback set which will resize window
  
  if(window == NULL)
  {
    printf("Failed to create GLFW windows\n");
    glfwDestroyWindow(window);
    return -1;
  }

  glfwMakeContextCurrent(window); //will display the windows on monitor


  // why is  this causing performance issues???
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

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);


  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shaderCompileStatus);
  
  if(!shaderCompileStatus)
  {
    printf("Failed to compile fragment shader\n");
  }

  //Last step is linking the two shaders together for final shader
  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &shaderCompileStatus);
  
  if(!shaderCompileStatus)
  {
    printf("Failed to compile final shader\n");
  }

  glUseProgram(shaderProgram);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //want to bind the EBO specifying indices for rectangle
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  while(!glfwWindowShouldClose(window))
  {
    processInput(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(  GL_LINE_LOOP, 0, 4);

    glfwPollEvents();
    glfwSwapBuffers(window); //swaps the back buffer with front buffer(double buffer technique??), to actually display to the buffer to screen
  }

  glfwDestroyWindow(window);
  return 0;
}

int main()
{
  //INSTATIATE A GLFW WINDOWS for opengl 4.6.  
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow * window = glfwCreateWindow(800,600, "Testing Hello Triangle", NULL, NULL);
  pthread_t thread1;
  int iret1 = pthread_create(&thread1, NULL, (void *)(void *)fpsOverlay, NULL);

  glfwSetWindowTitle(window, "FPS: ?? ");
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

  unsigned int indices[] = {  // note that we start from 0!
      0, 1, 3,   // first triangle
      1, 2, 3    // second triangle
  };
  unsigned int indices_triangle[] = {  // note that we start from 0!
      0, 1, 3,   // first triangle
  }; 

  unsigned int indices_triangle2[] = {
      4, 1, 2,   // second triangle
  }; 
  
  unsigned vertexShader;
  unsigned int VBO; // vertex buffer object. 
  unsigned int VAO; // vertex array object
  unsigned int EBO; // element buffer objects(rectangle)
  unsigned int EBO2;// element buffer objects(triangle)
  unsigned int EBO3;// element buffer objects(triangle)
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
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  //not gonna verify if shader 2 compiled correctly
  unsigned int fragmentShader2;
  fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
  glCompileShader(fragmentShader2);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shaderCompileStatus);
  
  if(!shaderCompileStatus)
  {
    printf("Failed to compile fragment shader\n");
  }

  //Last step is linking the two shaders together for final shader
  unsigned int shaderProgram = glCreateProgram();
  unsigned int shaderProgram2 = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glAttachShader(shaderProgram2, vertexShader);
  glAttachShader(shaderProgram2, fragmentShader2);
  glLinkProgram(shaderProgram2);

  glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &shaderCompileStatus);
  
  if(!shaderCompileStatus)
  {
    printf("Failed to compile final shader\n");
  }

  //delete the individual shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader); 
  glDeleteShader(fragmentShader2); 

  glGenVertexArrays(1, &VAO);  
  glBindVertexArray(VAO);
  
  glGenBuffers(1, &VBO);
  //here we are binding the VBO to the GL_ARRAY_BUFFER target, so any calls to the buffer type will be for VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO); 
  //copy vertex data into the buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Here we are describing the format of the buffer of vertices to OpenGL. 
  // based off this, OpenGL will run the shader with the proper input
  // the vertex data is provided with float data type and x,y,z format(vec3)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);  // the attribute of the vertex we are configuring is 'position'

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glGenBuffers(1, &EBO2);
  glGenBuffers(1, &EBO3);


  //render loop
  enum shape s = triangle;
  glfwSwapInterval(1); // enabled v-sync, so the the buffer swap happens after monitor refresh
  int frameCount = 0;
  double previousTime = glfwGetTime();

  while(!glfwWindowShouldClose(window))
  {
    processInput(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // printf("%d \n", s);
    switch(s)
    {
      case triangle:
        glUseProgram(shaderProgram);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //want to bind the EBO specifying indices for rectangle
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_triangle), indices_triangle, GL_STATIC_DRAW);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        if(frameCount % 160 > 40)
        {
          s = triangle2;
        }
        break;
      case triangle2:
        glUseProgram(shaderProgram2);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //want to bind the EBO specifying indices for rectangle
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_triangle2), indices_triangle2, GL_STATIC_DRAW);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        if(frameCount % 160 > 80)
        {
          s = rectangle;
        }
        break;
      case rectangle:
        glUseProgram(shaderProgram);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //want to bind the EBO specifying indices for rectangle
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        if(frameCount % 160 > 120)
        {
          s = wire_rectangle;
        }
        break;
      case wire_rectangle:
      default:
        glUseProgram(shaderProgram2);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //want to bind the EBO specifying indices for rectangle
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        if(frameCount % 160 == 0)
        {
          s = triangle;
        }
        break;
    }

    glfwPollEvents();
    glfwSwapBuffers(window); //swaps the back buffer with front buffer(double buffer technique??), to actually display to the buffer to screen
    frameCount ++;

    //calculate fps every second
    double currentTime = glfwGetTime();
    double delta = currentTime - previousTime;
    if (delta >= 1.0) { 
        int fps = (double)frameCount / delta;
        char buffer[33];
        memset(buffer, 33, 1);
        sprintf(buffer, "FPS: %d", fps);
        glfwSetWindowTitle(window, (const char *)(buffer));
        frameCount = 0;
        previousTime = currentTime;
    }
  }

  pthread_join(thread1, NULL);
  printf("fps overlay window exited now %d\n", iret1);

  glfwTerminate(); //cleanup allo glfw's resources including the opengl context that was allocated
  return 0;
}