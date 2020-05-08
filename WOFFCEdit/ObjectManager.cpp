#include "ObjectManager.h"

Game * ObjectManager::m_game;
InputCommands * ObjectManager::m_input;
std::vector<int> ObjectManager::m_selectedObjectIDs;
std::vector<SceneObject> ObjectManager::m_objectsToCopy;
std::vector<std::pair<DirectX::SimpleMath::Vector3, int>> ObjectManager::m_storedObjectScales;
std::vector<std::pair<DirectX::SimpleMath::Vector3, int>> ObjectManager::m_storedObjectRotations;
std::vector<std::pair<DirectX::SimpleMath::Vector3, int>> ObjectManager::m_storedObjectTranslations;
float ObjectManager::m_snapFactor;

// Spawn an object at a location & add to database
bool ObjectManager::SpawnObject(OBJECT_TYPE objectType, DirectX::SimpleMath::Vector3 position,
	int lightType, XMFLOAT3 diffuse, float constA, float linA, float quadA)
{
	// Store game scene graph
	std::vector<SceneObject> sceneGraph = m_game->GetSceneGraph();
	
	// Count lights
	if (objectType == OBJECT_TYPE::LIGHT)
	{
		int count = 0;
		for (int i = 0; i < sceneGraph.size(); ++i)
		{
			if (sceneGraph[i].m_type == OBJECT_TYPE::LIGHT)
			{
				count++;
			}
		}

		// If light count is above limit, don't continue
		if (count >= 10) { return false; }
	}
		
	// Setup temp object
	SceneObject object;

	// Define object values
	{
		object.ID = sceneGraph.size();
		object.chunk_ID = 0;
		object.posX = position.x;
		object.posY = position.y;
		object.posZ = position.z;
		object.rotX = 0.f;
		object.rotY = 0.f;
		object.rotZ = 0.f;
		object.scaX = 1.f;
		object.scaY = 1.f;
		object.scaZ = 1.f;
		object.render = false;
		object.collectable = false;
		object.collision_mesh = "";
		object.destructable = false;
		object.health_amount = 0;
		object.editor_render = true;
		object.editor_texture_vis = true;
		object.editor_normals_vis = false;
		object.editor_collision_vis = false;
		object.editor_pivot_vis = false;
		object.pivotX = 0.f;
		object.pivotY = 0.f;
		object.pivotZ = 0.f;
		object.snapToGround = false;
		object.AINode = false;
		object.audio_path = "";
		object.volume = 0.f;
		object.pitch = 0.f;
		object.pan = 0.f;
		object.one_shot = false;
		object.play_on_init = false;
		object.play_in_editor = false;
		object.min_dist = 0.f;
		object.max_dist = 0.f;
		object.camera = false;
		object.path_node = false;
		object.path_node_start = false;
		object.path_node_end = false;
		object.parent_id = 0;
		object.editor_wireframe = false;

		object.light_type = lightType;
		object.light_type = 1;
		object.light_diffuse_r = 100.f;
		object.light_diffuse_g = 100.f;
		object.light_diffuse_b = 100.f;
		object.light_constant = 1.f;
		object.light_linear = 0.f;
		object.light_quadratic = 1.f;

		object.light_ambient_r = 0.f;
		object.light_ambient_g = 0.1f;
		object.light_ambient_b = 0.1f;
		object.light_spot_cutoff = 8.f;

		object.m_type = objectType;
	}

	// Switch between type
	switch (object.m_type)
	{
		// Residential //////////////////////////////////////////////////////////////////////////
	case OBJECT_TYPE::HOUSE_ONE:
	{
		// Set object to house #1
		object.m_isWater = false;
		object.model_path = "database/data/house0.cmo";
		object.tex_diffuse_path = "database/data/house0.dds";
	}
	break;
	case OBJECT_TYPE::HOUSE_TWO:
	{
		// Set object to house #2
		object.m_isWater = false;
		object.model_path = "database/data/house1.cmo";
		object.tex_diffuse_path = "database/data/house1.dds";
	}
	break;
	case OBJECT_TYPE::CAVE:
	{
		// Set object to cave
		object.m_isWater = false;
		object.model_path = "database/data/cave.cmo";
		object.tex_diffuse_path = "database/data/cave.dds";
	}
	break;
	// Props ////////////////////////////////////////////////////////////////////////////////
	case OBJECT_TYPE::BRIDGE:
	{
		// Set object to bridge
		object.m_isWater = false;
		object.model_path = "database/data/bridge.cmo";
		object.tex_diffuse_path = "database/data/sand.dds";
	}
	break;
	case OBJECT_TYPE::FENCE:
	{
		// Set object to fence
		object.m_isWater = false;
		object.model_path = "database/data/fence.cmo";
		object.tex_diffuse_path = "database/data/sand.dds";
	}
	break;
	case OBJECT_TYPE::BOAT:
	{
		// Set object to boat
		object.m_isWater = false;
		object.model_path = "database/data/boat.cmo";
		object.tex_diffuse_path = "database/data/sand.dds";
	}
	break;
	// Nature ///////////////////////////////////////////////////////////////////////////////
	case OBJECT_TYPE::GRASS:
	{
		// Set object to grass
		object.m_isWater = false;
		object.model_path = "database/data/grass.cmo";
		object.tex_diffuse_path = "database/data/grass.dds";
	}
	break;
	case OBJECT_TYPE::TREE_ONE:
	{
		// Set object to tree
		object.m_isWater = false;
		object.model_path = "database/data/tree0.cmo";
		object.tex_diffuse_path = "database/data/tree0.dds";
	}
	break;
	case OBJECT_TYPE::TREE_TWO:
	{
		// Set object to tree
		object.m_isWater = false;
		object.model_path = "database/data/tree1.cmo";
		object.tex_diffuse_path = "database/data/tree1.dds";
	}
	break;
	case OBJECT_TYPE::WATER:
	{
		// Set object to water
		object.m_isWater = true;
		object.model_path = "database/data/water.cmo";
		object.tex_diffuse_path = "database/data/water.dds";
	}
	break;
	// Misc /////////////////////////////////////////////////////////////////////////////////
	case OBJECT_TYPE::LIGHT:
	{
		// Set object to light
		object.m_isWater = false;
		object.light_type = 2; // point light
		object.model_path = "database/data/light.cmo";
		object.tex_diffuse_path = "database/data/light.dds";
		object.enabled = true;
		object.rotY = 1.f;	
	}
	break;
	case OBJECT_TYPE::CUBE:
	{
		// Set object to cube
		object.m_isWater = false;
		object.model_path = "database/data/cube.cmo";
	}
	break;
	case OBJECT_TYPE::CYLINDER:
	{
		// Set object to cylinder
		object.m_isWater = false;
		object.model_path = "database/data/cylinder.cmo";
	}
	break;
	case OBJECT_TYPE::CONE:
	{
		// Set object to cone
		object.m_isWater = false;
		object.model_path = "database/data/cone.cmo";
	}
	break;
	}

	// Add new object to database
	SQLManager::AddObject(object);

	// Add new object to scene graph
	sceneGraph.push_back(object);

	// Rebuild display list
	m_game->RebuildDisplayList(&sceneGraph);	

	// Save current state
	SceneManager::SetScene(&sceneGraph, m_game->GetDisplayChunk());
}

