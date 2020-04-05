//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include "DisplayObject.h"
#include <string>


using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Game::Game()
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);
	m_displayList.clear();
	
	//initial Settings
	//modes
	m_grid = false;
	m_storeTerrainPosition = true;
	m_storeObjectDetails = true;
}

Game::~Game()
{

#ifdef DXTK_AUDIO
    if (m_audEngine)
    {
        m_audEngine->Suspend();
    }
#endif
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
	m_window = window;

	m_camera = std::make_unique<Camera>();
	
	m_gamePad = std::make_unique<GamePad>();

    m_keyboard = std::make_unique<Keyboard>();

    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(m_window);

    m_deviceResources->SetWindow(m_window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

	GetClientRect(m_window, &m_screenDimensions);

#ifdef DXTK_AUDIO
    // Create DirectXTK for Audio objects
    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
    eflags = eflags | AudioEngine_Debug;
#endif

    m_audEngine = std::make_unique<AudioEngine>(eflags);

    m_audioEvent = 0;
    m_audioTimerAcc = 10.f;
    m_retryDefault = false;

    m_waveBank = std::make_unique<WaveBank>(m_audEngine.get(), L"adpcmdroid.xwb");

    m_soundEffect = std::make_unique<SoundEffect>(m_audEngine.get(), L"MusicMono_adpcm.wav");
    m_effect1 = m_soundEffect->CreateInstance();
    m_effect2 = m_waveBank->CreateInstance(10);

    m_effect1->Play(true);
    m_effect2->Play();
#endif
}

void Game::SetGridState(bool state)
{
	m_grid = state;
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick(InputCommands *Input)
{
	//copy over the input commands so we have a local version to use elsewhere.
	m_inputCommands = *Input;
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

#ifdef DXTK_AUDIO
    // Only update audio engine once per frame
    if (!m_audEngine->IsCriticalError() && m_audEngine->Update())
    {
        // Setup a retry in 1 second
        m_audioTimerAcc = 1.f;
        m_retryDefault = true;
    }
#endif

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
	// Frame time
	m_deltaTime = timer.GetElapsedSeconds();

	// Light
	/*auto time = static_cast<float>(timer.GetTotalSeconds());
	float yaw = time * 0.4f;
	float pitch = time * 0.7f;
	float roll = time * 1.1f;
	auto quat = Quaternion::CreateFromYawPitchRoll(pitch, yaw, roll);
	auto light = XMVector3Rotate(DirectX::g_XMOne, quat);
	m_displayChunk.m_terrainEffect->SetLightDirection(0, light);*/
	
	// Loop through all objects
	//for (int i = 0; i < m_displayList.size(); ++i)
	//{
	//	// Offset position based on sine wave
	//	float yPos = sin(m_displayList[i].m_position.x + (m_deltaTime * 15));

	//	// Modify normals
	//	float xNorm = 1 - cos(m_displayList[i].m_position.x + m_deltaTime);
	//	float yNorm = abs(cos(m_displayList[i].m_position.x + m_deltaTime));
	//	
	//	// Calculate position of vertex against world, view and projection matrices
	//	m_displayList[i].m_position.y = yPos;
	//}
		
	// Handle all input
	HandleInput();
	
	// Custom camera
	UpdateCamera();

	// Water waves
	///m_displayChunk.Wave(m_deltaTime, m_world, m_view, m_projection);
	UpdateWaves();

#ifdef DXTK_AUDIO
    m_audioTimerAcc -= (float)timer.GetElapsedSeconds();
    if (m_audioTimerAcc < 0)
    {
        if (m_retryDefault)
        {
            m_retryDefault = false;
            if (m_audEngine->Reset())
            {
                // Restart looping audio
                m_effect1->Play(true);
            }
        }
        else
        {
            m_audioTimerAcc = 4.f;

            m_waveBank->Play(m_audioEvent++);

            if (m_audioEvent >= 11)
                m_audioEvent = 0;
        }
    }
#endif

}

void Game::HandleInput()
{
	// Switch between modes
	switch (m_editor)
	{
	case EDITOR::OBJECT_SPAWN:
	{
		// If mouse left is being pressed
		if (m_inputCommands.mouseLeft)
		{
			// Update picking point
			MousePicking();

			// Switch between objects to spawn
			switch (m_objectSpawn)
			{
				// Spawn grass at picking point
			case OBJECT_SPAWN::GRASS: SQL::AddObject(CreateObject(OBJECT_SPAWN::GRASS, m_pickingPoint)); break;
				// Spawn a tree at picking point
			case OBJECT_SPAWN::TREE: SQL::AddObject(CreateObject(OBJECT_SPAWN::TREE, m_pickingPoint)); break;
				// Spawn water at picking point
			case OBJECT_SPAWN::WATER: SQL::AddObject(CreateObject(OBJECT_SPAWN::WATER, m_pickingPoint)); break;
			}

			// Update scene graph
			BuildDisplayList(&m_sceneGraph);
		}
	}
	break;
	case EDITOR::OBJECT_TRANSFORM:
	{
		// If mouse left is being pressed
		if (m_inputCommands.mouseLeft)
		{
			bool save = false;

			// If any objects are selected
			if (m_selectedObjectIDs.size() != 0)
			{
				// If object positions should be stored
				if (m_storeObjectDetails)
				{
					// Reset vectors
					m_storedObjectScales.clear();
					m_storedObjectTranslations.clear();
					m_storedObjectRotations.clear();

					// Loop through selected objects
					for (int i = 0; i < m_selectedObjectIDs.size(); ++i)
					{
						// If selected ID isn't -1
						if (m_selectedObjectIDs[i] != -1)
						{
							// Fill vectors with selected object data
							m_storedObjectScales.push_back(m_displayList[m_selectedObjectIDs[i]].m_scale);
							m_storedObjectTranslations.push_back(m_displayList[m_selectedObjectIDs[i]].m_position);
							m_storedObjectRotations.push_back(m_displayList[m_selectedObjectIDs[i]].m_orientation);
						}
					}

					// Reset controller
					m_storeObjectDetails = false;
				}

				// Loop through selected objects
				for (int i = 0; i < m_selectedObjectIDs.size(); ++i)
				{
					// Update picking point
					MousePicking(i);

					// If picking point should be stored
					if (m_inputCommands.storeOnce)
					{
						// Store picking point position
						m_storedPickingPoint = m_pickingPoint;
						///m_storedPickingPoint = m_displayList[m_selectedObjectIDs[i]].m_position;

						// Reset controller
						m_inputCommands.storeOnce = false;
					}

					// If current object is intersecting with the ray trace
					///if (ObjectIntersection(i))
					// If picking point has moved
					///if (m_pickingPoint != m_storedPickingPoint)
					{
						// Switch between S, R, T
						switch (m_objectTransform)
						{
						case OBJECT_FUNCTION::SCALE:
						{
							// If should be scaled on the X and Y axis
							if (m_objectConstraint == OBJECT_CONSTRAINT::XY)
							{
								// Scale selected object based on picking point								
								m_sceneGraph[m_selectedObjectIDs[i]].scaX = m_storedObjectScales[i].x - m_pickingPoint.x;
								m_sceneGraph[m_selectedObjectIDs[i]].scaY = m_storedObjectScales[i].y - m_pickingPoint.y;
								save = true;
							}

							// Else, if should be scaled on the X and Z axis
							else if (m_objectConstraint == OBJECT_CONSTRAINT::XZ)
							{
								// Scale selected object based on picking point
								m_sceneGraph[m_selectedObjectIDs[i]].scaX = m_storedObjectScales[i].x - m_pickingPoint.x;
								m_sceneGraph[m_selectedObjectIDs[i]].scaZ = m_storedObjectScales[i].z - m_pickingPoint.z;
								save = true;
							}

							// Else, if should be scaled on the Y and Z axis
							else if (m_objectConstraint == OBJECT_CONSTRAINT::YZ)
							{
								// Scale selected object based on picking point
								m_sceneGraph[m_selectedObjectIDs[i]].scaY = m_storedObjectScales[i].y - m_pickingPoint.y;
								m_sceneGraph[m_selectedObjectIDs[i]].scaZ = m_storedObjectScales[i].z - m_pickingPoint.z;
								save = true;
							}

							// Else, if should be scaled on the X axis
							else if (m_objectConstraint == OBJECT_CONSTRAINT::X)
							{
								// Scale selected object based on picking point
								m_sceneGraph[m_selectedObjectIDs[i]].scaX = m_storedObjectScales[i].x - m_pickingPoint.x;
								save = true;
							}

							// Else, if should be scaled on the Y axis
							else if (m_objectConstraint == OBJECT_CONSTRAINT::Y)
							{
								// Scale selected object based on picking point
								m_sceneGraph[m_selectedObjectIDs[i]].scaY = m_storedObjectScales[i].y - m_pickingPoint.y;
								save = true;
							}

							// Else, if should be scaled on the Z axis
							else if (m_objectConstraint == OBJECT_CONSTRAINT::Z)
							{
								// Scale selected object based on picking point
								m_sceneGraph[m_selectedObjectIDs[i]].scaZ = m_storedObjectScales[i].z - m_pickingPoint.z;
								save = true;
							}

							// Else, if should be scaled freely
							else if (m_objectConstraint == OBJECT_CONSTRAINT::ALL)
							{
								// Scale selected object based on picking point
								m_sceneGraph[m_selectedObjectIDs[i]].scaX = m_storedObjectScales[i].x - m_pickingPoint.x;
								m_sceneGraph[m_selectedObjectIDs[i]].scaY = m_storedObjectScales[i].y - m_pickingPoint.y;
								m_sceneGraph[m_selectedObjectIDs[i]].scaZ = m_storedObjectScales[i].z - m_pickingPoint.z;
								save = true;
							}

							// Store manipulated scale
							///m_storedObjectScales[i] = m_displayList[m_selectedObjectIDs[i]].m_scale;						
						}
						break;
						case OBJECT_FUNCTION::TRANSLATE:
						{
							// If should be translated on the X and Y axis
							if (m_objectConstraint == OBJECT_CONSTRAINT::XY)
							{
								// Translate selected object based on picking point
								///m_displayList[m_selectedObjectIDs[i]].m_position.x = m_pickingPoint.x;								
								///m_displayList[m_selectedObjectIDs[i]].m_position.y = m_pickingPoint.y;

								m_sceneGraph[m_selectedObjectIDs[i]].posX = m_pickingPoint.x;
								m_sceneGraph[m_selectedObjectIDs[i]].posY = m_pickingPoint.y;
								save = true;
							}

							// Else, if should be translated on the X and Z axis
							else if (m_objectConstraint == OBJECT_CONSTRAINT::XZ)
							{
								// Translate selected object based on picking point
								///m_displayList[m_selectedObjectIDs[i]].m_position.x = m_pickingPoint.x;
								///m_displayList[m_selectedObjectIDs[i]].m_position.z = m_pickingPoint.z;

								m_sceneGraph[m_selectedObjectIDs[i]].posX = m_pickingPoint.x;
								m_sceneGraph[m_selectedObjectIDs[i]].posZ = m_pickingPoint.z;

								save = true;
							}

							// Else, if should be translated on the Y and Z axis
							else if (m_objectConstraint == OBJECT_CONSTRAINT::YZ)
							{
								// Translate selected object based on picking point								
								///m_displayList[m_selectedObjectIDs[i]].m_position.y = m_pickingPoint.y;
								///m_displayList[m_selectedObjectIDs[i]].m_position.z = m_pickingPoint.z;

								m_sceneGraph[m_selectedObjectIDs[i]].posY = m_pickingPoint.y;
								m_sceneGraph[m_selectedObjectIDs[i]].posZ = m_pickingPoint.z;
								save = true;
							}

							// Else, if should be translated on the X axis
							else if (m_objectConstraint == OBJECT_CONSTRAINT::X)
							{
								// Translate selected object based on picking point
								///m_displayList[m_selectedObjectIDs[i]].m_position.x = m_pickingPoint.x;
								m_sceneGraph[m_selectedObjectIDs[i]].posX = m_pickingPoint.x;

								///Vector3 dragPoint = GetDragPoint(&m_xAxes[m_selectedObjectIDs[i]], &m_pickingPoint);
								///m_displayList[m_selectedObjectIDs[i]].m_position.x = m_pickingPoint.x - dragPoint.x;
								save = true;
							}

							// Else, if should be translated on the Y axis
							else if (m_objectConstraint == OBJECT_CONSTRAINT::Y)
							{
								// Translate selected object based on picking point
								///m_displayList[m_selectedObjectIDs[i]].m_position.y = m_pickingPoint.y;
								m_sceneGraph[m_selectedObjectIDs[i]].posY = m_pickingPoint.y;
								save = true;
							}

							// Else, if should be translated on the Z axis
							else if (m_objectConstraint == OBJECT_CONSTRAINT::Z)
							{
								// Translate selected object based on picking point
								///m_displayList[m_selectedObjectIDs[i]].m_position.z = m_pickingPoint.z;
								m_sceneGraph[m_selectedObjectIDs[i]].posZ = m_pickingPoint.z;
								save = true;
							}

							// Else, if should be translated freely
							else if (m_objectConstraint == OBJECT_CONSTRAINT::ALL)
							{
								// Get distance between picking point and stored object position
								/*float x = sqrt((m_pickingPoint.x - m_storedObjectTranslations[i].x) * (m_pickingPoint.x - m_storedObjectTranslations[i].x));
								float y = sqrt((m_pickingPoint.y - m_storedObjectTranslations[i].y) * (m_pickingPoint.y - m_storedObjectTranslations[i].y));
								float z = sqrt((m_pickingPoint.z - m_storedObjectTranslations[i].z) * (m_pickingPoint.z - m_storedObjectTranslations[i].z));
								float distance = sqrt((m_pickingPoint.x - m_storedObjectTranslations[i].x) * (m_pickingPoint.x - m_storedObjectTranslations[i].x) +
									(m_pickingPoint.y - m_storedObjectTranslations[i].y) * (m_pickingPoint.y - m_storedObjectTranslations[i].y) +
									(m_pickingPoint.z - m_storedObjectTranslations[i].z) * (m_pickingPoint.z - m_storedObjectTranslations[i].z));*/

									// Translate selected object based on picking point
									///m_displayList[m_selectedObjectIDs[i]].m_position = m_pickingPoint;
									///m_displayList[m_selectedObjectIDs[i]].m_position = m_storedObjectTranslations[i] + m_pickingPoint;

								m_sceneGraph[m_selectedObjectIDs[i]].posX = m_pickingPoint.x;
								m_sceneGraph[m_selectedObjectIDs[i]].posY = m_pickingPoint.y;
								m_sceneGraph[m_selectedObjectIDs[i]].posZ = m_pickingPoint.z;

								save = true;
							}
						}
						break;
						case OBJECT_FUNCTION::ROTATE:
						{
							// If should be rotated on the X and Y axis
							if (m_objectConstraint == OBJECT_CONSTRAINT::XY)
							{
								// Rotate selected object based on picking point
								m_sceneGraph[m_selectedObjectIDs[i]].rotX += m_pickingPoint.x;
								m_sceneGraph[m_selectedObjectIDs[i]].rotY += m_pickingPoint.y;
							}

							// Else, if should be rotated on the X and Z axis
							else if (m_objectConstraint == OBJECT_CONSTRAINT::XZ)
							{
								// Rotate selected object based on picking point
								m_sceneGraph[m_selectedObjectIDs[i]].rotX += m_pickingPoint.x;
								m_sceneGraph[m_selectedObjectIDs[i]].rotZ += m_pickingPoint.z;
							}

							// Else, if should be rotated on the Y and Z axis
							else if (m_objectConstraint == OBJECT_CONSTRAINT::YZ)
							{
								// Rotate selected object based on picking point
								m_sceneGraph[m_selectedObjectIDs[i]].rotY += m_pickingPoint.y;
								m_sceneGraph[m_selectedObjectIDs[i]].rotZ += m_pickingPoint.z;
							}

							// Else, if should be rotated on the X axis
							else if (m_objectConstraint == OBJECT_CONSTRAINT::X)
							{
								// Rotate selected object based on picking point
								m_sceneGraph[m_selectedObjectIDs[i]].rotX += m_pickingPoint.x;
							}

							// Else, if should be rotated on the Y axis
							else if (m_objectConstraint == OBJECT_CONSTRAINT::Y)
							{
								// Rotate selected object based on picking point
								m_sceneGraph[m_selectedObjectIDs[i]].rotY += m_pickingPoint.y;
							}

							// Else, if should be rotated on the Z axis
							else if (m_objectConstraint == OBJECT_CONSTRAINT::Z)
							{
								// Rotate selected object based on picking point
								m_sceneGraph[m_selectedObjectIDs[i]].rotZ += m_pickingPoint.z;
							}

							// Else, if should be rotated freely
							else if (m_objectConstraint == OBJECT_CONSTRAINT::ALL)
							{
								// Rotate selected object based on picking point
								m_sceneGraph[m_selectedObjectIDs[i]].rotX += m_pickingPoint.x;
								m_sceneGraph[m_selectedObjectIDs[i]].rotY += m_pickingPoint.y;
								m_sceneGraph[m_selectedObjectIDs[i]].rotZ += m_pickingPoint.z;
							}
						}
						break;
						}
					}
				}
			}

			if (save)
			{
				// Update scene graph
				BuildDisplayList(&m_sceneGraph);
			}
		}
		// Else, if mouse isn't being pressed
		{
			// Reset camera look at
			m_camera->SetLookingAtObject(false);
		}
	}
	break;
	case EDITOR::LANDSCAPE_PAINT:
	{
		// If mouse is being pressed
		if (m_inputCommands.mouseLeft)
		{
			// If selected terrain is intersected by ray trace
			if (m_selectedTerrain.intersect)
			{
				// Store selected terrain row,column
				DirectX::SimpleMath::Vector2 location;
				location.x = m_selectedTerrain.row;
				location.y = m_selectedTerrain.column;

				// Paint terrain the selected texture
				m_displayChunk.PaintTerrain(m_selectedTerrain.row, m_selectedTerrain.column, m_landscapePaint, true);
				///m_displayChunk.PaintTerrain(m_selectedTerrain.row, m_selectedTerrain.column, LANDSCAPE_PAINT::GRASS_AND_DIRT);
			}
		}
	}
	break;
	case EDITOR::LANDSCAPE_SCULPT:
	{
		// If mouse is being pressed
		if (m_inputCommands.mouseLeft)
		{
			// If selected terrain is intersected by ray trace
			if (m_selectedTerrain.intersect)
			{
				// If increase or decrease
				if (m_landscapeSculpt == LANDSCAPE_SCULPT::INCREASE ||
					m_landscapeSculpt == LANDSCAPE_SCULPT::DECREASE)
				{
					// Increase/decrease terrain
					m_displayChunk.SculptTerrain(m_selectedTerrain.row, m_selectedTerrain.column, m_landscapeSculpt, m_landscapeConstraint);
				}

				// Else, if flatten
				else if (m_landscapeSculpt == LANDSCAPE_SCULPT::FLATTEN)
				{
					// If first position should be stored
					if (m_storeTerrainPosition)
					{
						m_storeTerrainPosition = false;
						m_storedTerrainPositions.clear();
						m_storedTerrainPositions.push_back(m_displayChunk.GetGeometry(m_selectedTerrain.row, m_selectedTerrain.column).position);
						m_storedTerrainPositions.push_back(m_displayChunk.GetGeometry(m_selectedTerrain.row, m_selectedTerrain.column + 1).position);
						m_storedTerrainPositions.push_back(m_displayChunk.GetGeometry(m_selectedTerrain.row + 1, m_selectedTerrain.column + 1).position);
						m_storedTerrainPositions.push_back(m_displayChunk.GetGeometry(m_selectedTerrain.row + 1, m_selectedTerrain.column).position);
					}

					// Check if selected terrain height doesn't match stored position height
					///if (m_selectedTerrain.position.y != m_storedTerrainPositions.y)
					{
						// Flatten height of selected terrain
						m_displayChunk.SculptTerrain(m_selectedTerrain.row, m_selectedTerrain.column, m_landscapeSculpt, m_landscapeConstraint, m_storedTerrainPositions);
					}
				}
			}
		}
	}
	break;
	}
}

// Updates the camera
void Game::UpdateCamera()
{
	DirectX::SimpleMath::Vector2 centre;
	centre.x = m_deviceResources->GetScreenViewport().Width / 2;
	centre.y = m_deviceResources->GetScreenViewport().Height / 2;

	// Cursor position
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	ScreenToClient(m_window, &cursorPos);

	m_camera->HandleInput(&m_inputCommands, m_deltaTime, centre.x, centre.y, cursorPos);
	m_camera->Update();

	//apply camera vectors
	m_view = Matrix::CreateLookAt(m_camera->GetPosition(), m_camera->GetLookAt(), Vector3::UnitY);

	m_batchEffect->SetView(m_view);
	m_batchEffect->SetWorld(Matrix::Identity);
	m_displayChunk.m_terrainEffect->SetView(m_view);
	m_displayChunk.m_terrainEffect->SetWorld(Matrix::Identity);
	///m_displayChunk.m_dualEffect->SetView(m_view);
	///m_displayChunk.m_dualEffect->SetWorld(Matrix::Identity);
	/*m_displayChunk.m_terrainBlendOne->SetView(m_view);
	m_displayChunk.m_terrainBlendOne->SetWorld(Matrix::Identity);
	m_displayChunk.m_terrainBlendTwo->SetView(m_view);
	m_displayChunk.m_terrainBlendTwo->SetWorld(Matrix::Identity);*/
}

// Updates the waves of all water objects
void Game::UpdateWaves()
{
	// Increase elapsed time
	m_elapsedTime += m_deltaTime;
	
	// Loop through display list
	for (int i = 0; i < m_displayList.size(); ++i)
	{
		// If current object is water
		if (m_displayList[i].m_type == MODEL_TYPE::WATER)
		{
			// Switch between positive/negative
			switch (m_switch)
			{
			case true: m_displayList[i].m_position.y += (m_elapsedTime / 6); break;
			case false: m_displayList[i].m_position.y -= (m_elapsedTime / 6); break;
			}
		}
	}

	// If elapsed time has passed half a second
	if (m_elapsedTime > .5f)
	{
		// Reset timer
		m_elapsedTime = 0.f;
		
		// Switch positive/negative
		if (m_switch) { m_switch = false; }
		else { m_switch = true; }
	}
	
	//// Temp y position
	//float tempY = 0.f;
	//
	//// Loop through display list
	//for (int i = 0; i < m_displayList.size(); ++i)
	//{
	//	// Store y position
	//	tempY = m_displayList[i].m_position.y;

	//	// If current object is water
	//	if (m_displayList[i].m_type == MODEL_TYPE::WATER)
	//	{			
	//		///m_displayList[i].m_position.y = sin(m_displayList[i].m_position.x - (m_deltaTime));/// *100.f));
	//		
	//		///float speed = 3.f, height = 10.f;
	//		///m_displayList[i].m_position.y = sin(speed * m_deltaTime) * height;

	//		///float amplitude = 10.f, freq = 1.f;
	//		///m_displayList[i].m_position.x += m_deltaTime;
	//		///m_displayList[i].m_position.y = (amplitude * (sin(2 * PI * freq * m_deltaTime + 5)));

	//		///m_displayList[i].m_position.y = Pulse();

	//		///m_displayList[i].m_position.y = GetNewHeight(tempY);

	//		///m_displayList[i].m_position.y = sin((((i / 5.f)*40.f) / 360.f)*PI*2.f);

	//		///m_displayList[i].m_position.y = sin(m_displayList[i].m_position.x / 3.f) * 10.f + 300.f;
	//	
	//		// Store temp position
	//		//Vector3 position = m_displayList[i].m_position;

	//		//// Offset position based on sinewave
	//		//position.y = sin(position.x + m_deltaTime) * 10.f;

	//		//// Calculate position of vertex against world, view and projection matrices
	//		//m_displayList[i].m_position = (Vector3)XMVector3Transform(position, m_world);
	//		//m_displayList[i].m_position = (Vector3)XMVector3Transform((Vector3)m_displayList[i].m_position, m_world);
	//		//m_displayList[i].m_position = (Vector3)XMVector3Transform((Vector3)m_displayList[i].m_position, m_world);

	//		m_waterTranslation += 0.001f;
	//		if (m_waterTranslation > .1f) 
	//		{ 
	//			m_waterTranslation -= .2f; 
	//		}
	//		m_displayList[i].m_position.y += m_waterTranslation;
	//		///m_displayList[i].m_position.y += 1.f;
	//	}
	//}
}
float Game::Pulse()
{
	const float freq = 10.f; //Frequency in Hz
	return 0.5 * (1 + sin(2 * PI * freq * m_deltaTime));
}
float Game::GetNewHeight(float OGheight)
{
	float theta = sin(m_deltaTime);
	return OGheight + (theta * 10.f);
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

	if (m_grid)
	{
		// Draw procedurally generated dynamic grid
		const XMVECTORF32 xaxis = { 512.f, 0.f, 0.f };
		const XMVECTORF32 yaxis = { 0.f, 0.f, 512.f };
		DrawGrid(xaxis, yaxis, g_XMZero, 512, 512, Colors::Gray);
	}
	
	//RENDER OBJECTS FROM SCENEGRAPH
	int numRenderObjects = m_displayList.size();
	for (int i = 0; i < numRenderObjects; i++)
	{
		m_deviceResources->PIXBeginEvent(L"Draw model");
		const XMVECTORF32 scale = { m_displayList[i].m_scale.x, m_displayList[i].m_scale.y, m_displayList[i].m_scale.z };
		const XMVECTORF32 translate = { m_displayList[i].m_position.x, m_displayList[i].m_position.y, m_displayList[i].m_position.z };

		//convert degrees into radians for rotation matrix
		XMVECTOR rotate = Quaternion::CreateFromYawPitchRoll(m_displayList[i].m_orientation.y *3.1415 / 180,
															m_displayList[i].m_orientation.x *3.1415 / 180,
															m_displayList[i].m_orientation.z *3.1415 / 180);

		XMMATRIX local = m_world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, scale, g_XMZero, rotate, translate);

		m_displayList[i].m_model->Draw(context, *m_states, local, m_view, m_projection, false);	//last variable in draw,  make TRUE for wireframe

		m_deviceResources->PIXEndEvent();
	}
	m_deviceResources->PIXEndEvent();

	//RENDER TERRAIN
	context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(m_states->DepthDefault(),0);
	context->RSSetState(m_states->CullNone());
	if (m_wireframe) { context->RSSetState(m_states->Wireframe()); }

	//Render the batch,  This is handled in the Display chunk becuase it has the potential to get complex
	m_displayChunk.RenderBatch(m_deviceResources);

	//HUD
	m_sprites->Begin();

	WCHAR   Buffer[256];

	// Camera
	/*std::wstring pos = L"Pos X: " + std::to_wstring(m_camera->GetPosition().x)
		+ L" Pos Y: " + std::to_wstring(m_camera->GetPosition().y)
		+ L" Pos Z: " + std::to_wstring(m_camera->GetPosition().z);
	std::wstring rot = L"Rot X: " + std::to_wstring(m_camera->GetLookAt().x)
		+ L" Rot Y: " + std::to_wstring(m_camera->GetLookAt().y)
		+ L" Rot Z: " + std::to_wstring(m_camera->GetLookAt().z);
	m_font->DrawString(m_sprites.get(), pos.c_str(), XMFLOAT2(100, 10), Colors::Yellow);
	m_font->DrawString(m_sprites.get(), rot.c_str(), XMFLOAT2(100, 40), Colors::Yellow);*/

	// Mouse
	/*POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(m_window, &mousePos);
	std::wstring mouse = L"Mouse X: " + std::to_wstring(mousePos.x)
		+ L" Mouse Y: " + std::to_wstring(mousePos.y);
	m_font->DrawString(m_sprites.get(), mouse.c_str(), XMFLOAT2(100, 70), Colors::Yellow);*/

	// Picking point
	/*std::wstring point = L"Point X: " + std::to_wstring(m_pickingPoint.x) +
		L"Point Y: " + std::to_wstring(m_pickingPoint.y) +
		L"Point Z: " + std::to_wstring(m_pickingPoint.z);
	m_font->DrawString(m_sprites.get(), point.c_str(), XMFLOAT2(100, 70), Colors::Yellow);*/

	// Frames Per Second
	std::wstring fps = L"FPS: " + std::to_wstring(int(1 / m_deltaTime));
	m_font->DrawString(m_sprites.get(), fps.c_str(), XMFLOAT2(800, 10), Colors::Red);

	// Current mode
	//std::wstring mode;
	//// Switch between modes
	//switch (m_editor)
	//{
	//case EDITOR::OBJECT_TRANSFORM: mode = L"MODE: OBJECT"; break;
	//case EDITOR::LANDSCAPE_SCULPT: mode = L"MODE: SCULPT"; break;
	//}
	//m_font->DrawString(m_sprites.get(), mode.c_str(), XMFLOAT2(100, 120), Colors::Yellow);

	m_sprites->End();
	
	// Coordinate system
	// Loop through selected objects
	for (int i = 0; i < m_selectedObjectIDs.size(); ++i)
	{		
		// Draw local axes
		DrawAxis(m_displayList[m_selectedObjectIDs[i]], m_selectedObjectIDs[i]);		
	}

	// WATER
	if (m_spawnWater) 
	{
		// Loop through all shapes
		for (int i = 0; i < m_shapes.size(); ++i) 
		{
			// Draw each shape
			m_shapes[i]->Draw(m_world, m_view, m_projection, Colors::Yellow);
		}
	}

    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetBackBufferRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}

void XM_CALLCONV Game::DrawGrid(FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color)
{
    m_deviceResources->PIXBeginEvent(L"Draw grid");

    auto context = m_deviceResources->GetD3DDeviceContext();
    context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
    context->OMSetDepthStencilState(m_states->DepthNone(), 0);
    context->RSSetState(m_states->CullCounterClockwise());

    m_batchEffect->Apply(context);

    context->IASetInputLayout(m_batchInputLayout.Get());

    m_batch->Begin();

    xdivs = std::max<size_t>(1, xdivs);
    ydivs = std::max<size_t>(1, ydivs);

    for (size_t i = 0; i <= xdivs; ++i)
    {
        float fPercent = float(i) / float(xdivs);
        fPercent = (fPercent * 2.0f) - 1.0f;
        XMVECTOR vScale = XMVectorScale(xAxis, fPercent);
        vScale = XMVectorAdd(vScale, origin);

        VertexPositionColor v1(XMVectorSubtract(vScale, yAxis), color);
        VertexPositionColor v2(XMVectorAdd(vScale, yAxis), color);
        m_batch->DrawLine(v1, v2);
    }

    for (size_t i = 0; i <= ydivs; i++)
    {
        float fPercent = float(i) / float(ydivs);
        fPercent = (fPercent * 2.0f) - 1.0f;
        XMVECTOR vScale = XMVectorScale(yAxis, fPercent);
        vScale = XMVectorAdd(vScale, origin);

        VertexPositionColor v1(XMVectorSubtract(vScale, xAxis), color);
        VertexPositionColor v2(XMVectorAdd(vScale, xAxis), color);
        m_batch->DrawLine(v1, v2);
    }

    m_batch->End();

    m_deviceResources->PIXEndEvent();
}
void Game::DrawAxis(DisplayObject object, int ID)
{
	// Setup vectors
	DirectX::FXMVECTOR origin = object.m_position;
	DirectX::FXMVECTOR x = { 1.f, 0.f, 0.f };
	DirectX::FXMVECTOR y = { 0.f, 1.f, 0.f };
	DirectX::FXMVECTOR z = { 0.f, 0.f, 1.5f };	
	DirectX::GXMVECTOR red = { 255.f, 0.f, 0.f };
	DirectX::GXMVECTOR green = { 0.f, 255.f, 0.f };
	DirectX::GXMVECTOR blue = { 0.f, 0.f, 255.f };
	XMVECTOR scaleX = XMVectorScale(x, 1); scaleX = XMVectorAdd(scaleX, origin);
	XMVECTOR scaleY = XMVectorScale(y, 1); scaleY = XMVectorAdd(scaleY, origin);
	XMVECTOR scaleZ = XMVectorScale(z, 1); scaleZ = XMVectorAdd(scaleZ, origin);

	// Setup context
	auto context = m_deviceResources->GetD3DDeviceContext();
	context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(m_states->DepthNone(), 0);
	context->RSSetState(m_states->CullCounterClockwise());

	m_batchEffect->Apply(context);

	context->IASetInputLayout(m_batchInputLayout.Get());

	m_batch->Begin();

	// X axis
	{
		VertexPositionColor v1(XMVectorSubtract(scaleX, x), red);
		VertexPositionColor v2(XMVectorAdd(scaleX, x), red);
		m_batch->DrawLine(v1, v2);

		///m_xRays[ID] = Ray(v1.position, { 1.f, 0.f, 0.f });
		m_xAxes[ID] = v1.position - v2.position;
	}

	// Y axis
	{
		VertexPositionColor v1(XMVectorSubtract(scaleY, y), green);
		VertexPositionColor v2(XMVectorAdd(scaleY, y), green);
		m_batch->DrawLine(v1, v2);

		///m_yRays[ID] = Ray(v1.position, { 0.f, 1.f, 0.f });
		m_yAxes[ID] = v1.position - v2.position;
	}

	// Z axis
	{
		VertexPositionColor v1(XMVectorSubtract(scaleZ, z), blue);
		VertexPositionColor v2(XMVectorAdd(scaleZ, z), blue);
		m_batch->DrawLine(v1, v2);

		///m_zRays[ID] = Ray(v1.position, { 0.f, 0.f, 1.f });
		m_zAxes[ID] = v1.position - v2.position;
	}

	m_batch->End();

	m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
}

void Game::OnDeactivated()
{
}

void Game::OnSuspending()
{
#ifdef DXTK_AUDIO
    m_audEngine->Suspend();
#endif
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

#ifdef DXTK_AUDIO
    m_audEngine->Resume();
#endif
}

void Game::OnWindowSizeChanged(int width, int height)
{	
	if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();
}

void Game::BuildDisplayList(std::vector<SceneObject> * sceneGraph)
{
	// Update local scene graph
	m_sceneGraph = *sceneGraph;
	
	auto device = m_deviceResources->GetD3DDevice();
	auto devicecontext = m_deviceResources->GetD3DDeviceContext();

	if (!m_displayList.empty())		//is the vector empty
	{
		m_displayList.clear();		//if not, empty it
	}

	//for every item in the scenegraph
	int numObjects = sceneGraph->size();
	for (int i = 0; i < numObjects; i++)
	{		
		//create a temp display object that we will populate then append to the display list.
		DisplayObject newDisplayObject;

		// Check & set model type
		if (sceneGraph->at(i).model_path == "database/data/grass.cmo") { newDisplayObject.m_type = MODEL_TYPE::GRASS; }
		else if (sceneGraph->at(i).model_path == "database/data/tree.cmo") { newDisplayObject.m_type = MODEL_TYPE::TREE; }
		else if (sceneGraph->at(i).model_path == "database/data/water.cmo") { newDisplayObject.m_type = MODEL_TYPE::WATER; }
		else { newDisplayObject.m_type = MODEL_TYPE::PLACEHOLDER; }
		
		//load model
		std::wstring modelwstr = StringToWCHART(sceneGraph->at(i).model_path);							//convect string to Wchar
		newDisplayObject.m_model = Model::CreateFromCMO(device, modelwstr.c_str(), *m_fxFactory, true);	//get DXSDK to load model "False" for LH coordinate system (maya)

		//Load Texture
		std::wstring texturewstr = StringToWCHART(sceneGraph->at(i).tex_diffuse_path);								//convect string to Wchar
		HRESULT rs;
		rs = CreateDDSTextureFromFile(device, texturewstr.c_str(), nullptr, &newDisplayObject.m_texture_diffuse);	//load tex into Shader resource

		//if texture fails.  load error default
		if (rs)
		{
			CreateDDSTextureFromFile(device, L"database/data/Error.dds", nullptr, &newDisplayObject.m_texture_diffuse);	//load tex into Shader resource
		}

		//apply new texture to models effect
		newDisplayObject.m_model->UpdateEffects([&](IEffect* effect) //This uses a Lambda function,  if you dont understand it: Look it up.
		{	
			auto lights = dynamic_cast<BasicEffect*>(effect);
			if (lights)
			{
				lights->SetTexture(newDisplayObject.m_texture_diffuse);			
			}
		});

		//set position
		newDisplayObject.m_position.x = sceneGraph->at(i).posX;
		newDisplayObject.m_position.y = sceneGraph->at(i).posY;
		newDisplayObject.m_position.z = sceneGraph->at(i).posZ;
		
		//setorientation
		newDisplayObject.m_orientation.x = sceneGraph->at(i).rotX;
		newDisplayObject.m_orientation.y = sceneGraph->at(i).rotY;
		newDisplayObject.m_orientation.z = sceneGraph->at(i).rotZ;

		//set scale
		newDisplayObject.m_scale.x = sceneGraph->at(i).scaX;
		newDisplayObject.m_scale.y = sceneGraph->at(i).scaY;
		newDisplayObject.m_scale.z = sceneGraph->at(i).scaZ;

		//set wireframe / render flags
		newDisplayObject.m_render		= sceneGraph->at(i).editor_render;
		newDisplayObject.m_wireframe	= sceneGraph->at(i).editor_wireframe;

		newDisplayObject.m_light_type		= sceneGraph->at(i).light_type;
		newDisplayObject.m_light_diffuse_r	= sceneGraph->at(i).light_diffuse_r;
		newDisplayObject.m_light_diffuse_g	= sceneGraph->at(i).light_diffuse_g;
		newDisplayObject.m_light_diffuse_b	= sceneGraph->at(i).light_diffuse_b;
		newDisplayObject.m_light_specular_r = sceneGraph->at(i).light_specular_r;
		newDisplayObject.m_light_specular_g = sceneGraph->at(i).light_specular_g;
		newDisplayObject.m_light_specular_b = sceneGraph->at(i).light_specular_b;
		newDisplayObject.m_light_spot_cutoff = sceneGraph->at(i).light_spot_cutoff;
		newDisplayObject.m_light_constant	= sceneGraph->at(i).light_constant;
		newDisplayObject.m_light_linear		= sceneGraph->at(i).light_linear;
		newDisplayObject.m_light_quadratic	= sceneGraph->at(i).light_quadratic;
		
		m_displayList.push_back(newDisplayObject);	

		// Setup rays for axes
		///m_xRays.push_back(Ray());
		///m_yRays.push_back(Ray());
		///m_zRays.push_back(Ray());
		m_xAxes.push_back(DirectX::SimpleMath::Vector3());
		m_yAxes.push_back(DirectX::SimpleMath::Vector3());
		m_zAxes.push_back(DirectX::SimpleMath::Vector3());
	}	
}

void Game::BuildDisplayChunk(ChunkObject * SceneChunk, std::vector<DirectX::SimpleMath::Vector2> location)
{
	//populate our local DISPLAYCHUNK with all the chunk info we need from the object stored in toolmain
	//which, to be honest, is almost all of it. Its mostly rendering related info so...
	m_displayChunk.PopulateChunkData(SceneChunk);		//migrate chunk data
	m_displayChunk.LoadHeightMap(m_deviceResources);
	m_displayChunk.m_terrainEffect->SetProjection(m_projection);
	///m_displayChunk.m_dualEffect->SetProjection(m_projection);
	/*m_displayChunk.m_terrainBlendOne->SetProjection(m_projection);
	m_displayChunk.m_terrainBlendTwo->SetProjection(m_projection);*/
	m_displayChunk.InitialiseBatch();
	
	// Initialise default texture across all geometry
	/*for (int i = 0; i < TERRAINRESOLUTION - 1; ++i)
	{
		for (int j = 0; j < TERRAINRESOLUTION - 1; ++j)
		{
			m_displayChunk.PaintTerrain(i, j, LANDSCAPE_PAINT::GRASS);
		}
	}*/
}

void Game::SaveDisplayChunk(ChunkObject * SceneChunk)
{
	m_displayChunk.SaveHeightMap();			//save heightmap to file.
}

void Game::MousePicking(int i)
{
	// Setup ray trace origin
	Vector3 origin = XMVector3Unproject(Vector3(m_inputCommands.mousePos.x, m_inputCommands.mousePos.y, 0.f),
		0,
		0,
		m_deviceResources->GetScreenViewport().Width,
		m_deviceResources->GetScreenViewport().Height,
		0,
		1,
		m_projection,
		m_view,
		m_world);

	// Setup ray trace destination
	Vector3 destination = XMVector3Unproject(Vector3(m_inputCommands.mousePos.x, m_inputCommands.mousePos.y, 1.f),
		0,
		0,
		m_deviceResources->GetScreenViewport().Width,
		m_deviceResources->GetScreenViewport().Height,
		0,
		1,
		m_projection,
		m_view,
		m_world);

	// Setup ray trace direction
	Vector3 direction = destination - origin;
	direction.Normalize();
	
	// Setup ray trace
	Ray ray(origin, direction);

	// Setup temp mouse position
	Vector3 mouse = ray.position;

	// If an object has been specified
	if (i != -1)
	{		
		// Setup temp object position
		Vector3 object = m_displayList[m_selectedObjectIDs[i]].m_position;

		// Distance between the two
		float distance = sqrt(
			(mouse.x - object.x) * (mouse.x - object.x) +
			(mouse.y - object.y) * (mouse.y - object.y) +
			(mouse.z - object.z) * (mouse.z - object.z));

		// Setup picking point
		m_pickingPoint = ray.position + (ray.direction * distance);

		/// Setup camera arcball here... ??
	}
	
	// Else, if no object has been specified
	else
	{
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
		m_pickingPoint = ray.position + (ray.direction * distance);

		if (temp.intersect) { m_pickingPoint.y += 1.f; }
	}

	// Setup camera arcball 
	///m_camera->SetLookAt(m_pickingPoint);
	/*DirectX::SimpleMath::Vector2 centre;
	centre.x = m_deviceResources->GetScreenViewport().Width / 2;
	centre.y = m_deviceResources->GetScreenViewport().Height / 2;	
	m_camera->TrackObject(centre.x, centre.y, m_displayList[m_selectedObjectIDs[i]].m_position, m_deltaTime);*/
}

bool Game::PickingObjects(bool select)
{
	// Controllers
	int selectedID = -1;
	float pickedDistance = 0.f, storedDistance = 1.f;
	bool firstPick = true;

	// Setup near & far planes of frustrum with mouse x,y passed from ToolMain
	const XMVECTOR nearSource = XMVectorSet(m_inputCommands.mousePos.x, m_inputCommands.mousePos.y, 0.f, 1.f);
	const XMVECTOR farSource = XMVectorSet(m_inputCommands.mousePos.x, m_inputCommands.mousePos.y, 1.f, 1.f);

	// Loop through entire object display list & pick em
	for (int i = 0; i < m_displayList.size(); i++)
	{
		// Get object scale factor & translation
		const XMVECTORF32 scale = { m_displayList[i].m_scale.x, m_displayList[i].m_scale.y, m_displayList[i].m_scale.z };
		const XMVECTORF32 translate = { m_displayList[i].m_position.x, m_displayList[i].m_position.y, m_displayList[i].m_position.z };

		// Convert euler angles into a quaternion for object rotation
		XMVECTOR rotate = Quaternion::CreateFromYawPitchRoll(m_displayList[i].m_orientation.y * PI / 180, 
			m_displayList[i].m_orientation.x * PI / 180, m_displayList[i].m_orientation.z * PI / 180);

		// Set selected object matrix in the world based on scale, rotation & translation
		XMMATRIX local = m_world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, scale, g_XMZero, rotate, translate);

		// UNPROJECT the points on the near & far plane, respecting the previously created matrix
		XMVECTOR nearPoint = XMVector3Unproject(nearSource, 0.f, 0.f, m_screenDimensions.right, m_screenDimensions.bottom,
			m_deviceResources->GetScreenViewport().MinDepth, m_deviceResources->GetScreenViewport().MaxDepth, m_projection, m_view, local);

		XMVECTOR farPoint = XMVector3Unproject(farSource, 0.f, 0.f, m_screenDimensions.right, m_screenDimensions.bottom,
			m_deviceResources->GetScreenViewport().MinDepth, m_deviceResources->GetScreenViewport().MaxDepth, m_projection, m_view, local);

		// Turn transformed points into picking vector
		XMVECTOR pickingVector = farPoint - nearPoint;
		pickingVector = XMVector3Normalize(pickingVector);

		// Loop through mesh list for object
		for (int j = 0; j < m_displayList[i].m_model.get()->meshes.size(); j++)
		{
			// Check for intersection
			if (m_displayList[i].m_model.get()->meshes[j]->boundingBox.Intersects(nearPoint, pickingVector, pickedDistance))
			{				
				// Update ID with the first intersected object
				if (firstPick)
				{
					firstPick = false;
					storedDistance = pickedDistance;
					selectedID = i;
				}
				// Update ID if a closer object has been intersected
				else if (pickedDistance < storedDistance)
				{
					storedDistance = pickedDistance;
					selectedID = i;
				}
			}
		}
	}

	// If an object has been selected
	if (selectedID != -1)
	{
		// If current selected ID is already in the vector
		if (std::count(m_selectedObjectIDs.begin(), m_selectedObjectIDs.end(), selectedID))
		{
			// If isn't selecting
			if (!select)
			{				
				// Remove from vector storage
				m_selectedObjectIDs.erase(std::remove(m_selectedObjectIDs.begin(), m_selectedObjectIDs.end(), selectedID), m_selectedObjectIDs.end());
			}
		}
		// Else, if current selected ID is new
		else
		{
			// If is selecting
			if (select)
			{
				// Add to vector storage
				m_selectedObjectIDs.push_back(selectedID);
			}
		}

		// Object has been intersected
		return true;
	}
	else
	{
		// Clear entire selection vector
		///m_selectedObjectIDs.clear();
		
		// Object hasn't been intersected
		return false;
	}
}

