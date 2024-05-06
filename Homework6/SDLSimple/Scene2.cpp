#include "Scene2.h"

void Scene2::initialise() {
	m_camera.m_scale = 1.0f;

	srand((unsigned int)time(NULL)); // Reset Random

	// Initialize Font
	m_gameState.font.m_textureID = Util::loadTexture("Assets/FontAtlas4c.png");
	m_gameState.font.m_cols = 16;
	m_gameState.font.m_rows = 4;

	// Initialize Objects (models and assets)

	//----- Map -----

	// Canvas UI
	GameObject* canvasGO = new GameObject("canvasGO", &m_gameState.allGameObjects);
	canvasGO->addTag("UI");
	//canvasGO->addComponent<SlicedImageRenderer>(
	//	new SlicedImageRenderer(canvasGO,
	//		Util::loadTexture("Assets/Silverframe.png"), 10.0f, 7.5f));

	GameObject* moveBGGO = new GameObject("moveBGGO", &m_gameState.allGameObjects);
	moveBGGO->addTag("UI");
	canvasGO->addChild(moveBGGO);
	moveBGGO->addComponent<SlicedImageRenderer>(
		new SlicedImageRenderer(moveBGGO,
			Util::loadTexture("Assets/Silverframe.png"), 5.0f, 2.0f));
	moveBGGO->setPositionRelative({ 5.0f - 5.0f / 2.0f - 0.1f, -3.75f + 2.0f / 2.0f + 0.1f, 0.0f });

	GameObject* moveGO = nullptr;
	SlicedImageRenderer* moveBG = nullptr;
	for (int i = 0; i < 4; i++) {
		moveGO = new GameObject("moveGO", &m_gameState.allGameObjects);
		moveGO->addTag("UI");
		moveBGGO->addChild(moveGO);
		moveBG = moveGO->addComponent<SlicedImageRenderer>(
			new SlicedImageRenderer(moveGO,
				Util::loadTexture("Assets/Silverframe.png"), 2.5f, 1.0f));
		moveBG->addTexture(Util::loadTexture("Assets/Blueframe.png"));
		EntityMoveDisplayController* moveDisplay = moveGO->addComponent<EntityMoveDisplayController>(
			new EntityMoveDisplayController(moveGO, &m_gameState.allButtons, moveBG, &m_camera, m_gameState.font, i, &m_usingMove));
		m_moveDisplays.push_back(moveDisplay);
		moveGO->setPositionRelative({ 2.5f / 2.0f * (i < 2 ? -1.0f : 1.0f), 1.0f / 2.0f * (i % 2 == 0 ? -1.0f : 1.0f), 0.0f });
	}

	GameObject* unitStatsBGGO = new GameObject("unitStatsBGGO", &m_gameState.allGameObjects);
	unitStatsBGGO->addTag("UI");
	canvasGO->addChild(unitStatsBGGO);
	//unitStatsBGGO->setScaleRelative({ 0.5f, 0.5f, 1.0f });
	unitStatsBGGO->addComponent<SlicedImageRenderer>(
		new SlicedImageRenderer(unitStatsBGGO,
			Util::loadTexture("Assets/Silverframe.png"), 4.8f, 2.0f));
	unitStatsBGGO->setPositionRelative({ -5.0f + 4.80f / 2.0f + 0.1f, -2.65f, 0.0f });

	GameObject* unitStatsUnitIconGO = new GameObject("unitStatsUnitIconGO", &m_gameState.allGameObjects);
	unitStatsUnitIconGO->addTag("UI");
	unitStatsBGGO->addChild(unitStatsUnitIconGO);
	//unitStatsBGGO->setScaleRelative({ 0.5f, 0.5f, 1.0f });
	unitStatsUnitIconGO->addComponent<SlicedImageRenderer>(
		new SlicedImageRenderer(unitStatsUnitIconGO,
			Util::loadTexture("Assets/Silverframe.png"), 2.0f, 2.0f));
	unitStatsUnitIconGO->setPositionRelative({ -2.4f + 2.0f / 2.0f, 0.0f, 0.0f });
	EntityStacksDisplayController* unitStacksDisplay = unitStatsUnitIconGO->addComponent<EntityStacksDisplayController>(
		new EntityStacksDisplayController(unitStatsUnitIconGO, &m_gameState.allButtons, unitStatsUnitIconGO->getComponent<SlicedImageRenderer>(), &m_camera, m_gameState.font)
		);

	GameObject* unitStatsTrayGO = new GameObject("unitStatsTrayGO", &m_gameState.allGameObjects);
	unitStatsTrayGO->addTag("UI");
	unitStatsBGGO->addChild(unitStatsTrayGO);
	//unitStatsBGGO->setScaleRelative({ 0.5f, 0.5f, 1.0f });
	unitStatsTrayGO->addComponent<SlicedImageRenderer>(
		new SlicedImageRenderer(unitStatsTrayGO,
			Util::loadTexture("Assets/Silverframe.png"), 2.8f, 2.0f));
	unitStatsTrayGO->setPositionRelative({ 2.4f - 2.8f / 2.0f, 0.0f, 0.0f });
	EntityStatsDisplayController* unitStatsDisplay = unitStatsTrayGO->addComponent<EntityStatsDisplayController>(
		new EntityStatsDisplayController(unitStatsTrayGO, unitStatsTrayGO->getComponent<SlicedImageRenderer>(), m_gameState.font)
		);

	GameObject* turnOrderBGGO = new GameObject("turnOrderBGGO", &m_gameState.allGameObjects);
	turnOrderBGGO->addTag("UI");
	canvasGO->addChild(turnOrderBGGO);
	turnOrderBGGO->setScaleRelative({ 0.5f, 0.5f, 1.0f });
	SlicedImageRenderer* turnOrderBGImage = turnOrderBGGO->addComponent<SlicedImageRenderer>(
		new SlicedImageRenderer(turnOrderBGGO,
			Util::loadTexture("Assets/Silverframe.png"), 6.0f, 1.0f));
	m_turnOrderDisplay = turnOrderBGGO->addComponent<EntityTurnOrderDisplayController>(
		new EntityTurnOrderDisplayController(turnOrderBGGO, &m_gameState.allButtons, turnOrderBGImage, &m_camera, m_gameState.font, unitStatsDisplay, m_moveDisplays, unitStacksDisplay, &m_usingMove));
	turnOrderBGGO->setPositionRelative({ -5.0f + 3.00f / 2.0f + 0.1f, -1.4f, 0.0f });
	
	GameObject* rotateGO = nullptr;

	rotateGO = new GameObject("rotateGOR", &m_gameState.allGameObjects);
	rotateGO->addTag("UI");
	moveBGGO->addChild(rotateGO);
	rotateGO->addComponent<SlicedImageRenderer>(
		new SlicedImageRenderer(rotateGO,
			Util::loadTexture("Assets/Silverframe.png"), 1.0f, 1.0f));
	rotateGO->addComponent<RotateLeftButton>(new RotateLeftButton(rotateGO, &m_gameState.allButtons, rotateGO->getComponent<SlicedImageRenderer>(), &m_camera, &m_allyHexData, &m_usingMove, m_turnOrderDisplay));
	rotateGO->setScaleRelative({ 0.5f, 0.5f, 1.0f });
	rotateGO->setPositionRelative({ 2.5f - 0.25f, 1.0f + 0.25, 0.0f });

	rotateGO = new GameObject("rotateGOL", &m_gameState.allGameObjects);
	rotateGO->addTag("UI");
	moveBGGO->addChild(rotateGO);
	rotateGO->addComponent<SlicedImageRenderer>(
		new SlicedImageRenderer(rotateGO,
			Util::loadTexture("Assets/Silverframe.png"), 1.0f, 1.0f));
	rotateGO->addComponent<RotateRightButton>(new RotateRightButton(rotateGO, &m_gameState.allButtons, rotateGO->getComponent<SlicedImageRenderer>(), &m_camera, &m_allyHexData, &m_usingMove, m_turnOrderDisplay));
	rotateGO->setScaleRelative({ 0.5f, 0.5f, 1.0f });
	rotateGO->setPositionRelative({ 2.5f - 0.75f, 1.0f + 0.25, 0.0f });
	// Background stuff

	// Ally Tiles

	GameObject* allyHexesBack = new GameObject("allyTilesBack", &m_gameState.allGameObjects);

	GameObject* allyHex = nullptr;
	std::vector<glm::vec3> allyHexPositions = {
		{ 0.45f, 0.77f - 13.75f, 0.0f },
		{ 0.9f, 0.0f - 13.75f, 0.0f },
		{ 0.45f, -0.77f - 13.75f, 0.0f },
		{ -0.45f, -0.77f - 13.75f, 0.0f },
		{ -0.9f, 0.0f - 13.75f, 0.0f },
		{ -0.45f, 0.77f - 13.75f, 0.0f },
	};

	for (auto allyHexPosition : allyHexPositions) {
		allyHex = new GameObject("allyHex", &m_gameState.allGameObjects);
		allyHex->addComponent<SpriteRenderer>(
			new SpriteRenderer(allyHex,
				Util::loadTexture("Assets/HexTileG.png"), 1, 1, 0, {}, 0, 0));
		allyHexesBack->addChild(allyHex);
		m_allyHexData.m_hexes.push_back(allyHex->addComponent<HexController>());
		allyHex->setRotationRelative(Z, 90);
		allyHex->setPositionRelative(allyHexPosition);
	}

	allyHexesBack->setScale({ 0.4f, 1.5f, 1.0f });
	allyHexesBack->setPosition({ -2.5f, 5.0f, 0.0f });

	// Enemy Tiles

	GameObject* enemyHexesBack = new GameObject("enemyTilesBack", &m_gameState.allGameObjects);

	GameObject* enemyHex = nullptr;
	std::vector<glm::vec3> enemyHexPositions = {
		{ 0.45f, 0.77f - 13.75f, 0.0f },
		{ 0.9f, 0.0f - 13.75f, 0.0f },
		{ 0.45f, -0.77f - 13.75f, 0.0f },
		{ -0.45f, -0.77f - 13.75f, 0.0f },
		{ -0.9f, 0.0f - 13.75f, 0.0f },
		{ -0.45f, 0.77f - 13.75f, 0.0f },
	};
	
	for (auto enemyHexPosition : enemyHexPositions) {
		enemyHex = new GameObject("enemyHex", &m_gameState.allGameObjects);
		enemyHex->addComponent<SpriteRenderer>(
			new SpriteRenderer(enemyHex,
				Util::loadTexture("Assets/HexTileR.png"), 1, 1, 0, {}, 0, 0));
		enemyHexesBack->addChild(enemyHex);
		m_enemyHexData.m_hexes.push_back(enemyHex->addComponent<HexController>());
		enemyHex->setRotationRelative(Z, 90);
		enemyHex->setPositionRelative(enemyHexPosition);
	}

	enemyHexesBack->setScale({ 0.4f, 1.5f, 1.0f });
	enemyHexesBack->setPosition({ 2.5f, 5.0f, 0.0f });

	// Entities
	// Allies
	
	GameObject* ant = nullptr;
	EntityController* antEC = nullptr;
	SlicedImageRenderer* antImg = nullptr;

	for (int i = 0; i < 3; i++) {
		ant = new GameObject("ant", &m_gameState.allGameObjects);
		antEC = ant->addComponent<EntityController>(new EntityController(ant, 0, true, &m_allies, &m_enemies, &m_allyHexData, &m_enemyHexData, m_turnOrderDisplay));
		ant->addComponent<SpriteRenderer>(
			new SpriteRenderer(ant,
				antEC->getSprite(), 1, 1, 0, {}, 0, 0));
		antImg = ant->addComponent<SlicedImageRenderer>(
			new SlicedImageRenderer(ant,
				Util::loadTexture("Assets/Empty.png"), 0.5f, 0.75f));
		antImg->addTexture(Util::loadTexture("Assets/Target.png"));
		ant->addComponent<Rigidbody2D>(new Rigidbody2D(ant));
		m_allHoverControllers.push_back(ant->addComponent<EntityStatsHoverController>(new EntityStatsHoverController(ant, &m_gameState.allButtons, antImg, &m_camera, antEC, unitStatsDisplay, m_moveDisplays, unitStacksDisplay, &m_usingMove, m_turnOrderDisplay)));
		ant->setRotation(Y, 180.0f);
		ant->setScale({ 1.5f, 1.5f, 1.0f });
		m_allies.push_back(antEC);
		m_turnOrderDisplay->addEntityController(antEC);

		// Set Hex
		antEC->setHex(i);
		ant->setPosition({ m_allyHexData.m_hexes[i]->getPosition().x, m_allyHexData.m_hexes[i]->getPosition().y + 0.7f, 3.5f - abs(2.5f - i) });
		m_allyHexData.m_hexes[i]->setOccupant(antEC);
	}

	// Enemies

	GameObject* enemy = nullptr;
	EntityController* enemyEC = nullptr;
	SlicedImageRenderer* enemyImg = nullptr;

	for (int i = 0; i < 6; i++) {
		enemy = new GameObject("enemy", &m_gameState.allGameObjects);
		enemyEC = enemy->addComponent<EntityController>(new EntityController(enemy, 1, false, &m_enemies, &m_allies, &m_enemyHexData, &m_allyHexData, m_turnOrderDisplay));
		enemy->addComponent<SpriteRenderer>(
			new SpriteRenderer(enemy,
				enemyEC->getSprite(), 1, 1, 0, {}, 0, 0));
		enemyImg = enemy->addComponent<SlicedImageRenderer>(
			new SlicedImageRenderer(enemy,
				Util::loadTexture("Assets/Empty.png"), 0.5f, 0.75f));
		enemyImg->addTexture(Util::loadTexture("Assets/Target.png"));
		enemy->addComponent<Rigidbody2D>(new Rigidbody2D(enemy));
		m_allHoverControllers.push_back(enemy->addComponent<EntityStatsHoverController>(new EntityStatsHoverController(enemy, &m_gameState.allButtons, enemyImg, &m_camera, enemyEC, unitStatsDisplay, m_moveDisplays, unitStacksDisplay, &m_usingMove, m_turnOrderDisplay)));
		enemy->setScale({ 1.5f, 1.5f, 1.0f });
		m_enemies.push_back(enemyEC);
		m_turnOrderDisplay->addEntityController(enemyEC);

		// Set Hex
		enemyEC->setHex(i);
		enemy->setPosition({ m_enemyHexData.m_hexes[i]->getPosition().x, m_enemyHexData.m_hexes[i]->getPosition().y + 0.7f, 3.5f - abs(2.5f - i) });
		m_enemyHexData.m_hexes[i]->setOccupant(enemyEC);

		enemy->setAlive(false);
	}

	// Sort Turn Order and let's begin the show!
	m_turnOrderDisplay->sortUnitTurnOrder();
}

