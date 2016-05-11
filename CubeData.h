/**
  * Author: Sam Albon
  * Created: 21/02/2011
  * Defines the struct for the grid, as well as functions related to
  * 		modifying the data related to the grid.
  */

#pragma once
#include "InputManager.h"

/* Macros for porting */
#define WIDTH _size.x
#define HEIGHT _size.y
#define DEPTH _size.z

/* Locations in the grid */
typedef struct
{
	unsigned char x;
	unsigned char y;
	unsigned char z;
} CartesianCoordinate;

/* Game states */
typedef struct
{
	unsigned int current;
	unsigned int previous;
} LineData;

#define MAXUPDATESPEED 30
#define TESTCONTROLS($value) (input.Buttons & $value) != 0x0 && (_previousController.Buttons & $value) == 0x0

class CubeData
{
public:
	CubeData(unsigned char, unsigned char, unsigned char);
	~CubeData(void);

	/* Functions */
	void Update(void);

	/* Accessors */
	unsigned int GetLineData(unsigned char, unsigned char);
	bool GetCubeState(unsigned char, unsigned char, unsigned char);
	CartesianCoordinate GetSize(void) const;
	InputManager* GetInput(void);

	/* Mutators */
	void SetCubeState(CartesianCoordinate);

private:
	/* GOL Variables */
	LineData*			_grid;
	CartesianCoordinate	_size;

	/* Player controls */
	InputManager*		_playerInput;

	/* Functions */
	void CheckNeighbours(CartesianCoordinate);
};
