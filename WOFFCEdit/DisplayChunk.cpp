#include <string>
#include "DisplayChunk.h"
#include "Game.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

DisplayChunk::DisplayChunk()
{
	//terrain size in meters. note that this is hard coded here, we COULD get it from the terrain chunk along with the other info from the tool if we want to be more flexible.
	m_terrainSize = 512;
	m_terrainHeightScale = 0.25;  //convert our 0-256 terrain to 64
	m_textureCoordStep = 1.0 / (TERRAINRESOLUTION-1);	//-1 becuase its split into chunks. not vertices.  we want the last one in each row to have tex coord 1
	m_terrainPositionScalingFactor = m_terrainSize / (TERRAINRESOLUTION-1);
}

DisplayChunk::~DisplayChunk()
{
}

void DisplayChunk::PopulateChunkData(ChunkObject * SceneChunk)
{
	m_name = SceneChunk->name;
	m_chunk_x_size_metres = SceneChunk->chunk_x_size_metres;
	m_chunk_y_size_metres = SceneChunk->chunk_y_size_metres;
	m_chunk_base_resolution = SceneChunk->chunk_base_resolution;
	m_heightmap_path = SceneChunk->heightmap_path;
	m_tex_diffuse_path = SceneChunk->tex_diffuse_path;
	m_tex_splat_alpha_path = SceneChunk->tex_splat_alpha_path;
	m_tex_splat_1_path = SceneChunk->tex_splat_1_path;
	m_tex_splat_2_path = SceneChunk->tex_splat_2_path;
	m_tex_splat_3_path = SceneChunk->tex_splat_3_path;
	m_tex_splat_4_path = SceneChunk->tex_splat_4_path;
	m_render_wireframe = SceneChunk->render_wireframe;
	m_render_normals = SceneChunk->render_normals;
	m_tex_diffuse_tiling = SceneChunk->tex_diffuse_tiling;
	m_tex_splat_1_tiling = SceneChunk->tex_splat_1_tiling;
	m_tex_splat_2_tiling = SceneChunk->tex_splat_2_tiling;
	m_tex_splat_3_tiling = SceneChunk->tex_splat_3_tiling;
	m_tex_splat_4_tiling = SceneChunk->tex_splat_4_tiling;
}

void DisplayChunk::RenderBatch(std::shared_ptr<DX::DeviceResources> deviceResources, std::vector<Light*> lights)
{	
	// Setup device context
	auto context = deviceResources->GetD3DDeviceContext();
	///context->IASetInputLayout(m_terrainInputLayout.Get());

	XMFLOAT2 size;
	size.x = deviceResources->GetOutputSize().right;
	size.y = deviceResources->GetOutputSize().bottom;

	m_effect->Apply(context);

	// Draw all grass geometry
	if (m_grass.size() != 0) {
		m_batch->Begin();
		///ShaderManager::Shader(SHADER_TYPE::TEXTURE, context, lights, m_texture_splat_1);
		ShaderManager::Shader(SHADER_TYPE::TOON_SINGLE, context, lights, m_texture_splat_1);
		///ShaderManager::Shader(SHADER_TYPE::OUTLINE, context, m_texture_splat_1, NULL, size);
		DrawTerrain(m_grass);
		m_batch->End();
	}

	// Draw all dirt geometry
	if (m_dirt.size() != 0) {
		m_batch->Begin();
		///ShaderManager::Shader(SHADER_TYPE::TEXTURE, context, lights, m_texture_splat_2);
		ShaderManager::Shader(SHADER_TYPE::TOON_SINGLE, context, lights, m_texture_splat_2);
		DrawTerrain(m_dirt);
		m_batch->End();
	}

	// Draw all sand geometry
	if (m_sand.size() != 0)	{
		m_batch->Begin();
		///ShaderManager::Shader(SHADER_TYPE::TEXTURE, context, lights, m_texture_splat_3);
		ShaderManager::Shader(SHADER_TYPE::TOON_SINGLE, context, lights, m_texture_splat_3);
		DrawTerrain(m_sand);
		m_batch->End();
	}

	// Draw all stone geometry
	if (m_stone.size() != 0) {
		m_batch->Begin();
		///ShaderManager::Shader(SHADER_TYPE::TEXTURE, context, lights, m_texture_splat_4);
		ShaderManager::Shader(SHADER_TYPE::TOON_SINGLE, context, lights, m_texture_splat_4);
		DrawTerrain(m_stone);
		m_batch->End();
	}

	// Draw all snow geometry
	if (m_snow.size() != 0) {
		m_batch->Begin();
		///ShaderManager::Shader(SHADER_TYPE::TEXTURE, context, lights, m_texture_splat_5);
		ShaderManager::Shader(SHADER_TYPE::TOON_SINGLE, context, lights, m_texture_splat_5);
		DrawTerrain(m_snow);
		m_batch->End();
	}	
	
	// Draw all highlighted geometry
	if (m_highlight.size() != 0) {
		m_batch->Begin();
		DrawTerrain(m_highlight);
		m_batch->End();
	}

	// Draw all default geometry
	if (m_default.size() != 0) {
		m_batch->Begin();
		DrawTerrain(m_default);	
		m_batch->End();
	}

	// Draw all blended geometry
	DrawBlends(deviceResources, lights);
}

void DisplayChunk::InitialiseBatch()
{
	//build geometry for our terrain array
	//iterate through all the vertices of our required resolution terrain.
	int index = 0;

	for (size_t i = 0; i < TERRAINRESOLUTION; i++)
	{
		for (size_t j = 0; j < TERRAINRESOLUTION; j++)
		{
			index = (TERRAINRESOLUTION * i) + j;
			m_terrainGeometry[i][j].position =			Vector3(j*m_terrainPositionScalingFactor-(0.5*m_terrainSize), (float)(m_heightMap[index])*m_terrainHeightScale, i*m_terrainPositionScalingFactor-(0.5*m_terrainSize));	//This will create a terrain going from -64->64.  rather than 0->128.  So the center of the terrain is on the origin
			m_terrainGeometry[i][j].normal =			Vector3(0.0f, 1.0f, 0.0f);						//standard y =up
			m_terrainGeometry[i][j].textureCoordinate =	Vector2(((float)m_textureCoordStep*j)*m_tex_diffuse_tiling, ((float)m_textureCoordStep*i)*m_tex_diffuse_tiling);				//Spread tex coords so that its distributed evenly across the terrain from 0-1	
		}
	}
	CalculateTerrainNormals();
}

void DisplayChunk::LoadHeightMap(std::shared_ptr<DX::DeviceResources>  DevResources)
{	
	auto device = DevResources->GetD3DDevice();
	auto deviceContext = DevResources->GetD3DDeviceContext();

	//load in heightmap .raw
	FILE *pFile = NULL;

	// Open The File In Read / Binary Mode.

	pFile = fopen(m_heightmap_path.c_str(), "rb");
	// Check To See If We Found The File And Could Open It
	if (pFile == NULL)
	{
		// Display Error Message And Stop The Function
		MessageBox(NULL, L"Can't Find The Height Map!", L"Error", MB_OK);
		return;
	}

	// Here We Load The .RAW File Into Our pHeightMap Data Array
	// We Are Only Reading In '1', And The Size Is (Width * Height)
	fread(m_heightMap, 1, TERRAINRESOLUTION*TERRAINRESOLUTION, pFile);

	fclose(pFile);
		
	//////////////////////////////////////////////////////////////////////////////////////////

	// Load first splat texture (grass)
	std::wstring splat_1 = StringToWCHART(m_tex_splat_1_path);
	HRESULT rs1;
	rs1 = CreateDDSTextureFromFile(device, splat_1.c_str(), NULL, &m_texture_splat_1);

	//////////////////////////////////////////////////////////////////////////////////////////

	// Load second splat texture (dirt)
	std::wstring splat_2 = StringToWCHART(m_tex_splat_2_path);
	HRESULT rs2;
	rs2 = CreateDDSTextureFromFile(device, splat_2.c_str(), NULL, &m_texture_splat_2);

	//////////////////////////////////////////////////////////////////////////////////////////

	// Load third splat texture (sand)
	std::wstring splat_3 = StringToWCHART(m_tex_splat_3_path);
	HRESULT rs3;
	rs3 = CreateDDSTextureFromFile(device, splat_3.c_str(), NULL, &m_texture_splat_3);

	//////////////////////////////////////////////////////////////////////////////////////////

	// Load fourth splat texture (stone)
	std::wstring splat_4 = StringToWCHART(m_tex_splat_4_path);
	HRESULT rs4;
	rs4 = CreateDDSTextureFromFile(device, splat_4.c_str(), NULL, &m_texture_splat_4);
	
	//////////////////////////////////////////////////////////////////////////////////////////

	// Load fifth splat texture (snow)
	std::wstring splat_5 = StringToWCHART("database/data/snow.dds");
	HRESULT rs5;
	rs5 = CreateDDSTextureFromFile(device, splat_5.c_str(), NULL, &m_texture_splat_5);

	//////////////////////////////////////////////////////////////////////////////////////////

	// Load highlight texture
	std::wstring highlight = StringToWCHART("database/data/highlight.dds");
	HRESULT rsH;
	rsH = CreateDDSTextureFromFile(device, highlight.c_str(), NULL, &m_texture_highlight);

	//////////////////////////////////////////////////////////////////////////////////////////

	//load the default texture
	std::wstring diffuse = StringToWCHART(m_tex_diffuse_path);
	HRESULT rs0;
	rs0 = CreateDDSTextureFromFile(device, diffuse.c_str(), NULL, &m_texture_default);	//load tex into Shader resource	view and resource
																							
	//////////////////////////////////////////////////////////////////////////////////////////

	// Load normal map
	/*std::wstring normal = StringToWCHART("database/data/normalMap.dds");
	HRESULT rsn;
	rsn = CreateDDSTextureFromFile(device, normal.c_str(), NULL, &m_normalMap);*/

	//////////////////////////////////////////////////////////////////////////////////////////

	// Setup normal map effect
	///m_normalMapEffect = std::make_unique<NormalMapEffect>(device);
	///m_normalMapEffect->SetTexture(m_normalMap.Get());

	//////////////////////////////////////////////////////////////////////////////////////////

	// Setup state
	///m_states = std::make_unique<CommonStates>(device);

	//////////////////////////////////////////////////////////////////////////////////////////

	// Setup basic terrain effect
	m_effect = std::make_unique<BasicEffect>(device);
	m_effect->EnableDefaultLighting();
	m_effect->SetLightingEnabled(true);
	///m_effect->SetTextureEnabled(true);
	///m_effect->SetTexture(m_texture_default);

	void const* shaderByteCodeBasic;
	size_t byteCodeLengthBasic;

	m_effect->GetVertexShaderBytecode(&shaderByteCodeBasic, &byteCodeLengthBasic);

	//setup basic batch
	DX::ThrowIfFailed(
		device->CreateInputLayout(VertexPositionNormalTexture::InputElements,
			VertexPositionNormalTexture::InputElementCount,
			shaderByteCodeBasic,
			byteCodeLengthBasic,
			m_terrainInputLayout.GetAddressOf())
		);

	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormalTexture>>(deviceContext);	

	//////////////////////////////////////////////////////////////////////////////////////////

	// Setup blend terrain effect
	//m_effectBlend = std::make_unique<DualTextureEffect>(device);
	//m_effectBlend->SetTexture(m_texture_splat_1);
	//m_effectBlend->SetTexture2(m_texture_splat_2);

	//// setup blend batch
	//void const* shaderByteCodeBlend;
	//size_t byteCodeLengthBlend;

	//m_effectBlend->GetVertexShaderBytecode(&shaderByteCodeBlend, &byteCodeLengthBlend);

	//// setup blend batch
	//DX::ThrowIfFailed(
	//	device->CreateInputLayout(VertexPositionDualTexture::InputElements,
	//		VertexPositionDualTexture::InputElementCount,
	//		shaderByteCodeBlend,
	//		byteCodeLengthBlend,
	//		m_terrainInputLayout.GetAddressOf())
	//);

	//m_batchBlend = std::make_unique<PrimitiveBatch<VertexPositionDualTexture>>(deviceContext);

	//////////////////////////////////////////////////////////////////////////////////////////

	// Load all paints
	ReadAllPaints();
}

