#include "MouseManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Game * MouseManager::m_game;
InputCommands * MouseManager::m_input;
int MouseManager::m_index;

// Picking a single object
int MouseManager::PickObject(PICK_TYPE type)
{
	// Switch between picking type
	switch (type)
	{
	case PICK_TYPE::ANY: return AnyIntersection(RayTrace(m_input->mousePos)); break;
	case PICK_TYPE::OBJECT: return ObjectIntersection(RayTrace(m_input->mousePos)); break;
	case PICK_TYPE::LIGHT: return LightIntersection(RayTrace(m_input->mousePos)); break;
	}
}

// Picking multiple objects
void MouseManager::PickMultipleObjects(std::vector<int> & selectedIDs, PICK_TYPE type, bool select)
{
	// Setup temp ID
	int temp = PickObject(type);

	// If an object has been selected
	if (temp != -1)
	{
		// If current selected ID is already in the vector
		if (std::count(selectedIDs.begin(), selectedIDs.end(), temp))
		{
			// If isn't selecting
			if (!select)
			{
				// Remove from vector storage
				selectedIDs.erase(std::remove(selectedIDs.begin(), selectedIDs.end(), temp), selectedIDs.end());
			}
		}

		// Else, if current selected ID is new
		else
		{
			// If is selecting
			if (select)
			{
				// Add to vector storage
				selectedIDs.push_back(temp);
			}
		}
	}
}

// Picking a spawn point for an object
DirectX::SimpleMath::Vector3 MouseManager::PickSpawn()
{
	// Setup temp point
	DirectX::SimpleMath::Vector3 pickingPoint;
	
	// Setup ray trace from mouse position
	DirectX::SimpleMath::Ray ray = RayTrace(m_input->mousePos);

	// Setup temp mouse position
	DirectX::SimpleMath::Vector3 mouse = ray.position;

	// Setup default distance
	float distance = 10.f;

	// Setup temp terrain object
	TERRAIN temp = TerrainIntersection(ray);

	// If terrain has been intersected
	if (temp.intersect)
	{
		// Calculate distance between mouse and terrain
		distance = sqrt(
			(mouse.x - temp.position.x) * (mouse.x - temp.position.x) +
			(mouse.y - temp.position.y) * (mouse.y - temp.position.y) +
			(mouse.z - temp.position.z) * (mouse.z - temp.position.z));
	}

	// Setup picking point
	pickingPoint = ray.position + (ray.direction * distance);

	// Increase y position to spawn object just above terrain level
	if (temp.intersect) { pickingPoint.y += 1.f; }

	// Return picking point
	return pickingPoint;
}

// Picking a piece of terrain
TERRAIN MouseManager::PickTerrain()
{
	// Return picked terrain object
	TERRAIN terrain = TerrainIntersection(RayTrace(m_input->mousePos));

	// If terrain has been intersected
	if (terrain.intersect)
	{
		// Search for current terrain paint & apply
		terrain.paint = m_game->GetDisplayChunk()->GetPaint(terrain.row, terrain.column);
	}

	return terrain;
}

// Setup ray trace from given position
DirectX::SimpleMath::Ray MouseManager::RayTrace(DirectX::SimpleMath::Vector2 position)
{
	// Setup ray trace origin
	DirectX::SimpleMath::Vector3 origin = XMVector3Unproject(DirectX::SimpleMath::Vector3(position.x, position.y, 0.f),
		0,
		0,
		m_game->GetDeviceResources()->GetScreenViewport().Width,
		m_game->GetDeviceResources()->GetScreenViewport().Height,
		0,
		1,
		m_game->GetProjViewWorld()[0],
		m_game->GetProjViewWorld()[1],
		m_game->GetProjViewWorld()[2]);

	// Setup ray trace destination
	DirectX::SimpleMath::Vector3 destination = XMVector3Unproject(DirectX::SimpleMath::Vector3(position.x, position.y, 1.f),
		0,
		0,
		m_game->GetDeviceResources()->GetScreenViewport().Width,
		m_game->GetDeviceResources()->GetScreenViewport().Height,
		0,
		1,
		m_game->GetProjViewWorld()[0],
		m_game->GetProjViewWorld()[1],
		m_game->GetProjViewWorld()[2]);

	// Setup ray trace direction
	DirectX::SimpleMath::Vector3 direction = destination - origin;
	direction.Normalize();

	// Return ray trace
	return DirectX::SimpleMath::Ray(origin, direction);
}

// Check if ray trace intersects any object
int MouseManager::AnyIntersection(DirectX::SimpleMath::Ray ray)
{
	// Define controllers
	int ID = -1;
	float pickedDistance = 0.f, storedDistance = 1.f;
	bool firstPick = true;

	// Loop through all objects
	for (int i = 0; i < m_game->GetDisplayList().size(); ++i)
	{		
		// Loop through all meshes
		for (int j = 0; j < m_game->GetDisplayList()[i].m_model->meshes.size(); ++j)
		{
			// If current object intersects ray trace
			if (m_game->GetDisplayList()[i].m_model->meshes[j]->boundingBox.Intersects(ray.position, ray.direction, pickedDistance))
			{
				// If object is first picked
				if (firstPick)
				{
					// Setup ID to return
					ID = m_game->GetDisplayList()[i].m_ID;

					// Store current distance
					storedDistance = pickedDistance;

					// Reset controller
					firstPick = false;

					// Store index
					m_index = i;
				}
				// Else, if a closer object has been intersected
				else if (pickedDistance < storedDistance)
				{
					// Setup ID to return
					ID = m_game->GetDisplayList()[i].m_ID;

					// Store current distance
					storedDistance = pickedDistance;

					// Store index
					m_index = i;
				}
			}
		}		
	}

	// Return intersected object ID
	return ID;
}

