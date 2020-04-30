#pragma once
#include "pch.h"
#include "DeviceResources.h"
#include "ChunkObject.h"
#include "ShaderManager.h"
#include "Light.h"

//geometric resoltuion - note,  hard coded.
#define TERRAINRESOLUTION 128

class DisplayChunk
{
public:
	DisplayChunk();
	~DisplayChunk();
	void PopulateChunkData(ChunkObject * SceneChunk);
	void RenderBatch(std::shared_ptr<DX::DeviceResources> deviceResources, std::vector<Light*> lights);
	void InitialiseBatch();	//initial setup, base coordinates etc based on scale
	void LoadHeightMap(std::shared_ptr<DX::DeviceResources>  DevResources);
	void SaveHeightMap();			//saves the heightmap back to file.
	void UpdateTerrain();			//updates the geometry based on the heightmap
	void GenerateHeightmap();		//creates or alters the heightmap
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormalTexture>>  m_batch;
	std::unique_ptr<DirectX::BasicEffect>		m_effect;

	ID3D11ShaderResourceView *					m_texture_default;				//diffuse texture
	ID3D11ShaderResourceView *					m_texture_splat_1;				//grass texture
	ID3D11ShaderResourceView *					m_texture_splat_2;				//dirt texture
	ID3D11ShaderResourceView *					m_texture_splat_3;				//sand texture
	ID3D11ShaderResourceView *					m_texture_splat_4;				//stone texture
	ID3D11ShaderResourceView *					m_texture_splat_5;				//snow texture

	ID3D11ShaderResourceView *					m_texture_highlight;			//highlight texture
	ID3D11ShaderResourceView *					m_normalMap;					//normal map texture
	Microsoft::WRL::ComPtr<ID3D11InputLayout>   m_terrainInputLayout;

	DirectX::VertexPositionNormalTexture GetGeometry(int row, int column) { return m_terrainGeometry[row][column]; }
	LANDSCAPE_PAINT GetPaint(int row, int column);
	void PaintTerrain(int row, int column, LANDSCAPE_PAINT paint, bool checkSurroundings = false);
	void PaintOverBlended(LANDSCAPE_PAINT paint, std::pair<int, int> index);
	void SculptTerrain(int row, int column, LANDSCAPE_FUNCTION function, LANDSCAPE_CONSTRAINT constraint, std::vector<DirectX::SimpleMath::Vector3> position = { { 0,0,0 } });
	
	void SetSelected(bool selected, int row, int column);
	void DrawTerrain(std::vector<std::pair<int, int>> terrain);

	ChunkObject GetChunk();

private:
	DirectX::VertexPositionNormalTexture m_terrainGeometry[TERRAINRESOLUTION][TERRAINRESOLUTION];
	BYTE m_heightMap[TERRAINRESOLUTION*TERRAINRESOLUTION];
	void CalculateTerrainNormals();	
	bool FindInVector(int &index, std::vector<std::pair<int, int>> vector, std::pair<int, int> terrain);
	bool SearchAndDestroy(int &index, std::vector<std::pair<int, int>> &vector, std::pair<int, int> terrain);
	bool SearchForPaint(std::vector<std::pair<int, int>> vector, std::pair<int, int> terrain);
	LANDSCAPE_PAINT CheckForDuplicates(int row, int column, LANDSCAPE_PAINT paint = LANDSCAPE_PAINT::NA);
	void CheckSurroundings(int row, int column, LANDSCAPE_PAINT paint);
	LANDSCAPE_PAINT CheckPaint(int row, int column);
	void SavePaint(std::string path, std::vector<std::pair<int, int>> vector);
	void ReadPaints(std::string path, std::vector<std::pair<int, int>> &vector);
	void ReadAllPaints();
	void SaveAllPaints();

	void DrawBlends(std::shared_ptr<DX::DeviceResources> deviceResources, std::vector<Light*> lights);

	float	m_terrainHeightScale;
	int		m_terrainSize;				//size of terrain in metres
	float	m_textureCoordStep;			//step in texture coordinates between each vertex row / column
	float   m_terrainPositionScalingFactor;	//factor we multiply the position by to convert it from its native resolution( 0- Terrain Resolution) to full scale size in metres dictated by m_Terrainsize
		
	std::string m_name;
	int m_chunk_x_size_metres;
	int m_chunk_y_size_metres;
	int m_chunk_base_resolution;
	std::string m_heightmap_path;
	std::string m_tex_diffuse_path;
	std::string m_tex_splat_alpha_path;
	std::string m_tex_splat_1_path;
	std::string m_tex_splat_2_path;
	std::string m_tex_splat_3_path;
	std::string m_tex_splat_4_path;
	bool m_render_wireframe;
	bool m_render_normals;
	int m_tex_diffuse_tiling;
	int m_tex_splat_1_tiling;
	int m_tex_splat_2_tiling;
	int m_tex_splat_3_tiling;
	int m_tex_splat_4_tiling;	

	// Geometry containers
	std::vector<std::pair<int, int>> m_default;
	std::vector<std::pair<int, int>> m_grass, m_dirt, m_sand, m_stone, m_snow;

	std::vector<std::pair<int, int>> m_grassDirt, m_grassSand, m_grassStone, m_grassSnow;
	std::vector<std::pair<int, int>> m_dirtSand, m_dirtStone, m_dirtSnow;
	std::vector<std::pair<int, int>> m_sandStone, m_sandSnow;
	std::vector<std::pair<int, int>> m_stoneSnow;

	std::vector<std::pair<int, int>> m_grassTemp, m_dirtTemp, m_sandTemp, m_stoneTemp, m_snowTemp;
	std::vector<std::pair<int, int>> m_highlight;

	// Shaders
	///TextureShader m_shader;
};