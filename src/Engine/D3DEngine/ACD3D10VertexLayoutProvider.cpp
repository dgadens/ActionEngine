#include "ACD3D10VertexLayoutProvider.h"

ID3D10InputLayout* ACD3D10VertexLayoutProvider::pVertexPositionLayout = nullptr;
ID3D10InputLayout* ACD3D10VertexLayoutProvider::pVertexPositionColoredLayout = nullptr;
ID3D10InputLayout* ACD3D10VertexLayoutProvider::pVertexPositionTexturedLayout = nullptr;
ID3D10InputLayout* ACD3D10VertexLayoutProvider::pVertexPositionNormalTexturedLayout = nullptr;
ID3D10InputLayout* ACD3D10VertexLayoutProvider::pVertexPositionTexturedExtraInfoLayout = nullptr;
ID3D10InputLayout* ACD3D10VertexLayoutProvider::pVertexSpriteLayout = nullptr;

HRESULT ACD3D10VertexLayoutProvider::CreateInputLayout(ID3D10Device* gDevice, ID3D10Blob* pVSBuf, VertexFormat vertexFormat)
{
	HRESULT hr;

	switch (vertexFormat)
	{
		case VF_VertexPosition:
		{
			if (pVertexPositionLayout == nullptr)
			{
				// Define  o layout do positioncolored
				D3D10_INPUT_ELEMENT_DESC layout[] =
				{
					{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0 }
				};
				UINT numElements = sizeof( layout ) / sizeof( layout[0] );

				hr= gDevice->CreateInputLayout( layout, 
												numElements, 
												pVSBuf->GetBufferPointer(),
												pVSBuf->GetBufferSize(), 
												&pVertexPositionLayout );

				if( FAILED( hr ) )
					return hr;
			}
		};
		break;
		case VF_VertexPositionColored:
		{
			if (pVertexPositionColoredLayout == nullptr)
			{
				// Define  o layout do positioncolored
				D3D10_INPUT_ELEMENT_DESC layout[] =
				{
					{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
					{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 }
				};
				UINT numElements = sizeof( layout ) / sizeof( layout[0] );

				hr= gDevice->CreateInputLayout( layout, 
												numElements, 
												pVSBuf->GetBufferPointer(),
												pVSBuf->GetBufferSize(), 
												&pVertexPositionColoredLayout );

				if( FAILED( hr ) )
					return hr;
			}
		};
		break;
		case VF_VertexPositionTextured:
		{
			if (pVertexPositionTexturedLayout == nullptr)
			{
				// Define  o layout do positiontextured
				D3D10_INPUT_ELEMENT_DESC layout[] =
				{
					{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
					{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 }
				};
				UINT numElements = sizeof( layout ) / sizeof( layout[0] );

				hr= gDevice->CreateInputLayout( layout, 
												numElements, 
												pVSBuf->GetBufferPointer(),
												pVSBuf->GetBufferSize(), 
												&pVertexPositionTexturedLayout );

				if( FAILED( hr ) )
					return hr;
			}
		};
		break;
		case VF_VertexPositionNormalTextured:
		{
			if (pVertexPositionNormalTexturedLayout == nullptr)
			{
				// Define  o layout do positionnormaltextured
				D3D10_INPUT_ELEMENT_DESC layout[] =
				{
					{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
					{"NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
					{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 }
				};
				UINT numElements = sizeof( layout ) / sizeof( layout[0] );

				hr= gDevice->CreateInputLayout( layout, 
												numElements, 
												pVSBuf->GetBufferPointer(),
												pVSBuf->GetBufferSize(), 
												&pVertexPositionNormalTexturedLayout );

				if( FAILED( hr ) )
					return hr;
			}
		};
		break;
		case VF_VertexPositionTexturedExtraInfo:
		{
			if (pVertexPositionTexturedExtraInfoLayout == nullptr)
			{
				// Define  o layout do positiontexturedextrainfo
				D3D10_INPUT_ELEMENT_DESC layout[] =
				{
					{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
					{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
					{"EXTRAINFO", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D10_INPUT_PER_VERTEX_DATA, 0 },
				};
				UINT numElements = sizeof( layout ) / sizeof( layout[0] );

				hr= gDevice->CreateInputLayout( layout, 
												numElements, 
												pVSBuf->GetBufferPointer(),
												pVSBuf->GetBufferSize(), 
												&pVertexPositionTexturedExtraInfoLayout );

				if( FAILED( hr ) )
					return hr;
			}
		};
		break;
		case VF_VertexSprite:
		{
			if (pVertexSpriteLayout == nullptr)
			{
				// Define  o layout do sprite
				D3D10_INPUT_ELEMENT_DESC layout[] =
				{
					{"ANCHOR",		0, DXGI_FORMAT_R32G32_FLOAT,		0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
					{"DIMENSIONS",  0, DXGI_FORMAT_R32G32_FLOAT,		0,  8, D3D10_INPUT_PER_VERTEX_DATA, 0 },
					{"TEXCOORD",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 16, D3D10_INPUT_PER_VERTEX_DATA, 0 },
					{"TEXCOORD",    1, DXGI_FORMAT_R32_FLOAT,			0, 32, D3D10_INPUT_PER_VERTEX_DATA, 0 },
					{"TEXCOORD",    2, DXGI_FORMAT_R32_FLOAT,			0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0 },
					{"TEXCOORD",    3, DXGI_FORMAT_R32G32_FLOAT,		0, 40, D3D10_INPUT_PER_VERTEX_DATA, 0 },
					{"COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 48, D3D10_INPUT_PER_VERTEX_DATA, 0 }
				};
				UINT numElements = sizeof( layout ) / sizeof( layout[0] );

				hr= gDevice->CreateInputLayout( layout, 
												numElements, 
												pVSBuf->GetBufferPointer(),
												pVSBuf->GetBufferSize(), 
												&pVertexSpriteLayout );

				if( FAILED( hr ) )
					return hr;
			}
		};
		break;

	};

	return AC_OK;
};

void ACD3D10VertexLayoutProvider::ReleaseAll()
{
	SAFE_RELEASE(pVertexPositionLayout);
	SAFE_RELEASE(pVertexPositionColoredLayout);
	SAFE_RELEASE(pVertexPositionTexturedLayout);
	SAFE_RELEASE(pVertexPositionNormalTexturedLayout);
	SAFE_RELEASE(pVertexPositionTexturedExtraInfoLayout);
	SAFE_RELEASE(pVertexSpriteLayout);
};