// Remove an object from scene graph & database
std::vector<SceneObject> ObjectManager::Delete(std::vector<int> & IDs, int ID)
{			
	// Store game scene graph
	std::vector<SceneObject> sceneGraph = m_game->GetSceneGraph();
	
	// If ID has been specified
	if (ID != -1)
	{		
		// Remove object from database
		SQLManager::RemoveObject(sceneGraph[ID]);

		// Remove ID from storage
		for (int i = 0; i < IDs.size(); ++i)
		{
			// If IDs match
			if (IDs[i] == ID)
			{
				// Remove
				IDs.erase(IDs.begin() + i);
				break;
			}
		}
	}

	// Else, if no ID has been specified (delete all selected objects)
	else
	{
		// Loop through selected objects
		for (int i = 0; i < IDs.size(); ++i)
		{
			// Loop through scene graph
			for (int j = 0; j < sceneGraph.size(); ++j)
			{
				// If scene graph ID matches selected ID
				if (sceneGraph[j].ID == IDs[i])
				{
					// Remove objects from database
					SQLManager::RemoveObject(sceneGraph[j]);
				}
			}					
		}	

		// Clear all selected object IDs
		IDs.clear();
	}

	// Clear old scene graph
	sceneGraph.clear();

	// Send query to select entire object table
	SQLManager::SendQuery("SELECT * FROM Objects", true);

	// Loop through entire object table & create each object into new scene graph
	while (SQLManager::GetObjectStep() == SQLITE_ROW) { sceneGraph.push_back(SQLManager::CreateObject()); }

	// Rebuild display list from new table data
	m_game->BuildDisplayList(&sceneGraph);

	// Overwrite selected objects list
	m_selectedObjectIDs = IDs;

	// Save current state
	SceneManager::SetScene(&sceneGraph, m_game->GetDisplayChunk());

	return m_game->GetSceneGraph();
}

// Temporarily store details of all objects
void ObjectManager::Store(std::vector<int> IDs)
{
	// Reset storages vectors
	m_storedObjectScales.clear();
	m_storedObjectRotations.clear();
	m_storedObjectTranslations.clear();

	// Overwrite selected object IDs
	m_selectedObjectIDs = IDs;

	// Loop through selected objects
	for (int i = 0; i < m_selectedObjectIDs.size(); ++i)
	{
		// If selected ID isn't -1
		if (m_selectedObjectIDs[i] >= 0)
		{
			// Loop through display list
			for (int j = 0; j < m_game->GetDisplayList().size(); ++j)
			{
				// Check if current ID matches object ID
				if (m_selectedObjectIDs[i] == m_game->GetDisplayList()[j].m_ID)
				{					
					// Add selected object details to storage
					m_storedObjectScales.push_back(std::pair<DirectX::SimpleMath::Vector3, int>(m_game->GetDisplayList()[j].m_scale, j));
					m_storedObjectRotations.push_back(std::pair<DirectX::SimpleMath::Vector3, int>(m_game->GetDisplayList()[j].m_orientation, j));
					m_storedObjectTranslations.push_back(std::pair<DirectX::SimpleMath::Vector3, int>(m_game->GetDisplayList()[j].m_position, j));
				}
			}			
		}
	}
}

// Transform selected objects
void ObjectManager::Transform(OBJECT_FUNCTION function, CONSTRAINT constraint, std::vector<int> IDs)
{
	// Store game scene graph
	std::vector<SceneObject> sceneGraph = m_game->GetSceneGraph();
	
	// Switch between function
	switch (function)
	{
	case OBJECT_FUNCTION::SCALE: Scale(constraint, IDs, sceneGraph); break;
	case OBJECT_FUNCTION::ROTATE: Rotate(constraint, IDs, sceneGraph); break;
	case OBJECT_FUNCTION::TRANSLATE: Translate(constraint, IDs, sceneGraph); break;
	}
}

