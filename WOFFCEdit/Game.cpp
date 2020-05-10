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
	m_fpsCount = m_fpsTrue = 0;
}

Game::~Game()
{
	///TextureShader::Shutdown();

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

	// Set shader parameters
	ShaderManager::SetWorld(&m_world);
	ShaderManager::SetView(&m_view);
	ShaderManager::SetProjection(&m_projection);
	
	// Custom camera
	UpdateCamera();

	// Water waves
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

	// Setup view matrix
	m_view = Matrix::CreateLookAt(m_camera->GetPosition(), m_camera->GetLookAt(), Vector3::UnitY);

	// Apply camera matrices
	m_batchEffect->SetView(m_view);
	m_batchEffect->SetWorld(Matrix::Identity);
	m_displayChunk.m_effect->SetView(m_view);
	m_displayChunk.m_effect->SetWorld(Matrix::Identity);
	///m_displayChunk.m_effectBlend->SetView(m_view);
	///m_displayChunk.m_effectBlend->SetWorld(Matrix::Identity);
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
		if (m_displayList[i].m_isWater)
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

	DirectX::SimpleMath::Plane p(0, 1, 0, 0);

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

	// Create vector of only lights
	std::vector<DisplayObject> lights;
	for (int i = 0; i < m_displayList.size(); ++i)
	{
		// If current object is a light
		if (m_displayList[i].m_objectType == OBJECT_TYPE::LIGHT)
		{
			lights.push_back(m_displayList[i]);
		}
	}
	
	//Render the batch,  This is handled in the Display chunk becuase it has the potential to get complex
	m_displayChunk.RenderBatch(m_deviceResources, lights);

	//HUD
	m_sprites->Begin();

	WCHAR   Buffer[256];

	std::wstring fps = L"FPS: " + std::to_wstring(int(m_timer.GetFramesPerSecond() / m_deltaTime) / 10);
	m_font->DrawString(m_sprites.get(), fps.c_str(), XMFLOAT2(800, m_screenDimensions.top + 10), Colors::Red);

	// Show autosave
	std::wstring as = L"";
	if (!m_autosaveTimer) { 
		if (m_autosave) { as = L"Autosave ON"; }
		else { as = L"Autosave OFF"; }		
	}		
	else if (m_autosaveTimer) { as = L"Autosave in: " + std::to_wstring(m_autosaveCount); }
	m_font->DrawString(m_sprites.get(), as.c_str(), XMFLOAT2(10, m_screenDimensions.bottom - 40), Colors::Red);

	m_sprites->End();	

	// Selection system
	if (m_displayList.size() != 0)
	{
		// Loop through selected objects
		for (int i = 0; i < m_selectedObjectIDs.size(); ++i)
		{
			// If selected object ID is within range and valid
			if (m_selectedObjectIDs[i] >= 0 && m_selectedObjectIDs[i] <= 100 && m_selectedObjectIDs[i] != -1);
			{
				// Loop through display list
				for (int j = 0; j < m_displayList.size(); ++j)
				{
					// If IDs match
					if (m_selectedObjectIDs[i] == m_displayList[j].m_ID)
					{
						// Draw bounding box & local axes
						DrawDebug(j);
					}
				}				
			}
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

// Draw local axes and bounding boxes
void Game::DrawDebug(int i)
{
	// Setup vectors
	DirectX::FXMVECTOR x = { 3.f, 0.f, 0.f };
	DirectX::FXMVECTOR y = { 0.f, 3.f, 0.f };
	DirectX::FXMVECTOR z = { 0.f, 0.f, 3.5f };
	DirectX::GXMVECTOR red = { 255.f, 0.f, 0.f };
	DirectX::GXMVECTOR green = { 0.f, 255.f, 0.f };
	DirectX::GXMVECTOR blue = { 0.f, 0.f, 255.f };
	XMVECTOR scaleX = XMVectorScale(x, 1); scaleX = XMVectorAdd(scaleX, m_displayList[i].m_position);
	XMVECTOR scaleY = XMVectorScale(y, 1); scaleY = XMVectorAdd(scaleY, m_displayList[i].m_position);
	XMVECTOR scaleZ = XMVectorScale(z, 1); scaleZ = XMVectorAdd(scaleZ, m_displayList[i].m_position);
	
	// Setup context
	auto context = m_deviceResources->GetD3DDeviceContext();
	context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(m_states->DepthNone(), 0);
	context->RSSetState(m_states->CullNone());
	m_batchEffect->SetWorld(m_world);
	m_batchEffect->SetView(m_view);
	m_batchEffect->SetProjection(m_projection);

	m_batchEffect->Apply(context);

	context->IASetInputLayout(m_batchInputLayout.Get());

	m_batch->Begin();	

	// Bounding box
	for (int j = 0; j < m_displayList[i].m_model->meshes.size(); ++j)
	{ 
		Draw(m_batch.get(), m_displayList[i].m_model->meshes[j]->boundingBox, Colors::Orange);
	}		
	///Draw(m_batch.get(), object.m_model->meshes[0]->boundingSphere, Colors::Red);

	// X axis
	VertexPositionColor x1(XMVectorSubtract(scaleX, x), red);
	VertexPositionColor x2(XMVectorAdd(scaleX, x), red);
	m_batch->DrawLine(x1, x2);

	// Y axis
	VertexPositionColor y1(XMVectorSubtract(scaleY, y), green);
	VertexPositionColor y2(XMVectorAdd(scaleY, y), green);
	m_batch->DrawLine(y1, y2);

	// Z axis
	VertexPositionColor z1(XMVectorSubtract(scaleZ, z), blue);
	VertexPositionColor z2(XMVectorAdd(scaleZ, z), blue);
	m_batch->DrawLine(z1, z2);
	
	m_batch->End();

	m_deviceResources->PIXEndEvent();
}

std::vector<SceneObject> Game::DefineObjects(std::vector<SceneObject> sceneGraph)
{
	// Loop through scene graph
	for (int i = 0; i < sceneGraph.size(); ++i)
	{		
		// Object isn't water
		sceneGraph[i].m_isWater = false;

		// If object is house 1
		if (sceneGraph[i].model_path == "database/data/house0.cmo") { sceneGraph[i].m_type = OBJECT_TYPE::HOUSE_ONE; }

		// Else, if object is house 2
		else if (sceneGraph[i].model_path == "database/data/house1.cmo") { sceneGraph[i].m_type = OBJECT_TYPE::HOUSE_TWO; }

		// Else, if object is cave
		else if (sceneGraph[i].model_path == "database/data/cave.cmo") { sceneGraph[i].m_type = OBJECT_TYPE::CAVE; }

		// Else, if object is bridge
		else if (sceneGraph[i].model_path == "database/data/bridge.cmo") { sceneGraph[i].m_type = OBJECT_TYPE::BRIDGE; }

		// Else, if object is fence
		else if (sceneGraph[i].model_path == "database/data/fence.cmo") { sceneGraph[i].m_type = OBJECT_TYPE::FENCE; }

		// Else, if object is boat
		else if (sceneGraph[i].model_path == "database/data/boat.cmo") { sceneGraph[i].m_type = OBJECT_TYPE::BOAT; }

		// Else, if object is grass
		else if (sceneGraph[i].model_path == "database/data/grass.cmo") { sceneGraph[i].m_type = OBJECT_TYPE::GRASS; }

		// Else, if object is tree 1
		else if (sceneGraph[i].model_path == "database/data/tree0.cmo") { sceneGraph[i].m_type = OBJECT_TYPE::TREE_ONE; }

		// Else, if object is tree 2
		else if (sceneGraph[i].model_path == "database/data/tree1.cmo") { sceneGraph[i].m_type = OBJECT_TYPE::TREE_TWO; }

		// Else, if object is water
		else if (sceneGraph[i].model_path == "database/data/water.cmo") { sceneGraph[i].m_type = OBJECT_TYPE::WATER; sceneGraph[i].m_isWater = true; }

		// Else, if object is light
		else if (sceneGraph[i].model_path == "database/data/light.cmo") { sceneGraph[i].m_type = OBJECT_TYPE::LIGHT; }

		// Else, if object is cube
		else if (sceneGraph[i].model_path == "database/data/cube.cmo") { sceneGraph[i].m_type = OBJECT_TYPE::CUBE; }

		// Else, if object is cylinder
		else if (sceneGraph[i].model_path == "database/data/cylinder.cmo") { sceneGraph[i].m_type = OBJECT_TYPE::CYLINDER; }

		// Else, if object is cone
		else if (sceneGraph[i].model_path == "database/data/cone.cmo") { sceneGraph[i].m_type = OBJECT_TYPE::CONE; }
	}

	return sceneGraph;
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
	CreateDeviceDependentResources();
	
	// Update local scene graph
	m_sceneGraph = DefineObjects(*sceneGraph);
	
	auto device = m_deviceResources->GetD3DDevice();
	auto context = m_deviceResources->GetD3DDeviceContext();

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
		
		// Set if water or not
		newDisplayObject.m_isWater = m_sceneGraph[i].m_isWater;

		// Set object type
		newDisplayObject.m_objectType = m_sceneGraph[i].m_type;

		//load model
		std::wstring modelwstr = StringToWCHART(sceneGraph->at(i).model_path);							//convect string to Wchar
		newDisplayObject.m_model = Model::CreateFromCMO(device, modelwstr.c_str(), *m_fxFactory, true);	//get DXSDK to load model "False" for LH coordinate system (maya)

		//Load Texture
		std::wstring texturewstr = StringToWCHART(sceneGraph->at(i).tex_diffuse_path);								//convect string to Wchar
		HRESULT rs;
		rs = CreateDDSTextureFromFile(device, texturewstr.c_str(), nullptr, &newDisplayObject.m_texture_diffuse);	//load tex into Shader resource

		//if texture fails.  load error default
		if (rs) { CreateDDSTextureFromFile(device, L"database/data/Error.dds", nullptr, &newDisplayObject.m_texture_diffuse); }	//load tex into Shader resource

		//apply new texture to models effect
		//newDisplayObject.m_model->UpdateEffects([&](IEffect* effect) //This uses a Lambda function,  if you dont understand it: Look it up.
		//{
		//	auto lights = dynamic_cast<BasicEffect*>(effect);
		//	if (lights)
		//	{
		//		lights->SetTexture(newDisplayObject.m_texture_diffuse);
		//	}
		//});

		// set ID
		newDisplayObject.m_ID = sceneGraph->at(i).ID;

		//set position
		newDisplayObject.m_position.x = sceneGraph->at(i).posX;
		newDisplayObject.m_position.y = sceneGraph->at(i).posY;
		newDisplayObject.m_position.z = sceneGraph->at(i).posZ;

		//set orientation
		newDisplayObject.m_orientation.x = sceneGraph->at(i).rotX;
		newDisplayObject.m_orientation.y = sceneGraph->at(i).rotY;
		newDisplayObject.m_orientation.z = sceneGraph->at(i).rotZ;

		//set scale
		newDisplayObject.m_scale.x = sceneGraph->at(i).scaX;
		newDisplayObject.m_scale.y = sceneGraph->at(i).scaY;
		newDisplayObject.m_scale.z = sceneGraph->at(i).scaZ;

		//set wireframe / render flags
		newDisplayObject.m_render = sceneGraph->at(i).editor_render;
		newDisplayObject.m_wireframe = sceneGraph->at(i).editor_wireframe;

		//set lights
		newDisplayObject.m_light_type = sceneGraph->at(i).light_type;
		newDisplayObject.m_light_diffuse_r = sceneGraph->at(i).light_diffuse_r;
		newDisplayObject.m_light_diffuse_g = sceneGraph->at(i).light_diffuse_g;
		newDisplayObject.m_light_diffuse_b = sceneGraph->at(i).light_diffuse_b;
		newDisplayObject.m_light_ambient_r = sceneGraph->at(i).light_ambient_r;
		newDisplayObject.m_light_ambient_g = sceneGraph->at(i).light_ambient_g;
		newDisplayObject.m_light_ambient_b = sceneGraph->at(i).light_ambient_b;
		newDisplayObject.m_light_spot_cutoff = sceneGraph->at(i).light_spot_cutoff;
		newDisplayObject.m_light_constant = sceneGraph->at(i).light_constant;
		newDisplayObject.m_light_linear = sceneGraph->at(i).light_linear;
		newDisplayObject.m_light_quadratic = sceneGraph->at(i).light_quadratic;
		newDisplayObject.SetEnabled(sceneGraph->at(i).enabled);

		// Set bounding box		
		for (int j = 0; j < newDisplayObject.m_model->meshes.size(); ++j)
		{
			newDisplayObject.m_model->meshes[j]->boundingBox.Extents.x *= newDisplayObject.m_scale.x;
			newDisplayObject.m_model->meshes[j]->boundingBox.Extents.y *= newDisplayObject.m_scale.y;
			newDisplayObject.m_model->meshes[j]->boundingBox.Extents.z *= newDisplayObject.m_scale.z;
			newDisplayObject.m_model->meshes[j]->boundingBox.Center = newDisplayObject.m_position;
			if (newDisplayObject.m_objectType != OBJECT_TYPE::CUBE && newDisplayObject.m_objectType != OBJECT_TYPE::CYLINDER &&
				newDisplayObject.m_objectType != OBJECT_TYPE::CONE && newDisplayObject.m_objectType != OBJECT_TYPE::LIGHT &&
				newDisplayObject.m_objectType != OBJECT_TYPE::WATER) {
				newDisplayObject.m_model->meshes[0]->boundingBox.Center.y += newDisplayObject.m_model->meshes[0]->boundingBox.Extents.y;
			}
		}	
		
		// Add object to list
		m_displayList.push_back(newDisplayObject);	
	}	
}

void Game::RebuildDisplayList(std::vector<SceneObject>* sceneGraph)
{
	CreateDeviceDependentResources();

	// Update local scene graph
	m_sceneGraph = *sceneGraph;

	auto device = m_deviceResources->GetD3DDevice();
	auto context = m_deviceResources->GetD3DDeviceContext();

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

		// Check & set if water or not
		newDisplayObject.m_isWater = (sceneGraph->at(i).model_path == "database/data/water.cmo");

		// Set object type
		newDisplayObject.m_objectType = m_sceneGraph[i].m_type;

		//load model
		std::wstring modelwstr = StringToWCHART(sceneGraph->at(i).model_path);							//convect string to Wchar
		newDisplayObject.m_model = Model::CreateFromCMO(device, modelwstr.c_str(), *m_fxFactory, true);	//get DXSDK to load model "False" for LH coordinate system (maya)

		//Load Texture
		std::wstring texturewstr = StringToWCHART(sceneGraph->at(i).tex_diffuse_path);								//convect string to Wchar
		HRESULT rs;
		rs = CreateDDSTextureFromFile(device, texturewstr.c_str(), nullptr, &newDisplayObject.m_texture_diffuse);	//load tex into Shader resource

		//if texture fails.  load error default
		if (rs) { CreateDDSTextureFromFile(device, L"database/data/Error.dds", nullptr, &newDisplayObject.m_texture_diffuse); }	//load tex into Shader resource

		//apply new texture to models effect
		//newDisplayObject.m_model->UpdateEffects([&](IEffect* effect) //This uses a Lambda function,  if you dont understand it: Look it up.
		//{
		//	auto lights = dynamic_cast<BasicEffect*>(effect);
		//	if (lights)
		//	{
		//		lights->SetTexture(newDisplayObject.m_texture_diffuse);
		//	}
		//});

		// Set index
		newDisplayObject.m_ID = sceneGraph->at(i).ID;

		//set position
		newDisplayObject.m_position.x = sceneGraph->at(i).posX;
		newDisplayObject.m_position.y = sceneGraph->at(i).posY;
		newDisplayObject.m_position.z = sceneGraph->at(i).posZ;

		//set orientation
		newDisplayObject.m_orientation.x = sceneGraph->at(i).rotX;
		newDisplayObject.m_orientation.y = sceneGraph->at(i).rotY;
		newDisplayObject.m_orientation.z = sceneGraph->at(i).rotZ;

		//set scale
		newDisplayObject.m_scale.x = sceneGraph->at(i).scaX;
		newDisplayObject.m_scale.y = sceneGraph->at(i).scaY;
		newDisplayObject.m_scale.z = sceneGraph->at(i).scaZ;

		//set wireframe / render flags
		newDisplayObject.m_render = sceneGraph->at(i).editor_render;
		newDisplayObject.m_wireframe = sceneGraph->at(i).editor_wireframe;

		//set lights
		newDisplayObject.m_light_type = sceneGraph->at(i).light_type;
		newDisplayObject.m_light_diffuse_r = sceneGraph->at(i).light_diffuse_r;
		newDisplayObject.m_light_diffuse_g = sceneGraph->at(i).light_diffuse_g;
		newDisplayObject.m_light_diffuse_b = sceneGraph->at(i).light_diffuse_b;
		newDisplayObject.m_light_ambient_r = sceneGraph->at(i).light_ambient_r;
		newDisplayObject.m_light_ambient_g = sceneGraph->at(i).light_ambient_g;
		newDisplayObject.m_light_ambient_b = sceneGraph->at(i).light_ambient_b;
		newDisplayObject.m_light_spot_cutoff = sceneGraph->at(i).light_spot_cutoff;
		newDisplayObject.m_light_constant = sceneGraph->at(i).light_constant;
		newDisplayObject.m_light_linear = sceneGraph->at(i).light_linear;
		newDisplayObject.m_light_quadratic = sceneGraph->at(i).light_quadratic;
		newDisplayObject.SetEnabled(sceneGraph->at(i).enabled);

		// Set bounding box		
		for (int j = 0; j < newDisplayObject.m_model->meshes.size(); ++j)
		{
			newDisplayObject.m_model->meshes[j]->boundingBox.Extents.x *= newDisplayObject.m_scale.x;
			newDisplayObject.m_model->meshes[j]->boundingBox.Extents.y *= newDisplayObject.m_scale.y;
			newDisplayObject.m_model->meshes[j]->boundingBox.Extents.z *= newDisplayObject.m_scale.z;
			newDisplayObject.m_model->meshes[j]->boundingBox.Center = newDisplayObject.m_position;
			if (newDisplayObject.m_objectType != OBJECT_TYPE::CUBE && newDisplayObject.m_objectType != OBJECT_TYPE::CYLINDER &&
				newDisplayObject.m_objectType != OBJECT_TYPE::CONE && newDisplayObject.m_objectType != OBJECT_TYPE::LIGHT &&
				newDisplayObject.m_objectType != OBJECT_TYPE::WATER) {
				newDisplayObject.m_model->meshes[0]->boundingBox.Center.y += newDisplayObject.m_model->meshes[0]->boundingBox.Extents.y;
			}
		}		

		// Add object to list
		m_displayList.push_back(newDisplayObject);
	}
}

void Game::BuildDisplayChunk(ChunkObject * SceneChunk, std::vector<DirectX::SimpleMath::Vector2> location)
{	
	//populate our local DISPLAYCHUNK with all the chunk info we need from the object stored in toolmain
	//which, to be honest, is almost all of it. Its mostly rendering related info so...
	m_displayChunk.PopulateChunkData(SceneChunk);		//migrate chunk data
	m_displayChunk.LoadHeightMap(m_deviceResources);
	m_displayChunk.m_effect->SetProjection(m_projection);
	m_displayChunk.InitialiseBatch();
}

void Game::SaveDisplayChunk()
{
	m_displayChunk.GenerateHeightmap();
	m_displayChunk.SaveHeightMap();			//save heightmap to file.
}

void Game::SaveDisplayList()
{
	// Loop through display list
	for (int i = 0; i < m_displayList.size(); ++i)
	{
		// Update scene graph positions
		m_sceneGraph[i].posX = m_displayList[i].m_position.x;
		m_sceneGraph[i].posY = m_displayList[i].m_position.y;
		m_sceneGraph[i].posZ = m_displayList[i].m_position.z;

		// Update scene graph rotations
		m_sceneGraph[i].rotX = m_displayList[i].m_orientation.x;
		m_sceneGraph[i].rotY = m_displayList[i].m_orientation.y;
		m_sceneGraph[i].rotZ = m_displayList[i].m_orientation.z;

		// Update scene graph scales
		m_sceneGraph[i].scaX = m_displayList[i].m_scale.x;
		m_sceneGraph[i].scaY = m_displayList[i].m_scale.y;
		m_sceneGraph[i].scaZ = m_displayList[i].m_scale.z;
	}
}

std::vector<DisplayObject> Game::GetLights()
{
	// Setup temp container
	std::vector<DisplayObject> lights;

	// Loop through display list
	for (int i = 0; i < m_displayList.size(); ++i)
	{
		// If object is a light
		if (m_displayList[i].m_objectType == OBJECT_TYPE::LIGHT)
		{
			// Add to container
			lights.push_back(m_displayList[i]);
		}
	}

	// Return lights in scene
	return lights;
}

void Game::SetTransform(int index, OBJECT_FUNCTION function, DirectX::SimpleMath::Vector3 vector)
{
	// Switch between functions
	switch (function)
	{
	case OBJECT_FUNCTION::SCALE: 
	{
		// Update object scale
		m_displayList[index].m_scale = vector;		

		// Update object bounding box scale
		m_displayList[index].m_model->meshes[0]->boundingBox.Extents.x = (vector.x + vector.x);
		m_displayList[index].m_model->meshes[0]->boundingBox.Extents.y = (vector.y + vector.y);
		m_displayList[index].m_model->meshes[0]->boundingBox.Extents.z = (vector.z + vector.z);
	}
	break;
	case OBJECT_FUNCTION::ROTATE: 
	{		
		// If object is a light, normalise vector
		if (m_displayList[index].m_objectType == OBJECT_TYPE::LIGHT) { vector.Normalize(); }
		
		// Update object rotation
		m_displayList[index].m_orientation = vector;
	}
	break;
	case OBJECT_FUNCTION::TRANSLATE: 
	{
		// Update object translation
		m_displayList[index].m_position = vector;	
	}
	break;
	}

	// Update object bounding box translation
	m_displayList[index].m_model->meshes[0]->boundingBox.Center = m_displayList[index].m_position;
	/*if (m_displayList[index].m_objectType == OBJECT_TYPE::HOUSE_ONE || m_displayList[index].m_objectType == OBJECT_TYPE::HOUSE_TWO || 
		m_displayList[index].m_objectType == OBJECT_TYPE::TREE_ONE || m_displayList[index].m_objectType == OBJECT_TYPE::TREE_TWO) {
		m_displayList[index].m_model->meshes[0]->boundingBox.Center.y += m_displayList[index].m_model->meshes[0]->boundingBox.Extents.y;
	}*/
	if (m_displayList[index].m_objectType != OBJECT_TYPE::CUBE && m_displayList[index].m_objectType != OBJECT_TYPE::CYLINDER &&
		m_displayList[index].m_objectType != OBJECT_TYPE::CONE && m_displayList[index].m_objectType != OBJECT_TYPE::LIGHT &&
		m_displayList[index].m_objectType != OBJECT_TYPE::WATER) {
		m_displayList[index].m_model->meshes[0]->boundingBox.Center.y += m_displayList[index].m_model->meshes[0]->boundingBox.Extents.y;
	}

	SaveDisplayList();
}

void Game::SetFocus(int ID)
{
	// If ID is valid
	if (ID >= 0)
	{
		// Loop through display list
		for (int i = 0; i < m_displayList.size(); ++i)
		{
			// If display object matches focus ID
			if (m_displayList[i].m_ID == ID)
			{
				// Tell camera to focus
				m_camera->SetFocus(true);

				// Define object to focus on
				m_camera->SetFocusObject(m_displayList[i]);
			}
		}
	}
	
	// Else, if ID isn't valid
	else
	{
		// Tell camera to stop focussing
		m_camera->SetFocus(false);
	}	
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
	
	// Load and setup vertex/pixel shaders
	ShaderManager::Initialise(device);

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
