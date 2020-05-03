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
	m_toolInputCommands.E			= false;
	m_toolInputCommands.Q			= false;
	m_toolInputCommands.mouseLeft	= false;
	m_toolInputCommands.mouseRight	= false;
	m_toolInputCommands.mouseWheel	= false;
	m_toolInputCommands.mouseDrag	= false;
	m_toolInputCommands.escape		= false;
	m_toolInputCommands.SHIFT		= false;
	m_toolInputCommands.CTRL		= false;
	m_toolInputCommands.X			= false;
	m_toolInputCommands.C			= false;
	m_toolInputCommands.V			= false;
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

	// Set static mouse manager game & input commands
	MouseManager::SetGame(&m_d3dRenderer);
	MouseManager::SetInput(&m_toolInputCommands);

	// Set static object manager game & input commands
	ObjectManager::SetGame(&m_d3dRenderer);
	ObjectManager::SetInput(&m_toolInputCommands);

	// Set static landscape manager display chunk
	TerrainManager::SetDisplayChunk(m_d3dRenderer.GetDisplayChunk());
	TerrainManager::StorePosition(true);

	// Estable database connection
	if (SQLManager::Connect()) { TRACE("Database connection: fail"); }
	else { TRACE("Database connection: success"); }

	// Load data from database
	onActionLoad();

	// Set static scene manager game & display chunk
	SceneManager::SetGame(&m_d3dRenderer);
	SceneManager::SetDisplayChunk(m_d3dRenderer.GetDisplayChunk(), true);
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
	
	// Process object results into renderable
	m_d3dRenderer.BuildDisplayList(&m_sceneGraph);

	// Chunk /////////////////////////////////////////////////////

	// Database query to retrieve all records from chunks table
	SQLManager::SendQuery("SELECT * FROM Chunks", false);

	// Create & store chunk from table data
	m_chunk = SQLManager::CreateChunk();	

	// Build the renderable chunk
	m_d3dRenderer.BuildDisplayChunk(&m_chunk);

	// Save current state
	SceneManager::SetSceneGraph(&m_sceneGraph);
}

void ToolMain::onActionSave()
{	
	SceneManager::Save();
	
	// Update scene graph
	///m_sceneGraph = m_d3dRenderer.GetSceneGraph();
	//m_d3dRenderer.SaveDisplayList();
	//
	//// Save terrain
	//m_d3dRenderer.SaveDisplayChunk();
	//
	//// Database query to delete all records from objects table
	//SQLManager::SendQuery("DELETE FROM Objects", true);
	//SQLManager::SetObjectStep();

	//// Save new scene graph
	//if (SQLManager::SaveObjects(m_d3dRenderer.GetSceneGraph())) { MessageBox(NULL, L"World Saved", L"Notification", MB_OK); }
	//else { MessageBox(NULL, L"World Failed to Save", L"Notification", MB_OK); }
}

void ToolMain::onActionSaveTerrain()
{
	m_d3dRenderer.SaveDisplayChunk();
}

void ToolMain::onActionDeleteObjects()
{	
	// Delete all selected objects
	ObjectManager::Remove(m_selectedObjectIDs, m_sceneGraph);	
}

void ToolMain::onActionUndo()
{
	// Scene objects
	std::pair<std::vector<SceneObject>, bool> object = SceneManager::UndoObjects();
	if (object.second)
	{
		m_sceneGraph = object.first;
		m_d3dRenderer.BuildDisplayList(&m_sceneGraph);
	}	

	// Terrain geometry	
	bool terrain = SceneManager::UndoGeometry();
	
	// If both were unsuccessful
	if (!object.second && !terrain) { MessageBox(NULL, L"There's nothing to undo!", L"Hey there!", MB_OK); }
	else
	{
		// Reduce count if possible
		SceneManager::ReduceCount();
	}
}

void ToolMain::onActionRedo()
{
	// Scene objects	
	std::pair<std::vector<SceneObject>, bool> object = SceneManager::RedoObjects();
	if (object.second)
	{
		m_sceneGraph = object.first;
		m_d3dRenderer.BuildDisplayList(&m_sceneGraph);
	}	

	// Terrain geometry	
	bool terrain = SceneManager::RedoGeometry();
	
	// If both were unsuccessful
	if (!object.second && !terrain)	{ MessageBox(NULL, L"There's nothing to redo!", L"Hey there!", MB_OK); }
	else 
	{
		// Increase count if possible
		SceneManager::IncreaseCount();
	}
}

