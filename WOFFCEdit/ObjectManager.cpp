#include "ObjectManager.h"

Game * ObjectManager::m_game;
InputCommands * ObjectManager::m_input;
std::vector<int> ObjectManager::m_selectedObjectIDs;
std::vector<DirectX::SimpleMath::Vector3> ObjectManager::m_storedObjectScales;
std::vector<DirectX::SimpleMath::Vector3> ObjectManager::m_storedObjectRotations;
std::vector<DirectX::SimpleMath::Vector3> ObjectManager::m_storedObjectTranslations;

// Spawn an object at a location
SceneObject ObjectManager::Spawn(OBJECT_SPAWN spawn, DirectX::SimpleMath::Vector3 position, 
	std::vector<SceneObject> & sceneGraph)
{
	// Setup temp object
	SceneObject object;

	// Define object values
	object.ID = sceneGraph.size() + 1;
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
	object.collectable = false;
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
	object.light_type = 1;
	object.light_diffuse_r = 2.f;
	object.light_diffuse_g = 3.f;
	object.light_diffuse_b = 4.f;
	object.light_specular_r = 5.f;
	object.light_specular_g = 6.f;
	object.light_specular_b = 7.f;
	object.light_spot_cutoff = 8.f;
	object.light_constant = 9.f;
	object.light_linear = 0.f;
	object.light_quadratic = 1.f;

	// Switch between spawn
	switch (spawn)
	{
		// Residential //////////////////////////////////////////////////////////////////////////
	case OBJECT_SPAWN::HOUSE_ONE:
	{
		// Set object to house #1
		object.m_type = MODEL_TYPE::NOT_WATER;
		object.model_path = "database/data/house0.cmo";
		object.tex_diffuse_path = "database/data/house0.dds";

		object.name = "House #" + object.ID;
	}
	break;
	case OBJECT_SPAWN::HOUSE_TWO:
	{
		// Set object to house #2
		object.m_type = MODEL_TYPE::NOT_WATER;
		object.model_path = "database/data/house1.cmo";
		object.tex_diffuse_path = "database/data/house1.dds";

		object.name = "House #" + object.ID;
	}
	break;
	case OBJECT_SPAWN::CAVE:
	{
		// Set object to cave
		object.m_type = MODEL_TYPE::NOT_WATER;
		object.model_path = "database/data/cave.cmo";
		object.tex_diffuse_path = "database/data/cave.dds";

		object.name = "Cave #" + object.ID;
	}
	break;
	// Props ////////////////////////////////////////////////////////////////////////////////
	case OBJECT_SPAWN::BRIDGE:
	{
		// Set object to bridge
		object.m_type = MODEL_TYPE::NOT_WATER;
		object.model_path = "database/data/bridge.cmo";
		object.tex_diffuse_path = "database/data/sand.dds";
	}
	break;
	case OBJECT_SPAWN::FENCE:
	{
		// Set object to fence
		object.m_type = MODEL_TYPE::NOT_WATER;
		object.model_path = "database/data/fence.cmo";
		object.tex_diffuse_path = "database/data/sand.dds";
	}
	break;
	case OBJECT_SPAWN::BOAT:
	{
		// Set object to boat
		object.m_type = MODEL_TYPE::NOT_WATER;
		object.model_path = "database/data/boat.cmo";
		object.tex_diffuse_path = "database/data/sand.dds";
	}
	break;
	// Nature ///////////////////////////////////////////////////////////////////////////////
	case OBJECT_SPAWN::GRASS:
	{
		// Set object to grass
		object.m_type = MODEL_TYPE::NOT_WATER;
		object.model_path = "database/data/grass.cmo";
		object.tex_diffuse_path = "database/data/grass.dds";

		object.name = "Grass #" + object.ID;
	}
	break;
	case OBJECT_SPAWN::TREE_ONE:
	{
		// Set object to tree
		object.m_type = MODEL_TYPE::NOT_WATER;
		object.model_path = "database/data/tree0.cmo";
		object.tex_diffuse_path = "database/data/tree0.dds";

		object.name = "Tree #" + object.ID;
	}
	break;
	case OBJECT_SPAWN::TREE_TWO:
	{
		// Set object to tree
		object.m_type = MODEL_TYPE::NOT_WATER;
		object.model_path = "database/data/tree1.cmo";
		object.tex_diffuse_path = "database/data/tree1.dds";

		object.name = "Tree #" + object.ID;
	}
	break;
	case OBJECT_SPAWN::WATER:
	{
		// Set object to water
		object.m_type = MODEL_TYPE::WATER;
		object.model_path = "database/data/water.cmo";
		object.tex_diffuse_path = "database/data/water.dds";
		object.name = "Water #" + object.ID;
	}
	break;
	}

	// Add new object to scene graph
	sceneGraph.push_back(object);

	// Add new object to database
	SQLManager::AddObject(object);

	// Return object
	return object;
}

