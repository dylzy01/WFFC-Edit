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
	// Handle all input
	HandleInput();
	
	// Custom camera
	UpdateCamera(timer.GetElapsedSeconds());

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
	// Change mode to OBJECT
	if (m_inputCommands.ONE) { m_mode = MODE::OBJECT; }

	// Change mode to LANDSCAPE
	else if (m_inputCommands.TWO) { m_mode = MODE::LANDSCAPE; }

	// Switch wireframe on/off
	if (m_inputCommands.ZERO)
	{
		if (m_wireframe) { m_wireframe = false; }
		else { m_wireframe = true; }
	}

	// Switch between modes
	switch (m_mode)
	{
	case MODE::OBJECT:
	{
		// Change modification to SCALE
		if (m_inputCommands.SCALE) { m_modify = MODIFY::SCALE; }
		
		// Change modification to TRANSLATE
		else if (m_inputCommands.TRANSLATE) { m_modify = MODIFY::TRANSLATE; }

		// Change modification to ROTATE
		else if (m_inputCommands.ROTATE) { m_modify = MODIFY::ROTATE; }
		
		// If mouse is being pressed
		if (m_inputCommands.mouseLeft)
		{						
			// If any objects are selected
			if (m_selectedObjectIDs.size() != 0)
			{
				// If mouse position should be stored
				if (m_inputCommands.storeOnce)
				{
					// Store current mouse position
					m_storedMousePosition = m_inputCommands.mousePos;

					// Reset controller
					m_inputCommands.storeOnce = false;
				}

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
							m_storedObjectScales.push_back(m_displayList[i].m_scale);
							m_storedObjectTranslations.push_back(m_displayList[i].m_position);
							m_storedObjectRotations.push_back(m_displayList[i].m_orientation);
						}						
					}

					// Reset controller
					m_storeObjectDetails = false;
				}

				// Loop through selected objects
				for (int i = 0; i < m_selectedObjectIDs.size(); ++i)
				{
					// Switch between S, R, T
					switch (m_modify)
					{
					case MODIFY::SCALE:
					{
						// If mouse has moved
						if (m_inputCommands.mousePos != m_storedMousePosition)
						{
							// If should be scaled on the X axis
							if (m_inputCommands.X)
							{
								// Scale selected object based on how much the mouse has moved from its stored position
								m_displayList[m_selectedObjectIDs[i]].m_scale.x *= (m_inputCommands.mousePos.x - m_storedMousePosition.x);
							}

							// Else, if should be scaled on the Y axis
							else if (m_inputCommands.Y)
							{
								// Scale selected object based on how much the mouse has moved from its stored position
								m_displayList[m_selectedObjectIDs[i]].m_scale.y *= (m_inputCommands.mousePos.y - m_storedMousePosition.y);
							}

							// Else, if should be scaled on the Z axis
							else if (m_inputCommands.Z)
							{
								// Scale selected object based on how much the mouse has moved from its stored position
								m_displayList[m_selectedObjectIDs[i]].m_scale.z *= (m_inputCommands.mousePos.x - m_storedMousePosition.x) + (m_inputCommands.mousePos.y - m_storedMousePosition.y);
							}

							// Else, if should be scaled freely
							else
							{
							}
						}
					}
					break;
					case MODIFY::TRANSLATE:
					{
						MousePicking();
						///PickingObjects();
						///PickingTerrain();
						
						// If mouse has moved
						///if (m_inputCommands.mousePos != m_storedMousePosition)
						{
							// If should be translated on the X axis
							if (m_inputCommands.X)
							{
								// Translate selected object based on how much the mouse has moved from its stored position
								///m_displayList[m_selectedObjectIDs[i]].m_position.x *= (m_inputCommands.mousePos.x - m_storedMousePosition.x);
								m_displayList[m_selectedObjectIDs[i]].m_position.x += m_pickingPoint.x;
							}

							// Else, if should be translated on the Y axis
							else if (m_inputCommands.Y)
							{
								// Translate selected object based on how much the mouse has moved from its stored position
								m_displayList[m_selectedObjectIDs[i]].m_position.y *= (m_inputCommands.mousePos.y - m_storedMousePosition.y);
							}

							// Else, if should be translated on the Z axis
							else if (m_inputCommands.Z)
							{
								// Translate selected object based on how much the mouse has moved from its stored position
								m_displayList[m_selectedObjectIDs[i]].m_position.x *= (m_inputCommands.mousePos.x - m_storedMousePosition.x) + (m_inputCommands.mousePos.y - m_storedMousePosition.y);
							}

							// Else, if should be translated freely
							else
							{
								m_displayList[m_selectedObjectIDs[i]].m_position = m_pickingPoint;
							}
						}
					}
					break;
					case MODIFY::ROTATE:
					{
						// If mouse has moved
						if (m_inputCommands.mousePos != m_storedMousePosition)
						{
							// If should be rotated on the X axis
							if (m_inputCommands.X)
							{
								// Rotate selected object based on how much the mouse has moved from its stored position
								m_displayList[m_selectedObjectIDs[i]].m_orientation.x *= (m_inputCommands.mousePos.x - m_storedMousePosition.x);
							}

							// Else, if should be rotated on the Y axis
							else if (m_inputCommands.Y)
							{
								// Rotate selected object based on how much the mouse has moved from its stored position
								m_displayList[m_selectedObjectIDs[i]].m_orientation.y *= (m_inputCommands.mousePos.y - m_storedMousePosition.y);
							}

							// Else, if should be rotated on the Z axis
							else if (m_inputCommands.Z)
							{
								// Rotate selected object based on how much the mouse has moved from its stored position
								m_displayList[m_selectedObjectIDs[i]].m_orientation.x *= (m_inputCommands.mousePos.x - m_storedMousePosition.x) + (m_inputCommands.mousePos.y - m_storedMousePosition.y);
							}

							// Else, if should be rotated freely
							else
							{
							}
						}
					}
					break;
					}
				}
			}
		}
	}
	break;
	case MODE::LANDSCAPE:
	{
		// If mouse is being pressed
		if (m_inputCommands.mouseLeft)
		{
			// If selected terrain is intersected by ray trace
			if (m_selectedTerrain.intersect)
			{
				// If Z is being pressed
				if (m_inputCommands.INCREASE)
				{
					// Increase height of selected terrain
					m_displayChunk.UpdateTerrainHeight(m_selectedTerrain.row, m_selectedTerrain.column, HEIGHT::INCREASE);
				}

				// Else, if X is being pressed
				else if (m_inputCommands.DECREASE)
				{
					// Decrease height of selecteed terrain
					m_displayChunk.UpdateTerrainHeight(m_selectedTerrain.row, m_selectedTerrain.column, HEIGHT::DECREASE);
				}

				// Else, if C is being pressed
				else if (m_inputCommands.FLATTEN)
				{
					// If first position should be stored
					if (m_storeTerrainPosition)
					{
						m_storeTerrainPosition = false;
						m_storedTerrainPosition = m_selectedTerrain.position;
					}

					// Check if selected terrain height doesn't match stored position height
					if (m_selectedTerrain.position.y != m_storedTerrainPosition.y)
					{
						// Flatten height of selected terrain
						m_displayChunk.UpdateTerrainHeight(m_selectedTerrain.row, m_selectedTerrain.column, HEIGHT::FLATTEN, m_storedTerrainPosition);
					}				
				}
			}
		}			
	}
	break;
	}
}

