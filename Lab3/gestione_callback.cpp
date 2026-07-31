#pragma once
#include "lib.h"
#include "strutture.h"
#include "gestione_callback.h"
#include "gestione_telecamera.h"
#include "c:/Windows/dependencies_GL_GLFW/ImGui/imgui.h"
#include "utilities.h"

extern GLFWwindow*  window;
extern int          height, width;
extern mat4         Projection;
extern mat4         View;
extern PerspectiveSetup SetupProspettiva;
extern ViewSetup    SetupTelecamera;
extern bool         flagAncora;
extern int          selected_obj;
extern vector<Mesh> Scena;
extern vector<vector<MeshObj>> ScenaObj;

string  stringa_asse, Operazione;
float   w_up = width, h_up = height;
int     last_mouse_pos_X, last_mouse_pos_Y;
float   Theta = -89.0;
float   Phi = 0.0;
bool    moving_trackball = false;
bool    isNavigationMode = false;
float   raggio_sfera = 1.0;
float   amount = 0.2;
vec3    asse;
bool selected_from_obj = false;

extern bool moveCameraOnBezier;
extern int currentPoint;

enum {
    NAVIGATION,
    CAMERA_MOVING,
    TRANSLATE,
    ROTATING,
    SCALING
} OperationMode;

void cursor_position_callback(GLFWwindow* window, double xposIn, double yposIn) {
    
    height = h_up;
    width = w_up;
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    float xoffset, yoffset;
    float alfa = 0.05; //serve ridimensionare l'offset tra due posizioni successive del mouse
    ypos = height - ypos;

    float center_x = width / 2.0f;
    float center_y = height / 2.0f;

    xoffset = xpos - center_x;
    yoffset = ypos - center_y;
    
    if (isNavigationMode && !moving_trackball)
    {
            xoffset *= alfa;
            yoffset *= alfa;
            // Aggiorna gli angoli di rotazione orizzontale(Theta) e verticale(Phi) 
            // in base agli offset della posizione (x,y) del mouse calcolati.
            // L'angolo Theta controlla la rotazione attorno all'asse y, 
            // mentre Phi controlla la rotazione attorno all'asse x.
            Theta += xoffset;    
            Phi += yoffset;   

            // Facciamo si' che l'angolo di Phi vari tra -90 e 90, evitando così di capovolgere la telecamera.
            if (Phi > 89.0f)
                Phi = 89.0f;
            if (Phi < -89.0f)
                Phi = -89.0f;

            // Calcola le coordinate x, y e z di un punto sulla sfera unitaria, 
            // utilizzando gli angoli Theta e Phi convertiti in radianti.
            // Questo punto rappresenta la direzione in cui punta la telecamera.
            vec3 front;
            front.x = cos(radians(Theta)) * cos(radians(Phi));
            front.y = sin(radians(Phi));
            front.z = sin(radians(Theta)) * cos(radians(Phi));
            
            //Normalizza il vettore front per ottenere un vettore unitario che rappresenta la nuova direzione della telecamera.
            SetupTelecamera.direction = normalize(front); //Aggiorno la direzione della telecamera
            SetupTelecamera.target = SetupTelecamera.position + SetupTelecamera.direction; //aggiorno il punto in cui guarda la telecamera
            
            //Disabilita il cursore del mouse per evitare che si muova fuori dalla finestra durante la navigazione.
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            // Imposta la nuova posizione del cursore

            //Riposiziona il cursore al centro della finestra per mantenere una navigazione fluida.
            glfwSetCursorPos(window, (int)center_x, (int)center_y);
    }
    if (moving_trackball==true && !isNavigationMode)
       {
         
        float velocity = 10.0;
        glm::vec3 destination = getTrackBallPoint(xpos, ypos);
        glm::vec3 origin = getTrackBallPoint(last_mouse_pos_X, last_mouse_pos_Y);
        float dx, dy, dz;
        dx = destination.x - origin.x;
        dy = destination.y - origin.y;
        dz = destination.z - origin.z;
        if (dx || dy || dz) {
            // rotation angle = acos( (v dot w) / (len(v) * len(w)) ) o approssimato da ||dest-orig||;
            float pi = glm::pi<float>();
            float angle = sqrt(dx * dx + dy * dy + dz * dz) * velocity;
            // rotation axis = (dest vec orig) / (len(dest vec orig))
            glm::vec3 rotation_vec = glm::cross(origin, destination);
            // calcolo del vettore direzione w = C - A
            SetupTelecamera.direction = -SetupTelecamera.target + SetupTelecamera.position;
            // rotazione del vettore direzione w 
            // determinazione della nuova posizione della camera 
            SetupTelecamera.position = SetupTelecamera.target + vec3(rotate(mat4(1.0f), radians(-angle), rotation_vec) * vec4(SetupTelecamera.direction, 0.0));
        }
       
        last_mouse_pos_X = xpos; last_mouse_pos_Y = ypos; 
    }
}