void Scene2::load() {
	m_override = false;
	int loadMax = 3;
	if (g_gameGlobal->g_ants.size() < loadMax) {
		loadMax = g_gameGlobal->g_ants.size();
	}

	for (int i = 0; i < m_allies.size(); i++) {
		if (i < loadMax) {
			m_allies[i]->loadStats(g_gameGlobal->g_ants[i]->getComponent<EntityController>());
			m_allies[i]->getComponent<SpriteRenderer>()->setTexture(m_allies[i]->getComponent<EntityController>()->getSprite(), 1, 1, 0, {}, 0, 0);
			m_allies[i]->getParent()->setAlive(true);

			// Set Hex
			int spawnHex = g_gameGlobal->g_ants[i]->getComponent<EntityController>()->getSpawnHex();
			m_allies[i]->setHex(spawnHex);
			m_allies[i]->setPosition({ m_allyHexData.m_hexes[spawnHex]->getPosition().x, m_allyHexData.m_hexes[spawnHex]->getPosition().y + 0.7f, 3.5f - abs(2.5f - spawnHex) });
			m_allyHexData.m_hexes[spawnHex]->setOccupant(m_allies[i]);
		}
		else {
			m_allies[i]->getParent()->setAlive(false);
		}
	}

	// Load Enemies
	std::vector<EntityController> spawnEnemies = {};
	int encounter = rand() % 5;
	if (encounter == 0) {
		spawnEnemies.push_back(EntityController(nullptr, 3, false, nullptr, nullptr, nullptr, nullptr, nullptr));
		spawnEnemies[spawnEnemies.size() - 1].setSpawnHex(4);
	}
	else if (encounter == 1) {
		spawnEnemies.push_back(EntityController(nullptr, 5, false, nullptr, nullptr, nullptr, nullptr, nullptr));
		spawnEnemies[spawnEnemies.size() - 1].setSpawnHex(5);
		spawnEnemies.push_back(EntityController(nullptr, 5, false, nullptr, nullptr, nullptr, nullptr, nullptr));
		spawnEnemies[spawnEnemies.size() - 1].setSpawnHex(3);
	}
	else if (encounter == 2) {
		spawnEnemies.push_back(EntityController(nullptr, 6, false, nullptr, nullptr, nullptr, nullptr, nullptr));
		spawnEnemies[spawnEnemies.size() - 1].setSpawnHex(0);
		spawnEnemies.push_back(EntityController(nullptr, 6, false, nullptr, nullptr, nullptr, nullptr, nullptr));
		spawnEnemies[spawnEnemies.size() - 1].setSpawnHex(2);
	}
	else if (encounter == 3) {
		spawnEnemies.push_back(EntityController(nullptr, 4, false, nullptr, nullptr, nullptr, nullptr, nullptr));
		spawnEnemies[spawnEnemies.size() - 1].setSpawnHex(4);
		spawnEnemies.push_back(EntityController(nullptr, 4, false, nullptr, nullptr, nullptr, nullptr, nullptr));
		spawnEnemies[spawnEnemies.size() - 1].setSpawnHex(1);
	}
	else if (encounter == 4) {
		spawnEnemies.push_back(EntityController(nullptr, 3, false, nullptr, nullptr, nullptr, nullptr, nullptr));
		spawnEnemies[spawnEnemies.size() - 1].setSpawnHex(0);
		spawnEnemies.push_back(EntityController(nullptr, 5, false, nullptr, nullptr, nullptr, nullptr, nullptr));
		spawnEnemies[spawnEnemies.size() - 1].setSpawnHex(2);
		spawnEnemies.push_back(EntityController(nullptr, 4, false, nullptr, nullptr, nullptr, nullptr, nullptr));
		spawnEnemies[spawnEnemies.size() - 1].setSpawnHex(3);
		spawnEnemies.push_back(EntityController(nullptr, 6, false, nullptr, nullptr, nullptr, nullptr, nullptr));
		spawnEnemies[spawnEnemies.size() - 1].setSpawnHex(5);
	}

	for (int i = 0; i < m_enemies.size(); i++) {
		m_enemies[i]->getParent()->setAlive(false);
	}

	for (int i = 0; i < spawnEnemies.size(); i++) {
		m_enemies[spawnEnemies[i].getSpawnHex()]->loadStats(&spawnEnemies[i]);
		m_enemies[spawnEnemies[i].getSpawnHex()]->getComponent<SpriteRenderer>()->setTexture(m_enemies[spawnEnemies[i].getSpawnHex()]->getComponent<EntityController>()->getSprite(), 1, 1, 0, {}, 0, 0);
		m_enemies[spawnEnemies[i].getSpawnHex()]->getParent()->setAlive(true);
	}

	// Sort Turn Order and let's begin the show!
	m_turnOrderDisplay->sortUnitTurnOrder();
	m_turnOrderDisplay->startTurn();
	m_gameState.gameMode = GAME_LEVEL;
}

