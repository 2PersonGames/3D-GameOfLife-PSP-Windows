/**
  * Author: Sam Albon
  * Created: 21/02/2011
  * Defines the entry point of the program
  */

#pragma once
#include "FrontEnd.h"
#include "CubeData.h"
#include "InputManager.h"

#ifdef PC
#include <math.h>
#define ALWAYSRENDERCUBES false
#else
	SCE_MODULE_INFO( GameOfLife_100068439, 0, 1, 1 );
#define ALWAYSRENDERCUBES false
#endif

#ifdef PC
int WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
#else
int main(void)
#endif
{
	/* Create the backend */
	Vertex temp;
	CubeData* backend = new CubeData( 16, 16, 16 );

	/* Create the frontend */
	FrontEnd* frontend = new FrontEnd(backend->GetInput());
#ifdef PC
	frontend->StartUpDisplay( hInst );
#else
	frontend->StartUpDisplay();
#endif
	temp.x = backend->GetSize().x * 0.5f;
	temp.y = backend->GetSize().y * 0.5f;
	temp.z = -25.0f;
	frontend->SetCameraView( &temp );
	frontend->SetForegroundAlpha( 0xff );

	/* Enter update-render loop */
	while( true )
	{
		/* Update backend */
		backend->Update();

		/* Prepare the frontend world matrix */
		frontend;
		
		/* Render cubes */
		frontend->StartRendering();
		temp.x = backend->GetSize().x * 0.5f;
		temp.y = backend->GetSize().y * 0.5f;
		temp.z = backend->GetSize().z * 0.5f;
		frontend->SetCameraView( &temp );
		for( unsigned int i = 0; i < backend->GetSize().z; ++i )
		{
			/* Set the new colour */
#ifdef PC
			frontend->SetForegroundBlue( (unsigned char)floor(((float)i / (float)backend->GetSize().z) * 255.0f) );
#else
			frontend->SetForegroundBlue( (((float)i / backend->GetSize().z) * 255.0f) );
#endif

			for( unsigned int j = 0; j < backend->GetSize().y; ++j)
			{
				/* Check if the line is even alive */
				if( backend->GetLineData( j, i ) || ALWAYSRENDERCUBES )
				{
					/* Set the new colour */
#ifdef PC
					frontend->SetForegroundGreen( (unsigned char)floor(((float)j / (float)backend->GetSize().y) * 255.0f) );
#else
					frontend->SetForegroundGreen( (((float)j / backend->GetSize().y) * 255.0f) );
#endif

					for( unsigned int k = 0; k < backend->GetSize().x; ++k )
					{
						/* Check if this cube is alive */
						if( backend->GetCubeState( k, j, i ) || ALWAYSRENDERCUBES )
						{
							/* Set new colour */
#ifdef PC
							frontend->SetForegroundRed( (unsigned char)floor(((float)k / (float)backend->GetSize().x) * 255.0f) );
#else
							frontend->SetForegroundRed( (((float)k / backend->GetSize().x) * 255.0f) );
#endif

							/* Render the cube */
							frontend->RenderCube( k, j, i );
						}
					}
				}
			}
		}
		frontend->StopRendering();
		temp.x = backend->GetSize().x * 0.5f;
		temp.y = backend->GetSize().y * 0.5f;
		temp.z = -25.0f;
		frontend->SetCameraView( &temp );

		if( backend->GetInput()->IsSelectDown() )
		{
			return 0;
		}
	}

	return 0;
}
