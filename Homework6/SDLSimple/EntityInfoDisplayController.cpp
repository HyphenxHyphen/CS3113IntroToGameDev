#include "EntityInfoDisplayController.h"
#include "EntityInfoHoverController.h"

EntityStatsDisplayController::EntityStatsDisplayController(GameObject* parent, SlicedImageRenderer* bg, Font font) :Component(parent) {
	GameObject* nameDisplayGO = new GameObject("nameDisplayGO", getAllGameObjects());
	nameDisplayGO->addTag("UI");
	m_nameDisplay = nameDisplayGO->addComponent<Text>(new Text(nameDisplayGO, font, "NAME", 0.9f));
	m_parent->addChild(nameDisplayGO);
	nameDisplayGO->setPositionRelative({ -bg->getWidth() / 2.0f + 0.1f, 0.7f, 0.0f });

	GameObject* hpDisplayTextGO = new GameObject("hpDisplayTextGO", getAllGameObjects());
	hpDisplayTextGO->addTag("UI");
	hpDisplayTextGO->addComponent<Text>(new Text(hpDisplayTextGO, font, "HP: ", 0.7f));
	m_parent->addChild(hpDisplayTextGO);
	hpDisplayTextGO->setPositionRelative({ -bg->getWidth() / 2.0f + 0.1f, 0.4f, 0.0f });

	GameObject* hpDisplayValueGO = new GameObject("hpDisplayValueGO", getAllGameObjects());
	hpDisplayValueGO->addTag("UI");
	m_hpDisplay = hpDisplayValueGO->addComponent<Text>(new Text(hpDisplayValueGO, font, "100/100", 0.7f));
	m_hpDisplay->setTextAlign(RIGHTTEXTALIGN);
	m_parent->addChild(hpDisplayValueGO);
	hpDisplayValueGO->setPositionRelative({ bg->getWidth() / 2.0f - 0.1f, 0.4f, 0.0f });

	GameObject* atkDisplayTextGO = new GameObject("atkDisplayTextGO", getAllGameObjects());
	atkDisplayTextGO->addTag("UI");
	atkDisplayTextGO->addComponent<Text>(new Text(atkDisplayTextGO, font, "ATK: ", 0.7f));
	m_parent->addChild(atkDisplayTextGO);
	atkDisplayTextGO->setPositionRelative({ -bg->getWidth() / 2.0f + 0.1f, 0.2f, 0.0f });

	GameObject* atkDisplayValueGO = new GameObject("atkDisplayValueGO", getAllGameObjects());
	atkDisplayValueGO->addTag("UI");
	m_atkDisplay = atkDisplayValueGO->addComponent<Text>(new Text(atkDisplayValueGO, font, "100", 0.7f));
	m_atkDisplay->setTextAlign(RIGHTTEXTALIGN);
	m_parent->addChild(atkDisplayValueGO);
	atkDisplayValueGO->setPositionRelative({ bg->getWidth() / 2.0f - 0.1f, 0.2f, 0.0f });

	GameObject* defDisplayTextGO = new GameObject("defDisplayTextGO", getAllGameObjects());
	defDisplayTextGO->addTag("UI");
	defDisplayTextGO->addComponent<Text>(new Text(defDisplayTextGO, font, "DEF: ", 0.7f));
	m_parent->addChild(defDisplayTextGO);
	defDisplayTextGO->setPositionRelative({ -bg->getWidth() / 2.0f + 0.1f, 0.0f, 0.0f });

	GameObject* defDisplayValueGO = new GameObject("defDisplayValueGO", getAllGameObjects());
	defDisplayValueGO->addTag("UI");
	m_defDisplay = defDisplayValueGO->addComponent<Text>(new Text(defDisplayValueGO, font, "100", 0.7f));
	m_defDisplay->setTextAlign(RIGHTTEXTALIGN);
	m_parent->addChild(defDisplayValueGO);
	defDisplayValueGO->setPositionRelative({ bg->getWidth() / 2.0f - 0.1f, 0.0f, 0.0f });

	GameObject* accDisplayTextGO = new GameObject("accDisplayTextGO", getAllGameObjects());
	accDisplayTextGO->addTag("UI");
	accDisplayTextGO->addComponent<Text>(new Text(accDisplayTextGO, font, "ACC: ", 0.7f));
	m_parent->addChild(accDisplayTextGO);
	accDisplayTextGO->setPositionRelative({ -bg->getWidth() / 2.0f + 0.1f, -0.2f, 0.0f });

	GameObject* accDisplayValueGO = new GameObject("accDisplayValueGO", getAllGameObjects());
	accDisplayValueGO->addTag("UI");
	m_accDisplay = accDisplayValueGO->addComponent<Text>(new Text(accDisplayValueGO, font, "100", 0.7f));
	m_accDisplay->setTextAlign(RIGHTTEXTALIGN);
	m_parent->addChild(accDisplayValueGO);
	accDisplayValueGO->setPositionRelative({ bg->getWidth() / 2.0f - 0.1f, -0.2f, 0.0f });

	GameObject* spdDisplayTextGO = new GameObject("spdDisplayTextGO", getAllGameObjects());
	spdDisplayTextGO->addTag("UI");
	spdDisplayTextGO->addComponent<Text>(new Text(spdDisplayTextGO, font, "SPD: ", 0.7f));
	m_parent->addChild(spdDisplayTextGO);
	spdDisplayTextGO->setPositionRelative({ -bg->getWidth() / 2.0f + 0.1f, -0.4f, 0.0f });

	GameObject* spdDisplayValueGO = new GameObject("spdDisplayValueGO", getAllGameObjects());
	spdDisplayValueGO->addTag("UI");
	m_spdDisplay = spdDisplayValueGO->addComponent<Text>(new Text(spdDisplayValueGO, font, "100", 0.7f));
	m_spdDisplay->setTextAlign(RIGHTTEXTALIGN);
	m_parent->addChild(spdDisplayValueGO);
	spdDisplayValueGO->setPositionRelative({ bg->getWidth() / 2.0f - 0.1f, -0.4f, 0.0f });
}

