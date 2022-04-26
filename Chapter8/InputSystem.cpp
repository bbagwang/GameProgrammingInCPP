// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "InputSystem.h"
#include <SDL/SDL.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

bool KeyboardState::GetKeyValue(SDL_Scancode keyCode) const
{
	return mCurrState[keyCode] == 1;
}

ButtonState KeyboardState::GetKeyState(SDL_Scancode keyCode) const
{
	if (mPrevState[keyCode] == 0)
	{
		if (mCurrState[keyCode] == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else // Prev state must be 1
	{
		if (mCurrState[keyCode] == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}

bool MouseState::GetButtonValue(int button) const
{
	return (SDL_BUTTON(button) & mCurrButtons) == 1;
}

ButtonState MouseState::GetButtonState(int button) const
{
	int mask = SDL_BUTTON(button);
	if ((mask & mPrevButtons) == 0)
	{
		if ((mask & mCurrButtons) == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else
	{
		if ((mask & mCurrButtons) == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}

bool ControllerState::GetButtonValue(SDL_GameControllerButton button) const
{
	return mCurrButtons[button] == 1;
}

ButtonState ControllerState::GetButtonState(SDL_GameControllerButton button) const
{
	if (mPrevButtons[button] == 0)
	{
		if (mCurrButtons[button] == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else // Prev state must be 1
	{
		if (mCurrButtons[button] == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}

bool InputSystem::Initialize()
{
	// Keyboard
	// Assign current state pointer
	mState.Keyboard.mCurrState = SDL_GetKeyboardState(NULL);
	// Clear previous state memory
	memset(mState.Keyboard.mPrevState, 0,
		SDL_NUM_SCANCODES);

	// Mouse (just set everything to 0)
	mState.Mouse.mCurrButtons = 0;
	mState.Mouse.mPrevButtons = 0;

	for (int i = 0; i < MAX_CONTROLLER_COUNT; i++)
	{
		// Get the connected controller, if it exists
		
		if (SDL_IsGameController(i))
		{
			mControllers[i] = SDL_GameControllerOpen(i);
		}

		// Initialize controller state
		mState.Controllers[i].mIsConnected = (mControllers[i] != nullptr);
		memset(mState.Controllers[i].mCurrButtons, 0,
			SDL_CONTROLLER_BUTTON_MAX);
		memset(mState.Controllers[i].mPrevButtons, 0,
			SDL_CONTROLLER_BUTTON_MAX);
	}

	LoadInputMapping();

	return true;
}

void InputSystem::Shutdown()
{
}

void InputSystem::PrepareForUpdate()
{
	// Copy current state to previous
	// Keyboard
	memcpy(mState.Keyboard.mPrevState,
		mState.Keyboard.mCurrState,
		SDL_NUM_SCANCODES);

	// Mouse
	mState.Mouse.mPrevButtons = mState.Mouse.mCurrButtons;
	mState.Mouse.mIsRelative = false;
	mState.Mouse.mScrollWheel = Vector2::Zero;

	for (int i = 0; i < MAX_CONTROLLER_COUNT; i++)
	{
		// Controller
		memcpy(mState.Controllers[i].mPrevButtons,
			mState.Controllers[i].mCurrButtons,
			SDL_CONTROLLER_BUTTON_MAX);
	}
}

void InputSystem::Update()
{
	// Mouse
	int x = 0, y = 0;
	if (mState.Mouse.mIsRelative)
	{
		mState.Mouse.mCurrButtons = 
			SDL_GetRelativeMouseState(&x, &y);
	}
	else
	{
		mState.Mouse.mCurrButtons = 
			SDL_GetMouseState(&x, &y);
	}

	mState.Mouse.mMousePos.x = static_cast<float>(x);
	mState.Mouse.mMousePos.y = static_cast<float>(y);

	// Controller
	// Buttons
	
	for (int i = 0; i < MAX_CONTROLLER_COUNT; i++)
	{
		for (int j = 0; j < SDL_CONTROLLER_BUTTON_MAX; j++)
		{
			mState.Controllers[i].mCurrButtons[j] =
				SDL_GameControllerGetButton(mControllers[i],
					SDL_GameControllerButton(j));
		}

		// Triggers
		mState.Controllers[i].mLeftTrigger =
			Filter1D(SDL_GameControllerGetAxis(mControllers[i],
				SDL_CONTROLLER_AXIS_TRIGGERLEFT));
		mState.Controllers[i].mRightTrigger =
			Filter1D(SDL_GameControllerGetAxis(mControllers[i],
				SDL_CONTROLLER_AXIS_TRIGGERRIGHT));

		// Sticks
		x = SDL_GameControllerGetAxis(mControllers[i],
			SDL_CONTROLLER_AXIS_LEFTX);
		y = -SDL_GameControllerGetAxis(mControllers[i],
			SDL_CONTROLLER_AXIS_LEFTY);
		mState.Controllers[i].mLeftStick = Filter2D(x, y);

		x = SDL_GameControllerGetAxis(mControllers[i],
			SDL_CONTROLLER_AXIS_RIGHTX);
		y = -SDL_GameControllerGetAxis(mControllers[i],
			SDL_CONTROLLER_AXIS_RIGHTY);
		mState.Controllers[i].mRightStick = Filter2D(x, y);
	}
}

void InputSystem::ProcessEvent(SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEWHEEL:
		mState.Mouse.mScrollWheel = Vector2(
			static_cast<float>(event.wheel.x),
			static_cast<float>(event.wheel.y));
		break;


	case SDL_CONTROLLERDEVICEADDED:
		OnDeviceConnected(event.cdevice);
		break;
	case SDL_CONTROLLERDEVICEREMOVED:
		OnDeviceDisconnected(event.cdevice);
		break;
	default:
		break;
	}
}

void InputSystem::OnDeviceConnected(SDL_ControllerDeviceEvent& Device)
{
	RefreshControllers();
}

void InputSystem::OnDeviceDisconnected(SDL_ControllerDeviceEvent& Device)
{
	RefreshControllers();
}

void InputSystem::RefreshControllers()
{
	for (int i = 0; i < MAX_CONTROLLER_COUNT; i++)
	{
		mControllers[i] = nullptr;

		if (SDL_IsGameController(i))
		{
			mControllers[i] = SDL_GameControllerOpen(i);
		}

		mState.Controllers[i].mIsConnected = (mControllers[i] != nullptr);
		
		memset(mState.Controllers[i].mCurrButtons, 0, SDL_CONTROLLER_BUTTON_MAX);
		memset(mState.Controllers[i].mPrevButtons, 0, SDL_CONTROLLER_BUTTON_MAX);
	}
}

void InputSystem::LoadInputMapping()
{
	std::ifstream InputMappingFile("InputMapping.txt");
	std::string str;

	if (InputMappingFile.is_open())
	{
		InputMappingFile >> str;
	}

	std::istringstream ss(str);
	std::string token;

	std::string ActionName;
	std::string InputType;
	std::string KeyName;

	enum class STEP { ACTION_NAME = 0, INPUT_TYPE = 1, KEY_NAME = 2, ASSEMBLY = 3 };
	STEP Step = STEP::ACTION_NAME;

	while (std::getline(ss, token, ','))
	{
		switch (Step)
		{
		case STEP::ACTION_NAME:
			ActionName = token;
			Step = STEP::INPUT_TYPE;
			break;
		case STEP::INPUT_TYPE:
			InputType = token;
			Step = STEP::KEY_NAME;
			break;
		case STEP::KEY_NAME:
		{
			KeyName = token;

			InputBindData NewBindData;
			auto FindResultIter = InputBindMap.find(ActionName);
			if (FindResultIter != InputBindMap.end())
			{
				NewBindData = FindResultIter->second;
			}

			if (InputType == "Controller")
			{
				if (KeyName == "A")
				{
					NewBindData.ControllerButton = SDL_CONTROLLER_BUTTON_A;
				}
			}
			else if (InputType == "KeyBoard")
			{
				if (KeyName == "Space")
				{
					NewBindData.KeyboardButton = SDL_SCANCODE_SPACE;
				}
			}

			InputBindMap.insert({ ActionName, NewBindData });

			InputMappingFile >> str;

			Step = STEP::ACTION_NAME;
		}
		default:
			break;
		}
	}
}

void InputSystem::SetRelativeMouseMode(bool value)
{
	SDL_bool set = value ? SDL_TRUE : SDL_FALSE;
	SDL_SetRelativeMouseMode(set);

	mState.Mouse.mIsRelative = value;
}

ButtonState InputSystem::GetMappedButtonState(const std::string& ActionName, int ControllerDeviceID /*= 0*/)
{
	auto ResultIter = InputBindMap.find(ActionName);
	if (ResultIter != InputBindMap.end())
	{
		return mState.Controllers[ControllerDeviceID].GetButtonState(ResultIter->second.ControllerButton);
	}

	return ButtonState::ENone;
}

float InputSystem::Filter1D(int input)
{
	// A value < dead zone is interpreted as 0%
	const int deadZone = 250;
	// A value > max value is interpreted as 100%
	const int maxValue = 30000;

	float retVal = 0.0f;

	// Take absolute value of input
	int absValue = input > 0 ? input : -input;
	// Ignore input within dead zone
	if (absValue > deadZone)
	{
		// Compute fractional value between dead zone and max value
		retVal = static_cast<float>(absValue - deadZone) /
		(maxValue - deadZone);
		// Make sure sign matches original value
		retVal = input > 0 ? retVal : -1.0f * retVal;
		// Clamp between -1.0f and 1.0f
		retVal = Math::Clamp(retVal, -1.0f, 1.0f);
	}

	return retVal;
}

Vector2 InputSystem::Filter2D(int inputX, int inputY)
{
	const float deadZone = 8000.0f;
	const float maxValue = 30000.0f;

	// Make into 2D vector
	Vector2 dir;
	dir.x = static_cast<float>(inputX);
	dir.y = static_cast<float>(inputY);

	float length = dir.Length();

	// If length < deadZone, should be no input
	if (length < deadZone)
	{
		dir = Vector2::Zero;
	}
	else
	{
		// Calculate fractional value between
		// dead zone and max value circles
		float f = (length - deadZone) / (maxValue - deadZone);
		// Clamp f between 0.0f and 1.0f
		f = Math::Clamp(f, 0.0f, 1.0f);
		// Normalize the vector, and then scale it to the
		// fractional value
		dir *= f / length;
	}

	return dir;
}
