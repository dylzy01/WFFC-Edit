#include "sMouse.h"

Game * sMouse::m_game;
InputCommands * sMouse::m_input;

bool sMouse::PickSpawn(DirectX::SimpleMath::Vector3 & pickingPoint)
{
	// Return value
	bool check = false;

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

		// Set pick to true
		check = true;
	}

	// Setup picking point
	pickingPoint = ray.position + (ray.direction * distance);

	// Increase y position to spawn object just above terrain level
	if (temp.intersect) { pickingPoint.y += 1.f; }

	// Return if intersected
	return check;
}

// Setup a ray trace from given position
DirectX::SimpleMath::Ray sMouse::RayTrace(DirectX::SimpleMath::Vector2 position)
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

// Check if ray trace intersects a piece of terrain
TERRAIN sMouse::TerrainIntersection(DirectX::SimpleMath::Ray ray)
{
	// Setup temp display chunk
	DisplayChunk* chunk = m_game->GetDisplayChunk();

	// Setup temp terrain
	TERRAIN terrain;
	terrain.intersect = false;

	// Define controllers
	float distance = 10000.f, pickedDistance = 0.f, storedDistance = 1.f;
	bool firstPick = true;
	DirectX::SimpleMath::Vector3 one = ray.position;
	DirectX::SimpleMath::Vector3 two = ray.position + (ray.direction * distance);

	// Loop through terrain row size
	for (int i = 0; i < TERRAINRESOLUTION - 1; ++i)
	{
		// Loop through terrain column size
		for (int j = 0; j < TERRAINRESOLUTION - 1; ++j)
		{
			// Setup local vectors of current geometry corner positions
			DirectX::SimpleMath::Vector3 bottomLeft = chunk->GetGeometry(i, j).position;
			DirectX::SimpleMath::Vector3 bottomRight = chunk->GetGeometry(i, j + 1).position;
			DirectX::SimpleMath::Vector3 topRight = chunk->GetGeometry(i + 1, j + 1).position;
			DirectX::SimpleMath::Vector3 topLeft = chunk->GetGeometry(i + 1, j).position;

			// If ray intersects with either triangle in current geometry
			if (ray.Intersects(bottomLeft, bottomRight, topRight, distance) || ray.Intersects(bottomLeft, topLeft, topRight, distance))
			{
				// If current geometry is within ray trace bounds
				///if (m_game->GetDisplayChunk()->GetGeometry(i, j).position.y < one.y && m_game->GetDisplayChunk()->GetGeometry(i, j).position.y > two.y)
				if (chunk->GetGeometry(i, j).position.y < one.y && chunk->GetGeometry(i, j).position.y > two.y)
				{
					// If terrain is first picked
					if (firstPick)
					{
						// Setup values to return
						terrain.row = i;
						terrain.column = j;
						terrain.intersect = true;
						terrain.position = chunk->GetGeometry(i, j).position;

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
						terrain.position = chunk->GetGeometry(i, j).position;

						// Store current distance
						storedDistance = distance;
					}
				}
			}
		}
	}

	// Return empty values if no intersection
	return terrain;
}
