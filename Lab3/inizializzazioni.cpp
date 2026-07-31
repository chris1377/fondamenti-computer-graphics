#pragma once
#include "lib.h"
#include "ShaderMaker.h"
#include "strutture.h"
#include "init_geometrie.h"
#include "utilities.h"

extern ViewSetup        SetupTelecamera;
extern PerspectiveSetup SetupProspettiva;
extern int              width, height;

extern vector<Mesh> Scena;
extern vector<vector<MeshObj>> ScenaObj;

extern point_light light;
extern vector<MaterialObj> materials;
extern vector<Shader> shaders;

//MATERIALI DI BASE
glm::vec3 red_plastic_ambient = { 0.1, 0.0, 0.0 }, red_plastic_diffuse = { 0.6, 0.1, 0.1 }, red_plastic_specular = { 0.7, 0.6, 0.6 }; GLfloat red_plastic_shininess = 150.0f;
glm::vec3 brass_ambient = { 0.5, 0.06, 0.015 }, brass_diffuse = { 0.78, 0.57, 0.11 }, brass_specular = { 0.99, 0.91, 0.81 }; GLfloat brass_shininess = 57.8f;
glm::vec3 emerald_ambient = { 0.0215, 0.04745, 0.0215 }, emerald_diffuse = { 0.07568, 0.61424, 0.07568 }, emerald_specular = { 0.633, 0.727811, 0.633 }; GLfloat emerald_shininess = 78.8f;
glm::vec3 snow_white_ambient = { 0.2, 0.2, 0.2 }, snow_white_diffuse = { 0.95, 0.96, 0.98 }, snow_white_specular{ 0.8, 0.8, 0.8 }; GLfloat snow_white_shininess = 1.78125f;
glm::vec3 yellow_ambient = { 0.8,	0.8,	0.0 }, yellow_diffuse = { 1.0,1.0,0.6 }, yellow_specular{ 0.9,	0.9	,0.04 }; GLfloat yellow_shininess = 1.78125f;
glm::vec3 pink_ambient = { 0.05f,0.0f,0.0f }, pink_diffuse = { 0.5f,0.4f,0.4f }, pink_specular{ 0.7f,0.04f,0.04f }; GLfloat pink_shininess = 1.78125f;
glm::vec3 brown_ambient = { 0.19125f, 0.0735f, 0.0225f }, brown_diffuse = { 0.7038f, 0.27048f, 0.0828f }, brown_specular{ 0.256777f, 0.137622f, 0.086014f }; GLfloat brown_shininess = 12.8f;

void INIT_VAO(Mesh* mesh)
{

    glGenVertexArrays(1, &mesh->VAO);
    glBindVertexArray(mesh->VAO);
    //Genero , rendo attivo, riempio il VBO della geometria dei vertici
    glGenBuffers(1, &mesh->VBO_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO_vertices);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(vec3), mesh->vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    //Genero , rendo attivo, riempio il VBO dei colori
    glGenBuffers(1, &mesh->VBO_colors);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO_colors);
    glBufferData(GL_ARRAY_BUFFER, mesh->colors.size() * sizeof(vec4), mesh->colors.data(), GL_STATIC_DRAW);
    //Adesso carico il VBO dei colori nel layer 2
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

	//Genero , rendo attivo, riempio il VBO delle normali
	glGenBuffers(1, &mesh->VBO_normals);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, mesh->normals.size() * sizeof(vec3), mesh->normals.data(), GL_STATIC_DRAW);
	//Adesso carico il VBO delle NORMALI nel layer 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);

    //EBO di tipo indici
    glGenBuffers(1, &mesh->EBO_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(GLuint), mesh->indices.data(), GL_STATIC_DRAW);
}

void INIT_CAMERA_PROJECTION(void)
{
    //Imposto la telecamera

    SetupTelecamera.position = glm::vec3(0.0, 0.5, 30.0 ); //lo cambiamo con i punti della curva di bezier
    SetupTelecamera.target = glm::vec3(0.0, 0.0, 0.0 );
    SetupTelecamera.direction = SetupTelecamera.target - SetupTelecamera.position;
    SetupTelecamera.upVector = glm::vec3(0.0, 1.0, 0.0 );

    //Imposto la proiezione prospettica

    SetupProspettiva.aspect = (GLfloat)width / (GLfloat)height;
    SetupProspettiva.fovY = 45.0f;
    SetupProspettiva.far_plane = 2000.0f;
    SetupProspettiva.near_plane = 0.1f;
}

