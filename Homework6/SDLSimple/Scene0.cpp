#include "Scene0.h"

void Scene0::initialise() {
	m_gameState.victory = false;
	srand((unsigned int)time(NULL)); // Reset Random

	// Initialize Font
	m_gameState.font.m_textureID = Util::loadTexture("Assets/FontAtlas4c.png");
	m_gameState.font.m_cols = 16;
	m_gameState.font.m_rows = 4;

	GameObject* canvasGO = new GameObject("canvasGO", &m_gameState.allGameObjects);
	canvasGO->addTag("UI");

	GameObject* titleBGGO = new GameObject("titleBGGO", &m_gameState.allGameObjects);
	titleBGGO->addTag("UI");
	canvasGO->addChild(titleBGGO);
	titleBGGO->addComponent<SlicedImageRenderer>(
		new SlicedImageRenderer(titleBGGO,
			Util::loadTexture("Assets/Silverframe.png"), 13.0f / 3.5f, 2.0f / 3.5f));
	titleBGGO->setScaleRelative({ 3.5f, 3.5f, 1.0f });
	titleBGGO->setPositionRelative({ 0.0f, 3.0f, 0.0f });

	GameObject* titleTextGO = new GameObject("titleTextGO", &m_gameState.allGameObjects);
	titleTextGO->addTag("UI");
	Text* titleText = titleTextGO->addComponent<Text>(new Text(titleTextGO, m_gameState.font, "PROJECT LEAFCUTTER", 2.5f));
	titleText->setTextAlign(CENTERTEXTALIGN);
	titleText->setColor({ 79, 255, 129 });
	titleBGGO->addChild(titleTextGO);

	GameObject* start1BGGO = new GameObject("start1BGGO", &m_gameState.allGameObjects);
	start1BGGO->addTag("UI");
	canvasGO->addChild(start1BGGO);
	SlicedImageRenderer* startImg1 = start1BGGO->addComponent<SlicedImageRenderer>(
		new SlicedImageRenderer(start1BGGO,
			Util::loadTexture("Assets/Silverframe.png"), 7.5f/1.5f, 1.0f/1.5f));
	Text* start1Text = start1BGGO->addComponent<Text>(new Text(start1BGGO, m_gameState.font, "START - FIRE", 1.5f));
	start1Text->setTextAlign(CENTERTEXTALIGN);
	start1Text->setColor({ 255, 42, 0 });
	StartGameButton* startGameButton1 = start1BGGO->addComponent<StartGameButton>(new StartGameButton(start1BGGO, &m_gameState.allButtons, startImg1, &m_camera, &g_gameGlobal->g_ants, start1Text, 1, { 255, 42, 0 }, { 255, 255, 255 }, &m_nextScene));
	start1BGGO->setScaleRelative({ 1.5f, 1.5f, 1.0f });
	start1BGGO->setPositionRelative({ 0.0f, 0.0f, 0.0f });

	GameObject* start2BGGO = new GameObject("start2BGGO", &m_gameState.allGameObjects);
	start2BGGO->addTag("UI");
	canvasGO->addChild(start2BGGO);
	SlicedImageRenderer* startImg2 = start2BGGO->addComponent<SlicedImageRenderer>(
		new SlicedImageRenderer(start2BGGO,
			Util::loadTexture("Assets/Silverframe.png"), 7.5f / 1.5f, 1.0f / 1.5f));
	Text* start2Text = start2BGGO->addComponent<Text>(new Text(start2BGGO, m_gameState.font, "START - WATER", 1.5f));
	start2Text->setTextAlign(CENTERTEXTALIGN);
	start2Text->setColor({ 0, 174, 255 });
	StartGameButton* startGameButton2 = start2BGGO->addComponent<StartGameButton>(new StartGameButton(start2BGGO, &m_gameState.allButtons, startImg2, &m_camera, &g_gameGlobal->g_ants, start2Text, 2, { 0, 174, 255 }, { 255, 255, 255 }, &m_nextScene));
	start2BGGO->setScaleRelative({ 1.5f, 1.5f, 1.0f });
	start2BGGO->setPositionRelative({ 0.0f, -1.5f, 0.0f });

	GameObject* start3BGGO = new GameObject("start3BGGO", &m_gameState.allGameObjects);
	start3BGGO->addTag("UI");
	canvasGO->addChild(start3BGGO);
	SlicedImageRenderer* startImg3 = start3BGGO->addComponent<SlicedImageRenderer>(
		new SlicedImageRenderer(start3BGGO,
			Util::loadTexture("Assets/Silverframe.png"), 7.5f / 1.5f, 1.0f / 1.5f));
	Text* start3Text = start3BGGO->addComponent<Text>(new Text(start3BGGO, m_gameState.font, "START - GRASS", 1.5f));
	start3Text->setTextAlign(CENTERTEXTALIGN);
	start3Text->setColor({ 43, 184, 28 });
	StartGameButton* startGameButton3 = start3BGGO->addComponent<StartGameButton>(new StartGameButton(start3BGGO, &m_gameState.allButtons, startImg3, &m_camera, &g_gameGlobal->g_ants, start3Text, 0, { 43, 184, 28 }, { 255, 255, 255 }, &m_nextScene));
	start3BGGO->setScaleRelative({ 1.5f, 1.5f, 1.0f });
	start3BGGO->setPositionRelative({ 0.0f, -3.0f, 0.0f });

	//spdDisplayTextGO->setPositionRelative({ -bg->getWidth() / 2.0f + 0.1f, -0.4f, 0.0f });
}

