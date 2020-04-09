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
	void Wave(float deltaTime, DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);
	void PopulateChunkData(ChunkObject * SceneChunk);
	void RenderBatch(std::shared_ptr<DX::DeviceResources> deviceResources);
	void InitialiseBatch();	//initial setup, base coordinates etc based on scale
	void LoadHeightMap(std::shared_ptr<DX::DeviceResources>  DevResources);
	void SaveHeightMap();			//saves the heightmap back to file.
	void UpdateTerrain();			//updates the geometry based on the heightmap
	void GenerateHeightmap();		//creates or alters the heightmap
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormalTexture>>  m_batch;
	///std::unique_ptr<DirectX::CommonStates>		m_states;
	std::unique_ptr<DirectX::BasicEffect>		m_terrainEffect;
	std::unique_ptr<DirectX::DualTextureEffect>	m_dualEffect;
	/*std::unique_ptr<DirectX::NormalMapEffect>	m_normalMapEffect;
	std::unique_ptr<DirectX::BasicEffect>     m_terrainBlendOne;
	std::unique_ptr<DirectX::BasicEffect>       m_terrainBlendTwo;*/

	ID3D11ShaderResourceView *					m_texture_default;				//diffuse texture
	ID3D11ShaderResourceView *					m_texture_splat_1;				//grass texture
	ID3D11ShaderResourceView *					m_texture_splat_2;				//dirt texture
	ID3D11ShaderResourceView *					m_texture_splat_3;				//sand texture
	ID3D11ShaderResourceView *					m_texture_splat_4;				//stone texture
	ID3D11ShaderResourceView *					m_texture_splat_5;				//snow texture
	ID3D11ShaderResourceView *					m_texture_highlight;			//highlight texture
	ID3D11ShaderResourceView *					m_normalMap;					//normal map texture
	Microsoft::WRL::ComPtr<ID3D11InputLayout>   m_terrainInputLayout;
	///Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_normalMap;

	DirectX::VertexPositionNormalTexture GetGeometry(int row, int column) { return m_terrainGeometry[row][column]; }
	void PaintTerrain(int row, int column, LANDSCAPE_PAINT paint, bool checkSurroundings = false);
	void SculptTerrain(int row, int column, LANDSCAPE_FUNCTION function, LANDSCAPE_CONSTRAINT constraint, std::vector<DirectX::SimpleMath::Vector3> position = { { 0,0,0 } });
	
	void SetSelected(bool selected, int row, int column);
	void DrawTerrain(std::vector<std::pair<int, int>> terrain);

	ChunkObject GetChunk();

private:
	DirectX::VertexPositionNormalTexture m_terrainGeometry[TERRAINRESOLUTION][TERRAINRESOLUTION];
	BYTE m_heightMap[TERRAINRESOLUTION*TERRAINRESOLUTION];
	void CalculateTerrainNormals();
	bool FindInVector(int &index, std::vector<std::pair<int, int>> vector, std::pair<int, int> terrain);
	void CheckForDuplicates(int row, int column, LANDSCAPE_PAINT paint = LANDSCAPE_PAINT::NA);
	void CheckSurroundings(int row, int column, LANDSCAPE_PAINT paint);
	LANDSCAPE_PAINT CheckPaint(int row, int column);
	void SavePaints(std::string path, std::vector<std::pair<int, int>> vector);
	void ReadPaints(std::string path, std::vector<std::pair<int, int>> &vector);

	///Vector3 Mul(Vector3 vector, DirectX::SimpleMath::Matrix matrix);
	///Vector3 Mul(Vector3 vector, Matrix matrix);

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
	std::vector<std::pair<int, int>> m_grassTemp, m_dirtTemp, m_sandTemp, m_stoneTemp, m_snowTemp;
	std::vector<std::pair<int, int>> m_highlight;
	/*std::vector<DirectX::VertexPositionNormalTexture> m_defaultGeometry,
		m_grassGeometry, m_dirtGeometry, m_sandGeometry;
	std::vector<int> m_defaultIDs, m_grassIDs, m_dirtIDs, m_sandIDs;*/
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