void EntityStatsDisplayController::update(float deltaTime) {
	if (m_ec != nullptr) {
		m_nameDisplay->setText(m_ec->getName());
		m_hpDisplay->setText(std::to_string((int)std::ceil(m_ec->getStat("hp"))) + "/" + std::to_string((int)std::ceil(m_ec->getStat("hpMax"))));
		m_atkDisplay->setText(std::to_string((int)std::ceil(m_ec->getStat("atk"))));
		m_defDisplay->setText(std::to_string((int)std::ceil(m_ec->getStat("def"))));
		m_accDisplay->setText(std::to_string((int)std::ceil(m_ec->getStat("acc"))));
		m_spdDisplay->setText(std::to_string((int)std::ceil(m_ec->getStat("spd"))));
	}
}

EntityMoveDisplayController::EntityMoveDisplayController(GameObject* parent, std::vector<Button*>* allButtons, SlicedImageRenderer* bg, Camera* camera, Font font, 
	int moveInd, EntityMoveDisplayController** usingMove) :Button(parent, allButtons, bg, camera) {
	m_bg = bg;
	m_moveInd = moveInd;
	m_usingMove = usingMove;

	GameObject* moveNameGO = new GameObject("moveNameGO", getAllGameObjects());
	moveNameGO->addTag("UI");
	m_moveName = moveNameGO->addComponent<Text>(new Text(moveNameGO, font, "MOVE NAME", 0.5f));
	m_parent->addChild(moveNameGO);
	moveNameGO->setPositionRelative({ -bg->getWidth() / 2.0f + 0.1f, bg->getHeight() / 2.0f - 0.2f, 0.0f });

	GameObject* moveRangeGO = new GameObject("moveRangeGO", getAllGameObjects());
	moveRangeGO->addTag("UI");
	m_moveRange = moveRangeGO->addComponent<Text>(new Text(moveRangeGO, font, "MOVE RANGE", 0.25f));
	m_parent->addChild(moveRangeGO);
	moveRangeGO->setPositionRelative({ -bg->getWidth() / 2.0f + 0.1f, bg->getHeight() / 2.0f - 0.325f, 0.0f });

	GameObject* moveDescGO = new GameObject("moveDescGO", getAllGameObjects());
	moveDescGO->addTag("UI");
	m_moveDesc = moveDescGO->addComponent<Text>(new Text(moveDescGO, font, "MOVE DESCRIPTION", 0.2f));
	m_moveDesc->setWrap(30);
	m_parent->addChild(moveDescGO);
	moveDescGO->setPositionRelative({ -bg->getWidth() / 2.0f + 0.1f, bg->getHeight() / 2.0f - 0.425f, 0.0f });
}

