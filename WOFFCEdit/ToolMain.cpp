#include "ToolMain.h"
#include "resource.h"
#include <vector>
#include <sstream>

//
//ToolMain Class
ToolMain::ToolMain()
{
	m_currentChunk = 0;		//default value
	///m_selectedObject = 0;	//initial selection ID
	m_sceneGraph.clear();	//clear the vector for the scenegraph
	m_databaseConnection = NULL;

	//zero input commands
	m_toolInputCommands.W			= false;
	m_toolInputCommands.S			= false;
	m_toolInputCommands.A			= false;
	m_toolInputCommands.D			= false;
	m_toolInputCommands.mouseLeft	= false;
	m_toolInputCommands.mouseRight	= false;
	m_toolInputCommands.mouseWheel	= false;
	m_toolInputCommands.mouseDrag	= false;
	m_toolInputCommands.escape		= false;
	m_toolInputCommands.pickOnce	= true;
	m_toolInputCommands.storeOnce	= true;
	m_toolInputCommands.toggle		= true;
	m_toolInputCommands.mousePos	= DirectX::SimpleMath::Vector2(0.f, 0.f);
}

void ToolMain::onActionInitialise(HWND handle, int width, int height)
{
	//window size, handle etc for directX
	m_width		= width;
	m_height	= height;
	
	// Setup game
	m_d3dRenderer.Initialize(handle, m_width, m_height);

	// Set static mouse game & input commands
	MouseManager::SetGame(&m_d3dRenderer);
	MouseManager::SetInput(&m_toolInputCommands);

	// Estable database connection
	if (SQLManager::Connect()) { TRACE("Database connection: fail"); }
	else { TRACE("Database connection: success"); }

	// Load data from database
	onActionLoad();
}

void ToolMain::onActionLoad()
{
	//load current chunk and objects into lists
	if (!m_sceneGraph.empty())		//is the vector empty
	{
		m_sceneGraph.clear();		//if not, empty it
	}

	// Objects ///////////////////////////////////////////////////

	// Database query to retrieve all records from objects table
	SQLManager::SendQuery("SELECT * FROM Objects", true);

	// Loop through each row in results
	while (SQLManager::GetObjectStep() == SQLITE_ROW)
	{
		// Create & store an object from table data
		m_sceneGraph.push_back(SQLManager::CreateObject());
	}

	// Chunk /////////////////////////////////////////////////////

	// Database query to retrieve all records from chunks table
	SQLManager::SendQuery("SELECT * FROM Chunks", false);

	// Create & store chunk from table data
	m_chunk = SQLManager::CreateChunk();

	// Process results into renderable
	m_d3dRenderer.BuildDisplayList(&m_sceneGraph);

	// Build the renderable chunk
	m_d3dRenderer.BuildDisplayChunk(&m_chunk);
}

