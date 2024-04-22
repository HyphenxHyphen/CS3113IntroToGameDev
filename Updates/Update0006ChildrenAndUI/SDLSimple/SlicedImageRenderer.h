#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include "glm/mat4x4.hpp"
#include "ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"

//#include <map>   
//#include "Util.h"

#include "Component.h"
class SlicedImageRenderer : public Component {
private:
	float m_width;
	float m_height;

	GLuint m_textureID = 0;
	std::vector<float> m_vertices = std::vector<float>(12 * 9, 0.0f);
	std::vector<float> m_textureCoordinates = std::vector<float>(12 * 9, 0.0f);

public:
	SlicedImageRenderer(GameObject* parent) :Component(parent) {};
	SlicedImageRenderer(GameObject* parent, GLint textureID, float width, float height) :Component(parent) { m_textureID = textureID; m_width = width; m_height = height; build(); };

	void build();
	//void drawFromAtlas(ShaderProgram* program, GLuint textureID, int index);
	void update(float deltaTime) {};
	void render(ShaderProgram* program);
};