bool Game::ObjectIntersection(int i)
{
	// Controllers
	float pickedDistance = 0.f, storedDistance = 1.f;

	// Setup near & far planes of frustrum with mouse x,y passed from ToolMain
	const XMVECTOR nearSource = XMVectorSet(m_inputCommands.mousePos.x, m_inputCommands.mousePos.y, 0.f, 1.f);
	const XMVECTOR farSource = XMVectorSet(m_inputCommands.mousePos.x, m_inputCommands.mousePos.y, 1.f, 1.f);

	// Get object scale factor & translation
	const XMVECTORF32 scale = { m_displayList[m_selectedObjectIDs[i]].m_scale.x, m_displayList[m_selectedObjectIDs[i]].m_scale.y, m_displayList[m_selectedObjectIDs[i]].m_scale.z };
	const XMVECTORF32 translate = { m_displayList[m_selectedObjectIDs[i]].m_position.x, m_displayList[m_selectedObjectIDs[i]].m_position.y, m_displayList[m_selectedObjectIDs[i]].m_position.z };

	// Convert euler angles into a quaternion for object rotation
	XMVECTOR rotate = Quaternion::CreateFromYawPitchRoll(m_displayList[m_selectedObjectIDs[i]].m_orientation.y * PI / 180,
		m_displayList[m_selectedObjectIDs[i]].m_orientation.x * PI / 180, m_displayList[m_selectedObjectIDs[i]].m_orientation.z * PI / 180);

	// Set selected object matrix in the world based on scale, rotation & translation
	XMMATRIX local = m_world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, scale, g_XMZero, rotate, translate);

	// UNPROJECT the points on the near & far plane, respecting the previously created matrix
	XMVECTOR nearPoint = XMVector3Unproject(nearSource, 0.f, 0.f, m_screenDimensions.right, m_screenDimensions.bottom,
		m_deviceResources->GetScreenViewport().MinDepth, m_deviceResources->GetScreenViewport().MaxDepth, m_projection, m_view, local);

	XMVECTOR farPoint = XMVector3Unproject(farSource, 0.f, 0.f, m_screenDimensions.right, m_screenDimensions.bottom,
		m_deviceResources->GetScreenViewport().MinDepth, m_deviceResources->GetScreenViewport().MaxDepth, m_projection, m_view, local);

	// Turn transformed points into picking vector
	XMVECTOR pickingVector = farPoint - nearPoint;
	pickingVector = XMVector3Normalize(pickingVector);

	// Loop through mesh list for object
	for (int j = 0; j < m_displayList[m_selectedObjectIDs[i]].m_model.get()->meshes.size(); j++)
	{
		// Check for intersection
		return (m_displayList[m_selectedObjectIDs[i]].m_model.get()->meshes[j]->boundingBox.Intersects(nearPoint, pickingVector, pickedDistance));
	}
}

