#include "Scene1.h"

void Scene1::initialise() {
	m_gameState.victory = false;

	for (auto gameObject : m_gameState.allGameObjects)
	{
		delete gameObject;
	}
	m_gameState.allGameObjects.clear();

	srand(time(NULL)); // Reset Random

	// Initialize Objects (models and assets)


	// Reference
	/*
	GameObject* reference = new GameObject("Reference", &m_gameState.allGameObjects);
	m_gameState.allGameObjects.push_back(reference);
	reference->addComponent<SpriteRenderer>(
		new SpriteRenderer(reference, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Factions/Knights/Troops/Warrior/Blue/Warrior_Blue.png"), 6, 8, 0,
			{ { 0,  1,  2,  3,  4,  5},
				{ 6,  7,  8,  9, 10, 11},
				{12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23},
				{24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35},
				{36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
			},
			0, 12));

	reference->setPosition(glm::vec3(3.5f, 0.0f, 1.0f));
	//reference->update(FIXED_TIMESTEP);
	reference->addComponent<Rigidbody2D>();
	reference->addComponent<PolygonCollider2D>(
		new PolygonCollider2D(reference,
			{ {-0.5f,  0.5f, 0.0f},
				{ 0.5f,  0.5f, 0.0f},
				{ 0.5f, -0.5f, 0.0f},
				{-0.5f, -0.5f, 0.0f}, }))->setColliderScale({ 0.4f, 0.4f, 1.0f });
	*/

	// Elevation Ground
	GameObject* tileMapEG1 = new GameObject("TileMapElevationGround", &m_gameState.allGameObjects);
	m_gameState.allGameObjects.push_back(tileMapEG1);
	tileMapEG1->addComponent<TileMap>(
		new TileMap(tileMapEG1, true, 29, 15,
			0, {}, 0,
			Util::loadLevelData("Assets/Maps/Level 1/Ground 1_1.png"),
			Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Ground/Tilemap_Elevation.png"), 1.0f, 4, 8,
			{ {"TL",  0 }, // Top Left
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
	tileMapEG1->setPosition(Z, 1.0f); //Use this to set z = elevation only


	// Player
	GameObject* player = new GameObject("Player", &m_gameState.allGameObjects);
	m_gameState.allGameObjects.push_back(player);
	player->addComponent<SpriteRenderer>(
		new SpriteRenderer(player, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Factions/Knights/Troops/Warrior/Blue/Warrior_Blue.png"), 6, 8, 0,
			{ { 0,  1,  2,  3,  4,  5},
				{ 6,  7,  8,  9, 10, 11},
				{12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23},
				{24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35},
				{36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
			},
			0, 12));
	player->addComponent<Rigidbody2D>();
	player->setPosition(glm::vec3(3.5f, 1.5f, 1.0f));
	//player->update(FIXED_TIMESTEP);

	player->addComponent<PolygonCollider2D>(
		new PolygonCollider2D(player,
			{ {-0.5f,  0.5f, 0.0f},
				{ 0.5f,  0.5f, 0.0f},
				{ 0.5f, -0.5f, 0.0f},
				{-0.5f, -0.5f, 0.0f}, }))->setColliderScale({ 0.4f, 0.4f, 1.0f });
	//player->addComponent<CircleCollider2D>(new CircleCollider2D(player, { 1.0f, 1.0f, 1.0f }))->setColliderScale({ 0.25f, 0.25f, 0.25f });
	player->addComponent<PlayerMovementController>()->setMoveSpeed(2.0f);

	//m_gameState.gameMode = GAME_LEVEL;
}

void Scene1::update(float deltaTime)
{
	for (auto gameObject : m_gameState.allGameObjects) {
		gameObject->update(deltaTime);
	}
}

void Scene1::render(ShaderProgram* program)
{
	if (GameObject * player = m_gameState.findGameObject("Player")) {
		//std::cout << "Player alive? " << m_gameState.allEntities["Player"].getPosition().x << ", " << m_gameState.allEntities["Player"].getPosition().y << std::endl;
		m_gameState.cameraPos = glm::vec3(player->getPosition().x, player->getPosition().y, 0.0f);
		m_viewMatrix = glm::mat4(1.0f);
		m_viewMatrix = glm::translate(m_viewMatrix, glm::vec3(-m_gameState.cameraPos.x, -m_gameState.cameraPos.y, m_gameState.cameraPos.z));

		m_projectionMatrix = glm::ortho(-((WINDOW_WIDTH / 320.0f) * 1.25f * cameraScale), ((WINDOW_WIDTH / 320.0f) * 1.25f * cameraScale),
			-((WINDOW_HEIGHT / 320.0f) * 1.25f * cameraScale), ((WINDOW_HEIGHT / 320.0f) * 1.25f * cameraScale),
			-1.0f, 1.0f);
		program->set_projection_matrix(m_projectionMatrix);
		program->set_view_matrix(m_viewMatrix);
	}

	// Sort allEntities to Render from top of map first (for illusion of depth)
	std::vector<GameObject*> sortedRender = { };
	for (auto gameObject : m_gameState.allGameObjects) {
		sortedRender.push_back(gameObject);
	}

	// Does map render over entities?
	for (int i = 1; i < sortedRender.size(); i++) {
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
}