void ToolMain::onActionLoad_()
{
	//load current chunk and objects into lists
	if (!m_sceneGraph.empty())		//is the vector empty
	{
		m_sceneGraph.clear();		//if not, empty it
	}
	
	//SQL
	int rc;
	char *sqlCommand;
	char *ErrMSG = 0;
	sqlite3_stmt *pResults;								//results of the query
	sqlite3_stmt *pResultsChunk;

	//OBJECTS IN THE WORLD
	//prepare SQL Text
	sqlCommand = "SELECT * from Objects";				//sql command which will return all records from the objects table.
	//Send Command and fill result object
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResults, 0 );
	
	//loop for each row in results until there are no more rows.  ie for every row in the results. We create and object
	while (sqlite3_step(pResults) == SQLITE_ROW)
	{	
		SceneObject newSceneObject;
		newSceneObject.ID = sqlite3_column_int(pResults, 0);
		newSceneObject.chunk_ID = sqlite3_column_int(pResults, 1);
		newSceneObject.model_path		= reinterpret_cast<const char*>(sqlite3_column_text(pResults, 2));
		newSceneObject.tex_diffuse_path = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 3));
		newSceneObject.posX = sqlite3_column_double(pResults, 4);
		newSceneObject.posY = sqlite3_column_double(pResults, 5);
		newSceneObject.posZ = sqlite3_column_double(pResults, 6);
		newSceneObject.rotX = sqlite3_column_double(pResults, 7);
		newSceneObject.rotY = sqlite3_column_double(pResults, 8);
		newSceneObject.rotZ = sqlite3_column_double(pResults, 9);
		newSceneObject.scaX = sqlite3_column_double(pResults, 10);
		newSceneObject.scaY = sqlite3_column_double(pResults, 11);
		newSceneObject.scaZ = sqlite3_column_double(pResults, 12);
		newSceneObject.render = sqlite3_column_int(pResults, 13);
		newSceneObject.collision = sqlite3_column_int(pResults, 14);
		newSceneObject.collision_mesh = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 15));
		newSceneObject.collectable = sqlite3_column_int(pResults, 16);
		newSceneObject.destructable = sqlite3_column_int(pResults, 17);
		newSceneObject.health_amount = sqlite3_column_int(pResults, 18);
		newSceneObject.editor_render = sqlite3_column_int(pResults, 19);
		newSceneObject.editor_texture_vis = sqlite3_column_int(pResults, 20);
		newSceneObject.editor_normals_vis = sqlite3_column_int(pResults, 21);
		newSceneObject.editor_collision_vis = sqlite3_column_int(pResults, 22);
		newSceneObject.editor_pivot_vis = sqlite3_column_int(pResults, 23);
		newSceneObject.pivotX = sqlite3_column_double(pResults, 24);
		newSceneObject.pivotY = sqlite3_column_double(pResults, 25);
		newSceneObject.pivotZ = sqlite3_column_double(pResults, 26);
		newSceneObject.snapToGround = sqlite3_column_int(pResults, 27);
		newSceneObject.AINode = sqlite3_column_int(pResults, 28);
		newSceneObject.audio_path = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 29));
		newSceneObject.volume = sqlite3_column_double(pResults, 30);
		newSceneObject.pitch = sqlite3_column_double(pResults, 31);
		newSceneObject.pan = sqlite3_column_int(pResults, 32);
		newSceneObject.one_shot = sqlite3_column_int(pResults, 33);
		newSceneObject.play_on_init = sqlite3_column_int(pResults, 34);
		newSceneObject.play_in_editor = sqlite3_column_int(pResults, 35);
		newSceneObject.min_dist = sqlite3_column_double(pResults, 36);
		newSceneObject.max_dist = sqlite3_column_double(pResults, 37);
		newSceneObject.camera = sqlite3_column_int(pResults, 38);
		newSceneObject.path_node = sqlite3_column_int(pResults, 39);
		newSceneObject.path_node_start = sqlite3_column_int(pResults, 40);
		newSceneObject.path_node_end = sqlite3_column_int(pResults, 41);
		newSceneObject.parent_id = sqlite3_column_int(pResults, 42);
		newSceneObject.editor_wireframe = sqlite3_column_int(pResults, 43);
		newSceneObject.name = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 44));

		newSceneObject.light_type = sqlite3_column_int(pResults, 45);
		newSceneObject.light_diffuse_r = sqlite3_column_double(pResults, 46);
		newSceneObject.light_diffuse_g = sqlite3_column_double(pResults, 47);
		newSceneObject.light_diffuse_b = sqlite3_column_double(pResults, 48);
		newSceneObject.light_specular_r = sqlite3_column_double(pResults, 49);
		newSceneObject.light_specular_g = sqlite3_column_double(pResults, 50);
		newSceneObject.light_specular_b = sqlite3_column_double(pResults, 51);
		newSceneObject.light_spot_cutoff = sqlite3_column_double(pResults, 52);
		newSceneObject.light_constant = sqlite3_column_double(pResults, 53);
		newSceneObject.light_linear = sqlite3_column_double(pResults, 54);
		newSceneObject.light_quadratic = sqlite3_column_double(pResults, 55);
	

		//send completed object to scenegraph
		m_sceneGraph.push_back(newSceneObject);
	}

	//THE WORLD CHUNK
	//prepare SQL Text
	sqlCommand = "SELECT * from Chunks";				//sql command which will return all records from chunks table. There is only one tho.
														//Send Command and fill result object
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResultsChunk, 0);


	sqlite3_step(pResultsChunk);
	m_chunk.ID = sqlite3_column_int(pResultsChunk, 0);
	m_chunk.name = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 1));
	m_chunk.chunk_x_size_metres = sqlite3_column_int(pResultsChunk, 2);
	m_chunk.chunk_y_size_metres = sqlite3_column_int(pResultsChunk, 3);
	m_chunk.chunk_base_resolution = sqlite3_column_int(pResultsChunk, 4);
	m_chunk.heightmap_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 5));
	m_chunk.tex_diffuse_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 6));
	m_chunk.tex_splat_alpha_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 7));
	m_chunk.tex_splat_1_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 8));
	m_chunk.tex_splat_2_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 9));
	m_chunk.tex_splat_3_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 10));
	m_chunk.tex_splat_4_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 11));
	m_chunk.render_wireframe = sqlite3_column_int(pResultsChunk, 12);
	m_chunk.render_normals = sqlite3_column_int(pResultsChunk, 13);
	m_chunk.tex_diffuse_tiling = sqlite3_column_int(pResultsChunk, 14);
	m_chunk.tex_splat_1_tiling = sqlite3_column_int(pResultsChunk, 15);
	m_chunk.tex_splat_2_tiling = sqlite3_column_int(pResultsChunk, 16);
	m_chunk.tex_splat_3_tiling = sqlite3_column_int(pResultsChunk, 17);
	m_chunk.tex_splat_4_tiling = sqlite3_column_int(pResultsChunk, 18);


	//Process results into renderable
	m_d3dRenderer.BuildDisplayList(&m_sceneGraph);
	//build the renderable chunk 
	m_d3dRenderer.BuildDisplayChunk(&m_chunk);

}