void DisplayChunk::SaveHeightMap()
{
/*	for (size_t i = 0; i < TERRAINRESOLUTION*TERRAINRESOLUTION; i++)
	{
		m_heightMap[i] = 0;
	}*/

	// Height map
	{
		FILE *pFile = NULL;

		// Open The File In Read / Binary Mode.
		pFile = fopen(m_heightmap_path.c_str(), "wb+");;
		// Check To See If We Found The File And Could Open It
		if (pFile == NULL)
		{
			// Display Error Message And Stop The Function
			MessageBox(NULL, L"Can't Find The Height Map!", L"Error", MB_OK);
			return;
		}

		fwrite(m_heightMap, 1, TERRAINRESOLUTION*TERRAINRESOLUTION, pFile);
		fclose(pFile);
	}

	// Paints
	SaveAllPaints();
}

void DisplayChunk::UpdateTerrain()
{
	//all this is doing is transferring the height from the heightmap into the terrain geometry.
	int index;
	for (size_t i = 0; i < TERRAINRESOLUTION; i++)
	{
		for (size_t j = 0; j < TERRAINRESOLUTION; j++)
		{
			index = (TERRAINRESOLUTION * i) + j;
			m_terrainGeometry[i][j].position.y = (float)(m_heightMap[index])*m_terrainHeightScale;	
		}
	}
	CalculateTerrainNormals();
}

void DisplayChunk::GenerateHeightmap()
{
	// Updating height map to match manipulated geometry
	int index;
	for (size_t i = 0; i < TERRAINRESOLUTION; ++i)
	{
		for (size_t j = 0; j < TERRAINRESOLUTION; ++j)
		{
			index = (TERRAINRESOLUTION * i) + j;
			m_heightMap[index] = m_terrainGeometry[i][j].position.y / m_terrainHeightScale;
		}
	}
}

LANDSCAPE_PAINT DisplayChunk::GetPaint(int row, int column)
{
	// Setup temp terrain
	std::pair<int, int> terrain;
	terrain.first = row;
	terrain.second = column;
	
	// Setup temp empty paint
	LANDSCAPE_PAINT paint = LANDSCAPE_PAINT::NA;
	
	// Search through grass storage
	if (SearchForPaint(m_grass, terrain)) { paint = LANDSCAPE_PAINT::GRASS; }

	// Else, search through dirt storage
	else if (SearchForPaint(m_dirt, terrain)) { paint = LANDSCAPE_PAINT::DIRT; }

	// Else, search through sand storage
	else if (SearchForPaint(m_sand, terrain)) { paint = LANDSCAPE_PAINT::SAND; }

	// Else, search through stone storage
	else if (SearchForPaint(m_stone, terrain)) { paint = LANDSCAPE_PAINT::STONE; }

	// Else, search through snow storage
	else if (SearchForPaint(m_snow, terrain)) { paint = LANDSCAPE_PAINT::SNOW; }

	// Else, search through grass/dirt storage
	else if (SearchForPaint(m_grassDirt, terrain)) { paint = LANDSCAPE_PAINT::GRASS_DIRT; }

	// Else, search through grass/sand storage
	else if (SearchForPaint(m_grassSand, terrain)) { paint = LANDSCAPE_PAINT::GRASS_SAND; }

	// Else, search through grass/stone storage
	else if (SearchForPaint(m_grassStone, terrain)) { paint = LANDSCAPE_PAINT::GRASS_STONE; }

	// Else, search through grass/snow storage
	else if (SearchForPaint(m_grassSnow, terrain)) { paint = LANDSCAPE_PAINT::GRASS_SNOW; }

	// Else, search through dirt/sand storage
	else if (SearchForPaint(m_dirtSand, terrain)) { paint = LANDSCAPE_PAINT::DIRT_SAND; }

	// Else, search through dirt/stone storage
	else if (SearchForPaint(m_dirtStone, terrain)) { paint = LANDSCAPE_PAINT::DIRT_STONE; }

	// Else, search through dirt/snow storage
	else if (SearchForPaint(m_dirtSnow, terrain)) { paint = LANDSCAPE_PAINT::DIRT_SNOW; }

	// Else, search through sand/stone storage
	else if (SearchForPaint(m_sandStone, terrain)) { paint = LANDSCAPE_PAINT::SAND_STONE; }

	// Else, search through sand/snow storage
	else if (SearchForPaint(m_sandSnow, terrain)) { paint = LANDSCAPE_PAINT::SAND_SNOW; }

	// Else, search through stone/snow storage
	else if (SearchForPaint(m_stoneSnow, terrain)) { paint = LANDSCAPE_PAINT::STONE_SNOW; }

	return paint;
}

