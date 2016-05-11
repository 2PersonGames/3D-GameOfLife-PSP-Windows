/**
  * Author: Sam Albon
  * Created: 24/02/2011
  * Defines all the functions related to rendering
  * on the PSP display
  */

#include "FrontEnd.h"
#ifdef PC
#else
#include <displaysvc.h>
#endif

/* msg proc function */
#ifdef PC
LRESULT CALLBACK FrontEnd::MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch ( msg )
	{
	case WM_CLOSE:
		return 0;
	default:
		return DefWindowProc( hWnd, msg, wParam, lParam );
	}
}
#endif

FrontEnd::FrontEnd(InputManager* input)
{
	_playerInput = input;
	_bgColour = 0xff000000;
	_fgColour = 0xffff0000;
	_rotationSpeed = 1.0f;
	_viewMode = 0;
}

FrontEnd::~FrontEnd(void)
{
#ifdef PC
#endif
}

/* Functions */

/**
  * Created: 24/02/2011
  * Initialises the PSP display for rendering
  */
#ifdef PC
void FrontEnd::StartUpDisplay(HINSTANCE hInst)
#else
void FrontEnd::StartUpDisplay(void)
#endif
{
#ifdef PC
	UNREFERENCED_PARAMETER( hInst );

	// register window class
	WNDCLASSEX wc = 
	{
		sizeof( WNDCLASSEX ), CS_CLASSDC, MainWndProc, 0L, 0L,
		GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
		L"Cross-Platform Development", NULL
	};
	wc.hInstance = hInst;
	_wc = wc;
	if( !RegisterClassEx( &wc ) )
	{
		MessageBox( 0, L"Failed to Register Class", 0, 0 );
		PostQuitMessage( 0 );
	}

	// create the window
	_d3dParameters.BackBufferHeight = 272;
	_d3dParameters.BackBufferWidth = 480;
	HWND hWnd = CreateWindow( L"Cross-Platform Development", L"Cross-Platform Development", WS_OVERLAPPEDWINDOW, 100, 100, 
		_d3dParameters.BackBufferWidth, _d3dParameters.BackBufferHeight, NULL, NULL, wc.hInstance, NULL );

	// check if the hWnd has been created
	if( !hWnd )
	{
		MessageBox( 0, L"Failed to Create Window", 0, 0 );
		PostQuitMessage( 0 );
	}

	/* Create the device */
	if ( NULL == ( _pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		MessageBox( 0, L"DirectX SDK Version Wrong", 0, 0 );
		PostQuitMessage( 0 );
	}

	// set up the structure used to create the D3DDevice
	int height = _d3dParameters.BackBufferHeight;
	int width = _d3dParameters.BackBufferWidth;
	ZeroMemory( &_d3dParameters, sizeof( _d3dParameters ) );
	_d3dParameters.Windowed = TRUE;
	_d3dParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	_d3dParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	_d3dParameters.BackBufferHeight = height;
	_d3dParameters.BackBufferWidth = width;
	_d3dParameters.EnableAutoDepthStencil = TRUE;		/* Allow directX to create and manage a z buffer for me */
	_d3dParameters.AutoDepthStencilFormat = D3DFMT_D16;	/* Use the 16bit format */

	// create D3DDevice
	if ( FAILED( _pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &_d3dParameters, &_pd3dDevice ) ) )
	{
		if ( FAILED( _pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &_d3dParameters, &_pd3dDevice ) ) )
		{
			MessageBox( 0, L"This machine cannot process vertices", 0, 0 );
			PostQuitMessage( 0 );
		}
	}

	// set up some default render modes
	_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );		/*< Solid fill rendering */
	_pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_FLAT );		/*< Default shading */
	_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );		/*< Culling off */ 
	_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );				/*< Lighting on */ 
	_pd3dDevice->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB(255,255,255) );
	_pd3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );			/*< Turn on z buffering */
	_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );			/*< Allow writing to the z buffer */
	_pd3dDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );		/*< Render each pixel if it's less than or equal to an existing pixel */

	ShowWindow( hWnd, UpdateWindow( hWnd ) );

	/* Set up the vertex buffer for rendering */
	Vertex* vertexBuffer = false;
	if( SUCCEEDED( _pd3dDevice->CreateVertexBuffer( 36 * sizeof( D3DFVF_Vertex ), 0, 
		D3DFVF_XYZ, D3DPOOL_DEFAULT, &_vertexBuffer, NULL ) ) )
	{
		if( SUCCEEDED( _vertexBuffer->Lock( 0, 36 * sizeof( D3DFVF_Vertex ), (void**)&vertexBuffer, 0 ) ) )
		{	
			for( DWORD i = 0; i < 36; ++i )
			{
				vertexBuffer[i].x = Cube[i].x;
				vertexBuffer[i].y = Cube[i].y;
				vertexBuffer[i].z = Cube[i].z;
			}

			_vertexBuffer->Unlock();

			/* Apply a material for lighting to work */
			D3DMATERIAL9 material;
			ZeroMemory( &material, sizeof( material ) );
			material.Diffuse.r = material.Ambient.r = 1.0f;
			material.Diffuse.g = material.Ambient.g = 1.0f;
			material.Diffuse.b = material.Ambient.b = 1.0f;
			material.Diffuse.a = material.Ambient.a = 1.0f;

			_pd3dDevice->SetMaterial( &material );
			_pd3dDevice->SetStreamSource( 0, _vertexBuffer, 0, sizeof(D3DFVF_Vertex) );
			_pd3dDevice->SetFVF( D3DFVF_XYZ );
		}
	}
