#pragma once
#include "Component.h"
#include "Canvas.h"

enum TextAlign { LEFTTEXTALIGN, CENTERTEXTALIGN, RIGHTTEXTALIGN
};

struct Font {
	GLuint m_textureID = 0;
	int m_cols = 0;
	int m_rows = 0;
};

class Text : public Component {
private:
	Font m_font;
	std::string m_text;
	float m_fontSize = 1.0f;
	TextAlign m_textAlign = LEFTTEXTALIGN;
	int m_wrap = 0;
	glm::vec4 m_color = { 255.0f, 255.0f, 255.0f, 255.0f };

public:
	Text(GameObject* parent) :Component(parent) {};
	Text(GameObject* parent, Font font, std::string text, float fontSize);

	void update(float deltatime) {};
	void render(ShaderProgram* program);

	// ————— GETTERS ————— //
	TextAlign const getTextAlign() const { return m_textAlign; };

	// ————— SETTERS ————— //
	void const setTextAlign(TextAlign textAlign) { m_textAlign = textAlign; };
	void const setText(std::string text) { m_text = text; };
	void const setWrap(int wrap) { m_wrap = wrap; };
	void const setColor(glm::vec4 color) { m_color = color; };
	void const setColor(glm::vec3 color) { m_color[0] = color[0]; m_color[1] = color[1]; m_color[2] = color[2]; };
	void const setAlpha(float alpha) { m_color[3] = alpha; };
};