void DisplayChunk::PaintTerrain(int i, int j, LANDSCAPE_PAINT paint, bool checkSurroundings)
{	
	// Store row & column
	std::pair<int, int> index(i, j);

	// Check for duplicates and return current paint
	LANDSCAPE_PAINT current = CheckForDuplicates(i, j, paint);

	// If current paint is grass
	if (current == LANDSCAPE_PAINT::GRASS)
	{
		// If selected paint is grass, do nothing
		if (paint == LANDSCAPE_PAINT::GRASS) {}

		// Else, apply selected paint
		else
		{
			// Switch between selected paint
			switch (paint)
			{
			case LANDSCAPE_PAINT::DIRT:		m_grassDirt.push_back(index); break;
			case LANDSCAPE_PAINT::SAND:		m_grassSand.push_back(index); break;
			case LANDSCAPE_PAINT::STONE:	m_grassStone.push_back(index); break;
			case LANDSCAPE_PAINT::SNOW:		m_grassSnow.push_back(index); break;
			}
		}
	}

	// Else, if current paint is dirt
	else if (current == LANDSCAPE_PAINT::DIRT)
	{
		// If selected paint is dirt, do nothing
		if (paint == LANDSCAPE_PAINT::DIRT) {}

		// Else, apply selected paint
		else
		{
			// Switch between selected paint
			switch (paint)
			{
			case LANDSCAPE_PAINT::GRASS:	m_grassDirt.push_back(index); break;
			case LANDSCAPE_PAINT::SAND:		m_dirtSand.push_back(index); break;
			case LANDSCAPE_PAINT::STONE:	m_dirtStone.push_back(index); break;
			case LANDSCAPE_PAINT::SNOW:		m_dirtSnow.push_back(index); break;
			}
		}
	}

	// Else, if current paint is sand
	else if (current == LANDSCAPE_PAINT::SAND)
	{
		// If selected paint is sand, do nothing
		if (paint == LANDSCAPE_PAINT::SAND) {}

		// Else, apply selected paint
		else
		{
			// Switch between selected paint
			switch (paint)
			{
			case LANDSCAPE_PAINT::GRASS: m_grassSand.push_back(index); break;
			case LANDSCAPE_PAINT::DIRT: m_dirtSand.push_back(index); break;
			case LANDSCAPE_PAINT::STONE: m_sandStone.push_back(index); break;
			case LANDSCAPE_PAINT::SNOW: m_sandSnow.push_back(index); break;
			}
		}
	}

	// Else, if current paint is stone
	else if (current == LANDSCAPE_PAINT::STONE)
	{
		// If selected paint is stone, do nothing
		if (paint == LANDSCAPE_PAINT::STONE) {}

		// Else, apply selected paint
		else
		{
			// Switch between selected paint
			switch (paint)
			{
			case LANDSCAPE_PAINT::GRASS: m_grassStone.push_back(index); break;
			case LANDSCAPE_PAINT::DIRT: m_dirtStone.push_back(index); break;
			case LANDSCAPE_PAINT::SAND: m_sandStone.push_back(index); break;
			case LANDSCAPE_PAINT::SNOW: m_stoneSnow.push_back(index); break;
			}
		}
	}

	// Else, if current paint is snow
	else if (current == LANDSCAPE_PAINT::SNOW)
	{
		// If selected paint is snow, do nothing
		if (paint == LANDSCAPE_PAINT::SNOW) {}

		// Else, apply selected paint
		else
		{
			// Switch between selected paint
			switch (paint)
			{
			case LANDSCAPE_PAINT::GRASS: m_grassSnow.push_back(index); break;
			case LANDSCAPE_PAINT::DIRT: m_dirtSnow.push_back(index); break;
			case LANDSCAPE_PAINT::SAND: m_sandSnow.push_back(index); break;
			case LANDSCAPE_PAINT::STONE: m_stoneSnow.push_back(index); break;
			}
		}
	}
	
	// Else, current paint must be blended
	else { PaintOverBlended(paint, index); }
	
	// Switch between current paint
	//switch (CheckForDuplicates(i, j, paint))
	//{
	//case LANDSCAPE_PAINT::GRASS:
	//{
	//	// If selected paint is grass, do nothing
	//	if (paint == LANDSCAPE_PAINT::GRASS) {}

	//	// Else, apply selected paint
	//	else 
	//	{
	//		// Switch between selected paint
	//		switch (paint)
	//		{
	//		case LANDSCAPE_PAINT::DIRT:		m_grassDirt.push_back(index); break;
	//		case LANDSCAPE_PAINT::SAND:		m_grassSand.push_back(index); break;
	//		case LANDSCAPE_PAINT::STONE:	m_grassStone.push_back(index); break;
	//		case LANDSCAPE_PAINT::SNOW:		m_grassSnow.push_back(index); break;
	//		}
	//	}
	//}
	//break;
	//case LANDSCAPE_PAINT::DIRT:
	//{
	//	// If selected paint is dirt, do nothing
	//	if (paint == LANDSCAPE_PAINT::DIRT) {}

	//	// Else, apply selected paint
	//	else
	//	{
	//		// Switch between selected paint
	//		switch (paint)
	//		{
	//		case LANDSCAPE_PAINT::GRASS:	m_grassDirt.push_back(index); break;
	//		case LANDSCAPE_PAINT::SAND:		m_dirtSand.push_back(index); break;
	//		case LANDSCAPE_PAINT::STONE:	m_dirtStone.push_back(index); break;
	//		case LANDSCAPE_PAINT::SNOW:		m_dirtSnow.push_back(index); break;
	//		}
	//	}
	//}
	//break;
	//case LANDSCAPE_PAINT::SAND:
	//{
	//	// If selected paint is sand, do nothing
	//	if (paint == LANDSCAPE_PAINT::SAND) {}

	//	// Else, apply selected paint
	//	else
	//	{
	//		// Switch between selected paint
	//		switch (paint)
	//		{
	//		case LANDSCAPE_PAINT::GRASS: m_grassSand.push_back(index); break;
	//		case LANDSCAPE_PAINT::DIRT: m_dirtSand.push_back(index); break;
	//		case LANDSCAPE_PAINT::STONE: m_sandStone.push_back(index); break;
	//		case LANDSCAPE_PAINT::SNOW: m_sandSnow.push_back(index); break;
	//		}
	//	}
	//}
	//break;
	//case LANDSCAPE_PAINT::STONE:
	//{
	//	// If selected paint is stone, do nothing
	//	if (paint == LANDSCAPE_PAINT::STONE) {}

	//	// Else, apply selected paint
	//	else
	//	{
	//		// Switch between selected paint
	//		switch (paint)
	//		{
	//		case LANDSCAPE_PAINT::GRASS: m_grassStone.push_back(index); break;
	//		case LANDSCAPE_PAINT::DIRT: m_dirtStone.push_back(index); break;
	//		case LANDSCAPE_PAINT::SAND: m_sandStone.push_back(index); break;
	//		case LANDSCAPE_PAINT::SNOW: m_stoneSnow.push_back(index); break;
	//		}
	//	}
	//}
	//break;
	//case LANDSCAPE_PAINT::SNOW:
	//{
	//	// If selected paint is snow, do nothing
	//	if (paint == LANDSCAPE_PAINT::SNOW) {}

	//	// Else, apply selected paint
	//	else
	//	{
	//		// Switch between selected paint
	//		switch (paint)
	//		{
	//		case LANDSCAPE_PAINT::GRASS: m_grassSnow.push_back(index); break;
	//		case LANDSCAPE_PAINT::DIRT: m_dirtSnow.push_back(index); break;
	//		case LANDSCAPE_PAINT::SAND: m_sandSnow.push_back(index); break;
	//		case LANDSCAPE_PAINT::STONE: m_stoneSnow.push_back(index); break;
	//		}
	//	}
	//}
	//break;
	//}
	
	// Check if selected geometry is within other containers & remove
	///LANDSCAPE_PAINT current = CheckForDuplicates(i, j, paint);

	// Blend surrounding geometry paints
	///if (checkSurroundings) { CheckSurroundings(i, j, paint); }

	// Switch between paints
	//switch (paint)
	//{
	//case LANDSCAPE_PAINT::NA: m_default.push_back(std::pair<int, int>(i, j)); break;
	//case LANDSCAPE_PAINT::GRASS: m_grass.push_back(std::pair<int, int>(i, j)); break;
	//case LANDSCAPE_PAINT::DIRT: 
	//{
	//	///m_dirt.push_back(std::pair<int, int>(i, j));
	//	// Switch between current paint
	//	switch (current)
	//	{
	//	case LANDSCAPE_PAINT::GRASS: m_grassDirt.push_back(std::pair<int, int>(i, j)); break;
	//	case LANDSCAPE_PAINT::DIRT: m_dirt.push_back(std::pair<int, int>(i, j)); break;
	//	case LANDSCAPE_PAINT::SAND: m_dirtSand.push_back(std::pair<int, int>(i, j)); break;
	//	case LANDSCAPE_PAINT::STONE: m_dirtStone.push_back(std::pair<int, int>(i, j)); break;
	//	case LANDSCAPE_PAINT::SNOW: m_dirtSnow.push_back(std::pair<int, int>(i, j)); break;
	//	case LANDSCAPE_PAINT::GRASS_DIRT: m_dirt.push_back(std::pair<int, int>(i, j)); break;
	//	}
	//}
	//break;
	//case LANDSCAPE_PAINT::SAND: m_sand.push_back(std::pair<int, int>(i, j)); break;
	//case LANDSCAPE_PAINT::STONE: m_stone.push_back(std::pair<int, int>(i, j)); break;
	//case LANDSCAPE_PAINT::SNOW: m_snow.push_back(std::pair<int, int>(i, j)); break;


	//case LANDSCAPE_PAINT::GRASS_DIRT: m_grassDirt.push_back(std::pair<int, int>(i, j)); break;
	//case LANDSCAPE_PAINT::GRASS_SAND: m_grassSand.push_back(std::pair<int, int>(i, j)); break;
	//case LANDSCAPE_PAINT::GRASS_STONE: m_grassStone.push_back(std::pair<int, int>(i, j)); break;
	//case LANDSCAPE_PAINT::GRASS_SNOW: m_grassSnow.push_back(std::pair<int, int>(i, j)); break;
	//case LANDSCAPE_PAINT::DIRT_SAND: m_dirtSand.push_back(std::pair<int, int>(i, j)); break;
	//case LANDSCAPE_PAINT::DIRT_STONE: m_dirtStone.push_back(std::pair<int, int>(i, j)); break;
	//case LANDSCAPE_PAINT::DIRT_SNOW: m_dirtSnow.push_back(std::pair<int, int>(i, j)); break;
	//case LANDSCAPE_PAINT::SAND_STONE: m_sandStone.push_back(std::pair<int, int>(i, j)); break;
	//case LANDSCAPE_PAINT::SAND_SNOW: m_sandSnow.push_back(std::pair<int, int>(i, j)); break;
	//case LANDSCAPE_PAINT::STONE_SNOW: m_stoneSnow.push_back(std::pair<int, int>(i, j)); break;
	//}
}

void DisplayChunk::PaintOverBlended(LANDSCAPE_PAINT paint, std::pair<int, int> index)
{
	// Switch between selected paint
	switch (paint)
	{
	case LANDSCAPE_PAINT::GRASS: m_grass.push_back(index); break;
	case LANDSCAPE_PAINT::DIRT: m_dirt.push_back(index); break;
	case LANDSCAPE_PAINT::SAND: m_sand.push_back(index); break;
	case LANDSCAPE_PAINT::STONE: m_stone.push_back(index); break;
	case LANDSCAPE_PAINT::SNOW: m_snow.push_back(index); break;
	}
}

