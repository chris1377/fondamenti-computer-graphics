/*  LAB 03
gestione interattiva di una scena 3D con oggetti mesh, ed illuminazione.

NAVIGAZIONE
mouse drag	->rotazione trackball
move camera forward (zoom in)/backward (zoom out) FOV
move camera left/right  A/A+SHIFT
move camera up/down     U/U+SHIFT

WIN-1 e WIN-2 sono finestre di testo, sempre attive; 
POPUP-1    si attiva con click RMB; 
           navigazione (il punto di vista segue il movimento del mouse)
           interazione (si attiva la selezione di un oggetto)
POPUP-2    si attiva con click RMB+SHIFT.
           material  cambia il materiale all'oggetto selezionato
           shader    cambia lo shader all'oggetto selezionato

Selezione di un oggetto (in modalità interazione): 
    click sull'ancora dell'oggetto con mouse tasto centrale MMB 

Modifica di un oggetto:
translation     G
rotation        R
scale           S
axis selection X/Y/Z
frecce          < decrementa > incrementa

Camera che si vuove su percorso predefinito:
Start B
Stop N

***********************************************************************/

#include "lib.h"
#include "ShaderMaker.h"
#include "strutture.h" 
#include "inizializzazioni.h"
#include "init_geometrie.h"
#include "gestione_callback.h"
#include "gestione_telecamera.h"
#include "utilities.h"
#include "c:/Windows/dependencies_GL_GLFW/ImGui/imgui.h"
#include "c:/Windows/dependencies_GL_GLFW/ImGui/imgui_impl_glfw.h"
#include "c:/Windows/dependencies_GL_GLFW/ImGui/imgui_impl_opengl3.h"
#include "Gui.h"
#include <math.h>

unsigned int programId;
GLFWwindow* window;

float   clear_color[3] = { 0.4f,0.4f,0.4f };
int     height = 1600, width = 1600;

bool moveCameraOnBezier = false;
int currentPoint = 0;

//Gestione Telecamera e Prospettiva
mat4                Projection, View;
ViewSetup           SetupTelecamera;
PerspectiveSetup    SetupProspettiva;

//geometria della scena
vector<Mesh>            Scena;
vector<vector<MeshObj>> ScenaObj; 
// ScenaObj è una variabile che contiene una collezione di vettori 
// dove ogni vettore può contenere oggetti di tipo MeshObj

//Luci,materiali e shaders
point_light light;
vector<MaterialObj> materials;
vector<Shader> shaders;

// variabili uniform per le matrici di modellazione, di vista, posizione della telecamera e scelta_shader da utilizzare.
Uniform             uniform;
//Uniform per la gestione dell'illuminazione
LightShaderUniform  light_unif = {};

string Meshdir      = "Meshes/";

//Interfaccia
bool flagWf = false;
bool flagAncora = false;

int tCasteljau = 200;
#define MaxNumPts 300
float   vPositions_CP[MaxNumPts][2];        // Control Polygon
float   vPositions_C[MaxNumPts][3];         // Curve 

int selected_obj = -1;  //Variabile che contiene l'indice dell'oggetto selezionato nella scena

#define BUFFER_OFFSET(i) ((char *)NULL + (i))


