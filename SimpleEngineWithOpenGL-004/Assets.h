#pragma once
#include <map>
#include <string>
#include "Texture.h"

using std::map;
using std::string;

// A static singleton Assets class that hosts several
// functions to load ressources. Each loaded ressources is also
// stored for futur reference by string handles. All functions
// and ressources are static and no public public constructor is defined. 

class Assets
{
public:
	static std::map<std::string, Texture> textures;

	// Loads a texture from file
	static Texture loadTexture(Renderer& renderer, const string& filename, const string& name);

	// Retrieves a stored texture
	static Texture& getTexture(const string& name); // Question: Pourquoi doit on rajouter std:: ici pour le string?

	// Properly de-allocates all loaded ressources
	static void clear();

private:
	Assets() {}

	// Loads a single texture from file
	static Texture loadTextureFromFile(Renderer& renderer, const string& filename);
};