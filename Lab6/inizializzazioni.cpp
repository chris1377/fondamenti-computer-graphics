#pragma once
#include "lib.h"
#include "ShaderMaker.h"
#include "strutture.h"
#include "init_geometrie.h"
#include "utilities.h"
#include "gestione_textures.h"

extern ViewSetup SetupTelecamera;
extern PerspectiveSetup SetupProspettiva;
extern int width, height;
extern string SkyboxDir;
extern vector<Mesh> Scena;
extern vector<vector<MeshObj>> ScenaObj;

extern point_light light;
extern vector<MaterialObj> materials;
extern vector<Shader> shaders;
extern Mesh sky;

string Meshdir      = "Meshes/";
string Imagedir     = "Textures/";
string SkyboxDir    = "Textures/SkyBoxes/field/";
 
//MATERIALI DI BASE
glm::vec3 red_plastic_ambient = { 0.1, 0.0, 0.0 }, red_plastic_diffuse = { 0.6, 0.1, 0.1 }, red_plastic_specular = { 0.7, 0.6, 0.6 }; GLfloat red_plastic_shininess = 150.0f;
glm::vec3 brass_ambient = { 0.5, 0.06, 0.015 }, brass_diffuse = { 0.78, 0.57, 0.11 }, brass_specular = { 0.99, 0.91, 0.81 }; GLfloat brass_shininess = 27.8f;
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

    //Genero , rendo attivo, riempio il VBO delle texture
    glGenBuffers(1, &mesh->VBO_coord_texture);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO_coord_texture);
    glBufferData(GL_ARRAY_BUFFER, mesh->texCoords.size() * sizeof(vec2), mesh->texCoords.data(), GL_STATIC_DRAW);
    //Adesso carico il VBO delle texture nel layer 3
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(3);

    //EBO di tipo indici
    glGenBuffers(1, &mesh->EBO_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(GLuint), mesh->indices.data(), GL_STATIC_DRAW);
}

void INIT_CAMERA_PROJECTION(void)
{
    //Imposto la telecamera
    SetupTelecamera.position = glm::vec3(0.0, 2.5, 30.0 );
    SetupTelecamera.target = glm::vec3(-10.0, 0.0, -5.0 );
    SetupTelecamera.direction = SetupTelecamera.target - SetupTelecamera.position;
    SetupTelecamera.upVector = glm::vec3(0.0, 1.0, 0.0 );

    //Imposto la proiezione prospettica
    SetupProspettiva.aspect = (GLfloat)width / (GLfloat)height;
    SetupProspettiva.fovY = 45.0f;
    SetupProspettiva.far_plane = 2000.0f;
    SetupProspettiva.near_plane = 0.1f;
}

void INIT_lighting()
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

    materials[MaterialType::PINK].name = "ROSA";
    materials[MaterialType::PINK].ambient = pink_ambient;
    materials[MaterialType::PINK].diffuse = pink_diffuse;
    materials[MaterialType::PINK].specular = pink_specular;
    materials[MaterialType::PINK].shininess = pink_shininess;

    materials[MaterialType::BROWN].name = "MARRONE";
    materials[MaterialType::BROWN].ambient = brown_ambient;
    materials[MaterialType::BROWN].diffuse = brown_diffuse;
    materials[MaterialType::BROWN].specular = brown_specular;
    materials[MaterialType::BROWN].shininess = brown_shininess;
    materials[MaterialType::NO_MATERIAL].name = "NO_MATERIAL";
    materials[MaterialType::NO_MATERIAL].ambient = glm::vec3(1, 1, 1);
    materials[MaterialType::NO_MATERIAL].diffuse = glm::vec3(0, 0, 0);
    materials[MaterialType::NO_MATERIAL].specular = glm::vec3(0, 0, 0);
    materials[MaterialType::NO_MATERIAL].shininess = 1.f;

    //Setup degli shader
    shaders.resize(10);
    shaders[ShaderOption::NONE].value = 0;
    shaders[ShaderOption::NONE].name = "NONE";
    shaders[ShaderOption::GOURAUD_SHADING].value = 1;
    shaders[ShaderOption::GOURAUD_SHADING].name = "GOURAUD SHADING";
    shaders[ShaderOption::TEX_PHONG_SHADING].value = 2;
    shaders[ShaderOption::TEX_PHONG_SHADING].name = "TEX PHONG SHADING";
    shaders[ShaderOption::PHONG_SHADING].value = 3;
    shaders[ShaderOption::PHONG_SHADING].name = "PHONG SHADING";
    shaders[ShaderOption::ONLY_TEXTURE].value = 4;
    shaders[ShaderOption::ONLY_TEXTURE].name = "ONLY TEXTURE";
    shaders[ShaderOption::WAVE].value = 5;
    shaders[ShaderOption::WAVE].name = "WAVE";
    shaders[ShaderOption::FLAG].value = 6;
    shaders[ShaderOption::FLAG].name = "FLAG";
    shaders[ShaderOption::NORMAL_MAPPING].value = 7;
    shaders[ShaderOption::NORMAL_MAPPING].name = "NORMAL_MAPPING";
    shaders[ShaderOption::REFLECTION].value = 8;
    shaders[ShaderOption::REFLECTION].name = "REFLECTION";
    shaders[ShaderOption::REFRACTION].value = 9;
    shaders[ShaderOption::REFRACTION].name = "REFRACTION";
}