void Game::UpdateCamera(float deltaTime)
{
	DirectX::SimpleMath::Vector2 centre;
	centre.x = m_deviceResources->GetScreenViewport().Width / 2;
	centre.y = m_deviceResources->GetScreenViewport().Height / 2;

	// Cursor position
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	ScreenToClient(m_window, &cursorPos);

	m_camera->HandleInput(&m_inputCommands, deltaTime, centre.x, centre.y, cursorPos);
	m_camera->Update();

	//apply camera vectors
	m_view = Matrix::CreateLookAt(m_camera->GetPosition(), m_camera->GetLookAt(), Vector3::UnitY);

	m_batchEffect->SetView(m_view);
	m_batchEffect->SetWorld(Matrix::Identity);
	m_displayChunk.m_terrainEffect->SetView(m_view);
	m_displayChunk.m_terrainEffect->SetWorld(Matrix::Identity);
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

	//CAMERA POSITION ON HUD
	m_sprites->Begin();

	WCHAR   Buffer[256];

	// Camera
	std::wstring pos = L"Pos X: " + std::to_wstring(m_camera->GetPosition().x)
		+ L" Pos Y: " + std::to_wstring(m_camera->GetPosition().y)
		+ L" Pos Z: " + std::to_wstring(m_camera->GetPosition().z);
	std::wstring rot = L"Rot X: " + std::to_wstring(m_camera->GetLookAt().x)
		+ L" Rot Y: " + std::to_wstring(m_camera->GetLookAt().y)
		+ L" Rot Z: " + std::to_wstring(m_camera->GetLookAt().z);
	m_font->DrawString(m_sprites.get(), pos.c_str(), XMFLOAT2(100, 10), Colors::Yellow);
	m_font->DrawString(m_sprites.get(), rot.c_str(), XMFLOAT2(100, 40), Colors::Yellow);

	// Mouse
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(m_window, &mousePos);
	std::wstring mouse = L"Mouse X: " + std::to_wstring(mousePos.x)
		+ L" Mouse Y: " + std::to_wstring(mousePos.y);
	m_font->DrawString(m_sprites.get(), mouse.c_str(), XMFLOAT2(100, 70), Colors::Yellow);

	// Current mode
	std::wstring mode;
	// Switch between modes
	switch (m_mode)
	{
	case MODE::OBJECT: mode = L"MODE: OBJECT"; break;
	case MODE::LANDSCAPE: mode = L"MODE: LANDSCAPE"; break;
	}
	m_font->DrawString(m_sprites.get(), mode.c_str(), XMFLOAT2(100, 120), Colors::Yellow);

	m_sprites->End();


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

void Game::BuildDisplayList(std::vector<SceneObject> * SceneGraph)
{
	auto device = m_deviceResources->GetD3DDevice();
	auto devicecontext = m_deviceResources->GetD3DDeviceContext();

	if (!m_displayList.empty())		//is the vector empty
	{
		m_displayList.clear();		//if not, empty it
	}

	//for every item in the scenegraph
	int numObjects = SceneGraph->size();
	for (int i = 0; i < numObjects; i++)
	{		
		//create a temp display object that we will populate then append to the display list.
		DisplayObject newDisplayObject;
		
		//load model
		std::wstring modelwstr = StringToWCHART(SceneGraph->at(i).model_path);							//convect string to Wchar
		newDisplayObject.m_model = Model::CreateFromCMO(device, modelwstr.c_str(), *m_fxFactory, true);	//get DXSDK to load model "False" for LH coordinate system (maya)

		//Load Texture
		std::wstring texturewstr = StringToWCHART(SceneGraph->at(i).tex_diffuse_path);								//convect string to Wchar
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
		newDisplayObject.m_position.x = SceneGraph->at(i).posX;
		newDisplayObject.m_position.y = SceneGraph->at(i).posY;
		newDisplayObject.m_position.z = SceneGraph->at(i).posZ;
		
		//setorientation
		newDisplayObject.m_orientation.x = SceneGraph->at(i).rotX;
		newDisplayObject.m_orientation.y = SceneGraph->at(i).rotY;
		newDisplayObject.m_orientation.z = SceneGraph->at(i).rotZ;

		//set scale
		newDisplayObject.m_scale.x = SceneGraph->at(i).scaX;
		newDisplayObject.m_scale.y = SceneGraph->at(i).scaY;
		newDisplayObject.m_scale.z = SceneGraph->at(i).scaZ;

		//set wireframe / render flags
		newDisplayObject.m_render		= SceneGraph->at(i).editor_render;
		newDisplayObject.m_wireframe	= SceneGraph->at(i).editor_wireframe;

		newDisplayObject.m_light_type		= SceneGraph->at(i).light_type;
		newDisplayObject.m_light_diffuse_r	= SceneGraph->at(i).light_diffuse_r;
		newDisplayObject.m_light_diffuse_g	= SceneGraph->at(i).light_diffuse_g;
		newDisplayObject.m_light_diffuse_b	= SceneGraph->at(i).light_diffuse_b;
		newDisplayObject.m_light_specular_r = SceneGraph->at(i).light_specular_r;
		newDisplayObject.m_light_specular_g = SceneGraph->at(i).light_specular_g;
		newDisplayObject.m_light_specular_b = SceneGraph->at(i).light_specular_b;
		newDisplayObject.m_light_spot_cutoff = SceneGraph->at(i).light_spot_cutoff;
		newDisplayObject.m_light_constant	= SceneGraph->at(i).light_constant;
		newDisplayObject.m_light_linear		= SceneGraph->at(i).light_linear;
		newDisplayObject.m_light_quadratic	= SceneGraph->at(i).light_quadratic;
		
		m_displayList.push_back(newDisplayObject);		
	}	
}

void Game::BuildDisplayChunk(ChunkObject * SceneChunk)
{
	//populate our local DISPLAYCHUNK with all the chunk info we need from the object stored in toolmain
	//which, to be honest, is almost all of it. Its mostly rendering related info so...
	m_displayChunk.PopulateChunkData(SceneChunk);		//migrate chunk data
	m_displayChunk.LoadHeightMap(m_deviceResources);
	m_displayChunk.m_terrainEffect->SetProjection(m_projection);
	m_displayChunk.InitialiseBatch();
}

void Game::SaveDisplayChunk(ChunkObject * SceneChunk)
{
	m_displayChunk.SaveHeightMap();			//save heightmap to file.
}

void Game::MousePicking()
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
	
	// Local vector storage
	Vector3 mouse = ray.position;
	Vector3 object = m_displayList[m_selectedObjectIDs[0]].m_position;

	// Distance between the two
	float distance = sqrt(
		(mouse.x - object.x) * (mouse.x - object.x) +
		(mouse.y - object.y) * (mouse.y - object.y) +
		(mouse.z - object.z) * (mouse.z - object.z));

	// Setup picking point
	m_pickingPoint = ray.position + (ray.direction * distance);
}

