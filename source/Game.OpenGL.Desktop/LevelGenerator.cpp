#include "pch.h"
#include "LevelGenerator.h"
#include <fstream>
#include <string>

void LevelGenerator::LoadLevel(const std::string & readLevelTxt, const std::string& writeLevelJson)
{
	std::ifstream inputFileStream;
	std::ofstream outputFileStream;
	std::string currentLine;
	float currentRow = topRow;	
	float currentCol = leftMostCol;	
	bool firstObject = true;

	inputFileStream.open(readLevelTxt);
	outputFileStream.open(writeLevelJson);

	if (!inputFileStream.is_open())
	{
		throw std::runtime_error("Cannot read passed in level txt file");
	}
	if (!outputFileStream.is_open())
	{
		throw std::runtime_error("Cannot write to level tile json");
	}

	outputFileStream << _initJsonFile;
	while (getline(inputFileStream, currentLine))
	{
		for (char& c : currentLine)
		{
			//only insert "," and "\n" after first object, will not reach here after reading last element of file, avoids last comma
			if (!firstObject && c != '_')
			{
				outputFileStream << ",\n";
			}
			else
			{
				firstObject = false;
			}

			switch (c)
			{
			case '_':
				/* Do nothing as _ denotes empty space */
				break;
			case '#':
				InsertPhysicsEntity(outputFileStream, "Test Ground", "LevelGeometry", _blockTexture, currentCol, currentRow);
				break;
			case '$':
				InsertPhysicsEntity(outputFileStream, "Respawn", "RespawnZone", _spawnPointTexture, currentCol, currentRow);
				break;
			case '|':
				InsertPhysicsEntity(outputFileStream, "Wall", "LevelWall", _wallTexture, currentCol, currentRow);
				break;
			}

			currentCol += _blockScale;
		}
		currentCol = leftMostCol;
		currentRow -= _blockScale;
	}
	outputFileStream << _finishJsonFile;

	inputFileStream.close();
	outputFileStream.close();
}

void LevelGenerator::InsertPhysicsEntity(std::ofstream & outputFileStream, const std::string & entityName, const std::string& entityTag, const std::string& entityTexture, const float & positionX, const float & positionY)
{
	const size_t bufferSize = 1000;
	char entityNameJsonSection[bufferSize];
	char entityTagJsonSection[bufferSize];
	char entityPositionJsonSection[bufferSize];
	char entityScaleJsonSection[bufferSize];
	char entityTextureJsonSection[bufferSize];

	snprintf(entityNameJsonSection, bufferSize, _nameJsonSection.c_str(), entityName.c_str());
	snprintf(entityTagJsonSection, bufferSize, _tagJsonSection.c_str(), entityTag.c_str());
	snprintf(entityPositionJsonSection, bufferSize, _positionJsonSection.c_str(), positionX, positionY);
	if (entityTag.compare("RespawnZone") == 0)
	{
		snprintf(entityScaleJsonSection, bufferSize, _scaleJsonSection.c_str(), _blockScale*3.0, _blockScale);
	}
	else
	{
		snprintf(entityScaleJsonSection, bufferSize, _scaleJsonSection.c_str(), _blockScale, _blockScale);
	}
	snprintf(entityTextureJsonSection, bufferSize, _textureJsonSection.c_str(), entityTexture.c_str());

	outputFileStream
		<< _initJsonSectionPhysicsEntity
		<< entityNameJsonSection
		<< entityTagJsonSection
		<< entityPositionJsonSection
		<< entityScaleJsonSection
		<< entityTextureJsonSection
		<< _bodyJsonSection
		<< _finishJsonSection;
}

void LevelGenerator::InsertEntity(std::ofstream& outputFileStream, const std::string& entityName, const std::string& entityTag, const float& positionX, const float& positionY)
{
	const size_t bufferSize = 1000;
	char entityNameJsonSection[bufferSize];
	char entityTagJsonSection[bufferSize];
	char entityPositionJsonSection[bufferSize];
	char entityScaleJsonSection[bufferSize];

	snprintf(entityNameJsonSection, bufferSize, _nameJsonSection.c_str(), entityName.c_str());
	snprintf(entityTagJsonSection, bufferSize, _tagJsonSection.c_str(), entityTag.c_str());
	snprintf(entityPositionJsonSection, bufferSize, _positionJsonSection.c_str(), positionX, positionY);
	snprintf(entityScaleJsonSection, bufferSize, _scaleJsonSection.c_str(), _blockScale, _blockScale);

	outputFileStream
		<< _initJsonSectionEntity
		<< entityNameJsonSection
		<< entityTagJsonSection
		<< entityPositionJsonSection
		<< entityScaleJsonSection
		<< _bodyJsonSection
		<< _finishJsonSection;
}