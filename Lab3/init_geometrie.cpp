#include "Strutture.h"
#include "Lib.h"
#include "inizializzazioni.h"
#include "utilities.h"
// Include AssImp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags


extern string Meshdir;
extern vector<Mesh> Scena;

void crea_cubo(Mesh* mesh)
{

	mesh->vertices.push_back(vec3(-1.0, -1.0, 1.0));
	mesh->colors.push_back(vec4(1.0, 0.0, 0.0, 0.5));
	mesh->vertices.push_back(vec3(1.0, -1.0, 1.0));
	mesh->colors.push_back(vec4(0.0, 1.0, 0.0, 0.5));
	mesh->vertices.push_back(vec3(1.0, 1.0, 1.0));
	mesh->colors.push_back(vec4(0.0, 0.0, 1.0, 0.5));
	mesh->vertices.push_back(vec3(-1.0, 1.0, 1.0));
	mesh->colors.push_back(vec4(1.0, 0.0, 1.0, 0.5));
	
	// back
	mesh->vertices.push_back(vec3(-1.0, -1.0, -1.0));
	mesh->colors.push_back(vec4(1.0, 1.0, 1.0, 0.5));
	mesh->vertices.push_back(vec3(1.0, -1.0, -1.0));
	mesh->colors.push_back(vec4(1.0, 1.0, 1.0, 0.5));
	mesh->vertices.push_back(vec3(1.0, 1.0, -1.0));
	mesh->colors.push_back(vec4(1.0, 1.0, 1.0, 0.5));
	mesh->vertices.push_back(vec3(-1.0, 1.0, -1.0));
	mesh->colors.push_back(vec4(1.0, 1.0, 1.0, 0.5));
	mesh->vertices.push_back(vec3(0.0, 0.0, 0.0));
	mesh->colors.push_back(vec4(0.0, 1.0, 0.0, 1.0));
	mesh->ancora_obj = (vec4(0.0, 0.0, 0.0, 1.0));

	mesh->indices.push_back(0); mesh->indices.push_back(1); mesh->indices.push_back(2);
	mesh->indices.push_back(2); mesh->indices.push_back(3); mesh->indices.push_back(0);
	mesh->indices.push_back(1); mesh->indices.push_back(5); mesh->indices.push_back(6);
	mesh->indices.push_back(6); mesh->indices.push_back(2); mesh->indices.push_back(1);
	mesh->indices.push_back(7); mesh->indices.push_back(6); mesh->indices.push_back(5);
	mesh->indices.push_back(5); mesh->indices.push_back(4); mesh->indices.push_back(7);
	mesh->indices.push_back(4); mesh->indices.push_back(0); mesh->indices.push_back(3);
	mesh->indices.push_back(3); mesh->indices.push_back(7); mesh->indices.push_back(4);
	mesh->indices.push_back(4); mesh->indices.push_back(5); mesh->indices.push_back(1);
	mesh->indices.push_back(1); mesh->indices.push_back(0); mesh->indices.push_back(4);
	mesh->indices.push_back(3); mesh->indices.push_back(2); mesh->indices.push_back(6);
	mesh->indices.push_back(6); mesh->indices.push_back(7); mesh->indices.push_back(3);
	
	mesh->normals.push_back(normalize(vec3(0, 0, 1)));
	mesh->normals.push_back(normalize(vec3(0, 0, 1)));
	mesh->normals.push_back(normalize(vec3(0, 0, 1)));
	mesh->normals.push_back(normalize(vec3(0, 0, 1)));
	mesh->normals.push_back(normalize(vec3(0, 0, -1)));
	mesh->normals.push_back(normalize(vec3(0, 0, -1)));
	mesh->normals.push_back(normalize(vec3(0, 0, -1)));
	mesh->normals.push_back(normalize(vec3(0, 0, -1)));

	int nv = mesh->vertices.size();
 
	mesh->indices.push_back(nv-1);

	findBB(mesh);
	cout << "max BB cubo" << endl;
	cout << mesh->max_BB_obj.x << ";" << mesh->max_BB_obj.y << ";" << mesh->max_BB_obj.z << endl;
	cout << "min BB" << endl;
	cout << mesh->min_BB_obj.x << ";" << mesh->min_BB_obj.y << ";" << mesh->min_BB_obj.z << endl;
}
void crea_piano(Mesh* mesh, vec4 colore)
{

	mesh->vertices.push_back(vec3(-0.5, 0.0, 0.5));
	mesh->colors.push_back(colore);
	mesh->vertices.push_back(vec3(0.5, 0.0, 0.5));
	mesh->colors.push_back(colore);
	mesh->vertices.push_back(vec3(0.5, 0.0, -0.5));
	mesh->colors.push_back(colore);
	mesh->vertices.push_back(vec3(-0.5, 0.0, -0.5));
	mesh->colors.push_back(colore);

	mesh->indices.push_back(0); mesh->indices.push_back(1); mesh->indices.push_back(2);
	mesh->indices.push_back(0); mesh->indices.push_back(2); mesh->indices.push_back(3);

	mesh->normals.push_back(vec3(0.0, 1.0, 0.0));
	mesh->normals.push_back(vec3(0.0, 1.0, 0.0));
	mesh->normals.push_back(vec3(0.0, 1.0, 0.0));
	mesh->normals.push_back(vec3(0.0, 1.0, 0.0));

	mesh->vertices.push_back(vec3(0.0, 0.0, 0.0));  //Memorizzo come ultimo vertice l'ancora per poterla visualizzare
	mesh->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));
	mesh->ancora_obj = (vec4(0.0, 0.0, 0.0, 1.0));  //memorizzo l'ancora nel campo ancora_obj
	 
	int nv = mesh->vertices.size();
	mesh->indices.push_back(nv-1);
	findBB(mesh);
}
void crea_piano_suddiviso(Mesh* mesh, vec4 colore)
{
	int N = 1024;
	int i, j;
	float x, y,s,t;

	for (i = 0; i < N; i++)
	{
		x =   (float)i / N;
		for (j = 0; j < N; j++)
			
		{
			y =   (float)j / N;
			mesh->vertices.push_back(vec3(x, 0.0,y));
			mesh->colors.push_back(colore);
			mesh->normals.push_back(vec3(0.0, 1.0, 0.0));
			//Coordinate di texture
			s = x;
			t = y;
			mesh->texCoords.push_back(vec2(s, t));
		}
	}
	int cont = -1;
	
	for (i = 0; i <= pow(N, 2) - (N + 1); i++) {

		j = i % (N);

		if (j != N - 1)
		{
			mesh->indices.push_back(i);
			mesh->indices.push_back(i + 1);
			mesh->indices.push_back(i + N );

			mesh->indices.push_back(i + N + 1);
			mesh->indices.push_back(i+1);
			mesh->indices.push_back(i + N);
		}
	}
 
	mesh->vertices.push_back(vec3(0.0, 0.0, 0.0));
	mesh->colors.push_back(vec4(0.0, 1.0, 0.0, 1.0));
	mesh->ancora_obj = (vec4(0.0, 0.0, 0.0, 1.0));
	mesh->colors.push_back(vec4(0.0, 1.0, 0.0, 1.0));
	int nv = mesh->vertices.size();
	mesh->indices.push_back(nv-1);
}
void crea_sfera(Mesh* mesh, vec4 colore)
{
	float s, t;
	vec3 centro = vec3(0.0, 0.0, 0.0);
	vec3 raggio = vec3(1.0, 1.0, 1.0);

	int Stacks = 30;  // numero di suddivisioni sull'asse y
	int Slices = 30;  // numero di suddivisioni sull'asse x

	//Stacks= suddivisioni lungo l'asse y
	for (int i = 0; i <= Stacks; ++i) {

		float V = i / (float)Stacks;
		float phi = V * pi <float>();

		// Loop Through Slices suddivisioni lungo l'asse x
		for (int j = 0; j <= Slices; ++j) {

			float U = j / (float)Slices;
			float theta = U * (pi <float>() * 2);

			// Calc The Vertex Positions
			float x = centro.x + raggio.x * (cosf(theta) * sinf(phi));
			float y = centro.y + raggio.y * cosf(phi);
			float z = centro.z + raggio.z * sinf(theta) * sinf(phi);

			mesh->vertices.push_back(vec3(x, y, z)),
			mesh->colors.push_back(colore);

			//Normale nel vertice
			mesh->normals.push_back(vec3(x, y, z));
			//coordinata di Texture
			s = U;
			t = V;
			mesh->texCoords.push_back(vec2(s, t));
		}
	}

	// Calc The Index Positions
	for (int i = 0; i < Slices * Stacks + Slices; ++i) {

		mesh->indices.push_back(i);
		mesh->indices.push_back(i + Slices + 1);
		mesh->indices.push_back(i + Slices);


		mesh->indices.push_back(i + Slices + 1);
		mesh->indices.push_back(i);
		mesh->indices.push_back(i + 1);
	}

	mesh->vertices.push_back(vec3(0.0, 0.0, 0.0));
	mesh->colors.push_back(vec4(0.0, 1.0, 0.0, 1.0));

	mesh->ancora_obj = (vec4(0.0, 0.0, 0.0, 1.0));

	int nv = mesh->vertices.size();
	mesh->indices.push_back(nv-1);

	findBB(mesh);
	cout << "max OBJ  sfera " << endl;
	cout << mesh->max_BB_obj.x << ";" << mesh->max_BB_obj.y << ";" <<  mesh->max_BB_obj.z << endl;
	cout << "min BB" << endl;
	cout << mesh->min_BB_obj.x << ";" << mesh->min_BB_obj.y << ";" << mesh->min_BB_obj.z << endl;

}
void crea_toro(Mesh* mesh, vec4 colore)
{
	int Stacks = 10;  //numero di suddivisioni sull'asse x
	int Slices = 10;  // numero di suddivisioni sull'asse y
	float R = 1, r = 0.5;
	float s, t;

	//Calc Vertices
	for (int i = 0; i <= Stacks; ++i) {

		float V = i / (float)Stacks;
		float phi = V * glm::pi <float>() * 2;

		// Loop Through Slices
		for (int j = 0; j <= Slices; ++j) {

			float U = j / (float)Slices;
			float theta = U * (glm::pi <float>() * 2);

			// Calc Vertex Positions
			float x = (R + r * cosf(phi)) * cosf(theta);
			float y = r * sinf(phi);
			float z = (R + r * cosf(phi)) * sinf(theta);

			// Push Back Vertex Data
			mesh->vertices.push_back(vec3(x, y, z));
			mesh->colors.push_back(colore);

			//Normale al vertice
			mesh->normals.push_back(vec3(normalize(vec3(sin(phi) * cos(theta), cos(phi), sin(theta) * sin(phi)))));
			s = U;
			t = V;
			mesh->texCoords.push_back(vec2(s, t));
		}
	}

	// Calc The Index Positions
	for (int i = 0; i < Slices * Stacks + Slices; ++i) {

		mesh->indices.push_back(i);
		mesh->indices.push_back(i + Slices + 1);
		mesh->indices.push_back(i + Slices);


		mesh->indices.push_back(i + Slices + 1);
		mesh->indices.push_back(i);
		mesh->indices.push_back(i + 1);
	}
	mesh->vertices.push_back(vec3(0.0, 0.0, 0.0));
	mesh->colors.push_back(vec4(0.0, 1.0, 0.0, 1.0));

	mesh->ancora_obj = (vec4(0.0, 0.0, 0.0, 1.0));

	int nv = mesh->vertices.size();
	mesh->indices.push_back(nv - 1);
	findBB(mesh);

	cout << "max BB toro" << endl;
	cout << mesh->max_BB_obj.x << ";" << mesh->max_BB_obj.y << ";" << mesh->max_BB_obj.z << endl;
	cout << "min BB" << endl;
	cout << mesh->min_BB_obj.x << ";" << mesh->min_BB_obj.y << ";" << mesh->min_BB_obj.z << endl;
}
void crea_cono(Mesh* mesh, vec4 colore)
{
	int Stacks = 30;  //numero di suddivisioni sull'asse x
	int Slices = 30;  // numero di suddivisioni sull'asse y
	float s, t;


	//Calc The Vertices
	for (int i = 0; i <= Stacks; ++i) {

		float V = i / (float)Stacks;
		float h = V;

		// Loop Through Slices
		for (int j = 0; j <= Slices; ++j) {

			float U = j / (float)Slices;
			float theta = U * (glm::pi <float>() * 2);

			// Calc The Vertex Positions
			float x = h * cosf(theta);
			float y = h;
			float z = h * sinf(theta);


			// Push Back Vertex Data
			mesh->vertices.push_back(vec3(x, y, z));
			mesh->colors.push_back(colore);
			//Normale nel vertice
			mesh->normals.push_back(normalize(vec3(cos(theta) / sqrt(2.0f), -1 / sqrt(2.0f), sin(theta) / sqrt(2.0f))));
			//coordinata di texture
			s = U;
			t = V;
			mesh->texCoords.push_back(vec2(s, t));

		}
	}

	// Calc The Index Positions
	for (int i = 0; i < Slices * Stacks + Slices; ++i) {

		mesh->indices.push_back(i);
		mesh->indices.push_back(i + Slices + 1);
		mesh->indices.push_back(i + Slices);


		mesh->indices.push_back(i + Slices + 1);
		mesh->indices.push_back(i);
		mesh->indices.push_back(i + 1);
	}

	mesh->ancora_obj = (vec4(0.0, 0.0, 0.0, 1.0));
	findBB(mesh);
	cout << "max BB cono" << endl;
	cout << mesh->max_BB_obj.x << ";" << mesh->max_BB_obj.y << ";" << mesh->max_BB_obj.z << endl;
	cout << "min BB" << endl;
	cout << mesh->min_BB_obj.x << ";" << mesh->min_BB_obj.y << ";" << mesh->min_BB_obj.z << endl;
}
void crea_cilindro(Mesh* mesh, vec4 colore)
{
	int Stacks = 30;  //numero di suddivisioni sull'asse x
	int Slices = 30;  // numero di suddivisioni sull'asse y

	float s, t;
	//Calc The Vertices
	for (int i = 0; i <= Stacks; ++i) {

		float V = i / (float)Stacks;
		float h = V;

		// Loop Through Slices
		for (int j = 0; j <= Slices; ++j) {

			float U = j / (float)Slices;
			float theta = U * (glm::pi <float>() * 2);

			// Calc The Vertex Positions
			float x = cosf(theta);
			float y = h;
			float z = sinf(theta);

			// Push Back Vertex Data
			mesh->vertices.push_back(vec3(x, y, z));
			mesh->colors.push_back(colore);
			mesh->normals.push_back(vec3(normalize(vec3(cos(theta), 0, sin(theta)))));
			//Coordinata di texture
			s = U;
			t = V;
			mesh->texCoords.push_back(vec2(s, t));
		}
	}

	// Calc The Index Positions
	for (int i = 0; i < Slices * Stacks + Slices; ++i) {

		mesh->indices.push_back(i);
		mesh->indices.push_back(i + Slices + 1);
		mesh->indices.push_back(i + Slices);
		mesh->indices.push_back(i + Slices + 1);
		mesh->indices.push_back(i);
		mesh->indices.push_back(i + 1);
	}

	mesh->ancora_obj = (vec4(0.0, 0.0, 0.0, 1.0));
	findBB(mesh);
	 
	cout << "max BB cilindro" << endl;
	cout << mesh->max_BB_obj.x << ";" << mesh->max_BB_obj.y << ";" << mesh->max_BB_obj.z << endl;
	cout << "min BB" << endl;
	cout << mesh->min_BB_obj.x << ";" << mesh->min_BB_obj.y << ";" << mesh->min_BB_obj.z << endl;

}
bool loadAssImp(const char* path, vector<MeshObj>& mesh);