#else
	_rotationSpeed = 1.0f;
	_rotation.x = 0.0f;
	_rotation.y = 0.0f;
	_rotation.z = 0.0f;
	_frame = 0;
	_viewMode = -1;
	sceGuInit();
	sceGuStart(SCEGU_IMMEDIATE, (void*)_displist, sizeof(_displist));

	sceGuDrawBuffer(SCEGU_PF5551, SCEGU_VRAM_BP_0, SCEGU_VRAM_WIDTH);
	sceGuDispBuffer(SCEGU_SCR_WIDTH, SCEGU_SCR_HEIGHT, SCEGU_VRAM_BP_1,
		SCEGU_VRAM_WIDTH);
	sceGuDepthBuffer(SCEGU_VRAM_BP_2, SCEGU_VRAM_WIDTH);

	sceGuOffset(SCEGU_SCR_OFFSETX, SCEGU_SCR_OFFSETY);
	sceGuViewport(2048, 2048, SCEGU_SCR_WIDTH, SCEGU_SCR_HEIGHT);
	sceGuDepthRange(50000, 10000);

	sceGuDisable(SCEGU_BLEND);

	sceGuScissor(0, 0, SCEGU_SCR_WIDTH, SCEGU_SCR_HEIGHT);
	sceGuEnable(SCEGU_SCISSOR_TEST);

	sceGuDepthFunc(SCEGU_GEQUAL);
	sceGuEnable(SCEGU_DEPTH_TEST);

	sceGuFrontFace(SCEGU_CW);

	sceGuDisable(SCEGU_TEXTURE);
	sceGuShadeModel(SCEGU_SMOOTH);

	sceGuEnable(SCEGU_DITHER);

	sceGumSetMatrixStack(_matrixStack, 1, 1, 2, 0);
	sceGumMatrixMode(SCEGU_MATRIX_PROJECTION);
	sceGumLoadIdentity();
	sceGumPerspective(DEGSTORADS(45.0f), SCEGU_SCR_ASPECT,
		1.000000f, 100.000000f);

	sceGumMatrixMode(SCEGU_MATRIX_WORLD);

	sceGuClearDepth(0);
	sceGuClearStencil(0);

	sceGuFinish();
	sceGuSync(SCEGU_SYNC_FINISH, SCEGU_SYNC_WAIT);

	sceGuDisplay(SCEGU_DISPLAY_ON);
