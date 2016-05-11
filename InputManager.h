#pragma once
#ifdef PC
#include <Windows.h>
#else
#include <ctrlsvc.h>
#endif

typedef struct
{
	bool q;
	bool e;
	bool r;
} KeyboardState;

class InputManager
{
public:

	/* Constructors */
	InputManager(void);
	~InputManager(void);

	/* Functions */
	void Tick(void);

	/* Accessors */
	bool IsCrossDown(void);
	bool IsSquareDown(void);
	bool IsCircleDown(void);
	bool IsTriangleDown(void);
	bool IsDPadUp(void);
	bool IsDPadDown(void);
	bool IsDPadLeft(void);
	bool IsDPadRight(void);
	bool IsSelectDown(void);
	bool IsStartDown(void);
	bool IsLShoulderDown(void);
	bool IsRShoulderDown(void);

private:
#ifdef PC
	KeyboardState	_currentInput;
	KeyboardState	_previousInput;
	bool TestKey(char, KeyboardState);
	void KeyPressed(char);
	void KeyReleased(char);
	bool IsKeyDown(char);
	LARGE_INTEGER _lastTime;
#else
	SceCtrlData		_currentInput;
	SceCtrlData		_previousInput;
#endif
};
