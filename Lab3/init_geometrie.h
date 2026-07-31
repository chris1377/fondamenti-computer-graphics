#pragma once
#include "Lib.h"
#include "Strutture.h"

void crea_cubo(Mesh* mesh);
void crea_piano(Mesh* mesh, vec4 colore);
void crea_piano_suddiviso(Mesh* mesh,vec4 colore);
void crea_sfera(Mesh* mesh, vec4 colore);
void crea_cilindro(Mesh* mesh, vec4 colore);
void crea_toro(Mesh* mesh, vec4 colore);
void crea_cono(Mesh* mesh, vec4 colore);

void add_obj(const char* name,int shaderType, vec3 translateVec, vec3 scaleVec, float ancolo, vec3 rotation_axis, vector<MeshObj>& Model3D);
void normalizeModel(vector<MeshObj>& Model3D);