TERRAIN Game::PickingTerrain()
{
	// Setup ray trace origin
	Vector3 origin = XMVector3Unproject(Vector3(m_inputCommands.mousePos.x, m_inputCommands.mousePos.y, 0.f),
		0,
		0,
		m_deviceResources->GetScreenViewport().Width,
		m_deviceResources->GetScreenViewport().Height,
		0,
		1,
		m_projection,
		m_view,
		m_world);

	// Setup ray trace destination
	Vector3 destination = XMVector3Unproject(Vector3(m_inputCommands.mousePos.x, m_inputCommands.mousePos.y, 1.f),
		0,
		0,
		m_deviceResources->GetScreenViewport().Width,
		m_deviceResources->GetScreenViewport().Height,
		0,
		1,
		m_projection,
		m_view,
		m_world);

	// Setup ray trace direction
	Vector3 direction = destination - origin;
	direction.Normalize();

	// Calculate chunk intersection
	m_selectedTerrain = TerrainIntersection(Ray(origin, direction));

	// Update mouse picking point in 3D world
	m_pickingPoint = m_selectedTerrain.position;

	// Return selection
	return m_selectedTerrain;
}

std::vector<TERRAIN> Game::PickingTerrains()
{
	// Controllers
	bool alreadyChosen = false;
	/*int selectedID = -1;
	float pickedDistance = 0.f, storedDistance = 1.f;
	bool firstPick = true;*/

	// Setup ray trace origin
	Vector3 origin = XMVector3Unproject(Vector3(m_inputCommands.mousePos.x, m_inputCommands.mousePos.y, 0.f),
		0,
		0,
		m_deviceResources->GetScreenViewport().Width,
		m_deviceResources->GetScreenViewport().Height,
		0,
		1,
		m_projection,
		m_view,
		m_world);

	// Setup ray trace destination
	Vector3 destination = XMVector3Unproject(Vector3(m_inputCommands.mousePos.x, m_inputCommands.mousePos.y, 1.f),
		0,
		0,
		m_deviceResources->GetScreenViewport().Width,
		m_deviceResources->GetScreenViewport().Height,
		0,
		1,
		m_projection,
		m_view,
		m_world);

	// Setup ray trace direction
	Vector3 direction = destination - origin;
	direction.Normalize();

	// Calculate chunk intersection
	TERRAIN terrain = TerrainIntersection(Ray(origin, direction));

	// If ray has intersected the terrain
	if (terrain.intersect)
	{
		// Loop through currently selected terrains
		for (int i = 0; i < m_selectedTerrains.size(); ++i)
		{
			// If selected terrain row/column matches current terrain row/column
			///if (terrain.ID == m_selectedTerrains[i].ID)
			if (terrain.row == m_selectedTerrains[i].row &&
				terrain.column == m_selectedTerrains[i].column)
			{
				// Set as already chosen
				alreadyChosen = true;

				// Deselect (un-highlight)
				m_displayChunk.SetSelected(false, terrain.row, terrain.column);

				// Remove from storage
				m_selectedTerrains.erase(m_selectedTerrains.begin() + i);

				// Loop through terrain geometry
				//for (int i = 0; i < (TERRAINRESOLUTION - 1); i++)
				//{
				//	for (int j = 0; j < (TERRAINRESOLUTION - 1); j++)
				//	{
				//		// If selected chunk ID matches geometry ID
				//		if (terrain.ID == m_displayChunk.GetGeometry(i, j).ID)
				//		{
				//			// Deselect geometry
				//			m_displayChunk.SetSelected(false, m_displayChunk.GetGeometry(i, j).ID);
				//		}
				//	}
				//}
			}
		}

		// If selected chunk isn't already chosen
		if (!alreadyChosen)
		{
			// Select (highlight)
			m_displayChunk.SetSelected(true, terrain.row, terrain.column);
			
			// Add to vector storage
			m_selectedTerrains.push_back(terrain);
		}
	}

	// Return chunks
	return m_selectedTerrains;	
}