// Remove an object from scene graph & database
void ObjectManager::Remove(int i, std::vector<int> & IDs, std::vector<SceneObject> & sceneGraph)
{
	// Remove selected object from database
	//SQLManager::RemoveObject(sceneGraph[IDs[i]]);
	//
	//// Remove selected object from scene graph
	//sceneGraph.erase(sceneGraph.begin() + IDs[i]);

	//// Remove selected object from selected objects list
	//IDs.erase(IDs.begin() + i);

	//// Overwrite selected objects list
	//m_selectedObjectIDs = IDs;
}

// Temporarily store details of all objects
void ObjectManager::Store(std::vector<int> IDs)
{
	// Reset storages vectors
	m_storedObjectScales.clear();
	m_storedObjectRotations.clear();
	m_storedObjectTranslations.clear();
	m_selectedObjectIDs.clear();

	// Loop through selected objects
	for (int i = 0; i < IDs.size(); ++i)
	{
		// If selected ID isn't -1
		if (IDs[i] != -1)
		{
			m_selectedObjectIDs.push_back(IDs[i]);
			// Add selected object details to storage
			m_storedObjectScales.push_back(m_game->GetDisplayList()[m_selectedObjectIDs[i]].m_scale);
			m_storedObjectRotations.push_back(m_game->GetDisplayList()[m_selectedObjectIDs[i]].m_orientation);
			m_storedObjectTranslations.push_back(m_game->GetDisplayList()[m_selectedObjectIDs[i]].m_position);			
		}
	}
}

// Transform selected objects
void ObjectManager::Transform(OBJECT_FUNCTION function, OBJECT_CONSTRAINT constraint, std::vector<int> IDs, std::vector<SceneObject>& sceneGraph)
{
	// Switch between function
	switch (function)
	{
	case OBJECT_FUNCTION::SCALE: Scale(constraint, IDs, sceneGraph); break;
	case OBJECT_FUNCTION::ROTATE: Rotate(constraint, IDs, sceneGraph); break;
	case OBJECT_FUNCTION::TRANSLATE: Translate(constraint, IDs, sceneGraph); break;
	}
}

// Scale selected objects
void ObjectManager::Scale(OBJECT_CONSTRAINT constraint, std::vector<int> IDs, std::vector<SceneObject>& sceneGraph)
{
	// Overwrite selected object IDs
	m_selectedObjectIDs = IDs;

	// Loop through selected objects
	for (int i = 0; i < m_selectedObjectIDs.size(); ++i)
	{
		DirectX::SimpleMath::Vector3 scale = GetScale(i);

		// Switch between object constraint
		switch (constraint)
		{
		case OBJECT_CONSTRAINT::X:
		{
			// Scale selected object based on mouse drag
			sceneGraph[m_selectedObjectIDs[i]].scaX = scale.x;
		}
		break;
		case OBJECT_CONSTRAINT::Y:
		{
			// Scale selected object based on mouse drag
			sceneGraph[m_selectedObjectIDs[i]].scaY = scale.y;
		}
		break;
		case OBJECT_CONSTRAINT::Z:
		{
			// Scale selected object based on mouse drag
			sceneGraph[m_selectedObjectIDs[i]].scaZ = scale.z;
		}
		break;
		case OBJECT_CONSTRAINT::XY:
		{
			// Scale selected object based on mouse drag
			sceneGraph[m_selectedObjectIDs[i]].scaX = scale.x;
			sceneGraph[m_selectedObjectIDs[i]].scaY = scale.y;
		}
		break;
		case OBJECT_CONSTRAINT::XZ:
		{
			// Scale selected object based on mouse drag
			sceneGraph[m_selectedObjectIDs[i]].scaX = scale.x;
			sceneGraph[m_selectedObjectIDs[i]].scaZ = scale.z;
		}
		break;
		case OBJECT_CONSTRAINT::YZ:
		{
			// Scale selected object based on mouse drag
			sceneGraph[m_selectedObjectIDs[i]].scaY = scale.y;
			sceneGraph[m_selectedObjectIDs[i]].scaZ = scale.z;
		}
		break;
		case OBJECT_CONSTRAINT::ALL:
		{
			// Scale selected object based on mouse drag
			sceneGraph[m_selectedObjectIDs[i]].scaX = scale.x;
			sceneGraph[m_selectedObjectIDs[i]].scaY = scale.y;
			sceneGraph[m_selectedObjectIDs[i]].scaZ = scale.z;
		}
		break;
		}
	}
}

