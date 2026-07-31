/*
#pragma once //: È una direttiva che indica al compilatore di includere il file una sola volta per unità di compilazione.
//  Il meccanismo esatto varia a seconda del compilatore, ma in generale si basa su un meccanismo interno per tenere traccia dei file già inclusi.
//Non è parte dello standard C++, quindi la sua disponibilità e il comportamento preciso possono variare tra diversi compilatori. Tuttavia, è supportata dalla maggior parte dei compilatori moderni.
*/
#pragma once

#include "lib.h"
#include <string>

/************** Lighting & Shading  *****************/
typedef enum {
    RED_PLASTIC,
    EMERALD,
    BRASS,
    SNOW_WHITE,
    YELLOW,
    PINK,
    BROWN,
    NO_MATERIAL
} MaterialType;

typedef enum { // used also as index, don't modify order
    NONE,
    GOURAUD_SHADING,
    TEX_PHONG_SHADING,
    PHONG_SHADING,
    ONLY_TEXTURE,
    WAVE,
    FLAG,
    NORMAL_MAPPING,
    REFLECTION,
    REFRACTION,
} ShaderOption;
/**********************************************************/

typedef struct {
    string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    GLfloat shininess;
}MaterialObj;

 

struct Mesh {
    vector<vec3> vertices;   // Coordinate dei vertici della mesh
    vector<vec4> colors;     // Colori associati a ciascun vertice
    vector<GLuint> indices;  // Indici dei vertici che definiscono i triangoli (o altre primitive)
    vector<vec3> normals;    // Vettori normali ai vertici (utili per il calcolo dell'illuminazione)
    vector<vec3> tangents;   // Vettori Tangenti
    vector<vec2> texCoords;  // Coordinate texture per ciascun vertice
    mat4 Model;  // Matrice di modellazione per applicare trasformazioni alla mesh
     
    // Indice del vertex shader da utilizzare
    int sceltaVS;
    // Indice del fragment shader da utilizzare
    int sceltaFS;
    int sceltaShader;
    GLuint VAO;             // Vertex Array Object: identificatore dell'oggetto che contiene tutte le informazioni sulla mesh
    GLuint VBO_vertices;    // Vertex Buffer Object per le coordinate dei vertici
    GLuint VBO_colors;      // Vertex Buffer Object per i colori dei vertici
    GLuint VBO_normals;     // Vertex Buffer Object per le normali dei vertici
    GLuint EBO_indices;     // Element Buffer Object per gli indici dei vertici
    GLuint VBO_coord_texture;   // Vertex Buffer Object per le coordinate delle texture
    string nome;            // Nome della mesh (utile per l'identificazione)
    ShaderOption shading;
    MaterialType material;
    vec4 ancora_obj;        // Punto di ancoraggio della mesh nello spazio locale dell'oggetto
    vec4 ancora_world;      // Punto di ancoraggio della mesh nello spazio del mondo

    vec4 min_BB_obj;          
    vec4 max_BB_obj;          
    vec4 min_BB;              
    vec4 max_BB;              
} ;

struct  MeshObj : Mesh {
    MaterialObj materiale;
};

typedef struct {
    string name;
    int value;
} Shader;


enum ElementiDiScena
{
    Sfera = 0,
    cubo = 1,
    Piano = 20,
    Toro = 2,
    Asta = 3,
    Drappo = 4,
    Cono = 5,
};
enum TipiTexture
{
    cemento = 0,
    mare = 1,
    bandiera_ita = 2,
    mattoni=3,
    donuts =4,
    roccia =5,
    roccia_n = 6,
    chess = 7,  // texture procedurale

};

typedef struct {
    glm::vec3 position;
    glm::vec3 color;
    GLfloat power;
} point_light;

typedef struct {
	GLuint light_position_pointer;
	GLuint light_color_pointer;
	GLuint light_power_pointer;
	GLuint material_diffuse;
	GLuint material_ambient;
	GLuint material_specular;
	GLuint material_shininess;
} LightShaderUniform;

typedef struct
{ 
    GLuint MatModel;
    GLuint MatView;
    GLuint MatProj;
    GLuint loc_view_pos;
    GLuint loc_time;
    GLuint loc_sceltaShader;
    GLuint loc_texture1;
    GLuint loc_texture2;
    GLuint loc_skybox;
    GLuint MatProjS;
    GLuint MatViewS;
    GLuint MatModelR;
    GLuint MatProjR;
    GLuint MatViewR;
    GLuint loc_view_posR;
    GLuint loc_cubemapR;
} Uniform;

//Gestione Vista
typedef struct {
    vec3 position; // Posizione della camera nello spazio 3D
    vec3 target; // Punto verso cui la camera è puntata
    vec3 upVector; // Vettore che indica la direzione "up" della camera
    vec3 direction; // Vettore che indica la direzione di visione della camera
} ViewSetup;

//gestione proiezione
typedef struct {
    float fovY; // Campo visivo verticale in gradi
    float aspect; // Rapporto tra larghezza e altezza del viewport
    float near_plane; // Distanza del piano di clipping vicino
    float far_plane; // Distanza del piano di clipping lontano
} PerspectiveSetup;