TERRAIN Game::TerrainIntersection(DirectX::SimpleMath::Ray ray)
{
	// Local chunk
	TERRAIN terrain;
	terrain.intersect = false;
	
	// Define controllers
	float distance = 10000, pickedDistance = 0.f, storedDistance = 1.f;
	bool firstPick = true;
	Vector3 one = ray.position;
	Vector3 two = ray.position + (ray.direction * distance);

	// Loop through terrain row size
	for (size_t i = 0; i < TERRAINRESOLUTION - 1; ++i)
	{
		// Loop through terrain column size
		for (size_t j = 0; j < TERRAINRESOLUTION - 1; ++j)
		{
			// Setup local vectors of current geometry corner positions
			Vector3 bottomLeft	= m_displayChunk.GetGeometry(i, j).position;
			Vector3 bottomRight = m_displayChunk.GetGeometry(i, j + 1).position;
			Vector3 topRight	= m_displayChunk.GetGeometry(i + 1, j + 1).position;
			Vector3 topLeft		= m_displayChunk.GetGeometry(i + 1, j).position;

			// If ray intersects with either triangle in current geometry
			if (ray.Intersects(bottomLeft, bottomRight, topRight, pickedDistance) || ray.Intersects(bottomLeft, topLeft, topRight, pickedDistance))
			{
				// If current geometry is within ray trace bounds
				if (m_displayChunk.GetGeometry(i, j).position.y < one.y && m_displayChunk.GetGeometry(i, j).position.y > two.y)
				{
					// If terrain is first picked
					if (firstPick)
					{
						// Setup values to return
						terrain.row = i;
						terrain.column = j;
						terrain.intersect = true;
						terrain.position = m_displayChunk.GetGeometry(i, j).position;

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
						terrain.position = m_displayChunk.GetGeometry(i, j).position;

						// Store current distance
						storedDistance = distance;
					}					

					/// Set geometry as selected
					///m_displayChunk.SetSelected(true, m_displayChunk.GetGeometry(i, j).ID);
				}
			}
		}
	}

	// Return empty values if no intersection
	return terrain;
}

