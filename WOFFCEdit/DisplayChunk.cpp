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
	else if (m_default.size() != 0) {
		m_batch->Begin();
		m_terrainEffect->SetTexture(m_texture_default);
		m_terrainEffect->Apply(context);
		DrawTerrain(m_default);	
		m_batch->End();
	}
	
	//for (size_t i = 0; i < TERRAINRESOLUTION-1; i++)	//looping through QUADS.  so we subtrack one from the terrain array or it will try to draw a quad starting with the last vertex in each row. Which wont work
	//{
	//	for (size_t j = 0; j < TERRAINRESOLUTION-1; j++)//same as above
	//	{
	//		m_batch->Begin();
	//		
	//		///int index = (TERRAINRESOLUTION * i) + j;
	//		
	//		// If current paint is default
	//		/*if (m_paint[i][j] == LANDSCAPE_PAINT::NA) 
	//		{ m_terrainEffect->SetTexture(m_texture_default); }*/

	//		// Else, if current paint is grass
	//		if (m_paint[i][j] == LANDSCAPE_PAINT::GRASS) 
	//		{ 
	//			m_terrainEffect->SetTexture(m_texture_splat_1);
	//			m_terrainEffect->Apply(context);
	//		}
	//		///if (m_paint[index] == LANDSCAPE_PAINT::GRASS) 
	//		
	//		// Else, if current paint is dirt
	//		else if (m_paint[i][j] == LANDSCAPE_PAINT::DIRT) 
	//		{ 
	//			m_terrainEffect->SetTexture(m_texture_splat_2);
	//			m_terrainEffect->Apply(context);
	//		}
	//		///else if (m_paint[index] == LANDSCAPE_PAINT::DIRT)
	//		
	//		// Else, if current paint is sand
	//		else if (m_paint[i][j] == LANDSCAPE_PAINT::SAND) 
	//		{ 
	//			m_terrainEffect->SetTexture(m_texture_splat_3);
	//			m_terrainEffect->Apply(context);
	//		}
	//		///else if (m_paint[index] == LANDSCAPE_PAINT::SAND)

	//		// Else, if no paint is selected
	//		else
	//		{
	//			m_terrainEffect->SetTexture(m_texture_default);
	//			m_terrainEffect->Apply(context);
	//			
	//		}
	//		
	//		m_batch->DrawQuad(m_terrainGeometry[i][j], m_terrainGeometry[i][j+1], m_terrainGeometry[i+1][j+1], m_terrainGeometry[i+1][j]); //bottom left bottom right, top right top left.
	//	
	//		m_batch->End();
	//	}
	//}
	
	//m_batch->Begin();
	//m_terrainEffect->SetTexture(m_texture_splat_1);
	//m_terrainEffect->Apply(context1);
	//m_batch->DrawQuad(m_terrainGeometry[0][0], m_terrainGeometry[0][1], m_terrainGeometry[1][1], m_terrainGeometry[1][0]); //bottom left bottom right, top right top left.
	//m_batch->End();
	//
	//m_batch->Begin();
	//m_terrainEffect->SetTexture(m_texture_splat_2);
	//m_terrainEffect->Apply(context2);
	//m_batch->DrawQuad(m_terrainGeometry[2][2], m_terrainGeometry[2][3], m_terrainGeometry[3][3], m_terrainGeometry[3][2]); //bottom left bottom right, top right top left.
	//m_batch->End();
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
	/*int index = (TERRAINRESOLUTION * row) + column;
	m_paint[index] = paint;*/
	
	/*m_paint[row][column] = paint;
	m_paint[row][column + 1] = paint;
	m_paint[row + 1][column + 1] = paint;
	m_paint[row + 1][column] = paint;*/

	std::pair<DirectX::VertexPositionNormalTexture, int> t0;
	t0.first = m_terrainGeometry[i][j];
	t0.second = (TERRAINRESOLUTION * i) + j;

	std::pair<DirectX::VertexPositionNormalTexture, int> t1;
	t1.first = m_terrainGeometry[i][j];
	t1.second = (TERRAINRESOLUTION * i) + (j + 1);

	std::pair<DirectX::VertexPositionNormalTexture, int> t2;
	t2.first = m_terrainGeometry[i][j];
	t2.second = (TERRAINRESOLUTION * (i + 1)) + (j + 1);

	std::pair<DirectX::VertexPositionNormalTexture, int> t3;
	t3.first = m_terrainGeometry[i][j];
	t3.second = (TERRAINRESOLUTION * (i + 1)) + j;
	
	// Switch between paints
	switch (paint)
	{
	case LANDSCAPE_PAINT::GRASS:
	{
		m_grass.push_back(t0);
		m_grass.push_back(t1);
		m_grass.push_back(t2);
		m_grass.push_back(t3);		
	}
	break;
	case LANDSCAPE_PAINT::DIRT:
	{
		m_dirt.push_back(t0);
		m_dirt.push_back(t1);
		m_dirt.push_back(t2);
		m_dirt.push_back(t3);
	}
	break;
	case LANDSCAPE_PAINT::SAND:
	{
		m_sand.push_back(t0);
		m_sand.push_back(t1);
		m_sand.push_back(t2);
		m_sand.push_back(t3);
	}
	break;
	case LANDSCAPE_PAINT::NA:
	{
		m_default.push_back(t0);
		m_default.push_back(t1);
		m_default.push_back(t2);
		m_default.push_back(t3);
	}
	break;
	}

	// Check if selected geometry is within other containers & remove
	///CheckTerrain(m_terrainGeometry[row][column], paint);
	///CheckTerrain(m_terrainGeometry[row][column + 1], paint);
	///CheckTerrain(m_terrainGeometry[row + 1][column + 1], paint);
	///CheckTerrain(m_terrainGeometry[row + 1][column], paint);
	CheckForDuplicates(t0, paint);
	CheckForDuplicates(t1, paint);
	CheckForDuplicates(t2, paint);
	CheckForDuplicates(t3, paint);
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