void Scene2::unload() {
	std::cout << "scene 2 unload" << std::endl;


	// Back to map or to title screen

	// Back to the map

	// Clear Stacks
	for (EntityController* ally : m_allies) {
		ally->clearStacks();
	}

	for (EntityController* enemy : m_enemies) {
		enemy->clearStacks();
	}

	// Make sure global ants always have at least 3 members to overwrite into
	while (m_allies.size() > g_gameGlobal->g_ants.size()) {
		GameObject* ant = new GameObject("ant", &g_gameGlobal->g_ants);
		ant->addComponent<EntityController>(new EntityController(ant, 0, true, nullptr, nullptr, nullptr, nullptr, nullptr));
	}

	// Overwrite the first 3 members of global ants
	for (int i = m_allies.size() - 1; i > -1; i--) {
		if (m_allies[i]->getParent()->isAlive()) {
			g_gameGlobal->g_ants[i]->getComponent<EntityController>()->loadStats(m_allies[i]);
		}
		else {
			delete g_gameGlobal->g_ants[i];
			g_gameGlobal->g_ants.erase(g_gameGlobal->g_ants.begin() + i);
		}
	}

	if (g_gameGlobal->g_ants.size() > 0) {
		m_nextScene = 1;
	}
	else {
		m_nextScene = 0;
		g_gameGlobal->g_activeNodes.clear();
	}
}

