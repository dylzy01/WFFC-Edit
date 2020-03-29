#include "SQL.h"

sqlite3 *SQL::m_databaseConnection;
sqlite3_stmt *SQL::m_resultObject, *SQL::m_resultChunk;

// Send query to database
void SQL::SendQuery(char * query, bool isObject)
{
	if (isObject) { sqlite3_prepare_v2(m_databaseConnection, query, -1, &m_resultObject, 0); }
	else { sqlite3_prepare_v2(m_databaseConnection, query, -1, &m_resultChunk, 0); }
}

// Create object from object table
SceneObject SQL::CreateObject()
{
	// Setup temp object
	SceneObject object;

	object.ID = sqlite3_column_int(m_resultObject, 0);
	object.chunk_ID = sqlite3_column_int(m_resultObject, 1);
	object.model_path = reinterpret_cast<const char*>(sqlite3_column_text(m_resultObject, 2));
	object.tex_diffuse_path = reinterpret_cast<const char*>(sqlite3_column_text(m_resultObject, 3));
	object.posX = sqlite3_column_double(m_resultObject, 4);
	object.posY = sqlite3_column_double(m_resultObject, 5);
	object.posZ = sqlite3_column_double(m_resultObject, 6);
	object.rotX = sqlite3_column_double(m_resultObject, 7);
	object.rotY = sqlite3_column_double(m_resultObject, 8);
	object.rotZ = sqlite3_column_double(m_resultObject, 9);
	object.scaX = sqlite3_column_double(m_resultObject, 10);
	object.scaY = sqlite3_column_double(m_resultObject, 11);
	object.scaZ = sqlite3_column_double(m_resultObject, 12);
	object.render = sqlite3_column_int(m_resultObject, 13);
	object.collision = sqlite3_column_int(m_resultObject, 14);
	object.collision_mesh = reinterpret_cast<const char*>(sqlite3_column_text(m_resultObject, 15));
	object.collectable = sqlite3_column_int(m_resultObject, 16);
	object.destructable = sqlite3_column_int(m_resultObject, 17);
	object.health_amount = sqlite3_column_int(m_resultObject, 18);
	object.editor_render = sqlite3_column_int(m_resultObject, 19);
	object.editor_texture_vis = sqlite3_column_int(m_resultObject, 20);
	object.editor_normals_vis = sqlite3_column_int(m_resultObject, 21);
	object.editor_collision_vis = sqlite3_column_int(m_resultObject, 22);
	object.editor_pivot_vis = sqlite3_column_int(m_resultObject, 23);
	object.pivotX = sqlite3_column_double(m_resultObject, 24);
	object.pivotY = sqlite3_column_double(m_resultObject, 25);
	object.pivotZ = sqlite3_column_double(m_resultObject, 26);
	object.snapToGround = sqlite3_column_int(m_resultObject, 27);
	object.AINode = sqlite3_column_int(m_resultObject, 28);
	object.audio_path = reinterpret_cast<const char*>(sqlite3_column_text(m_resultObject, 29));
	object.volume = sqlite3_column_double(m_resultObject, 30);
	object.pitch = sqlite3_column_double(m_resultObject, 31);
	object.pan = sqlite3_column_int(m_resultObject, 32);
	object.one_shot = sqlite3_column_int(m_resultObject, 33);
	object.play_on_init = sqlite3_column_int(m_resultObject, 34);
	object.play_in_editor = sqlite3_column_int(m_resultObject, 35);
	object.min_dist = sqlite3_column_double(m_resultObject, 36);
	object.max_dist = sqlite3_column_double(m_resultObject, 37);
	object.camera = sqlite3_column_int(m_resultObject, 38);
	object.path_node = sqlite3_column_int(m_resultObject, 39);
	object.path_node_start = sqlite3_column_int(m_resultObject, 40);
	object.path_node_end = sqlite3_column_int(m_resultObject, 41);
	object.parent_id = sqlite3_column_int(m_resultObject, 42);
	object.editor_wireframe = sqlite3_column_int(m_resultObject, 43);
	object.name = reinterpret_cast<const char*>(sqlite3_column_text(m_resultObject, 44));

	object.light_type = sqlite3_column_int(m_resultObject, 45);
	object.light_diffuse_r = sqlite3_column_double(m_resultObject, 46);
	object.light_diffuse_g = sqlite3_column_double(m_resultObject, 47);
	object.light_diffuse_b = sqlite3_column_double(m_resultObject, 48);
	object.light_specular_r = sqlite3_column_double(m_resultObject, 49);
	object.light_specular_g = sqlite3_column_double(m_resultObject, 50);
	object.light_specular_b = sqlite3_column_double(m_resultObject, 51);
	object.light_spot_cutoff = sqlite3_column_double(m_resultObject, 52);
	object.light_constant = sqlite3_column_double(m_resultObject, 53);
	object.light_linear = sqlite3_column_double(m_resultObject, 54);
	object.light_quadratic = sqlite3_column_double(m_resultObject, 55);

	// Return new object
	return object;
}