void DisplayChunk::DrawTerrain(std::vector<std::pair<DirectX::VertexPositionNormalTexture, int>> terrain)
{
	int count = 0;
	for (int i = 0; i < terrain.size(); ++i)
	{
		if (count != 3) { count++; }
		else
		{
			count = 0;
			m_batch->DrawQuad(terrain[i - 3].first, terrain[i - 2].first, terrain[i - 1].first, terrain[i].first);
		}
	}
}

void DisplayChunk::CheckTerrain(DirectX::VertexPositionNormalTexture terrain, LANDSCAPE_PAINT paint)
{	
	// Switch between paint
	//switch (paint)
	//{
	//case LANDSCAPE_PAINT::GRASS:
	//{
	//	// Loop through dirt vector
	//	auto dirt = m_dirt.begin();
	//	while (dirt != m_dirt.end())
	//	{
	//		if (dirt->position.x == terrain.position.x &&
	//			dirt->position.y == terrain.position.y &&
	//			dirt->position.z == terrain.position.z)
	//		{
	//			dirt = m_dirt.erase(dirt);
	//			///break;
	//		}
	//		else { ++dirt; }
	//	}		

	//	// Loop through sand vector
	//	auto sand = m_sand.begin();
	//	while (sand != m_sand.end())
	//	{
	//		if (sand->position.x == terrain.position.x &&
	//			sand->position.y == terrain.position.y &&
	//			sand->position.z == terrain.position.z)
	//		{
	//			sand = m_sand.erase(sand);
	//			///break;
	//		}
	//		else { ++sand; }
	//	}
	//	
	//	// Loop through default vector
	//	auto _default = m_default.begin();
	//	while (_default != m_default.end())
	//	{
	//		if (_default->position.x == terrain.position.x &&
	//			_default->position.y == terrain.position.y &&
	//			_default->position.z == terrain.position.z)
	//		{
	//			_default = m_default.erase(_default);
	//			///break;
	//		}
	//		else { ++_default; }
	//	}
	//}
	//break;
	//case LANDSCAPE_PAINT::DIRT:
	//{
	//	// Loop through grass vector
	//	auto grass = m_grass.begin();
	//	while (grass != m_grass.end())
	//	{
	//		if (grass->position.x == terrain.position.x &&
	//			grass->position.y == terrain.position.y &&
	//			grass->position.z == terrain.position.z)
	//		{
	//			grass = m_grass.erase(grass);
	//			///break;
	//		}
	//		else { ++grass; }
	//	}

	//	// Loop through sand vector
	//	auto sand = m_sand.begin();
	//	while (sand != m_sand.end())
	//	{
	//		if (sand->position.x == terrain.position.x &&
	//			sand->position.y == terrain.position.y &&
	//			sand->position.z == terrain.position.z)
	//		{
	//			sand = m_sand.erase(sand);
	//			///break;
	//		}
	//		else { ++sand; }
	//	}

	//	// Loop through default vector
	//	auto _default = m_default.begin();
	//	while (_default != m_default.end())
	//	{
	//		if (_default->position.x == terrain.position.x &&
	//			_default->position.y == terrain.position.y &&
	//			_default->position.z == terrain.position.z)
	//		{
	//			_default = m_default.erase(_default);
	//			///break;
	//		}
	//		else { ++_default; }
	//	}
	//}
	//break;
	//case LANDSCAPE_PAINT::SAND:
	//{
	//	// Loop through grass vector
	//	auto grass = m_grass.begin();
	//	while (grass != m_grass.end())
	//	{
	//		if (grass->position.x == terrain.position.x &&
	//			grass->position.y == terrain.position.y &&
	//			grass->position.z == terrain.position.z)
	//		{
	//			grass = m_grass.erase(grass);
	//			///break;
	//		}
	//		else { ++grass; }
	//	}

	//	// Loop through dirt vector
	//	auto dirt = m_dirt.begin();
//	while (dirt != m_dirt.end())
//	{
//		if (dirt->position.x == terrain.position.x &&
//			dirt->position.y == terrain.position.y &&
//			dirt->position.z == terrain.position.z)
//		{
//			dirt = m_dirt.erase(dirt);
//			///break;
//		}
//		else { ++dirt; }
//	}

//	// Loop through default vector
//	auto _default = m_default.begin();
//	while (_default != m_default.end())
//	{
//		if (_default->position.x == terrain.position.x &&
//			_default->position.y == terrain.position.y &&
//			_default->position.z == terrain.position.z)
//		{
//			_default = m_default.erase(_default);
//			///break;
//		}
//		else { ++_default; }
//	}
//}
//break;
//case LANDSCAPE_PAINT::NA:
//{
//	// Loop through grass vector
//	auto grass = m_grass.begin();
//	while (grass != m_grass.end())
//	{
//		if (grass->position.x == terrain.position.x &&
//			grass->position.y == terrain.position.y &&
//			grass->position.z == terrain.position.z)
//		{
//			grass = m_grass.erase(grass);
//			///break;
//		}
//		else { ++grass; }
//	}

//	// Loop through dirt vector
//	auto dirt = m_dirt.begin();
//	while (dirt != m_dirt.end())
//	{
//		if (dirt->position.x == terrain.position.x &&
//			dirt->position.y == terrain.position.y &&
//			dirt->position.z == terrain.position.z)
//		{
//			dirt = m_dirt.erase(dirt);
//			///break;
//		}
//		else { ++dirt; }
//	}

//	// Loop through sand vector
//	auto sand = m_sand.begin();
//	while (sand != m_sand.end())
//	{
//		if (sand->position.x == terrain.position.x &&
//			sand->position.y == terrain.position.y &&
//			sand->position.z == terrain.position.z)
//		{
//			sand = m_sand.erase(sand);
//			///break;
//		}
//		else { ++sand; }
//	}
//}
//break;
//}
}

void DisplayChunk::CheckForDuplicates(std::pair<DirectX::VertexPositionNormalTexture, int> terrain, LANDSCAPE_PAINT paint)
{
	// Temp
	int index = -1;
	
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

bool DisplayChunk::FindInVector(int &index, std::vector<std::pair<DirectX::VertexPositionNormalTexture, int>> vector, std::pair<DirectX::VertexPositionNormalTexture, int> terrain)
{	
	// Loop through vector
	for (int i = 0; i < vector.size(); ++i)
	{
		// If current ID matches selected ID 
		if (vector[i].second == terrain.second)
		{
			// Set index
			index = i;

			// Return true if found
			return true;
		}
	}

	// Return false if not found
	return false;
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