#endif
}

/**
  * Created 24/02/2011
  * Prepares and clears the PSP display for rendering
  */
bool FrontEnd::StartRendering(void)
{
	//printf("FrontEnd::StartRendering\n");
	
	/* Increment the frame */
	++_frame;

	/* Check controls */
	if( _playerInput->IsLShoulderDown() )
	{
		++_viewMode;
		if( _viewMode >= 3 )
		{
			_viewMode = 0;
		}
	}
	else if( _playerInput->IsRShoulderDown() )
	{
		--_viewMode;
		if( _viewMode < 0 )
		{
			_viewMode = 2;
		}
	}
	if( _playerInput->IsDPadLeft() )
	{
		if( _rotationSpeed > 1 )
		{
			--_rotationSpeed;
		}
	}
	else if( _playerInput->IsDPadRight() )
	{
		if( _rotationSpeed < 180 )
		{
			++_rotationSpeed;
		}
	}

	/* Increment the rotation */
	switch( _viewMode )
	{
	case -1:
			break;
		case 0:
			_rotation.x += DEGSTORADS( _rotationSpeed );
			if( _rotation.x > DEGSTORADS( 360.0f ) )
			{
				_rotation.x = 0.0f;
			}
			break;
		case 1:
			_rotation.y += DEGSTORADS( _rotationSpeed );
			if( _rotation.y > DEGSTORADS( 360.0f ) )
			{
				_rotation.y = 0.0f;
			}
			break;
		case 2:
			_rotation.z += DEGSTORADS( _rotationSpeed );
			if( _rotation.z > DEGSTORADS( 360.0f ) )
			{
				_rotation.z = 0.0f;
			}
			break;
	}
	
#ifdef PC
	/* Start the device */
	_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, IntToD3DColor( _bgColour ), 1.0f, 0 );
	_pd3dDevice->BeginScene();

	/* Set up the view matrix */
	D3DXMATRIXA16 view;
	D3DXMatrixTranslation( &view, _view.x, _view.y, 25.0f );
	_pd3dDevice->SetTransform( D3DTS_VIEW, &view );

	/* Set up the projection matrix */
	D3DXMATRIXA16 projection;
	D3DXMatrixPerspectiveFovLH( &projection, DEGSTORADS(45.0f), 1.7647f, 1.0f, 100.0f ); 
	_pd3dDevice->SetTransform( D3DTS_PROJECTION, &projection );

#else
	/* Set up the starting matrices */
	//sceGumPushMatrix();
	ScePspFVector3 view;
	view.x = _view.x;
	view.y = _view.y;
	view.z = _view.z;
	sceGumLoadIdentity();
	sceGumTranslate( &view );
	//BlockUntilFrameReady();	/*< Blocks the program until the frame is ready */
	
	/* Preps the display for rendering */
	sceGuSwapBuffers();
	sceGuStart(SCEGU_IMMEDIATE, (void*)_displist, sizeof(_displist));
	sceGuClearColor(_bgColour);
	sceGuClear(SCEGU_CLEAR_ALL);
#endif

	return true;
}

/**
  * Created 24/02/2011
  * Finishes rendering
  */
void FrontEnd::StopRendering(void)
{
#ifdef PC
	_pd3dDevice->EndScene();
	_pd3dDevice->Present( NULL, NULL, NULL, NULL );
#else
	sceGumLoadIdentity();
	sceGuFinish();
	sceGuSync(SCEGU_SYNC_FINISH, SCEGU_SYNC_WAIT);
	sceGuDebugFlush();
	sceDisplayWaitVblankStart();
	//sceGumPopMatrix();
	//printf("FrontEnd::StopRendering\n");
#endif
}