bool Game::PickingObjects()
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

				// Update mouse picking point in 3D world
				m_pickingPoint = m_displayList[i].m_position;
			}
		}
	}

	// If an object has been selected
	if (selectedID != -1)
	{
		// If current selected ID is already in the vector
		if (std::count(m_selectedObjectIDs.begin(), m_selectedObjectIDs.end(), selectedID))
		{
			// Remove from vector storage
			m_selectedObjectIDs.erase(std::remove(m_selectedObjectIDs.begin(), m_selectedObjectIDs.end(), selectedID), m_selectedObjectIDs.end());
		}
		// Else, if current selected ID is new
		else
		{
			// Add to vector storage
			m_selectedObjectIDs.push_back(selectedID);
		}

		// Object has been intersected
		return true;
	}
	else
	{
		// Object hasn't been intersected
		return false;
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
			if (ray.Intersects(bottomLeft, bottomRight, topRight, distance) || ray.Intersects(bottomLeft, topLeft, topRight, distance))
			{
				// If current geometry is within ray trace bounds
				if (m_displayChunk.GetGeometry(i, j).position.y < one.y && m_displayChunk.GetGeometry(i, j).position.y > two.y)
				{
					// Setup values to return
					terrain.row = i;
					terrain.column = j;
					terrain.intersect = true;
					terrain.position = m_displayChunk.GetGeometry(i, j).position;

					// Set geometry as selected
					///m_displayChunk.SetSelected(true, m_displayChunk.GetGeometry(i, j).ID);

					// Return values from intersection
					return terrain;
				}
			}
		}
	}

	// Return empty values if no intersection
	return terrain;
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
    m_model.reset();
    m_texture1.Reset();
    m_texture2.Reset();
    m_batchInputLayout.Reset();
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
