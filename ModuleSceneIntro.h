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
	Animation Ball_anim;
	Animation Speed_booster;
	Animation Lights_anim;
	Animation speedbooster;
	Animation Squared_Bumper;
	float speedboosterleft, speedboosterright;
	SDL_Texture* Sprites;
	uint bonus_fx;
	uint lights_fx;
	p2Point<int> ray;
	bool ray_on;
private:
	p2List<PhysBody*> Bumpers;
	p2List<PhysBody*> SpeedBoosters;
	p2List<PhysBody*> Lights;
	p2List<PhysBody*> Squared_bumpers;
	PhysJoint* launcher_joint, *Left_flipper;
	PhysBody* Ball, *Chain, *Rare_thing_left, *Rare_thing_right, *Curve_left, *Curve_right, *Center_chain, *Left_bumper_chain, *Right_bumper_chain;

	int structure_chain[126] = {
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
		55, 395,
		48, 413,
		49, 575,
		42, 582,
		28, 583,
		18, 575,
		21, 1078,
		47, 1078,
		294, 1206,
		546, 1078
	};

	int rare_thing_left[24] = {
		60, 713,
		71, 709,
		82, 712,
		88, 717,
		94, 698,
		106, 684,
		116, 677,
		118, 663,
		111, 650,
		80, 614,
		66, 614,
		57, 623
	};
	int rare_thing_right[24] = {
		513, 716,
		520, 710,
		530, 710,
		540, 713,
		541, 623,
		533, 614,
		520, 614,
		478, 660,
		477, 672,
		483, 680,
		494, 686,
		505, 696
	};
	int curve_left[16] = {
		20, 726,
		34, 756,
		49, 772,
		55, 775,
		50, 778,
		46, 776,
		27, 757,
		18, 736
	};
	int curve_right[16] = {
		579, 723,
		571, 747,
		558, 765,
		545, 772,
		547, 778,
		559, 770,
		573, 755,
		579, 744
	};

	int center_chain[24] = {
		277, 684,
		288, 678,
		305, 678,
		321, 684,
		335, 676,
		334, 660,
		320, 652,
		306, 659,
		289, 659,
		272, 652,
		259, 661,
		260, 676
	};
	int left_bumper_chain[20] = {
		103, 980,
		104, 839,
		108, 826,
		119, 822,
		131, 828,
		190, 1009,
		190, 1021,
		181, 1029,
		168, 1027,
		110, 993
	};

	int right_bumper_chain[18] = {
		495, 976,
		495, 839,
		485, 825,
		470, 826,
		462, 838,
		411, 1007,
		414, 1024,
		430, 1029,
		491, 989
	};

};