/**
  * Created: 01/03/2011
  * Renders a cube to the demo space according to it's game location
  */
void FrontEnd::RenderCube(float x, float y, float z)
{
#ifdef PC
	/* Set the colour */
	_pd3dDevice->SetRenderState( D3DRS_AMBIENT, IntToD3DColor( _fgColour ) );

	D3DXMATRIXA16 rotation;
	D3DXMATRIXA16 world;
	D3DXMATRIXA16 temp;

	D3DXMatrixIdentity( &rotation );
	D3DXMatrixIdentity( &world );

	/* Create the rotation matrix */
	D3DXMatrixIdentity( &temp );
	D3DXMatrixRotationX( &temp, _rotation.y );
	rotation *= temp;
	D3DXMatrixRotationY( &temp, _rotation.z );
	rotation *= temp;
	D3DXMatrixRotationZ( &temp, _rotation.x );
	rotation *= temp;
	
	/* Translate the cube */
	D3DXMatrixIdentity( &temp );
	D3DXMatrixTranslation( &temp, -x, -y, z );
	world *= temp;

	/* Rotate the cube */
	D3DXMatrixIdentity( &temp );
	D3DXMatrixTranslation( &temp, _view.x, _view.y, -_view.z );
	world *= temp;
	world *= rotation;
	D3DXMatrixIdentity( &temp );
	D3DXMatrixTranslation( &temp, -_view.x, -_view.y, _view.z );
	world *= temp;

	/* Set the world matrix */
	_pd3dDevice->SetTransform( D3DTS_WORLD, &world );

	/* Render the cube */
	_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 36 );
#else
	ScePspFVector3 vec; 

	/* Create the rotation matrix */
	ScePspFMatrix4 rotation;
	ScePspFVector3 rotator;
	rotator.x = _rotation.x;
	rotator.y = _rotation.y;
	rotator.z = _rotation.z;
	sceGumPushMatrix();
	sceGumRotateXYZ( &rotator );
	sceGumStoreMatrix( &rotation);
	sceGumPopMatrix();

	/* Push a new matrix */
	sceGumPushMatrix();
	//printf("FrontEnd::RenderCube() started\n");

	/* Rotate the cube's translation */
	ScePspFVector3 view;
	view.x = -_view.x;
	view.y = -_view.y;
	view.z = -_view.z;
	sceGumTranslate( &view );
	sceGumRotateXYZ( &rotator );
	view.x = -view.x;
	view.y = -view.y;
	view.z = -view.z;
	sceGumTranslate( &view );

	vec.x = (float)-x;
	vec.y = (float)-y;
	vec.z = (float)-z;
	sceGumTranslate( &vec );
	
	/* Set the colour */
	sceGuColor( _fgColour );

	/* Render the cube */
	sceGumDrawArray(SCEGU_PRIM_TRIANGLES, SCEGU_VERTEX_FLOAT, 36, 0, Cube);

	/* Pop the matrix */
	sceGumPopMatrix();
#endif
}


/* Accessors */

Vertex* FrontEnd::GetCameraView(void)
{
	return &_view;
}

unsigned int FrontEnd::GetForegroundColour() const
{
	return _fgColour;
}

/* Mutators */

void FrontEnd::SetCameraView(Vertex* view)
{
	_view = *view;
}

void FrontEnd::SetForegroundRed(unsigned char colour)
{
	_fgColour &= 0xffffff00;
	_fgColour |= colour;
}

void FrontEnd::SetForegroundGreen(unsigned char colour)
{
	_fgColour &= 0xffff00ff;
	_fgColour |= colour << 8;
}

void FrontEnd::SetForegroundBlue(unsigned char colour)
{
	_fgColour &= 0xff00ffff;
	_fgColour |= colour << 16;
}

void FrontEnd::SetForegroundAlpha(unsigned char colour)
{
	_fgColour &= 0x00ffffff;
	_fgColour |= colour << 24;
}