void ToolMain::Tick(MSG *msg)
{
	//do we have a selection						//done
	//do we have a mode								//done
	//are we clicking / dragging /releasing			//done
	//has something changed							//*** still do write a check for this ***//
		//update Scenegraph							//done
		//add to scenegraph							//done
		//resend scenegraph to Direct X renderer	//done

	// Get updated scene graph
	m_sceneGraph = m_d3dRenderer.GetSceneGraph();
		
	// If mouse right has been pressed & mouse position is different from stored
	if (m_toolInputCommands.mouseRight &&
		m_toolInputCommands.mousePosPrevious != m_toolInputCommands.mousePos)
	{
		// Mouse has been dragged
		m_toolInputCommands.mouseDrag = true;
	}
		
	// If CTRL is pressed
	if (m_toolInputCommands.CTRL)
	{
		// If X is pressed (cut)
		if (m_toolInputCommands.X) { ObjectManager::Cut(m_selectedObjectIDs, m_sceneGraph); }
			
		// If C is pressed (copy)
		else if (m_toolInputCommands.C) { ObjectManager::Copy(m_selectedObjectIDs, m_sceneGraph); }

		// If V is pressed (paste)
		else if (m_toolInputCommands.V) { ObjectManager::Paste(m_sceneGraph); }
	}	
		
	// If right mouse button is pressed
	if (m_toolInputCommands.mouseRight)
	{				
		// Switch between modes
		switch(m_editor)
		{
		case EDITOR::OBJECT_SPAWN:
		{
			// If control key is pressed
			if (m_toolInputCommands.CTRL)
			{				
				// Remove an object
				ObjectManager::Remove(m_selectedObjectIDs, m_sceneGraph, MouseManager::PickObject(PICK_TYPE::ANY));				
			}

			// Else, if not
			else
			{
				// Define light or not
				LIGHT_TYPE lightType;
				if (m_objectType == OBJECT_TYPE::LIGHT) { lightType = LIGHT_TYPE::POINT; }
				else { lightType = LIGHT_TYPE::NA; }

				// Create object at picking point
				ObjectManager::SpawnObject(m_objectType, MouseManager::PickSpawn(), m_sceneGraph, (int)lightType);			}						
		}
		break;
		case EDITOR::OBJECT_FUNCTION:
		{			
			// If object function is select
			if (m_objectFunction == OBJECT_FUNCTION::SELECT)
			{
				// If allowed to pick
				if (m_toolInputCommands.pickOnce)
				{
					// If shift key is pressed
					if (m_toolInputCommands.SHIFT)
					{
						// Select multiple objects
						MouseManager::PickMultipleObjects(m_selectedObjectIDs, PICK_TYPE::OBJECT, true);
					}

					// Else, if control key is pressed
					else if (m_toolInputCommands.CTRL)
					{
						// Deselect multiple objects
						MouseManager::PickMultipleObjects(m_selectedObjectIDs, PICK_TYPE::OBJECT, false);
					}

					// Else, if not
					else
					{
						// Select a single object
						m_selectedObjectIDs.clear();
						m_selectedObjectIDs.push_back(MouseManager::PickObject(PICK_TYPE::OBJECT));
					}	

					// There's been a new selection
					m_newSelection = true;

					// Reset picking controller
					m_toolInputCommands.pickOnce = false;
				}
			}
			
			// Else, if any other function is active
			else
			{
				// If objects are selected and mouse has been dragged
				if (m_selectedObjectIDs.size() != 0 && m_toolInputCommands.mouseDrag)
				{
					// Transform selected objects
					ObjectManager::Transform(m_objectFunction, m_objectConstraint, m_selectedObjectIDs, m_sceneGraph);
				}
			}			
		}			
		break;
		case EDITOR::TERRAIN_PAINT:
		{
			// Paint terrain
			TerrainManager::Paint(m_selectedTerrain, m_paint);
		}
		break;
		case EDITOR::SCULPT_FREELY:
		{
			// Sculpt terrain
			TerrainManager::Sculpt(m_selectedTerrain, m_terrainFunction, m_terrainConstraint);

			m_sculpting = true;
		}
		break;
		case EDITOR::SCULPT_SINGLE:
		{
			// Sculpt selected terrain
			TerrainManager::Sculpt(m_selectedTerrain, m_terrainFunction, m_terrainConstraint, true);

			m_sculpting = true;
		}
		break;
		case EDITOR::LIGHTS:
		{
			// If translating
			if (m_objectFunction == OBJECT_FUNCTION::TRANSLATE)
			{
				// If mouse has been dragged
				if (m_toolInputCommands.mouseDrag)
				{					
					// Transform lights
					ObjectManager::Transform(OBJECT_FUNCTION::TRANSLATE, m_objectConstraint, m_selectedObjectIDs, m_sceneGraph);
				}
			}

			// Else, must be selecting
			else
			{
				// If allowed to pick
				if (m_toolInputCommands.pickOnce)
				{
					// If shift key is pressed
					if (m_toolInputCommands.SHIFT)
					{
						// Select multiple lights
						MouseManager::PickMultipleObjects(m_selectedObjectIDs, PICK_TYPE::LIGHT, true);
					}

					// Else, if control key is pressed
					else if (m_toolInputCommands.CTRL)
					{
						// Deselect multiple lights
						MouseManager::PickMultipleObjects(m_selectedObjectIDs, PICK_TYPE::LIGHT, false);
					}

					// Else, if not
					else
					{
						// Select a single light
						m_selectedObjectIDs.clear();
						m_selectedObjectIDs.push_back(MouseManager::PickObject(PICK_TYPE::LIGHT));
					}

					// There's been a new selection
					m_newSelection = true;

					// Reset picking controller
					m_toolInputCommands.pickOnce = false;
				}
			}
		}
		break;
		}		
	}

	// Set renderer selected objects to draw bounding boxes
	m_d3dRenderer.SetSelectedObjectIDs(m_selectedObjectIDs);

	//Renderer Update Call
	m_d3dRenderer.Tick(&m_toolInputCommands);
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
		break;

	case WM_LBUTTONUP:
		m_lDown = m_toolInputCommands.mouseLeft = m_toolInputCommands.mouseDrag = false;
		m_toolInputCommands.toggle = true;
		break;

	case WM_RBUTTONDOWN:
		if (!m_rDown)
		{
			m_rDown = true;
			m_toolInputCommands.mouseRight = true;
			m_toolInputCommands.pickOnce = true;

			// Store all object details
			ObjectManager::Store(m_selectedObjectIDs);

			// Store current mouse position
			m_toolInputCommands.mousePosPrevious = m_toolInputCommands.mousePos;
		}
		break;

	case WM_RBUTTONUP:
		m_rDown = m_toolInputCommands.mouseRight = m_toolInputCommands.mouseDrag = false;
		TerrainManager::StorePosition(true);		
		///SaveDisplayList(m_d3dRenderer.GetDisplayList());
		m_d3dRenderer.BuildDisplayList(&m_sceneGraph);		
		///SceneManager::QuickSave();
		if (!m_sculpting)
		{
			SceneManager::SetSceneGraph(&m_sceneGraph);
		}
		else if (m_sculpting) {
			m_sculpting = false;
			SceneManager::SetDisplayChunk(m_d3dRenderer.GetDisplayChunk());
		}
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

	// SHIFT key to select more than one object
	if (m_keyArray[(int)VK_SHIFT]) { m_toolInputCommands.SHIFT = true; }
	else { m_toolInputCommands.SHIFT = false; } 	

	// CTRL key to delete objects rather than spawning via mouseRight
	// CTRL key to cut, copy or paste an object via X,C,V
	if (m_keyArray[(int)VK_CONTROL]) { m_toolInputCommands.CTRL = true; }
	else { m_toolInputCommands.CTRL = false; }

	// X key to cut an object
	if (m_keyArray['X']) { m_toolInputCommands.X = true; }
	else { m_toolInputCommands.X = false; }

	// C key to copy an object
	if (m_keyArray['C']) { m_toolInputCommands.C = true; }
	else { m_toolInputCommands.C = false; }

	// V key to paste an object
	if (m_keyArray['V']) { m_toolInputCommands.V = true; }
	else { m_toolInputCommands.V = false; }
}

bool ToolMain::GetNewSelection()
{
	if (m_newSelection) 
	{
		m_newSelection = false;
		return true;
	}
	else { return false; }
}

void ToolMain::SaveDisplayList(std::vector<DisplayObject> displayList)
{
	// Loop through display list
	for (int i = 0; i < displayList.size(); ++i)
	{
		// Update scene graph positions
		m_sceneGraph[i].posX = displayList[i].m_position.x;
		m_sceneGraph[i].posY = displayList[i].m_position.y;
		m_sceneGraph[i].posZ = displayList[i].m_position.z;

		// Update scene graph rotations
		m_sceneGraph[i].rotX = displayList[i].m_orientation.x;
		m_sceneGraph[i].rotY = displayList[i].m_orientation.y;
		m_sceneGraph[i].rotZ = displayList[i].m_orientation.z;

		// Update scene graph scales
		m_sceneGraph[i].scaX = displayList[i].m_scale.x;
		m_sceneGraph[i].scaY = displayList[i].m_scale.y;
		m_sceneGraph[i].scaZ = displayList[i].m_scale.z;
	}

	// Build new display list
	m_d3dRenderer.BuildDisplayList(&m_sceneGraph);
}