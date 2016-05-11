/**
  * Author: Sam Albon
  * Created: 21/02/2011
  * Defines functions related to modifying the data related to the grid.
  */

#include "CubeData.h"
#ifdef PSP
#include <Allegrex.h>
#endif

CubeData::CubeData(unsigned char width, unsigned char height, unsigned char depth)
{
	/* Create the player input controller */
	_playerInput = new InputManager();

	/* Initialise the GOL grid */
	_size.x = width > 32 ? 32 : width;	/* Clamp to the max size of an unsigned int */
	_size.y = height > 32 ? 32 : height;
	_size.z = depth > 32 ? 32 : depth;
	_grid = new LineData[ height * depth ];

	/* Default cubes */
	for( unsigned int i = 0; i < DEPTH * WIDTH; ++i )
	{
		_grid[i].current = 0x0;
	}
	
	/* Exploder */
	_grid[6].current = 0x540;
	_grid[7].current = 0x440;
	_grid[8].current = 0x440;
	_grid[9].current = 0x440;
	_grid[10].current = 0x540;
}

CubeData::~CubeData(void)
{
	/* Delete the GOL grid */
	if( _grid )
	{
		delete [] _grid;
		_grid = false;
	}

	/* Delete the input manager */
	if( _playerInput )
	{
		_playerInput->~InputManager();
		delete _playerInput;
		_playerInput = false;
	}
}

/* Mutators */

/**
  * Created: 24/02/2011
  * Inverts the state of the cube at that location in the grid
  * Inline for efficiency
  */
void CubeData::SetCubeState(CartesianCoordinate location)
{
	unsigned int mask = 0x1;	/* Creates a mask of the value ...0001 */
	mask <<= location.x;		/* Shifts the mask along to meet the value being set */
	
	/* Masks the line to invert the coordinate's state */
	_grid[(location.z * DEPTH) + location.y].current ^= mask;	
}

/* Accessors */

unsigned int CubeData::GetLineData(unsigned char y, unsigned char z)
{
	return _grid[ ( z * DEPTH ) + y ].current; 
}

CartesianCoordinate CubeData::GetSize(void) const
{
	return _size;
}

/**
  * Created: 24/02/2011
  * Returns a boolean of alive
  * Needs changing for efficiency
  * Inline for efficiency
  */
bool CubeData::GetCubeState(unsigned char x, unsigned char y, unsigned char z)
{
	unsigned int mask = 0x1;	/*< Creates a mask of the value ...0001 */
	mask <<= x;		/*< Shifts the mask along to meet the value required */
	
	/* Masks the line and returns the result */
	return _grid[ ( z * DEPTH ) + y ].previous & mask;
}

/* Functions */

void CubeData::Update()
{
	/* Update input manager */
	_playerInput->Tick();

	/* Declare the counters */
	CartesianCoordinate location;

	/* Load the grid into cache */
#ifdef PSP
	for (location.z = 0; location.z < DEPTH; ++location.z)
	{
		for (location.y = 0; location.y < WIDTH; ++location.y)
		{
			__builtin_allegrex_cache(SCE_ALLEGREX_CACHEOP_FILL_WITH_LOCK_I, &_grid[(location.z * DEPTH) + location.y].previous);
		}
	}
#endif
	
	/* Iterate through the grid */
	for (location.z = 0; location.z < DEPTH; ++location.z)
	{
		for (location.y = 0; location.y < WIDTH; ++location.y)
		{
			for (location.x = 0; location.x < HEIGHT; ++location.x)
			{
				CheckNeighbours(location);
			}
		}
	}
	
	/* Archive all working states */
	for (location.z = 0; location.z < DEPTH; ++location.z)
	{
		for (location.y = 0; location.y < WIDTH; ++location.y)
		{
			_grid[(location.z * DEPTH) + location.y].previous = _grid[(location.z * DEPTH) + location.y].current;
		}
	}
	
	/* Force a write back */
#ifdef PSP
	for (location.z = 0; location.z < DEPTH; ++location.z)
	{
		for (location.y = 0; location.y < WIDTH; ++location.y)
		{
			__builtin_allegrex_cache(SCE_ALLEGREX_CACHEOP_INDEX_UNLOCK_I, &_grid[(location.z * DEPTH) + location.y].previous);
		}
	}
#endif
}

/**
  * Created: 24/02/2011
  * Checks the neighbours of a cube at that location and applies the GOL rules
  * Needs changing for efficiency
  * Inline for efficiency
  */
void CubeData::CheckNeighbours(CartesianCoordinate location)
{
	/* Declare counters */
	char z;
	char i;
	unsigned char living;
	
	/* Iterate through the neighbours */
	living = 0;
	for (i = -1; i < 2; ++i)
	{
		/* Test z axis bounds */
		z = location.z + i;
		if (z >= 0 && z < DEPTH)
		{
			/* Test x axis bounds */
			if (location.x > 0)			/*< Left */
			{
				/* Top Left */
				if (location.y > 0 && GetCubeState(location.x - 1, location.y - 1, z) > 0)
				{
					++living;
				}
				/* Middle Left */
				if (GetCubeState(location.x - 1, location.y, z) > 0)
				{
					++living;
				}
				/* Bottom Left */
				if (location.y < WIDTH - 1 && GetCubeState(location.x - 1, location.y + 1, z) > 0)
				{
					++living;
				}
			}
			/* Test x axis bounds */
			if (location.x < HEIGHT - 1)	/*< Right */
			{
				if (location.y > 0 && GetCubeState(location.x + 1, location.y - 1, z) > 0)
				{
					++living;
				}
				if (GetCubeState(location.x + 1, location.y, z) > 0)
				{
					++living;
				}
				if (location.y < WIDTH - 1 && GetCubeState(location.x + 1, location.y + 1, z) > 0)
				{
					++living;
				}
			}
			/* Test y axis bounds */
			if (location.y > 0 && GetCubeState(location.x, location.y - 1, z) > 0)
			{
				++living;
			}
			if (location.y < WIDTH - 1 && GetCubeState(location.x, location.y + 1, z) > 0)
			{
				++living;
			}
			if (z != location.z && GetCubeState(location.x, location.y, z))
			{
				++living;
			}
			
			/* Test if living is greater than 3 */
			if (living > 3)
			{
				break;
			}
		}
	}
	
	/* Apply rules */
	if (GetCubeState(location.x, location.y, location.z) == 0)
	{
		/* Cube is currently dead */
		if (living == 3)
		{
			SetCubeState(location);	/*< Set state to alive */
		}
		return;	/* Possible make the function more efficient */
	}
	else if (living == 2 || living == 3)
	{
		/* Cube is currently alive */
		return;	/* Possible make the function more efficient */
	}
	else
	{
		SetCubeState(location);	/*< Set state to dread */
		return;
	}
}


InputManager* CubeData::GetInput(void)
{
	return _playerInput;
}