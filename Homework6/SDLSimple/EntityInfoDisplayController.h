#pragma once
#include "Component.h"
#include "Text.h"
#include "SlicedImageRenderer.h"
#include "EntityController.h"
#include "Button.h"

class EntityTurnOrderDisplayController;

class EntityStatsDisplayController : public Component {
private:
	EntityController* m_ec = nullptr;

	Text* m_nameDisplay = nullptr;
	Text* m_hpDisplay = nullptr;
	Text* m_atkDisplay = nullptr;
	Text* m_defDisplay = nullptr;
	Text* m_accDisplay = nullptr;
	Text* m_spdDisplay = nullptr;

public:
	EntityStatsDisplayController(GameObject* parent, SlicedImageRenderer* bg, Font font);
	void update(float deltaTime);

	// ————— SETTERS ————— //
	void const setEntityController(EntityController* ec) { m_ec = ec; };
};

class EntityMoveDisplayController : public Button {
private:
	EntityMoveDisplayController** m_usingMove;

	EntityController* m_ec = nullptr;
	SlicedImageRenderer* m_bg = nullptr;
	EntityTurnOrderDisplayController* m_turnOrderDisplay = nullptr;

	Text* m_moveName = nullptr;
	Text* m_moveRange = nullptr;
	Text* m_moveDesc = nullptr;

	//bool m_targeting = false;
	int m_moveInd = 0;
public:
	EntityMoveDisplayController(GameObject* parent, std::vector<Button*>* allButtons, SlicedImageRenderer* bg, Camera* camera, Font font, 
		int moveInd, EntityMoveDisplayController** usingMove);
	void update(float deltaTime);
	void processInput() override {};
	void press() override;
	std::string rangeToText(UnitMoveRange range);


	// ————— GETTERS ————— //
	//bool const getTargeting() const { return m_targeting; };
	EntityController* const getEntityController() const { return m_ec; };
	UnitMoveInfo const getMove() const { return m_ec->getUnitMovesKnown()[m_moveInd]; };
	SlicedImageRenderer* const getBG() const { return m_bg; };

	// ————— SETTERS ————— //
	//void resetTargeting() { m_targeting = false; };
	void setEntityController(EntityController* ec) { m_ec = ec; };
	void setTurnOrderDisplay(EntityTurnOrderDisplayController* turnOrderDisplay) { m_turnOrderDisplay = turnOrderDisplay; };
};

class EntityStacksDisplayController : public Button {
private:
	EntityController* m_ec = nullptr;

	std::vector<Text*> m_stacks = {};
public:
	EntityStacksDisplayController(GameObject* parent, std::vector<Button*>* allButtons, SlicedImageRenderer* bg, Camera* camera, Font font);
	void update(float deltaTime);
	void processInput() override {};
	void press() override {};

	// ————— GETTERS ————— //
	EntityController* const getEntityController() const { return m_ec; };

	// ————— SETTERS ————— //
	void setEntityController(EntityController* ec) { m_ec = ec; };
};

class EntityTurnOrderDisplayController : public Component {
private:
	std::vector<EntityController*> m_allUnits = {};
	std::vector<EntityController*> m_turnOrder = {};
	std::vector<SlicedImageRenderer*> m_turnOrderImages = {};

	SlicedImageRenderer* m_bg = nullptr;

public:
	EntityTurnOrderDisplayController(GameObject* parent, std::vector<Button*>* allButtons, SlicedImageRenderer* bg, Camera* camera, Font font, EntityStatsDisplayController* statsDisplay, std::vector<EntityMoveDisplayController*> moveDisplays, EntityStacksDisplayController* stacksDisplay, EntityMoveDisplayController** usingMove);
	void update(float deltaTime);
	void processInput() override {};
	void sortUnitTurnOrder();
	void startTurn();
	void endTurn();

	// ————— GETTERS ————— //
	SlicedImageRenderer* const getBG() const { return m_bg; };
	std::vector<EntityController*>* const getTurnOrder() { return &m_turnOrder; };

	// ————— SETTERS ————— //
	void addEntityController(EntityController* ec) { m_allUnits.push_back(ec); };
};