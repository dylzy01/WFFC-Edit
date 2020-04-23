#pragma once
#include "pch.h"
#include "DeviceResources.h"
#include "ChunkObject.h"
#include "TextureShader.h"

//geometric resoltuion - note,  hard coded.
#define TERRAINRESOLUTION 128

class DisplayChunk
{
public:
	DisplayChunk();
	~DisplayChunk();
	void PopulateChunkData(ChunkObject * SceneChunk);
	void RenderBatch(std::shared_ptr<DX::DeviceResources> deviceResources);
	void InitialiseBatch();	//initial setup, base coordinates etc based on scale
	void LoadHeightMap(std::shared_ptr<DX::DeviceResources>  DevResources);
	void SaveHeightMap();			//saves the heightmap back to file.
	void UpdateTerrain();			//updates the geometry based on the heightmap
	void GenerateHeightmap();		//creates or alters the heightmap
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormalTexture>>  m_batchBasic;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionDualTexture>>  m_batchBlend;
	std::unique_ptr<DirectX::BasicEffect>		m_effectBasic;
	std::unique_ptr<DirectX::DualTextureEffect>	m_effectBlend;

	ID3D11ShaderResourceView *					m_texture_default;				//diffuse texture
	ID3D11ShaderResourceView *					m_texture_splat_1;				//grass texture
	ID3D11ShaderResourceView *					m_texture_splat_2;				//dirt texture
	ID3D11ShaderResourceView *					m_texture_splat_3;				//sand texture
	ID3D11ShaderResourceView *					m_texture_splat_4;				//stone texture
	ID3D11ShaderResourceView *					m_texture_splat_5;				//snow texture

	ID3D11ShaderResourceView *					m_texture_blend_1;				//grass/dirt texture
	ID3D11ShaderResourceView *					m_texture_blend_2;				//grass/sand texture
	ID3D11ShaderResourceView *					m_texture_blend_3;				//grass/stone texture
	ID3D11ShaderResourceView *					m_texture_blend_4;				//grass/snow texture
	ID3D11ShaderResourceView *					m_texture_blend_5;				//dirt/sand texture
	ID3D11ShaderResourceView *					m_texture_blend_6;				//dirt/stone texture
	ID3D11ShaderResourceView *					m_texture_blend_7;				//dirt/snow texture
	ID3D11ShaderResourceView *					m_texture_blend_8;				//sand/stone texture
	ID3D11ShaderResourceView *					m_texture_blend_9;				//sand/snow texture
	ID3D11ShaderResourceView *					m_texture_blend_10;				//stone/snow texture

	ID3D11ShaderResourceView *					m_texture_highlight;			//highlight texture
	ID3D11ShaderResourceView *					m_normalMap;					//normal map texture
	Microsoft::WRL::ComPtr<ID3D11InputLayout>   m_terrainInputLayout;

	DirectX::VertexPositionNormalTexture GetGeometryBasic(int row, int column) { return m_terrainGeometryBasic[row][column]; }
	DirectX::VertexPositionDualTexture GetGeometryBlend(int row, int column) { return m_terrainGeometryBlend[row][column]; }
	void PaintTerrain(int row, int column, LANDSCAPE_PAINT paint, bool checkSurroundings = false);
	void SculptTerrain(int row, int column, LANDSCAPE_FUNCTION function, LANDSCAPE_CONSTRAINT constraint, std::vector<DirectX::SimpleMath::Vector3> position = { { 0,0,0 } });
	
	void SetSelected(bool selected, int row, int column);
	void DrawTerrainBasic(std::vector<std::pair<int, int>> terrain);
	void DrawTerrainBlend(std::vector<std::pair<int, int>> terrain);

	ChunkObject GetChunk();

private:
	DirectX::VertexPositionNormalTexture m_terrainGeometryBasic[TERRAINRESOLUTION][TERRAINRESOLUTION];
	DirectX::VertexPositionDualTexture m_terrainGeometryBlend[TERRAINRESOLUTION][TERRAINRESOLUTION];
	BYTE m_heightMap[TERRAINRESOLUTION*TERRAINRESOLUTION];
	void CalculateTerrainNormals();
	bool Search(int &index, std::vector<std::pair<int, int>> &vector, std::pair<int, int> terrain);
	bool FindInVector(int &index, std::vector<std::pair<int, int>> vector, std::pair<int, int> terrain);
	LANDSCAPE_PAINT CheckForDuplicates(int row, int column, LANDSCAPE_PAINT paint = LANDSCAPE_PAINT::NA);
	void CheckSurroundings(int row, int column, LANDSCAPE_PAINT paint);
	LANDSCAPE_PAINT CheckPaint(int row, int column);
	void SavePaint(std::string path, std::vector<std::pair<int, int>> vector);
	void ReadPaints(std::string path, std::vector<std::pair<int, int>> &vector);
	void ReadAllPaints();
	void SaveAllPaints();

	void DrawBlends(std::shared_ptr<DX::DeviceResources> deviceResources);

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