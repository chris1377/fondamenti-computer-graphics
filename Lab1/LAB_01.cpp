/*
 * Lab-01.c
 *
 *     This program draws straight lines connecting dots placed with mouse clicks.
 *
 * Usage:
 *   Left click to place a control point.
 *		Maximum number of control points allowed is currently set at 300.
 *	 Press "f" to remove the first control point
 *	 Press "l" to remove the last control point.
 *    
 *   Press "c" to change from interpolation to approximation and viceversa (Alternative a)
 * 
 *	 Press escape to exit.
 */
#include "lib.h"
#include "ShaderMaker.h"
#include "gestione_callback.h"

/********* Include GLM; libreria matematica per le opengl *******/
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int		width = 800;
int		height = 800;

///////////////////////////////////////////////////////////
// Variabili globali -------------------------------------------------------------------
static unsigned int programId;

float r = 0.0, g = 0.5, b = 1.0;

int totalCatmullPoint;

int mouseOverIndex = -1;
int tCasteljau = 100;
int numCatmullSegments, totalCurvePoints;

unsigned int    vao,   vposition_CP_ID;     // Control Polygon
unsigned int    vao_2, vposition_Curve_ID;  // Curve 

#define MaxNumPts 300
float   vPositions_CP[MaxNumPts][2];        // Control Polygon
float   vPositions_C[MaxNumPts][2];         // Curve 
float   vPositions_inter_C[30000][2];       //Catmull


float catmullPts[MaxNumPts*3][2];

bool catmull = true;

int     NumPts = 0;
int     NumPoints = 0;
//----------------------------------------------------------------------------------------
void removeFirstPoint() {
    int i;
    if (NumPts > 0) {
        // Remove the first point, slide the rest down
        NumPts--;
        for (i = 0; i < NumPts; i++) {
            vPositions_CP[i][0] = vPositions_CP[i + 1][0];
            vPositions_CP[i][1] = vPositions_CP[i + 1][1];
        }
    }
}
// Add a new point to the end of the list.  
// Remove the first point in the list if too many points.
void removeLastPoint() {
    if (NumPts > 0) {
        NumPts--;
    }
}
// Add a new point to the end of the list.  
// Remove the first point in the list if too many points.
void addNewPoint(float x, float y) {
    if (NumPts >= MaxNumPts) {
        removeFirstPoint();
    }
    vPositions_CP[NumPts][0] = x;
    vPositions_CP[NumPts][1] = y;
    printf("CP %i coords (%f, %f) \n", NumPts, vPositions_CP[NumPts][0], vPositions_CP[NumPts][1]);
    NumPts++;
}

void findPoint(float xPos, float yPos) {
    float dist;
    for (int i = 0; i < NumPts; i++) {
        dist = sqrt(pow(vPositions_CP[i][0] - xPos, 2) + pow(vPositions_CP[i][1] - yPos, 2)); //calcola distanza
        if (dist < 0.03) {
            mouseOverIndex = i;
            printf("Mouse over CP %i\n", i);
            return; //come trova la vicinanza ad un punto esce dalla ricerca
        }
        else mouseOverIndex = -1;
    }
}

