#pragma once
#include "pch.h"
#include "DeviceResources.h"
#include "ChunkObject.h"

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
	void PaintTerrain(int row, int column, LANDSCAPE_PAINT paint);
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormalTexture>>  m_batch;
	std::unique_ptr<DirectX::BasicEffect>       m_terrainEffect;

	ID3D11ShaderResourceView *					m_texture_default;				//diffuse texture
	ID3D11ShaderResourceView *					m_texture_splat_1;				//diffuse texture
	ID3D11ShaderResourceView *					m_texture_splat_2;				//diffuse texture
	ID3D11ShaderResourceView *					m_texture_splat_3;				//diffuse texture
	Microsoft::WRL::ComPtr<ID3D11InputLayout>   m_terrainInputLayout;

	DirectX::VertexPositionNormalTexture GetGeometry(int row, int column) { return m_terrainGeometry[row][column]; }
	void SculptTerrain(int row, int column, LANDSCAPE_SCULPT function, LANDSCAPE_CONSTRAINT constraint, std::vector<DirectX::SimpleMath::Vector3> position = { { 0,0,0 } });

	void SetSelected(bool selected, int ID);
	void DrawTerrain(std::vector<std::pair<DirectX::VertexPositionNormalTexture, int>> terrain);

private:
	DirectX::VertexPositionNormalTexture m_terrainGeometry[TERRAINRESOLUTION][TERRAINRESOLUTION];
	BYTE m_heightMap[TERRAINRESOLUTION*TERRAINRESOLUTION];
	void CalculateTerrainNormals();
	void CheckTerrain(DirectX::VertexPositionNormalTexture terrain, LANDSCAPE_PAINT paint);
	void CheckForDuplicates(std::pair< DirectX::VertexPositionNormalTexture, int> terrain, LANDSCAPE_PAINT paint);
	bool FindInVector(int &index, std::vector<std::pair<DirectX::VertexPositionNormalTexture, int>> vector, std::pair< DirectX::VertexPositionNormalTexture, int> terrain);
	
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

	LANDSCAPE_PAINT m_paint[TERRAINRESOLUTION][TERRAINRESOLUTION];
	///LANDSCAPE_PAINT m_paint[TERRAINRESOLUTION*TERRAINRESOLUTION];

	// Geometry containers
	bool m_addOnce = true;
	std::vector<std::pair<DirectX::VertexPositionNormalTexture, int>> m_default;
	std::vector<std::pair<DirectX::VertexPositionNormalTexture, int>> m_grass;
	std::vector<std::pair<DirectX::VertexPositionNormalTexture, int>> m_dirt;
	std::vector<std::pair<DirectX::VertexPositionNormalTexture, int>> m_sand;
};

//template <typename T>
//int FindInVector(const std::vector<std::pair<T, int>> & vector, const std::pair<T, int> &element)
//{
//	// Temp
//	int index = -1;
//	
//	// Loop through vector
//	for (int i = 0; i < vector.size(); ++i)
//	{
//		if (vector[i].second == element.second)
//		{
//			index = i;
//		}
//	}
//
//	// Find given element in vector
//	auto it = std::find(vector.begin(), vector.end(), element);
//
//	// If element has been found
//	if (it != vector.end()) { return std::distance(vector.begin(), it); }
//		
//	// Else, if element hasn't been found
//	else { return -1; }
//}