void Scene0::processInput(bool* gameIsRunning) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if ((event.type == SDL_QUIT) || (event.type == SDL_WINDOWEVENT_CLOSE)) {
			*gameIsRunning = false;
		}
		else if ((event.type == SDL_MOUSEWHEEL) && (event.wheel.y != 0)) {
			if (event.wheel.y > 0) { // scroll up
				m_camera.m_scale -= 0.1f;
			}
			else if (event.wheel.y < 0) { // scroll down
				m_camera.m_scale += 0.1f;
			}
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_RETURN) {
				switchScene(1);
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

void Scene0::update(float deltaTime) {
	//std::cout << (m_gameState.allGameObjects.size()) << std::endl;

	for (unsigned int i = 0; i < m_gameState.allGameObjects.size(); i++) {
		GameObject* gameObject = m_gameState.allGameObjects[i];
		if (gameObject->isAlive()) {
			gameObject->update(deltaTime);
		}
	}

	seconds += deltaTime;
}

void Scene0::render(ShaderProgram* program) {
	// Move Camera
	m_projectionMatrix = glm::ortho(-((m_camera.WINDOW_WIDTH / 320.0f) * 1.25f * m_camera.m_scale), ((m_camera.WINDOW_WIDTH / 320.0f) * 1.25f * m_camera.m_scale),
		-((m_camera.WINDOW_HEIGHT / 320.0f) * 1.25f * m_camera.m_scale), ((m_camera.WINDOW_HEIGHT / 320.0f) * 1.25f * m_camera.m_scale),
		-1.0f, 1.0f);
	program->set_projection_matrix(m_projectionMatrix);
	program->set_view_matrix(m_viewMatrix);

	m_viewMatrix = glm::mat4(1.0f);
	m_viewMatrix = glm::translate(m_viewMatrix, glm::vec3(-m_camera.m_position.x, -m_camera.m_position.y, m_camera.m_position.z));

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

	glUseProgram(program->get_program_id());
}

void Scene0::clearScene() {
	for (int i = m_gameState.allGameObjects.size() - 1; i >= 0; i--) {
		delete m_gameState.allGameObjects[i];
	}
	m_gameState.allGameObjects.clear();
}

void StartGameButton::update(float deltaTime) {
	if (isHovered()) {
		m_text->setColor(m_hoveredColor);
	}
	else {
		m_text->setColor(m_normalColor);
	}
}

void StartGameButton::press() {
	if (g_ants->size() == 0) {
		GameObject* ant = new GameObject("ant", g_ants);
		ant->addComponent<EntityController>(new EntityController(ant, m_antId, true, nullptr, nullptr, nullptr, nullptr, nullptr));
	}
	*m_nextScene = 1;
}