#include "Scene2.h"

void Scene2::initialise() {
	gameEndTimer = 0.0f;
	m_gameState.victory = false;
	cameraScale = 1.5f;

	clearScene();

	srand((unsigned int)time(NULL)); // Reset Random

	// Initialize Font
	m_gameState.font.m_textureID = Util::loadTexture("Assets/FontAtlas3.png");
	m_gameState.font.m_cols = 5;
	m_gameState.font.m_rows = 9;

	// Initialize Objects (models and assets)

	//----- Map -----
	// Collision Layer
	GameObject* tileMapLayer = new GameObject("TileMapCollisionLayer0", &m_gameState.allGameObjects);
	//m_gameState.allGameObjects.push_back(tileMapLayer);
	tileMapLayer->addComponent<TileMap>(
		new TileMap(tileMapLayer, true, 29, 15,
			0, {}, 0,
			Util::loadLevelData("Assets/Maps/Level 1/Collision 0_1.png"),
			Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Water/Water.png"), 1.0f, 1, 1, {}));
	tileMapLayer->setPosition(Z, 0.0f); //Use this to set z = elevation only
	tileMapLayer->addTag("Pit");
	tileMapLayer->getComponent<Rigidbody2D>()->addIgnoreTag("Airborn");

	// Pits 0
	tileMapLayer = new GameObject("TileMapPits0", &m_gameState.allGameObjects);
	//m_gameState.allGameObjects.push_back(tileMapLayer);
	tileMapLayer->addComponent<TileMap>(
		new TileMap(tileMapLayer, true, 29, 15,
			0, {}, 0,
			Util::loadLevelData("Assets/Maps/Level 1/Pits 0_1.png"),
			Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Water/Water.png"), 1.0f, 1, 1, {}));
	tileMapLayer->setPosition(Z, 0.0f); //Use this to set z = elevation only
	tileMapLayer->addTag("Pit");
	tileMapLayer->getComponent<Rigidbody2D>()->addIgnoreTag("Airborn");

	// Pits 1
	tileMapLayer = new GameObject("TileMapPits1", &m_gameState.allGameObjects);
	//m_gameState.allGameObjects.push_back(tileMapLayer);
	tileMapLayer->addComponent<TileMap>(
		new TileMap(tileMapLayer, true, 29, 15,
			0, {}, 0,
			Util::loadLevelData("Assets/Maps/Level 1/Pits 1_1.png"),
			Util::loadTexture("Assets/Empty.png"), 1.0f, 1, 1, {}));
	tileMapLayer->setPosition(Z, 1.0f); //Use this to set z = elevation only
	tileMapLayer->addTag("Pit");
	tileMapLayer->getComponent<Rigidbody2D>()->addIgnoreTag("Airborn");

	//Tile Edge Foam
	tileMapLayer = new GameObject("TileMapTileEdgeFoam0", &m_gameState.allGameObjects);
	//m_gameState.allGameObjects.push_back(tileMapLayer);
	tileMapLayer->addComponent<TileMap>(
		new TileMap(tileMapLayer, false, 29, 15,
			0,
			{ { 0, { 0,  3,  6,  9, 12, 15, 18, 21, }},
			   { 1, { 1,  4,  7, 10, 13, 16, 19, 22, }},
			   { 2, { 2,  5,  8, 11, 14, 17, 20, 23, }},
			   {24, {24, 27, 30, 33, 36, 39, 42, 45, }},
			   {25, {31}}, //25, 28, 31, 34, 37, 40, 43, 46, }},
			   {26, {26, 29, 32, 35, 38, 41, 44, 47, }},
			   {48, {48, 51, 54, 57, 60, 63, 66, 69, }},
			   {49, {49, 52, 55, 58, 61, 64, 67, 70, }},
			   {50, {50, 53, 56, 59, 62, 65, 68, 71, }},
			}, 0,
			Util::loadLevelData("Assets/Maps/Level 1/Foam 0_1.png"),
			Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Water/Foam/Foam.png"), 1.0f, 24, 3,
			{ {"TL",  0 }, // Top Left
				{"TC",  1 }, // Top Center
				{"TR",  2 }, // Top Right
				{"ML", 24 }, // Middle Left
				{"MC", 25 }, // Middle Center
				{"MR", 26 }, // Middle Right
				{"BL", 48 }, // Bottom Left
				{"BC", 49 }, // Bottom Center
				{"BR", 50 }, // Bottom Right
				{"VT",  1 }, // Vertical Top
				{"VM", 25 }, // Vertical Middle
				{"VB", 49 }, // Vertical Bottom
				{"HL", 48 }, // Horizontal Left
				{"HC", 49 }, // Horizontal Center
				{"HR", 50 }, // Horizontal Right
				{"[]", 49 }, // Isolated
			}));
	tileMapLayer->setPosition(Z, 0.0f); //Use this to set z = elevation only
	
	// Elevation Wall
	tileMapLayer = new GameObject("TileElevationWall0", &m_gameState.allGameObjects);
	//m_gameState.allGameObjects.push_back(tileMapLayer);
	tileMapLayer->addComponent<TileMap>(
		new TileMap(tileMapLayer, false, 29, 15,
			0, {}, 0,
			Util::loadLevelData("Assets/Maps/Level 1/Wall 0_1.png"),
			Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Ground/Tilemap_Elevation.png"), 1.0f, 4, 8, 
			{ {"TL", 12 }, // Top Left
				{"TC", 13 }, // Top Center
				{"TR", 14 }, // Top Right
				{"ML", 12 }, // Middle Left
				{"MC", 13 }, // Middle Center
				{"MR", 14 }, // Middle Right
				{"BL", 12 }, // Bottom Left
				{"BC", 13 }, // Bottom Center
				{"BR", 14 }, // Bottom Right
				{"VT", 15 }, // Vertical Top
				{"VM", 15 }, // Vertical Middle
				{"VB", 15 }, // Vertical Bottom
				{"HL", 12 }, // Horizontal Left
				{"HC", 13 }, // Horizontal Center
				{"HR", 14 }, // Horizontal Right
				{"[]", 15 }, // Isolated
			}));
	tileMapLayer->setPosition(Z, 0.0f); //Use this to set z = elevation only

	// Elevation Ground
	tileMapLayer = new GameObject("TileElevationGround1", &m_gameState.allGameObjects);
	//m_gameState.allGameObjects.push_back(tileMapLayer);
	tileMapLayer->addComponent<TileMap>(
		new TileMap(tileMapLayer, false, 29, 15,
			0, {}, 0,
			Util::loadLevelData("Assets/Maps/Level 1/Ground 1_1.png"),
			Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Ground/Tilemap_Elevation.png"), 1.0f, 4, 8,
			{   {"TL",  0 }, // Top Left
				{"TC",  1 }, // Top Center
				{"TR",  2 }, // Top Right
				{"ML",  4 }, // Middle Left
				{"MC",  5 }, // Middle Center
				{"MR",  6 }, // Middle Right
				{"BL",  8 }, // Bottom Left
				{"BC",  9 }, // Bottom Center
				{"BR", 10 }, // Bottom Right
				{"VT",  3 }, // Vertical Top
				{"VM",  7 }, // Vertical Middle
				{"VB", 11 }, // Vertical Bottom
				{"HL", 16 }, // Horizontal Left
				{"HC", 17 }, // Horizontal Center
				{"HR", 18 }, // Horizontal Right
				{"[]", 19 }, // Isolated
			}));
	tileMapLayer->setPosition(Z, 1.0f); //Use this to set z = elevation only

	// Elevation Grass
	tileMapLayer = new GameObject("TileElevationGrass1", &m_gameState.allGameObjects);
	//m_gameState.allGameObjects.push_back(tileMapLayer);
	tileMapLayer->addComponent<TileMap>(
		new TileMap(tileMapLayer, false, 29, 15,
			0, {}, 0,
			Util::loadLevelData("Assets/Maps/Level 1/Grass 1_1.png"),
			Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Ground/Tilemap_Flat.png"), 1.0f, 10, 4,
			{ {"TL",  0 }, // Top Left
				{"TC",  1 }, // Top Center
				{"TR",  2 }, // Top Right
				{"ML", 10 }, // Middle Left
				{"MC", 11 }, // Middle Center
				{"MR", 12 }, // Middle Right
				{"BL", 20 }, // Bottom Left
				{"BC", 21 }, // Bottom Center
				{"BR", 22 }, // Bottom Right
				{"VT",  3 }, // Vertical Top
				{"VM", 13 }, // Vertical Middle
				{"VB", 23 }, // Vertical Bottom
				{"HL", 30 }, // Horizontal Left
				{"HC", 31 }, // Horizontal Center
				{"HR", 32 }, // Horizontal Right
				{"[]", 33 }, // Isolated
			}));
	tileMapLayer->setPosition(Z, 1.0f); //Use this to set z = elevation only

	// Sand
	tileMapLayer = new GameObject("TileElevationSand1", &m_gameState.allGameObjects);
	//m_gameState.allGameObjects.push_back(tileMapLayer);
	tileMapLayer->addComponent<TileMap>(
		new TileMap(tileMapLayer, false, 29, 15,
			0, {}, 0,
			Util::loadLevelData("Assets/Maps/Level 1/Sand 1_1.png"),
			Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Ground/Tilemap_Flat.png"), 1.0f, 10, 4,
			{ {"TL",  5 }, // Top Left
			{"TC",  6 }, // Top Center
			{"TR",  7 }, // Top Right
			{"ML", 15 }, // Middle Left
			{"MC", 16 }, // Middle Center
			{"MR", 17 }, // Middle Right
			{"BL", 25 }, // Bottom Left
			{"BC", 26 }, // Bottom Center
			{"BR", 27 }, // Bottom Right
			{"VT",  8 }, // Vertical Top
			{"VM", 18 }, // Vertical Middle
			{"VB", 28 }, // Vertical Bottom
			{"HL", 35 }, // Horizontal Left
			{"HC", 36 }, // Horizontal Center
			{"HR", 37 }, // Horizontal Right
			{"[]", 38 }, // Isolated
			}));
	tileMapLayer->setPosition(Z, 1.0f); //Use this to set z = elevation only

	// Entities

	// Player
	GameObject* player = new GameObject("Player", &m_gameState.allGameObjects);
	//m_gameState.allGameObjects.push_back(player);
	player->addComponent<SpriteRenderer>(
		new SpriteRenderer(player, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Factions/Knights/Troops/Warrior/Blue/Warrior_Blue.png"), 6, 8, 0,
			{ { 0,  1,  2,  3,  4,  5},
				{ 6,  7,  8,  9, 10, 11},
				{12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23},
				{24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35},
				{36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
			},
			0, 12));
	player->setPosition({ 5.0f, -6.0f, 1.0f });
	player->addTag("Entity");
	player->addComponent<Rigidbody2D>()->addIgnoreTag("Entity");
	player->addComponent<PolygonCollider2D>(
		new PolygonCollider2D(player,
			{ {-0.5f,  0.5f, 0.0f},
				{ 0.5f,  0.5f, 0.0f},
				{ 0.5f, -0.5f, 0.0f},
				{-0.5f, -0.5f, 0.0f}, }))->setColliderScale({ 0.4f, 0.4f, 1.0f });
	player->addComponent<EntityController>()->setMoveSpeed(3.5f);
	player->getComponent<EntityController>()->setHealth(5.0f);
	player->getComponent<EntityController>()->setAttack(2.0f);
	player->getComponent<EntityController>()->setJumpSFX(g_gameGlobal->g_jumpSFX);
	player->getComponent<EntityController>()->setAtkSFX(g_gameGlobal->g_atkSFX);
	player->getComponent<EntityController>()->setHitSFX(g_gameGlobal->g_hitSFX);
	player->getComponent<EntityController>()->setDieSFX(g_gameGlobal->g_dieSFX);
	player->addComponent<PlayerMovementController>();

	// Enemy
	GameObject* enemyEntity;
	std::string enemyName = "EnemyGuard";
	std::vector<glm::vec3> enemySpawnPositions = {
			{4.0f, -5.0f, 1.0f},
			{6.0f, -5.0f, 1.0f},
			{4.0f, -7.0f, 1.0f},
			{6.0f, -7.0f, 1.0f},
			{14.0f, -5.0f, 1.0f},
			{16.0f, -5.0f, 1.0f},
			{14.0f, -7.0f, 1.0f},
			{16.0f, -7.0f, 1.0f},

			{15.0f, -3.0f, 1.0f},
			//{13.0f, -4.0f, 1.0f},
			//{17.0f, -4.0f, 1.0f},
			{12.0f, -6.0f, 1.0f},
			//{15.0f, -6.0f, 1.0f},
			{18.0f, -6.0f, 1.0f},
			//{13.0f, -8.0f, 1.0f},
			//{17.0f, -8.0f, 1.0f},
			{15.0f, -9.0f, 1.0f},
	};
	for (int enemy = 0; enemy < enemySpawnPositions.size(); enemy++) {
		std::string tempEnemyName = enemyName + std::to_string(enemy);

		enemyEntity = new GameObject(enemyName, &m_gameState.allGameObjects);
		//m_gameState.allGameObjects.push_back(enemyEntity);

		enemyEntity->addComponent<SpriteRenderer>(
			new SpriteRenderer(enemyEntity, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Factions/Knights/Troops/Warrior/Red/Warrior_Red.png"), 6, 8, 0,
				{ { 0,  1,  2,  3,  4,  5},
				  { 6,  7,  8,  9, 10, 11},
				  {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23},
				  {24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35},
				  {36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
				},
				0, 12));
		enemyEntity->setPosition(enemySpawnPositions[enemy]);
		enemyEntity->setRotation(Y, ((enemySpawnPositions[enemy].x == 4) || (enemySpawnPositions[enemy].x == 16)) ? 0.0f : 180.0f);
		enemyEntity->addTag("Entity");
		enemyEntity->addComponent<Rigidbody2D>()->addIgnoreTag("Entity");
		enemyEntity->addComponent<PolygonCollider2D>(
			new PolygonCollider2D(enemyEntity,
				{ {-0.5f,  0.5f, 0.0f},
					{ 0.5f,  0.5f, 0.0f},
					{ 0.5f, -0.5f, 0.0f},
					{-0.5f, -0.5f, 0.0f}, }))->setColliderScale({ 0.4f, 0.4f, 1.0f });
		enemyEntity->addComponent<EntityController>()->setMoveSpeed(2.0f);
		enemyEntity->getComponent<EntityController>()->setHealth(5.0f);
		enemyEntity->getComponent<EntityController>()->setAttack(1.0f);
		enemyEntity->getComponent<EntityController>()->setJumpSFX(g_gameGlobal->g_jumpSFX);
		enemyEntity->getComponent<EntityController>()->setAtkSFX(g_gameGlobal->g_atkSFX);
		enemyEntity->getComponent<EntityController>()->setHitSFX(g_gameGlobal->g_hitSFX);
		enemyEntity->getComponent<EntityController>()->setDieSFX(g_gameGlobal->g_dieSFX);
		enemyEntity->getComponent<EntityController>()->setTarget(player->getComponent<EntityController>());
		enemyEntity->getComponent<EntityController>()->setEvil(true);
		enemyEntity->addComponent<GoblinAI>();

		//enemyEntity->addComponent<PlayerMovementController>()->setMoveSpeed(2.0f);
		//g_gameState.allEntities[tempEnemyName].update(g_gameState.allLevelData[g_gameState.currentLevelIndex], g_gameState.allRed, g_gameState.allPlayers, g_gameState.allPlayers, &g_gameState.allEffects, FIXED_TIMESTEP);
		//g_gameState.allRed[tempEnemyName] = &g_gameState.allEntities[tempEnemyName];
		//g_gameState.allEntities[tempEnemyName].m_aiType = GUARD;
		//g_gameState.allEntities[tempEnemyName].m_target = &g_gameState.allEntities["Player"];
	}

	enemyName = "EnemyGoblin";
	enemySpawnPositions = {
		{24.0f, -6.0f, 1.0f},
	};
	for (unsigned int enemy = 0; enemy < enemySpawnPositions.size(); enemy++) {
		std::string tempEnemyName = enemyName + std::to_string(enemy + 5);

		enemyEntity = new GameObject(enemyName, &m_gameState.allGameObjects);
		//m_gameState.allGameObjects.push_back(enemyEntity);

		enemyEntity->addComponent<SpriteRenderer>(
			new SpriteRenderer(enemyEntity, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Factions/Goblins/Troops/Torch/Red/Torch_Red.png"), 7, 5, 0,
				{ { 0,  1,  2,  3,  4,  5, 6},
				  { 7,  8,  9, 10, 11, 12},
				  {14, 15, 16, 17, 18},
				  {21, 22, 23, 24, 25},
				  {28, 29, 30, 31, 32},
				},
				0, 12));
		enemyEntity->setPosition(enemySpawnPositions[enemy]);
		enemyEntity->setRotation(Y, 180.0f);
		enemyEntity->addTag("Entity");
		enemyEntity->addComponent<Rigidbody2D>()->addIgnoreTag("Entity");
		enemyEntity->addComponent<PolygonCollider2D>(
			new PolygonCollider2D(enemyEntity,
				{ {-0.5f,  0.5f, 0.0f},
					{ 0.5f,  0.5f, 0.0f},
					{ 0.5f, -0.5f, 0.0f},
					{-0.5f, -0.5f, 0.0f}, }))->setColliderScale({ 0.4f, 0.4f, 1.0f });
		enemyEntity->addComponent<EntityController>()->setMoveSpeed(3.0f);
		enemyEntity->getComponent<EntityController>()->setHealth(5.0f);
		enemyEntity->getComponent<EntityController>()->setAttack(3.0f);
		enemyEntity->getComponent<EntityController>()->setJumpSFX(g_gameGlobal->g_jumpSFX);
		enemyEntity->getComponent<EntityController>()->setAtkSFX(g_gameGlobal->g_atkSFX);
		enemyEntity->getComponent<EntityController>()->setHitSFX(g_gameGlobal->g_hitSFX);
		enemyEntity->getComponent<EntityController>()->setDieSFX(g_gameGlobal->g_dieSFX);
		enemyEntity->getComponent<EntityController>()->setTarget(player->getComponent<EntityController>());
		enemyEntity->getComponent<EntityController>()->setEvil(true);
		enemyEntity->addComponent<GoblinAI>();

		//g_gameState.allEntities[tempEnemyName].setPosition(enemySpawnPositions[enemy]);
		//g_gameState.allEntities[tempEnemyName].setRotation({ g_gameState.allEntities[tempEnemyName].getRotation().x, (enemySpawnPositions[enemy].x == 16) ? 0.0f : 180.0f, g_gameState.allEntities[tempEnemyName].getRotation().z });
		//g_gameState.allEntities[tempEnemyName].update(g_gameState.allLevelData[g_gameState.currentLevelIndex], g_gameState.allRed, g_gameState.allPlayers, g_gameState.allPlayers, &g_gameState.allEffects, FIXED_TIMESTEP);
		//g_gameState.allEnemies[tempEnemyName] = &g_gameState.allEntities[tempEnemyName];
		//g_gameState.allEntities[tempEnemyName].m_aiType = GOBLIN;
		//g_gameState.allEntities[tempEnemyName].m_target = &g_gameState.allEntities["Player"];
		//g_gameState.allEntities[tempEnemyName].m_evil = true;
	}

	m_gameState.gameMode = MAIN_MENU;
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
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if ((event.type == SDL_QUIT) || (event.type == SDL_WINDOWEVENT_CLOSE)) {
			*gameIsRunning = false;
		}
		else if ((event.type == SDL_MOUSEWHEEL) && (event.wheel.y != 0)) {
			if (event.wheel.y > 0) { // scroll up
				cameraScale -= 0.1f;
			}
			else if (event.wheel.y < 0) { // scroll down
				cameraScale += 0.1f;
			}
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
			if (m_gameState.victory) {
				switchScene(3);
			}
			else if (g_gameGlobal->playerLives > 0) {
				switchScene(2);
			}
			else {
				switchScene(0);
			}
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
	//std::cout << m_gameState.allGameObjects.size() << std::endl;

	bool evilVanquished = true;
	bool playerAlive = false;

	for (unsigned int i = 0; i < m_gameState.allGameObjects.size(); i++) {
		GameObject* gameObject = m_gameState.allGameObjects[i];
		if (gameObject->isAlive()) {
			if (TileMap * tileMap = gameObject->getComponent<TileMap>()) {
				//std::cout << gameObject->getObjectID() << " has " << gameObject->getComponentsSize() << std::endl;
			}
			gameObject->update(deltaTime);
			if (EntityController * ec = gameObject->getComponent<EntityController>()) {
				if (ec->isEvil()) {
					evilVanquished = false;
				}
			}
			if (gameObject->getObjectID() == "Player") {
				playerAlive = true;
			}
		}
	}

	if ((evilVanquished) || (!playerAlive)) {
		gameEndTimer += deltaTime;
	}

	if (gameEndTimer > 1.5f) {
		if (evilVanquished) {
			m_gameState.victory = true;
			m_gameState.gameMode = GAME_OVER;
		}
		if (!playerAlive) {
			m_gameState.victory = false;
			m_gameState.gameMode = GAME_OVER;
			g_gameGlobal->playerLives -= 1;
		}
	}
}

void Scene2::updateGameOver(float deltaTime) {

}

void Scene2::render(ShaderProgram* program) {
	if (GameObject * player = m_gameState.findGameObject("Player")) {
		m_projectionMatrix = glm::ortho(-((WINDOW_WIDTH / 320.0f) * 1.25f * cameraScale), ((WINDOW_WIDTH / 320.0f) * 1.25f * cameraScale),
			-((WINDOW_HEIGHT / 320.0f) * 1.25f * cameraScale), ((WINDOW_HEIGHT / 320.0f) * 1.25f * cameraScale),
			-1.0f, 1.0f);
		program->set_projection_matrix(m_projectionMatrix);
		program->set_view_matrix(m_viewMatrix);

		m_gameState.cameraPos = glm::vec3(player->getPosition().x, player->getPosition().y, 0.0f);
		m_viewMatrix = glm::mat4(1.0f);
		m_viewMatrix = glm::translate(m_viewMatrix, glm::vec3(-m_gameState.cameraPos.x, -m_gameState.cameraPos.y, m_gameState.cameraPos.z));
	}

	// Sort allEntities to Render from top of map first (for illusion of depth)
	std::vector<GameObject*> sortedRender = { };
	for (auto gameObject : m_gameState.allGameObjects) {
		if (gameObject->isAlive()) {
			sortedRender.push_back(gameObject);
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

	// Finally, render everything sorted
	for (auto& renderable : sortedRender) {
		//std::cout << "Rendering " << renderable->getObjectID() << std::endl;
		renderable->render(program);
	}

	// Render Text
	if (m_gameState.gameMode == MAIN_MENU) {
		Util::drawText(program, "KILL ALL ENEMIES", glm::vec3(0.0f, 0.75f, 0.0f) + m_gameState.cameraPos, 1.5f, m_gameState.font.m_cols, m_gameState.font.m_rows, m_gameState.font.m_textureID);
		Util::drawText(program, "PRESS ANY KEY TO START", glm::vec3(0.0f, -0.25f, 0.0f) + m_gameState.cameraPos, 1.5f, m_gameState.font.m_cols, m_gameState.font.m_rows, m_gameState.font.m_textureID);
	}
	else if ((m_gameState.gameMode == GAME_OVER) && (m_gameState.victory)) {
		Util::drawText(program, "YOU WIN", glm::vec3(0.0f, 0.75f, 0.0f) + m_gameState.cameraPos, 1.5f, m_gameState.font.m_cols, m_gameState.font.m_rows, m_gameState.font.m_textureID);
		Util::drawText(program, "PRESS ANY KEY TO GO TO THE NEXT LEVEL", glm::vec3(0.0f, -0.25f, 0.0f) + m_gameState.cameraPos, 1.5f, m_gameState.font.m_cols, m_gameState.font.m_rows, m_gameState.font.m_textureID);
	}
	else if ((m_gameState.gameMode == GAME_OVER) && (!m_gameState.victory)) {
		Util::drawText(program, "YOU DIED", glm::vec3(0.0f, 0.75f, 0.0f) + m_gameState.cameraPos, 1.5f, m_gameState.font.m_cols, m_gameState.font.m_rows, m_gameState.font.m_textureID);
		Util::drawText(program, "YOU HAVE " + std::to_string(g_gameGlobal->playerLives) + (g_gameGlobal->playerLives == 1 ? " LIFE" : " LIVES") + " REMAINING", glm::vec3(0.0f, -0.00f, 0.0f) + m_gameState.cameraPos, 1.0f, m_gameState.font.m_cols, m_gameState.font.m_rows, m_gameState.font.m_textureID);
		if (g_gameGlobal->playerLives > 0) {
			Util::drawText(program, "PRESS ANY KEY TO RESTART", glm::vec3(0.0f, -0.75f, 0.0f) + m_gameState.cameraPos, 1.5f, m_gameState.font.m_cols, m_gameState.font.m_rows, m_gameState.font.m_textureID);
		}
		else {
			Util::drawText(program, "PRESS ANY KEY TO GO TO MAIN MENU", glm::vec3(0.0f, -0.75f, 0.0f) + m_gameState.cameraPos, 1.5f, m_gameState.font.m_cols, m_gameState.font.m_rows, m_gameState.font.m_textureID);
		}
	}
	else if (m_gameState.gameMode == GAME_PAUSED) {
		Util::drawText(program, "PAUSED", glm::vec3(0.0f, 0.75f, 0.0f) + m_gameState.cameraPos, 1.5f, m_gameState.font.m_cols, m_gameState.font.m_rows, m_gameState.font.m_textureID);
		Util::drawText(program, "PRESS P TO RESUME", glm::vec3(0.0f, -0.25f, 0.0f) + m_gameState.cameraPos, 1.5f, m_gameState.font.m_cols, m_gameState.font.m_rows, m_gameState.font.m_textureID);
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