// Rotate selected objects
void ObjectManager::Rotate(OBJECT_CONSTRAINT constraint, std::vector<int> IDs, std::vector<SceneObject>& sceneGraph)
{
	// Overwrite selected object IDs
	m_selectedObjectIDs = IDs;

	// Loop through selected objects
	for (int i = 0; i < m_selectedObjectIDs.size(); ++i)
	{
		DirectX::SimpleMath::Vector3 rotate = GetRotation(i);

		// Switch between object constraint
		switch (constraint)
		{
		case OBJECT_CONSTRAINT::X:
		{
			// Rotate selected object based on mouse drag
			sceneGraph[m_selectedObjectIDs[i]].rotX = rotate.x;
		}
		break;
		case OBJECT_CONSTRAINT::Y:
		{
			// Rotate selected object based on mouse drag
			sceneGraph[m_selectedObjectIDs[i]].rotY = rotate.y;
		}
		break;
		case OBJECT_CONSTRAINT::Z:
		{
			// Rotate selected object based on mouse drag
			sceneGraph[m_selectedObjectIDs[i]].rotZ = rotate.z;
		}
		break;
		case OBJECT_CONSTRAINT::XY:
		{
			// Rotate selected object based on mouse drag
			sceneGraph[m_selectedObjectIDs[i]].rotX = rotate.x;
			sceneGraph[m_selectedObjectIDs[i]].rotY = rotate.y;
		}
		break;
		case OBJECT_CONSTRAINT::XZ:
		{
			// Rotate selected object based on mouse drag
			sceneGraph[m_selectedObjectIDs[i]].rotX = rotate.x;
			sceneGraph[m_selectedObjectIDs[i]].rotZ = rotate.z;
		}
		break;
		case OBJECT_CONSTRAINT::YZ:
		{
			// Rotate selected object based on mouse drag
			sceneGraph[m_selectedObjectIDs[i]].rotY = rotate.y;
			sceneGraph[m_selectedObjectIDs[i]].rotZ = rotate.z;
		}
		break;
		case OBJECT_CONSTRAINT::ALL:
		{
			// Rotate selected object based on mouse drag
			sceneGraph[m_selectedObjectIDs[i]].rotX = rotate.x;
			sceneGraph[m_selectedObjectIDs[i]].rotY = rotate.y;
			sceneGraph[m_selectedObjectIDs[i]].rotZ = rotate.z;
		}
		break;
		}
	}
}

// Translate selected objects
void ObjectManager::Translate(OBJECT_CONSTRAINT constraint, std::vector<int> IDs, std::vector<SceneObject>& sceneGraph)
{
	// Overwrite selected object IDs
	m_selectedObjectIDs = IDs;

	// Loop through selected objects
	for (int i = 0; i < m_selectedObjectIDs.size(); ++i)
	{
		DirectX::SimpleMath::Vector3 translate = GetTranslation(i);

		// Switch between object constraint
		switch (constraint)
		{
		case OBJECT_CONSTRAINT::X:
		{
			// Translate selected object based on mouse drag
			sceneGraph[m_selectedObjectIDs[i]].posX = translate.x;
		}
		break;
		case OBJECT_CONSTRAINT::Y:
		{
			// Translate selected object based on mouse drag
			sceneGraph[m_selectedObjectIDs[i]].posY = translate.y;
		}
		break;
		case OBJECT_CONSTRAINT::Z:
		{
			// Translate selected object based on mouse drag
			sceneGraph[m_selectedObjectIDs[i]].posZ = translate.z;
		}
		break;
		case OBJECT_CONSTRAINT::XY:
		{
			// Translate selected object based on mouse drag
			sceneGraph[m_selectedObjectIDs[i]].posX = translate.x;
			sceneGraph[m_selectedObjectIDs[i]].posY = translate.y;
		}
		break;
		case OBJECT_CONSTRAINT::XZ:
		{
			// Translate selected object based on mouse drag
			sceneGraph[m_selectedObjectIDs[i]].posX = translate.x;
			sceneGraph[m_selectedObjectIDs[i]].posZ = translate.z;
		}
		break;
		case OBJECT_CONSTRAINT::YZ:
		{
			// Translate selected object based on mouse drag
			sceneGraph[m_selectedObjectIDs[i]].posY = translate.y;
			sceneGraph[m_selectedObjectIDs[i]].posZ = translate.z;
		}
		break;
		case OBJECT_CONSTRAINT::ALL:
		{
			// Translate selected object based on mouse drag
			sceneGraph[m_selectedObjectIDs[i]].posX = translate.x;
			sceneGraph[m_selectedObjectIDs[i]].posY = translate.y;
			sceneGraph[m_selectedObjectIDs[i]].posZ = translate.z;
		}
		break;
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