// Copy details of selected objects & remove from database
void ObjectManager::Cut(std::vector<int> & IDs)
{
	// Store game scene graph
	std::vector<SceneObject> sceneGraph = m_game->GetSceneGraph();
	
	// Clear current objects to copy
	m_objectsToCopy.clear();

	// Loop through scene graph
	for (int i = 0; i < sceneGraph.size(); ++i)
	{
		// Loop through selected objects
		for (int j = 0; j < IDs.size(); ++j)
		{
			// If IDs match
			if (sceneGraph[i].ID == IDs[j])
			{
				// Setup temp scene object
				SceneObject object;

				// Define object values
				{
					object.ID = IDs[j];
					object.chunk_ID = sceneGraph[i].chunk_ID;
					object.posX = sceneGraph[i].posX;
					object.posY = sceneGraph[i].posY;
					object.posZ = sceneGraph[i].posZ;
					object.rotX = sceneGraph[i].rotX;
					object.rotY = sceneGraph[i].rotY;
					object.rotZ = sceneGraph[i].rotZ;
					object.scaX = sceneGraph[i].scaX;
					object.scaY = sceneGraph[i].scaY;
					object.scaZ = sceneGraph[i].scaZ;
					object.render = sceneGraph[i].render;
					object.collectable = sceneGraph[i].collectable;
					object.collision_mesh = sceneGraph[i].collision_mesh;
					object.destructable = sceneGraph[i].destructable;
					object.health_amount = sceneGraph[i].health_amount;
					object.editor_render = sceneGraph[i].editor_render;
					object.editor_texture_vis = sceneGraph[i].editor_texture_vis;
					object.editor_normals_vis = sceneGraph[i].editor_normals_vis;
					object.editor_collision_vis = sceneGraph[i].editor_collision_vis;
					object.editor_pivot_vis = sceneGraph[i].editor_pivot_vis;
					object.pivotX = sceneGraph[i].pivotX;
					object.pivotY = sceneGraph[i].pivotY;
					object.pivotZ = sceneGraph[i].pivotZ;
					object.snapToGround = sceneGraph[i].snapToGround;
					object.AINode = sceneGraph[i].AINode;
					object.audio_path = sceneGraph[i].audio_path;
					object.volume = sceneGraph[i].volume;
					object.pitch = sceneGraph[i].pitch;
					object.pan = sceneGraph[i].pan;
					object.one_shot = sceneGraph[i].one_shot;
					object.play_on_init = sceneGraph[i].play_on_init;
					object.play_in_editor = sceneGraph[i].play_in_editor;
					object.min_dist = sceneGraph[i].min_dist;
					object.max_dist = sceneGraph[i].max_dist;
					object.camera = sceneGraph[i].camera;
					object.path_node = sceneGraph[i].path_node;
					object.path_node_start = sceneGraph[i].path_node_start;
					object.path_node_end = sceneGraph[i].path_node_end;
					object.parent_id = sceneGraph[i].parent_id;
					object.editor_wireframe = sceneGraph[i].editor_wireframe;
					object.light_type = sceneGraph[i].light_type;
					object.light_diffuse_r = sceneGraph[i].light_diffuse_r;
					object.light_diffuse_g = sceneGraph[i].light_diffuse_g;
					object.light_diffuse_b = sceneGraph[i].light_diffuse_b;
					object.light_ambient_r = sceneGraph[i].light_ambient_r;
					object.light_ambient_g = sceneGraph[i].light_ambient_g;
					object.light_ambient_b = sceneGraph[i].light_ambient_b;
					object.light_spot_cutoff = sceneGraph[i].light_spot_cutoff;
					object.light_constant = sceneGraph[i].light_constant;
					object.light_linear = sceneGraph[i].light_linear;
					object.light_quadratic = sceneGraph[i].light_quadratic;

					object.m_isWater = sceneGraph[i].m_isWater;
					object.model_path = sceneGraph[i].model_path;
					object.tex_diffuse_path = sceneGraph[i].tex_diffuse_path;
				}

				// Add object to storage
				m_objectsToCopy.push_back(object);
			}			
		}
	}	

	// Remove objects from database
	Delete(IDs);
}

// Copy details of selected objects
void ObjectManager::Copy(std::vector<int> IDs)
{
	// Store game scene graph
	std::vector<SceneObject> sceneGraph = m_game->GetSceneGraph();
	
	// Clear current objects to copy
	m_objectsToCopy.clear();

	// Fetch available IDs
	std::vector<int> availableIDs = GetAvailableIDs(IDs, sceneGraph);

	// Controller
	bool match = false;
	
	// Loop through selected objects
	for (int i = 0; i < IDs.size(); ++i)
	{
		// Reset controller
		match = false;

		// Loop through scene graph
		for (int j = 0; j < sceneGraph.size(); ++j)
		{
			// If there hasn't already been a match
			if (!match)
			{
				// If scene object ID matches selected ID
				if (sceneGraph[j].ID == IDs[i])
				{
					// Update controller
					match = true;

					// Setup temp scene object
					SceneObject object;

					// Check available IDs container is valid
					if (availableIDs.size() > 0 && availableIDs[0] < sceneGraph.size())
					{
						// Set new object ID to first available ID
						object.ID = availableIDs[0];

						// Clear available ID from vector
						availableIDs.erase(availableIDs.begin());
					}

					// Else, set new object ID as scene graph size
					else { object.ID = sceneGraph.size(); }

					// Define remaining object values
					{
						object.chunk_ID = sceneGraph[j].chunk_ID;
						object.posX = sceneGraph[j].posX + 5.f;
						object.posY = sceneGraph[j].posY;
						object.posZ = sceneGraph[j].posZ + 5.f;
						object.rotX = sceneGraph[j].rotX;
						object.rotY = sceneGraph[j].rotY;
						object.rotZ = sceneGraph[j].rotZ;
						object.scaX = sceneGraph[j].scaX;
						object.scaY = sceneGraph[j].scaY;
						object.scaZ = sceneGraph[j].scaZ;
						object.render = sceneGraph[j].render;
						object.collectable = sceneGraph[j].collectable;
						object.collision_mesh = sceneGraph[j].collision_mesh;
						object.destructable = sceneGraph[j].destructable;
						object.health_amount = sceneGraph[j].health_amount;
						object.editor_render = sceneGraph[j].editor_render;
						object.editor_texture_vis = sceneGraph[j].editor_texture_vis;
						object.editor_normals_vis = sceneGraph[j].editor_normals_vis;
						object.editor_collision_vis = sceneGraph[j].editor_collision_vis;
						object.editor_pivot_vis = sceneGraph[j].editor_pivot_vis;
						object.pivotX = sceneGraph[j].pivotX;
						object.pivotY = sceneGraph[j].pivotY;
						object.pivotZ = sceneGraph[j].pivotZ;
						object.snapToGround = sceneGraph[j].snapToGround;
						object.AINode = sceneGraph[j].AINode;
						object.audio_path = sceneGraph[j].audio_path;
						object.volume = sceneGraph[j].volume;
						object.pitch = sceneGraph[j].pitch;
						object.pan = sceneGraph[j].pan;
						object.one_shot = sceneGraph[j].one_shot;
						object.play_on_init = sceneGraph[j].play_on_init;
						object.play_in_editor = sceneGraph[j].play_in_editor;
						object.min_dist = sceneGraph[j].min_dist;
						object.max_dist = sceneGraph[j].max_dist;
						object.camera = sceneGraph[j].camera;
						object.path_node = sceneGraph[j].path_node;
						object.path_node_start = sceneGraph[j].path_node_start;
						object.path_node_end = sceneGraph[j].path_node_end;
						object.parent_id = sceneGraph[j].parent_id;
						object.editor_wireframe = sceneGraph[j].editor_wireframe;
						object.light_type = sceneGraph[j].light_type;
						object.light_diffuse_r = sceneGraph[j].light_diffuse_r;
						object.light_diffuse_g = sceneGraph[j].light_diffuse_g;
						object.light_diffuse_b = sceneGraph[j].light_diffuse_b;
						object.light_ambient_r = sceneGraph[j].light_ambient_r;
						object.light_ambient_g = sceneGraph[j].light_ambient_g;
						object.light_ambient_b = sceneGraph[j].light_ambient_b;
						object.light_spot_cutoff = sceneGraph[j].light_spot_cutoff;
						object.light_constant = sceneGraph[j].light_constant;
						object.light_linear = sceneGraph[j].light_linear;
						object.light_quadratic = sceneGraph[j].light_quadratic;

						object.m_isWater = sceneGraph[j].m_isWater;
						object.model_path = sceneGraph[j].model_path;
						object.tex_diffuse_path = sceneGraph[j].tex_diffuse_path;
					}

					// Add object to storage
					m_objectsToCopy.push_back(object);
				}
			}
		}			
	}
}

