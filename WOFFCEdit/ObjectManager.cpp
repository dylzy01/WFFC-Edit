#include "ObjectManager.h"

Game * ObjectManager::m_game;
InputCommands * ObjectManager::m_input;
std::vector<int> ObjectManager::m_selectedObjectIDs;
std::vector<SceneObject> ObjectManager::m_objectsToCopy;
std::vector<DirectX::SimpleMath::Vector3> ObjectManager::m_storedObjectScales;
std::vector<DirectX::SimpleMath::Vector3> ObjectManager::m_storedObjectRotations;
std::vector<DirectX::SimpleMath::Vector3> ObjectManager::m_storedObjectTranslations;

// Spawn an object at a location & add to database
bool ObjectManager::SpawnObject(OBJECT_TYPE objectType, DirectX::SimpleMath::Vector3 position,
	std::vector<SceneObject> & sceneGraph, int lightType, XMFLOAT3 diffuse, float constA, float linA, float quadA)
{
	// Count lights
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
	
	// If object is a light
	//if (objectType == OBJECT_TYPE::LIGHT)
	//{
	//	// If active lights is above limit
	//	if (m_game->GetLights().first.size() >= 10)
	//	{
	//		// Prevent object addition
	//		return false;
	//	}
	//}
		
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
		///object.light_type = 3;
		object.light_diffuse_r = 2.f;
		object.light_diffuse_g = 3.f;
		object.light_diffuse_b = 4.f;
		object.light_constant = 1.f;
		object.light_linear = 0.f;
		object.light_quadratic = 1.f;

		object.light_specular_r = 4.f;
		object.light_specular_g = 5.f;
		object.light_specular_b = 6.f;
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
		object.rotX = 0.f;
		object.rotY = 1.f;
		object.rotZ = 0.f;
		object.ambR = 0.2f;
		object.ambG = 0.2f;
		object.ambB = 0.2f;
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
	m_game->BuildDisplayList(&sceneGraph);	
}

// Remove an object from scene graph & database
void ObjectManager::Remove(std::vector<int> & IDs, std::vector<SceneObject> & sceneGraph, int ID)
{			
	// If ID has been specified
	if (ID != -1)
	{		
		// Remove object from database
		SQLManager::RemoveObject(sceneGraph[ID]);		
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
			// Add selected object details to storage
			m_storedObjectScales.push_back(m_game->GetDisplayList()[m_selectedObjectIDs[i]].m_scale);
			m_storedObjectRotations.push_back(m_game->GetDisplayList()[m_selectedObjectIDs[i]].m_orientation);
			m_storedObjectTranslations.push_back(m_game->GetDisplayList()[m_selectedObjectIDs[i]].m_position);
		}
	}
}

// Transform selected objects
void ObjectManager::Transform(OBJECT_FUNCTION function, CONSTRAINT constraint, std::vector<int> IDs, std::vector<SceneObject>& sceneGraph)
{
	// Switch between function
	switch (function)
	{
	case OBJECT_FUNCTION::SCALE: Scale(constraint, IDs, sceneGraph); break;
	case OBJECT_FUNCTION::ROTATE: Rotate(constraint, IDs, sceneGraph); break;
	case OBJECT_FUNCTION::TRANSLATE: Translate(constraint, IDs, sceneGraph); break;
	}

	// Save current state
	SceneManager::SetScene(&sceneGraph, m_game->GetDisplayChunk());
}

