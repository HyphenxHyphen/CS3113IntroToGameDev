#include "Scene1.h"

void Scene1::initialise() {
	//std::cout << "checkpoint 1" << std::endl;

	m_camera.m_scale = 1.5f;

	srand((unsigned int)time(NULL)); // Reset Random

	// Initialize Font
	m_gameState.font.m_textureID = Util::loadTexture("Assets/FontAtlas4c.png");
	m_gameState.font.m_cols = 16;
	m_gameState.font.m_rows = 4;

	// Initialize Objects (models and assets)

	//----- Map -----

	// Canvas
	GameObject* mapGO = new GameObject("mapGO", &m_gameState.allGameObjects);
	mapGO->addComponent<Canvas>(new Canvas(mapGO, nullptr));
	SlicedImageRenderer* mapBackImage = mapGO->addComponent<SlicedImageRenderer>(
		new SlicedImageRenderer(mapGO,
			Util::loadTexture("Assets/Silverframe.png"), 11.0f, 30.0f));
	mapGO->addTag("UI");
	//mapGO->setPositionRelative({ -mapBackImage->getWidth()/2.0f, mapBackImage->getHeight() / 2.0f, 0.0f });
	//mapGO->setPosition(Z, 0.0f); //Use this to set z = elevation only

	GameObject* node = nullptr;
	for (int y = 0; y < m_mapHeight; y++) {
		for (int x = 0; x < m_mapWidth; x++) {
			node = new GameObject("Node", &m_gameState.allGameObjects);
			SlicedImageRenderer* nodeImage = node->addComponent<SlicedImageRenderer>(
				new SlicedImageRenderer(node,
					Util::loadTexture("Assets/Silverframe.png"), 1.0f, 1.0f));
			node->addComponent<NodeController>(new NodeController(node, &m_gameState.allButtons, nodeImage, &m_camera, &m_chosenNode, &g_gameGlobal->g_activeNodes));
			node->addTag("UI");
			//node->addComponent<Button>(new Button(node, nodeImage, &m_camera));
			node->setPosition(X, -((mapBackImage->getWidth()) / 2.0f) + ((.5f + x) * (mapBackImage->getWidth()) / (m_mapWidth)) + (0.50f * (((rand() % 50) - 25) / 50.0f)));
			node->setPosition(Y, -((mapBackImage->getHeight()) / 2.0f) + ((.5f + y) * mapBackImage->getHeight() / (m_mapHeight + 2.5f)) + (0.90f * (((rand() % 50) - 25) / 50.0f) * (y != 0)));
			node->setScale({ 0.75f, 0.75f, 1.0f });
			node->setPosition(Z, 1.0f);
			mapGO->addChild(node);
			m_nodes.push_back(node);
		}
	}

	GameObject* bossNode = new GameObject("BossNode", &m_gameState.allGameObjects);
	SlicedImageRenderer* nodeImage = bossNode->addComponent<SlicedImageRenderer>(
		new SlicedImageRenderer(bossNode,
			Util::loadTexture("Assets/Silverframe.png"), 1.0f, 1.0f));
	bossNode->addComponent<NodeController>(new NodeController(node, &m_gameState.allButtons, nodeImage, &m_camera, &m_chosenNode, &g_gameGlobal->g_activeNodes));
	bossNode->addTag("UI");
	//node->addComponent<Button>(new Button(node, nodeImage, &m_camera));
	bossNode->setPosition(Y, ((mapBackImage->getHeight()) / 2.0f) - 2.0f );
	bossNode->setScale({ 3.0f, 3.0f, 1.0f });
	bossNode->setPosition(Z, 1.0f);
	mapGO->addChild(bossNode);
	//m_nodes.push_back(node); // maybe needed?

	std::vector<int> currentNodes = {};
	std::vector<int> nextNodes = {};

	for (int i = 0; i < m_mapWidth; i++) {
		int nextNode = (rand() % 7);

		// this ensures there are at least 2 unique starting nodes
		if (currentNodes.size() == 1) {
			while (nextNode == currentNodes[0]) {
				nextNode = (rand() % 7);
			}
		}

		currentNodes.push_back(nextNode);
	}

	// Sort for easy processing
	for (unsigned int i = 1; i < currentNodes.size(); i++) {
		int j = i;
		while (j > 0 && (currentNodes[j] < currentNodes[j - 1])) {
			std::swap(currentNodes[j], currentNodes[j - 1]);
			j--;
		}
	}

	//std::cout << "Current Nodes size: " << currentNodes.size() << std::endl;
	//std::cout << "Next Nodes size: " << nextNodes.size() << std::endl;

	//std::cout << "checkpoint 2" << std::endl;

	for (int y = 1; y < m_mapHeight; y++) {
		for (int i = 0; i < currentNodes.size(); i++) {
			int nextNode = m_mapWidth + currentNodes[i];
			//std::cout << "Next node default: " << nextNode << std::endl;
			//std::cout << "Current Nodes size: " << currentNodes.size() << std::endl;
			//std::cout << "Next Nodes size: " << nextNodes.size() << std::endl;
			//std::cout << "i: " << i << std::endl;
			//std::cout << "y: " << y << std::endl;

			if ((nextNode == y * m_mapWidth) || ((nextNode != ((y + 1) * m_mapWidth) - 1) && (i >= 1) && m_nodes[currentNodes[i] - 1]->getComponent<NodeController>()->hasNextNode(m_nodes[nextNode]))) {
				nextNode += (rand() % 2);
			}
			else if ((nextNode != ((y + 1) * m_mapWidth) - 1) && ((i == 0) || ((i >= 1) && !m_nodes[currentNodes[i] - 1]->getComponent<NodeController>()->hasNextNode(m_nodes[nextNode])))) {
				nextNode += (rand() % 3) - 1;
			}
			else if ((nextNode == ((y + 1) * m_mapWidth) - 1) && ((i == 0) || ((i >= 1) && !m_nodes[currentNodes[i] - 1]->getComponent<NodeController>()->hasNextNode(m_nodes[nextNode])))) {
				nextNode -= (rand() % 2);
			}

			//std::cout << "Next node random: " << nextNode << std::endl;
			if (!m_nodes[currentNodes[i]]->getComponent<NodeController>()->hasNextNode(m_nodes[nextNode])) {
				m_nodes[currentNodes[i]]->getComponent<NodeController>()->addNextNode(m_nodes[nextNode]);
				nextNodes.push_back(nextNode);
			}
			
			GameObject* path = new GameObject("Path", &m_gameState.allGameObjects);
			path->addComponent<LineRenderer>(new LineRenderer(path, Util::loadTexture("Assets/GrabberArm.png"), m_nodes[currentNodes[i]]->getPosition(), m_nodes[nextNode]->getPosition(), 1.0f));
			path->addTag("UI");
			path->setPosition(Z, 0.0f);
			mapGO->addChild(path);

			// Sort for easy processing
			for (unsigned int i = 1; i < nextNodes.size(); i++) {
				int j = i;
				while (j > 0 && (nextNodes[j] < nextNodes[j - 1])) {
					std::swap(nextNodes[j], nextNodes[j - 1]);
					j--;
				}
			}
		}

		currentNodes = nextNodes;
		nextNodes.clear();
	}

	//std::cout << "checkpoint 3" << std::endl;

	for (int i = 0; i < currentNodes.size(); i++) {
		m_nodes[currentNodes[i]]->getComponent<NodeController>()->setNodeType(SCRAP);
		m_nodes[currentNodes[i]]->getComponent<NodeController>()->addNextNode(bossNode);
		GameObject* path = new GameObject("Path", &m_gameState.allGameObjects);
		path->addComponent<LineRenderer>(new LineRenderer(path, Util::loadTexture("Assets/GrabberArm.png"), m_nodes[currentNodes[i]]->getPosition(), bossNode->getPosition(), 1.0f));
		path->addTag("UI");
		path->setPosition(Z, 0.0f);
		mapGO->addChild(path);
	}
	
	for (int i = 0; i < m_nodes.size(); i++) {
		if (m_nodes[i]->getComponent<NodeController>()->getNextNodes().size() == 0) {
			m_nodes[i]->setAlive(false);
		}
	}

	for (int x = 0; x < m_mapWidth; x++) {
		m_nodes[8 * m_mapWidth + x]->getComponent<NodeController>()->setNodeType(TREASURE);
	}

	for (int x = 0; x < m_mapWidth; x++) {
		m_nodes[0 * m_mapWidth + x]->getComponent<NodeController>()->setNodeType(BATTLE);
	}

	//std::cout << "checkpoint 4" << std::endl;

	for (int y = 0; y < 14; y++) {
		if (y != 7) {
			for (int x = 0; x < m_mapWidth; x++) {
				if (m_nodes[y * m_mapWidth + x]->isAlive()) {
					std::vector<NodeType> chosenTypes = { NODENONE };

					if ((m_nodes[y * m_mapWidth + x]->getComponent<NodeController>()->getNodeType() == ELITE) || (y < 6)) {
						chosenTypes.push_back(ELITE);
					}
					if ((m_nodes[y * m_mapWidth + x]->getComponent<NodeController>()->getNodeType() == SCRAP) || (y < 6)) {
						chosenTypes.push_back(SCRAP);
					}
					if (m_nodes[y * m_mapWidth + x]->getComponent<NodeController>()->getNodeType() == TUTOR) {
						chosenTypes.push_back(TUTOR);
					}
					if (m_nodes[y * m_mapWidth + x]->getComponent<NodeController>()->getNodeType() == TRADER) {
						chosenTypes.push_back(TRADER);
					}

					NodeType chosenType = NODENONE;
					//std::cout << "Node at " << m_nodes[y * m_mapWidth + x]->getPosition().y << ", " << m_nodes[y * m_mapWidth + x]->getPosition().x << " has " << m_nodes[y * m_mapWidth + x]->getComponent<NodeController>()->getNextNodes().size() << " next nodes and is " << m_nodes[y * m_mapWidth + x]->getComponent<NodeController>()->getNodeType() << std::endl;
					for (auto node : m_nodes[y * m_mapWidth + x]->getComponent<NodeController>()->getNextNodes()) {
						if (node->getComponent<NodeController>()->getNodeType() == NODENONE) {
							//std::cout << "Node at " << node->getPosition().y << ", " << node->getPosition().x << std::endl;
							//for (auto chosen : chosenTypes) {
							//	std::cout << chosen << ", ";
							//}
							//std::cout << std::endl;
							while ((std::find(chosenTypes.begin(), chosenTypes.end(), chosenType) != chosenTypes.end()) && (chosenTypes.size() < 7)) {
								int choice = (rand() % 20);
								if ((choice >= 0) && (choice < 9)) {
									chosenType = BATTLE;
								}
								else if ((choice >= 9) && (choice < 13)) {
									chosenType = EVENT;
								}
								else if ((choice >= 13) && (choice < 16)) {
									chosenType = ELITE;
								}
								else if ((choice >= 16) && (choice < 18)) {
									chosenType = SCRAP;
								}
								else if ((choice >= 18) && (choice < 19)) {
									chosenType = TUTOR;
								}
								else if ((choice >= 19) && (choice < 20)) {
									chosenType = TRADER;
								}
							}
							node->getComponent<NodeController>()->setNodeType(chosenType);
							chosenTypes.push_back(chosenType);
						}
						else {
							//std::cout << "Node at " << node->getPosition().y << ", " << node->getPosition().x  << " is set" << std::endl;
							chosenTypes.push_back(node->getComponent<NodeController>()->getNodeType());
						}
					}
				}
			}
		}
	}

	for (auto node : m_nodes) {
		if (node->isAlive()) {
			//node->addComponent<Text>(new Text(node, m_gameState.font, std::to_string(node->getComponent<NodeController>()->getNodeType()), 1.5f))->setTextAlign(CENTERTEXTALIGN);
			std::string filePath = (std::string)"Assets/NodeIcons/" + (std::string)std::to_string(node->getComponent<NodeController>()->getNodeType()) + ".png";
			node->addComponent<SlicedImageRenderer>(new SlicedImageRenderer(node, Util::loadTexture(filePath.c_str()), 1.0f, 1.0f));
		}
	}

	for (int x = 0; x < m_mapWidth; x++) {
		if (m_nodes[x]->isAlive()) {
			m_startingNodes.push_back(m_nodes[x]);
		}
	}

	m_camera.m_position.y = -10.5f;

	//std::cout << "Finished loading map" << std::endl;

	// Time to load the party?

	// Entities
	m_gameState.gameMode = GAME_LEVEL;
}