void EntityMoveDisplayController::update(float deltaTime) {
	if (m_ec == nullptr) { return; }

	std::vector<UnitMoveInfo> unitMoves = m_ec->getUnitMovesKnown();
	m_moveName->setText(unitMoves[m_moveInd].m_moveName);
	m_moveRange->setText(rangeToText(unitMoves[m_moveInd].m_moveRange));
	m_moveDesc->setText(unitMoves[m_moveInd].m_moveDesc);
}

void EntityMoveDisplayController::press() {
	if (m_ec == nullptr) { return; }
	if (m_turnOrderDisplay->getTurnOrder()->at(0) != m_ec) { return; }
	if (m_ec->getAnimState()!= IDLE) { return; }

	if ((*m_usingMove) != this) {
		if ((*m_usingMove) != nullptr) {
			(*m_usingMove)->getBG()->setTexture(0);
		}
		(*m_usingMove) = this;
		m_bg->setTexture(1);
	}
	else if ((*m_usingMove) == this) {
		m_bg->setTexture(0);
		(*m_usingMove) = nullptr;
	}
}

std::string EntityMoveDisplayController::rangeToText(UnitMoveRange range) {
	switch (range) {
	case SHORT:
		return "SHORT (1-2)";
	case MID:
		return "MID (3-4)";
	case LONG:
		return "LONG (5-7)";
	case SELF:
		return "SELF";
	case ALLY:
		return "ALLY";
	}
}

EntityStacksDisplayController::EntityStacksDisplayController(GameObject* parent, std::vector<Button*>* allButtons, SlicedImageRenderer* bg, Camera* camera, Font font) :Button(parent, allButtons, bg, camera) {
	for (int i = 0; i < 17; i++) {
		GameObject* stackDisplayTextGO = new GameObject("stackDisplayTextGO", getAllGameObjects());
		stackDisplayTextGO->addTag("UI");
		Text* stackDisplayText = stackDisplayTextGO->addComponent<Text>(new Text(stackDisplayTextGO, font, "STACK " + std::to_string(i) + ": 0", 0.3f));
		m_stacks.push_back(stackDisplayText);
		m_parent->addChild(stackDisplayTextGO);
		stackDisplayTextGO->setPositionRelative({ -bg->getWidth() / 2.0f + 0.1f, bg->getWidth() / 2.0f - 0.1f - (0.1f * (float)(i + 1)), 0.0f });
	}
}

void EntityStacksDisplayController::update(float deltaTime) {
	if (m_ec == nullptr) { return; }

	std::vector<std::string> keys;
	for (auto stack : *m_ec->getStacks()) {
		keys.push_back(stack->m_name);
	}

	int i = 0;
	while ((i < m_stacks.size()) && (i < keys.size())) {
		m_stacks[i]->setText(keys[i] + ": " + std::to_string(m_ec->getStackCount(keys[i])));
		i++;
	}

	while (i < m_stacks.size()) {
		m_stacks[i]->setText("");
		i++;
	}
}