void INIT_texture(vector<std::string> path_texture,vector<int>* texture)
{
    int i;

    string path_texture_name = Imagedir + "grey-cement.jpg";
    path_texture.push_back(path_texture_name); // Aggiungiamo la stringa al vettore
    path_texture_name = Imagedir + "texture_mare.jpg";
    path_texture.push_back(path_texture_name);
    path_texture_name = Imagedir + "bandiera_italiana.jpg";
    path_texture.push_back(path_texture_name);
    path_texture_name = Imagedir + "brickwall.jpg";
    path_texture.push_back(path_texture_name);
    path_texture_name = Imagedir + "donuts_2.jpg"; // "sugar_color.jpg";
    path_texture.push_back(path_texture_name);
    path_texture_name = Imagedir + "sharprockfree.jpg";
    path_texture.push_back(path_texture_name);
    path_texture_name = Imagedir + "sharprockfree_normal.jpg";
    path_texture.push_back(path_texture_name);

    for (i = 0; i < path_texture.size(); i++)
        texture->push_back(loadTexture(path_texture[i].c_str(), 0));

    texture->push_back(loadMyTexture_proc(path_texture[i].c_str()));
}

int INIT_Cubemap(string SkyboxDir)
{
    int cubemapTexture;
    vector<std::string> faces
    {
        /*"right.jpg",
            "left.jpg",
            "top.jpg",
            "bottom.jpg",
            "front.jpg",
            "back.jpg"*/
        SkyboxDir + "posx.jpg",
        SkyboxDir + "negx.jpg",
        SkyboxDir + "posy.jpg",
        SkyboxDir + "negy.jpg",
        SkyboxDir + "posz.jpg",
        SkyboxDir + "negz.jpg"
    };
    return cubemapTexture =loadCubemap(faces, 0);
}

