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

void DisplayChunk::Wave(float deltaTime)
{
	/*for (int i = 0; i < TERRAINRESOLUTION - 1; ++i)
	{
		for (int j = 0; j < TERRAINRESOLUTION - 1; ++j)
		{			
			m_terrainGeometry[i][j].position.y = sin(m_terrainGeometry[i][j].position.x + (deltaTime * 100.f));

			m_terrainGeometry[i][j].normal.x = 1 - cos(m_terrainGeometry[i][j].position.x + (deltaTime * 100.f));
			m_terrainGeometry[i][j].normal.y = abs(cos(m_terrainGeometry[i][j].position.x + (deltaTime * 100.f)));

			///m_terrainGeometry[i][j].position = DirectX::
		}
	}*/

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Read #1 and #2 of the Lab >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	////////////////////////////////////////////////////////////////////////////////////////////////////
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

void DisplayChunk::RenderBatch(std::shared_ptr<DX::DeviceResources> deviceResources)
{	
	// Setup device context
	auto context = deviceResources->GetD3DDeviceContext();
	///auto sampler = m_states->LinearClamp();
	///context->PSSetSamplers(0, 1, &sampler);		
	context->IASetInputLayout(m_terrainInputLayout.Get());
	/*ID3D11ShaderResourceView * textures[2];
	textures[1] = m_texture_splat_1;
	textures[2] = m_texture_splat_2;
	context->PSSetShaderResources(0, 2, textures);*/

	// Draw all grass geometry
	if (m_grass.size() != 0) {
		m_batch->Begin();
		m_terrainEffect->SetTexture(m_texture_splat_1);
		m_terrainEffect->Apply(context);
		///m_dualEffect->Apply(context);
		DrawTerrain(m_grass);
		m_batch->End();
	}

	// Draw all dirt geometry
	if (m_dirt.size() != 0) {
		m_batch->Begin();
		m_terrainEffect->SetTexture(m_texture_splat_2);
		m_terrainEffect->Apply(context);
		DrawTerrain(m_dirt);
		m_batch->End();
	}

	// Draw all sand geometry
	if (m_sand.size() != 0)	{
		m_batch->Begin();
		m_terrainEffect->SetTexture(m_texture_splat_3);
		m_terrainEffect->Apply(context);
		DrawTerrain(m_sand);
		m_batch->End();
	}

	// Draw all blended grass/dirt geometry
	if (m_grassAndDirt.size() != 0) {
		m_batch->Begin();
		/*m_terrainBlendOne->SetTexture(m_texture_splat_1);
		m_terrainBlendOne->Apply(context);
		m_terrainBlendTwo->SetTexture(m_texture_splat_2);
		m_terrainBlendTwo->Apply(context);*/
		m_terrainEffect->SetTexture(m_texture_blend_1_2);
		m_terrainEffect->Apply(context);
		DrawTerrain(m_grassAndDirt);
		m_batch->End();
	}

	// Draw all blended grass/sand geometry
	if (m_grassAndSand.size() != 0) {
		m_batch->Begin();
		m_terrainEffect->SetTexture(m_texture_blend_1_3);
		m_terrainEffect->Apply(context);
		DrawTerrain(m_grassAndSand);
		m_batch->End();
	}
	
	// Draw all blended dirt/sand geometry
	if (m_dirtAndSand.size() != 0) {
		m_batch->Begin();
		m_terrainEffect->SetTexture(m_texture_blend_2_3);
		m_terrainEffect->Apply(context);
		DrawTerrain(m_dirtAndSand);
		m_batch->End();
	}

	// Draw all default geometry
	if (m_default.size() != 0) {
		m_batch->Begin();
		m_terrainEffect->SetTexture(m_texture_default);
		m_terrainEffect->Apply(context);
		DrawTerrain(m_default);	
		m_batch->End();
	}
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

	// Load first splat texture
	std::wstring splat_1 = StringToWCHART(m_tex_splat_1_path);
	HRESULT rs1;
	rs1 = CreateDDSTextureFromFile(device, splat_1.c_str(), NULL, &m_texture_splat_1);

	//////////////////////////////////////////////////////////////////////////////////////////

	// Load second splat texture
	std::wstring splat_2 = StringToWCHART(m_tex_splat_2_path);
	HRESULT rs2;
	rs2 = CreateDDSTextureFromFile(device, splat_2.c_str(), NULL, &m_texture_splat_2);

	//////////////////////////////////////////////////////////////////////////////////////////

	// Load third splat texture
	std::wstring splat_3 = StringToWCHART(m_tex_splat_3_path);
	HRESULT rs3;
	rs3 = CreateDDSTextureFromFile(device, splat_3.c_str(), NULL, &m_texture_splat_3);

	//////////////////////////////////////////////////////////////////////////////////////////

	// Load first blend texture
	std::wstring blend_1 = StringToWCHART("database/data/grassDirt.dds");
	HRESULT rsb1;
	rsb1 = CreateDDSTextureFromFile(device, blend_1.c_str(), NULL, &m_texture_blend_1_2);

	//////////////////////////////////////////////////////////////////////////////////////////

	// Load second blend texture
	std::wstring blend_2 = StringToWCHART("database/data/grassSand.dds");
	HRESULT rsb2;
	rsb2 = CreateDDSTextureFromFile(device, blend_1.c_str(), NULL, &m_texture_blend_1_3);

	//////////////////////////////////////////////////////////////////////////////////////////

	// Load first blend texture
	std::wstring blend_3 = StringToWCHART("database/data/dirtSand.dds");
	HRESULT rsb3;
	rsb3 = CreateDDSTextureFromFile(device, blend_1.c_str(), NULL, &m_texture_blend_2_3);
	
	//////////////////////////////////////////////////////////////////////////////////////////

	//load the default texture
	std::wstring diffuse = StringToWCHART(m_tex_diffuse_path);
	HRESULT rs0;
	rs0 = CreateDDSTextureFromFile(device, diffuse.c_str(), NULL, &m_texture_default);	//load tex into Shader resource	view and resource
																							
	//////////////////////////////////////////////////////////////////////////////////////////

	// Load normal map
	std::wstring normal = StringToWCHART("database/data/normalMap.dds");
	HRESULT rsn;
	rsn = CreateDDSTextureFromFile(device, normal.c_str(), NULL, &m_normalMap);

	//////////////////////////////////////////////////////////////////////////////////////////

	// Setup normal map effect
	///m_normalMapEffect = std::make_unique<NormalMapEffect>(device);
	///m_normalMapEffect->SetTexture(m_normalMap.Get());

	//////////////////////////////////////////////////////////////////////////////////////////

	// Setup state
	///m_states = std::make_unique<CommonStates>(device);

	//////////////////////////////////////////////////////////////////////////////////////////

	// Setup dual texture effect
	m_dualEffect = std::make_unique<DualTextureEffect>(device);
	m_dualEffect->SetTexture(m_texture_splat_1);
	m_dualEffect->SetTexture2(m_texture_splat_2);

	//////////////////////////////////////////////////////////////////////////////////////////

	// Setup terrain effect
	///m_terrainEffect = std::make_unique<NormalMapEffect>(device);
	m_terrainEffect = std::make_unique<BasicEffect>(device);
	m_terrainEffect->EnableDefaultLighting();
	///m_terrainEffect->SetLightDiffuseColor(0, Colors::Gray);
	m_terrainEffect->SetLightingEnabled(true);
	m_terrainEffect->SetTextureEnabled(true);
	m_terrainEffect->SetTexture(m_texture_default);
	///m_terrainEffect->SetNormalTexture(m_normalMap);

	//////////////////////////////////////////////////////////////////////////////////////////

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_terrainEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	//setup batch
	DX::ThrowIfFailed(
		device->CreateInputLayout(VertexPositionNormalTexture::InputElements,
			VertexPositionNormalTexture::InputElementCount,
			shaderByteCode,
			byteCodeLength,
			m_terrainInputLayout.GetAddressOf())
		);

	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormalTexture>>(deviceContext);	

	//////////////////////////////////////////////////////////////////////////////////////////

	// Setup terrain blends
	/*m_terrainBlendOne = std::make_unique<BasicEffect>(device);
	m_terrainBlendOne->EnableDefaultLighting();
	m_terrainBlendOne->SetLightingEnabled(true);
	m_terrainBlendOne->SetTextureEnabled(true);
	m_terrainBlendOne->SetAlpha(.5f);

	m_terrainBlendTwo = std::make_unique<BasicEffect>(device);
	m_terrainBlendTwo->EnableDefaultLighting();
	m_terrainBlendTwo->SetLightingEnabled(true);
	m_terrainBlendTwo->SetTextureEnabled(true);
	m_terrainBlendTwo->SetAlpha(.5f);*/
}

void DisplayChunk::SaveHeightMap()
{
/*	for (size_t i = 0; i < TERRAINRESOLUTION*TERRAINRESOLUTION; i++)
	{
		m_heightMap[i] = 0;
	}*/

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
	//insert how YOU want to update the heightmap here! :D


}

void DisplayChunk::PaintTerrain(int i, int j, LANDSCAPE_PAINT paint, bool checkSurroundings)
{	
	// Check if selected geometry is within other containers & remove
	CheckForDuplicates(i, j, paint);

	// Blend surrounding geometry paints
	///if (checkSurroundings) { CheckSurroundings(i, j, paint); }

	// Switch between paints
	switch (paint)
	{
	case LANDSCAPE_PAINT::GRASS: m_grass.push_back(std::pair<int, int>(i, j)); break;
	case LANDSCAPE_PAINT::DIRT: m_dirt.push_back(std::pair<int, int>(i, j)); break;
	case LANDSCAPE_PAINT::SAND: m_sand.push_back(std::pair<int, int>(i, j)); break;
	case LANDSCAPE_PAINT::NA: m_default.push_back(std::pair<int, int>(i, j)); break;
	}
}

void DisplayChunk::SculptTerrain(int row, int column, LANDSCAPE_SCULPT sculpt, LANDSCAPE_CONSTRAINT constraint, std::vector<DirectX::SimpleMath::Vector3> position)
{
	// Store starting vertex
	//Vector3 start = m_terrainGeometry[row][column].position; 

	//// Store other vertices of quad
	//std::vector<Vector3> others;
	//others.push_back(m_terrainGeometry[row][column + 1].position);
	//others.push_back(m_terrainGeometry[row + 1][column + 1].position);
	//others.push_back(m_terrainGeometry[row + 1][column].position);

	//// Store distances between start and other vertices
	//std::vector<float> distances;
	//for (int i = 0; i < others.size(); ++i)
	//{
	//	float dist = sqrt(
	//		(start.x - others[i].x) * (start.x - others[i].x) +
	//		(start.y - others[i].y) * (start.y - others[i].y) +
	//		(start.z - others[i].z) * (start.z - others[i].z));
	//	distances.push_back(dist);
	//}
	//	
	//// Loop through distances
	//for (int i = 0; i < distances.size(); ++i)
	//{
	//	// Divide each distance to put in 0,1 interval
	//	distances[i] = (distances[i] * 2.f) - 1.f;

	//	// Plug into cosine
	//	distances[i] = sin(distances[i]);

	//	///cos(m_terrainGeometry[i][j].position.x + (deltaTime * 100.f))
	//}

	//// Test
	//if (sculpt == LANDSCAPE_SCULPT::INCREASE)
	//{
	//	switch (constraint)
	//	{
	//	case LANDSCAPE_CONSTRAINT::Y:
	//	{
	//		// Increase position
	//		m_terrainGeometry[row][column].position.y += 1.f;
	//		m_terrainGeometry[row][column + 1].position.y += distances[0];
	//		m_terrainGeometry[row + 1][column + 1].position.y += distances[1];
	//		m_terrainGeometry[row + 1][column].position.y += distances[2];
	//	}
	//	break;
	//	}
	//}
	
	// Switch between sculpt parameter
	switch (sculpt)
	{
	case LANDSCAPE_SCULPT::INCREASE:
	{
		// Switch between constraint
		switch (constraint)
		{
		case LANDSCAPE_CONSTRAINT::XY:
		{			
			// Increase position
			m_terrainGeometry[row][column].position.x += 1.f;
			m_terrainGeometry[row][column + 1].position.x += 1.f;
			m_terrainGeometry[row + 1][column + 1].position.x += 1.f;
			m_terrainGeometry[row + 1][column].position.x += 1.f;


			m_terrainGeometry[row][column].position.y += 1.f;
			m_terrainGeometry[row][column + 1].position.y += 1.f;
			m_terrainGeometry[row + 1][column + 1].position.y += 1.f;
			m_terrainGeometry[row + 1][column].position.y += 1.f;
		}
		break;
		case LANDSCAPE_CONSTRAINT::XZ:
		{
			// Increase position
			m_terrainGeometry[row][column].position.x += 1.f;
			m_terrainGeometry[row][column + 1].position.x += 1.f;
			m_terrainGeometry[row + 1][column + 1].position.x += 1.f;
			m_terrainGeometry[row + 1][column].position.x += 1.f;


			m_terrainGeometry[row][column].position.z += 1.f;
			m_terrainGeometry[row][column + 1].position.y += 1.f;
			m_terrainGeometry[row + 1][column + 1].position.y += 1.f;
			m_terrainGeometry[row + 1][column].position.y += 1.f;
		}
		break;
		case LANDSCAPE_CONSTRAINT::YZ:
		{
			// Increase position
			m_terrainGeometry[row][column].position.y += 1.f;
			m_terrainGeometry[row][column + 1].position.y += 1.f;
			m_terrainGeometry[row + 1][column + 1].position.y += 1.f;
			m_terrainGeometry[row + 1][column].position.y += 1.f;


			m_terrainGeometry[row][column].position.z += 1.f;
			m_terrainGeometry[row][column + 1].position.z += 1.f;
			m_terrainGeometry[row + 1][column + 1].position.z += 1.f;
			m_terrainGeometry[row + 1][column].position.z += 1.f;
		}
		break;
		case LANDSCAPE_CONSTRAINT::X:
		{
			// Increase position
			m_terrainGeometry[row][column].position.x += 1.f;
			m_terrainGeometry[row][column + 1].position.x += 1.f;
			m_terrainGeometry[row + 1][column + 1].position.x += 1.f;
			m_terrainGeometry[row + 1][column].position.x += 1.f;
		}
		break;
		case LANDSCAPE_CONSTRAINT::Y:
		{
			// Increase position
			m_terrainGeometry[row][column].position.y += 1.f;
			m_terrainGeometry[row][column + 1].position.y += 1.f;
			m_terrainGeometry[row + 1][column + 1].position.y += 1.f;
			m_terrainGeometry[row + 1][column].position.y += 1.f;
		}
		break;
		case LANDSCAPE_CONSTRAINT::Z:
		{
			// Increase position
			m_terrainGeometry[row][column].position.z += 1.f;
			m_terrainGeometry[row][column + 1].position.z += 1.f;
			m_terrainGeometry[row + 1][column + 1].position.z += 1.f;
			m_terrainGeometry[row + 1][column].position.z += 1.f;
		}
		break;
		case LANDSCAPE_CONSTRAINT::ALL:
		{
			// Increase position
			m_terrainGeometry[row][column].position.x += 1.f;
			m_terrainGeometry[row][column + 1].position.x += 1.f;
			m_terrainGeometry[row + 1][column + 1].position.x += 1.f;
			m_terrainGeometry[row + 1][column].position.x += 1.f;


			m_terrainGeometry[row][column].position.y += 1.f;
			m_terrainGeometry[row][column + 1].position.y += 1.f;
			m_terrainGeometry[row + 1][column + 1].position.y += 1.f;
			m_terrainGeometry[row + 1][column].position.y += 1.f;


			m_terrainGeometry[row][column].position.z += 1.f;
			m_terrainGeometry[row][column + 1].position.z += 1.f;
			m_terrainGeometry[row + 1][column + 1].position.z += 1.f;
			m_terrainGeometry[row + 1][column].position.z += 1.f;
		}
		break;
		}
	}
	break;
	case LANDSCAPE_SCULPT::DECREASE:
	{
		// Switch between constraint
		switch (constraint)
		{
		case LANDSCAPE_CONSTRAINT::XY:
		{
			// Decrease position
			m_terrainGeometry[row][column].position.x -= 1.f;
			m_terrainGeometry[row][column + 1].position.x -= 1.f;
			m_terrainGeometry[row + 1][column + 1].position.x -= 1.f;
			m_terrainGeometry[row + 1][column].position.x -= 1.f;


			m_terrainGeometry[row][column].position.y -= 1.f;
			m_terrainGeometry[row][column + 1].position.y -= 1.f;
			m_terrainGeometry[row + 1][column + 1].position.y -= 1.f;
			m_terrainGeometry[row + 1][column].position.y -= 1.f;
		}
		break;
		case LANDSCAPE_CONSTRAINT::XZ:
		{
			// Decrease position
			m_terrainGeometry[row][column].position.x -= 1.f;
			m_terrainGeometry[row][column + 1].position.x -= 1.f;
			m_terrainGeometry[row + 1][column + 1].position.x -= 1.f;
			m_terrainGeometry[row + 1][column].position.x -= 1.f;

			m_terrainGeometry[row][column].position.z -= 1.f;
			m_terrainGeometry[row][column + 1].position.z -= 1.f;
			m_terrainGeometry[row + 1][column + 1].position.z -= 1.f;
			m_terrainGeometry[row + 1][column].position.z -= 1.f;
		}
		break;
		case LANDSCAPE_CONSTRAINT::YZ:
		{
			// Decrease position
			m_terrainGeometry[row][column].position.y -= 1.f;
			m_terrainGeometry[row][column + 1].position.y -= 1.f;
			m_terrainGeometry[row + 1][column + 1].position.y -= 1.f;
			m_terrainGeometry[row + 1][column].position.y -= 1.f;

			m_terrainGeometry[row][column].position.z -= 1.f;
			m_terrainGeometry[row][column + 1].position.z -= 1.f;
			m_terrainGeometry[row + 1][column + 1].position.z -= 1.f;
			m_terrainGeometry[row + 1][column].position.z -= 1.f;
		}
		break;
		case LANDSCAPE_CONSTRAINT::X:
		{
			// Decrease position
			m_terrainGeometry[row][column].position.x -= 1.f;
			m_terrainGeometry[row][column + 1].position.x -= 1.f;
			m_terrainGeometry[row + 1][column + 1].position.x -= 1.f;
			m_terrainGeometry[row + 1][column].position.x -= 1.f;
		}
		break;
		case LANDSCAPE_CONSTRAINT::Y:
		{
			// Decrease position
			m_terrainGeometry[row][column].position.y -= 1.f;
			m_terrainGeometry[row][column + 1].position.y -= 1.f;
			m_terrainGeometry[row + 1][column + 1].position.y -= 1.f;
			m_terrainGeometry[row + 1][column].position.y -= 1.f;
		}
		break;
		case LANDSCAPE_CONSTRAINT::Z:
		{
			// Decrease position
			m_terrainGeometry[row][column].position.z -= 1.f;
			m_terrainGeometry[row][column + 1].position.z -= 1.f;
			m_terrainGeometry[row + 1][column + 1].position.z -= 1.f;
			m_terrainGeometry[row + 1][column].position.z -= 1.f;
		}
		break;
		case LANDSCAPE_CONSTRAINT::ALL:
		{
			// Decrease position
			m_terrainGeometry[row][column].position.x -= 1.f;
			m_terrainGeometry[row][column + 1].position.x -= 1.f;
			m_terrainGeometry[row + 1][column + 1].position.x -= 1.f;
			m_terrainGeometry[row + 1][column].position.x -= 1.f;


			m_terrainGeometry[row][column].position.y -= 1.f;
			m_terrainGeometry[row][column + 1].position.y -= 1.f;
			m_terrainGeometry[row + 1][column + 1].position.y -= 1.f;
			m_terrainGeometry[row + 1][column].position.y -= 1.f;


			m_terrainGeometry[row][column].position.z -= 1.f;
			m_terrainGeometry[row][column + 1].position.z -= 1.f;
			m_terrainGeometry[row + 1][column + 1].position.z -= 1.f;
			m_terrainGeometry[row + 1][column].position.z -= 1.f;
		}
		break;
		}
	}
	break;
	case LANDSCAPE_SCULPT::FLATTEN:
	{
		// Switch between constraint
		switch (constraint)
		{
		case LANDSCAPE_CONSTRAINT::XY:
		{
			// X
			{
				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row][column].position.x > position[0].x + 1.f) { m_terrainGeometry[row][column].position.x -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row][column].position.x < position[0].x - 1.f) { m_terrainGeometry[row][column].position.x += 1.f; }
				
				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row][column + 1].position.x > position[1].x + 1.f) { m_terrainGeometry[row][column + 1].position.x -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row][column + 1].position.x < position[1].x - 1.f) { m_terrainGeometry[row][column + 1].position.x += 1.f; }
				
				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row + 1][column + 1].position.x > position[2].x + 1.f) { m_terrainGeometry[row + 1][column + 1].position.x -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row + 1][column + 1].position.x < position[2].x - 1.f) { m_terrainGeometry[row + 1][column + 1].position.x += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row + 1][column].position.x > position[3].x + 1.f) { m_terrainGeometry[row + 1][column].position.x -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row + 1][column].position.x < position[3].x - 1.f) { m_terrainGeometry[row + 1][column].position.x += 1.f; }
			}

			// Y
			{
				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row][column].position.y > position[0].y + 1.f) { m_terrainGeometry[row][column].position.y -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row][column].position.y < position[0].y - 1.f) { m_terrainGeometry[row][column].position.y += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row][column + 1].position.y > position[1].y + 1.f) { m_terrainGeometry[row][column + 1].position.y -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row][column + 1].position.y < position[1].y - 1.f) { m_terrainGeometry[row][column + 1].position.y += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row + 1][column + 1].position.y > position[2].y + 1.f) { m_terrainGeometry[row + 1][column + 1].position.y -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row + 1][column + 1].position.y < position[2].y - 1.f) { m_terrainGeometry[row + 1][column + 1].position.y += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row + 1][column].position.y > position[3].y + 1.f) { m_terrainGeometry[row + 1][column].position.y -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row + 1][column].position.y < position[3].y - 1.f) { m_terrainGeometry[row + 1][column].position.y += 1.f; }
			}
		}
		break;
		case LANDSCAPE_CONSTRAINT::XZ:
		{
			// X
			{
				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row][column].position.x > position[0].x + 1.f) { m_terrainGeometry[row][column].position.x -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row][column].position.x < position[0].x - 1.f) { m_terrainGeometry[row][column].position.x += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row][column + 1].position.x > position[1].x + 1.f) { m_terrainGeometry[row][column + 1].position.x -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row][column + 1].position.x < position[1].x - 1.f) { m_terrainGeometry[row][column + 1].position.x += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row + 1][column + 1].position.x > position[2].x + 1.f) { m_terrainGeometry[row + 1][column + 1].position.x -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row + 1][column + 1].position.x < position[2].x - 1.f) { m_terrainGeometry[row + 1][column + 1].position.x += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row + 1][column].position.x > position[3].x + 1.f) { m_terrainGeometry[row + 1][column].position.x -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row + 1][column].position.x < position[3].x - 1.f) { m_terrainGeometry[row + 1][column].position.x += 1.f; }
			}

			// Z
			{
				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row][column].position.z > position[0].z + 1.f) { m_terrainGeometry[row][column].position.z -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row][column].position.z < position[0].z - 1.f) { m_terrainGeometry[row][column].position.z += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row][column + 1].position.z > position[1].z + 1.f) { m_terrainGeometry[row][column + 1].position.z -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row][column + 1].position.z < position[1].z - 1.f) { m_terrainGeometry[row][column + 1].position.z += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row + 1][column + 1].position.z > position[2].z + 1.f) { m_terrainGeometry[row + 1][column + 1].position.z -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row + 1][column + 1].position.z < position[2].z - 1.f) { m_terrainGeometry[row + 1][column + 1].position.z += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row + 1][column].position.z > position[3].z + 1.f) { m_terrainGeometry[row + 1][column].position.z -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row + 1][column].position.z < position[3].z - 1.f) { m_terrainGeometry[row + 1][column].position.z += 1.f; }
			}
		}
		break;
		case LANDSCAPE_CONSTRAINT::YZ:
		{
			// Y
			{
				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row][column].position.y > position[0].y + 1.f) { m_terrainGeometry[row][column].position.y -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row][column].position.y < position[0].y - 1.f) { m_terrainGeometry[row][column].position.y += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row][column + 1].position.y > position[1].y + 1.f) { m_terrainGeometry[row][column + 1].position.y -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row][column + 1].position.y < position[1].y - 1.f) { m_terrainGeometry[row][column + 1].position.y += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row + 1][column + 1].position.y > position[2].y + 1.f) { m_terrainGeometry[row + 1][column + 1].position.y -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row + 1][column + 1].position.y < position[2].y - 1.f) { m_terrainGeometry[row + 1][column + 1].position.y += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row + 1][column].position.y > position[3].y + 1.f) { m_terrainGeometry[row + 1][column].position.y -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row + 1][column].position.y < position[3].y - 1.f) { m_terrainGeometry[row + 1][column].position.y += 1.f; }
			}

			// Z
			{
				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row][column].position.z > position[0].z + 1.f) { m_terrainGeometry[row][column].position.z -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row][column].position.z < position[0].z - 1.f) { m_terrainGeometry[row][column].position.z += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row][column + 1].position.z > position[1].z + 1.f) { m_terrainGeometry[row][column + 1].position.z -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row][column + 1].position.z < position[1].z - 1.f) { m_terrainGeometry[row][column + 1].position.z += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row + 1][column + 1].position.z > position[2].z + 1.f) { m_terrainGeometry[row + 1][column + 1].position.z -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row + 1][column + 1].position.z < position[2].z - 1.f) { m_terrainGeometry[row + 1][column + 1].position.z += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row + 1][column].position.z > position[3].z + 1.f) { m_terrainGeometry[row + 1][column].position.z -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row + 1][column].position.z < position[3].z - 1.f) { m_terrainGeometry[row + 1][column].position.z += 1.f; }
			}
		}
		break;
		case LANDSCAPE_CONSTRAINT::X:
		{
			// If selected geometry is above ground level, decrease position
			if (m_terrainGeometry[row][column].position.x > position[0].x + 1.f) { m_terrainGeometry[row][column].position.x -= 1.f; }
			// Else, if selected geometry is below ground level, increase position
			else if (m_terrainGeometry[row][column].position.x < position[0].x - 1.f) { m_terrainGeometry[row][column].position.x += 1.f; }

			// If selected geometry is above ground level, decrease position
			if (m_terrainGeometry[row][column + 1].position.x > position[1].x + 1.f) { m_terrainGeometry[row][column + 1].position.x -= 1.f; }
			// Else, if selected geometry is below ground level, increase position
			else if (m_terrainGeometry[row][column + 1].position.x < position[1].x - 1.f) { m_terrainGeometry[row][column + 1].position.x += 1.f; }

			// If selected geometry is above ground level, decrease position
			if (m_terrainGeometry[row + 1][column + 1].position.x > position[2].x + 1.f) { m_terrainGeometry[row + 1][column + 1].position.x -= 1.f; }
			// Else, if selected geometry is below ground level, increase position
			else if (m_terrainGeometry[row + 1][column + 1].position.x < position[2].x - 1.f) { m_terrainGeometry[row + 1][column + 1].position.x += 1.f; }

			// If selected geometry is above ground level, decrease position
			if (m_terrainGeometry[row + 1][column].position.x > position[3].x + 1.f) { m_terrainGeometry[row + 1][column].position.x -= 1.f; }
			// Else, if selected geometry is below ground level, increase position
			else if (m_terrainGeometry[row + 1][column].position.x < position[3].x - 1.f) { m_terrainGeometry[row + 1][column].position.x += 1.f; }
		}
		break;
		case LANDSCAPE_CONSTRAINT::Y:
		{
			// If selected geometry is above ground level, decrease position
			if (m_terrainGeometry[row][column].position.y > position[0].y + 1.f) { m_terrainGeometry[row][column].position.y -= 1.f; }
			// Else, if selected geometry is below ground level, increase position
			else if (m_terrainGeometry[row][column].position.y < position[0].y - 1.f) { m_terrainGeometry[row][column].position.y += 1.f; }

			// If selected geometry is above ground level, decrease position
			if (m_terrainGeometry[row][column + 1].position.y > position[1].y + 1.f) { m_terrainGeometry[row][column + 1].position.y -= 1.f; }
			// Else, if selected geometry is below ground level, increase position
			else if (m_terrainGeometry[row][column + 1].position.y < position[1].y - 1.f) { m_terrainGeometry[row][column + 1].position.y += 1.f; }

			// If selected geometry is above ground level, decrease position
			if (m_terrainGeometry[row + 1][column + 1].position.y > position[2].y + 1.f) { m_terrainGeometry[row + 1][column + 1].position.y -= 1.f; }
			// Else, if selected geometry is below ground level, increase position
			else if (m_terrainGeometry[row + 1][column + 1].position.y < position[2].y - 1.f) { m_terrainGeometry[row + 1][column + 1].position.y += 1.f; }

			// If selected geometry is above ground level, decrease position
			if (m_terrainGeometry[row + 1][column].position.y > position[3].y + 1.f) { m_terrainGeometry[row + 1][column].position.y -= 1.f; }
			// Else, if selected geometry is below ground level, increase position
			else if (m_terrainGeometry[row + 1][column].position.y < position[3].y - 1.f) { m_terrainGeometry[row + 1][column].position.y += 1.f; }
		}
		break;
		case LANDSCAPE_CONSTRAINT::Z:
		{
			// If selected geometry is above ground level, decrease position
			if (m_terrainGeometry[row][column].position.z > position[0].z + 1.f) { m_terrainGeometry[row][column].position.z -= 1.f; }
			// Else, if selected geometry is below ground level, increase position
			else if (m_terrainGeometry[row][column].position.z < position[0].z - 1.f) { m_terrainGeometry[row][column].position.z += 1.f; }

			// If selected geometry is above ground level, decrease position
			if (m_terrainGeometry[row][column + 1].position.z > position[1].z + 1.f) { m_terrainGeometry[row][column + 1].position.z -= 1.f; }
			// Else, if selected geometry is below ground level, increase position
			else if (m_terrainGeometry[row][column + 1].position.z < position[1].z - 1.f) { m_terrainGeometry[row][column + 1].position.z += 1.f; }

			// If selected geometry is above ground level, decrease position
			if (m_terrainGeometry[row + 1][column + 1].position.z > position[2].z + 1.f) { m_terrainGeometry[row + 1][column + 1].position.z -= 1.f; }
			// Else, if selected geometry is below ground level, increase position
			else if (m_terrainGeometry[row + 1][column + 1].position.z < position[2].z - 1.f) { m_terrainGeometry[row + 1][column + 1].position.z += 1.f; }

			// If selected geometry is above ground level, decrease position
			if (m_terrainGeometry[row + 1][column].position.z > position[3].z + 1.f) { m_terrainGeometry[row + 1][column].position.z -= 1.f; }
			// Else, if selected geometry is below ground level, increase position
			else if (m_terrainGeometry[row + 1][column].position.z < position[3].z - 1.f) { m_terrainGeometry[row + 1][column].position.z += 1.f; }
		}
		break;
		case LANDSCAPE_CONSTRAINT::ALL:
		{
			// X
			{
				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row][column].position.x > position[0].x + 1.f) { m_terrainGeometry[row][column].position.x -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row][column].position.x < position[0].x - 1.f) { m_terrainGeometry[row][column].position.x += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row][column + 1].position.x > position[1].x + 1.f) { m_terrainGeometry[row][column + 1].position.x -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row][column + 1].position.x < position[1].x - 1.f) { m_terrainGeometry[row][column + 1].position.x += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row + 1][column + 1].position.x > position[2].x + 1.f) { m_terrainGeometry[row + 1][column + 1].position.x -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row + 1][column + 1].position.x < position[2].x - 1.f) { m_terrainGeometry[row + 1][column + 1].position.x += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row + 1][column].position.x > position[3].x + 1.f) { m_terrainGeometry[row + 1][column].position.x -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row + 1][column].position.x < position[3].x - 1.f) { m_terrainGeometry[row + 1][column].position.x += 1.f; }
			}
			
			// Y
			{
				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row][column].position.y > position[0].y + 1.f) { m_terrainGeometry[row][column].position.y -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row][column].position.y < position[0].y - 1.f) { m_terrainGeometry[row][column].position.y += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row][column + 1].position.y > position[1].y + 1.f) { m_terrainGeometry[row][column + 1].position.y -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row][column + 1].position.y < position[1].y - 1.f) { m_terrainGeometry[row][column + 1].position.y += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row + 1][column + 1].position.y > position[2].y + 1.f) { m_terrainGeometry[row + 1][column + 1].position.y -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row + 1][column + 1].position.y < position[2].y - 1.f) { m_terrainGeometry[row + 1][column + 1].position.y += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row + 1][column].position.y > position[3].y + 1.f) { m_terrainGeometry[row + 1][column].position.y -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row + 1][column].position.y < position[3].y - 1.f) { m_terrainGeometry[row + 1][column].position.y += 1.f; }
			}
			
			// Z
			{
				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row][column].position.z > position[0].z + 1.f) { m_terrainGeometry[row][column].position.z -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row][column].position.z < position[0].z - 1.f) { m_terrainGeometry[row][column].position.z += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row][column + 1].position.z > position[1].z + 1.f) { m_terrainGeometry[row][column + 1].position.z -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row][column + 1].position.z < position[1].z - 1.f) { m_terrainGeometry[row][column + 1].position.z += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row + 1][column + 1].position.z > position[2].z + 1.f) { m_terrainGeometry[row + 1][column + 1].position.z -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row + 1][column + 1].position.z < position[2].z - 1.f) { m_terrainGeometry[row + 1][column + 1].position.z += 1.f; }

				// If selected geometry is above ground level, decrease position
				if (m_terrainGeometry[row + 1][column].position.z > position[3].z + 1.f) { m_terrainGeometry[row + 1][column].position.z -= 1.f; }
				// Else, if selected geometry is below ground level, increase position
				else if (m_terrainGeometry[row + 1][column].position.z < position[3].z - 1.f) { m_terrainGeometry[row + 1][column].position.z += 1.f; }
			}			
		}
		break;
		}
	}
	break;
	}								
}

