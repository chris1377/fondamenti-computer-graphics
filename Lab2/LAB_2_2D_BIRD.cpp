// LAB_2_2D_BIRD.cpp : 
 /* 
 *
 *     This program is a 2d game.
 * 
 *  Bird deve attraversare gli ostacoli che vengono generati ad altezze differenti 
 %  Bird può volare verso l’alto (tenendo premuto il tasto “w”) per poi 
 %           planare nuovamente verso il terreno. 
 %  La partita termina quando Bird colpisce un ostacolo 
 %				Bird si capovolgerà, indicando la sua morte e nella console verrà stampato il numero di punti ottenuto
 %
 * Usage:
 *	 Premi "w" per saltare
 *	 Premi "r" per ricominciare una volta perso il gioco
 */

#include "lib.h"
#include "ShaderMaker.h"
#include "gestione_callback.h"

static unsigned int programId;
static unsigned int  programIdClouds;
unsigned int programIdObstacles;
static double limitFPS = 1.0 / 60.0;

unsigned int VAO, VAO_OBSTACLES, VAO_LAWN, VAO_CHARACTER, VAO_PARTICELLE;
unsigned int VBO, VBO_OBSTACLES, VBO_LAWN, VBO_CHARACTER, VBO_PARTICELLE;
unsigned int VAO_CLOUDS, VBO_CLOUDS;
unsigned int MatProj, MatModel;

typedef struct { float x, y, r, g, b, a; } Point;

void draw_plan(float x, float y, float width, float height, vec4 color_top, vec4 color_bot, Point* plan, int firstPoint);
void draw_character(Point* character);
void draw_obstacles(Point* obstacle, float size);

// Variabili globali -------------------------------------------------------------------

// viewport size
int width = 1200;
int height = 720;

mat4 Projection;
mat4 Model; 

static float	posyStart = float(height) * 0.2;

int points = 0;
bool printpoints = false;

/*----------------------------------------*/
/*-------------OBSTACLES------------------*/
extern float	dxobstacles;
extern float	dyobstaclesdown;
extern float	dyobstaclesup;
extern int		obstacles_per_row;
extern int		frame;
extern float	SPEEDOBSTACLES;
extern int		nVertices_obstacle;
extern float	sizeObstacles;
extern Point*	obstacle;
/*---------------------------------------*/


/*-------------CHARACTER---------------------*/
extern float	posx;
extern float	posy;
extern int		vertices_character;
extern float	posxCharacter ;
extern float	posyCharacter;
extern Point*	character;
#define CHARACTERDIMENSION  50
extern float	angleBird;
extern bool		goingUp;

extern double	verticalSpeedDown;
extern double	verticalSpeed;
//
bool hit = false;
bool pressing_top = false;

/*---------------------------------------*/

//particelle
extern Point* Particelle;
extern int n_Particellari;
int p_attivi = 0;
#define PI 3.14159265

void rimuoviPoiScorri(int index) {
	int i;
	p_attivi--;
	if (index != p_attivi) {
		// Remove the first point, slide the rest down
		for (i = index; i < p_attivi; i++) {
			Particelle[i] = Particelle[i + 1];
		}
	}
}

void aggiungiParticella(vec3 posizione, vec3 direzione) {

	// Eliminiamo particelle oltre il massimo consentito
	while (p_attivi >= n_Particellari)
		rimuoviPoiScorri(0);

	int i = p_attivi;
	Particelle[i].x = (float)posizione.x;
	Particelle[i].y = (float)posizione.y;
	Particelle[i].r = 0.0f;
	Particelle[i].g = 0.0f;
	Particelle[i].b = 1.0f;
	Particelle[i].a = 1.0f;
	p_attivi++;
}

void aggiungiFlussoColorato() {
	vec3 centro = { posxCharacter, posyCharacter + posy+10.0f, 0 };
	float step = PI / 3.0f;  // Angolo tra una particella e l'altra
	float raggioIniziale = 1.2f; // Distanza iniziale dal centro
	float incrementoRaggio = 0.2f; // Di quanto si allarga la spirale 

	float angolo = 0.0f;
	for (int i = 0; i < 32; i++) {  
		float r = raggioIniziale + incrementoRaggio * i;

		vec3 posizione;
		posizione.x = centro.x + r * cos(angolo);
		posizione.y = centro.y + r * sin(angolo);
		posizione.z = 0;

		vec3 direzione;
		direzione.x = cos(angolo);
		direzione.y = sin(angolo);
		direzione.z = 0;

		aggiungiParticella(posizione, direzione);

		angolo += step;  
	}
}


void muoviParticella(int index) {
	float r = Particelle[index].r;  
	float g = Particelle[index].g; 

	float angolo = r * 2 * PI;

	float dx = -cos(angolo) * 20.0f;  // Movimento orizzontale verso sinistra
	float dy = sin(angolo) * 5.0f;   // Inclinazione

	// Aggiorna la posizione della particella
	Particelle[index].x += dx;
	Particelle[index].y += dy;
	Particelle[index].a -= 0.08f;
}



