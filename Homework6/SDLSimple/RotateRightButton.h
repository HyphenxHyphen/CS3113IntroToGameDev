#pragma once
#include "Button.h"
#include "HexController.h"
#include "EntityInfoDisplayController.h"


class RotateRightButton : public Button {
private:
	HexData* m_allyHexData = nullptr;
	EntityMoveDisplayController** m_usingMove = nullptr;
	EntityTurnOrderDisplayController* m_turnOrderDisplay = nullptr;

public:
	RotateRightButton(GameObject* parent, std::vector<Button*>* allButtons, SlicedImageRenderer* image, Camera* camera, HexData* allyHexData, EntityMoveDisplayController** usingMove, EntityTurnOrderDisplayController* turnOrderDisplay) :Button(parent, allButtons, image, camera) { m_allyHexData = allyHexData; m_usingMove = usingMove; m_turnOrderDisplay = turnOrderDisplay; };
	void update(float deltaTime) {};
	void processInput() {};
	void press() override;
};