/***********************  FUNZIONI per OGGETTI MESH *******************************/
bool loadAssImp(const char* path, vector<MeshObj>& mymesh)
{

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene) {
		fprintf(stderr, importer.GetErrorString());
		getchar();
		return false;
	}
	const aiMesh* mesh;

	// Fill vertices positions
	int num_meshes = scene->mNumMeshes;  //Numero di oggetti che compongono il modello
	mymesh.resize(num_meshes);  //Faccio il resize di mymesh al numero di mesh che compongono l'oggetto

	for (unsigned int nm = 0; nm < num_meshes; nm++)
	{
		//Per ogni mesh dell'oggetto 
		mesh = scene->mMeshes[nm];
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		aiColor3D color;
		float value;
		// Read mtl file vertex data

		if (aiReturn_SUCCESS == material->Get(AI_MATKEY_COLOR_AMBIENT, color))
		{
			mymesh[nm].materiale.ambient = glm::vec3(color.r, color.g, color.b);
		}
		else
		{
			printf("Errore in ambientale \n");
		}
		if (aiReturn_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, color))
		{
			mymesh[nm].materiale.diffuse = glm::vec3(color.r, color.g, color.b);
		}
		else
		{
			mymesh[nm].materiale.diffuse = glm::vec3(1.0, 0.2, 0.1);
		}
		if (aiReturn_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, color))
		{
			mymesh[nm].materiale.specular = glm::vec3(color.r, color.g, color.b);
		}
		else
		{
			printf("Errore in specular \n");
			mymesh[nm].materiale.specular = glm::vec3(0.5, 0.5, 0.5);
		}
		if (aiReturn_SUCCESS == material->Get(AI_MATKEY_SHININESS_STRENGTH, value))
		{
			mymesh[nm].materiale.shininess = value;
		}
		else
		{
			//printf("Errore in shininess \n");
			mymesh[nm].materiale.shininess = 50.0f;
		}

		// http://assimp.sourceforge.net/lib_html/structai_material.html
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {

			aiVector3D pos = mesh->mVertices[i];
			mymesh[nm].vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));

		}

		// Fill vertices texture coordinates
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			mymesh[nm].texCoords.push_back(glm::vec2(0.0, 0.0));
			mymesh[nm].colors.push_back(vec4(1.0, 0.0, 1.0, 1.0));  //Colore di default assegnato ad ogni vertice
		}
		// Fill vertices normals
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			aiVector3D n = mesh->mNormals[i];
			mymesh[nm].normals.push_back(glm::vec3(n.x, n.y, n.z));
		}
		// Fill face indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			// Assume the model has only triangles.
			mymesh[nm].indices.push_back(mesh->mFaces[i].mIndices[0]);
			mymesh[nm].indices.push_back(mesh->mFaces[i].mIndices[1]);
			mymesh[nm].indices.push_back(mesh->mFaces[i].mIndices[2]);
		}
	}
	return true;
}

