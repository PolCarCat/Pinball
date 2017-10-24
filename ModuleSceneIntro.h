#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

class PhysBody;
class PhysJoint;
class Object;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:


	PhysBody* end_game_sensor;
	bool sensed;

	SDL_Texture* background;
	Animation Bumper;
	SDL_Texture* Sprites;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
private:
	p2List<Object*> Bumpers;
	PhysJoint* launcher_joint;
	PhysBody* Ball;

	int structure_chain[120] = {
	578, 1070,
	580, 568,
	572, 580,
	564, 584,
	551, 580,
	546, 555,
	548, 408,
	542, 393,
	525, 375,
	513, 371,
	509, 348,
	495, 326,
	472, 316,
	473, 241,
	467, 230,
	453, 225,
	500, 180,
	510, 177,
	524, 183,
	546, 216,
	561, 250,
	571, 288,
	577, 329,
	577, 372,
	581, 364,
	587, 362,
	592, 371,
	592, 1185,
	630, 1185,
	630, 331,
	607, 230,
	568, 155,
	517, 102,
	471, 70,
	405, 41,
	342, 29,
	260, 33,
	200, 51,
	135, 85,
	79, 140,
	54, 177,
	67, 169,
	80, 171,
	138, 222,
	130, 234,
	122, 244,
	122, 309,
	125, 321,
	120, 327,
	102, 334,
	90, 353,
	86, 371,
	71, 380,
	53, 396,
	49, 414,
	48, 570,
	41, 583,
	25, 583,
	19, 574,
	21, 1078,

		};
};