void aggiornaParticelle() {
	int i;
	for (i = 0; i < p_attivi; i++) {
		muoviParticella(i);
		if (Particelle[i].a <= 0)
			rimuoviPoiScorri(i);
	}
}




void updateObstacles(int value)
{
	if (!hit)
	{
		if (dxobstacles <= 0) {
		
			dxobstacles = width;
			dyobstaclesdown = ((rand() % 19) - 15)*10.0f;
			dyobstaclesup = dyobstaclesdown + 20 * 30.f;
			points++;
			
		} 

		frame++;
		if (frame % 8 == 0) dxobstacles -= SPEEDOBSTACLES;		
	}
}


void update(int a)
{
	
	bool moving = false;

	goingUp = false;

	if (p_attivi > 0)
		aggiornaParticelle();

	if (!hit)
	{
		if (pressing_top)
		{
			moving = true;
			verticalSpeed += 0.9; goingUp = true;
			verticalSpeedDown = 0;
		}
		if (!moving) {
			verticalSpeed = 0;

			verticalSpeedDown += 0.05;
		}
		posy = posy + verticalSpeed - verticalSpeedDown;
		
	}
}
void restart(void)
{
	printf("RESTART! \n");
	SPEEDOBSTACLES = 40;
	hit = false;
	dxobstacles = width;
	dyobstaclesdown = ((rand() % 19) - 15) * 10.0f;
	dyobstaclesup = dyobstaclesdown + 20 * 30.f;
	update(0);
	updateObstacles(0);
	posyCharacter = float(height) / 3;
	posy = posyStart;
	verticalSpeedDown = 0;
	verticalSpeed = 0;
	points = 0;
	printpoints = false;
	
}

void initShader(void)
{
	GLenum ErrorCheckValue = glGetError();

	char* vertexShader = (char*)"vertexShader_C_M.glsl";
	char* fragmentShader = (char*)"fragmentShader_C_M.glsl";
	programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
	

	char* vertexShaderClouds = (char*)"clouds_vertex_shader.glsl";
	char* fragmentShaderClouds = (char*)"cloud.glsl";
	programIdClouds = ShaderMaker::createProgram(vertexShaderClouds, fragmentShaderClouds);  // Programma per nuvole
	glUseProgram(programIdClouds);  // Usa il programma per le nuvole


	char* vertexShaderObstacles = (char*)"obstacle_vertex_shader.glsl";
	char* fragmentShaderObstacles = (char*)"obstacle_fragment_shader.glsl";
	programIdObstacles = ShaderMaker::createProgram(vertexShaderObstacles, fragmentShaderObstacles);


}

