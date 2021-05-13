#pragma once

#include "Vector.h"

class LevelGenerator
{

public:
	LevelGenerator(const LevelGenerator& other) = delete;
	LevelGenerator& operator= (const LevelGenerator& other) = delete;
	LevelGenerator(LevelGenerator&& other) = delete;
	LevelGenerator& operator= (LevelGenerator&& other) = delete;

	static void LoadLevel(const std::string& readLevelTxt, const std::string & writeLevelJson = _levelJsonFile);

private:
	LevelGenerator() = default;
	~LevelGenerator() = default;

	static void InsertPhysicsEntity(std::ofstream& outputFileStream, const std::string& entityName, const std::string& entityTag, const std::string& entityTexture, const float& positionX, const float& positionY);
	static void InsertEntity(std::ofstream& outputFileStream, const std::string& entityName, const std::string& entityTag, const float& positionX, const float& positionY);

	static const inline std::string& _levelJsonFile = "LevelTiles.json";
	static const inline std::string& _blockTexture = "BlockEnd.png";
	static const inline std::string& _spawnPointTexture = "PaulOnlineWhole.png";

	static const inline std::string& _wallTexture = "WallMiddle.png";

	/* Json format for writing the level Json at runtime to be read for generating level */
	static const inline std::string& _initJsonFile = "{\n\t\"type\": \"table\",\n\t\"value\": [\n";
	static const inline std::string& _finishJsonFile = "\n\t]\n}";
	static const inline std::string& _initJsonSectionPhysicsEntity = "\t\t{\n\t\t\t\"type\": \"table\",\n\t\t\t\"class\": \"PhysicsEntity\",\n\t\t\t\"value\": {\n";
	static const inline std::string& _initJsonSectionEntity = "\t\t{\n\t\t\t\"type\": \"table\",\n\t\t\t\"class\": \"Entity\",\n\t\t\t\"value\": {\n";
	static const inline std::string& _finishJsonSection = "\t\t\t}\n\t\t}";


	static const inline std::string& _nameJsonSection = "\t\t\t\t\"Name\": {\n\t\t\t\t\t\"type\": \"string\",\n\t\t\t\t\t\"value\" : \"%s\"\n\t\t\t\t},\n";
	static const inline std::string& _tagJsonSection = "\t\t\t\t\"Tag\": {\n\t\t\t\t\t\"type\": \"string\",\n\t\t\t\t\t\"value\" : \"%s\"\n\t\t\t\t},\n";
	static const inline std::string& _positionJsonSection = "\t\t\t\t\"Position\": {\n\t\t\t\t\t\"type\": \"vector\",\n\t\t\t\t\t\"value\" : \"vec4(%.1f, %.1f, 0.0, 0.0)\"\n\t\t\t\t},\n";
	static const inline std::string& _scaleJsonSection = "\t\t\t\t\"Scale\": {\n\t\t\t\t\t\"type\": \"vector\",\n\t\t\t\t\t\"value\" : \"vec4(%.1f, %.1f, 0.0, 0.0)\"\n\t\t\t\t},\n";
	static const inline std::string& _textureJsonSection = "\t\t\t\t\"Texture\": {\n\t\t\t\t\t\"type\": \"string\",\n\t\t\t\t\t\"value\" : \"%s\"\n\t\t\t\t},\n";
	static const inline std::string& _bodyJsonSection = "\t\t\t\t\"BodyType\": {\n\t\t\t\t\t\"type\": \"string\",\n\t\t\t\t\t\"value\" : \"static\"\n\t\t\t\t}\n";

	/* Modify only these values to be specific for your game */
	static const inline float & _blockScale = 6.0f;			/* dimensions for one block in our game, blockScale X blockScale*/
	static const inline float & _xUnitsOfScale = 160.0f;	/* amount of units in a row, XUnitsOfScale/blockScale is how many elements can go along the X*/
	static const inline float & _yUnitsOfScale = 90.0f;		/* amount of units in a column, YUnitsOfScale/blockScale is how many elements can go along the Y*/

	/* Do not modify these, they should scale properly based on the values assigned above */
	static const inline float leftMostCol = (_blockScale / 2.0f) - (_xUnitsOfScale /2.0f);		//left most side in our game where 0,0 is center in openGL setup
	static const inline float topRow = (_yUnitsOfScale / 2.0f) - (_blockScale / 2.0f);		//top row in our game where 0,0 is center in openGL setup
};

