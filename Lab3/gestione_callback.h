#pragma once
#include "lib.h"
#include "strutture.h"

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

 vec3 getTrackBallPoint(float x, float y);
 vec3 get_ray_from_mouse(float mouse_x, float mouse_y);
 
 