void drawScene(float currentFrame, Uniform uniform, LightShaderUniform light_unif)
{
    int i, k;

    glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Imposto la matrice di proiezione 
    Projection = perspective(radians(SetupProspettiva.fovY), SetupProspettiva.aspect, SetupProspettiva.near_plane, SetupProspettiva.far_plane);

    //Costruisco la matrice di Vista che trasforma vertici da WCS a VCS
    View = lookAt(vec3(SetupTelecamera.position), vec3(SetupTelecamera.target), vec3(SetupTelecamera.upVector));

    glUseProgram(programId);

    // Communicate the uniform variable value to the shader 
    //
    // Passaggio della matrice View
    glUniformMatrix4fv(uniform.MatView, 1, GL_FALSE, value_ptr(View));
    // Passaggio della matrice Proiezione che mappa il volume di vista in coordinate NDC
    glUniformMatrix4fv(uniform.MatProj, 1, GL_FALSE, value_ptr(Projection));
    glUniform1f(uniform.loc_time, currentFrame);
    // Passaggio variabili per lighting
    glUniform3f(light_unif.light_position_pointer, light.position.x, light.position.y, light.position.z);
    glUniform3f(light_unif.light_color_pointer, light.color.r, light.color.g, light.color.b);
    glUniform1f(light_unif.light_power_pointer, light.power);
    // Passaggio posizione della camera
    glUniform3f(uniform.loc_view_pos, SetupTelecamera.position.x, SetupTelecamera.position.y, SetupTelecamera.position.z);

    //flagWf: se true viene fatta la visualizzazione wireframe
    if (flagWf == true)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    /********************* Draw scene   ***************************/
    // Scena[0] è la sfera (luce) non è resa
    for (i = 1; i < Scena.size(); i++)
    {
        glUniformMatrix4fv(uniform.MatModel, 1, GL_FALSE, value_ptr(Scena[i].Model));
        glBindVertexArray(Scena[i].VAO);

        update_ancora(&Scena[i]);
        updateBB(&Scena[i]);
        if (checkCollisionCameraMesh(SetupTelecamera, Scena[i])) {
            cout << "Collisione " << endl;
            SetupTelecamera.position = glm::vec3(0.0, 0.5, 30.0);
        }
        glUniform1i(uniform.loc_sceltaShader, Scena[i].sceltaShader);

        // Passiamo allo shader il puntatore al materiale
        glUniform3fv(light_unif.material_ambient, 1, glm::value_ptr(materials[Scena[i].material].ambient));
        glUniform3fv(light_unif.material_diffuse, 1, glm::value_ptr(materials[Scena[i].material].diffuse));
        glUniform3fv(light_unif.material_specular, 1, glm::value_ptr(materials[Scena[i].material].specular));
        glUniform1f(light_unif.material_shininess, materials[Scena[i].material].shininess);
        glUniformMatrix4fv(uniform.MatModel, 1, GL_FALSE, value_ptr(Scena[i].Model));
        glBindVertexArray(Scena[i].VAO);

        glDrawElements(GL_TRIANGLES, (Scena[i].indices.size() - 1) * sizeof(GLuint), GL_UNSIGNED_INT, 0);
        if (flagAncora)
        {
            glPointSize(15.0);
            int ind = Scena[i].indices.size() - 1;
            glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, BUFFER_OFFSET(ind * sizeof(GLuint)));
        }
        glBindVertexArray(0);
    }

    /********************* Draw ScenaObj  ***************************/
    for (i = 0; i < ScenaObj.size(); i++) {
        for (int k = 0; k < ScenaObj[i].size(); k++)
        {

            update_ancora(&ScenaObj[i][k]);

            glUniformMatrix4fv(uniform.MatModel, 1, GL_FALSE, value_ptr(ScenaObj[i][k].Model));
            glUniform1i(uniform.loc_sceltaShader, ScenaObj[i][k].sceltaShader);

            // Passiamo allo shader il puntatore al materiale
            glUniform3fv(light_unif.material_ambient, 1, value_ptr(ScenaObj[i][k].materiale.ambient));
            glUniform3fv(light_unif.material_diffuse, 1, value_ptr(ScenaObj[i][k].materiale.diffuse));
            glUniform3fv(light_unif.material_specular, 1, value_ptr(ScenaObj[i][k].materiale.specular));
            glUniform1f(light_unif.material_shininess, ScenaObj[i][k].materiale.shininess);
            
            glBindVertexArray(ScenaObj[i][k].VAO);
            glDrawElements(GL_TRIANGLES, (ScenaObj[i][k].indices.size()) * sizeof(GLuint), GL_UNSIGNED_INT, 0);

            //ANCORA
            if (flagAncora) {

                glPointSize(15.0);
                int ind = ScenaObj[i][k].indices.size() - 1;
                glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, BUFFER_OFFSET(ind * sizeof(GLuint)));

            }
            

            glBindVertexArray(0);
        }
    }
}

/* Get the pointers to the uniform variables */
void getUniform() {

    uniform.MatProj = glGetUniformLocation(programId, "Projection");
    uniform.MatModel = glGetUniformLocation(programId, "Model");
    uniform.MatView = glGetUniformLocation(programId, "View");
    uniform.loc_time = glGetUniformLocation(programId, "time");
    uniform.loc_sceltaShader = glGetUniformLocation(programId, "sceltaShader");
    uniform.loc_view_pos = glGetUniformLocation(programId, "ViewPos");

    //location delle variabili uniformi per la gestione della luce
    light_unif.light_position_pointer = glGetUniformLocation(programId, "light.position");
    light_unif.light_color_pointer = glGetUniformLocation(programId, "light.color");
    light_unif.light_power_pointer = glGetUniformLocation(programId, "light.power");

    //location delle variabili uniformi per la gestione dei materiali
    light_unif.material_ambient = glGetUniformLocation(programId, "material.ambient");
    light_unif.material_diffuse = glGetUniformLocation(programId, "material.diffuse");
    light_unif.material_specular = glGetUniformLocation(programId, "material.specular");
    light_unif.material_shininess = glGetUniformLocation(programId, "material.shininess");
}