void Scene2::processInput(bool* gameIsRunning) {
	switch (m_gameState.gameMode)
	{
	case MAIN_MENU:
		processInputMainMenu(gameIsRunning);
		break;

	case GAME_LEVEL:
	case GAME_PAUSED:
		processInputGameLevel(gameIsRunning);
		break;

	case GAME_OVER:
		processInputGameOver(gameIsRunning);
		break;
	}
}

void Scene2::processInputMainMenu(bool* gameIsRunning) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if ((event.type == SDL_QUIT) || (event.type == SDL_WINDOWEVENT_CLOSE)) {
			*gameIsRunning = false;
		}
		else if (event.type == SDL_KEYDOWN) {
			m_gameState.gameMode = GAME_LEVEL;
		}
	}
}

void Scene2::processInputGameLevel(bool* gameIsRunning) {

	if (m_usingMove != nullptr) {
		std::vector<EntityStatsHoverController*> allHoverControllersSorted = m_allHoverControllers;
		for (unsigned int i = 1; i < allHoverControllersSorted.size(); i++) {
			int j = i;
			while (j > 0 && ((allHoverControllersSorted[j]->getPosition().z < allHoverControllersSorted[j - 1]->getPosition().z) || (allHoverControllersSorted[j]->getPosition().y > allHoverControllersSorted[j - 1]->getPosition().y))) {
				std::swap(allHoverControllersSorted[j], allHoverControllersSorted[j - 1]);
				j--;
			}
		}

		for (EntityStatsHoverController* hoverController : m_allHoverControllers) {
			hoverController->getImage()->setTexture(0);
		}
		bool hovered = false;
		for (EntityStatsHoverController* hoverController : allHoverControllersSorted) {
			if (!hovered) {
				hovered = hoverController->isHovered();
				if (hoverController->isHovered()) {
					hoverController->getImage()->setTexture(1);
				}
			}
		}
	}
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if ((event.type == SDL_QUIT) || (event.type == SDL_WINDOWEVENT_CLOSE)) {
			*gameIsRunning = false;
		}
		else if ((event.type == SDL_MOUSEWHEEL) && (event.wheel.y != 0)) {
			if (event.wheel.y > 0) { // scroll up
				m_camera.m_scale -= 0.1f;
				//m_camera.m_position.y += 1.0f;
			}
			else if (event.wheel.y < 0) { // scroll down
				m_camera.m_scale += 0.1f;
				//m_camera.m_position.y -= 1.0f;
			}
			//std::cout << m_camera.m_position.y << std::endl;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_p) {
				if (m_gameState.gameMode == GAME_LEVEL) {
					m_gameState.gameMode = GAME_PAUSED;
				}
				else if (m_gameState.gameMode == GAME_PAUSED) {
					m_gameState.gameMode = GAME_LEVEL;
				}
			}
			else if (event.key.keysym.sym == SDLK_b) {
				m_override = true;
			}
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				//handle a left-click

				std::vector<Button*> buttonsSorted = m_gameState.allButtons;
				for (unsigned int i = 1; i < buttonsSorted.size(); i++) {
					int j = i;
					while (j > 0 && ((buttonsSorted[j]->getPosition().z < buttonsSorted[j - 1]->getPosition().z) || (buttonsSorted[j]->getPosition().y > buttonsSorted[j - 1]->getPosition().y))) {
						std::swap(buttonsSorted[j], buttonsSorted[j - 1]);
						j--;
					}
				}

				bool hovered = false;
				for (Button* button : buttonsSorted) {
					if (button->getParent()->isAlive()) {
						if (!hovered) {
							hovered = button->isHovered();
							if (hovered) {
								button->press();
							}
						}
					}
				}

				// Reset moves if missed click entirely
				if (!hovered) {
					if (m_usingMove != nullptr) {
						m_usingMove->getBG()->setTexture(0);
						m_usingMove = nullptr; 
					}
				}

				for (EntityStatsHoverController* hoverController : m_allHoverControllers) {
					hoverController->getImage()->setTexture(0);
				}
			}
		}
	}

	// Hold
	//const Uint8* key_state = SDL_GetKeyboardState(NULL);

	// Process Objects
	for (auto gameObject : m_gameState.allGameObjects) {
		gameObject->processInput();
	}
}