// Rimappa la mesh in un volume [-1,1]^3 con centro l'origine WCS
void normalizeModel(vector<MeshObj>& Model3D) {
	int i, k;
	int nmeshes = Model3D.size();
	vector<vec3> minimo, massimo;
	float minx, miny, minz, maxx, maxy, maxz;
	vec3 centroid = { 0.0f, 0.0f, 0.0f };

	//Calcolo il centroide della mesh (facendo la media dei suoi vertici)
	int numVertices = 0;
	for (i = 0; i < Model3D.size(); i++)

		for (k = 0; k < Model3D[i].vertices.size(); k++)
		{
			centroid += Model3D[i].vertices[k];
			numVertices++;
		}
	centroid /= numVertices;

	for (i = 0; i < Model3D.size(); i++)
		for (k = 0; k < Model3D[i].vertices.size(); k++)
			Model3D[i].vertices[k] -= centroid;

	// Troviamo i valori minimi e massimi per tutte le coordinate del modello
	float minX = std::numeric_limits<float>::max();
	float maxX = -std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float maxY = -std::numeric_limits<float>::max();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = -std::numeric_limits<float>::max();

	// Iteriamo su tutte le mesh e troviamo i minimi e massimi globali
	for (i = 0; i < Model3D.size(); i++)
		for (k = 0; k < Model3D[i].vertices.size(); k++)
		{
			minX = std::min(minX, Model3D[i].vertices[k].x);
			maxX = std::max(maxX, Model3D[i].vertices[k].x);
			minY = std::min(minY, Model3D[i].vertices[k].y);
			maxY = std::max(maxY, Model3D[i].vertices[k].y);
			minZ = std::min(minZ, Model3D[i].vertices[k].z);
			maxZ = std::max(maxZ, Model3D[i].vertices[k].z);
		}

	// Calcoliamo if fattore di scala per ogni dimensione (per mantenere le proporzioni
	float rangeX = maxX - minX;
	float rangeY = maxY - minY;
	float rangeZ = maxZ - minZ;

	float maxRange = std::max({ rangeX, rangeY, rangeZ });
	for (i = 0; i < Model3D.size(); i++)
		for (k = 0; k < Model3D[i].vertices.size(); k++)
		{
			Model3D[i].vertices[k].x = 2.0f * (Model3D[i].vertices[k].x - minX) / maxRange - 1.0f;
			Model3D[i].vertices[k].y = 2.0f * (Model3D[i].vertices[k].y - minY) / maxRange - 1.0f;
			Model3D[i].vertices[k].z = 2.0f * (Model3D[i].vertices[k].z - minZ) / maxRange - 1.0f;
		}

	// Troviamo i valori minimi e massimi per tutte le coordinate del modello
	minX = std::numeric_limits<float>::max();
	maxX = -std::numeric_limits<float>::max();
	minY = std::numeric_limits<float>::max();
	maxY = -std::numeric_limits<float>::max();
	minZ = std::numeric_limits<float>::max();
	maxZ = -std::numeric_limits<float>::max();

	for (i = 0; i < Model3D.size(); i++)
		for (k = 0; k < Model3D[i].vertices.size(); k++)
		{
			minX = std::min(minX, Model3D[i].vertices[k].x);
			maxX = std::max(maxX, Model3D[i].vertices[k].x);
			minY = std::min(minY, Model3D[i].vertices[k].y);
			maxY = std::max(maxY, Model3D[i].vertices[k].y);
			minZ = std::min(minZ, Model3D[i].vertices[k].z);
			maxZ = std::max(maxZ, Model3D[i].vertices[k].z);
		}

	// Calcoliamo if fattore di scala per ogni dimensione (per mantenere le proporzioni
	rangeX = maxX - minX;
	rangeY = maxY - minY;
	rangeZ = maxZ - minZ;
	maxRange = std::max({ rangeX, rangeY, rangeZ });
	//cout << "range X" << rangeX << endl;
	//cout << "range Y" << rangeY << endl;
	//cout << "range Z" << rangeZ << endl;
	//cout << "nax range " << maxRange << endl;
}
void add_obj(const char* name, int shaderType, vec3 translateVec, vec3 scaleVec, float angolo, vec3 rotation_axis, vector<MeshObj>& Model3D) {

	bool obj;
	auto path = Meshdir + name;
	obj = loadAssImp(path.c_str(), Model3D);
	int nmeshes = Model3D.size();

	normalizeModel(Model3D);

	for (int i = 0; i < nmeshes; i++)
	{
		INIT_VAO(&Model3D[i]);
		Model3D[i].Model = mat4(1.0);
		Model3D[i].Model = translate(Model3D[i].Model, translateVec);
		Model3D[i].Model = rotate(Model3D[i].Model, cos(radians(angolo)), rotation_axis);
		Model3D[i].Model = scale(Model3D[i].Model, scaleVec);
		Model3D[i].nome = name;
		Model3D[i].sceltaShader = shaderType; 
		Model3D[i].ancora_obj = vec4(0.0, 0.0, 0.0, 1.0);

		//Scena.push_back(Model3D[i]);

	}
}