void init_shader(void)
{
    GLenum ErrorCheckValue = glGetError();

    char* vertexShader = (char*)"vertexshaderM3.glsl";
    char* fragmentShader = (char*)"fragmentshaderM3.glsl";
    
    programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
    glUseProgram(programId);
}

void casteljau(float t, float* result, float points[][2], int NumPts) {
    float coordX[MaxNumPts], coordY[MaxNumPts];

    for (int i = 0; i < NumPts; i++) { //costruisco il temp
        coordX[i] = points[i][0];
        coordY[i] = points[i][1];
    }

    for (int i = 1; i < NumPts; i++) {
        for (int k = 0; k < NumPts - 1; k++) {
            coordX[k] = (1 - t) * coordX[k] + (t)*coordX[k + 1];
            coordY[k] = (1 - t) * coordY[k] + (t)*coordY[k + 1];
        }
    }

    result[0] = coordX[0];
    result[1] = coordY[0];

}

void initBezierCurve() {
    
    vPositions_CP[0][0] = 100.0f; vPositions_CP[0][1] = 0.0f;   // Punti circonferenza
    vPositions_CP[1][0] = 0.00f; vPositions_CP[1][1] = 100.00f; // Punti su 45°
    vPositions_CP[2][0] = -100.0f; vPositions_CP[2][1] = 0.0f;   // Punti su 90°
    vPositions_CP[3][0] = 0.00f; vPositions_CP[3][1] = -100.00f; // Punti su 135°
    vPositions_CP[4][0] = 100.0f;   vPositions_CP[4][1] = 0.0f;
  
    

    // Calcolo i punti della curva
    for (int t = 0; t < tCasteljau; t++) {
        GLfloat result[3];
        float u = static_cast<float>(t) / (tCasteljau - 1);
        casteljau(u, result, vPositions_CP, 5);
        vPositions_C[t][0] = result[0];
        vPositions_C[t][1] = 70.0f;
        vPositions_C[t][2] = result[1];
    }

}

// Function to initialize OpenGL and create a window.
GLFWwindow* initOpenGL() {

    // Initialize GLFW.
    /******* Inizializzazioni GLFW library *************/
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW." << std::endl;
        glfwTerminate();
    }

    //Imposta le proprietà del contesto e del profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //Abilita il double buffering
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    /******* Create a window and its OpenGL context *****/
    GLFWwindow* window = glfwCreateWindow(width, height, "LAB_3_SCENE", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window !" << std::endl;
        glfwTerminate();
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window); //crea il context corrente e lo associa a window. In opengl un rendering context è una macchina astati che memorizza tutte le informazioni necessarie e le risorse per il rendering grafico

    // verifica se la libreria GLAD è riuscita a caricare correttamente tutti i puntatori 
    // alle funzioni OpenGL necessarie.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to load opengl function pointers !" << std::endl;
        glfwTerminate();
    }
    return window;
}

int main(void)
{
    // Initialize OpenGL and create a window
    GLFWwindow* window = initOpenGL();

    //Inizializza Shader, illuminazione, materiali, cubemap, geometria e carica modelli obj
    init_shader();
    /* Get the pointers to the uniform variables for shaders */
    getUniform();
    init();
    initBezierCurve();

    // ************  Gestione eventi *******************
    // Funzioni di callback chiamate quando si verificano determinati eventi
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //Abilita il test di profondità
    glEnable(GL_DEPTH_TEST);
    //Per impostare la trasparenza ed il blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Inizializza IMGUI
    init_GUI(window);   //in Gui.cpp

    /* Loop until the user closes the window */
    glViewport(0.0, 0.0, width, height);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        
        my_interface(window);   //in Gui.cpp

        if (moveCameraOnBezier) {
            SetupTelecamera.position = vec3(
                vPositions_C[currentPoint][0],
                vPositions_C[currentPoint][1],
                vPositions_C[currentPoint][2]
            );

            // Guarda verso il centro della scena
            glm::vec3 lookAtTarget = vec3(-2.0f, 2.0f, 2.5f);
            SetupTelecamera.target = lookAtTarget;

            currentPoint++;
            if (currentPoint >= tCasteljau) currentPoint = 0; // loop 
        }

        drawScene(currentFrame, uniform, light_unif);

        // Renderizza i dati di disegno di ImGui
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    // Chiude il menu di interfaccia con l'utente
    close_GUI();    //in Gui.cpp
    glfwTerminate();
    return 0;
}