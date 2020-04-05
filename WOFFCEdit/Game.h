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
#include "SQL.h"
#include "Water.h"

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
	void BuildDisplayList(std::vector<SceneObject> * SceneGraph); //note vector passed by reference 
	void BuildDisplayChunk(ChunkObject *SceneChunk, std::vector<DirectX::SimpleMath::Vector2> location = { { 0.f, 0.f } });
	void SaveDisplayChunk(ChunkObject *SceneChunk);	//saves geometry et al
	void ClearDisplayList();
	void DeleteSelectedObjects();

	// Mouse picking
	void MousePicking(int i = -1);
	bool PickingObjects(bool select);
	bool ObjectIntersection(int i);
	TERRAIN PickingTerrain();
	std::vector<TERRAIN> PickingTerrains();
	TERRAIN TerrainIntersection(DirectX::SimpleMath::Ray ray);	

	// Getters
	EDITOR GetEditor() { return m_editor; }
	std::vector<int> GetSelectedObjectIDs() { return m_selectedObjectIDs; }
	DirectX::SimpleMath::Vector3 GetDragPoint(DirectX::SimpleMath::Vector3* dragLine, DirectX::SimpleMath::Vector3* unProjLine);

	// Setters
	void SetWireframe(bool wireframe) { m_wireframe = wireframe; }
	void SetEditor(EDITOR editor) { 
		m_editor = editor; 
		switch (editor)
		{
		case EDITOR::OBJECT_TRANSFORM:
		{
			m_landscapeSculpt = LANDSCAPE_SCULPT::NA;
			m_landscapeConstraint = LANDSCAPE_CONSTRAINT::NA;
		}
		break;
		case EDITOR::LANDSCAPE_SCULPT:
		{
			m_objectTransform = OBJECT_FUNCTION::NA;
			m_objectConstraint = OBJECT_CONSTRAINT::NA;
		}
		break;
		}
	}
	void SetObjectSpawn(OBJECT_SPAWN spawn) { m_objectSpawn = spawn; }
	void SetResidentialSpawn(RESIDENTIAL_SPAWN spawn) { m_residentialSpawn = spawn; }
	void SetNatureSpawn(NATURE_SPAWN spawn) { m_natureSpawn = spawn; }
	void SetObjectTransform(OBJECT_FUNCTION function) { m_objectTransform = function; }
	void SetObjectConstraint(OBJECT_CONSTRAINT constraint) { m_objectConstraint = constraint; }
	void SetLandscapePaint(LANDSCAPE_PAINT paint) { m_landscapePaint = paint; }
	void SetLandscapeSculpt(LANDSCAPE_SCULPT function) { m_landscapeSculpt = function; }
	void SetLandscapeConstraint(LANDSCAPE_CONSTRAINT constraint) { m_landscapeConstraint = constraint; }
	void StorePickingPoint(bool store) { m_storePickingPoint = store; }
	void StoreObjectDetails(bool store) { m_storeObjectDetails = store; }
	void StoreTerrainPosition(bool store) { m_storeTerrainPosition = store; }

	// Creation functions
	SceneObject CreateObject(OBJECT_SPAWN spawn, DirectX::SimpleMath::Vector3 position);
	Water CreateWater(DirectX::SimpleMath::Vector3 position);

#ifdef DXTK_AUDIO
	void NewAudioDevice();
#endif

private:
	void HandleInput();

	void Update(DX::StepTimer const& timer);
	void UpdateCamera();
	void UpdateWaves();
	float Pulse();
	float GetNewHeight(float OGheight);

	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();

	void XM_CALLCONV DrawGrid(DirectX::FXMVECTOR xAxis, DirectX::FXMVECTOR yAxis, DirectX::FXMVECTOR origin, size_t xdivs, size_t ydivs, DirectX::GXMVECTOR color);
	void DrawAxis(DisplayObject object, int ID);

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

	//mouse picking
	std::vector<int> m_selectedObjectIDs;
	std::vector<int> m_selectedLandscapeIDs;
	TERRAIN m_selectedTerrain;
	std::vector<TERRAIN> m_selectedTerrains;
	DirectX::SimpleMath::Vector3 m_pickingPoint;

	// Stored terrain position for flattening
	bool m_storePickingPoint, m_storeObjectDetails, m_storeTerrainPosition;
	///std::vector<DirectX::SimpleMath::Matrix> m_storedObjectTransforms, m_storedTerrainTransforms;
	std::vector<DirectX::SimpleMath::Vector3> m_storedObjectScales, m_storedObjectTranslations, m_storedObjectRotations;
	DirectX::SimpleMath::Vector2 m_storedMousePosition;
	DirectX::SimpleMath::Vector3 m_storedPickingPoint;
	std::vector<DirectX::SimpleMath::Vector3> m_storedTerrainPositions;

	// Editor controllers
	EDITOR m_editor;
	OBJECT_SPAWN m_objectSpawn = OBJECT_SPAWN::NA;
	RESIDENTIAL_SPAWN m_residentialSpawn = RESIDENTIAL_SPAWN::NA;
	NATURE_SPAWN m_natureSpawn = NATURE_SPAWN::NA;
	OBJECT_FUNCTION m_objectTransform = OBJECT_FUNCTION::NA;
	OBJECT_CONSTRAINT m_objectConstraint = OBJECT_CONSTRAINT::NA;
	LANDSCAPE_PAINT m_landscapePaint = LANDSCAPE_PAINT::NA;
	LANDSCAPE_SCULPT m_landscapeSculpt = LANDSCAPE_SCULPT::NA;
	LANDSCAPE_CONSTRAINT m_landscapeConstraint = LANDSCAPE_CONSTRAINT::NA;

	// Water controllers
	std::vector<int> m_waterIDs;
	bool m_spawnWater = false;
	std::vector<DirectX::SimpleMath::Vector3> m_waterPositions;
	float m_waterTranslation = 0.f;
	float m_elapsedTime = 0.f;
	bool m_switch = true;

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
    std::vector<std::unique_ptr<DirectX::GeometricPrimitive>>               m_shapes;
	std::unique_ptr<DirectX::Model>                                         m_model;
    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>  m_batch;
    std::unique_ptr<DirectX::SpriteBatch>                                   m_sprites;
    std::unique_ptr<DirectX::SpriteFont>                                    m_font;

	// Axes
	///std::vector<Ray> m_xRays, m_yRays, m_zRays;
	std::vector<DirectX::SimpleMath::Vector3> m_xAxes, m_yAxes, m_zAxes;

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