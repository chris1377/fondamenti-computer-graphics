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

typedef enum {
    NONE,
    GOURAUD_SHADING,
    BLINNPHONG_SHADING,
    PHONG_SHADING,
    NO_TEXTURE,
    WAVE,
    TOON,
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
    vector<vec3> vertices;   //  coordinate dei vertici della mesh
    vector<vec4> colors;     //  colori associati a ciascun vertice
    vector<GLuint> indices;  //  indici dei vertici che definiscono i triangoli (o altre primitive)
    vector<vec3> normals;    //  vettori normali ai vertici (utili per il calcolo dell'illuminazione)
    vector<vec2> texCoords;  //  coordinate delle texture per ciascun vertice
    mat4 Model;  // Matrice di modellazione per applicare trasformazioni alla mesh
     
    // Indice del vertex shader da utilizzare
    int sceltaVS;
    // Indice del fragment shader da utilizzare
    int sceltaFS;
    int sceltaShader;
    GLuint VAO;                 // VAO: id. dell'oggetto che contiene info sulla mesh
    GLuint VBO_vertices;        // Vertex Buffer Object per le coordinate dei vertici
    GLuint VBO_colors;          // Vertex Buffer Object per i colori dei vertici
    GLuint VBO_normals;         // Vertex Buffer Object per le normali dei vertici
    GLuint EBO_indices;         // Element Buffer Object per gli indici dei vertici
    GLuint VBO_coord_texture;   // Vertex Buffer Object per le coordinate delle texture
    string nome;                // Nome della mesh (utile per l'identificazione)
    ShaderOption shading;
    MaterialType material;
    vec4 ancora_obj;     // Punto di ancoraggio della mesh nello spazio locale dell'oggetto
    vec4 ancora_world;   // Punto di ancoraggio della mesh nello spazio del mondo

    vec4 min_BB_obj;          // coord. minima Bounding Box nello spazio oggetto
    vec4 max_BB_obj;          // coord. massima Bounding Box nello spazio oggetto
    vec4 min_BB;              // coord. minima Bounding Box nello spazio mondo
    vec4 max_BB;              // coord. massima Bounding Box nello spazio mondo
} ;


struct  MeshObj : Mesh {
    MaterialObj materiale;
};

typedef struct {
    string name;
    int value;
} Shader;

typedef struct {
    glm::vec3 position;
    glm::vec3 color;
    GLfloat   power;
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
    GLuint MatProjS;
    GLuint MatViewS;
    GLuint loc_view_posR;
} Uniform;

//Gestione Vista
typedef struct {
    vec3 position;      // Posizione della camera nello spazio WCS 3D
    vec3 target;        // Punto verso cui la camera è puntata
    vec3 upVector;      // Vettore direzione "up" della camera
    vec3 direction;     // Vettore direzione di visione della camera
} ViewSetup;

//Gestione Proiezione
typedef struct {
    float fovY;         // Campo visivo verticale in gradi
    float aspect;       // Rapporto tra larghezza e altezza del viewport
    float near_plane;   // Distanza del piano di clipping vicino
    float far_plane;    // Distanza del piano di clipping lontano
} PerspectiveSetup;