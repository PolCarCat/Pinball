#include "ModuleInterface.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleFonts.h"
#include <stdlib.h>
#include <stdio.h>
#include "SDL\include\SDL_timer.h"


ModuleInterface::ModuleInterface(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


ModuleInterface::~ModuleInterface()
{
}

update_status ModuleInterface::PostUpdate() {

	for (uint i = 0; i < MAX_LABELS; i++) {
		Label* label = labels[i];
		if (label == nullptr)
			continue;

		if (label->life < 0) {
			App->fonts->BlitText(label);
			continue;
		}
		
		uint ticks = SDL_GetTicks();
		if (ticks > label->start_time + label->tickrate) {
			label->visible = !label->visible;
			label->start_time = ticks;
		}

		if (ticks > label->born + label->life) {
			RemoveLabel(label);
			continue;
		}

		if (label->visible)
			App->fonts->BlitText(label);
	}

	return UPDATE_CONTINUE;
}

bool ModuleInterface::CleanUp() {
	for (uint i = 0; i < MAX_LABELS; i++) {
		if (labels[i] != nullptr) {
			//delete[] labels[i]->string;
			delete labels[i];
			labels[i] = nullptr;
		}
	}

	return true;
}

int ModuleInterface::AddLabel(int id, const char* text, int posx, int posy, Text_alignment alignment, int life) {
	bool ret = false;

	for (int i = 0; i < MAX_LABELS; i++) {
		if (labels[i] == nullptr) {
			labels[i] = new Label();
			labels[i]->font_id = id;
			labels[i]->pos = { posx, posy };
			labels[i]->string = text;
			labels[i]->tickrate = 333;
			labels[i]->alignment = alignment;
			labels[i]->born = labels[i]->start_time = SDL_GetTicks();
			labels[i]->life = life;
			return i;
		}
	}
}

Label* ModuleInterface::getLabel(int id) {
	return labels[id];
}

bool ModuleInterface::RemoveLabel(Label* label) {
	if (label == nullptr)
		return false;

	for (uint i = 0; i < MAX_LABELS; i++) {
		if (label == labels[i]) {
			delete labels[i];
			labels[i] = nullptr;
			return true;
		}
	}
}

bool ModuleInterface::RemoveLabel(int label_id) {
	if (labels[label_id] == nullptr)
		return false;

	delete labels[label_id];
	labels[label_id] = nullptr;
	return true;
}