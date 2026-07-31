#include <iostream>
#include "Lib.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
 
// Load 2D texture images
unsigned int loadTexture(char const* path, int flip)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(flip);
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
	    // Load texture image from CPU to texture memory (GPU)
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		
		glGenerateMipmap(GL_TEXTURE_2D);
		//Texture displacement logic
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//Texture sampling logic
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

// Create and Load 2D procedural texture image
unsigned int loadTexture_proc(char const* path)
{
	unsigned int textureID;

	/////////////////////////////////////////////////////////////////////////
	//  Compute checkboard procedural_texture image of dimension 
	//  64x64 x 3 (RGB)
	/////////////////////////////////////////////////////////////////////////
	GLubyte data[64][64][3];
	int i, j, c;
	for (i = 0; i < 64; i++) {
		for (j = 0; j < 64; j++) {
			c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			data[i][j][0] = (GLubyte)c;
			data[i][j][1] = (GLubyte)c;
			data[i][j][2] = (GLubyte)c;
		}
	}
	/////////////////////////////////////////
	glGenTextures(1, &textureID);
	// Create texture object for a texture image
	glBindTexture(GL_TEXTURE_2D, textureID);
	// Load texture image from CPU to texture memory (GPU)
	glTexImage2D(GL_TEXTURE_2D,  //the target
		0, // the mip map level we want to generate
		GL_RGB, // the format of the texture
		64, //texture_size, width
		64, //texture_size, heigth
		0,  // border, leave 0
		GL_RGB, // we assume is a RGB color image with 24 bit depth per pixel
		GL_UNSIGNED_BYTE, // the data type
		data); 
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	return textureID;
}


unsigned int loadMyTexture_proc(char const* path)
{
	unsigned int textureID;
	const int width = 64;
	const int height = 64;
	const int centerX = width / 2;
	const int centerY = height / 2;
	const int radius = 10;

	GLubyte data[height][width][3]; //matrice formato rgb ogni pixel

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int dx = x - centerX; 
			int dy = y - centerY;
			int distSquared = dx * dx + dy * dy;

			if (distSquared < radius * radius) {
				// dentro cerchio
				data[y][x][0] = 255; //rosso
				data[y][x][1] = 0;
				data[y][x][2] = 0;
			}
			else {
				// fuori cerchio, bianco 
				data[y][x][0] = 255;
				data[y][x][1] = 255;
				data[y][x][2] = 255;
			}
		}
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return textureID;
}


unsigned int loadCubemap(vector<std::string> faces, int flip)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{	 
		stbi_set_flip_vertically_on_load(flip);
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
