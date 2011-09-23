#include <iostream>
#include <GL/glfw.h>
#include <stdlib.h>
#include <stdio.h>


//screen initials
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

//colors
const float RED [] = {1, 0, 0};
const float GREEN [] = {0, 1, 0};
const float BLUE [] = {0, 0, 1};
const float BLACK [] = {0, 0, 0};
const float WHITE [] = {1, 1, 1};
const float BROWN[] ={0.5, 0.3, 0.2};

const int FRACTAL_LEVEL_MIN = 0;
const int FRACTAL_LEVEL_MAX = 6;
int fractal_level = FRACTAL_LEVEL_MAX;

typedef struct {
  float xyz[3];
} point3f;

const int SHAPE_POINTS_N = 8;
const point3f SHAPE_POINTS[SHAPE_POINTS_N] = {
//big square
  {0.0, 0.0, 0.0},
  {0.0, 0.8, 0.0},
  {1.0, 0.8, 0.0},
  {1.0, 0.0, 0.0},
//small square
  {0.8, 0.8, 0.0},
  {0.8, 1.0, 0.0},
  {1.0, 1.0, 0.0},
  {1.0, 0.8, 0.0}
};

bool fractal_color = false;

int running = GL_TRUE;

void inc_fractal_level()
{
  fractal_level++;
  if (fractal_level > FRACTAL_LEVEL_MAX)
    fractal_level = FRACTAL_LEVEL_MAX;
}

void dec_fractal_level()
{
  fractal_level--;
  if (fractal_level < FRACTAL_LEVEL_MIN)
    fractal_level = FRACTAL_LEVEL_MIN;
}

void GLFWCALL keyhandler(int key, int action)
{
  if(action != GLFW_PRESS)
    return;

  switch(key) {
  case GLFW_KEY_ESC:
    running = GL_FALSE;
    break;
  case GLFW_KEY_LEFT:
    dec_fractal_level();
    break;
  case GLFW_KEY_RIGHT:
    inc_fractal_level();
    break;
  case (int)('C'):
    fractal_color = not fractal_color;
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

bool init_all()
{
  if (!glfwInit()) return false;
  if (!glfwOpenWindow(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
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

void draw_fractal(int n, bool colorable)
{
  if (n == FRACTAL_LEVEL_MIN) {
    glBegin(GL_QUADS);
    for (int i = 0; i < SHAPE_POINTS_N; i++)
      glVertex3fv(SHAPE_POINTS[i].xyz);
    glEnd();
  } else {

    glPushMatrix();
    glPushMatrix();
    glPushMatrix();

    if (colorable)
      glColor3fv(BLUE);
    
    glScalef(0.5, 0.5, 1.0);
    glTranslatef(2, 1, 0);

    glRotatef(90, 0, 0, 1);
    glRotatef(180, 0, 1, 0);

    draw_fractal(n - 1, false);

    glPopMatrix();

    if (colorable)
      glColor3fv(GREEN);
    

    glScalef(0.5, 0.5, 1.0);
    glTranslatef(1, 2, 0);

    glRotatef(180, 0, 1, 0);
    glRotatef(180, 0, 0, 1);

    draw_fractal(n - 1, false);


    glPopMatrix();


    if (colorable)
      glColor3fv(RED);
  

    glScalef(0.5, 0.5, 1.0);
    glTranslatef(0, 1, 0);

    draw_fractal(n - 1, false);

    glPopMatrix();


   if (colorable)
      glColor3fv(BROWN);

    glScalef(0.25, 0.25, 1);
    glTranslatef(2, 1, 0);
    glRotatef(90, 0, 0, 1);

    draw_fractal(n - 1, false);
}
}

int main(int argc, char** argv)
{
  if (!init_all())
    exit(EXIT_FAILURE);

  setup();

  char title[50];

  while (running) {
    sprintf(title, "Fractal level: <- %d -> . 'C' - toggle colors.", fractal_level);
    glfwSetWindowTitle(title);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(0, 0, 0);

    draw_fractal(fractal_level, fractal_color);

    glfwSwapBuffers();

    running &= glfwGetWindowParam(GLFW_OPENED);
  }

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