// Create new objects from copied
std::vector<SceneObject> ObjectManager::Paste()
{
	// Store game scene graph
	std::vector<SceneObject> sceneGraph = m_game->GetSceneGraph();
	
	// Loop through objects to copy
	for (int i = 0; i < m_objectsToCopy.size(); ++i)
	{		
		// Add new object to database
		SQLManager::AddObject(m_objectsToCopy[i]);

		// Add new object to scene graph
		sceneGraph.push_back(m_objectsToCopy[i]);		
	}

	// Rebuild display list
	m_game->BuildDisplayList(&sceneGraph);

	// Save current state
	SceneManager::SetScene(&sceneGraph, m_game->GetDisplayChunk());

	return m_game->GetSceneGraph();
}

// Replace the type of an object
bool ObjectManager::ReplaceType(SceneObject object,
	int lightType, XMFLOAT3 diffuse, float constA, float linA, float quadA)
{
	// Store game scene graph
	std::vector<SceneObject> sceneGraph = m_game->GetSceneGraph();
	
	// Count lights
	if (object.m_type == OBJECT_TYPE::LIGHT)
	{
		int count = 0;
		for (int i = 0; i < sceneGraph.size(); ++i)
		{
			if (sceneGraph[i].m_type == OBJECT_TYPE::LIGHT)
			{
				count++;
			}
		}

		// If light count is above limit, don't continue
		if (count >= 10) { return false; }
	}

	// Loop through scene graph
	for (int i = 0; i < sceneGraph.size(); ++i)
	{
		// If IDs match
		if (sceneGraph[i].ID == object.ID)
		{
			// Remove object from database
			SQLManager::RemoveObject(sceneGraph[i]);

			// Handle object type
			sceneGraph[i].m_type = object.m_type;

			// Setup replacement object
			SceneObject replacement = GetReplacement(sceneGraph[i]);		

			// Handle light type
			if (object.m_type == OBJECT_TYPE::LIGHT) { replacement.light_type = lightType; }
			else { replacement.light_type = 0; }

			// Add new object to database
			SQLManager::AddObject(replacement);

			// Replace object in scene graph
			sceneGraph[i] = replacement;

			break;
		}
	}

	// Rebuild display list
	m_game->BuildDisplayList(&sceneGraph);

	// Save current state
	SceneManager::SetScene(&sceneGraph, m_game->GetDisplayChunk());

	return true;
}

// Replace an entire object
bool ObjectManager::ReplaceObject(SceneObject newObject)
{
	// Store game scene graph
	std::vector<SceneObject> sceneGraph = m_game->GetSceneGraph();

	// Count lights
	if (newObject.m_type == OBJECT_TYPE::LIGHT)
	{
		int count = 0;
		for (int i = 0; i < sceneGraph.size(); ++i)
		{
			if (sceneGraph[i].m_type == OBJECT_TYPE::LIGHT)
			{
				count++;
			}
		}

		// If light count is above limit, don't continue
		if (count >= 10) { return false; }
	}

	// Loop through current scene graphs
	for (int i = 0; i < sceneGraph.size(); ++i)
	{
		// If IDs match
		if (sceneGraph[i].ID == newObject.ID)
		{
			// Remove object from database
			SQLManager::RemoveObject(sceneGraph[i]);

			// Setup replacement object
			SceneObject replacement = GetReplacement(newObject);

			// Add new object to database
			SQLManager::AddObject(replacement);

			// Replace object in scene graph
			sceneGraph[i] = replacement;

			break;
		}
	}

	// Rebuild display list
	m_game->RebuildDisplayList(&sceneGraph);

	// Save current state
	SceneManager::SetScene(&sceneGraph, m_game->GetDisplayChunk());

	return true;
}