void Scene1::load() {
	// handle level select
	if (g_gameGlobal->g_activeNodes.size() == 0) {
		clearScene();
		initialise();
		g_gameGlobal->g_activeNodes = m_startingNodes;
	}
}

void Scene1::unload() {
	switch (m_chosenNode) {
	case EVENT:
	case TRADER:
	case TREASURE:
	case TUTOR:
	case BATTLE:
	case ELITE:
	case BOSS:
	case SCRAP:
		m_nextScene = 2;
		break;
	}
	m_chosenNode = NODENONE;
}

void Scene1::processInput(bool* gameIsRunning) {
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

void Scene1::processInputMainMenu(bool* gameIsRunning) {
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

void Scene1::processInputGameLevel(bool* gameIsRunning) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if ((event.type == SDL_QUIT) || (event.type == SDL_WINDOWEVENT_CLOSE)) {
			*gameIsRunning = false;
		}
		else if ((event.type == SDL_MOUSEWHEEL) && (event.wheel.y != 0)) {
			if ((event.wheel.y > 0) && (m_camera.m_position.y <= 10.5f)) { // scroll up
				//m_camera.m_scale -= 0.1f;
				m_camera.m_position.y += 1.0f;
			}
			else if ((event.wheel.y < 0) && (m_camera.m_position.y >= -10.5f)) { // scroll down
				//m_camera.m_scale += 0.1f;
				m_camera.m_position.y -= 1.0f;
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
			if (event.key.keysym.sym == SDLK_SPACE) {
				clearScene();
				initialise();
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
					if (!hovered) {
						hovered = button->isHovered();
						if (hovered) {
							if (g_gameGlobal->g_sfx.count("button")) {
								Mix_PlayChannel(
									-1,       // using the first channel that is not currently in use...
									g_gameGlobal->g_sfx["button"],  // ...play this chunk of audio...
									0        // ...once.
								);
							}
							button->press();
						}
					}
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

void Scene1::processInputGameOver(bool* gameIsRunning) {
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

void Scene1::update(float deltaTime) {
	//std::cout << deltaTime << std::endl;

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

void Scene1::updateMainMenu(float deltaTime) {

}

void Scene1::updateGameLevel(float deltaTime) {
	for (unsigned int i = 0; i < m_gameState.allGameObjects.size(); i++) {
		GameObject* gameObject = m_gameState.allGameObjects[i];
		if ((gameObject->isAlive()) && (!gameObject->hasTag("UI"))) {
			gameObject->update(deltaTime);
		}
	}

	if (m_chosenNode != NODENONE) {
		unload();
	}

	// Update Camera Scale
	m_camera.m_orthoWidth = ((m_camera.WINDOW_WIDTH / 160.0f) * 1.25f * m_camera.m_scale);
	m_camera.m_orthoHeight = ((m_camera.WINDOW_HEIGHT / 160.0f) * 1.25f * m_camera.m_scale);

	// Camera Zoom
	m_projectionMatrix = glm::ortho(-m_camera.m_orthoWidth / 2.0f, m_camera.m_orthoWidth / 2.0f,
		-m_camera.m_orthoHeight / 2.0f, m_camera.m_orthoHeight / 2.0f,
		-1.0f, 1.0f);

	// Camera Position
	m_viewMatrix = glm::mat4(1.0f);
	m_viewMatrix = glm::translate(m_viewMatrix, glm::vec3(-m_camera.m_position.x, -m_camera.m_position.y, m_camera.m_position.z));

	// UI Update
	for (unsigned int i = 0; i < m_gameState.allGameObjects.size(); i++) {
		GameObject* gameObject = m_gameState.allGameObjects[i];
		if ((gameObject->isAlive()) && (gameObject->hasTag("UI"))) {
			gameObject->update(deltaTime);
		}
	}
}

void Scene1::updateGameOver(float deltaTime) {

}

void Scene1::render(ShaderProgram* program) {
	// Update Camera 
	program->set_projection_matrix(m_projectionMatrix);
	program->set_view_matrix(m_viewMatrix);

	// Load all GameObjects that are alive
	std::vector<GameObject*> sortedRender = { };
	for (auto gameObject : m_gameState.allGameObjects) {
		if (gameObject->isAlive()) {
			if (gameObject->getParent() == nullptr) {
				sortedRender.push_back(gameObject);
			}
		}
	}

	// Sort all GameObjects to Render from top of map first (for illusion of depth)
	for (unsigned int i = 1; i < sortedRender.size(); i++) {
		int j = i;
		while (j > 0 && ((sortedRender[j]->getPosition().z < sortedRender[j - 1]->getPosition().z) || (sortedRender[j]->getPosition().y > sortedRender[j - 1]->getPosition().y))) {
			std::swap(sortedRender[j], sortedRender[j - 1]);
			j--;
		}
	}

	// Load all child GameObjects that are alive
	std::vector<GameObject*> sortedRenderChildren = { };
	for (auto gameObject : sortedRender) {
		std::vector<GameObject*> gameObjectRender = recursiveRenderChildrenSort(gameObject);
		sortedRenderChildren.insert(sortedRenderChildren.end(), gameObjectRender.begin(), gameObjectRender.end());
	}

	// Finally, render everything sorted
	for (auto& renderable : sortedRenderChildren) {
		renderable->render(program);
	}
}

void Scene1::clearScene() {
	for (int i = m_gameState.allGameObjects.size() - 1; i >= 0; i--) {
		if (m_gameState.allGameObjects[i] != nullptr) {
			delete m_gameState.allGameObjects[i];
		}
	}
	m_gameState.allButtons.clear();
	m_nodes.clear();
	m_startingNodes.clear();
	m_gameState.allGameObjects.clear();
}