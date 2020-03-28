#pragma once
#include "sqlite3.h"
#include "SceneObject.h"
#include "ChunkObject.h"
#include "Tools.h"

#include <vector>
#include <sstream>

class SQL
{
public:
	SQL() {}
	~SQL() {}

	// Connect to database
	static bool Connect() { return sqlite3_open_v2("database/test.db", &m_databaseConnection, SQLITE_OPEN_READWRITE, NULL); }

	// Disconnect from database
	static bool Disconnect() { return sqlite3_close(m_databaseConnection); }

	// Send query to database
	static void SendQuery(char* query, bool isObject);

	// Create object from object table
	static SceneObject CreateObject();

	// Create chunk from chunk table
	static ChunkObject CreateChunk();

	// Save updated scene graph
	static bool SaveWorld(std::vector<SceneObject> sceneGraph);

	// Getters
	static int GetObjectStep() { return sqlite3_step(m_resultObject); }

	// Setters
	static void SetObjectStep() { sqlite3_step(m_resultObject); }

private:
	static sqlite3 *m_databaseConnection;
	static sqlite3_stmt *m_resultObject, *m_resultChunk;
};