// Replace the lighting details of an object
bool ObjectManager::ReplaceLight(SceneObject newObject)
{
	// Store game scene graph
	std::vector<SceneObject> sceneGraph = m_game->GetSceneGraph();

	// Loop through current scene graphs
	for (int i = 0; i < sceneGraph.size(); ++i)
	{
		// If IDs match
		if (sceneGraph[i].ID == newObject.ID)
		{
			// Remove object from database
			SQLManager::RemoveObject(sceneGraph[i]);

			// Setup replacement object
			SceneObject replacement = GetReplacement(newObject);
			replacement.posX = sceneGraph[i].posX;
			replacement.posY = sceneGraph[i].posY;
			replacement.posZ = sceneGraph[i].posZ;

			// Add new object to database
			SQLManager::AddObject(replacement);

			// Replace object in scene graph
			sceneGraph[i] = replacement;

			break;
		}
	}

	// Rebuild display list
	m_game->RebuildDisplayList(&sceneGraph);

	// Save current state
	SceneManager::SetScene(&sceneGraph, m_game->GetDisplayChunk());

	return true;
}

// Scale selected objects
void ObjectManager::Scale(CONSTRAINT constraint, std::vector<int> IDs, std::vector<SceneObject>& sceneGraph)
{
	// Overwrite selected object IDs
	m_selectedObjectIDs = IDs;

	// Loop through selected objects
	for (int i = 0; i < m_selectedObjectIDs.size(); ++i)
	{
		// If ID is valid
		if (m_selectedObjectIDs[i] >= 0)
		{
			// Loop through scene graph
			for (int j = 0; j < sceneGraph.size(); ++j)
			{
				// If selected ID matches object ID
				if (m_selectedObjectIDs[i] == sceneGraph[j].ID)
				{
					// Get scale from mouse position
					DirectX::SimpleMath::Vector3 scale = GetScale(j);

					// Switch between object constraint
					switch (constraint)
					{
					case CONSTRAINT::X:
					{
						// Scale selected object based on mouse drag
						sceneGraph[j].scaX = scale.x;

						// Setup for game transform			
						scale.y = sceneGraph[j].scaY;
						scale.z = sceneGraph[j].scaZ;
					}
					break;
					case CONSTRAINT::Y:
					{
						// Scale selected object based on mouse drag
						sceneGraph[j].scaY = scale.y;

						// Setup for game transform			
						scale.x = sceneGraph[j].scaX;
						scale.z = sceneGraph[j].scaZ;
					}
					break;
					case CONSTRAINT::Z:
					{
						// Scale selected object based on mouse drag
						sceneGraph[j].scaZ = scale.z;

						// Setup for game transform			
						scale.x = sceneGraph[j].scaX;
						scale.y = sceneGraph[j].scaY;
					}
					break;
					case CONSTRAINT::XY:
					{
						// Scale selected object based on mouse drag
						sceneGraph[j].scaX = scale.x;
						sceneGraph[j].scaY = scale.y;

						// Setup for game transform			
						scale.z = sceneGraph[j].scaZ;
					}
					break;
					case CONSTRAINT::XZ:
					{
						// Scale selected object based on mouse drag
						sceneGraph[j].scaX = scale.x;
						sceneGraph[j].scaZ = scale.z;

						// Setup for game transform			
						scale.y = sceneGraph[j].scaY;
					}
					break;
					case CONSTRAINT::YZ:
					{
						// Scale selected object based on mouse drag
						sceneGraph[j].scaY = scale.y;
						sceneGraph[j].scaZ = scale.z;

						// Setup for game transform			
						scale.x = sceneGraph[j].scaX;
					}
					break;
					case CONSTRAINT::NA:
					{
						// Scale selected object based on mouse drag
						sceneGraph[j].scaX = scale.x;
						sceneGraph[j].scaY = scale.y;
						sceneGraph[j].scaZ = scale.z;
					}
					break;
					}

					// Apply game transform
					m_game->SetTransform(j, OBJECT_FUNCTION::SCALE, scale);
				}
			}
		}
	}
}

// Rotate selected objects
void ObjectManager::Rotate(CONSTRAINT constraint, std::vector<int> IDs, std::vector<SceneObject>& sceneGraph)
{
	// Overwrite selected object IDs
	m_selectedObjectIDs = IDs;

	// Loop through selected objects
	for (int i = 0; i < m_selectedObjectIDs.size(); ++i)
	{
		// If ID is valid
		if (m_selectedObjectIDs[i] >= 0)
		{
			// Loop through scene graph
			for (int j = 0; j < sceneGraph.size(); ++j)
			{
				// If selected ID matches object ID
				if (m_selectedObjectIDs[i] == sceneGraph[j].ID)
				{
					// Get rotation from mouse position
					DirectX::SimpleMath::Vector3 rotate = GetRotation(j);

					// Switch between object constraint
					switch (constraint)
					{
					case CONSTRAINT::X:
					{
						// Rotate selected object based on mouse drag
						sceneGraph[j].rotX = rotate.x;

						// Setup for game transform			
						rotate.y = sceneGraph[j].rotY;
						rotate.z = sceneGraph[j].rotZ;
					}
					break;
					case CONSTRAINT::Y:
					{
						// Rotate selected object based on mouse drag
						sceneGraph[j].rotY = rotate.y;

						// Setup for game transform			
						rotate.x = sceneGraph[j].rotX;
						rotate.z = sceneGraph[j].rotZ;
					}
					break;
					case CONSTRAINT::Z:
					{
						// Rotate selected object based on mouse drag
						sceneGraph[j].rotZ = rotate.z;

						// Setup for game transform			
						rotate.x = sceneGraph[j].rotX;
						rotate.y = sceneGraph[j].rotY;
					}
					break;
					case CONSTRAINT::XY:
					{
						// Rotate selected object based on mouse drag
						sceneGraph[j].rotX = rotate.x;
						sceneGraph[j].rotY = rotate.y;

						// Setup for game transform			
						rotate.z = sceneGraph[j].rotZ;
					}
					break;
					case CONSTRAINT::XZ:
					{
						// Rotate selected object based on mouse drag
						sceneGraph[j].rotX = rotate.x;
						sceneGraph[j].rotZ = rotate.z;

						// Setup for game transform			
						rotate.y = sceneGraph[j].rotY;
					}
					break;
					case CONSTRAINT::YZ:
					{
						// Rotate selected object based on mouse drag
						sceneGraph[j].rotY = rotate.y;
						sceneGraph[j].rotZ = rotate.z;

						// Setup for game transform			
						rotate.x = sceneGraph[j].rotX;
					}
					break;
					case CONSTRAINT::NA:
					{
						// Rotate selected object based on mouse drag
						sceneGraph[j].rotX = rotate.x;
						sceneGraph[j].rotY = rotate.y;
						sceneGraph[j].rotZ = rotate.z;
					}
					break;
					}

					// Apply game transform
					m_game->SetTransform(j, OBJECT_FUNCTION::ROTATE, rotate);
				}
			}
		}
	}
}