// Calcola il raggio che parte dalla posizione della camera 
// e passa attraverso il punto sullo schermo corrispondente alla posizione del mouse.
vec3 get_ray_from_mouse(float mouse_x, float mouse_y) {

    height = h_up;
    width = w_up;
    //cout << height << endl;
    //cout << width << endl;
    mouse_y = height - mouse_y;

    // Trasforma coordinate viewport del mouse [0,width], [0,height] in coordinate Windows [-1,1]  
    float ndc_x = (2.0f * mouse_x) / width - 1.0;
    float ndc_y = (2.0f * mouse_y) / height - 1.0;

    // Nello spazio NDC a z più piccola corrisponde oggetto più vicino all'osservatore, 
    // quindi si pone la z a - 1, posizionando il punto mouse sul piano NEAR del frustum.
    // Questo significa che il raggio che stiamo calcolando partirà dalla telecamera e si dirigerà 
    // verso il punto più vicino visibile sullo schermo.
    float ndc_z = -1.0f;

    // Coordinate nel NDC space 
    vec4 P_clip = vec4(ndc_x, ndc_y, ndc_z, 1.0);

    // le coordinate nel VCS si ottengono premoltiplicando per l'inversa della matrice Projection.
    vec4 ViewModelp = inverse(Projection) * P_clip;

    // le coordinate nel WCS si ottengono premoltiplicando per l'inversa della matrice di Vista 
    ViewModelp.w = 1;
    vec4 Pw = inverse(View) * ViewModelp;

    //Il vettore del raggio viene calcolato sottraendo la posizione della camera 
    // (che è in WCS) dal punto mouse Pw nel WCS
    vec3 ray_wor = normalize(vec3(Pw) - vec3(SetupTelecamera.position));

    return ray_wor;
}