void init(void)
{
	Projection = ortho(0.0f, float(width), 0.0f, float(height));
	MatProj = glGetUniformLocation(programId, "Projection");
	MatModel = glGetUniformLocation(programId, "Model");

	// VAO e VBO per fullscreen nuvole
	float vertices_clouds[] = { // Mappo intera viewport
		-1.0f, -1.0f,
		 1.0f, -1.0f,
		-1.0f,  1.0f,
		-1.0f,  1.0f,
		 1.0f, -1.0f,
		 1.0f,  1.0f
	};

	glGenVertexArrays(1, &VAO_CLOUDS);
	glBindVertexArray(VAO_CLOUDS);
	glGenBuffers(1, &VBO_CLOUDS);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_CLOUDS);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_clouds), vertices_clouds, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	/***************************************
	* Geometry and color construction of the OBSTACLES
	****************************************/
	draw_obstacles(obstacle, sizeObstacles);
	glGenVertexArrays(1, &VAO_OBSTACLES);
	glBindVertexArray(VAO_OBSTACLES);
	glGenBuffers(1, &VBO_OBSTACLES);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_OBSTACLES);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Point), &obstacle[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	/***************************************
	* Geometry and color construction of the character
	****************************************/
	draw_character(character);
	glGenVertexArrays(1, &VAO_CHARACTER);
	glBindVertexArray(VAO_CHARACTER);
	glGenBuffers(1, &VBO_CHARACTER);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_CHARACTER);
	glBufferData(GL_ARRAY_BUFFER, vertices_character * sizeof(Point), &character[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//Genero VAO e VBO dei particellari
	glGenVertexArrays(1, &VAO_PARTICELLE);
	glBindVertexArray(VAO_PARTICELLE);
	glGenBuffers(1, &VBO_PARTICELLE);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_PARTICELLE);
	glBufferData(GL_ARRAY_BUFFER, n_Particellari * sizeof(Point), &Particelle[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//Defines the color that will be assigned to the screen
	glClearColor(1.0, 0.5, 0.0, 1.0);
}

void drawScene(void)
{

	/****************************
	* draw Cloud *************
	*****************************/
	glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(programIdClouds);
	// Invio tempo corrente e risoluzione allo shader
	int timeLoc = glGetUniformLocation(programIdClouds, "time");
	int resLoc = glGetUniformLocation(programIdClouds, "resolution");
	glUniform1f(timeLoc, glfwGetTime()); 
	glUniform2f(resLoc, width, height);
	glBindVertexArray(VAO_CLOUDS);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);


	glUseProgram(programId);
	
	/****************************
	* draw CHARACTER*************
	*****************************/
	Model = mat4(1.0);
	Model = translate(Model, vec3(posxCharacter, posyCharacter + posy, 0.0));
	Model = scale(Model, vec3((float)CHARACTERDIMENSION, (float)CHARACTERDIMENSION, 1.0));
	if (hit) {
		Model = rotate(Model, radians((float)90), vec3(0.0, 0.0, 1.0));
		
	} 
	else if (goingUp == true) { 
		Model = rotate(Model, radians(angleBird), vec3(0.0, 0.0, 1.0)); 
		vec3 pos_character = { posxCharacter, posyCharacter + posy, 0 };
		vec3 direzione = vec3{ 1,0,0 };
		aggiungiFlussoColorato(); //QUANDO ALZI QUOTA CREA PARTICELLE
	}

	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_CHARACTER);
	glDrawArrays(GL_TRIANGLES, 0, vertices_character);
	glBindVertexArray(0);

	/**********************************************
	*********   draw obstacles   ******************
	***********************************************/
	glUseProgram(programIdObstacles);

	glBindVertexArray(VAO_OBSTACLES);

	// Ostacolo down
	Model = mat4(1.0);
	Model = translate(Model, vec3(dxobstacles, dyobstaclesdown, 0));
	Model = scale(Model, vec3(30.0, 30.0, 1.0));
	glUniformMatrix4fv(glGetUniformLocation(programIdObstacles, "Model"), 1, GL_FALSE, value_ptr(Model));
	glUniformMatrix4fv(glGetUniformLocation(programIdObstacles, "Projection"), 1, GL_FALSE, value_ptr(Projection));
	glDrawArrays(GL_TRIANGLE_FAN, 0, nVertices_obstacle);

	// Ostacolo up
	Model = mat4(1.0);
	Model = translate(Model, vec3(dxobstacles, dyobstaclesup, 0));
	Model = scale(Model, vec3(30.0, 30.0, 1.0));
	glUniformMatrix4fv(glGetUniformLocation(programIdObstacles, "Model"), 1, GL_FALSE, value_ptr(Model));
	glUniformMatrix4fv(glGetUniformLocation(programIdObstacles, "Projection"), 1, GL_FALSE, value_ptr(Projection));
	glDrawArrays(GL_TRIANGLE_FAN, 0, nVertices_obstacle);

	glBindVertexArray(0);

	glUseProgram(programId);

	//Particellari
	Model = mat4(1.0);
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glGenVertexArrays(1, &VAO_PARTICELLE);
	glBindVertexArray(VAO_PARTICELLE);
	glGenBuffers(1, &VBO_PARTICELLE);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_PARTICELLE);
	glBufferData(GL_ARRAY_BUFFER, n_Particellari * sizeof(Point), &Particelle[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glPointSize(3.0);
	glDrawArrays(GL_POINTS, 0, p_attivi);
	glBindVertexArray(0);

	
	if ((posxCharacter >= dxobstacles) && ((posyCharacter + posy  <= dyobstaclesdown + sizeObstacles  * 30.0f) || (posyCharacter + posy + (4.8f*30.0f) >= dyobstaclesup)))
	{
		hit = true;
		if (!printpoints) {
			printf("Punteggio ottenuto %d\n", points);
			printpoints = true;
		}
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
	GLFWwindow* window = glfwCreateWindow(width, height, "LAB_2_DUCK", NULL, NULL);
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
	init();

	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	/******* Function callbacks : chiamate quando si verificano determinati eventi ******/
	// process keyboard input
	glfwSetKeyCallback(window, key_callback);
	// process mouse motion
	glfwSetCursorPosCallback(window, cursor_position_callback);
	// process mouse button pressed or released
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	// process window resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	// Gestione time
	int frames = 0, updates = 0;
	double lastTime = glfwGetTime(), timer = lastTime;
	double deltaTime = 0, nowTime = 0;
	
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// Measure time
		nowTime = glfwGetTime();
		deltaTime += (nowTime - lastTime) / limitFPS;
		lastTime = nowTime;

		// Only update at 60 frames / s
		while (deltaTime >= 1.0) {
			update(0);			// Update function
			updateObstacles(0);
			updates++;
			deltaTime--;		
		}

		// Render 
		drawScene();
		frames++;

		// Reset after one second
		if (glfwGetTime() - timer > 1.0) {
			timer++;
			std::cout << "FPS: " << frames << " Number of Updates:" << updates << std::endl;
			updates = 0, frames = 0;
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(programId);
	glDeleteVertexArrays(1, &VAO_OBSTACLES);
	glDeleteVertexArrays(1, &VAO_LAWN);
	glDeleteVertexArrays(1, &VAO_CHARACTER);
	glDeleteVertexArrays(1, &VAO);

	glfwTerminate(); //Libera le risorse allocate da glfwInit
	return 0;
}