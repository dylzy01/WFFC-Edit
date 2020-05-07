#pragma once
#include "sqlite3.h"
#include "SceneObject.h"
#include "ChunkObject.h"
#include "Tools.h"

#include <vector>
#include <sstream>

class SQLManager
{
public:
	SQLManager() {}
	~SQLManager() {}

	// Connect to database
	static bool Connect() { return sqlite3_open_v2("database/test.db", &m_databaseConnection, SQLITE_OPEN_READWRITE, NULL); }

	// Disconnect from database
	static bool Disconnect() { return sqlite3_close(m_databaseConnection); }

	// Send query to database
	static bool SendQuery(char* query, bool isObject);

	// Create object from object table
	static SceneObject CreateObject();

	// Create chunk from chunk table
	static ChunkObject CreateChunk();

	// Add new object to object table
	static bool AddObject(SceneObject object);

	// Remove an object from object table
	static bool RemoveObject(SceneObject object);

	// Save updated scene graph
	static bool SaveObjects(std::vector<SceneObject> sceneGraph);

	// Load a different height map
	static ChunkObject LoadHeightMap(ChunkObject chunk, std::string path);

	// Save updated chunk
	static bool SaveChunk(ChunkObject chunk);

	// Load chunk
	static ChunkObject LoadChunk(std::string name);

	// Getters
	static int GetObjectStep() { return sqlite3_step(m_resultObject); }
	static int GetChunkStep() { return sqlite3_step(m_resultChunk); }
	static std::vector<std::string> GetChunkNames();

	// Setters
	static void SetObjectStep() { sqlite3_step(m_resultObject); }
	static void SetChunkStep() { sqlite3_step(m_resultChunk); }

private:
	static sqlite3 *m_databaseConnection;
	static sqlite3_stmt *m_resultObject, *m_resultChunk;
};

