/*  LAB 06
gestione interattiva di una scena 3D con oggetti mesh, illuminazione e texture

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
***********************************************************************/
#include "lib.h"
#include "ShaderMaker.h"
#include "strutture.h" 
#include "inizializzazioni.h"
#include "init_geometrie.h"
#include "gestione_callback.h"
#include "gestione_telecamera.h"
#include "utilities.h"
#include "gestione_textures.h"
#include "c:/Windows/dependencies_GL_GLFW/ImGui/imgui.h"
#include "c:/Windows/dependencies_GL_GLFW/ImGui/imgui_impl_glfw.h"
#include "c:/Windows/dependencies_GL_GLFW/ImGui/imgui_impl_opengl3.h"
#include "Gui.h"
#include <math.h>

unsigned int programId, programId_sky, programIdr, programIdR, programIdN;
// Id shaders: 
// programId_sky gestisce cubemap per environment
// programId     gestisce tutti gli altri oggetti in scena
// programIdr    gestisce oggetto riflesso 
// programIdR    gestisce oggetto rifratto 
// programIdN    gestisce oggetto con normal mapping

GLFWwindow* window;

float clear_color[3] = { 0.4f,0.4f,0.4f };
GLsizei height = 1200, width = 1200;

//Gestione Telecamera e Prospettiva
mat4                Projection, View;
ViewSetup           SetupTelecamera;
PerspectiveSetup    SetupProspettiva;

//geometria della scena
Mesh sky;
vector<Mesh> Scena;
vector<vector<MeshObj>> ScenaObj; 
// ScenaObj è una variabile che contiene una collezione di vettori 
// dove ogni vettore contiene oggetti di tipo MeshObj

//Luci,materiali, texture e shaders
point_light light;
vector<MaterialObj> materials;
vector<Shader> shaders;
vector<string> path_texture;
vector <int>    texture;

// variabili uniform per le matrici di modellazione, di vista, posizione della telecamera e scelta_shader da utilizzare.
Uniform uniform;
//Uniform per la gestione dell'illuminazione
LightShaderUniform light_unif = {};

//Interfaccia
bool flagWf = false;
bool flagAncora = false;

// Larghezza ed altezza della finestra dopo 
// l'aggiornamento da parte dell'utente
float w_up = width, h_up = height; 

//Per la gestione dell'animazione
float frame = 0.0;
float cameraSpeed = 0.05f;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

int selected_obj = -1;  //Variabile che contiene l'indice dell'oggetto selezionato nella scena