// Create chunk from chunk table
ChunkObject SQL::CreateChunk()
{
	// Setup temp chunk object
	ChunkObject chunk;

	sqlite3_step(m_resultChunk);
	chunk.ID = sqlite3_column_int(m_resultChunk, 0);
	chunk.name = reinterpret_cast<const char*>(sqlite3_column_text(m_resultChunk, 1));
	chunk.chunk_x_size_metres = sqlite3_column_int(m_resultChunk, 2);
	chunk.chunk_y_size_metres = sqlite3_column_int(m_resultChunk, 3);
	chunk.chunk_base_resolution = sqlite3_column_int(m_resultChunk, 4);
	chunk.heightmap_path = reinterpret_cast<const char*>(sqlite3_column_text(m_resultChunk, 5));
	chunk.tex_diffuse_path = reinterpret_cast<const char*>(sqlite3_column_text(m_resultChunk, 6));
	chunk.tex_splat_alpha_path = reinterpret_cast<const char*>(sqlite3_column_text(m_resultChunk, 7));
	chunk.tex_splat_1_path = reinterpret_cast<const char*>(sqlite3_column_text(m_resultChunk, 8));
	chunk.tex_splat_2_path = reinterpret_cast<const char*>(sqlite3_column_text(m_resultChunk, 9));
	chunk.tex_splat_3_path = reinterpret_cast<const char*>(sqlite3_column_text(m_resultChunk, 10));
	chunk.tex_splat_4_path = reinterpret_cast<const char*>(sqlite3_column_text(m_resultChunk, 11));
	chunk.render_wireframe = sqlite3_column_int(m_resultChunk, 12);
	chunk.render_normals = sqlite3_column_int(m_resultChunk, 13);
	chunk.tex_diffuse_tiling = sqlite3_column_int(m_resultChunk, 14);
	chunk.tex_splat_1_tiling = sqlite3_column_int(m_resultChunk, 15);
	chunk.tex_splat_2_tiling = sqlite3_column_int(m_resultChunk, 16);
	chunk.tex_splat_3_tiling = sqlite3_column_int(m_resultChunk, 17);
	chunk.tex_splat_4_tiling = sqlite3_column_int(m_resultChunk, 18);

	// Return new chunk object
	return chunk;
}

// Add new object to object table
bool SQL::AddObject(SceneObject object)
{
	// Controller
	int rc = -1;

	// Setup command stream from object data
	std::stringstream ss;
	{
		ss << "INSERT INTO Objects "
			<< "VALUES(" << object.ID << ","
			<< object.chunk_ID << ","
			<< "'" << object.model_path << "'" << ","
			<< "'" << object.tex_diffuse_path << "'" << ","
			<< object.posX << ","
			<< object.posY << ","
			<< object.posZ << ","
			<< object.rotX << ","
			<< object.rotY << ","
			<< object.rotZ << ","
			<< object.scaX << ","
			<< object.scaY << ","
			<< object.scaZ << ","
			<< object.render << ","
			<< object.collision << ","
			<< "'" << object.collision_mesh << "'" << ","
			<< object.collectable << ","
			<< object.destructable << ","
			<< object.health_amount << ","
			<< object.editor_render << ","
			<< object.editor_texture_vis << ","
			<< object.editor_normals_vis << ","
			<< object.editor_collision_vis << ","
			<< object.editor_pivot_vis << ","
			<< object.pivotX << ","
			<< object.pivotY << ","
			<< object.pivotZ << ","
			<< object.snapToGround << ","
			<< object.AINode << ","
			<< "'" << object.audio_path << "'" << ","
			<< object.volume << ","
			<< object.pitch << ","
			<< object.pan << ","
			<< object.one_shot << ","
			<< object.play_on_init << ","
			<< object.play_in_editor << ","
			<< object.min_dist << ","
			<< object.max_dist << ","
			<< object.camera << ","
			<< object.path_node << ","
			<< object.path_node_start << ","
			<< object.path_node_end << ","
			<< object.parent_id << ","
			<< object.editor_wireframe << ","
			<< "'" << object.name << "'" << ","

			<< object.light_type << ","
			<< object.light_diffuse_r << ","
			<< object.light_diffuse_g << ","
			<< object.light_diffuse_b << ","
			<< object.light_specular_r << ","
			<< object.light_specular_g << ","
			<< object.light_specular_b << ","
			<< object.light_spot_cutoff << ","
			<< object.light_constant << ","
			<< object.light_linear << ","
			<< object.light_quadratic

			<< ")";
	}

	// Add stream to query string
	std::string command = ss.str();
	rc = sqlite3_prepare_v2(m_databaseConnection, command.c_str(), -1, &m_resultObject, 0);
	sqlite3_step(m_resultObject);

	if (rc) { return false; }
	else { return true; }
}