DirectX::SimpleMath::Vector3 Game::GetDragPoint(DirectX::SimpleMath::Vector3 * dragLine, DirectX::SimpleMath::Vector3 * unProjLine)
{
	DirectX::SimpleMath::Vector3 P0(dragLine[0]);
	DirectX::SimpleMath::Vector3 Vp(dragLine[1]);
	DirectX::SimpleMath::Vector3 Q0(unProjLine[0]);
	DirectX::SimpleMath::Vector3 Vq(unProjLine[1]);

	float a, b[2], c[2], s;

	Vp.Normalize();
	Vq.Normalize();
	a = 1.f / (1.f - (Vp.Dot(Vq) * Vp.Dot(Vq)));

	b[0] = a;
	b[1] = Vp.Dot(Vq) * a;

	c[0] = DirectX::SimpleMath::Vector3(Q0 - P0).Dot(Vp);
	c[1] = DirectX::SimpleMath::Vector3(P0 - Q0).Dot(Vq);

	s = b[0] * c[0] * b[1] * c[1];
	DirectX::SimpleMath::Vector3 dragPoint((P0 + Vp) * s);

	return dragPoint;
}

// Creation functions...

SceneObject Game::CreateObject(OBJECT_SPAWN spawn, DirectX::SimpleMath::Vector3 position)
{
	// Setup temp cube
	SceneObject object;	
	
	// Define object values
	object.ID = m_sceneGraph.size() + 1;
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
	object.name = "Name";
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
	case OBJECT_SPAWN::GRASS:
	{
		// Set model to grass
		object.m_type = MODEL_TYPE::GRASS;
		object.model_path = "database/data/grass.cmo";
		object.tex_diffuse_path = "database/data/grass.dds";
	}
	break;
	case OBJECT_SPAWN::TREE:
	{
		// Set model to tree
		object.m_type = MODEL_TYPE::TREE;
		object.model_path = "database/data/tree.cmo";
		object.tex_diffuse_path = "database/data/tree.dds";
	}
	break;
	case OBJECT_SPAWN::WATER:
	{
		// Set model to water
		object.m_type = MODEL_TYPE::WATER;
		object.model_path = "database/data/water.cmo";
		object.tex_diffuse_path = "database/data/water.dds";
		m_waterIDs.push_back(object.ID);
	}
	break;
	}

	// Add object to scene graph
	m_sceneGraph.push_back(object);
	
	// Return object
	return object;
}