void DisplayChunk::SetSelected(bool selected, int ID)
{
	// Loop through terrain geometry
	for (int i = 0; i < (TERRAINRESOLUTION - 1); i++)
	{
		for (int j = 0; j < (TERRAINRESOLUTION - 1); j++)
		{
			// If parameter ID matches current terrain ID
			//if (ID == m_terrainGeometry[i][j].ID) 
			//{
			//	// If terrain is selected
			//	if (selected)
			//	{
			//		//... highlight texture
			//		///m_terrainGeometry[i][j].
			//	}
			//	// Else, if terrain isn't selected
			//	else
			//	{
			//		//... un-highlight texture
			//	}
			//}
		}
	}

	///GenerateHeightmap();
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

void DisplayChunk::CheckForDuplicates(int row, int column, LANDSCAPE_PAINT paint)
{
	// Temp 
	int index = -1;
	std::pair<int, int> terrain;
	terrain.first = row;
	terrain.second = column;

	// Switch between paints
	switch (paint)
	{
	case LANDSCAPE_PAINT::GRASS:
	{
		// Loop through dirt vector & remove from storage
		if (FindInVector(index, m_dirt, terrain)) { m_dirt.erase(m_dirt.begin() + index); }

		// Else, loop through sand vector & remove from storage
		else if (FindInVector(index, m_sand, terrain)) { m_sand.erase(m_sand.begin() + index); }		

		// Else, loop through default vector & remove from storage
		else if (FindInVector(index, m_default, terrain)) { m_default.erase(m_default.begin() + index); }

		// Else, loop through grass and dirt vector & remove from storage
		else if (FindInVector(index, m_grassAndDirt, terrain)) { m_grassAndDirt.erase(m_grassAndDirt.begin() + index); }

		// Else, loop through grass and sand vector & remove from storage
		else if (FindInVector(index, m_grassAndSand, terrain)) { m_grassAndSand.erase(m_grassAndSand.begin() + index); }

		// Else, loop through dirt and sand vector & remove from storage
		else if (FindInVector(index, m_dirtAndSand, terrain)) { m_dirtAndSand.erase(m_dirtAndSand.begin() + index); }
	}
	break;
	case LANDSCAPE_PAINT::DIRT:
	{
		// Loop through grass vector & remove from storage
		if (FindInVector(index, m_grass, terrain)) { m_grass.erase(m_grass.begin() + index); }

		// Else, loop through sand vector & remove from storage
		else if (FindInVector(index, m_sand, terrain)) { m_sand.erase(m_sand.begin() + index); }

		// Else, loop through default vector & remove from storage
		else if (FindInVector(index, m_default, terrain)) { m_default.erase(m_default.begin() + index); }

		// Else, loop through grass and dirt vector & remove from storage
		else if (FindInVector(index, m_grassAndDirt, terrain)) { m_grassAndDirt.erase(m_grassAndDirt.begin() + index); }

		// Else, loop through grass and sand vector & remove from storage
		else if (FindInVector(index, m_grassAndSand, terrain)) { m_grassAndSand.erase(m_grassAndSand.begin() + index); }

		// Else, loop through dirt and sand vector & remove from storage
		else if (FindInVector(index, m_dirtAndSand, terrain)) { m_dirtAndSand.erase(m_dirtAndSand.begin() + index); }
	}
	break;
	case LANDSCAPE_PAINT::SAND:
	{
		// Loop through grass vector & remove from storage
		if (FindInVector(index, m_grass, terrain)) { m_grass.erase(m_grass.begin() + index); }

		// Else, loop through dirt vector & remove from storage
		else if (FindInVector(index, m_dirt, terrain)) { m_dirt.erase(m_dirt.begin() + index); }

		// Else, loop through default vector & remove from storage
		else if (FindInVector(index, m_default, terrain)) { m_default.erase(m_default.begin() + index); }

		// Else, loop through grass and dirt vector & remove from storage
		else if (FindInVector(index, m_grassAndDirt, terrain)) { m_grassAndDirt.erase(m_grassAndDirt.begin() + index); }

		// Else, loop through grass and sand vector & remove from storage
		else if (FindInVector(index, m_grassAndSand, terrain)) { m_grassAndSand.erase(m_grassAndSand.begin() + index); }

		// Else, loop through dirt and sand vector & remove from storage
		else if (FindInVector(index, m_dirtAndSand, terrain)) { m_dirtAndSand.erase(m_dirtAndSand.begin() + index); }
	}
	break;
	case LANDSCAPE_PAINT::GRASS_AND_DIRT:
	{
		// Loop through grass vector & remove from storage
		if (FindInVector(index, m_grass, terrain)) { m_grass.erase(m_grass.begin() + index); }

		// Else, loop through dirt vector & remove from storage
		else if (FindInVector(index, m_dirt, terrain)) { m_dirt.erase(m_dirt.begin() + index); }

		// Else, loop through sand vector & remove from storage
		else if (FindInVector(index, m_sand, terrain)) { m_sand.erase(m_sand.begin() + index); }

		// Else, loop through default vector & remove from storage
		else if (FindInVector(index, m_default, terrain)) { m_default.erase(m_default.begin() + index); }

		// Else, loop through grass and sand vector & remove from storage
		else if (FindInVector(index, m_grassAndSand, terrain)) { m_grassAndSand.erase(m_grassAndSand.begin() + index); }

		// Else, loop through dirt and sand vector & remove from storage
		else if (FindInVector(index, m_dirtAndSand, terrain)) { m_dirtAndSand.erase(m_dirtAndSand.begin() + index); }
	}
	break;
	case LANDSCAPE_PAINT::GRASS_AND_SAND:
	{
		// Loop through grass vector & remove from storage
		if (FindInVector(index, m_grass, terrain)) { m_grass.erase(m_grass.begin() + index); }

		// Else, loop through dirt vector & remove from storage
		else if (FindInVector(index, m_dirt, terrain)) { m_dirt.erase(m_dirt.begin() + index); }

		// Else, loop through sand vector & remove from storage
		else if (FindInVector(index, m_sand, terrain)) { m_sand.erase(m_sand.begin() + index); }

		// Else, loop through default vector & remove from storage
		else if (FindInVector(index, m_default, terrain)) { m_default.erase(m_default.begin() + index); }

		// Else, loop through grass and dirt vector & remove from storage
		else if (FindInVector(index, m_grassAndDirt, terrain)) { m_grassAndDirt.erase(m_grassAndDirt.begin() + index); }

		// Else, loop through dirt and sand vector & remove from storage
		else if (FindInVector(index, m_dirtAndSand, terrain)) { m_dirtAndSand.erase(m_dirtAndSand.begin() + index); }
	}
	break;
	case LANDSCAPE_PAINT::DIRT_AND_SAND:
	{
		// Loop through grass vector & remove from storage
		if (FindInVector(index, m_grass, terrain)) { m_grass.erase(m_grass.begin() + index); }

		// Else, loop through dirt vector & remove from storage
		else if (FindInVector(index, m_dirt, terrain)) { m_dirt.erase(m_dirt.begin() + index); }

		// Else, loop through sand vector & remove from storage
		else if (FindInVector(index, m_sand, terrain)) { m_sand.erase(m_sand.begin() + index); }

		// Else, loop through default vector & remove from storage
		else if (FindInVector(index, m_default, terrain)) { m_default.erase(m_default.begin() + index); }

		// Else, loop through grass and dirt vector & remove from storage
		else if (FindInVector(index, m_grassAndDirt, terrain)) { m_grassAndDirt.erase(m_grassAndDirt.begin() + index); }

		// Else, loop through grass and sand vector & remove from storage
		else if (FindInVector(index, m_grassAndSand, terrain)) { m_grassAndSand.erase(m_grassAndSand.begin() + index); }
	}
	break;
	case LANDSCAPE_PAINT::NA:
	{		
		// Loop through grass vector & remove from storage
		if (FindInVector(index, m_grass, terrain)) { m_grass.erase(m_grass.begin() + index); }

		// Else, loop through dirt vector & remove from storage
		else if (FindInVector(index, m_dirt, terrain)) { m_dirt.erase(m_dirt.begin() + index); }

		// Else, loop through sand vector & remove from storage
		else if (FindInVector(index, m_sand, terrain)) { m_sand.erase(m_sand.begin() + index); }

		// Else, loop through grass and dirt vector & remove from storage
		else if (FindInVector(index, m_grassAndDirt, terrain)) { m_grassAndDirt.erase(m_grassAndDirt.begin() + index); }

		// Else, loop through grass and sand vector & remove from storage
		else if (FindInVector(index, m_grassAndSand, terrain)) { m_grassAndSand.erase(m_grassAndSand.begin() + index); }

		// Else, loop through dirt and sand vector & remove from storage
		else if (FindInVector(index, m_dirtAndSand, terrain)) { m_dirtAndSand.erase(m_dirtAndSand.begin() + index); }
	}
	break;
	}
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

	// Store each 'c' index
	std::vector<std::pair<int, int>> c; {
		c.push_back(std::pair<int, int>(row - 2, column - 2));		// 2 upward, 2 backward
		c.push_back(std::pair<int, int>(row - 2, column));			// 2 upward
		c.push_back(std::pair<int, int>(row - 2, column + 2));		// 2 upward, 2 forward
		c.push_back(std::pair<int, int>(row, column - 2));			// 2 backward
		c.push_back(std::pair<int, int>(row, column + 2));			// 2 forward
		c.push_back(std::pair<int, int>(row + 2, column - 2));		// 2 downward, 2 backward
		c.push_back(std::pair<int, int>(row + 2, column));			// 2 downward
		c.push_back(std::pair<int, int>(row + 2, column + 2));		// 2 downward, 2 forward
	}
	
	// Check each 'c' index for their current paints
	std::vector<LANDSCAPE_PAINT> c_paints;
	for (int i = 0; i < c.size(); ++i) { 
		c_paints.push_back(CheckPaint(c[i].first, c[i].second)); 
	}
	
	// Store each 'o' index
	std::vector<std::pair<int, int>> o; {	
		o.push_back(std::pair<int, int>(row - 1, column - 1));		// 1 upward, 1 backward
		o.push_back(std::pair<int, int>(row - 1, column));			// 1 upward
		o.push_back(std::pair<int, int>(row - 1, column + 1));		// 1 upward, 1 forward
		o.push_back(std::pair<int, int>(row, column - 1));			// 1 backward
		o.push_back(std::pair<int, int>(row, column + 1));			// 1 forward
		o.push_back(std::pair<int, int>(row + 1, column - 1));		// 1 downward, 1 backward
		o.push_back(std::pair<int, int>(row + 1, column));			// 1 downward
		o.push_back(std::pair<int, int>(row + 1, column + 1));		// 1 downward, 1 forward
	}

	// Check each 'o' index for their current paints
	std::vector<LANDSCAPE_PAINT> o_paints;
	for (int i = 0; i < o.size(); ++i) { 
		o_paints.push_back(CheckPaint(o[i].first, o[i].second)); 
	}
		
	// Store 'x' paint
	LANDSCAPE_PAINT x_paint = CheckPaint(row, column);
	
	// Blend each 'o' index depending on 'x' & adjacent 'c' paints
	for (int i = 0; i < c.size(); ++i)
	{
		// Switch between 'c' paints
		switch (c_paints[i])
		{
		case LANDSCAPE_PAINT::GRASS:
		{
			// Switch between 'x' paint
			switch (x_paint)
			{
			case LANDSCAPE_PAINT::GRASS: o_paints[i] = LANDSCAPE_PAINT::GRASS; break;
			case LANDSCAPE_PAINT::DIRT: o_paints[i] = LANDSCAPE_PAINT::GRASS_AND_DIRT; break;
			case LANDSCAPE_PAINT::SAND: o_paints[i] = LANDSCAPE_PAINT::GRASS_AND_SAND; break;
			}
		}
		break;
		case LANDSCAPE_PAINT::DIRT:
		{
			// Switch between 'x' paint
			switch (x_paint)
			{
			case LANDSCAPE_PAINT::GRASS: o_paints[i] = LANDSCAPE_PAINT::GRASS_AND_DIRT; break;
			case LANDSCAPE_PAINT::DIRT: o_paints[i] = LANDSCAPE_PAINT::DIRT; break;
			case LANDSCAPE_PAINT::SAND: o_paints[i] = LANDSCAPE_PAINT::GRASS_AND_SAND; break;
			}
		}
		break;
		case LANDSCAPE_PAINT::SAND:
		{
			// Switch between 'x' paint
			switch (x_paint)
			{
			case LANDSCAPE_PAINT::GRASS: o_paints[i] = LANDSCAPE_PAINT::GRASS_AND_SAND; break;
			case LANDSCAPE_PAINT::DIRT: o_paints[i] = LANDSCAPE_PAINT::DIRT_AND_SAND; break;
			case LANDSCAPE_PAINT::SAND: o_paints[i] = LANDSCAPE_PAINT::SAND; break;
			}
		}
		break;
		}
	}

	// Loop through blended 'o' indexes
	for (int i = 0; i < o.size(); ++i)
	{
		// Remove any duplicates
		CheckForDuplicates(o[i].first, o[i].second, o_paints[i]);

		// Switch between blended paint
		switch (o_paints[i])
		{
		case LANDSCAPE_PAINT::GRASS: m_grass.push_back(std::pair<int, int>(o[i].first, o[i].second)); break;
		case LANDSCAPE_PAINT::DIRT: m_dirt.push_back(std::pair<int, int>(o[i].first, o[i].second)); break;
		case LANDSCAPE_PAINT::SAND: m_sand.push_back(std::pair<int, int>(o[i].first, o[i].second)); break;
		case LANDSCAPE_PAINT::GRASS_AND_DIRT: m_grassAndDirt.push_back(std::pair<int, int>(o[i].first, o[i].second)); break;
		case LANDSCAPE_PAINT::GRASS_AND_SAND: m_grassAndSand.push_back(std::pair<int, int>(o[i].first, o[i].second)); break;
		case LANDSCAPE_PAINT::DIRT_AND_SAND: m_dirtAndSand.push_back(std::pair<int, int>(o[i].first, o[i].second)); break;
		case LANDSCAPE_PAINT::NA: m_default.push_back(std::pair<int, int>(o[i].first, o[i].second)); break;
		}
	}
}

LANDSCAPE_PAINT DisplayChunk::CheckPaint(int row, int column)
{
	// Temp
	int index = -1;
	std::pair<int, int> terrain;
	terrain.first = row;
	terrain.second = column;

	// Loop through grass vector & remove from storage
	if (FindInVector(index, m_grass, terrain)) { return LANDSCAPE_PAINT::GRASS; }

	// Else, loop through dirt vector & return paint
	else if (FindInVector(index, m_dirt, terrain)) { return LANDSCAPE_PAINT::DIRT; }

	// Else, loop through sand vector & return paint
	else if (FindInVector(index, m_sand, terrain)) { return LANDSCAPE_PAINT::SAND; }

	// Else, loop through grass and dirt vector & return paint
	else if (FindInVector(index, m_grassAndDirt, terrain)) { return LANDSCAPE_PAINT::GRASS_AND_DIRT; }

	// Else, loop through grass and sand vector & return paint
	else if (FindInVector(index, m_grassAndSand, terrain)) { return LANDSCAPE_PAINT::GRASS_AND_SAND; }

	// Else, loop through dirt and sand vector & return paint
	else if (FindInVector(index, m_dirtAndSand, terrain)) { return LANDSCAPE_PAINT::DIRT_AND_SAND; }

	// Else, return default paint
	else { return LANDSCAPE_PAINT::NA; }
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
