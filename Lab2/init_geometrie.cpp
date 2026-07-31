// LAB_2_2D_DUCK.cpp : 
 
#include "lib.h"
#define PI 3.14159265358979323846

#include <vector>
#include <sstream>
#include <fstream>

// viewport size
extern int width;
extern int height;

typedef struct { float x, y, r, g, b, a; } Point;

static float	posyStart = 1;
/*----------------------------------------*/
/*-------------OBSTACLES------------------*/
float	dxobstacles = (GLfloat)width;
float	dyobstaclesdown = posyStart;
float	dyobstaclesup = float(height) - 100;
int		obstacles_per_row = 1;
int		frame = 0;
float	SPEEDOBSTACLES = 41;
int		nVertices_obstacle = 6;
float	sizeObstacles = 10;
Point*	obstacle = new Point[nVertices_obstacle];
/*---------------------------------------*/
//


//Particelle
int n_Particellari = 1000;
Point* Particelle = new Point[n_Particellari];


/*-------------CHARACTER---------------------*/
float	posx = float(width) / 4;
float	posy = posyStart;
int		vertices_character = 72;
float	posxCharacter = float(width) / 7;
float	posyCharacter = float(height) / 2;
Point*	character = new Point[vertices_character];
#define CHARACTERDIMENSION  50
float	angleBird = 45;
bool	goingUp = false;

double	verticalSpeedDown = 0;
double	verticalSpeed = 0;
double	maxJump = height / 2 + 50.0f;

/*-------------COLORI---------------------*/
vec4 col_white = { 1.0,1.0,1.0, 1.0 };
vec4 col_red = { 1.0,0.0,0.0, 1.0 };
vec4 col_black = { 0.0,0.0,0.0, 1.0 };
vec4 col_white2 = { 1,1,0.9, 1.0 };
vec4 col_pipe_green = { 0.0, 0.6, 0.2, 1.0 };

//duck
vec4 col_head = { 0.9, 0.8, 0.1, 1.0 };
vec4 col_body = { 0.9, 0.8, 0.1, 1.0 };
vec4 col_beak = { 0.9,0.2,0.3, 1.0 };
vec4 col_wing2 = { 0.9, 0.7, 0.1, 1.0 };
/*---------------------------------------*/
/*********************         SCENE DESIGN          *********************************************/
void draw_plan(float x, float y, float width, float height, vec4 color_top, vec4 color_bot, Point* plan, int firstPoint)
{
	plan[0 + firstPoint].x = x;	plan[0 + firstPoint].y = y;
	plan[0 + firstPoint].r = color_top.r; plan[0 + firstPoint].g = color_top.g; plan[0 + firstPoint].b = color_top.b; plan[0 + firstPoint].a = color_top.a;
	plan[1 + firstPoint].x = x + width;	plan[1 + firstPoint].y = y;
	plan[1 + firstPoint].r = color_bot.r; plan[1 + firstPoint].g = color_bot.g; plan[1 + firstPoint].b = color_bot.b; plan[1 + firstPoint].a = color_bot.a;
	plan[2 + firstPoint].x = x + width;	plan[2 + firstPoint].y = y + height;
	plan[2 + firstPoint].r = color_top.r; plan[2 + firstPoint].g = color_top.g; plan[2 + firstPoint].b = color_top.b; plan[2 + firstPoint].a = color_top.a;

	plan[3 + firstPoint].x = x + width;	plan[3 + firstPoint].y = y + height;
	plan[3 + firstPoint].r = color_top.r; plan[3 + firstPoint].g = color_top.g; plan[3 + firstPoint].b = color_top.b; plan[3 + firstPoint].a = color_top.a;
	plan[4 + firstPoint].x = x;	plan[4 + firstPoint].y = y + height;
	plan[4 + firstPoint].r = color_top.r; plan[4 + firstPoint].g = color_top.g; plan[4 + firstPoint].b = color_top.b; plan[4 + firstPoint].a = color_top.a;
	plan[5 + firstPoint].x = x;	plan[5 + firstPoint].y = y;
	plan[5 + firstPoint].r = color_top.r; plan[5 + firstPoint].g = color_top.g; plan[5 + firstPoint].b = color_top.b; plan[5 + firstPoint].a = color_top.a;
}



void draw_character(Point* character) {

	float x = 1;
	float headSize = 1;
	float BodySize = 1;
	float legsSize = headSize / 4;
	float eyeSize = headSize / 5;
	float beakLength = headSize / 3;
	float armSize = BodySize / 6;

	//head
	draw_plan(x, x, headSize, headSize, col_head, col_head, character, 0);
	//body
	draw_plan(x + headSize, x, -headSize - (BodySize / 3) * 2, -BodySize, col_body, col_body, character, 6);
	//wing
	draw_plan(x - BodySize / 4, x - BodySize / 4, BodySize / 3 + headSize / 2, -BodySize / 3, col_wing2, col_wing2, character, 12);
	//eye
	draw_plan(x + 1.5f * eyeSize, x + eyeSize * 2.1f, eyeSize * 1.8f, eyeSize * 1.8f, col_white2, col_white2, character, 30);
	//eye2
	draw_plan(x + 1.9f * eyeSize, x + eyeSize * 2.5f, eyeSize, eyeSize, col_black, col_black, character, 36);
	//beak
	draw_plan(x + headSize-0.4f, x + eyeSize-0.1f, beakLength * 2.0f, beakLength+0.1f, col_beak, col_beak, character, 42);
	//neck
	draw_plan(x, x + headSize / 8, headSize, -headSize / 8, col_white2, col_white2, character, 48);
	//tail
	draw_plan(x - (BodySize / 3) * 2, x, -BodySize / 4, -BodySize / 3, col_wing2, col_wing2, character, 54);
	

}

void draw_obstacles(Point* obstacle, float size) {
	float x = -0.2f;      // Posizione iniziale X
	float y = -1.0f;      // Posizione iniziale Y
	float width = 5.0f;   // Larghezza fissa del rettangolo

	// Definizione dei 4 vertici del rettangolo
	obstacle[0].x = x;          obstacle[0].y = y;           // Angolo inferiore sinistro
	obstacle[1].x = x + width;  obstacle[1].y = y;           // Angolo inferiore destro
	obstacle[2].x = x + width;  obstacle[2].y = y + size;    // Angolo superiore destro
	obstacle[3].x = x;          obstacle[3].y = y + size;    // Angolo superiore sinistro

	// Imposta il colore per tutti i vertici
	for (int i = 0; i < 4; i++) {
		obstacle[i].r = col_pipe_green.r;
		obstacle[i].g = col_pipe_green.g;
		obstacle[i].b = col_pipe_green.b;
		obstacle[i].a = col_pipe_green.a;
	}
}