Water Game::CreateWater(DirectX::SimpleMath::Vector3 position)
{
	// Setup water
	Water water;
	
	// Setup cube
	///m_shapes.push_back(GeometricPrimitive::CreateBox(m_deviceResources->GetD3DDeviceContext(), { 5.f, 5.f, 5.f }));
	///m_shapes.push_back(GeometricPrimitive::CreateBox(m_deviceResources->GetD3DDeviceContext(), { -5.f, -5.f, -5.f }));
	///m_shapes.push_back(GeometricPrimitive::CreateBox(m_deviceResources->GetD3DDeviceContext(), { 5.f, 5.f, 5.f }, false));
	
	///m_shape = GeometricPrimitive::CreateSphere(m_deviceResources->GetD3DDeviceContext());	
	///m_shape = GeometricPrimitive::CreateBox(m_deviceResources->GetD3DDeviceContext(), { 5.f, 5.f, 5.f });
	///m_waterPositions.push_back(position);

	///DirectX::SimpleMath::Plane plane(position, 0);

	if (!m_spawnWater) { m_spawnWater = true; }
	
	return water;
}

#ifdef DXTK_AUDIO

void Game::NewAudioDevice()
{
    if (m_audEngine && !m_audEngine->IsAudioDevicePresent())
    {
        // Setup a retry in 1 second
        m_audioTimerAcc = 1.f;
        m_retryDefault = true;
    }
}
#endif

