/**
  * Author: Sam Albon
  * Created: 24/02/2011
  * Contains all static data and prototyes related to
  * rendering on the PSP display
  */

#pragma once
#include "InputManager.h"
#ifdef PC
#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#else
#include <libgu.h>
#include <libgum.h>
#include <kernel.h>
#endif

/* Default vertex information */
typedef struct
{
	float x;
	float y;
	float z;
} Vertex;

#ifdef PC
#define DEGSTORADS(value) D3DXToRadian(value)
#define IntToD3DColor(colour) D3DXCOLOR( (float)(0xff & colour) / 255.0f, (float)(0xff & (colour >> 8)) / 255.0f, (float)(0xff & (colour >> 16)) / 255.0f, (float)(0xff & (colour >> 24)) / 255.0f)
#define D3DFVF_Vertex (D3DFVF_XYZ)
#else
#define DEGSTORADS(value) SCEGU_RAD(value)
#endif

class FrontEnd
{
public:
	FrontEnd(InputManager*);
	~FrontEnd(void);

	/* Functions */
#ifdef PC
	void StartUpDisplay(HINSTANCE);
	static LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
#else
	void StartUpDisplay(void);
#endif
	bool StartRendering(void);
	void StopRendering(void);
	void RenderCube(float, float, float);

	/* Mutators */
	void SetForegroundRed(unsigned char);
	void SetForegroundGreen(unsigned char);
	void SetForegroundBlue(unsigned char);
	void SetForegroundAlpha(unsigned char);
	void SetCameraView(Vertex*);
	
	/* Accessors */
	unsigned GetForegroundColour(void) const;
	Vertex* GetCameraView(void);

private:
	/* Render variables */
	unsigned int	_bgColour;
	unsigned int	_fgColour;
	Vertex			_rotation;
	float			_rotationSpeed;
	char			_viewMode;
	unsigned char	_frame;
	Vertex			_view;
	InputManager*	_playerInput;

#ifdef PC
	LPDIRECT3D9					_pD3D;
	LPDIRECT3DDEVICE9			_pd3dDevice;
	D3DPRESENT_PARAMETERS		_d3dParameters;
	WNDCLASSEX					_wc;
	//LPD3DXFONT					_fontFace;
	LPDIRECT3DVERTEXBUFFER9		_vertexBuffer;
#endif
};

/* Static render variables */
#ifdef PC
#else
char			_displist[0x100000] __attribute__((aligned(64)));
ScePspFMatrix4	_matrixStack[1+1+2];
#endif

/* Cube data */
static const Vertex Cube[36] = 
{
	{
		0.330000f, 0.330000f, -0.330000f,	/* 0 */
	},
	{
		0.330000f, 0.330000f, 0.330000f,	/* 1 */
	},
	{
		-0.330000f, 0.330000f, -0.330000f,	/* 2 */
	},

	{
		-0.330000f, 0.330000f, -0.330000f,	/* 2 */
	},
	{
		0.330000f, 0.330000f, 0.330000f,	/* 1 */
	},
	{
		-0.330000f, 0.330000f, 0.330000f,	/* 3 */
	},

	{
		-0.330000f, 0.330000f, -0.330000f,	/* 2 */
	},
	{
		-0.330000f,0.330000f, 0.330000f,	/* 3 */
	},
	{
		-0.330000f, -0.330000f, -0.330000f,	/* 4 */
	},

	{
		-0.330000f, -0.330000f, -0.330000f,	/* 4 */
	},
	{
		-0.330000f, 0.330000f, 0.330000f,	/* 3 */
	},
	{
		-0.330000f, -0.330000f, 0.330000f,	/* 5 */
	},

	{
		-0.330000f, 0.330000f, 0.330000f,	/* 3 */
	},
	{
		0.330000f, 0.330000f, 0.330000f,	/* 1 */
	},
	{
		-0.330000f, -0.330000f, 0.330000f,	/* 5 */
	},

	{
		-0.330000f, -0.330000f, 0.330000f,	/* 5 */
	},
	{
		0.330000f, 0.330000f, 0.330000f,	/* 1 */
	},
	{
		0.330000f, -0.330000f, 0.330000f,	/* 6 */
	},

	{
		0.330000f, 0.330000f, 0.330000f,	/* 1 */
	},
	{
		0.330000f, 0.330000f, -0.330000f,	/* 0 */
	},
	{
		0.330000f, -0.330000f, 0.330000f,	/* 6 */
	},

	{
		0.330000f, -0.330000f, 0.330000f,	/* 6 */
	},
	{
		0.330000f, 0.330000f, -0.330000f,	/* 0 */
	},
	{
		0.330000f, -0.330000f, -0.330000f,	/* 7 */
	},

	{
		0.330000f, 0.330000f, -0.330000f,	/* 0 */
	},
	{
		-0.330000f, 0.330000f, -0.330000f,	/* 2 */
	},
	{
		0.330000f, -0.330000f, -0.330000f,	/* 7 */
	},

	{
		0.330000f, -0.330000f, -0.330000f,	/* 7 */
	},
	{
		-0.330000f, 0.330000f, -0.330000f,	/* 2 */
	},
	{
		-0.330000f, -0.330000f, -0.330000f,	/* 4 */
	},

	{
		-0.330000f, -0.330000f, -0.330000f,	/* 4 */
	},
	{
		-0.330000f, -0.330000f,  0.330000f,	/* 5 */
	},
	{
		0.330000f, -0.330000f, -0.330000f,	/* 7 */
	},

	{
		0.330000f, -0.330000f, -0.330000f,	/* 7 */
	},
	{
		-0.330000f, -0.330000f, 0.330000f,	/* 5 */
	},
	{
		0.330000f, -0.330000f, 0.330000f,	/* 6 */
	}
};