// Translate selected objects
void ObjectManager::Translate(CONSTRAINT constraint, std::vector<int> IDs, std::vector<SceneObject>& sceneGraph)
{
	// Overwrite selected object IDs
	m_selectedObjectIDs = IDs;
	
	// Loop through selected objects
	for (int i = 0; i < m_selectedObjectIDs.size(); ++i)
	{		
		// If ID is valid
		if (m_selectedObjectIDs[i] >= 0)
		{
			// Loop through scene graph
			for (int j = 0; j < sceneGraph.size(); ++j)
			{
				// If selected ID matches object ID
				if (m_selectedObjectIDs[i] == sceneGraph[j].ID)
				{
					// Get translation from mouse position
					DirectX::SimpleMath::Vector3 translate = GetTranslation(j);

					// Switch between object constraint
					switch (constraint)
					{
					case CONSTRAINT::X:
					{
						// Translate selected object based on mouse drag
						sceneGraph[j].posX = translate.x;

						// Setup for game transform			
						translate.y = sceneGraph[j].posY;
						translate.z = sceneGraph[j].posZ;
					}
					break;
					case CONSTRAINT::Y:
					{
						// Translate selected object based on mouse drag
						sceneGraph[j].posY = translate.y;

						// Setup for game transform			
						translate.x = sceneGraph[j].posX;
						translate.z = sceneGraph[j].posZ;
					}
					break;
					case CONSTRAINT::Z:
					{
						// Translate selected object based on mouse drag
						sceneGraph[j].posZ = translate.z;

						// Setup for game transform			
						translate.x = sceneGraph[j].posX;
						translate.y = sceneGraph[j].posY;
					}
					break;
					case CONSTRAINT::XY:
					{
						// Translate selected object based on mouse drag
						sceneGraph[j].posX = translate.x;
						sceneGraph[j].posY = translate.y;

						// Setup for game transform			
						translate.z = sceneGraph[j].posZ;
					}
					break;
					case CONSTRAINT::XZ:
					{
						// Translate selected object based on mouse drag
						sceneGraph[j].posX = translate.x;
						sceneGraph[j].posZ = translate.z;

						// Setup for game transform			
						translate.y = sceneGraph[j].posY;
					}
					break;
					case CONSTRAINT::YZ:
					{
						// Translate selected object based on mouse drag
						sceneGraph[j].posY = translate.y;
						sceneGraph[j].posZ = translate.z;

						// Setup for game transform
						translate.x = sceneGraph[j].posX;
					}
					break;
					case CONSTRAINT::NA:
					{
						// Translate selected object based on mouse drag
						sceneGraph[j].posX = translate.x;
						sceneGraph[j].posY = translate.y;
						sceneGraph[j].posZ = translate.z;
					}
					break;
					}

					// Apply game transform
					m_game->SetTransform(j, OBJECT_FUNCTION::TRANSLATE, translate);
				}
			}			
		}
	}
}

// Get scale for object manipulation via mouse drag
DirectX::SimpleMath::Vector3 ObjectManager::GetScale(int index)
{
	// Temp storage 
	DirectX::SimpleMath::Vector2 previous = m_input->mousePosPrevious;
	DirectX::SimpleMath::Vector2 current = m_input->mousePos;
	DirectX::SimpleMath::Vector3 object = m_game->GetDisplayList()[index].m_position;
	DirectX::SimpleMath::Vector3 s = m_game->GetDisplayList()[index].m_scale;

	// Setup previous & current ray traces
	DirectX::SimpleMath::Ray pre = MouseManager::RayTrace(previous);
	DirectX::SimpleMath::Ray cur = MouseManager::RayTrace(current);

	// Distance between previous & object
	float distP = sqrt(
		(pre.position.x - object.x) * (pre.position.x - object.x) +
		(pre.position.y - object.y) * (pre.position.y - object.y) +
		(pre.position.z - object.z) * (pre.position.z - object.z));

	// Distance between current & object
	float distC = sqrt(
		(cur.position.x - object.x) * (cur.position.x - object.x) +
		(cur.position.y - object.y) * (cur.position.y - object.y) +
		(cur.position.z - object.z) * (cur.position.z - object.z));

	// Setup temp picking points
	DirectX::SimpleMath::Vector3 pickingP = pre.position + (pre.direction * distP);
	DirectX::SimpleMath::Vector3 pickingC = cur.position + (cur.direction * distC);

	// Setup temp scale
	DirectX::SimpleMath::Vector3 scale;

	// Loop through stored scale
	for (int i = 0; i < m_storedObjectScales.size(); ++i)
	{
		// If indexes math
		if (m_storedObjectScales[i].second == index)
		{
			// Define manipulated translation
			scale.x = m_storedObjectScales[i].first.x + (pickingC.x - pickingP.x);
			scale.y = m_storedObjectScales[i].first.y + (pickingC.y - pickingP.y);
			scale.z = m_storedObjectScales[i].first.z + (pickingC.z - pickingP.z);
		}
	}

	// If scale is below minimum scale
	if (scale.x < .1f) { scale.x = .1f; }
	if (scale.y < .1f) { scale.y = .1f; }
	if (scale.z < .1f) { scale.z = .1f; }

	// Return manipulated scale
	return scale;
}