void INIT_LIGHTING()
{
    //Setup della luce
    light.position = { 50.0, 2.0, 10.0 };
    light.color = { 1.0,1.0,1.0 };
    light.power = 2.f;

    //Setup dei materiali
    // Materials setup
    materials.resize(8);
    materials[MaterialType::RED_PLASTIC].name = "Red Plastic";
    materials[MaterialType::RED_PLASTIC].ambient = red_plastic_ambient;
    materials[MaterialType::RED_PLASTIC].diffuse = red_plastic_diffuse;
    materials[MaterialType::RED_PLASTIC].specular = red_plastic_specular;
    materials[MaterialType::RED_PLASTIC].shininess = red_plastic_shininess;

    materials[MaterialType::EMERALD].name = "Emerald";
    materials[MaterialType::EMERALD].ambient = emerald_ambient;
    materials[MaterialType::EMERALD].diffuse = emerald_diffuse;
    materials[MaterialType::EMERALD].specular = emerald_specular;
    materials[MaterialType::EMERALD].shininess = emerald_shininess;

    materials[MaterialType::BRASS].name = "Brass";
    materials[MaterialType::BRASS].ambient = brass_ambient;
    materials[MaterialType::BRASS].diffuse = brass_diffuse;
    materials[MaterialType::BRASS].specular = brass_specular;
    materials[MaterialType::BRASS].shininess = brass_shininess;

    materials[MaterialType::SNOW_WHITE].name = "Snow_White";
    materials[MaterialType::SNOW_WHITE].ambient = snow_white_ambient;
    materials[MaterialType::SNOW_WHITE].diffuse = snow_white_diffuse;
    materials[MaterialType::SNOW_WHITE].specular = snow_white_specular;
    materials[MaterialType::SNOW_WHITE].shininess = snow_white_shininess;

    materials[MaterialType::YELLOW].name = "Yellow";
    materials[MaterialType::YELLOW].ambient = yellow_ambient;
    materials[MaterialType::YELLOW].diffuse = yellow_diffuse;
    materials[MaterialType::YELLOW].specular = yellow_specular;
    materials[MaterialType::YELLOW].shininess = yellow_shininess;

    materials[MaterialType::PINK].name = "Pink";
    materials[MaterialType::PINK].ambient = pink_ambient;
    materials[MaterialType::PINK].diffuse = pink_diffuse;
    materials[MaterialType::PINK].specular = pink_specular;
    materials[MaterialType::PINK].shininess = pink_shininess;

    materials[MaterialType::BROWN].name = "Brown";
    materials[MaterialType::BROWN].ambient = brown_ambient;
    materials[MaterialType::BROWN].diffuse = brown_diffuse;
    materials[MaterialType::BROWN].specular = brown_specular;
    materials[MaterialType::BROWN].shininess = brown_shininess;
    
    materials[MaterialType::NO_MATERIAL].name = "NO_MATERIAL";
    materials[MaterialType::NO_MATERIAL].ambient = glm::vec3(1, 1, 1);
    materials[MaterialType::NO_MATERIAL].diffuse = glm::vec3(0.0, 0.0, 0.0);
    materials[MaterialType::NO_MATERIAL].specular = glm::vec3(0, 0, 0);
    materials[MaterialType::NO_MATERIAL].shininess = 1.f;

    //Setup degli shader
    shaders.resize(7);
    shaders[ShaderOption::NONE].value = 0;
    shaders[ShaderOption::NONE].name = "NONE";
    shaders[ShaderOption::GOURAUD_SHADING].value = 1;
    shaders[ShaderOption::GOURAUD_SHADING].name = "GOURAUD SHADING";
    shaders[ShaderOption::BLINNPHONG_SHADING].value = 2;
    shaders[ShaderOption::BLINNPHONG_SHADING].name = "BLINN PHONG SHADING";
    shaders[ShaderOption::PHONG_SHADING].value = 3;
    shaders[ShaderOption::PHONG_SHADING].name = "PHONG SHADING";
    shaders[ShaderOption::NO_TEXTURE].value = 4;
    shaders[ShaderOption::NO_TEXTURE].name = "NO TEXTURE";
    shaders[ShaderOption::WAVE].value = 5;
    shaders[ShaderOption::WAVE].name = "WAVE";
}

void clear_objModel(vector<MeshObj>* Model3D) {
    int i;
    for (i = 0; i < Model3D->size(); i++)
    {
        Model3D->at(i).vertices.clear();
        Model3D->at(i).colors.clear();
        Model3D->at(i).normals.clear();
        Model3D->at(i).indices.clear();
        Model3D->at(i).texCoords.clear();
    }
}