void ToolMain::onActionSave()
{	
	// Update scene graph
	///m_sceneGraph = m_d3dRenderer.GetSceneGraph();
	m_d3dRenderer.SaveDisplayList();
	
	// Save terrain
	m_d3dRenderer.SaveDisplayChunk();
	
	// Database query to delete all records from objects table
	SQLManager::SendQuery("DELETE FROM Objects", true);
	SQLManager::SetObjectStep();

	// Save new scene graph
	if (SQLManager::SaveObjects(m_d3dRenderer.GetSceneGraph())) { MessageBox(NULL, L"World Saved", L"Notification", MB_OK); }
	else { MessageBox(NULL, L"World Failed to Save", L"Notification", MB_OK); }
}

void ToolMain::onActionSave_()
{
	//SQL
	int rc;
	char *sqlCommand;
	char *ErrMSG = 0;
	sqlite3_stmt *pResults;								//results of the query	

	//OBJECTS IN THE WORLD Delete them all
	//prepare SQL Text
	sqlCommand = "DELETE FROM Objects";	 //will delete the whole object table.   Slightly risky but hey.
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResults, 0);
	sqlite3_step(pResults);

	//Populate with our new objects
	std::wstring sqlCommand2;
	int numObjects = m_sceneGraph.size();	//Loop thru the scengraph.

	for (int i = 0; i < numObjects; i++)
	{
		std::stringstream command;
		command << "INSERT INTO Objects " 
			<<"VALUES(" << m_sceneGraph.at(i).ID << ","
			<< m_sceneGraph.at(i).chunk_ID  << ","
			<< "'" << m_sceneGraph.at(i).model_path <<"'" << ","
			<< "'" << m_sceneGraph.at(i).tex_diffuse_path << "'" << ","
			<< m_sceneGraph.at(i).posX << ","
			<< m_sceneGraph.at(i).posY << ","
			<< m_sceneGraph.at(i).posZ << ","
			<< m_sceneGraph.at(i).rotX << ","
			<< m_sceneGraph.at(i).rotY << ","
			<< m_sceneGraph.at(i).rotZ << ","
			<< m_sceneGraph.at(i).scaX << ","
			<< m_sceneGraph.at(i).scaY << ","
			<< m_sceneGraph.at(i).scaZ << ","
			<< m_sceneGraph.at(i).render << ","
			<< m_sceneGraph.at(i).collision << ","
			<< "'" << m_sceneGraph.at(i).collision_mesh << "'" << ","
			<< m_sceneGraph.at(i).collectable << ","
			<< m_sceneGraph.at(i).destructable << ","
			<< m_sceneGraph.at(i).health_amount << ","
			<< m_sceneGraph.at(i).editor_render << ","
			<< m_sceneGraph.at(i).editor_texture_vis << ","
			<< m_sceneGraph.at(i).editor_normals_vis << ","
			<< m_sceneGraph.at(i).editor_collision_vis << ","
			<< m_sceneGraph.at(i).editor_pivot_vis << ","
			<< m_sceneGraph.at(i).pivotX << ","
			<< m_sceneGraph.at(i).pivotY << ","
			<< m_sceneGraph.at(i).pivotZ << ","
			<< m_sceneGraph.at(i).snapToGround << ","
			<< m_sceneGraph.at(i).AINode << ","
			<< "'" << m_sceneGraph.at(i).audio_path << "'" << ","
			<< m_sceneGraph.at(i).volume << ","
			<< m_sceneGraph.at(i).pitch << ","
			<< m_sceneGraph.at(i).pan << ","
			<< m_sceneGraph.at(i).one_shot << ","
			<< m_sceneGraph.at(i).play_on_init << ","
			<< m_sceneGraph.at(i).play_in_editor << ","
			<< m_sceneGraph.at(i).min_dist << ","
			<< m_sceneGraph.at(i).max_dist << ","
			<< m_sceneGraph.at(i).camera << ","
			<< m_sceneGraph.at(i).path_node << ","
			<< m_sceneGraph.at(i).path_node_start << ","
			<< m_sceneGraph.at(i).path_node_end << ","
			<< m_sceneGraph.at(i).parent_id << ","
			<< m_sceneGraph.at(i).editor_wireframe << ","
			<< "'" << m_sceneGraph.at(i).name << "'" << ","

			<< m_sceneGraph.at(i).light_type << ","
			<< m_sceneGraph.at(i).light_diffuse_r << ","
			<< m_sceneGraph.at(i).light_diffuse_g << ","
			<< m_sceneGraph.at(i).light_diffuse_b << ","
			<< m_sceneGraph.at(i).light_specular_r << ","
			<< m_sceneGraph.at(i).light_specular_g << ","
			<< m_sceneGraph.at(i).light_specular_b << ","
			<< m_sceneGraph.at(i).light_spot_cutoff << ","
			<< m_sceneGraph.at(i).light_constant << ","
			<< m_sceneGraph.at(i).light_linear << ","
			<< m_sceneGraph.at(i).light_quadratic

			<< ")";
		std::string sqlCommand2 = command.str();
		rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand2.c_str(), -1, &pResults, 0);
		sqlite3_step(pResults);	
	}

	if (rc) { MessageBox(NULL, L"Objects Not Saved", L"Notification", MB_OK); }
	else { MessageBox(NULL, L"Objects Saved", L"Notification", MB_OK); }	
}

