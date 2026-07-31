#pragma once

#include "lib.h"
#include "strutture.h"

extern int selected_obj;
extern vector<Mesh> Scena;
extern vector<vector<MeshObj>> ScenaObj;
extern bool selected_from_obj;

void modifyModelMatrix(vec3 translation_vector, vec3 rotation_vector, GLfloat angle, GLfloat scale_factor)
{
	//ricordare che mat4(1) costruisce una matrice identità di ordine 4
	vec3 traslModel;
	mat4 aa;
	aa= Scena[selected_obj].Model;
	
	// Estrazione della traslazione corrente :
	//Viene estratta la componente di traslazione dalla matrice di modellazione corrente dell'oggetto selezionato (l'ultima colonna della matrice di
	// Modellazione dell'oggetto selezionato.
	// Questa informazione è necessaria per centrare l'oggetto prima di applicare le trasformazioni di scala e rotazione.

	traslModel = glm::vec3(aa[3][0], aa[3][1], aa[3][2]);
	
	//Traslazione: Viene creata una matrice di traslazione che sposta l'oggetto in base al vettore di traslazione fornito.

	mat4 traslation = glm::translate(glm::mat4(1), translation_vector);
	
	//Per scalare intorno al centro dell'oggetto, lo traslo nell'origine, lo scalo e poi lo riposizono nella sua posizione memorizzata
	//in traslModel
	//Viene creata una matrice di traslazione temporanea per spostare l'oggetto nell'origine.
	// Viene applicata la scala all'oggetto nell'origine.
	// Viene creata una matrice di traslazione inversa per riportare l'oggetto nella sua posizione originale.

	mat4 scala= glm::translate(glm::mat4(1), traslModel);
	scala =scale(scala, glm::vec3(scale_factor, scale_factor, scale_factor));
	scala = translate(scala, -traslModel);


	//Per ruotare intorno al centro dell'oggetto, lo traslo nell'origine, ruoto e poi lo riposizono nella sua posizione memorizzata
	//in traslModel
	//Analogamente alla scala, viene creata una matrice di traslazione temporanea per centrare l'oggetto, 
	// una matrice di rotazione e infine una matrice di traslazione inversa per riportare l'oggetto nella sua posizione originale.
	
	mat4 rotation= glm::translate(glm::mat4(1), traslModel);
	rotation = glm::rotate(rotation, angle, rotation_vector);
	rotation = glm::translate(rotation, -traslModel);
	 

	//Le matrici di traslazione, rotazione e scala vengono moltiplicate nell'ordine corretto (traslazione, rotazione, scala) 
	// e premoltiplicate alla matrice di modellazione corrente dell'oggetto selezionato.
	// Questo aggiorna la matrice di modellazione dell'oggetto selezionato  con le nuove trasformazioni.
	if (selected_obj < 0) return;
	   

	   if (!selected_from_obj) {
		   Scena[selected_obj].Model = traslation * rotation * scala * Scena[selected_obj].Model;
	   }
	   else {
		   for (int j = 0; j < ScenaObj[selected_obj].size(); ++j) {
			   ScenaObj[selected_obj][j].Model = traslation * rotation * scala * ScenaObj[selected_obj][j].Model;
		   }
	   }

}

void update_ancora(Mesh* mesh )
{
	mesh->ancora_world = mesh->ancora_obj;
	mesh->ancora_world = mesh->Model * mesh->ancora_world;
}

void findBB(Mesh* mesh) {
	int n = mesh->vertices.size();
	if (n == 0) return; //Se non ci sono vertici, non facciamo nulla.

	// Inizializzazione degli endpoint con il primo vertice
	float minx = mesh->vertices[0].x;
	float miny = mesh->vertices[0].y;
	float minz = mesh->vertices[0].z;

	float maxx = mesh->vertices[0].x;
	float maxy = mesh->vertices[0].y;
	float maxz = mesh->vertices[0].z;

	// Iterare sui vertici per trovare minimi e massimi
	for (int i = 1; i < n; i++) {
		if (mesh->vertices[i].x < minx) minx = mesh->vertices[i].x;
		if (mesh->vertices[i].x > maxx) maxx = mesh->vertices[i].x;

		if (mesh->vertices[i].y < miny) miny = mesh->vertices[i].y;
		if (mesh->vertices[i].y > maxy) maxy = mesh->vertices[i].y;

		if (mesh->vertices[i].z < minz) minz = mesh->vertices[i].z;
		if (mesh->vertices[i].z > maxz) maxz = mesh->vertices[i].z;
	}

	// Assegnazione del riquadro di delimitazione nello spazio oggetti (locale)
	mesh->min_BB_obj = vec4(minx, miny, minz, 1.0); // Punto mínimo
	mesh->max_BB_obj = vec4(maxx, maxy, maxz, 1.0); // Punto máximo
}

void updateBB(Mesh* mesh) {
	mesh->min_BB = mesh->min_BB_obj;
	mesh->max_BB = mesh->max_BB_obj;
	mesh->min_BB = mesh->Model * mesh->min_BB;
	mesh->max_BB = mesh->Model * mesh->max_BB;
}

bool checkCollisionCameraMesh(ViewSetup camera, Mesh mesh) {

	// Verifichiamo le collisioni nei  3 assi X, Y, Z

	bool collisionX = camera.position.x <= mesh.max_BB.x && camera.position.x >= mesh.min_BB.x;
	bool collisionY = camera.position.y <= mesh.max_BB.y && camera.position.y >= mesh.min_BB.y;
	bool collisionZ = camera.position.z <= mesh.max_BB.z && camera.position.z >= mesh.min_BB.z;

	// Se c'è una collisione su tutti e tre gli assi, allora c'è una collisione
	return collisionX && collisionY && collisionZ;
}