// Check if ray trace intersects an object
int MouseManager::ObjectIntersection(DirectX::SimpleMath::Ray ray)
{
	// Define controllers
	int ID = -1;
	float pickedDistance = 0.f, storedDistance = 1.f;
	bool firstPick = true;

	// Loop through all objects
	for (int i = 0; i < m_game->GetDisplayList().size(); ++i)
	{
		// If current object is NOT a light
		if (m_game->GetDisplayList()[i].m_objectType != OBJECT_TYPE::LIGHT)
		{
			// Loop through all meshes
			for (int j = 0; j < m_game->GetDisplayList()[i].m_model->meshes.size(); ++j)
			{
				// If current object intersects ray trace
				if (m_game->GetDisplayList()[i].m_model->meshes[j]->boundingBox.Intersects(ray.position, ray.direction, pickedDistance))
				{
					// If object is first picked
					if (firstPick)
					{
						// Setup ID to return
						ID = m_game->GetDisplayList()[i].m_ID;

						// Store current distance
						storedDistance = pickedDistance;

						// Reset controller
						firstPick = false;

						// Store index
						m_index = i;
					}
					// Else, if a closer object has been intersected
					else if (pickedDistance < storedDistance)
					{
						// Setup ID to return
						ID = m_game->GetDisplayList()[i].m_ID;

						// Store current distance
						storedDistance = pickedDistance;

						// Store index
						m_index = i;
					}
				}
			}
		}
	}

	// Return intersected object ID
	return ID;
}

// Check if ray trace intersects a light
int MouseManager::LightIntersection(DirectX::SimpleMath::Ray ray)
{
	// Define controllers
	int ID = -1;
	float pickedDistance = 0.f, storedDistance = 1.f;
	bool firstPick = true;

	// Loop through all objects
	for (int i = 0; i < m_game->GetDisplayList().size(); ++i)
	{
		// If current object IS a light
		if (m_game->GetDisplayList()[i].m_objectType == OBJECT_TYPE::LIGHT)
		{
			// Loop through all meshes
			for (int j = 0; j < m_game->GetDisplayList()[i].m_model->meshes.size(); ++j)
			{
				// If current object intersects ray trace
				if (m_game->GetDisplayList()[i].m_model->meshes[j]->boundingBox.Intersects(ray.position, ray.direction, pickedDistance))
				{
					// If object is first picked
					if (firstPick)
					{
						// Setup ID to return
						ID = m_game->GetDisplayList()[i].m_ID;

						// Store current distance
						storedDistance = pickedDistance;

						// Reset controller
						firstPick = false;

						// Store index
						m_index = i;
					}
					// Else, if a closer object has been intersected
					else if (pickedDistance < storedDistance)
					{
						// Setup ID to return
						ID = m_game->GetDisplayList()[i].m_ID;

						// Store current distance
						storedDistance = pickedDistance;

						// Store index
						m_index = i;
					}
				}
			}
		}
	}

	// Return intersected object ID
	return ID;
}

// Check if ray trace intersects a piece of terrain
TERRAIN MouseManager::TerrainIntersection(DirectX::SimpleMath::Ray ray)
{
	// Setup temp terrain
	TERRAIN terrain;
	terrain.intersect = false;

	// Define controllers
	float distance = 10000.f, pickedDistance = 0.f, storedDistance = 1.f;
	bool firstPick = true;

	// Loop through terrain row size
	for (int i = 0; i < TERRAINRESOLUTION - 1; ++i)
	{
		// Loop through terrain column size
		for (int j = 0; j < TERRAINRESOLUTION - 1; ++j)
		{
			// Setup local vectors of current geometry corner positions
			DirectX::SimpleMath::Vector3 bottomLeft = m_game->GetDisplayChunk()->GetGeometry(i, j).position;
			DirectX::SimpleMath::Vector3 bottomRight = m_game->GetDisplayChunk()->GetGeometry(i, j + 1).position;
			DirectX::SimpleMath::Vector3 topRight = m_game->GetDisplayChunk()->GetGeometry(i + 1, j + 1).position;
			DirectX::SimpleMath::Vector3 topLeft = m_game->GetDisplayChunk()->GetGeometry(i + 1, j).position;

			// If ray intersects with either triangle in current geometry
			if (ray.Intersects(bottomLeft, bottomRight, topRight, pickedDistance) || ray.Intersects(bottomLeft, topLeft, topRight, pickedDistance))
			{				
				// If terrain is first picked
				if (firstPick)
				{
					// Setup values to return
					terrain.row = i;
					terrain.column = j;
					terrain.intersect = true;
					terrain.position = m_game->GetDisplayChunk()->GetGeometry(i, j).position;

					// Store current distance
					storedDistance = pickedDistance;

					// Reset controller
					firstPick = false;
				}

				// Else, if closer terrain has been intersected
				else if (pickedDistance < storedDistance)
				{
					// Setup values to return
					terrain.row = i;
					terrain.column = j;
					terrain.intersect = true;
					terrain.position = m_game->GetDisplayChunk()->GetGeometry(i, j).position;

					// Store current distance
					storedDistance = distance;
				}
			}
		}
	}

	return terrain;
}
