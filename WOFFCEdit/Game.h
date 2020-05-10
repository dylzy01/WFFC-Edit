//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "SceneObject.h"
#include "DisplayObject.h"
#include "DisplayChunk.h"
#include "ChunkObject.h"
#include "InputCommands.h"
#include "Camera.h"
#include "DebugDraw.h"

#include "SQLManager.h"
#include "ShaderManager.h"

///#include "DXF.h"

#include <vector>
#include <algorithm>

#define PI 3.1415
#define THETA (PI * PI)

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game : public DX::IDeviceNotify
{
public:
	Game();
	~Game();

	// Initialization and management
	void Initialize(HWND window, int width, int height);
	void SetGridState(bool state);

	// Basic game loop
	void Tick(InputCommands * Input);
	void Render();

	// Rendering helpers
	void Clear();

	// IDeviceNotify
	virtual void OnDeviceLost() override;
	virtual void OnDeviceRestored() override;

	// Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowSizeChanged(int width, int height);

	// Tool specific
	void BuildDisplayList(std::vector<SceneObject> * sceneGraph); //note vector passed by reference 
	void RebuildDisplayList(std::vector<SceneObject> * sceneGraph); //note vector passed by reference 
	void BuildDisplayChunk(ChunkObject *sceneChunk, std::vector<DirectX::SimpleMath::Vector2> location = { { 0.f, 0.f } });
	void SaveDisplayChunk();	//saves geometry et al
	void ClearDisplayList();
	void SaveDisplayList();	
	void AutosaveOnOff(bool onOff) { m_autosave = onOff; }
	void DisplayAutosaveTime(bool display, int count) { m_autosaveTimer = display; m_autosaveCount = count; }

	// Getters
	std::shared_ptr<DX::DeviceResources> GetDeviceResources() { return m_deviceResources; }
	std::vector<DirectX::SimpleMath::Matrix> GetProjViewWorld() { return std::vector<DirectX::SimpleMath::Matrix>{m_projection, m_view, m_world}; }
	std::vector<SceneObject> GetSceneGraph() { return m_sceneGraph; }
	std::vector<DisplayObject> GetDisplayList() { return m_displayList; }
	DisplayChunk* GetDisplayChunk() { return &m_displayChunk; }	
	EDITOR_COMPLEX GetEditor() { return m_editor; }
	OBJECT_FUNCTION GetObjectFunction() { return m_objectFunction; }
	RECT GetScreenDimensions() { return m_screenDimensions; }
	std::vector<DisplayObject> GetLights();

	// Setters
	void SetWireframe(bool wireframe) { m_wireframe = wireframe; }
	void SetSelectedObjectIDs(std::vector<int> selectedObjectIDs) { m_selectedObjectIDs = selectedObjectIDs; }
	void SetDisplayList(DisplayObject object, int i) { m_displayList[i] = object; }
	void SetTransform(int i, OBJECT_FUNCTION function, DirectX::SimpleMath::Vector3 vector);
	void SetSceneObject(SceneObject object, int index) { m_sceneGraph[index] = object; RebuildDisplayList(&m_sceneGraph); }
	void SetFocus(int ID);

#ifdef DXTK_AUDIO
	void NewAudioDevice();
#endif

private:

	void Update(DX::StepTimer const& timer);
	void UpdateCamera();
	void UpdateWaves();

	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();

	void XM_CALLCONV DrawGrid(DirectX::FXMVECTOR xAxis, DirectX::FXMVECTOR yAxis, DirectX::FXMVECTOR origin, size_t xdivs, size_t ydivs, DirectX::GXMVECTOR color);
	void DrawDebug(int i);

	std::vector<SceneObject> DefineObjects(std::vector<SceneObject> sceneGraph);

	// frame time
	float								m_deltaTime;

	//tool specific
	std::vector<SceneObject>			m_sceneGraph;	
	std::vector<DisplayObject>			m_displayList;
	DisplayChunk						m_displayChunk;
	InputCommands						m_inputCommands;

	//functionality
	float								m_movespeed;

	//camera
	std::shared_ptr<Camera>				m_camera;

	//viewport
	RECT								m_screenDimensions;

	//control variables
	bool m_grid;							//grid rendering on / off
	bool m_wireframe = false;				//wireframe on/off
	bool m_toggleRight = false;				//object snap to mouse location on/off
	int m_fpsCount, m_fpsTrue;
	bool m_autosave = false;
	bool m_autosaveTimer = false;
	int m_autosaveCount = 0;

	//mouse picking
	std::vector<int> m_selectedObjectIDs;
	std::vector<int> m_selectedLandscapeIDs;
	TERRAIN m_selectedTerrain;
	std::vector<TERRAIN> m_selectedTerrains;
	DirectX::SimpleMath::Vector3 m_pickingPoint;
	float m_distance = 0.f;

	// Stored details of objects & terrain
	bool m_storePickingPoint, m_storeObjectDetails, m_storeTerrainPosition;
	///std::vector<DirectX::SimpleMath::Matrix> m_storedObjectTransforms, m_storedTerrainTransforms;
	std::vector<DirectX::SimpleMath::Vector3> m_storedObjectScales, m_storedObjectTranslations, m_storedObjectRotations;
	std::vector<DirectX::SimpleMath::Vector3> m_storedTerrainPositions;

	// Editor controllers
	EDITOR_COMPLEX m_editor;
	OBJECT_TYPE m_objectSpawn = OBJECT_TYPE::NA;
	OBJECT_FUNCTION m_objectFunction = OBJECT_FUNCTION::NA;
	CONSTRAINT m_objectConstraint = CONSTRAINT::NA;
	TERRAIN_PAINT m_landscapePaint = TERRAIN_PAINT::NA;
	TERRAIN_SCULPT m_landscapeSculpt = TERRAIN_SCULPT::NA;
	CONSTRAINT m_landscapeConstraint = CONSTRAINT::NA;

	// Water controllers
	std::vector<int> m_waterIDs;
	bool m_spawnWater = false;
	std::vector<DirectX::SimpleMath::Vector3> m_waterPositions;
	float m_waterTranslation = 0.f;
	float m_elapsedTime = 0.f;
	bool m_switch = true;

	// Lights
	//std::vector<DisplayObject> m_lights;

	// Device resources.
    std::shared_ptr<DX::DeviceResources>    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;

    // Input devices.
    std::unique_ptr<DirectX::GamePad>       m_gamePad;
    std::unique_ptr<DirectX::Keyboard>      m_keyboard;
    std::unique_ptr<DirectX::Mouse>         m_mouse;

    // DirectXTK objects.
    std::unique_ptr<DirectX::CommonStates>                                  m_states;
    std::unique_ptr<DirectX::BasicEffect>                                   m_batchEffect;
    std::unique_ptr<DirectX::EffectFactory>                                 m_fxFactory;
    std::unique_ptr<DirectX::GeometricPrimitive>                            m_shape;
	std::unique_ptr<DirectX::Model>                                         m_model;
    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>  m_batch;
	///std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormalTexture>>  m_batch;
    std::unique_ptr<DirectX::SpriteBatch>                                   m_sprites;
    std::unique_ptr<DirectX::SpriteFont>                                    m_font;

	// Window
	HWND m_window;

#ifdef DXTK_AUDIO
    std::unique_ptr<DirectX::AudioEngine>                                   m_audEngine;
    std::unique_ptr<DirectX::WaveBank>                                      m_waveBank;
    std::unique_ptr<DirectX::SoundEffect>                                   m_soundEffect;
    std::unique_ptr<DirectX::SoundEffectInstance>                           m_effect1;
    std::unique_ptr<DirectX::SoundEffectInstance>                           m_effect2;
#endif

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_texture1;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_texture2;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>                               m_batchInputLayout;

#ifdef DXTK_AUDIO
    uint32_t                                                                m_audioEvent;
    float                                                                   m_audioTimerAcc;

    bool                                                                    m_retryDefault;
#endif

    DirectX::SimpleMath::Matrix                                             m_world;
    DirectX::SimpleMath::Matrix                                             m_view;
    DirectX::SimpleMath::Matrix                                             m_projection;
};

std::wstring StringToWCHART(std::string s);