EntityTurnOrderDisplayController::EntityTurnOrderDisplayController(GameObject* parent, std::vector<Button*>* allButtons, SlicedImageRenderer* bg, Camera* camera, Font font, EntityStatsDisplayController* statsDisplay, std::vector<EntityMoveDisplayController*> moveDisplays, EntityStacksDisplayController* stacksDisplay, EntityMoveDisplayController** usingMove) :Component(parent) {
	m_bg = bg;
	m_bg->setWidth(9 * (m_bg->getHeight() - 0.15) + 0.2f);

	for (auto& moveDisplay : moveDisplays) {
		moveDisplay->setTurnOrderDisplay(this);
	}

	for (int i = 0; i < 9; i++) {
		GameObject* unitTurnOrderDisplayGO = new GameObject("unitTurnOrderDisplayGO", getAllGameObjects());
		unitTurnOrderDisplayGO->addTag("UI");
		SlicedImageRenderer* unitTurnOrderDisplayImage = unitTurnOrderDisplayGO->addComponent<SlicedImageRenderer>(
			new SlicedImageRenderer(unitTurnOrderDisplayGO,
				Util::loadTexture("Assets/Silverframe.png"), 1.0f, 1.0f));
		unitTurnOrderDisplayGO->setScale({ m_bg->getHeight() - 0.15f, m_bg->getHeight() - 0.15f, 1.0f });
		m_turnOrderImages.push_back(unitTurnOrderDisplayImage);
		unitTurnOrderDisplayGO->addComponent<EntityStatsHoverController>(new EntityStatsHoverController(unitTurnOrderDisplayGO, allButtons, unitTurnOrderDisplayImage, camera, nullptr, statsDisplay, moveDisplays, stacksDisplay, usingMove, this));
		getParent()->addChild(unitTurnOrderDisplayGO);
		//unitTurnOrderDisplayGO->setPositionRelative({ (-m_bg->getWidth() / 2.0f) + (unitTurnOrderDisplayGO->getScale().x/2.0f) + 0.1f + (unitTurnOrderDisplayGO->getScale().x * i), 0.0f, 0.0f });
	}
}

void EntityTurnOrderDisplayController::update(float deltaTime) {
	// Turn order logic
	//if (m_turnOrderDisplay->getTurnOrder());
}

void EntityTurnOrderDisplayController::sortUnitTurnOrder() {
	m_turnOrder.clear();

	for (auto& unit : m_allUnits) {
		if (unit->getParent()->isAlive()) {
			m_turnOrder.push_back(unit);
		}
	}

	for (unsigned int i = 1; i < m_turnOrder.size(); i++) {
		int j = i;
		while (j > 0 && (m_turnOrder[j]->getStat("spd") > m_turnOrder[j - 1]->getStat("spd"))) {
			std::swap(m_turnOrder[j], m_turnOrder[j - 1]);
			j--;
		}
	}

	m_bg->setWidth(m_turnOrder.size() * (m_bg->getHeight() - 0.15) + 0.2f);
	getParent()->setPositionRelative(X, -5.0f + m_bg->getWidth() / 4.0f + 0.1f);

	for (int i = 0; i < m_turnOrderImages.size(); i++) {
		//std::cout << m_turnOrderImages[i]->getParent()->getScaleRelative().x << i << std::endl;
		m_turnOrderImages[i]->getParent()->setPositionRelative(X, (-m_bg->getWidth() / 2.0f) + (m_turnOrderImages[i]->getParent()->getScaleRelative().x / 2.0f) + 0.1f + (m_turnOrderImages[i]->getParent()->getScaleRelative().x * i));
		if (i < m_turnOrder.size()) {
			m_turnOrderImages[i]->setTexture(0, m_turnOrder[i]->getIcon());
			m_turnOrderImages[i]->getComponent<EntityStatsHoverController>()->setEC(m_turnOrder[i]);
			m_turnOrderImages[i]->getParent()->setAlive(true);
		}
		else {
			m_turnOrderImages[i]->getParent()->setAlive(false);
		}
	}
}

void EntityTurnOrderDisplayController::startTurn() {
	m_turnOrder[0]->onTurnStart();
}

void EntityTurnOrderDisplayController::endTurn() {
	m_turnOrder[0]->onTurnEnd();

	std::vector<int> removeTurns = { 0 };
	for (int i = 1; i < m_turnOrder.size(); i++) {
		if (m_turnOrder[i]->getStat("hp") <= 0) {
			removeTurns.push_back(i);
		}
	}

	for (int i = removeTurns.size()-1; i > -1; i--) {
		m_turnOrder.erase(m_turnOrder.begin() + removeTurns[i]);
	}

	if (m_turnOrder.size() > 0) {
		for (int i = 0; i < m_turnOrderImages.size(); i++) {
			if (i < m_turnOrder.size()) {
				m_turnOrderImages[i]->setTexture(0, m_turnOrder[i]->getIcon());
				m_turnOrderImages[i]->getComponent<EntityStatsHoverController>()->setEC(m_turnOrder[i]);
				m_turnOrderImages[i]->getParent()->setAlive(true);
			}
			else {
				m_turnOrderImages[i]->getParent()->setAlive(false);
			}
		}
	}
	else {
		sortUnitTurnOrder();
	}

	startTurn();
}
