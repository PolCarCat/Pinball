#pragma once
#include "Module.h"
#include "Globals.h"

#define MAX_MOUSE_BUTTONS 5
#define MAX_KEYS 300

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP,
	KEY_ERROR
};

enum EVENT_WINDOW
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

class ModuleInput : public Module
{
public:
	
	ModuleInput(Application* app, bool start_enabled = true);
	~ModuleInput();

	bool Init();
	update_status PreUpdate();
	bool CleanUp();

	KEY_STATE GetKey(uint id) const
	{
		return (id < MAX_KEYS) ? keyboard[id] : KEY_ERROR;
	}

	KEY_STATE GetMouseButton(uint id) const
	{
		return (id < MAX_MOUSE_BUTTONS) ? mouse_buttons[id] : KEY_ERROR;
	}

	int GetMouseX() const
	{
		return mouse_x;
	}

	int GetMouseY() const
	{
		return mouse_y;
	}

private:
	bool		windowEvents[WE_COUNT];
	KEY_STATE*	keyboard;
	KEY_STATE	mouse_buttons[MAX_MOUSE_BUTTONS];
	int			mouse_motion_x;
	int			mouse_motion_y;
	int			mouse_x;
	int			mouse_y;
};