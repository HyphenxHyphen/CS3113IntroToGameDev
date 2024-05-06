#pragma once
#include "Button.h"
#include "HexController.h"
#include "EntityInfoDisplayController.h"

class RotateLeftButton : public Button {
private:
	HexData* m_allyHexData = nullptr;
	EntityMoveDisplayController** m_usingMove = nullptr;
	EntityTurnOrderDisplayController* m_turnOrderDisplay = nullptr;

public:
	RotateLeftButton(GameObject* parent, std::vector<Button*>* allButtons, SlicedImageRenderer* image, Camera* camera, HexData* allyHexData, EntityMoveDisplayController** usingMove, EntityTurnOrderDisplayController* turnOrderDisplay) :Button(parent, allButtons, image, camera) { m_allyHexData = allyHexData; m_usingMove = usingMove; m_turnOrderDisplay = turnOrderDisplay; };
	void update(float deltaTime) {};
	void processInput() override {};
	void press() override;
};