// Controlla se un raggio interseca una sfera di raggio "sphere_radius", centro "sphere_centre_wor". 
// 
// In caso negativo restituisce false. False anche per le intersezioni dietro l'origine del raggio, 
// In caso positivo (true) pone intersection_distance all'intersezione più vicina.
bool ray_sphere(vec3 O, vec3 d, vec3 sphere_centre_wor, float sphere_radius, float* intersection_distance) {

    //Calcoliamo O-C
    vec3 dist_sfera = O - sphere_centre_wor;
    float b = dot(dist_sfera, d);
    float cc = dot(dist_sfera, dist_sfera) - sphere_radius * sphere_radius;

    float delta = b * b - cc;

    if (delta < 0)                      //Il raggio non interseca la sfera
        return false;
    
    //Calcolo i valori di t per cui il raggio interseca la sfera e restituisco il valore dell'intersezione 
    //più vicina all'osservatore (la t più piccola)
    if (delta > 0.0f) {                 //Il raggio interseca la sfera
        //calcola le due intersezioni
        float t_a = -b + sqrt(delta);
        float t_b = -b - sqrt(delta);
        *intersection_distance = t_b;

        //Caso di intersezioni dietro l'osservatore
        if (t_a < 0.0) {
            if (t_b < 0)
                return false;
        }
        return true;
    }
    //Caso in cui il raggio è tangente alla sfera: un'intersezione con molteplicità doppia.
    if (delta == 0) {
        float t = -b + sqrt(delta);
        if (t < 0)
            return false;
        *intersection_distance = t;
        return true;
    }

    return false;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double xpos, ypos;
    switch (button) {
      
    case GLFW_MOUSE_BUTTON_LEFT:
        if (!ImGui::IsAnyItemHovered())
        {
            if (action == GLFW_PRESS)
                moving_trackball = true;
            else
                moving_trackball = false;

            glfwGetCursorPos(window, &xpos, &ypos);
            last_mouse_pos_X = (int)xpos;
            last_mouse_pos_Y = (int)ypos;
        }
        break;

    case GLFW_MOUSE_BUTTON_MIDDLE:    //Con il tasto MMB (scroll wheel) si selezionano gli oggetti nella scena
        if (action == GLFW_PRESS)
        {
            glfwGetCursorPos(window, &xpos, &ypos);
            float xmouse = xpos;
            float ymouse = ypos;

            vec3 ray_wor = get_ray_from_mouse(xmouse, ymouse);

            selected_obj = -1;
            int selected_i = -1;
            int selected_k = -1;
            float closest_intersection = FLT_MAX;

            // Selezione tra gli oggetti di Scena
            for (int i = 0; i < Scena.size(); i++)
            {
                float t_dist = 0.0f;
                if (ray_sphere(SetupTelecamera.position, ray_wor, Scena[i].ancora_world, raggio_sfera, &t_dist))
                {
                    if (t_dist < closest_intersection)
                    {
                        selected_obj = i;
                        closest_intersection = t_dist;
                        selected_from_obj = false;
                    }
                }
            }

            // Selezione tra gli oggetti di ScenaObj
            for (int i = 0; i < ScenaObj.size(); i++)
            {
                for (int k = 0; k < ScenaObj[i].size(); k++)
                {
                    float t_dist = 0.0f;
                    if (ray_sphere(SetupTelecamera.position, ray_wor, ScenaObj[i][k].ancora_world, 4.0f, &t_dist))
                    {
                        if (t_dist < closest_intersection)
                        {
                            
                            selected_obj = i;
                            selected_k = k;
                            closest_intersection = t_dist;
                            selected_from_obj = true;
                        }
                    }
                }
            }

            
            
        }
        break;

    default:
        break;
    }
}
 
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    // Se il tasto ESCAPE è premuto, chiude la finestra
    switch (key) {
    case GLFW_KEY_ESCAPE:
        if (action == GLFW_PRESS)
            //Imposta a True il flag booleano di chiusura della finestra
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;

    case GLFW_KEY_B:
        if (action == GLFW_PRESS) {
            moveCameraOnBezier = true;
            currentPoint = 0;
        }
        break;

    case GLFW_KEY_N:
        if (action == GLFW_PRESS) {
            moveCameraOnBezier = false;
            currentPoint = 0;
        }
        break;
    case GLFW_KEY_A:     
        if (mods & GLFW_MOD_SHIFT)
            moveCameraLeft();//Se premuto insieme al tasto shift Muove la camera a sinistra
        else
            moveCameraRight();//Muove la camera a destra
        break;
    case GLFW_KEY_U:
        if (mods & GLFW_MOD_SHIFT)
            moveCameraDown();   //Se premuto insieme al tasto shift sposta la camera in basso
        else
            moveCameraUp();     //Sposta la camera in alto
        break;
    case GLFW_KEY_G:  //Si entra in modalità di operazione traslazione
        OperationMode = TRANSLATE;
        Operazione = "TRASLAZIONE";
        break;
    case GLFW_KEY_R: //Si entra in modalità di operazione rotazione
        OperationMode = ROTATING;
        Operazione = "ROTAZIONE";
        break;
    case GLFW_KEY_S: //Si entra in modalità di operazione scalatura
        OperationMode = SCALING; //Si entra in modalità di operazione scalatura
        Operazione = "SCALATURA";
        break;
    case GLFW_KEY_X:
        //Seleziona l'asse X come asse lungo cui effettuare l'operazione selezionata (tra traslazione, rotazione, scalatura)
        stringa_asse = " Asse X";
        asse = glm::vec3(1.0, 0.0, 0.0);
        break;
    case GLFW_KEY_Y:
        //Seleziona l'asse Y come asse lungo cui effettuare l'operazione selezionata (tra traslazione, rotazione, scalatura)
        stringa_asse = " Asse Y";
        asse = glm::vec3(0.0, 1.0, 0.0);
        break;
    case GLFW_KEY_Z:
        //Seleziona l'asse Z come asse lungo cui effettuare l'operazione selezionata (tra traslazione, rotazione, scalatura)
        stringa_asse = " Asse Z";  
        asse = glm::vec3(0.0, 0.0, 1.0); 
        break;
    default:
        break;
    }
    
    // I tasti -> e <-  aggiornano lo spostamento a destra o a sinistra, 
    // la rotazione in segno antiorario o in senso orario, 
    // la scalatura come amplificazione o diminuizione delle dimensioni
    switch (OperationMode) {
        //la funzione modifyModelMatrix(glm::vec3 translation_vector, glm::vec3 rotation_vector, GLfloat angle, GLfloat scale_factor) 
        // definisce la matrice di modellazione che si vuole postmoltiplicare alla matrice di modellazione dell'oggetto selezionato, per poterlo traslare, ruotare scalare.
    case TRANSLATE:
            if (key==GLFW_KEY_RIGHT)
            {  
                amount = abs(amount);
                modifyModelMatrix(asse * amount, asse, 0.0f, 1.0f);
            }        
            if (key == GLFW_KEY_LEFT )
            {
                amount = -abs(amount);
                modifyModelMatrix(asse * amount, asse, 0.0f, 1.0f);
            }
        break;
    case ROTATING:
        // SI mette a zero il vettore di traslazione (vec3(0) e ad 1 il fattore di scale
        if (key == GLFW_KEY_RIGHT) 
        {
            amount = abs(amount);
            modifyModelMatrix(glm::vec3(0), asse, amount * 2.0f, 1.0f);
        }
        if (key == GLFW_KEY_LEFT) 
        {
            amount = -abs(amount);
            modifyModelMatrix(glm::vec3(0), asse, amount * 2.0f, 1.0f);
        }
        break;

    case SCALING:
        // SI mette a zero il vettore di traslazione (vec3(0), angolo di rotazione a 0 e ad 1 il fattore di scala 1+amount.
        if (key == GLFW_KEY_RIGHT) 
        {
            amount = abs(amount);
            modifyModelMatrix(glm::vec3(0), asse, 0.0f, 1.0f + amount);
        }
        if (key == GLFW_KEY_LEFT) 
        {
            amount = -abs(amount);
            modifyModelMatrix(glm::vec3(0), asse, 0.0f, 1.0f + amount);
        }
        break;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (yoffset < 0)
        SetupProspettiva.fovY -= 1;    //Rotella del mouse indietro
    else
        SetupProspettiva.fovY += 1;  //Rotella del mouse in avanti
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
    float AspectRatio_mondo = (float)(width) / (float)(height); 
    
    if (AspectRatio_mondo > w / h)   //Se ridimensioniamo la larghezza della Viewport
    {   
        w_up = (float)w;
        h_up = w / AspectRatio_mondo;
    }
    else {  //Se ridimensioniamo la larghezza della viewport oppure se l'aspect ratio tra la finestra del mondo 
        //e la finestra sullo schermo sono uguali
        glViewport(0, 0, h * AspectRatio_mondo, h);
        w_up = h * AspectRatio_mondo;
        h_up = (float)h;
    }
    glViewport(0, 0, w_up, h_up);
}

vec3 getTrackBallPoint(float x, float y)
{
    //Dalla posizione del mouse al punto proiettato sulla semisfera con centro l'origine e raggio 1

    float Delta, tmp;
    vec3 point;
    //map to [-1;1]
    point.x = (2.0f * x - width) / width;
    point.y = (height - 2.0f * y) / height;

    //Cooordinata z del punto di coordinate (x,y,z) che si muove sulla sfera virtuale con centro (0,0,0) e raggio r=1
    tmp = pow(point.x, 2.0) - pow(point.y, 2.0);
    Delta = 1.0f - tmp;
    if (Delta > 0.0f)
        point.z = sqrt(Delta);
    else
        point.z = 0;

    return normalize(point);
}