// Save updated scene graph
bool SQL::SaveWorld(std::vector<SceneObject> sceneGraph)
{
	// Controller
	int rc = -1;
	
	// Delete all current objects
	SQL::SendQuery("DELETE FROM Objects", true);
	SQL::SetObjectStep();

	// Loop through scene graph
	for (int i = 0; i < sceneGraph.size(); ++i)
	{
		// Setup command stream from current object data
		std::stringstream ss;
		ss << "INSERT INTO Objects "
			<< "VALUES(" << sceneGraph.at(i).ID << ","
			<< sceneGraph.at(i).chunk_ID << ","
			<< "'" << sceneGraph.at(i).model_path << "'" << ","
			<< "'" << sceneGraph.at(i).tex_diffuse_path << "'" << ","
			<< sceneGraph.at(i).posX << ","
			<< sceneGraph.at(i).posY << ","
			<< sceneGraph.at(i).posZ << ","
			<< sceneGraph.at(i).rotX << ","
			<< sceneGraph.at(i).rotY << ","
			<< sceneGraph.at(i).rotZ << ","
			<< sceneGraph.at(i).scaX << ","
			<< sceneGraph.at(i).scaY << ","
			<< sceneGraph.at(i).scaZ << ","
			<< sceneGraph.at(i).render << ","
			<< sceneGraph.at(i).collision << ","
			<< "'" << sceneGraph.at(i).collision_mesh << "'" << ","
			<< sceneGraph.at(i).collectable << ","
			<< sceneGraph.at(i).destructable << ","
			<< sceneGraph.at(i).health_amount << ","
			<< sceneGraph.at(i).editor_render << ","
			<< sceneGraph.at(i).editor_texture_vis << ","
			<< sceneGraph.at(i).editor_normals_vis << ","
			<< sceneGraph.at(i).editor_collision_vis << ","
			<< sceneGraph.at(i).editor_pivot_vis << ","
			<< sceneGraph.at(i).pivotX << ","
			<< sceneGraph.at(i).pivotY << ","
			<< sceneGraph.at(i).pivotZ << ","
			<< sceneGraph.at(i).snapToGround << ","
			<< sceneGraph.at(i).AINode << ","
			<< "'" << sceneGraph.at(i).audio_path << "'" << ","
			<< sceneGraph.at(i).volume << ","
			<< sceneGraph.at(i).pitch << ","
			<< sceneGraph.at(i).pan << ","
			<< sceneGraph.at(i).one_shot << ","
			<< sceneGraph.at(i).play_on_init << ","
			<< sceneGraph.at(i).play_in_editor << ","
			<< sceneGraph.at(i).min_dist << ","
			<< sceneGraph.at(i).max_dist << ","
			<< sceneGraph.at(i).camera << ","
			<< sceneGraph.at(i).path_node << ","
			<< sceneGraph.at(i).path_node_start << ","
			<< sceneGraph.at(i).path_node_end << ","
			<< sceneGraph.at(i).parent_id << ","
			<< sceneGraph.at(i).editor_wireframe << ","
			<< "'" << sceneGraph.at(i).name << "'" << ","

			<< sceneGraph.at(i).light_type << ","
			<< sceneGraph.at(i).light_diffuse_r << ","
			<< sceneGraph.at(i).light_diffuse_g << ","
			<< sceneGraph.at(i).light_diffuse_b << ","
			<< sceneGraph.at(i).light_specular_r << ","
			<< sceneGraph.at(i).light_specular_g << ","
			<< sceneGraph.at(i).light_specular_b << ","
			<< sceneGraph.at(i).light_spot_cutoff << ","
			<< sceneGraph.at(i).light_constant << ","
			<< sceneGraph.at(i).light_linear << ","
			<< sceneGraph.at(i).light_quadratic

			<< ")";

		// Add stream to query string
		std::string command = ss.str();
		rc = sqlite3_prepare_v2(m_databaseConnection, command.c_str(), -1, &m_resultObject, 0);
		sqlite3_step(m_resultObject);
	}

	if (rc) { return false; }
	else { return true; }
}

bool SQL::PaintTerrain(DirectX::SimpleMath::Vector2 location, LANDSCAPE_PAINT paint)
{
	// Controller
	int rc = -1;



	if (rc) { return false; }
	else { return true; }
}
