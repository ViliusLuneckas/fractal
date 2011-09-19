#include <iostream>
#include <GL/glfw.h>
#include <stdlib.h>

int running = GL_TRUE;

void GLFWCALL keyhandler(int key, int action)
{
  if(action != GLFW_PRESS)
    return;

  switch(key)
  {
  case GLFW_KEY_ESC:
    running = GL_FALSE;
    break;
  default:
    std::cout << "Untracked key #" << (int)key << " (" << (char)key << ") has been pressed." << std::endl;
  }
}

void GLFWCALL window_resize(int w, int h)
{
  w = (w < 1) ? 1 : w;
  h = (h < 1) ? 1 : h;
  
  glViewport(0, 0, w, h);
  glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
  double min_x = -0.1, min_y = -0.1, max_x = 1.1, max_y = 1.1;

  if ((max_x - min_x)/w < (max_y - min_y)/h) {
    double scale = (max_y - min_y) * w / (h * (max_x - min_x));
    double center = (max_x + min_x) / 2;
    min_x = center - (center - min_x) * scale;
    max_x = center + (max_x - center) * scale;
  } else {
    double scale = (max_x - min_x) * h / (w * (max_y - min_y));
    double center = (max_y + min_y) / 2;
    min_y = center - (center - min_y) * scale;
    max_y = center + (max_y - center) * scale;
  }

  glOrtho(min_x, max_x, min_y, max_y, -1.0, 1.0);
}

bool init_all(){
  if (!glfwInit()) return false;
  if (!glfwOpenWindow(900, 700, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)){
    glfwTerminate();
    return false;
  }
  glfwSetKeyCallback(keyhandler);
  glfwSetWindowSizeCallback(window_resize); 
  
  return true;
}

void setup()
{
  glEnable(GL_SMOOTH);
  glClearColor(1, 1, 1, 0.5);
}

int main(int argc, char** argv)
{
  if (!init_all())
    exit(EXIT_FAILURE);
  
  setup();

  while (running) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(0, 0, 0);
  
    glfwSwapBuffers();
    
    running &= glfwGetWindowParam(GLFW_OPENED);
  }

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
