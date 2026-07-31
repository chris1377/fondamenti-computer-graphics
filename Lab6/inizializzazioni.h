#pragma once

/*
#pragma once //: È una direttiva che indica al compilatore di includere il file una sola volta per unità di compilazione.
//  Il meccanismo esatto varia a seconda del compilatore, ma in generale si basa su un meccanismo interno per tenere traccia dei file già inclusi.
//Non è parte dello standard C++, quindi la sua disponibilità e il comportamento preciso possono variare tra diversi compilatori. Tuttavia, è supportata dalla maggior parte dei compilatori moderni.
*/

void INIT_VAO(Mesh* fig);
void INIT_CAMERA_PROJECTION();
void INIT_lighting();
int  INIT_Cubemap(string SkyboxDir);
void  INIT_texture(vector<std::string> path_texture, vector<int>* texture);
void init(vector<std::string> path_texture, vector<int>* texture, int* cubemapTexture);