void drawScene(float currentFrame, Uniform uniform, LightShaderUniform light_unif, vector<int>texture, int cubemapTexture)
{
    int i;

    glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Imposto la matrice di proiezione 
    Projection = perspective(radians(SetupProspettiva.fovY), SetupProspettiva.aspect, SetupProspettiva.near_plane, SetupProspettiva.far_plane);

    //Costruisco la matrice di Vista che trasforma vertici da WCS a VCS
    View = lookAt(vec3(SetupTelecamera.position), vec3(SetupTelecamera.target), vec3(SetupTelecamera.upVector));

    // Disegno Sky box
    // 
    // Disabilita il test di profondità. 
    // This way the skybox will always be drawn at the background of all the other objects.
    glDepthMask(GL_FALSE);
    glUseProgram(programId_sky);
    glUniform1i(glGetUniformLocation(programId_sky, "skybox"), 0);
    glUniformMatrix4fv(uniform.MatProjS, 1, GL_FALSE, value_ptr(Projection));
    glUniformMatrix4fv(uniform.MatViewS, 1, GL_FALSE, value_ptr(View));
    // skybox cube
    glBindVertexArray(sky.VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    //TOBE UNCOMMENTED to see the environment on the cube
    glDrawElements(GL_TRIANGLES, sky.indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //Riabilita il test di profondità per il rendering degli altri oggetti della scena.
    glDepthMask(GL_TRUE);
    //
    //
    if (Scena[0].sceltaShader == REFLECTION)   //sphere
    {
    // Draw the object Scena[0].Model in REFLECTION mode with the environment mapped onto it
    // TOBE UNCOMMENTED when shaders for reflection are implemented
    glUseProgram(programIdr);
    glUniformMatrix4fv(uniform.MatProjR, 1, GL_FALSE, value_ptr(Projection));
    glUniformMatrix4fv(uniform.MatModelR, 1, GL_FALSE, value_ptr(Scena[0].Model));
    glUniformMatrix4fv(uniform.MatViewR, 1, GL_FALSE, value_ptr(View));
    glUniform3f(uniform.loc_view_posR, SetupTelecamera.position.x, SetupTelecamera.position.y, SetupTelecamera.position.z);
    glBindVertexArray(Scena[0].VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawElements(GL_TRIANGLES, (Scena[0].indices.size() - 1) * sizeof(GLuint), GL_UNSIGNED_INT, 0);
    glBindVertexArray(Scena[0].VAO);
    
    }
    if (Scena[1].sceltaShader == REFRACTION)  // cube
    {
    // Draw the object Scena[1].Model in REFRACTION mode with the environment through it
    // TOBE UNCOMMENTED when shaders for refraction are implemented
    glUseProgram(programIdR);
    glUniformMatrix4fv(uniform.MatProjR, 1, GL_FALSE, value_ptr(Projection));
    glUniformMatrix4fv(uniform.MatModelR, 1, GL_FALSE, value_ptr(Scena[1].Model));
    glUniformMatrix4fv(uniform.MatViewR, 1, GL_FALSE, value_ptr(View));
    glUniform3f(uniform.loc_view_posR, SetupTelecamera.position.x, SetupTelecamera.position.y, SetupTelecamera.position.z);
    glBindVertexArray(Scena[1].VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawElements(GL_TRIANGLES, (Scena[1].indices.size() - 1) * sizeof(GLuint), GL_UNSIGNED_INT, 0);
    
    }
    // Draw the rest of the scene
    glUseProgram(programId);
    glUniformMatrix4fv(uniform.MatView, 1, GL_FALSE, value_ptr(View));
    glUniformMatrix4fv(uniform.MatProj, 1, GL_FALSE, value_ptr(Projection));
    glUniform1f(uniform.loc_time, currentFrame);
    glUniform3f(light_unif.light_position_pointer, light.position.x, light.position.y, light.position.z);
    glUniform3f(light_unif.light_color_pointer, light.color.r, light.color.g, light.color.b);
    glUniform1f(light_unif.light_power_pointer, light.power);
    //Passo allo shader il puntatore alla posizione della camera
    glUniform3f(uniform.loc_view_pos, SetupTelecamera.position.x, SetupTelecamera.position.y, SetupTelecamera.position.z);
    
    //flagWf: se true viene richiesta la visualizzazione wireframe
    if (flagWf == true)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for (i = 0; i < Scena.size(); i++)
    {
        if ((Scena[i].sceltaShader != REFLECTION) && (Scena[0].sceltaShader != REFRACTION))
        {
            glUniformMatrix4fv(uniform.MatModel, 1, GL_FALSE, value_ptr(Scena[i].Model));
            glBindVertexArray(Scena[i].VAO);

            update_ancora(&Scena[i]);
            updateBB(&Scena[i]);
            if (checkCollisionCameraMesh(SetupTelecamera, Scena[i])) {
                cout << "Collisione " << endl;
                SetupTelecamera.position = glm::vec3(0.0, 0.5, 30.0);
            }
            ///La funzione glDrawElements è uno strumento fondamentale in OpenGL per rendere primitive grafiche (come triangoli, linee o punti) 
            // utilizzando dati di vertici memorizzati in buffer di vertici (VBO) e indicizzati da un buffer di elementi (EBO).
            //Utilizzando gli indici, possiamo ottimizzare il processo di rendering e creare geometrie complesse con meno dati di vertici.
            //GL_TRIANGLES: Questo parametro specifica il tipo di primitiva da rendere.In questo caso, stiamo rendendo dei triangoli.
            //(cubo.indices.size() - 1) * sizeof(GLuint) : Questo calcola il numero di indici da disegnare.Il termine cubo.indices.size() indica il numero totale di indici, ma sottraiamo 1 perché gli indici sono spesso memorizzati a coppie o triple per definire i triangoli.Moltiplicando per sizeof(GLuint) otteniamo la dimensione in byte.
            //GL_UNSIGNED_INT : Questo specifica il tipo di dato degli indici nel buffer di elementi.In questo caso, sono interi senza segno.
            //0 : Questo è l'offset in byte all'interno del buffer di elementi.In questo caso, iniziamo dall'inizio.
            //Utilizzando gli indici, possiamo ottimizzare il processo di rendering e creare geometrie complesse con meno dati di vertici

            glUniform1i(uniform.loc_sceltaShader, Scena[i].sceltaShader);

            glUniform3fv(light_unif.material_ambient, 1, glm::value_ptr(materials[Scena[i].material].ambient));
            glUniform3fv(light_unif.material_diffuse, 1, glm::value_ptr(materials[Scena[i].material].diffuse));
            glUniform3fv(light_unif.material_specular, 1, glm::value_ptr(materials[Scena[i].material].specular));
            glUniform1f(light_unif.material_shininess, materials[Scena[i].material].shininess);
            glUniformMatrix4fv(uniform.MatModel, 1, GL_FALSE, value_ptr(Scena[i].Model));
            glBindVertexArray(Scena[i].VAO);

            if (i == Piano)
            {
                glUniform1i(uniform.loc_texture1, 0);
                glBindTexture(GL_TEXTURE_2D, texture[mare]);
            }
            else if (i == Drappo)
            {
                glUniform1i(uniform.loc_texture1, 0);
                //glBindTexture(GL_TEXTURE_2D, texture[bandiera_ita]);
                // TOBE UNCOMMENTED to apply procedural texture mapping instead
                glBindTexture(GL_TEXTURE_2D, texture[chess]);
            }
            else if (i == Asta)
            {
                glUniform1i(uniform.loc_texture1, 0);
                glBindTexture(GL_TEXTURE_2D, texture[cemento]);
            }
            else if (i == Toro)
            {
                glUniform1i(uniform.loc_texture1, 0);
                glBindTexture(GL_TEXTURE_2D, texture[donuts]);
            }
            else
            {
                glUniform1i(uniform.loc_texture1, 0);
                glBindTexture(GL_TEXTURE_2D, texture[cemento]);
            }

            glDrawElements(GL_TRIANGLES, (Scena[i].indices.size() - 1) * sizeof(GLuint), GL_UNSIGNED_INT, 0);
            if (flagAncora)
            {
                glPointSize(15.0);
                int ind = Scena[i].indices.size() - 1;
                glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, BUFFER_OFFSET(ind * sizeof(GLuint)));
            }
            glBindVertexArray(Scena[i].VAO);
        }
    }

    //Rendering della ScenaObj
    for (i = 0; i < ScenaObj.size(); i++) {
        for (int k = 0; k < ScenaObj[i].size(); k++)
        {
            glUniformMatrix4fv(uniform.MatModel, 1, GL_FALSE, value_ptr(ScenaObj[i][k].Model));
            glUniform1i(uniform.loc_sceltaShader, ScenaObj[i][k].sceltaShader);

            //Passo allo shader il puntatore ai materiali
            glUniform3fv(light_unif.material_ambient, 1, value_ptr(ScenaObj[i][k].materiale.ambient));
            glUniform3fv(light_unif.material_diffuse, 1, value_ptr(ScenaObj[i][k].materiale.diffuse));
            glUniform3fv(light_unif.material_specular, 1, value_ptr(ScenaObj[i][k].materiale.specular));
            glUniform1f(light_unif.material_shininess, ScenaObj[i][k].materiale.shininess);
            glBindVertexArray(ScenaObj[i][k].VAO);
           
            if (i == 2 && k == 0)  //sharprockfree.obj
            {
                glActiveTexture(GL_TEXTURE0);
                glUniform1i(uniform.loc_texture1, 0);
                glBindTexture(GL_TEXTURE_2D, texture[roccia]);
                //glActiveTexture(GL_TEXTURE1);
                //glUniform1i(uniform.loc_texture2, 1); 
                //glBindTexture(GL_TEXTURE_2D, texture[roccia_n]);
            }
            glDrawElements(GL_TRIANGLES, (ScenaObj[i][k].indices.size()) * sizeof(GLuint), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }
}

void init_shader(void)
{
    GLenum ErrorCheckValue = glGetError();

    char* vertexShader = (char*)"vertexShaderM.glsl";
    char* fragmentShader = (char*)"fragmentShaderM.glsl";
    programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
    glUseProgram(programId);

    // Crea un program shader per la cubemap
    vertexShader = (char*)"vertexShader_cubemap.glsl";
    fragmentShader = (char*)"fragmentShader_cubemap.glsl";
    programId_sky = ShaderMaker::createProgram(vertexShader, fragmentShader);
    
    
    // Crea un program shader per creare effetto riflessione
    vertexShader = (char*)"v_reflection.glsl";
    fragmentShader = (char*)"f_reflection.glsl";
    programIdr = ShaderMaker::createProgram(vertexShader, fragmentShader);
    
    
    // Crea un program shader per creare effetto rifrazione
    vertexShader = (char*)"v_refraction.glsl";
    fragmentShader = (char*)"f_refraction.glsl";
    programIdR = ShaderMaker::createProgram(vertexShader, fragmentShader);
    
    
    // Crea un program shader per creare effetto normal mapping
    vertexShader = (char*)"v_normal_map.glsl";
    fragmentShader = (char*)"f_normal_map.glsl";
    programIdN = ShaderMaker::createProgram(vertexShader, fragmentShader);
    
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
    GLFWwindow* window = glfwCreateWindow(width, height, "LAB_6_TEXTURED SCENE", NULL, NULL);
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
    int cubemapTexture;

    // Initialize OpenGL and create a window
    GLFWwindow* window = initOpenGL();

    //Inizializza Shader, illuminazione, materiali, texture, cubemap, geometria e carica modelli obj
    init_shader();
    /* Get the pointers to the uniform variables for shaders */
    getUniform(); 
    init(path_texture,&texture, &cubemapTexture);
    
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
    init_GUI(window);

    GLint maxTextureUnits;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    std::cout << "Numero massimo di texture unit: " << maxTextureUnits << std::endl;
    
    /* Loop until the user closes the window */
    glViewport(0.0, 0.0, width, height);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        my_interface(window);   //in Gui.cpp

        drawScene(currentFrame, uniform, light_unif, texture, cubemapTexture);

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