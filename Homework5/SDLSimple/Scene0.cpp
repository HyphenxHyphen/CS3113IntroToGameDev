#include "Scene0.h"

void Scene0::initialise() {
	g_gameGlobal->playerLives = 3;
	m_gameState.victory = false;

	for (auto gameObject : m_gameState.allGameObjects) {
		delete gameObject;
	}
	m_gameState.allGameObjects.clear();

	srand((unsigned int)time(NULL)); // Reset Random

	// Initialize Font
	m_gameState.font.m_textureID = Util::loadTexture("Assets/FontAtlas3.png");
	m_gameState.font.m_cols = 5;
	m_gameState.font.m_rows = 9;
}

void Scene0::processInput(bool* gameIsRunning) {
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
			if (event.key.keysym.sym == SDLK_RETURN) {
				switchScene(1);
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
	m_projectionMatrix = glm::ortho(-((WINDOW_WIDTH / 320.0f) * 1.25f * cameraScale), ((WINDOW_WIDTH / 320.0f) * 1.25f * cameraScale),
		-((WINDOW_HEIGHT / 320.0f) * 1.25f * cameraScale), ((WINDOW_HEIGHT / 320.0f) * 1.25f * cameraScale),
		-1.0f, 1.0f);
	program->set_projection_matrix(m_projectionMatrix);
	program->set_view_matrix(m_viewMatrix);

	m_viewMatrix = glm::mat4(1.0f);
	m_viewMatrix = glm::translate(m_viewMatrix, glm::vec3(-m_gameState.cameraPos.x, -m_gameState.cameraPos.y, m_gameState.cameraPos.z));

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
	Util::drawText(program, "PROJECT GO:", glm::vec3(0.0f, 0.75f, 0.0f) + m_gameState.cameraPos, 2.0f, m_gameState.font.m_cols, m_gameState.font.m_rows, m_gameState.font.m_textureID);
	Util::drawText(program, "PLATFORMER", glm::vec3(0.0f, -0.25f, 0.0f) + m_gameState.cameraPos, 3.0f, m_gameState.font.m_cols, m_gameState.font.m_rows, m_gameState.font.m_textureID);
	Util::drawText(program, "<THE JOKE IS GO IS 5 IN JAPANESE>", glm::vec3(0.0f, 0.25f, 0.0f) + m_gameState.cameraPos, 0.75f, m_gameState.font.m_cols, m_gameState.font.m_rows, m_gameState.font.m_textureID);

	if (((int)seconds % 2) == 0) {
		Util::drawText(program, "PRESS ENTER", glm::vec3(0.0f, -1.25f, 0.0f) + m_gameState.cameraPos, 1.5f, m_gameState.font.m_cols, m_gameState.font.m_rows, m_gameState.font.m_textureID);
	}

	glUseProgram(program->get_program_id());
}

void Scene0::clearScene() {
	for (auto gameObject : m_gameState.allGameObjects) {
		delete gameObject;
	}
	m_gameState.allGameObjects.clear();
}