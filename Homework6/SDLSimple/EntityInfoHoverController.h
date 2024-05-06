#pragma once
#include "Button.h"
#include "EntityController.h"
#include "EntityInfoDisplayController.h"

class EntityStatsHoverController : public Button {
private:
	EntityController* m_ec = nullptr;
	EntityStatsDisplayController* m_statsDisplay = nullptr;
	std::vector<EntityMoveDisplayController*> m_moveDisplays = {};
	EntityMoveDisplayController** m_usingMove = nullptr;
	EntityStacksDisplayController* m_stacksDisplay = nullptr;
	EntityTurnOrderDisplayController* m_turnOrderDisplay = nullptr;
public:
	EntityStatsHoverController(GameObject* parent, std::vector<Button*>* allButtons, SlicedImageRenderer* image, Camera* camera, EntityController* ec, EntityStatsDisplayController* statsDisplay, std::vector<EntityMoveDisplayController*> moveDisplays, EntityStacksDisplayController* stacksDisplay, EntityMoveDisplayController** usingMove, EntityTurnOrderDisplayController* turnOrderDisplay) :Button(parent, allButtons, image, camera) { m_ec = ec; m_statsDisplay = statsDisplay; m_moveDisplays = moveDisplays; m_stacksDisplay = stacksDisplay; m_usingMove = usingMove; m_turnOrderDisplay = turnOrderDisplay; };
	void update(float deltaTime);
	void press() override;

	// ————— SETTERS ————— //
	void const setEC(EntityController* ec) { m_ec = ec; };
};