// Get rotation for object manipulation via mouse drag
DirectX::SimpleMath::Vector3 ObjectManager::GetRotation(int index)
{
	// Temp storage 
	DirectX::SimpleMath::Vector2 previous = m_input->mousePosPrevious;
	DirectX::SimpleMath::Vector2 current = m_input->mousePos;
	DirectX::SimpleMath::Vector3 object = m_game->GetDisplayList()[index].m_position;

	// Setup previous & current ray traces
	DirectX::SimpleMath::Ray pre = MouseManager::RayTrace(previous);
	DirectX::SimpleMath::Ray cur = MouseManager::RayTrace(current);

	// Distance between previous & object
	float distP = sqrt(
		(pre.position.x - object.x) * (pre.position.x - object.x) +
		(pre.position.y - object.y) * (pre.position.y - object.y) +
		(pre.position.z - object.z) * (pre.position.z - object.z));

	// Distance between current & object
	float distC = sqrt(
		(cur.position.x - object.x) * (cur.position.x - object.x) +
		(cur.position.y - object.y) * (cur.position.y - object.y) +
		(cur.position.z - object.z) * (cur.position.z - object.z));

	// Setup temp picking points
	DirectX::SimpleMath::Vector3 pickingP = pre.position + (pre.direction * distP);
	DirectX::SimpleMath::Vector3 pickingC = cur.position + (cur.direction * distC);

	// Setup temp rotation
	DirectX::SimpleMath::Vector3 rotate;

	// Loop through stored rotations
	for (int i = 0; i < m_storedObjectRotations.size(); ++i)
	{
		// If indexes math
		if (m_storedObjectRotations[i].second == index)
		{
			// Define manipulated rotation
			rotate.x = m_storedObjectRotations[i].first.x + (pickingC.x - pickingP.x) * 50.f;
			rotate.y = m_storedObjectRotations[i].first.y + (pickingC.y - pickingP.y) * 50.f;
			rotate.z = m_storedObjectRotations[i].first.z + (pickingC.z - pickingP.z) * 50.f;
		}
	}

	// Return manipulated rotation
	return rotate;
}

// Get translation for object manipulation via mouse drag
DirectX::SimpleMath::Vector3 ObjectManager::GetTranslation(int index)
{
	// Temp storage 
	DirectX::SimpleMath::Vector2 previous = m_input->mousePosPrevious;
	DirectX::SimpleMath::Vector2 current = m_input->mousePos;
	DirectX::SimpleMath::Vector3 object = m_game->GetDisplayList()[index].m_position;

	// Setup previous & current ray traces
	DirectX::SimpleMath::Ray pre = MouseManager::RayTrace(previous);
	DirectX::SimpleMath::Ray cur = MouseManager::RayTrace(current);

	// Distance between previous & object
	float distP = sqrt(
		(pre.position.x - object.x) * (pre.position.x - object.x) +
		(pre.position.y - object.y) * (pre.position.y - object.y) +
		(pre.position.z - object.z) * (pre.position.z - object.z));

	// Distance between current & object
	float distC = sqrt(
		(cur.position.x - object.x) * (cur.position.x - object.x) +
		(cur.position.y - object.y) * (cur.position.y - object.y) +
		(cur.position.z - object.z) * (cur.position.z - object.z));

	// Setup temp picking points
	DirectX::SimpleMath::Vector3 pickingP = pre.position + (pre.direction * distP);
	DirectX::SimpleMath::Vector3 pickingC = cur.position + (cur.direction * distC);

	// Setup temp translation
	DirectX::SimpleMath::Vector3 position;

	// Get difference between picking points
	DirectX::SimpleMath::Vector3 difference;
	difference.x = pickingC.x - pickingP.x;
	difference.y = pickingC.y - pickingP.y;
	difference.z = pickingC.z - pickingP.z;

	// Loop through stored translations
	for (int i = 0; i < m_storedObjectTranslations.size(); ++i)
	{
		// If indexes match
		if (m_storedObjectTranslations[i].second == index)
		{
			// Define manipulated translation
			position.x = m_storedObjectTranslations[i].first.x + difference.x;
			position.y = m_storedObjectTranslations[i].first.y + difference.y;
			position.z = m_storedObjectTranslations[i].first.z + difference.z;			
		}
	}	

	// Return manipulated translation
	return position;
}

// Check scene graph for available IDs
std::vector<int> ObjectManager::GetAvailableIDs(std::vector<int> IDs, std::vector<SceneObject> & sceneGraph)
{
	// Setup containers
	std::vector<int> availableIDs;
	std::vector<int> allIDs;
		
	// Loop through scene graph
	for (int i = 0; i < sceneGraph.size(); ++i)
	{
		// Add current object IDs to allIDs
		allIDs.push_back(sceneGraph[i].ID);		
	}	

	// Loop through scene graph
	for (int i = 0; i < sceneGraph.size(); ++i)
	{		
		// Search for current index in IDs
		if (std::find(allIDs.begin(), allIDs.end(), i) != allIDs.end()) {}
		else 
		{
			// If not found, add current index to available IDs
			availableIDs.push_back(i);
		}		
	}

	// Return container
	return availableIDs;
}