void DisplayChunk::SculptTerrain(int row, int column, LANDSCAPE_FUNCTION sculpt, LANDSCAPE_CONSTRAINT constraint, std::vector<DirectX::SimpleMath::Vector3> position)
{
	// Store starting vertex
	Vector3 start = m_terrainGeometry[row][column].position; 

	// Store other vertices of quad
	std::vector<Vector3> others;
	others.push_back(m_terrainGeometry[row][column + 1].position);
	others.push_back(m_terrainGeometry[row + 1][column + 1].position);
	others.push_back(m_terrainGeometry[row + 1][column].position);

	// Store distances between start and other vertices
	std::vector<float> distances;
	for (int i = 0; i < others.size(); ++i)
	{
		float dist = sqrt(
			(start.x - others[i].x) * (start.x - others[i].x) +
			(start.y - others[i].y) * (start.y - others[i].y) +
			(start.z - others[i].z) * (start.z - others[i].z));
		distances.push_back(dist);
	}
		
	// Loop through distances
	for (int i = 0; i < distances.size(); ++i)
	{
		// Divide each distance to put in 0,1 interval
		distances[i] = (distances[i] * 2.f) - 1.f;

		// Plug into cosine
		distances[i] = sin(distances[i]);

		///cos(m_terrainGeometry[i][j].position.x + (deltaTime * 100.f))
	}

	// Test
	if (sculpt == LANDSCAPE_FUNCTION::INCREASE)
	{
		switch (constraint)
		{
		case LANDSCAPE_CONSTRAINT::Y:
		{
			// Increase position
			m_terrainGeometry[row][column].position.y += 1.f;
			m_terrainGeometry[row][column + 1].position.y += distances[0];
			m_terrainGeometry[row + 1][column + 1].position.y += distances[1];
			m_terrainGeometry[row + 1][column].position.y += distances[2];
		}
		break;
		}
	}
	
	// Switch between sculpt parameter
	//switch (sculpt)
	//{
	//case LANDSCAPE_FUNCTION::INCREASE:
	//{
	//	// Switch between constraint
	//	switch (constraint)
	//	{
	//	case LANDSCAPE_CONSTRAINT::XY:
	//	{			
	//		// Increase position
	//		m_terrainGeometry[row][column].position.x += 1.f;
	//		m_terrainGeometry[row][column + 1].position.x += 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.x += 1.f;
	//		m_terrainGeometry[row + 1][column].position.x += 1.f;

	//		m_terrainGeometry[row][column].position.y += 1.f;
	//		m_terrainGeometry[row][column + 1].position.y += 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.y += 1.f;
	//		m_terrainGeometry[row + 1][column].position.y += 1.f;
	//	}
	//	break;
	//	case LANDSCAPE_CONSTRAINT::XZ:
	//	{
	//		// Increase position
	//		m_terrainGeometry[row][column].position.x += 1.f;
	//		m_terrainGeometry[row][column + 1].position.x += 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.x += 1.f;
	//		m_terrainGeometry[row + 1][column].position.x += 1.f;

	//		m_terrainGeometry[row][column].position.z += 1.f;
	//		m_terrainGeometry[row][column + 1].position.y += 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.y += 1.f;
	//		m_terrainGeometry[row + 1][column].position.y += 1.f;
	//	}
	//	break;
	//	case LANDSCAPE_CONSTRAINT::YZ:
	//	{
	//		// Increase position
	//		m_terrainGeometry[row][column].position.y += 1.f;
	//		m_terrainGeometry[row][column + 1].position.y += 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.y += 1.f;
	//		m_terrainGeometry[row + 1][column].position.y += 1.f;

	//		m_terrainGeometry[row][column].position.z += 1.f;
	//		m_terrainGeometry[row][column + 1].position.z += 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.z += 1.f;
	//		m_terrainGeometry[row + 1][column].position.z += 1.f;
	//	}
	//	break;
	//	case LANDSCAPE_CONSTRAINT::X:
	//	{
	//		// Increase position
	//		m_terrainGeometry[row][column].position.x += 1.f;
	//		m_terrainGeometry[row][column + 1].position.x += 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.x += 1.f;
	//		m_terrainGeometry[row + 1][column].position.x += 1.f;
	//	}
	//	break;
	//	case LANDSCAPE_CONSTRAINT::Y:
	//	{
	//		// Increase position
	//		m_terrainGeometry[row][column].position.y += 1.f;
	//		m_terrainGeometry[row][column + 1].position.y += 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.y += 1.f;
	//		m_terrainGeometry[row + 1][column].position.y += 1.f;
	//	}
	//	break;
	//	case LANDSCAPE_CONSTRAINT::Z:
	//	{
	//		// Increase position
	//		m_terrainGeometry[row][column].position.z += 1.f;
	//		m_terrainGeometry[row][column + 1].position.z += 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.z += 1.f;
	//		m_terrainGeometry[row + 1][column].position.z += 1.f;
	//	}
	//	break;
	//	case LANDSCAPE_CONSTRAINT::ALL:
	//	{
	//		// Increase position
	//		m_terrainGeometry[row][column].position.x += 1.f;
	//		m_terrainGeometry[row][column + 1].position.x += 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.x += 1.f;
	//		m_terrainGeometry[row + 1][column].position.x += 1.f;

	//		m_terrainGeometry[row][column].position.y += 1.f;
	//		m_terrainGeometry[row][column + 1].position.y += 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.y += 1.f;
	//		m_terrainGeometry[row + 1][column].position.y += 1.f;

	//		m_terrainGeometry[row][column].position.z += 1.f;
	//		m_terrainGeometry[row][column + 1].position.z += 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.z += 1.f;
	//		m_terrainGeometry[row + 1][column].position.z += 1.f;
	//	}
	//	break;
	//	}
	//}
	//break;
	//case LANDSCAPE_FUNCTION::DECREASE:
	//{
	//	// Switch between constraint
	//	switch (constraint)
	//	{
	//	case LANDSCAPE_CONSTRAINT::XY:
	//	{
	//		// Decrease position
	//		m_terrainGeometry[row][column].position.x -= 1.f;
	//		m_terrainGeometry[row][column + 1].position.x -= 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.x -= 1.f;
	//		m_terrainGeometry[row + 1][column].position.x -= 1.f;

	//		m_terrainGeometry[row][column].position.y -= 1.f;
	//		m_terrainGeometry[row][column + 1].position.y -= 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.y -= 1.f;
	//		m_terrainGeometry[row + 1][column].position.y -= 1.f;
	//	}
	//	break;
	//	case LANDSCAPE_CONSTRAINT::XZ:
	//	{
	//		// Decrease position
	//		m_terrainGeometry[row][column].position.x -= 1.f;
	//		m_terrainGeometry[row][column + 1].position.x -= 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.x -= 1.f;
	//		m_terrainGeometry[row + 1][column].position.x -= 1.f;

	//		m_terrainGeometry[row][column].position.z -= 1.f;
	//		m_terrainGeometry[row][column + 1].position.z -= 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.z -= 1.f;
	//		m_terrainGeometry[row + 1][column].position.z -= 1.f;
	//	}
	//	break;
	//	case LANDSCAPE_CONSTRAINT::YZ:
	//	{
	//		// Decrease position
	//		m_terrainGeometry[row][column].position.y -= 1.f;
	//		m_terrainGeometry[row][column + 1].position.y -= 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.y -= 1.f;
	//		m_terrainGeometry[row + 1][column].position.y -= 1.f;

	//		m_terrainGeometry[row][column].position.z -= 1.f;
	//		m_terrainGeometry[row][column + 1].position.z -= 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.z -= 1.f;
	//		m_terrainGeometry[row + 1][column].position.z -= 1.f;
	//	}
	//	break;
	//	case LANDSCAPE_CONSTRAINT::X:
	//	{
	//		// Decrease position
	//		m_terrainGeometry[row][column].position.x -= 1.f;
	//		m_terrainGeometry[row][column + 1].position.x -= 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.x -= 1.f;
	//		m_terrainGeometry[row + 1][column].position.x -= 1.f;
	//	}
	//	break;
	//	case LANDSCAPE_CONSTRAINT::Y:
	//	{
	//		// Decrease position
	//		m_terrainGeometry[row][column].position.y -= 1.f;
	//		m_terrainGeometry[row][column + 1].position.y -= 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.y -= 1.f;
	//		m_terrainGeometry[row + 1][column].position.y -= 1.f;
	//	}
	//	break;
	//	case LANDSCAPE_CONSTRAINT::Z:
	//	{
	//		// Decrease position
	//		m_terrainGeometry[row][column].position.z -= 1.f;
	//		m_terrainGeometry[row][column + 1].position.z -= 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.z -= 1.f;
	//		m_terrainGeometry[row + 1][column].position.z -= 1.f;
	//	}
	//	break;
	//	case LANDSCAPE_CONSTRAINT::ALL:
	//	{
	//		// Decrease position
	//		m_terrainGeometry[row][column].position.x -= 1.f;
	//		m_terrainGeometry[row][column + 1].position.x -= 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.x -= 1.f;
	//		m_terrainGeometry[row + 1][column].position.x -= 1.f;

	//		m_terrainGeometry[row][column].position.y -= 1.f;
	//		m_terrainGeometry[row][column + 1].position.y -= 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.y -= 1.f;
	//		m_terrainGeometry[row + 1][column].position.y -= 1.f;

	//		m_terrainGeometry[row][column].position.z -= 1.f;
	//		m_terrainGeometry[row][column + 1].position.z -= 1.f;
	//		m_terrainGeometry[row + 1][column + 1].position.z -= 1.f;
	//		m_terrainGeometry[row + 1][column].position.z -= 1.f;
	//	}
	//	break;
	//	}
	//}
	//break;
	//case LANDSCAPE_FUNCTION::FLATTEN:
	//{
	//	// Switch between constraint
	//	switch (constraint)
	//	{
	//	case LANDSCAPE_CONSTRAINT::XY:
	//	{
	//		// X
	//		{
	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row][column].position.x > position[0].x + 1.f) { m_terrainGeometry[row][column].position.x -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row][column].position.x < position[0].x - 1.f) { m_terrainGeometry[row][column].position.x += 1.f; }
	//			
	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row][column + 1].position.x > position[1].x + 1.f) { m_terrainGeometry[row][column + 1].position.x -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row][column + 1].position.x < position[1].x - 1.f) { m_terrainGeometry[row][column + 1].position.x += 1.f; }
	//			
	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row + 1][column + 1].position.x > position[2].x + 1.f) { m_terrainGeometry[row + 1][column + 1].position.x -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row + 1][column + 1].position.x < position[2].x - 1.f) { m_terrainGeometry[row + 1][column + 1].position.x += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row + 1][column].position.x > position[3].x + 1.f) { m_terrainGeometry[row + 1][column].position.x -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row + 1][column].position.x < position[3].x - 1.f) { m_terrainGeometry[row + 1][column].position.x += 1.f; }
	//		}

	//		// Y
	//		{
	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row][column].position.y > position[0].y + 1.f) { m_terrainGeometry[row][column].position.y -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row][column].position.y < position[0].y - 1.f) { m_terrainGeometry[row][column].position.y += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row][column + 1].position.y > position[1].y + 1.f) { m_terrainGeometry[row][column + 1].position.y -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row][column + 1].position.y < position[1].y - 1.f) { m_terrainGeometry[row][column + 1].position.y += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row + 1][column + 1].position.y > position[2].y + 1.f) { m_terrainGeometry[row + 1][column + 1].position.y -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row + 1][column + 1].position.y < position[2].y - 1.f) { m_terrainGeometry[row + 1][column + 1].position.y += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row + 1][column].position.y > position[3].y + 1.f) { m_terrainGeometry[row + 1][column].position.y -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row + 1][column].position.y < position[3].y - 1.f) { m_terrainGeometry[row + 1][column].position.y += 1.f; }
	//		}
	//	}
	//	break;
	//	case LANDSCAPE_CONSTRAINT::XZ:
	//	{
	//		// X
	//		{
	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row][column].position.x > position[0].x + 1.f) { m_terrainGeometry[row][column].position.x -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row][column].position.x < position[0].x - 1.f) { m_terrainGeometry[row][column].position.x += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row][column + 1].position.x > position[1].x + 1.f) { m_terrainGeometry[row][column + 1].position.x -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row][column + 1].position.x < position[1].x - 1.f) { m_terrainGeometry[row][column + 1].position.x += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row + 1][column + 1].position.x > position[2].x + 1.f) { m_terrainGeometry[row + 1][column + 1].position.x -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row + 1][column + 1].position.x < position[2].x - 1.f) { m_terrainGeometry[row + 1][column + 1].position.x += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row + 1][column].position.x > position[3].x + 1.f) { m_terrainGeometry[row + 1][column].position.x -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row + 1][column].position.x < position[3].x - 1.f) { m_terrainGeometry[row + 1][column].position.x += 1.f; }
	//		}

	//		// Z
	//		{
	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row][column].position.z > position[0].z + 1.f) { m_terrainGeometry[row][column].position.z -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row][column].position.z < position[0].z - 1.f) { m_terrainGeometry[row][column].position.z += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row][column + 1].position.z > position[1].z + 1.f) { m_terrainGeometry[row][column + 1].position.z -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row][column + 1].position.z < position[1].z - 1.f) { m_terrainGeometry[row][column + 1].position.z += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row + 1][column + 1].position.z > position[2].z + 1.f) { m_terrainGeometry[row + 1][column + 1].position.z -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row + 1][column + 1].position.z < position[2].z - 1.f) { m_terrainGeometry[row + 1][column + 1].position.z += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row + 1][column].position.z > position[3].z + 1.f) { m_terrainGeometry[row + 1][column].position.z -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row + 1][column].position.z < position[3].z - 1.f) { m_terrainGeometry[row + 1][column].position.z += 1.f; }
	//		}
	//	}
	//	break;
	//	case LANDSCAPE_CONSTRAINT::YZ:
	//	{
	//		// Y
	//		{
	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row][column].position.y > position[0].y + 1.f) { m_terrainGeometry[row][column].position.y -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row][column].position.y < position[0].y - 1.f) { m_terrainGeometry[row][column].position.y += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row][column + 1].position.y > position[1].y + 1.f) { m_terrainGeometry[row][column + 1].position.y -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row][column + 1].position.y < position[1].y - 1.f) { m_terrainGeometry[row][column + 1].position.y += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row + 1][column + 1].position.y > position[2].y + 1.f) { m_terrainGeometry[row + 1][column + 1].position.y -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row + 1][column + 1].position.y < position[2].y - 1.f) { m_terrainGeometry[row + 1][column + 1].position.y += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row + 1][column].position.y > position[3].y + 1.f) { m_terrainGeometry[row + 1][column].position.y -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row + 1][column].position.y < position[3].y - 1.f) { m_terrainGeometry[row + 1][column].position.y += 1.f; }
	//		}

	//		// Z
	//		{
	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row][column].position.z > position[0].z + 1.f) { m_terrainGeometry[row][column].position.z -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row][column].position.z < position[0].z - 1.f) { m_terrainGeometry[row][column].position.z += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row][column + 1].position.z > position[1].z + 1.f) { m_terrainGeometry[row][column + 1].position.z -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row][column + 1].position.z < position[1].z - 1.f) { m_terrainGeometry[row][column + 1].position.z += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row + 1][column + 1].position.z > position[2].z + 1.f) { m_terrainGeometry[row + 1][column + 1].position.z -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row + 1][column + 1].position.z < position[2].z - 1.f) { m_terrainGeometry[row + 1][column + 1].position.z += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row + 1][column].position.z > position[3].z + 1.f) { m_terrainGeometry[row + 1][column].position.z -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row + 1][column].position.z < position[3].z - 1.f) { m_terrainGeometry[row + 1][column].position.z += 1.f; }
	//		}
	//	}
	//	break;
	//	case LANDSCAPE_CONSTRAINT::X:
	//	{
	//		// If selected geometry is above ground level, decrease position
	//		if (m_terrainGeometry[row][column].position.x > position[0].x + 1.f) { m_terrainGeometry[row][column].position.x -= 1.f; }
	//		// Else, if selected geometry is below ground level, increase position
	//		else if (m_terrainGeometry[row][column].position.x < position[0].x - 1.f) { m_terrainGeometry[row][column].position.x += 1.f; }

	//		// If selected geometry is above ground level, decrease position
	//		if (m_terrainGeometry[row][column + 1].position.x > position[1].x + 1.f) { m_terrainGeometry[row][column + 1].position.x -= 1.f; }
	//		// Else, if selected geometry is below ground level, increase position
	//		else if (m_terrainGeometry[row][column + 1].position.x < position[1].x - 1.f) { m_terrainGeometry[row][column + 1].position.x += 1.f; }

	//		// If selected geometry is above ground level, decrease position
	//		if (m_terrainGeometry[row + 1][column + 1].position.x > position[2].x + 1.f) { m_terrainGeometry[row + 1][column + 1].position.x -= 1.f; }
	//		// Else, if selected geometry is below ground level, increase position
	//		else if (m_terrainGeometry[row + 1][column + 1].position.x < position[2].x - 1.f) { m_terrainGeometry[row + 1][column + 1].position.x += 1.f; }

	//		// If selected geometry is above ground level, decrease position
	//		if (m_terrainGeometry[row + 1][column].position.x > position[3].x + 1.f) { m_terrainGeometry[row + 1][column].position.x -= 1.f; }
	//		// Else, if selected geometry is below ground level, increase position
	//		else if (m_terrainGeometry[row + 1][column].position.x < position[3].x - 1.f) { m_terrainGeometry[row + 1][column].position.x += 1.f; }
	//	}
	//	break;
	//	case LANDSCAPE_CONSTRAINT::Y:
	//	{
	//		// If selected geometry is above ground level, decrease position
	//		if (m_terrainGeometry[row][column].position.y > position[0].y + 1.f) { m_terrainGeometry[row][column].position.y -= 1.f; }
	//		// Else, if selected geometry is below ground level, increase position
	//		else if (m_terrainGeometry[row][column].position.y < position[0].y - 1.f) { m_terrainGeometry[row][column].position.y += 1.f; }

	//		// If selected geometry is above ground level, decrease position
	//		if (m_terrainGeometry[row][column + 1].position.y > position[1].y + 1.f) { m_terrainGeometry[row][column + 1].position.y -= 1.f; }
	//		// Else, if selected geometry is below ground level, increase position
	//		else if (m_terrainGeometry[row][column + 1].position.y < position[1].y - 1.f) { m_terrainGeometry[row][column + 1].position.y += 1.f; }

	//		// If selected geometry is above ground level, decrease position
	//		if (m_terrainGeometry[row + 1][column + 1].position.y > position[2].y + 1.f) { m_terrainGeometry[row + 1][column + 1].position.y -= 1.f; }
	//		// Else, if selected geometry is below ground level, increase position
	//		else if (m_terrainGeometry[row + 1][column + 1].position.y < position[2].y - 1.f) { m_terrainGeometry[row + 1][column + 1].position.y += 1.f; }

	//		// If selected geometry is above ground level, decrease position
	//		if (m_terrainGeometry[row + 1][column].position.y > position[3].y + 1.f) { m_terrainGeometry[row + 1][column].position.y -= 1.f; }
	//		// Else, if selected geometry is below ground level, increase position
	//		else if (m_terrainGeometry[row + 1][column].position.y < position[3].y - 1.f) { m_terrainGeometry[row + 1][column].position.y += 1.f; }
	//	}
	//	break;
	//	case LANDSCAPE_CONSTRAINT::Z:
	//	{
	//		// If selected geometry is above ground level, decrease position
	//		if (m_terrainGeometry[row][column].position.z > position[0].z + 1.f) { m_terrainGeometry[row][column].position.z -= 1.f; }
	//		// Else, if selected geometry is below ground level, increase position
	//		else if (m_terrainGeometry[row][column].position.z < position[0].z - 1.f) { m_terrainGeometry[row][column].position.z += 1.f; }

	//		// If selected geometry is above ground level, decrease position
	//		if (m_terrainGeometry[row][column + 1].position.z > position[1].z + 1.f) { m_terrainGeometry[row][column + 1].position.z -= 1.f; }
	//		// Else, if selected geometry is below ground level, increase position
	//		else if (m_terrainGeometry[row][column + 1].position.z < position[1].z - 1.f) { m_terrainGeometry[row][column + 1].position.z += 1.f; }

	//		// If selected geometry is above ground level, decrease position
	//		if (m_terrainGeometry[row + 1][column + 1].position.z > position[2].z + 1.f) { m_terrainGeometry[row + 1][column + 1].position.z -= 1.f; }
	//		// Else, if selected geometry is below ground level, increase position
	//		else if (m_terrainGeometry[row + 1][column + 1].position.z < position[2].z - 1.f) { m_terrainGeometry[row + 1][column + 1].position.z += 1.f; }

	//		// If selected geometry is above ground level, decrease position
	//		if (m_terrainGeometry[row + 1][column].position.z > position[3].z + 1.f) { m_terrainGeometry[row + 1][column].position.z -= 1.f; }
	//		// Else, if selected geometry is below ground level, increase position
	//		else if (m_terrainGeometry[row + 1][column].position.z < position[3].z - 1.f) { m_terrainGeometry[row + 1][column].position.z += 1.f; }
	//	}
	//	break;
	//	case LANDSCAPE_CONSTRAINT::ALL:
	//	{
	//		// X
	//		{
	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row][column].position.x > position[0].x + 1.f) { m_terrainGeometry[row][column].position.x -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row][column].position.x < position[0].x - 1.f) { m_terrainGeometry[row][column].position.x += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row][column + 1].position.x > position[1].x + 1.f) { m_terrainGeometry[row][column + 1].position.x -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row][column + 1].position.x < position[1].x - 1.f) { m_terrainGeometry[row][column + 1].position.x += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row + 1][column + 1].position.x > position[2].x + 1.f) { m_terrainGeometry[row + 1][column + 1].position.x -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row + 1][column + 1].position.x < position[2].x - 1.f) { m_terrainGeometry[row + 1][column + 1].position.x += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row + 1][column].position.x > position[3].x + 1.f) { m_terrainGeometry[row + 1][column].position.x -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row + 1][column].position.x < position[3].x - 1.f) { m_terrainGeometry[row + 1][column].position.x += 1.f; }
	//		}
	//		
	//		// Y
	//		{
	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row][column].position.y > position[0].y + 1.f) { m_terrainGeometry[row][column].position.y -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row][column].position.y < position[0].y - 1.f) { m_terrainGeometry[row][column].position.y += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row][column + 1].position.y > position[1].y + 1.f) { m_terrainGeometry[row][column + 1].position.y -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row][column + 1].position.y < position[1].y - 1.f) { m_terrainGeometry[row][column + 1].position.y += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row + 1][column + 1].position.y > position[2].y + 1.f) { m_terrainGeometry[row + 1][column + 1].position.y -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row + 1][column + 1].position.y < position[2].y - 1.f) { m_terrainGeometry[row + 1][column + 1].position.y += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row + 1][column].position.y > position[3].y + 1.f) { m_terrainGeometry[row + 1][column].position.y -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row + 1][column].position.y < position[3].y - 1.f) { m_terrainGeometry[row + 1][column].position.y += 1.f; }
	//		}
	//		
	//		// Z
	//		{
	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row][column].position.z > position[0].z + 1.f) { m_terrainGeometry[row][column].position.z -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row][column].position.z < position[0].z - 1.f) { m_terrainGeometry[row][column].position.z += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row][column + 1].position.z > position[1].z + 1.f) { m_terrainGeometry[row][column + 1].position.z -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row][column + 1].position.z < position[1].z - 1.f) { m_terrainGeometry[row][column + 1].position.z += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row + 1][column + 1].position.z > position[2].z + 1.f) { m_terrainGeometry[row + 1][column + 1].position.z -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row + 1][column + 1].position.z < position[2].z - 1.f) { m_terrainGeometry[row + 1][column + 1].position.z += 1.f; }

	//			// If selected geometry is above ground level, decrease position
	//			if (m_terrainGeometry[row + 1][column].position.z > position[3].z + 1.f) { m_terrainGeometry[row + 1][column].position.z -= 1.f; }
	//			// Else, if selected geometry is below ground level, increase position
	//			else if (m_terrainGeometry[row + 1][column].position.z < position[3].z - 1.f) { m_terrainGeometry[row + 1][column].position.z += 1.f; }
	//		}			
	//	}
	//	break;
	//	}
	//}
	//break;
	//}		

	// Ensure terrain height can't go below 0
	if (m_terrainGeometry[row][column].position.y < 0.f)			{ m_terrainGeometry[row][column].position.y = 0.f; }
	if (m_terrainGeometry[row][column + 1].position.y < 0.f)		{ m_terrainGeometry[row][column + 1].position.y = 0.f; }
	if (m_terrainGeometry[row + 1][column + 1].position.y < 0.f)	{ m_terrainGeometry[row + 1][column + 1].position.y = 0.f; }
	if (m_terrainGeometry[row + 1][column].position.y < 0.f)		{ m_terrainGeometry[row + 1][column].position.y = 0.f; }
}

