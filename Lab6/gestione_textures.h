#pragma once

unsigned int loadTexture(char const* path, int flip);
unsigned int loadTexture_proc(char const* path);
unsigned int loadMyTexture_proc(char const* path);
unsigned int loadCubemap(vector<std::string> faces, int flip);