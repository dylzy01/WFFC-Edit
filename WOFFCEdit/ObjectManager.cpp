#include "ObjectManager.h"

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