void DisplayChunk::SetSelected(bool selected, int row, int column)
{
	// If selecting
	if (selected)
	{
		// Remove from other vectors
		CheckForDuplicates(row, column);

		// Add to highlight vector
		m_highlight.push_back(std::pair<int, int>(row, column));
	}
	// Else, if unselecting
	else
	{
		// Temp
		int index = -1;
		std::pair<int, int> terrain = std::pair<int, int>(row, column);
			
		// Search through temp grass vector
		if (FindInVector(index, m_grassTemp, terrain))
		{
			// Add to main storage
			m_grass.push_back(terrain);

			// Remove from temp storage
			m_grassTemp.erase(m_grassTemp.begin() + index);
		}

		// Else, search through temp dirt vector
		else if (FindInVector(index, m_dirtTemp, terrain))
		{
			// Add to main storage
			m_dirt.push_back(terrain);

			// Remove from temp storage
			m_dirtTemp.erase(m_dirtTemp.begin() + index);
		}

		// Else, search through temp sand vector
		else if (FindInVector(index, m_sandTemp, terrain))
		{
			// Add to main storage
			m_sand.push_back(terrain);

			// Remove from temp storage
			m_sandTemp.erase(m_sandTemp.begin() + index);
		}
	}
}

void DisplayChunk::DrawTerrain(std::vector<std::pair<int, int>> terrain)
{
	for (int i = 0; i < terrain.size(); ++i)
	{
		m_batch->DrawQuad(
			m_terrainGeometry[terrain[i].first][terrain[i].second],
			m_terrainGeometry[terrain[i].first][terrain[i].second + 1],
			m_terrainGeometry[terrain[i].first + 1][terrain[i].second + 1],
			m_terrainGeometry[terrain[i].first + 1][terrain[i].second]
		);
	}
}