void modifyPoint(float xPos, float yPos) {
    vPositions_CP[mouseOverIndex][0] = xPos;
    vPositions_CP[mouseOverIndex][1] = yPos;
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

void CatmullRomSpline() {
    
    int index = 0;

    // Punto di controllo iniziale
    catmullPts[index][0] = vPositions_CP[0][0];
    catmullPts[index][1] = vPositions_CP[0][1];
    index++;

    float m[2];
    m[0] = (vPositions_CP[1][0] - vPositions_CP[0][0]); 
    m[1] = (vPositions_CP[1][1] - vPositions_CP[0][1]);
    catmullPts[index][0] = vPositions_CP[0][0] + (m[0] / 3.0f);
    catmullPts[index][1] = vPositions_CP[0][1] + (m[1] / 3.0f);
    index++;

    for (int i = 1; i < NumPts - 1; i++) { // Evitiamo i punti estremi
        
        m[0] = (vPositions_CP[i + 1][0] - vPositions_CP[i - 1][0]) / 2.0f;
        m[1] = (vPositions_CP[i + 1][1] - vPositions_CP[i - 1][1]) / 2.0f;
        
        // Pi- 
        catmullPts[index][0] = vPositions_CP[i][0] - (m[0] / 3.0f);
        catmullPts[index][1] = vPositions_CP[i][1] - (m[1] / 3.0f);
        index++;

        // Pi
        catmullPts[index][0] = vPositions_CP[i][0];
        catmullPts[index][1] = vPositions_CP[i][1];
        index++;

        // Pi+
        catmullPts[index][0] = vPositions_CP[i][0] + (m[0] / 3.0f);
        catmullPts[index][1] = vPositions_CP[i][1] + (m[1] / 3.0f);
        index++;
    }

    // Punto di controllo finale
    m[0] = (vPositions_CP[NumPts - 1][0] - vPositions_CP[NumPts - 2][0]);
    m[1] = (vPositions_CP[NumPts - 1][1] - vPositions_CP[NumPts - 2][1]);
    catmullPts[index][0] = vPositions_CP[NumPts - 1][0] - (m[0] / 3.0f);
    catmullPts[index][1] = vPositions_CP[NumPts - 1][1] - (m[1] / 3.0f);
    index++;

    catmullPts[index][0] = vPositions_CP[NumPts - 1][0];
    catmullPts[index][1] = vPositions_CP[NumPts - 1][1];
    index++;

    totalCatmullPoint = index;
    
}

void init(void)
{
    // Create a vertex array object for CONTROL POLYGON
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    // attribute VBO : positions 
    glGenBuffers(1, &vposition_CP_ID);
    glBindBuffer(GL_ARRAY_BUFFER, vposition_CP_ID);
    
    // Create a vertex array object  for CURVE
    glGenVertexArrays(1, &vao_2);
    glBindVertexArray(vao_2);
    // attribute VBO : positions 
    glGenBuffers(1, &vposition_Curve_ID);
    glBindBuffer(GL_ARRAY_BUFFER, vposition_Curve_ID);
    
    // Background color
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glViewport(0, 0, width, height);
}
void initShader(void)
{
    GLenum ErrorCheckValue = glGetError();

    char* vertexShader = (char*)"vertexshaderC.glsl";
    char* fragmentShader = (char*)"fragmentshaderC.glsl";
    // La funzione successiva crea un programma shader completo a partire da due shader individuali: 
    // uno per la gestione dei vertici e uno per la gestione dei pixel. 
    // Il programma shader risultante viene identificato da un identificativo univoco (il programId) 
    programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
    //La funzione successiva "attiva" il programma shader associato all'identificatore programId.
    glUseProgram(programId);
}
void drawScene(void)
{
    float result[2];
    // Background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    if (NumPts > 1) {

        if (!catmull) {
            CatmullRomSpline();
            numCatmullSegments = NumPts - 1;
            totalCurvePoints = numCatmullSegments * tCasteljau;
            int index = 0; // Indice per vPositions_inter_C
            // Calcoliamo casteljau per ogni sottoinsieme di 4 punti
            for (int i = 0; i < numCatmullSegments; i++) {

                float segmentPts[4][2] = {
                    { catmullPts[i * 3][0], catmullPts[i * 3][1] },
                    { catmullPts[i * 3 + 1][0], catmullPts[i * 3 + 1][1] },
                    { catmullPts[i * 3 + 2][0], catmullPts[i * 3 + 2][1] },
                    { catmullPts[i * 3 + 3][0], catmullPts[i * 3 + 3][1] }
                };


                for (int t = 0; t < tCasteljau; t++) {
                    casteljau((float)t/tCasteljau, result, segmentPts, 4);
                    vPositions_inter_C[index][0] = result[0];
                    vPositions_inter_C[index][1] = result[1];
                    index++;
                }

            }
            glBindVertexArray(vao_2);
            glBindBuffer(GL_ARRAY_BUFFER, vposition_Curve_ID);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vPositions_inter_C), &vPositions_inter_C[0], GL_STREAM_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glLineWidth(0.5);
            glDrawArrays(GL_LINE_STRIP, 0, totalCurvePoints);
            glBindVertexArray(0);

            
            // Draw control polygon
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vposition_CP_ID);
            glBufferData(GL_ARRAY_BUFFER, sizeof(catmullPts), &catmullPts[0], GL_STREAM_DRAW); //trasferisce i dati in gpu
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // Draw the control points CP
            glPointSize(6.0);
            glDrawArrays(GL_POINTS, 0, totalCatmullPoint);
            //Draw the line segments between CP
            glLineWidth(2.0);
            glDrawArrays(GL_LINE_STRIP, 0, totalCatmullPoint);
            glBindVertexArray(0);

        }
        else {
            float result[3];

            for (int t = 0; t < tCasteljau; t++) {
                casteljau((GLfloat)t / tCasteljau, result, vPositions_CP, NumPts); 
                vPositions_C[t][0] = result[0];
                vPositions_C[t][1] = result[1];
            }
            glBindVertexArray(vao_2);
            glBindBuffer(GL_ARRAY_BUFFER, vposition_Curve_ID);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vPositions_C), &vPositions_C[0], GL_STREAM_DRAW); 
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glLineWidth(0.5);
            glDrawArrays(GL_LINE_STRIP, 0, tCasteljau);
            glBindVertexArray(0);

            // Draw control polygon
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vposition_CP_ID);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vPositions_CP), &vPositions_CP[0], GL_STREAM_DRAW); //trasferisce i dati in gpu
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // Draw the control points CP
            glPointSize(6.0);
            glDrawArrays(GL_POINTS, 0, NumPts);
            //Draw the line segments between CP
            glLineWidth(2.0);
            glDrawArrays(GL_LINE_STRIP, 0, NumPts);
            glBindVertexArray(0);

        }

    }
    else {
       
        // Draw one point
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vposition_CP_ID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vPositions_CP), &vPositions_CP[0], GL_STREAM_DRAW); //trasferisce i dati in gpu
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glPointSize(6.0);
        glDrawArrays(GL_POINTS, 0, NumPts);
        
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
    GLFWwindow* window = glfwCreateWindow(width, height, "LAB_01", NULL, NULL);
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
    // Initialize OpenGL and create a window.
    GLFWwindow* window = initOpenGL();
        
    initShader(); 
    init(); //inizializza le strutture
    
    /******* Funzioni callback chiamate quando si verificano determinati eventi ******/
    //Chiusura dell'applicazione premendo il tasto Esc  e la modifica del colore dello sfondo con space bar
    glfwSetKeyCallback(window, key_callback);
    //Visualizza le coordinate del mouse che si muove sulla finestra grafica
    glfwSetCursorPosCallback(window, cursor_position_callback);
    //Visualizza le coordinate individuate dal tasto sinistro premuto
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    //Visualizza le dimensioni della finestra se ridimensionata
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Render 
        drawScene();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(programId);
    glDeleteVertexArrays(1, &vao);
    glDeleteVertexArrays(1, &vao_2);

    glfwTerminate(); //Libera le risorse allocate da glfwInit
    return 0;
}