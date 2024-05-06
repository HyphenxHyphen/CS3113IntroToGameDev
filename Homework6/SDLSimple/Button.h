#pragma once
#include <SDL.h>
#include "Component.h"
#include "SlicedImageRenderer.h"
#include "Camera.h"

class Button : public Component {
protected:
	SlicedImageRenderer* m_image = nullptr;
	Camera* m_camera = nullptr;
	bool m_pressed = false;
	bool m_hover = false;
	std::vector<Button*>* m_allButtons;
public:
	Button(GameObject* parent, std::vector<Button*>* allButtons, SlicedImageRenderer* image, Camera* camera) :Component(parent) { m_allButtons = allButtons; allButtons->push_back(this); m_image = image; m_camera = camera; };
	void virtual press() = 0;
	bool isHovered();

	// ————— GETTERS ————— //
	std::vector<Button*>* const getAllButtons() const { return m_allButtons; };
	SlicedImageRenderer* const getImage() const { return m_image; };
};