void Scene2::processInputGameOver(bool* gameIsRunning) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if ((event.type == SDL_QUIT) || (event.type == SDL_WINDOWEVENT_CLOSE)) {
			*gameIsRunning = false;
		}
		else if (event.type == SDL_KEYDOWN) {
			//m_gameState.gameMode = GAME_LEVEL;
			/*
			if (m_gameState.victory) {
				switchScene(2);
			}
			else if (g_gameGlobal->playerLives > 0) {
				switchScene(1);
			}
			else {
				switchScene(0);
			}
			*/
		}
	}
}

void Scene2::update(float deltaTime) {
	switch (m_gameState.gameMode)
	{
	case MAIN_MENU:
		updateMainMenu(deltaTime);
		break;

	case GAME_LEVEL:
		updateGameLevel(deltaTime);
		break;

	case GAME_OVER:
		updateGameOver(deltaTime);
		break;
	}
}

void Scene2::updateMainMenu(float deltaTime) {

}

void Scene2::updateGameLevel(float deltaTime) {
	for (unsigned int i = 0; i < m_gameState.allGameObjects.size(); i++) {
		GameObject* gameObject = m_gameState.allGameObjects[i];
		if ((gameObject->isAlive()) && (!gameObject->hasTag("UI"))) {
			gameObject->update(deltaTime);
		}
	}

	bool allAlliesDead = true;
	for (auto& ally : m_allies) {
		if (allAlliesDead) {
			allAlliesDead = !ally->getParent()->isAlive();
		}
	}

	bool allEnemiesDead = true;
	for (auto& enemy : m_enemies) {
		if (allEnemiesDead) {
			allEnemiesDead = !enemy->getParent()->isAlive();
		}
	}

	if (allAlliesDead || allEnemiesDead || m_override) {
		unload();
	}

	// Update Camera Scale
	m_camera.m_orthoWidth = ((m_camera.WINDOW_WIDTH / 160.0f) * 1.25f * m_camera.m_scale);
	m_camera.m_orthoHeight = ((m_camera.WINDOW_HEIGHT / 160.0f) * 1.25f * m_camera.m_scale);
}

