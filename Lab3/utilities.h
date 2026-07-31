#pragma once
#include "lib.h"
#include "strutture.h"
void modifyModelMatrix(vec3 translation_vector, vec3 rotation_vector, GLfloat angle, GLfloat scale_factor);
void update_ancora(Mesh* mesh);
void normalizeModel(std::vector<MeshObj>& Model3D);
void findBB(Mesh* mesh);
void updateBB(Mesh* mesh);
bool checkCollisionCameraMesh(ViewSetup camera, Mesh mesh);
