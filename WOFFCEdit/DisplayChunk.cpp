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

void DisplayChunk::RenderBatch(std::shared_ptr<DX::DeviceResources> deviceResources)
{	
	// Setup device context
	auto context = deviceResources->GetD3DDeviceContext();
	context->IASetInputLayout(m_terrainInputLayout.Get());
	
	// Draw all grass geometry
	if (m_grass.size() != 0) {
		m_batch->Begin();
		m_terrainEffect->SetTexture(m_texture_splat_1);
		m_terrainEffect->Apply(context);
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

	//load the default texture
	std::wstring diffuse = StringToWCHART(m_tex_diffuse_path);
	HRESULT rs0;
	rs0 = CreateDDSTextureFromFile(device, diffuse.c_str(), NULL, &m_texture_default);	//load tex into Shader resource	view and resource
																							
	//////////////////////////////////////////////////////////////////////////////////////////
																						
	// Setup terrain effect
	m_terrainEffect = std::make_unique<BasicEffect>(device);
	m_terrainEffect->EnableDefaultLighting();
	m_terrainEffect->SetLightingEnabled(true);
	m_terrainEffect->SetTextureEnabled(true);
	m_terrainEffect->SetTexture(m_texture_default);

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

void DisplayChunk::PaintTerrain(int i, int j, LANDSCAPE_PAINT paint)
{	
	// Check if selected geometry is within other containers & remove
	CheckForDuplicates(i, j, paint);

	// Temp 
	std::pair<int, int> terrain;
	terrain.first = i;
	terrain.second = j;

	// Switch between paints
	switch (paint)
	{
	case LANDSCAPE_PAINT::GRASS: m_grass.push_back(terrain); break;
	case LANDSCAPE_PAINT::DIRT: m_dirt.push_back(terrain); break;
	case LANDSCAPE_PAINT::SAND: m_sand.push_back(terrain); break;
	case LANDSCAPE_PAINT::NA: m_default.push_back(terrain); break;
	}
}

void DisplayChunk::SculptTerrain(int row, int column, LANDSCAPE_SCULPT sculpt, LANDSCAPE_CONSTRAINT constraint, std::vector<DirectX::SimpleMath::Vector3> position)
{
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
	}
	break;
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