void init()
{
    int i;
    Mesh cubo = {}, piano = {}, cono = {}, sfera = {}, toro = {};
    vector<MeshObj> Model3D;

    INIT_CAMERA_PROJECTION();
    INIT_LIGHTING();
    
    /************************** Load objects in SCENE *********************************/
    //
    // SFERA : inizializza oggetto, la sua matrice di modellazione, il suo materiale ed il tipo di shader
    crea_sfera(&sfera, vec4(-5.0, 10.0, 2.5, 1.0));
    sfera.Model = mat4(1.0);
    sfera.Model = translate(sfera.Model, vec3(3.5, 5.5, 2.5));
    sfera.Model = scale(sfera.Model, vec3(0.5, 0.5, 0.5));
    sfera.nome = "Sfera";
    sfera.sceltaShader = NO_TEXTURE; 
    sfera.material = MaterialType::SNOW_WHITE;
    //Genera il VAO della sfera
    INIT_VAO(&sfera);
    Scena.push_back(sfera);

    // PIANO : inizializza oggetto, la sua matrice di modellazione, il suo materiale ed il tipo di shader
    crea_piano_suddiviso(&piano, vec4(0.0, 0.2, 1.0, 1.0));
    piano.Model = mat4(1.0);
    piano.Model = rotate(piano.Model, radians(1.5f), vec3(1.0, 0.0, 0.0));
    piano.Model = translate(piano.Model, vec3(0.5, 0.0, 0.5));
    piano.Model = scale(piano.Model, vec3(300.0f, 1.0f, 300.0f));
    piano.Model = translate(piano.Model, vec3(-0.5, 0.0, -0.5));
    piano.nome = "Piano";
    piano.sceltaShader = WAVE;// WAVE;
    piano.material = MaterialType::BROWN;
    //Genera il VAO del piano
    INIT_VAO(&piano);
    Scena.push_back(piano);

    crea_cubo(&cubo);
    cubo.Model = mat4(1.0);
    cubo.Model = translate(cubo.Model, vec3(3.5, 2.0, 2.5));
    cubo.Model = scale(cubo.Model, vec3(2.0f, 2.0f, 2.0f));
    cubo.Model = rotate(cubo.Model, radians(10.0f), vec3(0.0, 1.0, 0.0));
    cubo.nome = "Cubo";
    cubo.sceltaShader = PHONG_SHADING;
    cubo.material = MaterialType::EMERALD;
    //Genera il VAO del cubo
    INIT_VAO(&cubo);
    Scena.push_back(cubo);
    
    // TORO : inizializza oggetto, la sua matrice di modellazione, il suo materiale ed il tipo di shader    
    crea_toro(&toro, vec4(0.0, 0.0, 1.0, 0.8));
    toro.Model = mat4(1.0);
    toro.Model = translate(toro.Model, vec3(-2.0, 8.3, 2.5));
    toro.Model = rotate(toro.Model, radians(90.0f), vec3(0.0, 1.0, 0.0));
    toro.Model = scale(toro.Model, vec3(0.5, 0.5, 0.5));
    //Genera il VAO del toro
    toro.nome = "Toro";
    toro.sceltaShader = PHONG_SHADING; //PHONG_SHADING;// NO_TEXTURE; // GOURAUD_SHADING;
    toro.material = MaterialType::BRASS;
    INIT_VAO(&toro);
    Scena.push_back(toro);

    //carica modelli memorizzati in file obj e li aggiunge alla struttura ScenaObj

    //scansione 3d
    add_obj("fortnite.obj", NO_TEXTURE, vec3(-7.0, 10.0, 2.5), vec3(2.0, 2.0, 2.0), -90.0f, vec3(0.0, 1.0, 0.0), Model3D);
    ScenaObj.push_back(Model3D);
    clear_objModel(&Model3D);

    add_obj("cup.obj", NO_TEXTURE, vec3(1.0, 9.1, 2.5), vec3(1.0, 1.0, 1.0), -90.0f, vec3(0.0, 1.0, 0.0), Model3D);
    ScenaObj.push_back(Model3D);
    clear_objModel(&Model3D);
    
    add_obj("pot.obj", TOON, vec3(5, 10, 4), vec3(2.0, 2.0, 2.0), -90.0f, vec3(0.0, 1.0, 0.0), Model3D);
    ScenaObj.push_back(Model3D);
    clear_objModel(&Model3D);
    
    add_obj("table.obj", NO_TEXTURE, vec3(-2.0, 10.0, 2.5), vec3(8.0,10.0, 8.0), 0.0f, vec3(0.0, 1.0, 0.0), Model3D);
    ScenaObj.push_back(Model3D);
    clear_objModel(&Model3D);

 }