void ToolMain::onActionSaveTerrain()
{
	m_d3dRenderer.SaveDisplayChunk();
}

void ToolMain::onActionDeleteObjects()
{
	m_d3dRenderer.DeleteSelectedObjects();

	// Re-setup scene graph
	m_sceneGraph.clear();
	SQLManager::SendQuery("SELECT * FROM Objects", true);
	while (SQLManager::GetObjectStep() == SQLITE_ROW) { m_sceneGraph.push_back(SQLManager::CreateObject()); }
	m_d3dRenderer.BuildDisplayList(&m_sceneGraph);
}

void ToolMain::Tick(MSG *msg)
{
	//do we have a selection						//done
	//do we have a mode								//done
	//are we clicking / dragging /releasing
	//has something changed
		//update Scenegraph
		//add to scenegraph
		//resend scenegraph to Direct X renderer

	// If mouse right has been pressed & mouse position is different from stored
	if (m_toolInputCommands.mouseRight &&
		m_toolInputCommands.mousePosPrevious != m_toolInputCommands.mousePos)
	{
		// Mouse has been dragged
		m_toolInputCommands.mouseDrag = true;
	}
		
	// If left mouse right button is pressed
	if (m_toolInputCommands.mouseRight)
	{		
		// Switch between modes
		switch(m_editor)
		{
		case EDITOR::OBJECT_SPAWN:
		{
			// Create object at picking point
			///SQLManager::AddObject(m_d3dRenderer.CreateObject(m_objectSpawn, MouseManager::PickSpawn()));
			ObjectManager::Spawn(m_objectSpawn, MouseManager::PickSpawn(), m_sceneGraph);

			// Update scene graph
			///m_d3dRenderer.BuildDisplayList(&m_d3dRenderer.GetSceneGraph());
			m_d3dRenderer.BuildDisplayList(&m_sceneGraph);
		}
		break;
		case EDITOR::OBJECT_FUNCTION:
		{
			// If allowed to pick
			if (m_toolInputCommands.pickOnce)
			{
				// If selecting objects
				if (m_d3dRenderer.GetObjectFunction() == OBJECT_FUNCTION::SELECT)
				{
					// Select an object
					m_d3dRenderer.PickingObjects(true);
					m_selectedObjects = m_d3dRenderer.GetSelectedObjectIDs();
				}

				// Else, if any other object function is active
				else
				{
					m_d3dRenderer.StoreObjectDetails(true);
				}

				// Reset picking controller
				m_toolInputCommands.pickOnce = false;
			}			
		}
		break;
		case EDITOR::LANDSCAPE_PAINT:
		{
			// Select terrain
			m_selectedTerrain = m_d3dRenderer.PickingTerrain();
		}
		break;
		case EDITOR::LANDSCAPE_FUNCTION:
		{
			// Select terrain
			m_selectedTerrain = m_d3dRenderer.PickingTerrain();
		}
		break;
		}		
	}

	// Else, if mouse left button is pressed
	else if (m_toolInputCommands.mouseLeft)
	{		
		// Switch between modes
		switch (m_d3dRenderer.GetEditor())
		{
		case EDITOR::OBJECT_FUNCTION:
		{
			// If allowed to pick
			if (m_toolInputCommands.pickOnce)
			{
				// If an object has been intersected
				///if (m_d3dRenderer.PickingObjects(false))
				{
					// Deselect an object
					m_d3dRenderer.PickingObjects(false);
					m_selectedObjects = m_d3dRenderer.GetSelectedObjectIDs();
				}

				// Reset picking controller
				m_toolInputCommands.pickOnce = false;
			}
		}
		break;
		}
	}

	//Renderer Update Call
	m_d3dRenderer.Tick(&m_toolInputCommands);

	// Update local scene graph
	m_sceneGraph = m_d3dRenderer.GetSceneGraph();
}