// Copy details of selected objects & remove from database
void ObjectManager::Cut(std::vector<int> & IDs, std::vector<SceneObject> & sceneGraph)
{
	// Clear current objects to copy
	m_objectsToCopy.clear();

	// Loop through selected objects
	for (int i = 0; i < IDs.size(); ++i)
	{
		// Setup temp scene object
		SceneObject object;

		// Define object values
		{
			object.ID = IDs[i];
			object.chunk_ID = sceneGraph[IDs[i]].chunk_ID;
			object.posX = sceneGraph[IDs[i]].posX;
			object.posY = sceneGraph[IDs[i]].posY;
			object.posZ = sceneGraph[IDs[i]].posZ;
			object.rotX = sceneGraph[IDs[i]].rotX;
			object.rotY = sceneGraph[IDs[i]].rotY;
			object.rotZ = sceneGraph[IDs[i]].rotZ;
			object.scaX = sceneGraph[IDs[i]].scaX;
			object.scaY = sceneGraph[IDs[i]].scaY;
			object.scaZ = sceneGraph[IDs[i]].scaZ;
			object.render = sceneGraph[IDs[i]].render;
			object.collectable = sceneGraph[IDs[i]].collectable;
			object.collision_mesh = sceneGraph[IDs[i]].collision_mesh;
			object.destructable = sceneGraph[IDs[i]].destructable;
			object.health_amount = sceneGraph[IDs[i]].health_amount;
			object.editor_render = sceneGraph[IDs[i]].editor_render;
			object.editor_texture_vis = sceneGraph[IDs[i]].editor_texture_vis;
			object.editor_normals_vis = sceneGraph[IDs[i]].editor_normals_vis;
			object.editor_collision_vis = sceneGraph[IDs[i]].editor_collision_vis;
			object.editor_pivot_vis = sceneGraph[IDs[i]].editor_pivot_vis;
			object.pivotX = sceneGraph[IDs[i]].pivotX;
			object.pivotY = sceneGraph[IDs[i]].pivotY;
			object.pivotZ = sceneGraph[IDs[i]].pivotZ;
			object.snapToGround = sceneGraph[IDs[i]].snapToGround;
			object.AINode = sceneGraph[IDs[i]].AINode;
			object.audio_path = sceneGraph[IDs[i]].audio_path;
			object.volume = sceneGraph[IDs[i]].volume;
			object.pitch = sceneGraph[IDs[i]].pitch;
			object.pan = sceneGraph[IDs[i]].pan;
			object.one_shot = sceneGraph[IDs[i]].one_shot;
			object.play_on_init = sceneGraph[IDs[i]].play_on_init;
			object.play_in_editor = sceneGraph[IDs[i]].play_in_editor;
			object.min_dist = sceneGraph[IDs[i]].min_dist;
			object.max_dist = sceneGraph[IDs[i]].max_dist;
			object.camera = sceneGraph[IDs[i]].camera;
			object.path_node = sceneGraph[IDs[i]].path_node;
			object.path_node_start = sceneGraph[IDs[i]].path_node_start;
			object.path_node_end = sceneGraph[IDs[i]].path_node_end;
			object.parent_id = sceneGraph[IDs[i]].parent_id;
			object.editor_wireframe = sceneGraph[IDs[i]].editor_wireframe;
			object.light_type = sceneGraph[IDs[i]].light_type;
			object.light_diffuse_r = sceneGraph[IDs[i]].light_diffuse_r;
			object.light_diffuse_g = sceneGraph[IDs[i]].light_diffuse_g;
			object.light_diffuse_b = sceneGraph[IDs[i]].light_diffuse_b;
			object.light_specular_r = sceneGraph[IDs[i]].light_specular_r;
			object.light_specular_g = sceneGraph[IDs[i]].light_specular_g;
			object.light_specular_b = sceneGraph[IDs[i]].light_specular_b;
			object.light_spot_cutoff = sceneGraph[IDs[i]].light_spot_cutoff;
			object.light_constant = sceneGraph[IDs[i]].light_constant;
			object.light_linear = sceneGraph[IDs[i]].light_linear;
			object.light_quadratic = sceneGraph[IDs[i]].light_quadratic;

			object.m_isWater = sceneGraph[IDs[i]].m_isWater;
			object.model_path = sceneGraph[IDs[i]].model_path;
			object.tex_diffuse_path = sceneGraph[IDs[i]].tex_diffuse_path;
		}

		// Add object to storage
		m_objectsToCopy.push_back(object);
	}

	// Remove objects from database
	Remove(IDs, sceneGraph);
}