ChunkObject DisplayChunk::GetChunk()
{
	// Setup temp chunk object
	ChunkObject temp;
	
	// Populate temp chunk object
	temp.name = m_name;
	temp.chunk_x_size_metres = m_chunk_x_size_metres;
	temp.chunk_y_size_metres = m_chunk_y_size_metres;
	temp.chunk_base_resolution = m_chunk_base_resolution;
	temp.heightmap_path = m_heightmap_path;
	temp.tex_diffuse_path = m_tex_diffuse_path;
	temp.tex_splat_alpha_path = m_tex_splat_alpha_path;
	temp.tex_splat_1_path = m_tex_splat_1_path;
	temp.tex_splat_2_path = m_tex_splat_2_path;
	temp.tex_splat_3_path = m_tex_splat_3_path;
	temp.tex_splat_4_path = m_tex_splat_4_path;
	temp.render_wireframe = m_render_wireframe;
	temp.render_normals = m_render_normals;
	temp.tex_diffuse_tiling = m_tex_diffuse_tiling;
	temp.tex_splat_1_tiling = m_tex_splat_1_tiling;
	temp.tex_splat_2_tiling = m_tex_splat_2_tiling;
	temp.tex_splat_3_tiling = m_tex_splat_3_tiling;
	temp.tex_splat_4_tiling = m_tex_splat_4_tiling;

	// Return temp chunk object
	return temp;
}