// Get a replacement object
SceneObject ObjectManager::GetReplacement(SceneObject object)
{
	// Setup temp object
	SceneObject replacement;
	
	// Define object values
	{
		replacement.ID = object.ID;
		replacement.chunk_ID = object.chunk_ID;
		replacement.posX = object.posX;
		replacement.posY = object.posY;
		replacement.posZ = object.posZ;
		replacement.rotX = object.rotX;
		replacement.rotY = object.rotY;
		replacement.rotZ = object.rotZ;
		replacement.scaX = object.scaX;
		replacement.scaY = object.scaY;
		replacement.scaZ = object.scaZ;
		replacement.render = object.render;
		replacement.collectable = object.collectable;
		replacement.collision_mesh = object.collision_mesh;
		replacement.destructable = object.destructable;
		replacement.health_amount = object.health_amount;
		replacement.editor_render = object.editor_render;
		replacement.editor_texture_vis = object.editor_texture_vis;
		replacement.editor_normals_vis = object.editor_normals_vis;
		replacement.editor_collision_vis = object.editor_collision_vis;
		replacement.editor_pivot_vis = object.editor_pivot_vis;
		replacement.pivotX = object.pivotX;
		replacement.pivotY = object.pivotY;
		replacement.pivotZ = object.pivotZ;
		replacement.snapToGround = object.snapToGround;
		replacement.AINode = object.AINode;
		replacement.audio_path = object.audio_path;
		replacement.volume = object.volume;
		replacement.pitch = object.pitch;
		replacement.pan = object.pan;
		replacement.one_shot = object.one_shot;
		replacement.play_on_init = object.play_on_init;
		replacement.play_in_editor = object.play_in_editor;
		replacement.min_dist = object.min_dist;
		replacement.max_dist = object.max_dist;
		replacement.camera = object.camera;
		replacement.path_node = object.path_node;
		replacement.path_node_start = object.path_node_start;
		replacement.path_node_end = object.path_node_end;
		replacement.parent_id = object.parent_id;
		replacement.editor_wireframe = object.editor_wireframe;

		if (object.m_type == OBJECT_TYPE::LIGHT) { replacement.light_type = object.light_type; }
		else { replacement.light_type = 1; }
		
		replacement.light_diffuse_r = object.light_diffuse_r;
		replacement.light_diffuse_g = object.light_diffuse_g;
		replacement.light_diffuse_b = object.light_diffuse_b;

		replacement.light_constant = object.light_constant;
		replacement.light_linear = object.light_linear;
		replacement.light_quadratic = object.light_quadratic;

		replacement.light_ambient_r = object.light_ambient_r;
		replacement.light_ambient_g = object.light_ambient_g;
		replacement.light_ambient_b = object.light_ambient_b;

		replacement.light_spot_cutoff = object.light_spot_cutoff;

		replacement.m_type = object.m_type;
		replacement.enabled = object.enabled;
	}

	// Switch between type
	switch (replacement.m_type)
	{
		// Residential //////////////////////////////////////////////////////////////////////////
	case OBJECT_TYPE::HOUSE_ONE:
	{
		// Set object to house #1
		replacement.m_isWater = false;
		replacement.model_path = "database/data/house0.cmo";
		replacement.tex_diffuse_path = "database/data/house0.dds";
	}
	break;
	case OBJECT_TYPE::HOUSE_TWO:
	{
		// Set object to house #2
		replacement.m_isWater = false;
		replacement.model_path = "database/data/house1.cmo";
		replacement.tex_diffuse_path = "database/data/house1.dds";
	}
	break;
	case OBJECT_TYPE::CAVE:
	{
		// Set object to cave
		replacement.m_isWater = false;
		replacement.model_path = "database/data/cave.cmo";
		replacement.tex_diffuse_path = "database/data/cave.dds";
	}
	break;
	// Props ////////////////////////////////////////////////////////////////////////////////
	case OBJECT_TYPE::BRIDGE:
	{
		// Set object to bridge
		replacement.m_isWater = false;
		replacement.model_path = "database/data/bridge.cmo";
		replacement.tex_diffuse_path = "database/data/sand.dds";
	}
	break;
	case OBJECT_TYPE::FENCE:
	{
		// Set object to fence
		replacement.m_isWater = false;
		replacement.model_path = "database/data/fence.cmo";
		replacement.tex_diffuse_path = "database/data/sand.dds";
	}
	break;
	case OBJECT_TYPE::BOAT:
	{
		// Set object to boat
		replacement.m_isWater = false;
		replacement.model_path = "database/data/boat.cmo";
		replacement.tex_diffuse_path = "database/data/sand.dds";
	}
	break;
	// Nature ///////////////////////////////////////////////////////////////////////////////
	case OBJECT_TYPE::GRASS:
	{
		// Set object to grass
		replacement.m_isWater = false;
		replacement.model_path = "database/data/grass.cmo";
		replacement.tex_diffuse_path = "database/data/grass.dds";
	}
	break;
	case OBJECT_TYPE::TREE_ONE:
	{
		// Set object to tree
		replacement.m_isWater = false;
		replacement.model_path = "database/data/tree0.cmo";
		replacement.tex_diffuse_path = "database/data/tree0.dds";
	}
	break;
	case OBJECT_TYPE::TREE_TWO:
	{
		// Set object to tree
		replacement.m_isWater = false;
		replacement.model_path = "database/data/tree1.cmo";
		replacement.tex_diffuse_path = "database/data/tree1.dds";
	}
	break;
	case OBJECT_TYPE::WATER:
	{
		// Set object to water
		replacement.m_isWater = true;
		replacement.model_path = "database/data/water.cmo";
		replacement.tex_diffuse_path = "database/data/water.dds";
	}
	break;
	// Misc /////////////////////////////////////////////////////////////////////////////////
	case OBJECT_TYPE::LIGHT:
	{
		// Set object to light
		replacement.m_isWater = false;
		replacement.model_path = "database/data/light.cmo";
		replacement.tex_diffuse_path = "database/data/light.dds";
		replacement.enabled = true;
	}
	break;
	case OBJECT_TYPE::CUBE:
	{
		// Set object to cube
		replacement.m_isWater = false;
		replacement.model_path = "database/data/cube.cmo";
	}
	break;
	case OBJECT_TYPE::CYLINDER:
	{
		// Set object to cylinder
		replacement.m_isWater = false;
		replacement.model_path = "database/data/cylinder.cmo";
	}
	break;
	case OBJECT_TYPE::CONE:
	{
		// Set object to cone
		replacement.m_isWater = false;
		replacement.model_path = "database/data/cone.cmo";
	}
	break;
	}

	return replacement;
}