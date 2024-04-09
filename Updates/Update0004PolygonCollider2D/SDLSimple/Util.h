#pragma once

#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define NUMBER_OF_TEXTURES 1
#define LEVEL_OF_DETAIL    0
#define TEXTURE_BORDER     0
//#define FONTBANK_SIZE      16

class Util
{
public:
	static GLuint loadTexture(const char* filepath);
	static std::vector<unsigned int> loadLevelData(const char* filepath);
	static void drawText(ShaderProgram& program, std::string text, glm::vec3 position, int fontCols, int fontRows, GLuint fontTextureID);
};