LANDSCAPE_PAINT DisplayChunk::CheckForDuplicates(int row, int column, LANDSCAPE_PAINT paint)
{
	// Temp 
	LANDSCAPE_PAINT current = LANDSCAPE_PAINT::NA;
	int index = -1;
	std::pair<int, int> terrain;
	terrain.first = row;
	terrain.second = column;
	bool check = false;

	// Switch between paints
	switch (paint)
	{
	case LANDSCAPE_PAINT::GRASS:
	{
		// Else, search through dirt vector
		check = SearchAndDestroy(index, m_dirt, terrain);
		if (check) { current = LANDSCAPE_PAINT::DIRT; break; }

		// Else, search through sand vector
		if (!check) { check = SearchAndDestroy(index, m_sand, terrain); }
		if (check) { current = LANDSCAPE_PAINT::SAND; break; }

		// Else, search through stone vector
		if (!check) { check = SearchAndDestroy(index, m_stone, terrain); }
		if (check) { current = LANDSCAPE_PAINT::STONE; break; }

		// Else, search through snow vector
		if (!check) { check = SearchAndDestroy(index, m_snow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::SNOW; break; }

		// Else, search through grass/dirt blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassDirt, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_DIRT; break; }

		// Else, search through grass/sand blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassSand, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_SAND; break; }

		// Else, search through grass/stone blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassStone, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_STONE; break; }

		// Else, search through grass/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_SNOW; break; }

		// Else, search through dirt/sand blend vector
		if (!check) { check = SearchAndDestroy(index, m_dirtSand, terrain); }
		if (check) { current = LANDSCAPE_PAINT::DIRT_SAND; break; }

		// Else, search through dirt/stone blend vector
		if (!check) { check = SearchAndDestroy(index, m_dirtStone, terrain); }
		if (check) { current = LANDSCAPE_PAINT::DIRT_STONE; break; }

		// Else, search through dirt/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_dirtSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::DIRT_SNOW; break; }

		// Else, search through sand/stone blend vector
		if (!check) { check = SearchAndDestroy(index, m_sandStone, terrain); }
		if (check) { current = LANDSCAPE_PAINT::SAND_STONE; break; }

		// Else, search through sand/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_sandSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::SAND_SNOW; break; }

		// Else, search through stone/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_stoneSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::STONE_SNOW; break; }
	}
	break;
	case LANDSCAPE_PAINT::DIRT:
	{
		// Search through grass vector
		check = SearchAndDestroy(index, m_grass, terrain);
		if (check) { current = LANDSCAPE_PAINT::GRASS; break; }

		// Else, search through sand vector
		if (!check) { check = SearchAndDestroy(index, m_sand, terrain); }
		if (check) { current = LANDSCAPE_PAINT::SAND; break; }

		// Else, search through stone vector
		if (!check) { check = SearchAndDestroy(index, m_stone, terrain); }
		if (check) { current = LANDSCAPE_PAINT::STONE; break; }

		// Else, search through snow vector
		if (!check) { check = SearchAndDestroy(index, m_snow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::SNOW; break; }

		// Else, search through grass/dirt blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassDirt, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_DIRT; break; }

		// Else, search through grass/sand blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassSand, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_SAND; break; }

		// Else, search through grass/stone blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassStone, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_STONE; break; }

		// Else, search through grass/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_SNOW; break; }

		// Else, search through dirt/sand blend vector
		if (!check) { check = SearchAndDestroy(index, m_dirtSand, terrain); }
		if (check) { current = LANDSCAPE_PAINT::DIRT_SAND; break; }

		// Else, search through dirt/stone blend vector
		if (!check) { check = SearchAndDestroy(index, m_dirtStone, terrain); }
		if (check) { current = LANDSCAPE_PAINT::DIRT_STONE; break; }

		// Else, search through dirt/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_dirtSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::DIRT_SNOW; break; }

		// Else, search through sand/stone blend vector
		if (!check) { check = SearchAndDestroy(index, m_sandStone, terrain); }
		if (check) { current = LANDSCAPE_PAINT::SAND_STONE; break; }

		// Else, search through sand/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_sandSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::SAND_SNOW; break; }

		// Else, search through stone/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_stoneSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::STONE_SNOW; break; }
	}
	break;
	case LANDSCAPE_PAINT::SAND:
	{
		// Search through grass vector
		check = SearchAndDestroy(index, m_grass, terrain);
		if (check) { current = LANDSCAPE_PAINT::GRASS; break; }

		// Else, search through dirt vector
		if (!check) { check = SearchAndDestroy(index, m_dirt, terrain); }
		if (check) { current = LANDSCAPE_PAINT::DIRT; break; }

		// Else, search through stone vector
		if (!check) { check = SearchAndDestroy(index, m_stone, terrain); }
		if (check) { current = LANDSCAPE_PAINT::STONE; break; }

		// Else, search through snow vector
		if (!check) { check = SearchAndDestroy(index, m_snow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::SNOW; break; }

		// Else, search through grass/dirt blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassDirt, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_DIRT; break; }

		// Else, search through grass/sand blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassSand, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_SAND; break; }

		// Else, search through grass/stone blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassStone, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_STONE; break; }

		// Else, search through grass/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_SNOW; break; }

		// Else, search through dirt/sand blend vector
		if (!check) { check = SearchAndDestroy(index, m_dirtSand, terrain); }
		if (check) { current = LANDSCAPE_PAINT::DIRT_SAND; break; }

		// Else, search through dirt/stone blend vector
		if (!check) { check = SearchAndDestroy(index, m_dirtStone, terrain); }
		if (check) { current = LANDSCAPE_PAINT::DIRT_STONE; break; }

		// Else, search through dirt/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_dirtSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::DIRT_SNOW; break; }

		// Else, search through sand/stone blend vector
		if (!check) { check = SearchAndDestroy(index, m_sandStone, terrain); }
		if (check) { current = LANDSCAPE_PAINT::SAND_STONE; break; }

		// Else, search through sand/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_sandSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::SAND_SNOW; break; }

		// Else, search through stone/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_stoneSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::STONE_SNOW; break; }
	}
	break;
	case LANDSCAPE_PAINT::STONE:
	{
		// Search through grass vector
		check = SearchAndDestroy(index, m_grass, terrain);
		if (check) { current = LANDSCAPE_PAINT::GRASS; break; }

		// Else, search through dirt vector
		if (!check) { check = SearchAndDestroy(index, m_dirt, terrain); }
		if (check) { current = LANDSCAPE_PAINT::DIRT; break; }

		// Else, search through sand vector
		if (!check) { check = SearchAndDestroy(index, m_sand, terrain); }
		if (check) { current = LANDSCAPE_PAINT::SAND; break; }

		// Else, search through snow vector
		if (!check) { check = SearchAndDestroy(index, m_snow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::SNOW; break; }

		// Else, search through grass/dirt blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassDirt, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_DIRT; break; }

		// Else, search through grass/sand blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassSand, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_SAND; break; }

		// Else, search through grass/stone blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassStone, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_STONE; break; }

		// Else, search through grass/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_SNOW; break; }

		// Else, search through dirt/sand blend vector
		if (!check) { check = SearchAndDestroy(index, m_dirtSand, terrain); }
		if (check) { current = LANDSCAPE_PAINT::DIRT_SAND; break; }

		// Else, search through dirt/stone blend vector
		if (!check) { check = SearchAndDestroy(index, m_dirtStone, terrain); }
		if (check) { current = LANDSCAPE_PAINT::DIRT_STONE; break; }

		// Else, search through dirt/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_dirtSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::DIRT_SNOW; break; }

		// Else, search through sand/stone blend vector
		if (!check) { check = SearchAndDestroy(index, m_sandStone, terrain); }
		if (check) { current = LANDSCAPE_PAINT::SAND_STONE; break; }

		// Else, search through sand/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_sandSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::SAND_SNOW; break; }

		// Else, search through stone/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_stoneSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::STONE_SNOW; break; }
	}
	break;
	case LANDSCAPE_PAINT::SNOW:
	{
		// Search through grass vector
		check = SearchAndDestroy(index, m_grass, terrain);
		if (check) { current = LANDSCAPE_PAINT::GRASS; break; }

		// Else, search through dirt vector
		if (!check) { check = SearchAndDestroy(index, m_dirt, terrain); }
		if (check) { current = LANDSCAPE_PAINT::DIRT; break; }

		// Else, search through sand vector
		if (!check) { check = SearchAndDestroy(index, m_sand, terrain); }
		if (check) { current = LANDSCAPE_PAINT::SAND; break; }

		// Else, search through stone vector
		if (!check) { check = SearchAndDestroy(index, m_stone, terrain); }
		if (check) { current = LANDSCAPE_PAINT::STONE; break; }

		// Else, search through grass/dirt blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassDirt, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_DIRT; break; }

		// Else, search through grass/sand blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassSand, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_SAND; break; }

		// Else, search through grass/stone blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassStone, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_STONE; break; }

		// Else, search through grass/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_SNOW; break; }

		// Else, search through dirt/sand blend vector
		if (!check) { check = SearchAndDestroy(index, m_dirtSand, terrain); }
		if (check) { current = LANDSCAPE_PAINT::DIRT_SAND; break; }

		// Else, search through dirt/stone blend vector
		if (!check) { check = SearchAndDestroy(index, m_dirtStone, terrain); }
		if (check) { current = LANDSCAPE_PAINT::DIRT_STONE; break; }

		// Else, search through dirt/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_dirtSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::DIRT_SNOW; break; }

		// Else, search through sand/stone blend vector
		if (!check) { check = SearchAndDestroy(index, m_sandStone, terrain); }
		if (check) { current = LANDSCAPE_PAINT::SAND_STONE; break; }

		// Else, search through sand/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_sandSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::SAND_SNOW; break; }

		// Else, search through stone/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_stoneSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::STONE_SNOW; break; }
	}
	break;
	case LANDSCAPE_PAINT::NA:
	{		
		// Search through grass vector
		check = SearchAndDestroy(index, m_grass, terrain);
		if (check) { current = LANDSCAPE_PAINT::GRASS; break; }

		// Else, search through dirt vector
		if (!check) { check = SearchAndDestroy(index, m_dirt, terrain); }
		if (check) { current = LANDSCAPE_PAINT::DIRT; break; }
		
		// Else, search through sand vector
		if (!check) { check = SearchAndDestroy(index, m_sand, terrain); }
		if (check) { current = LANDSCAPE_PAINT::SAND; break; }

		// Else, search through stone vector
		if (!check) { check = SearchAndDestroy(index, m_stone, terrain); }
		if (check) { current = LANDSCAPE_PAINT::STONE; break; }

		// Else, search through snow vector
		if (!check) { check = SearchAndDestroy(index, m_snow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::SNOW; break; }

		// Else, search through grass/dirt blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassDirt, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_DIRT; break; }

		// Else, search through grass/sand blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassSand, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_SAND; break; }

		// Else, search through grass/stone blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassStone, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_STONE; break; }

		// Else, search through grass/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_grassSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::GRASS_SNOW; break; }

		// Else, search through dirt/sand blend vector
		if (!check) { check = SearchAndDestroy(index, m_dirtSand, terrain); }
		if (check) { current = LANDSCAPE_PAINT::DIRT_SAND; break; }

		// Else, search through dirt/stone blend vector
		if (!check) { check = SearchAndDestroy(index, m_dirtStone, terrain); }
		if (check) { current = LANDSCAPE_PAINT::DIRT_STONE; break; }

		// Else, search through dirt/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_dirtSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::DIRT_SNOW; break; }
		
		// Else, search through sand/stone blend vector
		if (!check) { check = SearchAndDestroy(index, m_sandStone, terrain); }
		if (check) { current = LANDSCAPE_PAINT::SAND_STONE; break; }

		// Else, search through sand/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_sandSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::SAND_SNOW; break; }

		// Else, search through stone/snow blend vector
		if (!check) { check = SearchAndDestroy(index, m_stoneSnow, terrain); }
		if (check) { current = LANDSCAPE_PAINT::STONE_SNOW; break; }
	}
	break;
	}

	// Return current terrain paint
	return current;
}

void DisplayChunk::CheckSurroundings(int row, int column, LANDSCAPE_PAINT paint)
{
	// c   c   c
	//   o o o 
	// c o x o c
	//   o o o 
	// c   c   c

	// Run a check here, in a different function, increase e.g. 'row - 2' goes off the grid
	// maybe replace with '-1' instead and run another check when applying blending texture
	// just to blend it same as the rest if the value is '-1'
	// if the rest is evenly spread, randomly choose ??

	//// Store each 'c' index
	//std::vector<std::pair<int, int>> c; {
	//	c.push_back(std::pair<int, int>(row - 2, column - 2));		// 2 upward, 2 backward
	//	c.push_back(std::pair<int, int>(row - 2, column));			// 2 upward
	//	c.push_back(std::pair<int, int>(row - 2, column + 2));		// 2 upward, 2 forward
	//	c.push_back(std::pair<int, int>(row, column - 2));			// 2 backward
	//	c.push_back(std::pair<int, int>(row, column + 2));			// 2 forward
	//	c.push_back(std::pair<int, int>(row + 2, column - 2));		// 2 downward, 2 backward
	//	c.push_back(std::pair<int, int>(row + 2, column));			// 2 downward
	//	c.push_back(std::pair<int, int>(row + 2, column + 2));		// 2 downward, 2 forward
	//}
	//
	//// Check each 'c' index for their current paints
	//std::vector<LANDSCAPE_PAINT> c_paints;
	//for (int i = 0; i < c.size(); ++i) { 
	//	c_paints.push_back(CheckPaint(c[i].first, c[i].second)); 
	//}
	//
	//// Store each 'o' index
	//std::vector<std::pair<int, int>> o; {	
	//	o.push_back(std::pair<int, int>(row - 1, column - 1));		// 1 upward, 1 backward
	//	o.push_back(std::pair<int, int>(row - 1, column));			// 1 upward
	//	o.push_back(std::pair<int, int>(row - 1, column + 1));		// 1 upward, 1 forward
	//	o.push_back(std::pair<int, int>(row, column - 1));			// 1 backward
	//	o.push_back(std::pair<int, int>(row, column + 1));			// 1 forward
	//	o.push_back(std::pair<int, int>(row + 1, column - 1));		// 1 downward, 1 backward
	//	o.push_back(std::pair<int, int>(row + 1, column));			// 1 downward
	//	o.push_back(std::pair<int, int>(row + 1, column + 1));		// 1 downward, 1 forward
	//}

	//// Check each 'o' index for their current paints
	//std::vector<LANDSCAPE_PAINT> o_paints;
	//for (int i = 0; i < o.size(); ++i) { 
	//	o_paints.push_back(CheckPaint(o[i].first, o[i].second)); 
	//}
	//	
	//// Store 'x' paint
	//LANDSCAPE_PAINT x_paint = CheckPaint(row, column);
	//
	//// Blend each 'o' index depending on 'x' & adjacent 'c' paints
	//for (int i = 0; i < c.size(); ++i)
	//{
	//	// Switch between 'c' paints
	//	switch (c_paints[i])
	//	{
	//	case LANDSCAPE_PAINT::GRASS:
	//	{
	//		// Switch between 'x' paint
	//		switch (x_paint)
	//		{
	//		case LANDSCAPE_PAINT::GRASS: o_paints[i] = LANDSCAPE_PAINT::GRASS; break;
	//		case LANDSCAPE_PAINT::DIRT: o_paints[i] = LANDSCAPE_PAINT::GRASS_AND_DIRT; break;
	//		case LANDSCAPE_PAINT::SAND: o_paints[i] = LANDSCAPE_PAINT::GRASS_AND_SAND; break;
	//		}
	//	}
	//	break;
	//	case LANDSCAPE_PAINT::DIRT:
	//	{
	//		// Switch between 'x' paint
	//		switch (x_paint)
	//		{
	//		case LANDSCAPE_PAINT::GRASS: o_paints[i] = LANDSCAPE_PAINT::GRASS_AND_DIRT; break;
	//		case LANDSCAPE_PAINT::DIRT: o_paints[i] = LANDSCAPE_PAINT::DIRT; break;
	//		case LANDSCAPE_PAINT::SAND: o_paints[i] = LANDSCAPE_PAINT::GRASS_AND_SAND; break;
	//		}
	//	}
	//	break;
	//	case LANDSCAPE_PAINT::SAND:
	//	{
	//		// Switch between 'x' paint
	//		switch (x_paint)
	//		{
	//		case LANDSCAPE_PAINT::GRASS: o_paints[i] = LANDSCAPE_PAINT::GRASS_AND_SAND; break;
	//		case LANDSCAPE_PAINT::DIRT: o_paints[i] = LANDSCAPE_PAINT::DIRT_AND_SAND; break;
	//		case LANDSCAPE_PAINT::SAND: o_paints[i] = LANDSCAPE_PAINT::SAND; break;
	//		}
	//	}
	//	break;
	//	}
	//}

	//// Loop through blended 'o' indexes
	//for (int i = 0; i < o.size(); ++i)
	//{
	//	// Remove any duplicates
	//	CheckForDuplicates(o[i].first, o[i].second, o_paints[i]);

	//	// Switch between blended paint
	//	switch (o_paints[i])
	//	{
	//	case LANDSCAPE_PAINT::GRASS: m_grass.push_back(std::pair<int, int>(o[i].first, o[i].second)); break;
	//	case LANDSCAPE_PAINT::DIRT: m_dirt.push_back(std::pair<int, int>(o[i].first, o[i].second)); break;
	//	case LANDSCAPE_PAINT::SAND: m_sand.push_back(std::pair<int, int>(o[i].first, o[i].second)); break;
	//	case LANDSCAPE_PAINT::GRASS_AND_DIRT: m_grassAndDirt.push_back(std::pair<int, int>(o[i].first, o[i].second)); break;
	//	case LANDSCAPE_PAINT::GRASS_AND_SAND: m_grassAndSand.push_back(std::pair<int, int>(o[i].first, o[i].second)); break;
	//	case LANDSCAPE_PAINT::DIRT_AND_SAND: m_dirtAndSand.push_back(std::pair<int, int>(o[i].first, o[i].second)); break;
	//	case LANDSCAPE_PAINT::NA: m_default.push_back(std::pair<int, int>(o[i].first, o[i].second)); break;
	//	}
	//}
}

LANDSCAPE_PAINT DisplayChunk::CheckPaint(int row, int column)
{
	// Temp
	//int index = -1;
	//std::pair<int, int> terrain;
	//terrain.first = row;
	//terrain.second = column;

	//// Loop through grass vector & remove from storage
	//if (FindInVector(index, m_grass, terrain)) { return LANDSCAPE_PAINT::GRASS; }

	//// Else, loop through dirt vector & return paint
	//else if (FindInVector(index, m_dirt, terrain)) { return LANDSCAPE_PAINT::DIRT; }

	//// Else, loop through sand vector & return paint
	//else if (FindInVector(index, m_sand, terrain)) { return LANDSCAPE_PAINT::SAND; }

	//// Else, loop through grass and dirt vector & return paint
	//else if (FindInVector(index, m_grassAndDirt, terrain)) { return LANDSCAPE_PAINT::GRASS_AND_DIRT; }

	//// Else, loop through grass and sand vector & return paint
	//else if (FindInVector(index, m_grassAndSand, terrain)) { return LANDSCAPE_PAINT::GRASS_AND_SAND; }

	//// Else, loop through dirt and sand vector & return paint
	//else if (FindInVector(index, m_dirtAndSand, terrain)) { return LANDSCAPE_PAINT::DIRT_AND_SAND; }

	//// Else, return default paint
	//else { return LANDSCAPE_PAINT::NA; }

	return LANDSCAPE_PAINT::NA;
}

void DisplayChunk::SavePaint(std::string path, std::vector<std::pair<int, int>> vector)
{
	// Create output filestream object
	std::ofstream file(path);
	
	// Loop through terrain
	for (int i = 0; i < vector.size(); ++i)
	{
		// Store row, column & paint
		file << vector[i].first << ",";
		file << vector[i].second << "\n";
	}

	// Close file
	file.close();
}

void DisplayChunk::ReadPaints(std::string path, std::vector<std::pair<int, int>> &vector)
{
	// Create input filestream
	std::ifstream file(path);

	// Ensure file is open
	if (!file.is_open()) { MessageBox(NULL, L"Can't Find The Paint File!", L"Error", MB_OK); }

	std::vector<std::string> lines;
	std::string line;
	int val;
	
	// Read data, line by line
	while (std::getline(file, line))
	{
		// Create stringstream of current line
		std::stringstream ss(line);

		lines.push_back(line);

		//// Keep track of current geometry
		//int ID = 0;

		//// Extract each integer
		//while (ss >> val)
		//{
		//	// Add current integer to vector
		//	vector.at(ID).first = val;

		//	// If next token is a point, ignore and carry on
		//	if (ss.peek() == ',') { ss.ignore(); }
		//	else 
		//	{
		//		// Add next integer
		//		vector.at(ID).second = val;
		//	}

		//	// If next token isn't a comma, add both integers together
		//	if (ss.peek() != ',') {  }
		//}
	}
	
	// Loop through lines
	for (int i = 0; i < lines.size(); ++i)
	{
		std::string one, two;
		bool comma = false;
		
		// Loop through individual line size
		for (int j = 0; j < lines[i].size(); ++j)
		{			
			// If there hasn't been a comma
			if (!comma)
			{
				// If token isn't a comma
				if (lines[i][j] != ',')
				{
					// Add to temp
					one += lines[i][j];
				}
				else { comma = true; }
			}

			// If there has been a comma			
			if (comma)
			{
				// If token isn't a comma
				if (lines[i][j] != ',')
				{
					// Add to temp
					two += lines[i][j];
				}
			}			
		}

		// Add to vector
		std::pair<int, int> pair;
		pair.first = std::stoi(one);
		pair.second = std::stoi(two);
		vector.push_back(pair);
	}
}

void DisplayChunk::ReadAllPaints()
{
	// Basic paints
	ReadPaints("database/CSV/grass.csv", m_grass);
	ReadPaints("database/CSV/dirt.csv", m_dirt);
	ReadPaints("database/CSV/sand.csv", m_sand);
	ReadPaints("database/CSV/stone.csv", m_stone);
	ReadPaints("database/CSV/snow.csv", m_snow);

	// Blend paints
	ReadPaints("database/CSV/grassDirt.csv", m_grassDirt);
	ReadPaints("database/CSV/grassSand.csv", m_grassSand);
	ReadPaints("database/CSV/grassStone.csv", m_grassStone);
	ReadPaints("database/CSV/grassSnow.csv", m_grassSnow);
	ReadPaints("database/CSV/dirtSand.csv", m_dirtSand);
	ReadPaints("database/CSV/dirtStone.csv", m_dirtStone);
	ReadPaints("database/CSV/dirtSnow.csv", m_dirtSnow);
	ReadPaints("database/CSV/sandStone.csv", m_sandStone);
	ReadPaints("database/CSV/sandSnow.csv", m_sandSnow);
	ReadPaints("database/CSV/stoneSnow.csv", m_stoneSnow);
}

void DisplayChunk::SaveAllPaints()
{
	SavePaint("database/CSV/grass.csv", m_grass);
	SavePaint("database/CSV/dirt.csv", m_dirt);
	SavePaint("database/CSV/sand.csv", m_sand);
	SavePaint("database/CSV/stone.csv", m_stone);
	SavePaint("database/CSV/snow.csv", m_snow);
						
	SavePaint("database/CSV/grassDirt.csv", m_grassDirt);
	SavePaint("database/CSV/grassSand.csv", m_grassSand);
	SavePaint("database/CSV/grassStone.csv", m_grassStone);
	SavePaint("database/CSV/grassSnow.csv", m_grassSnow);
	SavePaint("database/CSV/dirtSand.csv", m_dirtSand);
	SavePaint("database/CSV/dirtStone.csv", m_dirtStone);
	SavePaint("database/CSV/dirtSnow.csv", m_dirtSnow);
	SavePaint("database/CSV/sandStone.csv", m_sandStone);
	SavePaint("database/CSV/sandSnow.csv", m_sandSnow);
	SavePaint("database/CSV/stoneSnow.csv", m_stoneSnow);
}

void DisplayChunk::DrawBlends(std::shared_ptr<DX::DeviceResources> deviceResources, std::vector<Light*> lights)
{
	// Setup device context
	auto context = deviceResources->GetD3DDeviceContext();
	context->IASetInputLayout(m_terrainInputLayout.Get());

	// Draw all grass/dirt geometry
	if (m_grassDirt.size() != 0) {
		m_batch->Begin();
		ShaderManager::Shader(SHADER_TYPE::TOON_BLEND, context, lights, m_texture_splat_1, m_texture_splat_2);
		DrawTerrain(m_grassDirt);
		m_batch->End();
	}

	// Draw all grass/sand geometry
	if (m_grassSand.size() != 0) {
		m_batch->Begin();
		ShaderManager::Shader(SHADER_TYPE::TOON_BLEND, context, lights, m_texture_splat_1, m_texture_splat_3);
		DrawTerrain(m_grassSand);
		m_batch->End();
	}

	// Draw all grass/stone geometry
	if (m_grassStone.size() != 0) {
		m_batch->Begin();
		ShaderManager::Shader(SHADER_TYPE::TOON_BLEND, context, lights, m_texture_splat_1, m_texture_splat_4);
		DrawTerrain(m_grassStone);
		m_batch->End();
	}

	// Draw all grass/snow geometry
	if (m_grassSnow.size() != 0) {
		m_batch->Begin();
		ShaderManager::Shader(SHADER_TYPE::TOON_BLEND, context, lights, m_texture_splat_1, m_texture_splat_5);
		DrawTerrain(m_grassSnow);
		m_batch->End();
	}

	// Draw all dirt/sand geometry
	if (m_dirtSand.size() != 0) {
		m_batch->Begin();
		ShaderManager::Shader(SHADER_TYPE::TOON_BLEND, context, lights, m_texture_splat_2, m_texture_splat_3);
		DrawTerrain(m_dirtSand);
		m_batch->End();
	}

	// Draw all dirt/stone geometry
	if (m_dirtStone.size() != 0) {
		m_batch->Begin();
		ShaderManager::Shader(SHADER_TYPE::TOON_BLEND, context, lights, m_texture_splat_2, m_texture_splat_4);
		DrawTerrain(m_dirtStone);
		m_batch->End();
	}

	// Draw all dirt/snow geometry
	if (m_dirtSnow.size() != 0) {
		m_batch->Begin();
		ShaderManager::Shader(SHADER_TYPE::TOON_BLEND, context, lights, m_texture_splat_2, m_texture_splat_5);
		DrawTerrain(m_dirtSnow);
		m_batch->End();
	}

	// Draw all sand/stone geometry
	if (m_sandStone.size() != 0) {
		m_batch->Begin();
		ShaderManager::Shader(SHADER_TYPE::TOON_BLEND, context, lights, m_texture_splat_3, m_texture_splat_4);
		DrawTerrain(m_sandStone);
		m_batch->End();
	}

	// Draw all sand/snow geometry
	if (m_sandSnow.size() != 0) {
		m_batch->Begin();
		ShaderManager::Shader(SHADER_TYPE::TOON_BLEND, context, lights, m_texture_splat_3, m_texture_splat_5);
		DrawTerrain(m_sandSnow);
		m_batch->End();
	}

	// Draw all stone/snow geometry
	if (m_stoneSnow.size() != 0) {
		m_batch->Begin();
		ShaderManager::Shader(SHADER_TYPE::TOON_BLEND, context, lights, m_texture_splat_4, m_texture_splat_5);
		DrawTerrain(m_stoneSnow);
		m_batch->End();
	}
}

void DisplayChunk::CalculateTerrainNormals()
{
	int index1, index2, index3, index4;
	DirectX::SimpleMath::Vector3 upDownVector, leftRightVector, normalVector;

	for (int i = 0; i<(TERRAINRESOLUTION - 1); i++)
	{
		for (int j = 0; j<(TERRAINRESOLUTION - 1); j++)
		{
			upDownVector.x = (m_terrainGeometry[i + 1][j].position.x - m_terrainGeometry[i - 1][j].position.x);
			upDownVector.y = (m_terrainGeometry[i + 1][j].position.y - m_terrainGeometry[i - 1][j].position.y);
			upDownVector.z = (m_terrainGeometry[i + 1][j].position.z - m_terrainGeometry[i - 1][j].position.z);

			leftRightVector.x = (m_terrainGeometry[i][j - 1].position.x - m_terrainGeometry[i][j + 1].position.x);
			leftRightVector.y = (m_terrainGeometry[i][j - 1].position.y - m_terrainGeometry[i][j + 1].position.y);
			leftRightVector.z = (m_terrainGeometry[i][j - 1].position.z - m_terrainGeometry[i][j + 1].position.z);
			
			leftRightVector.Cross(upDownVector, normalVector);	//get cross product
			normalVector.Normalize();			//normalise it.

			m_terrainGeometry[i][j].normal = normalVector;	//set the normal for this point based on our result
		}
	}
}

bool DisplayChunk::FindInVector(int & index, std::vector<std::pair<int, int>> vector, std::pair<int, int> terrain)
{
	// Loop through vector
	for (int i = 0; i < vector.size(); ++i)
	{
		// If current row,column matches selected row,column
		if (vector[i].first == terrain.first &&
			vector[i].second == terrain.second)
		{
			// Set index
			index = i;

			// Return true if found
			return true;
		}
	}

	// Return false is not found
	return false;
}

bool DisplayChunk::SearchAndDestroy(int & index, std::vector<std::pair<int, int>> &vector, std::pair<int, int> terrain)
{
	// Search through vector
	if (FindInVector(index, vector, terrain))
	{
		// Remove from storage
		vector.erase(vector.begin() + index);

		return true;
	}
	else { return false; }
}

bool DisplayChunk::SearchForPaint(std::vector<std::pair<int, int>> vector, std::pair<int, int> terrain)
{
	// Setup temp index
	int index = 0.f;

	// Search through vector
	return FindInVector(index, vector, terrain);
}
