#include "InputManager.h"

#ifdef PC
#include <math.h>
#else
#define TESTCONTROLS(mask) (_currentInput.Buttons & mask) != 0x0 && !(_previousInput.Buttons & mask) == 0x0
#endif

/* Constructors */

InputManager::InputManager(void)
{
#ifdef PC
	_currentInput.q = false;
	_currentInput.e = false;
	_currentInput.r = false;
	_previousInput.q = false;
	_previousInput.e = false;
	_previousInput.r = false;
#endif
}

InputManager::~InputManager(void)
{
}

/* Functions */

void InputManager::Tick(void)
{
	/* Archive the old input state */
	_previousInput = _currentInput;

#ifdef PC
	/* Get the next packet message */
	MSG msg;
	bool message = false;
	ZeroMemory( &msg, sizeof(msg) );

	/* Set up the correct timing loop */
	LARGE_INTEGER currentTime;
	QueryPerformanceFrequency( &currentTime );
	DWORD frameTime = (DWORD)floor( currentTime.QuadPart / 30.0f );
	currentTime.QuadPart = 0;

	while( message = PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) ||
		  ( QueryPerformanceCounter( &currentTime ) && 
		    currentTime.QuadPart < _lastTime.QuadPart + frameTime ) )
	{
		if( message )
		{				
			TranslateMessage( &msg );					
			DispatchMessage( &msg );

			switch( msg.message )
			{
				case WM_CLOSE:
					_currentInput.r = true;
					_previousInput.r = false;

					MessageBox( 0, L"Closing program", 0, 0 );
					return;
				case WM_KEYDOWN:
					KeyPressed( MapVirtualKey( msg.wParam, MAPVK_VK_TO_CHAR ) );
					break;
				case WM_KEYUP:
					KeyReleased( MapVirtualKey( msg.wParam, MAPVK_VK_TO_CHAR ) );
					break;
			}
		}
	}
	_lastTime = currentTime;
#endif

#ifdef PSP
	/* Retrieve the current gamepad state */
	sceCtrlReadBufferPositive( &_currentInput, 1 );
#endif
}

/* Accessors */

bool InputManager::IsDPadDown(void)
{
#ifdef PC
	return TestKey('S', _currentInput);
#else
	return TESTCONTROLS( SCE_CTRL_DOWN );
#endif
}

bool InputManager::IsDPadLeft(void)
{
#ifdef PC
	return TestKey('A', _currentInput);
#else
	return TESTCONTROLS( SCE_CTRL_LEFT );
#endif
}

bool InputManager::IsDPadUp(void)
{
#ifdef PC
	return TestKey('W', _currentInput);
#else
	return TESTCONTROLS( SCE_CTRL_UP );
#endif
}

bool InputManager::IsDPadRight(void)
{
#ifdef PC
	return TestKey('D', _currentInput);
#else
	return TESTCONTROLS( SCE_CTRL_RIGHT );
#endif
}

bool InputManager::IsRShoulderDown(void)
{
#ifdef PC
	return TestKey('E', _currentInput);
#else
	return TESTCONTROLS( SCE_CTRL_L );
#endif
}

bool InputManager::IsLShoulderDown(void)
{
#ifdef PC
	return TestKey('Q', _currentInput);
#else
	return TESTCONTROLS( SCE_CTRL_R );
#endif
}

bool InputManager::IsSelectDown(void)
{
#ifdef PC
	if( TestKey('R', _currentInput) && !TestKey('R', _previousInput) )
	{
		return true;
	}
	else
	{
		return false;
	}
#else
	return TESTCONTROLS( SCE_CTRL_SELECT );
#endif
}

bool InputManager::IsStartDown(void)
{
#ifdef PC
	if( TestKey('Y', _currentInput) && !TestKey('Y', _currentInput) )
	{
		return true;
	}
	else
	{
		return false;
	}
#else
	return TESTCONTROLS( SCE_CTRL_START );
#endif
}

bool InputManager::IsCrossDown(void)
{
#ifdef PC
	return TestKey('G', _currentInput);
#else
	return TESTCONTROLS( SCE_CTRL_CROSS );
#endif
}

bool InputManager::IsTriangleDown(void)
{
#ifdef PC
	return TestKey('T', _currentInput);
#else
	return TESTCONTROLS( SCE_CTRL_TRIANGLE );
#endif
}

bool InputManager::IsSquareDown(void)
{
#ifdef PC
	return TestKey('F', _currentInput);
#else
	return TESTCONTROLS( SCE_CTRL_SQUARE );
#endif
}

bool InputManager::IsCircleDown(void)
{
#ifdef PC
	return TestKey('H', _currentInput);
#else
	return TESTCONTROLS( SCE_CTRL_CIRCLE );
#endif
}


#ifdef PC
bool InputManager::TestKey(char character, KeyboardState input)
{
	unsigned long long mask = 0x1;
	mask <<= character;

	/* Create the mask and test the appropriate state */
	switch( character )
	{
	case 'Q':
		return input.q;
	case 'E':
		return input.e;
	case 'R':
		return input.r;
	}
}
void InputManager::KeyPressed(char character)
{
	switch( character )
	{
	case 'Q':
		_currentInput.q = true;
		break;
	case 'E':
		_currentInput.e = true;
		break;
	case 'R':
		_currentInput.r = true;
		break;
	}
}
void InputManager::KeyReleased(char character)
{
	switch( character )
	{
	case 'Q':
		_currentInput.q = false;
		break;
	case 'E':
		_currentInput.e = false;
		break;
	case 'R':
		_currentInput.r = false;
		break;
	}
}
#endif