#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto device = m_deviceResources->GetD3DDevice();

    m_states = std::make_unique<CommonStates>(device);

    m_fxFactory = std::make_unique<EffectFactory>(device);
	m_fxFactory->SetDirectory(L"database/data/"); //fx Factory will look in the database directory
	m_fxFactory->SetSharing(false);	//we must set this to false otherwise it will share effects based on the initial tex loaded (When the model loads) rather than what we will change them to.

    m_sprites = std::make_unique<SpriteBatch>(context);

    m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);

    m_batchEffect = std::make_unique<BasicEffect>(device);
    m_batchEffect->SetVertexColorEnabled(true);

    {
        void const* shaderByteCode;
        size_t byteCodeLength;

        m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

        DX::ThrowIfFailed(
            device->CreateInputLayout(VertexPositionColor::InputElements,
                VertexPositionColor::InputElementCount,
                shaderByteCode, byteCodeLength,
                m_batchInputLayout.ReleaseAndGetAddressOf())
        );
    }

    m_font = std::make_unique<SpriteFont>(device, L"SegoeUI_18.spritefont");

//    m_shape = GeometricPrimitive::CreateTeapot(context, 4.f, 8);

    // SDKMESH has to use clockwise winding with right-handed coordinates, so textures are flipped in U
    m_model = Model::CreateFromSDKMESH(device, L"tiny.sdkmesh", *m_fxFactory);
	

    // Load textures
    DX::ThrowIfFailed(
        CreateDDSTextureFromFile(device, L"seafloor.dds", nullptr, m_texture1.ReleaseAndGetAddressOf())
    );

    DX::ThrowIfFailed(
        CreateDDSTextureFromFile(device, L"windowslogo.dds", nullptr, m_texture2.ReleaseAndGetAddressOf())
    );

}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    auto size = m_deviceResources->GetOutputSize();
    float aspectRatio = float(size.right) / float(size.bottom);
    float fovAngleY = 70.0f * XM_PI / 180.0f;

    // This is a simple example of change that can be made when the app is in
    // portrait or snapped view.
    if (aspectRatio < 1.0f)
    {
        fovAngleY *= 2.0f;
    }

    // This sample makes use of a right-handed coordinate system using row-major matrices.
    m_projection = Matrix::CreatePerspectiveFieldOfView(
        fovAngleY,
        aspectRatio,
        0.01f,
        1000.0f
    );

    m_batchEffect->SetProjection(m_projection);
	
	/*m_screenCentre.x = (float)size.right / 2;
	m_screenCentre.y = (float)size.bottom / 2;*/
}

void Game::OnDeviceLost()
{
    m_states.reset();
    m_fxFactory.reset();
    m_sprites.reset();
    m_batch.reset();
    m_batchEffect.reset();
    m_font.reset();
    m_shape.reset();
	for (int i = 0; i < m_shapes.size(); ++i) { m_shapes[i].reset(); }
    m_model.reset();
    m_texture1.Reset();
    m_texture2.Reset();
    m_batchInputLayout.Reset();
	///m_displayChunk.m_normalMap.Reset();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion

std::wstring StringToWCHART(std::string s)
{

	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}
