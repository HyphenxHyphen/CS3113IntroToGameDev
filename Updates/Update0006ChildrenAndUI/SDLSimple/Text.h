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
	GameObject* m_canvas;
	Font m_font;
	std::string m_text;
	float m_fontSize;
	TextAlign m_textAlign = LEFTTEXTALIGN;

public:
	Text(GameObject* parent) :Component(parent) {};
	Text(GameObject* parent, GameObject* canvas, Font font, std::string text, float fontSize);

	void update(float deltatime) {};
	void render(ShaderProgram* program);

	// ————— GETTERS ————— //
	TextAlign const getTextAlign() const { return m_textAlign; };

	// ————— SETTERS ————— //
	void const setTextAlign(TextAlign textAlign) { m_textAlign = textAlign; };
};