void ToolMain::UpdateInput(MSG * msg)
{
	switch (msg->message)
	{
		//Global inputs,  mouse position and keys etc
	case WM_KEYDOWN:
		m_keyArray[msg->wParam] = true;
		break;

	case WM_KEYUP:
		m_keyArray[msg->wParam] = false;
		break;

	case WM_MOUSEMOVE:
		// Update mouse x,y to send to the Renderer
		m_toolInputCommands.mousePos.x = GET_X_LPARAM(msg->lParam);
		m_toolInputCommands.mousePos.y = GET_Y_LPARAM(msg->lParam);
		break;

	case WM_LBUTTONDOWN:	
		if (!m_lDown)
		{
			m_lDown = true;
			m_toolInputCommands.mouseLeft = true;
			m_toolInputCommands.pickOnce = true;
			if (m_toolInputCommands.toggle) { m_toolInputCommands.storeOnce = true; m_toolInputCommands.toggle = false; }

			// Store current mouse position
			m_toolInputCommands.mousePosPrevious = m_toolInputCommands.mousePos;
		}

		// Check if mouse has moved
		///if (m_mousePosition != m_toolInputCommands.mousePos) { m_toolInputCommands.mouseDrag = true; }
		break;

	case WM_LBUTTONUP:
		m_lDown = m_toolInputCommands.mouseLeft = m_toolInputCommands.mouseDrag = false;
		m_toolInputCommands.toggle = true;
		m_d3dRenderer.StoreTerrainPosition(true);
		break;

	case WM_RBUTTONDOWN:
		if (!m_rDown)
		{
			m_rDown = true;
			m_toolInputCommands.mouseRight = true;
			m_toolInputCommands.pickOnce = true;

			// Store current mouse position
			m_toolInputCommands.mousePosPrevious = m_toolInputCommands.mousePos;
		}

		// Check if mouse has moved
		///if (m_mousePosition != m_toolInputCommands.mousePos) { m_toolInputCommands.mouseDrag = true; }
		break;

	case WM_RBUTTONUP:
		m_rDown = m_toolInputCommands.mouseRight = m_toolInputCommands.mouseDrag = false;
		break;

	case WM_MBUTTONDOWN:
		m_toolInputCommands.mouseWheel = true;
		break;
		
	case WM_MBUTTONUP:
		m_toolInputCommands.mouseWheel = false;
		break;
	}

	//here we update all the actual app functionality that we want.  This information will either be used int toolmain, or sent down to the renderer (Camera movement etc
	
	// W key to move forward
	if (m_keyArray['W']) { m_toolInputCommands.W = true; }
	else { m_toolInputCommands.W = false; }
	
	// S key to move backward
	if (m_keyArray['S']) { m_toolInputCommands.S = true; }
	else { m_toolInputCommands.S = false; }

	// A key to strafe left
	if (m_keyArray['A']) { m_toolInputCommands.A = true; }
	else { m_toolInputCommands.A = false; }

	// D key to strafe right
	if (m_keyArray['D']) { m_toolInputCommands.D = true; }
	else { m_toolInputCommands.D = false; }

	// E key to move upward
	if (m_keyArray['E']) { m_toolInputCommands.E = true; }
	else { m_toolInputCommands.E = false; }

	// Q key to move downward
	if (m_keyArray['Q']) { m_toolInputCommands.Q = true; }
	else { m_toolInputCommands.Q = false; }
}