// Copy details of selected objects
void ObjectManager::Copy(std::vector<int> IDs, std::vector<SceneObject> sceneGraph)
{
	// Clear current objects to copy
	m_objectsToCopy.clear();
	
	// Loop through selected objects
	for (int i = 0; i < IDs.size(); ++i)
	{
		// Loop through scene graph
		for (int j = 0; j < sceneGraph.size(); ++j)
		{
			// If scene object ID matches selected ID
			if (sceneGraph[j].ID == IDs[i])
			{
				// Setup temp scene object
				SceneObject object;

				// Define object values
				{
					object.ID = sceneGraph.size();
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
					object.light_specular_r = sceneGraph[j].light_specular_r;
					object.light_specular_g = sceneGraph[j].light_specular_g;
					object.light_specular_b = sceneGraph[j].light_specular_b;
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

// Create new objects from copied
void ObjectManager::Paste(std::vector<SceneObject> & sceneGraph)
{
	// Loop through objects to copy
	for (int i = 0; i < m_objectsToCopy.size(); ++i)
	{
		// Update object ID
		m_objectsToCopy[i].ID = sceneGraph.size();
		
		// Add new object to database
		SQLManager::AddObject(m_objectsToCopy[i]);

		// Add new object to scene graph
		sceneGraph.push_back(m_objectsToCopy[i]);

		// Rebuild display list
		m_game->BuildDisplayList(&sceneGraph);
	}

	// Save current state
	SceneManager::SetScene(&sceneGraph, m_game->GetDisplayChunk());
}

// Replace the model of an object
bool ObjectManager::Replace(int ID, std::vector<SceneObject>& sceneGraph, OBJECT_TYPE objectType,
	int lightType, XMFLOAT3 diffuse, float constA, float linA, float quadA)
{
	// Count lights
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

	// Loop through scene graph
	for (int i = 0; i < sceneGraph.size(); ++i)
	{
		// If IDs match
		if (sceneGraph[i].ID == ID)
		{
			// Remove object from database
			SQLManager::RemoveObject(sceneGraph[i]);

			// Setup temp object
			SceneObject replacement;

			// Define object values
			{
				replacement.ID = sceneGraph[i].ID;
				replacement.chunk_ID = sceneGraph[i].chunk_ID;
				replacement.posX = sceneGraph[i].posX;
				replacement.posY = sceneGraph[i].posY;
				replacement.posZ = sceneGraph[i].posZ;
				replacement.rotX = sceneGraph[i].rotX;
				replacement.rotY = sceneGraph[i].rotY;
				replacement.rotZ = sceneGraph[i].rotZ;
				replacement.scaX = sceneGraph[i].scaX;
				replacement.scaY = sceneGraph[i].scaY;
				replacement.scaZ = sceneGraph[i].scaZ;
				replacement.render = sceneGraph[i].render;
				replacement.collectable = sceneGraph[i].collectable;
				replacement.collision_mesh = sceneGraph[i].collision_mesh;
				replacement.destructable = sceneGraph[i].destructable;
				replacement.health_amount = sceneGraph[i].health_amount;
				replacement.editor_render = sceneGraph[i].editor_render;
				replacement.editor_texture_vis = sceneGraph[i].editor_texture_vis;
				replacement.editor_normals_vis = sceneGraph[i].editor_normals_vis;
				replacement.editor_collision_vis = sceneGraph[i].editor_collision_vis;
				replacement.editor_pivot_vis = sceneGraph[i].editor_pivot_vis;
				replacement.pivotX = sceneGraph[i].pivotX;
				replacement.pivotY = sceneGraph[i].pivotY;
				replacement.pivotZ = sceneGraph[i].pivotZ;
				replacement.snapToGround = sceneGraph[i].snapToGround;
				replacement.AINode = sceneGraph[i].AINode;
				replacement.audio_path = sceneGraph[i].audio_path;
				replacement.volume = sceneGraph[i].volume;
				replacement.pitch = sceneGraph[i].pitch;
				replacement.pan = sceneGraph[i].pan;
				replacement.one_shot = sceneGraph[i].one_shot;
				replacement.play_on_init = sceneGraph[i].play_on_init;
				replacement.play_in_editor = sceneGraph[i].play_in_editor;
				replacement.min_dist = sceneGraph[i].min_dist;
				replacement.max_dist = sceneGraph[i].max_dist;
				replacement.camera = sceneGraph[i].camera;
				replacement.path_node = sceneGraph[i].path_node;
				replacement.path_node_start = sceneGraph[i].path_node_start;
				replacement.path_node_end = sceneGraph[i].path_node_end;
				replacement.parent_id = sceneGraph[i].parent_id;
				replacement.editor_wireframe = sceneGraph[i].editor_wireframe;

				replacement.light_type = lightType;
				replacement.light_diffuse_r = 2.f;
				replacement.light_diffuse_g = 3.f;
				replacement.light_diffuse_b = 4.f;
				replacement.light_constant = 1.f;
				replacement.light_linear = 0.f;
				replacement.light_quadratic = 1.f;

				replacement.light_specular_r = 4.f;
				replacement.light_specular_g = 5.f;
				replacement.light_specular_b = 6.f;
				replacement.light_spot_cutoff = 8.f;

				replacement.m_type = objectType;
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
				replacement.rotX = 0.f;
				replacement.rotY = 1.f;
				replacement.rotZ = 0.f;
				replacement.ambR = 0.2f;
				replacement.ambG = 0.2f;
				replacement.ambB = 0.2f;
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
			// Get scale from mouse position
			DirectX::SimpleMath::Vector3 scale = GetScale(i);

			// Switch between object constraint
			switch (constraint)
			{
			case CONSTRAINT::X:
			{
				// Scale selected object based on mouse drag
				sceneGraph[m_selectedObjectIDs[i]].scaX = scale.x;

				// Setup for game transform			
				scale.y = sceneGraph[m_selectedObjectIDs[i]].scaY;
				scale.z = sceneGraph[m_selectedObjectIDs[i]].scaZ;
			}
			break;
			case CONSTRAINT::Y:
			{
				// Scale selected object based on mouse drag
				sceneGraph[m_selectedObjectIDs[i]].scaY = scale.y;

				// Setup for game transform			
				scale.x = sceneGraph[m_selectedObjectIDs[i]].scaX;
				scale.z = sceneGraph[m_selectedObjectIDs[i]].scaZ;
			}
			break;
			case CONSTRAINT::Z:
			{
				// Scale selected object based on mouse drag
				sceneGraph[m_selectedObjectIDs[i]].scaZ = scale.z;

				// Setup for game transform			
				scale.x = sceneGraph[m_selectedObjectIDs[i]].scaX;
				scale.y = sceneGraph[m_selectedObjectIDs[i]].scaY;
			}
			break;
			case CONSTRAINT::XY:
			{
				// Scale selected object based on mouse drag
				sceneGraph[m_selectedObjectIDs[i]].scaX = scale.x;
				sceneGraph[m_selectedObjectIDs[i]].scaY = scale.y;

				// Setup for game transform			
				scale.z = sceneGraph[m_selectedObjectIDs[i]].scaZ;
			}
			break;
			case CONSTRAINT::XZ:
			{
				// Scale selected object based on mouse drag
				sceneGraph[m_selectedObjectIDs[i]].scaX = scale.x;
				sceneGraph[m_selectedObjectIDs[i]].scaZ = scale.z;

				// Setup for game transform			
				scale.y = sceneGraph[m_selectedObjectIDs[i]].scaY;
			}
			break;
			case CONSTRAINT::YZ:
			{
				// Scale selected object based on mouse drag
				sceneGraph[m_selectedObjectIDs[i]].scaY = scale.y;
				sceneGraph[m_selectedObjectIDs[i]].scaZ = scale.z;

				// Setup for game transform			
				scale.x = sceneGraph[m_selectedObjectIDs[i]].scaX;
			}
			break;
			case CONSTRAINT::NA:
			{
				// Scale selected object based on mouse drag
				sceneGraph[m_selectedObjectIDs[i]].scaX = scale.x;
				sceneGraph[m_selectedObjectIDs[i]].scaY = scale.y;
				sceneGraph[m_selectedObjectIDs[i]].scaZ = scale.z;
			}
			break;
			}

			// Apply game transform
			m_game->SetTransform(m_selectedObjectIDs[i], OBJECT_FUNCTION::SCALE, scale);
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
			// Get rotation from mouse position
			DirectX::SimpleMath::Vector3 rotate = GetRotation(i);

			// Switch between object constraint
			switch (constraint)
			{
			case CONSTRAINT::X:
			{
				// Rotate selected object based on mouse drag
				sceneGraph[m_selectedObjectIDs[i]].rotX = rotate.x;

				// Setup for game transform			
				rotate.y = sceneGraph[m_selectedObjectIDs[i]].rotY;
				rotate.z = sceneGraph[m_selectedObjectIDs[i]].rotZ;
			}
			break;
			case CONSTRAINT::Y:
			{
				// Rotate selected object based on mouse drag
				sceneGraph[m_selectedObjectIDs[i]].rotY = rotate.y;

				// Setup for game transform			
				rotate.x = sceneGraph[m_selectedObjectIDs[i]].rotX;
				rotate.z = sceneGraph[m_selectedObjectIDs[i]].rotZ;
			}
			break;
			case CONSTRAINT::Z:
			{
				// Rotate selected object based on mouse drag
				sceneGraph[m_selectedObjectIDs[i]].rotZ = rotate.z;

				// Setup for game transform			
				rotate.x = sceneGraph[m_selectedObjectIDs[i]].rotX;
				rotate.y = sceneGraph[m_selectedObjectIDs[i]].rotY;
			}
			break;
			case CONSTRAINT::XY:
			{
				// Rotate selected object based on mouse drag
				sceneGraph[m_selectedObjectIDs[i]].rotX = rotate.x;
				sceneGraph[m_selectedObjectIDs[i]].rotY = rotate.y;

				// Setup for game transform			
				rotate.z = sceneGraph[m_selectedObjectIDs[i]].rotZ;
			}
			break;
			case CONSTRAINT::XZ:
			{
				// Rotate selected object based on mouse drag
				sceneGraph[m_selectedObjectIDs[i]].rotX = rotate.x;
				sceneGraph[m_selectedObjectIDs[i]].rotZ = rotate.z;

				// Setup for game transform			
				rotate.y = sceneGraph[m_selectedObjectIDs[i]].rotY;
			}
			break;
			case CONSTRAINT::YZ:
			{
				// Rotate selected object based on mouse drag
				sceneGraph[m_selectedObjectIDs[i]].rotY = rotate.y;
				sceneGraph[m_selectedObjectIDs[i]].rotZ = rotate.z;

				// Setup for game transform			
				rotate.x = sceneGraph[m_selectedObjectIDs[i]].rotX;
			}
			break;
			case CONSTRAINT::NA:
			{
				// Rotate selected object based on mouse drag
				sceneGraph[m_selectedObjectIDs[i]].rotX = rotate.x;
				sceneGraph[m_selectedObjectIDs[i]].rotY = rotate.y;
				sceneGraph[m_selectedObjectIDs[i]].rotZ = rotate.z;
			}
			break;
			}

			// Apply game transform
			m_game->SetTransform(m_selectedObjectIDs[i], OBJECT_FUNCTION::ROTATE, rotate);
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
			// Get translation from mouse position
			DirectX::SimpleMath::Vector3 translate = GetTranslation(i);

			// Switch between object constraint
			switch (constraint)
			{
			case CONSTRAINT::X:
			{
				// Translate selected object based on mouse drag
				sceneGraph[m_selectedObjectIDs[i]].posX = translate.x;

				// Setup for game transform			
				translate.y = sceneGraph[m_selectedObjectIDs[i]].posY;
				translate.z = sceneGraph[m_selectedObjectIDs[i]].posZ;
			}
			break;
			case CONSTRAINT::Y:
			{
				// Translate selected object based on mouse drag
				sceneGraph[m_selectedObjectIDs[i]].posY = translate.y;

				// Setup for game transform			
				translate.x = sceneGraph[m_selectedObjectIDs[i]].posX;
				translate.z = sceneGraph[m_selectedObjectIDs[i]].posZ;
			}
			break;
			case CONSTRAINT::Z:
			{
				// Translate selected object based on mouse drag
				sceneGraph[m_selectedObjectIDs[i]].posZ = translate.z;

				// Setup for game transform			
				translate.x = sceneGraph[m_selectedObjectIDs[i]].posX;
				translate.y = sceneGraph[m_selectedObjectIDs[i]].posY;
			}
			break;
			case CONSTRAINT::XY:
			{
				// Translate selected object based on mouse drag
				sceneGraph[m_selectedObjectIDs[i]].posX = translate.x;
				sceneGraph[m_selectedObjectIDs[i]].posY = translate.y;

				// Setup for game transform			
				translate.z = sceneGraph[m_selectedObjectIDs[i]].posZ;
			}
			break;
			case CONSTRAINT::XZ:
			{
				// Translate selected object based on mouse drag
				sceneGraph[m_selectedObjectIDs[i]].posX = translate.x;
				sceneGraph[m_selectedObjectIDs[i]].posZ = translate.z;

				// Setup for game transform			
				translate.y = sceneGraph[m_selectedObjectIDs[i]].posY;
			}
			break;
			case CONSTRAINT::YZ:
			{
				// Translate selected object based on mouse drag
				sceneGraph[m_selectedObjectIDs[i]].posY = translate.y;
				sceneGraph[m_selectedObjectIDs[i]].posZ = translate.z;

				// Setup for game transform
				translate.x = sceneGraph[m_selectedObjectIDs[i]].posX;
			}
			break;
			case CONSTRAINT::NA:
			{
				// Translate selected object based on mouse drag
				sceneGraph[m_selectedObjectIDs[i]].posX = translate.x;
				sceneGraph[m_selectedObjectIDs[i]].posY = translate.y;
				sceneGraph[m_selectedObjectIDs[i]].posZ = translate.z;
			}
			break;
			}

			// Apply game transform
			m_game->SetTransform(m_selectedObjectIDs[i], OBJECT_FUNCTION::TRANSLATE, translate);
		}
	}
}

// Get scale for object manipulation via mouse drag
DirectX::SimpleMath::Vector3 ObjectManager::GetScale(int ID)
{
	// Temp storage 
	DirectX::SimpleMath::Vector2 previous = m_input->mousePosPrevious;
	DirectX::SimpleMath::Vector2 current = m_input->mousePos;
	DirectX::SimpleMath::Vector3 object = m_game->GetDisplayList()[m_selectedObjectIDs[ID]].m_position;
	DirectX::SimpleMath::Vector3 s = m_game->GetDisplayList()[m_selectedObjectIDs[ID]].m_scale;

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
	scale.x = m_storedObjectScales[ID].x + (pickingC.x - pickingP.x);
	scale.y = m_storedObjectScales[ID].y + (pickingC.y - pickingP.y);
	scale.z = m_storedObjectScales[ID].z + (pickingC.z - pickingP.z);

	// If scale is below minimum scale
	if (scale.x < .1f) { scale.x = .1f; }
	if (scale.y < .1f) { scale.y = .1f; }
	if (scale.z < .1f) { scale.z = .1f; }

	// Return manipulated scale
	return scale;
}

// Get rotation for object manipulation via mouse drag
DirectX::SimpleMath::Vector3 ObjectManager::GetRotation(int ID)
{
	// Temp storage 
	DirectX::SimpleMath::Vector2 previous = m_input->mousePosPrevious;
	DirectX::SimpleMath::Vector2 current = m_input->mousePos;
	DirectX::SimpleMath::Vector3 object = m_game->GetDisplayList()[m_selectedObjectIDs[ID]].m_position;

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
	rotate.x = m_storedObjectRotations[ID].x + (pickingC.x - pickingP.x) * 50.f;
	rotate.y = m_storedObjectRotations[ID].y + (pickingC.y - pickingP.y) * 50.f;
	rotate.z = m_storedObjectRotations[ID].z + (pickingC.z - pickingP.z) * 50.f;

	// Return manipulated rotation
	return rotate;
}

// Get translation for object manipulation via mouse drag
DirectX::SimpleMath::Vector3 ObjectManager::GetTranslation(int ID)
{
	// Temp storage 
	DirectX::SimpleMath::Vector2 previous = m_input->mousePosPrevious;
	DirectX::SimpleMath::Vector2 current = m_input->mousePos;
	DirectX::SimpleMath::Vector3 object = m_game->GetDisplayList()[m_selectedObjectIDs[ID]].m_position;

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
	position.x = m_storedObjectTranslations[ID].x + (pickingC.x - pickingP.x);
	position.y = m_storedObjectTranslations[ID].y + (pickingC.y - pickingP.y);
	position.z = m_storedObjectTranslations[ID].z + (pickingC.z - pickingP.z);

	// Return manipulated translation
	return position;
}