void init(vector<std::string> path_texture, vector<int>*texture, int* cubemapTexture)
{
    int i;
    Mesh cubo = {}, piano = {}, cono = {}, sfera = {}, toro = {}, asta = {}, drappo = {};
    vector<MeshObj> Model3D;

    INIT_CAMERA_PROJECTION();
    INIT_lighting();
    INIT_texture(path_texture,texture);
    
    // Create Skybox cubemap
    *cubemapTexture = INIT_Cubemap(SkyboxDir);
    crea_cubo(&sky);
    INIT_VAO(&sky);

    /************************** Load objects in SCENE *********************************/
    //
    // SFERA : inizializza oggetto, la sua matrice di modellazione, il suo materiale ed il tipo di shader
    crea_sfera(&sfera, vec4(0.6, 1.0, 0.5, 1.0));
    sfera.Model = mat4(1.0);
    sfera.Model = translate(sfera.Model, vec3(3.5, 5.5, 2.5));
    sfera.Model = scale(sfera.Model, vec3(1.5, 1.5, 1.5));
    sfera.material = MaterialType::SNOW_WHITE;
    sfera.nome = "Sfera";
    sfera.sceltaShader = 8;// 8 REFLECTION;// 1 GOURAUD_SHADING;
    //Genera il VAO della sfera
    INIT_VAO(&sfera);
    Scena.push_back(sfera);

    // CUBO : inizializza oggetto, la sua matrice di modellazione, il suo materiale ed il tipo di shader
    crea_cubo(&cubo);
    cubo.Model = mat4(1.0);
    cubo.Model = translate(cubo.Model, vec3(3.5, 2.0, 2.5));
    cubo.Model = scale(cubo.Model, vec3(2.0f, 2.0f, 2.0f));
    cubo.Model = rotate(cubo.Model, radians(10.0f), vec3(0.0, 1.0, 0.0));
    cubo.nome = "Cubo";
    cubo.sceltaShader = 9; //9 REFRACTION; // 1 GOURAUD_SHADING;
    cubo.material = MaterialType::EMERALD;
    //Genera il VAO del cubo
    INIT_VAO(&cubo);
    Scena.push_back(cubo);

    // PIANO : inizializza oggetto, la sua matrice di modellazione, il suo materiale ed il tipo di shader
    crea_piano_suddiviso(&piano, vec4(0.0, 0.2, 1.0, 1.0));
    piano.Model = mat4(1.0);
    piano.Model = rotate(piano.Model, radians(1.5f), vec3(1.0, 0.0, 0.0));
    piano.Model = translate(piano.Model, vec3(0.5, 0.0, 0.5));
    piano.Model = scale(piano.Model, vec3(300.0f, 1.0f, 300.0f));
    piano.Model = translate(piano.Model, vec3(-0.5, 0.0, -0.5));
    piano.nome = "Piano";
    piano.sceltaShader = WAVE;
    piano.material = MaterialType::BROWN;
    //Genera il VAO del piano
    INIT_VAO(&piano);
    //Scena.push_back(piano); 

    // TORO : inizializza oggetto, la sua matrice di modellazione, il suo materiale ed il tipo di shader    
    crea_toro(&toro, vec4(0.0, 0.0, 1.0, 0.8));
    toro.Model = mat4(1.0);
    toro.Model = translate(toro.Model, vec3(-8.0, -0.2, 18.5));
    toro.Model = rotate(toro.Model, radians(0.0f), vec3(1.0, 0.0, 0.0));
    toro.Model = scale(toro.Model, vec3(1.0, 0.8, 1.0));
    //Genera il VAO del toro
    toro.nome = "Toro";
    toro.sceltaShader = TEX_PHONG_SHADING;// 0 NO LIGHT NO TEXTURE; 4 ONLY_TEXTURE; // PHONG_SHADING; // TEX_PHONG_SHADING;
    INIT_VAO(&toro);
    Scena.push_back(toro);

    // CILINDRO : inizializza oggetto, la sua matrice di modellazione, il suo materiale ed il tipo di shader    
    crea_cilindro(&asta,vec4(1.0,0.0,0.0,1.0));
    asta.Model = mat4(1.0);
    asta.Model = translate(asta.Model, vec3(-4.0, 2.0, 12.0));
    asta.Model = scale(asta.Model, vec3(0.1, 8.0, 0.1));
    //Genera il VAO dell'asta
    asta.nome = "Asta";
    asta.sceltaShader = GOURAUD_SHADING;
    INIT_VAO(&asta);
    Scena.push_back(asta);

    // DRAPPO BANDIERA: inizializza oggetto, la sua matrice di modellazione, il suo materiale ed il tipo di shader    
    crea_piano_suddiviso(&drappo, vec4(1.0, 1.0, 0.0, 1.0));
    drappo.Model = mat4(1.0);
    //drappo.Model = translate(drappo.Model, vec3(8.0, 10.0, 0.0));
    drappo.Model = translate(drappo.Model, vec3(-2.5, 8.5, 12.0));
    drappo.Model = rotate(drappo.Model, radians(90.0f), vec3(1.0, 0.0, 0.0));
    drappo.Model = translate(drappo.Model, vec3(0.5, 0.0, 0.5));
    drappo.Model = scale(drappo.Model, vec3(4.0f, 1.0f, 2.0f));
    drappo.Model = translate(drappo.Model, vec3(-0.5,0.0, -0.5));
    drappo.nome = "Drappo";
    drappo.sceltaShader = FLAG;
    drappo.material = MaterialType::BROWN;
    //Genera il VAO del piano
    INIT_VAO(&drappo);
    Scena.push_back(drappo);
        
    //carica modelli memorizzati in file obj e li aggiunge alla struttura ScenaObj
    add_obj("piper_pa18.obj", GOURAUD_SHADING, vec3(-18.0, 14.0, 0.0), vec3(6.0, 6.0, 6.0), 180.0f, vec3(0.0, 1.0, 0.0), Model3D);
    ScenaObj.push_back(Model3D);
    clear_objModel(&Model3D);

    add_obj("dog.obj", GOURAUD_SHADING, vec3(-4.0, 0.0, 18.0), vec3(2.0, 2.0, 2.0), 0.0f, vec3(0.0, 1.0, 0.0), Model3D);
    ScenaObj.push_back(Model3D);
    clear_objModel(&Model3D);

    //add_obj("sharprockfree.obj", GOURAUD_SHADING, vec3(-4.0, 2.0, 12.0), vec3(4.0, 4.0, 4.0), 0.0f, vec3(0.0, 1.0, 0.0), Model3D);
    add_obj("sharprockfree.obj", TEX_PHONG_SHADING, vec3(-4.0, 2.0, 12.0), vec3(4.0, 4.0, 4.0), 0.0f, vec3(0.0, 1.0, 0.0), Model3D);
    ScenaObj.push_back(Model3D);
    clear_objModel(&Model3D);
    
 }