/**
* Author: John Li
* Assignment: Rise of the AI
* Date due: 2024-03-30, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Level.h"

void Level::initialize(std::vector<std::vector<GameMap>>& allLevelData) {

	//----- Map 0 -----
	allLevelData.push_back({});
	/*
	// Water Layer 0 // Just set the background color to the water color...
	allLevelData[0].push_back(GameMap(false, 20, 11,
		0, {}, 0,
		{   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			}, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Water/Water.png"), 1.0f, 1, 1, {}));
	*/
	// Collision Layer
	allLevelData[0].push_back(GameMap(true, true, 0.0f, 20, 12,
		0, {}, 0,
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		}, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Water/Water.png"), 1.0f, 1, 1, {}));
	//Tile Edge Foam
	allLevelData[0].push_back(GameMap(false, false, 0.0f, 20, 12,
		0,
		{ { 0, { 0,  3,  6,  9, 12, 15, 18, 21, }},
		   { 1, { 1,  4,  7, 10, 13, 16, 19, 22, }},
		   { 2, { 2,  5,  8, 11, 14, 17, 20, 23, }},
		   {24, {24, 27, 30, 33, 36, 39, 42, 45, }},
		   {25, {25, 28, 31, 34, 37, 40, 43, 46, }},
		   {26, {26, 29, 32, 35, 38, 41, 44, 47, }},
		   {48, {48, 51, 54, 57, 60, 63, 66, 69, }},
		   {49, {49, 52, 55, 58, 61, 64, 67, 70, }},
		   {50, {50, 53, 56, 59, 62, 65, 68, 71, }},

		}, 0,
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		}, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Water/Foam/Foam.png"), 1.0f, 24, 3,
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
	// Elevation Wall
	allLevelData[0].push_back(GameMap(false, false, 0.0f, 20, 12,
		0, {}, 0,
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

		}, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Ground/Tilemap_Elevation.png"), 1.0f, 4, 8,
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
	// Elevation Ground
	allLevelData[0].push_back(GameMap(false, false, 1.0f, 20, 12,
		0, {}, 0,
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

		}, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Ground/Tilemap_Elevation.png"), 1.0f, 4, 8,
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

	// Elevation Grass Green
	allLevelData[0].push_back(GameMap(false, false, 1.0f, 20, 12,
		0, {}, 0,
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

		}, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Ground/Tilemap_Flat.png"), 1.0f, 10, 4,
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

	//----- Map 1 -----
	allLevelData.push_back({});
	// Collision Layer
	allLevelData[1].push_back(GameMap(true, true, 0.0f, 29, 15,
		0, {}, 0,
		Util::loadLevelData("Assets/Maps/Level 1/Collision 0_1.png"),
		Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Water/Water.png"), 1.0f, 1, 1, {}));
	// Pits 0
	allLevelData[1].push_back(GameMap(false, true, 0.0f, 29, 15,
		0, {}, 0,
		Util::loadLevelData("Assets/Maps/Level 1/Pits 0_1.png"),
		Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Water/Water.png"), 1.0f, 1, 1, {}));
	// Pits 1
	allLevelData[1].push_back(GameMap(false, true, 1.0f, 29, 15,
		0, {}, 0,
		Util::loadLevelData("Assets/Maps/Level 1/Pits 1_1.png"),
		Util::loadTexture("Assets/Empty.png"), 1.0f, 1, 1, {}));
	//Tile Edge Foam
	allLevelData[1].push_back(GameMap(false, false, 0.0f, 29, 15,
		0,
		{ { 0, { 0,  3,  6,  9, 12, 15, 18, 21, }},
		   { 1, { 1,  4,  7, 10, 13, 16, 19, 22, }},
		   { 2, { 2,  5,  8, 11, 14, 17, 20, 23, }},
		   {24, {24, 27, 30, 33, 36, 39, 42, 45, }},
		   {25, {25, 28, 31, 34, 37, 40, 43, 46, }},
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
	// Elevation Wall
	allLevelData[1].push_back(GameMap(false, false, 0.0f, 29, 15,
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
	// Elevation Ground
	allLevelData[1].push_back(GameMap(false, false, 1.0f, 29, 15,
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

	// Elevation Grass Green
	allLevelData[1].push_back(GameMap(false, false, 1.0f, 29, 15,
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
	// Sand
	allLevelData[1].push_back(GameMap(false, false, 1.0f, 29, 15,
		0, {}, 0,
		Util::loadLevelData("Assets/Maps/Level 1/Sand 1_1.png"),
		Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Ground/Tilemap_Flat.png"), 1.0f, 10, 4,
		{   {"TL",  5 }, // Top Left
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

	//----- Map 2 -----
	allLevelData.push_back({});
	// Collision Layer
	allLevelData[2].push_back(GameMap(true, true, 0.0f, 51, 11,
		0, {}, 0,
		Util::loadLevelData("Assets/Maps/Level 2/Collision 0_1.png"),
		Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Water/Water.png"), 1.0f, 1, 1, {}));
	//Tile Edge Foam
	allLevelData[2].push_back(GameMap(false, false, 0.0f, 51, 11,
		0,
		{  { 0, { 0,  3,  6,  9, 12, 15, 18, 21, }},
		   { 1, { 1,  4,  7, 10, 13, 16, 19, 22, }},
		   { 2, { 2,  5,  8, 11, 14, 17, 20, 23, }},
		   {24, {24, 27, 30, 33, 36, 39, 42, 45, }},
		   {25, {31,}},//25, 28, 31, 34, 37, 40, 43, 46, }},
		   {26, {26, 29, 32, 35, 38, 41, 44, 47, }},
		   {48, {48, 51, 54, 57, 60, 63, 66, 69, }},
		   {49, {49, 52, 55, 58, 61, 64, 67, 70, }},
		   {50, {50, 53, 56, 59, 62, 65, 68, 71, }},

		}, 0,
		Util::loadLevelData("Assets/Maps/Level 2/Foam 0_1.png"),
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
	// Sand
	allLevelData[2].push_back(GameMap(false, false, 0.0f, 51, 11,
		0, {}, 0,
		Util::loadLevelData("Assets/Maps/Level 2/Sand 0_1.png"),
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
}