void Scene2::updateGameOver(float deltaTime) {

}

void Scene2::render(ShaderProgram* program) {
	// Camera Zoom
	m_projectionMatrix = glm::ortho(-m_camera.m_orthoWidth / 2.0f, m_camera.m_orthoWidth / 2.0f,
		-m_camera.m_orthoHeight / 2.0f, m_camera.m_orthoHeight / 2.0f,
		-1.0f, 1.0f);
	program->set_projection_matrix(m_projectionMatrix);

	// Camera Position
	m_viewMatrix = glm::mat4(1.0f);
	m_viewMatrix = glm::translate(m_viewMatrix, glm::vec3(-m_camera.m_position.x, -m_camera.m_position.y, m_camera.m_position.z));
	program->set_view_matrix(m_viewMatrix);

	// UI Update
	for (unsigned int i = 0; i < m_gameState.allGameObjects.size(); i++) {
		GameObject* gameObject = m_gameState.allGameObjects[i];
		if ((gameObject->isAlive()) && (gameObject->hasTag("UI"))) {
			gameObject->update(0.0f);
		}
	}

	// Sort allEntities to Render from top of map first (for illusion of depth)
	std::vector<GameObject*> sortedRender = { };
	for (auto gameObject : m_gameState.allGameObjects) {
		if (gameObject->isAlive()) {
			if (gameObject->getParent() == nullptr) {
				sortedRender.push_back(gameObject);
			}
		}
	}

	// Does map render over entities?
	for (unsigned int i = 1; i < sortedRender.size(); i++) {
		int j = i;
		while (j > 0 && ((sortedRender[j]->getPosition().z < sortedRender[j - 1]->getPosition().z) || (sortedRender[j]->getPosition().y > sortedRender[j - 1]->getPosition().y))) {
			std::swap(sortedRender[j], sortedRender[j - 1]);
			j--;
		}
	}

	std::vector<GameObject*> sortedRenderChildren = { };
	for (auto gameObject : sortedRender) {
		std::vector<GameObject*> gameObjectRender = recursiveRenderChildrenSort(gameObject);
		sortedRenderChildren.insert(sortedRenderChildren.end(), gameObjectRender.begin(), gameObjectRender.end());
	}

	//std::cout << "Rendering start" << std::endl;
	// Finally, render everything sorted
	for (auto& renderable : sortedRenderChildren) {
		//if (renderable->getObjectID() == "Path") {
		//	std::cout << renderable->getObjectID() << " scale at " << renderable->getScale().x << ", " << renderable->getScale().y << std::endl;
		//}
		//std::cout << "Rendering " << renderable->getObjectID() << " at " << renderable->getPosition().z << std::endl;
		renderable->render(program);
	}
}

void Scene2::clearScene() {
	for (int i = m_gameState.allGameObjects.size() - 1; i >= 0; i--) {
		if (m_gameState.allGameObjects[i] != nullptr) {
			delete m_gameState.allGameObjects[i];
		}
	}
	m_gameState.allGameObjects.clear();
}