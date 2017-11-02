#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{

	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;
 
	background = App->textures->Load("Sprites/background.png");

	Bumper.PushBack({ 405,0,60,60 });
	Bumper.PushBack({ 341,0,60,60 });
	Bumper.speed = 0;
	Bumper.loop = false;

	Ball_anim.PushBack({ 465,0, 30,30 });
	Ball_anim.speed = 0;

	Speed_booster.PushBack({ 465,0, 30,30 });
	Speed_booster.speed = 0;

	Lights_anim.PushBack({ 514,0,18,18 });
	Lights_anim.PushBack({ 495,0,18,18 });
	Lights_anim.speed = 0;
	Lights_anim.loop = false;

	speedbooster.PushBack({ 790,0, 33, 94 });
	speedbooster.speed = 0;

	Squared_Bumper.PushBack({ 0,0,0,0 });

	Flipper.PushBack({ 49,0, 110 ,38 });


	bonus_fx = App->audio->LoadFx("FX/bonus.wav");
	lights_fx = App->audio->LoadFx("FX/bonus 2.wav");
	Sprites = App->textures->Load("Sprites/Sprite sheet.png");

	end_game_sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, 1200, SCREEN_WIDTH, 50);
	end_game_sensor->body_type = END;


	Chain = App->physics->CreateChain(0, 0, structure_chain, 126, false );

	Rare_thing_left = App->physics->CreateChain(0, 0, rare_thing_left, 24, false);
	Rare_thing_right = App->physics->CreateChain(0, 0, rare_thing_right, 24, false);
	Curve_left = App->physics->CreateChain(0, 0, curve_left, 16, false);
	Curve_right = App->physics->CreateChain(0, 0, curve_right, 16, false);
	Center_chain = App->physics->CreateChain(0, 0, center_chain, 24, false);
	Left_bumper_chain = App->physics->CreateChain(0, 0, left_bumper_chain, 20);
	Right_bumper_chain = App->physics->CreateChain(0, 0, right_bumper_chain, 18);


	// Launcher Joint

	PhysBody *a = App->physics->CreateRectangle(610, 1155, 1, 1);
	PhysBody *b = App->physics->CreateRectangle(610, 1190, 20, 10, true);
	launcher_joint = App->physics->CreateJoint(a, b, e_prismaticJoint, 40.0f, -80.0f, false);

	// Flippers 

	PhysBody *static_part = App->physics->CreateCircle(180, 1090, 10);
	PhysBody *dynamic_part = App->physics->CreateRectangle(170, 1090, 100, 10, true);

	Left_flipper = App->physics->CreateJoint(static_part, dynamic_part, e_revoluteJoint, 0.0f, 100.0f, false, b2Vec2_zero, b2Vec2(0.0f, 0.0f), b2Vec2(-0.5f, 0.0f));


	static_part = App->physics->CreateCircle(415, 1090, 10);
	dynamic_part = App->physics->CreateRectangle(405, 1095, 100, 10, true);

	Right_flipper = App->physics->CreateJoint(static_part, dynamic_part, e_revoluteJoint, 0.0f, -100.0f, false, b2Vec2_zero, b2Vec2(0.0f, 0.0f), b2Vec2(+0.5f, 0.0f));


	//Upper part sticks

	App->physics->CreateRectangle(166 + 6, 246 + 25 ,5, 50, false);
	App->physics->CreateRectangle(230 + 4, 239 + 25, 5, 50, false);
	App->physics->CreateRectangle(294 + 4, 238 + 25, 5, 50, false);
	App->physics->CreateRectangle(359 + 2, 240 + 25, 5, 50, false);;
	App->physics->CreateRectangle(422 + 2, 245 + 25, 5, 50, false);
	App->physics->CreateRectangle(58 + 2, 868 + 55, 5, 126, false);
	App->physics->CreateRectangle(538 + 2, 868 + 55, 5, 126, false);
	App->physics->CreateRectangle(116, 1038, 5, 126, false, -58.0f);
	App->physics->CreateRectangle(482, 1038, 5, 126, false, 58.0f);


	float restitution = 1.3f;
	//Circle Bumpers
	PhysBody* aux_obj = new PhysBody();
	aux_obj = App->physics->CreateCircle(262+ 35, 394 + 35, 30, false);
	aux_obj->body_type = BUMPER;
	aux_obj->anim = Bumper;
	aux_obj->listener = this;
	aux_obj->body->GetFixtureList()->SetRestitution(restitution);
	Bumpers.add(aux_obj);


	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateCircle(171 + 35, 496 + 35, 30, false);
	aux_obj->body_type = BUMPER;
	aux_obj->anim = Bumper;
	aux_obj->listener = this;
	aux_obj->body->GetFixtureList()->SetRestitution(restitution);
	Bumpers.add(aux_obj);

	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateCircle(356 + 35, 496 + 35, 30, false);
	aux_obj->body_type = BUMPER;
	aux_obj->anim = Bumper;
	aux_obj->listener = this;
	aux_obj->body->GetFixtureList()->SetRestitution(restitution);
	Bumpers.add(aux_obj);

	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateCircle(262 + 35, 582 + 35, 30, false);
	aux_obj->body_type = BUMPER;
	aux_obj->anim = Bumper;
	aux_obj->listener = this;
	aux_obj->body->GetFixtureList()->SetRestitution(restitution);
	Bumpers.add(aux_obj);

	//Ball
	Ball = App->physics->CreateCircle(600, 1140, 15,true);
	Ball->body_type = BALL;
	Ball->anim = Ball_anim;
	Ball->listener = this;

	//Speedboosters
	speedboosterleft = 195;
	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateRectangleSensor(65, 750, 15, 75, speedboosterleft);
	aux_obj->body_type = SPEED_BOOSTER;
	aux_obj->anim = speedbooster;
	aux_obj->listener = this;
	SpeedBoosters.add(aux_obj);

	speedboosterright = -195;
	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateRectangleSensor(530, 750, 15, 75, speedboosterright);
	aux_obj->body_type = SPEED_BOOSTER;
	aux_obj->anim = speedbooster;
	aux_obj->listener = this;
	SpeedBoosters.add(aux_obj);



	//Squared bumpers

	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateRectangle(161, 925, 150, 10 , false, 73);
	aux_obj->initial_rotation = 73;
	aux_obj->body_type = SQUARED_BUMPER;
	aux_obj->anim = Squared_Bumper;
	aux_obj->listener = this;
	aux_obj->body->GetFixtureList()->SetRestitution(restitution);
	Bumpers.add(aux_obj);

	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateRectangle(438, 925, 150, 10, false,  108);
	aux_obj->initial_rotation = 108;
	aux_obj->body_type = SQUARED_BUMPER;
	aux_obj->anim = Squared_Bumper;
	aux_obj->listener = this;
	aux_obj->body->GetFixtureList()->SetRestitution(restitution);
	Bumpers.add(aux_obj);

	//Lights

	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateRectangleSensor(202, 260 - 18 , 18, 18);
	aux_obj->body_type = LIGHTS;
	aux_obj->anim = Lights_anim;
	aux_obj->listener = this;
	Lights.add(aux_obj);

	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateRectangleSensor(202, 290, 18, 18);
	aux_obj->body_type = LIGHTS;
	aux_obj->anim = Lights_anim;
	aux_obj->listener = this;
	Lights.add(aux_obj);
	
	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateRectangleSensor(266, 226, 18, 18);
	aux_obj->body_type = LIGHTS;
	aux_obj->anim = Lights_anim;
	aux_obj->listener = this;
	Lights.add(aux_obj);

	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateRectangleSensor(266, 286, 18, 18);
	aux_obj->body_type = LIGHTS;
	aux_obj->anim = Lights_anim;
	aux_obj->listener = this;
	Lights.add(aux_obj);

	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateRectangleSensor(330, 225, 18, 18);
	aux_obj->body_type = LIGHTS;
	aux_obj->anim = Lights_anim;
	aux_obj->listener = this;
	Lights.add(aux_obj);

	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateRectangleSensor(330, 287, 18, 18);
	aux_obj->body_type = LIGHTS;
	aux_obj->anim = Lights_anim;
	aux_obj->listener = this;
	Lights.add(aux_obj);

	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateRectangleSensor(393, 245, 18, 18);
	aux_obj->body_type = LIGHTS;
	aux_obj->anim = Lights_anim;
	aux_obj->listener = this;
	Lights.add(aux_obj);


	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateRectangleSensor(393, 290, 18, 18);
	aux_obj->body_type = LIGHTS;
	aux_obj->anim = Lights_anim;
	aux_obj->listener = this;
	Lights.add(aux_obj);

	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateRectangleSensor(140, 386, 18, 18);
	aux_obj->body_type = LIGHTS;
	aux_obj->anim = Lights_anim;
	aux_obj->listener = this;
	Lights.add(aux_obj);

	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateRectangleSensor(93, 432, 18, 18);
	aux_obj->body_type = LIGHTS;
	aux_obj->anim = Lights_anim;
	aux_obj->listener = this;
	Lights.add(aux_obj);

	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateRectangleSensor(458, 389, 18, 18);
	aux_obj->body_type = LIGHTS;
	aux_obj->anim = Lights_anim;
	aux_obj->listener = this;
	Lights.add(aux_obj);

	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateRectangleSensor(505, 434, 18, 18);
	aux_obj->body_type = LIGHTS;
	aux_obj->anim = Lights_anim;
	aux_obj->listener = this;
	Lights.add(aux_obj);

	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateRectangleSensor(199, 668, 18, 18);
	aux_obj->body_type = LIGHTS;
	aux_obj->anim = Lights_anim;
	aux_obj->listener = this;
	Lights.add(aux_obj);

	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateRectangleSensor(395, 670, 18, 18);
	aux_obj->body_type = LIGHTS;
	aux_obj->anim = Lights_anim;
	aux_obj->listener = this;
	Lights.add(aux_obj);

	lifes = 3;

	return ret;
}

update_status ModuleSceneIntro::PostUpdate()
{
	if (sensed == true)
	{
		Reset();

	
		sensed = false;
	}
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::Reset() {
	Ball->Reset();
	launcher_joint->body1->Reset();

	p2List_item<PhysBody*>* light;
	for (light = Lights.getFirst(); light != nullptr; light = light->next) {
		light->data->Reset();
	}

	p2List_item<PhysBody*>* bumper;
	for (bumper = Bumpers.getFirst(); bumper != nullptr; bumper = bumper->next) {
		bumper->data->Reset();
	}

	lifes--;
	if (lifes == 0)
	{
		puntuatuion = 0;
		lifes = 3;
	}
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{


	// Keyboard Inputs 

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		((b2PrismaticJoint*)launcher_joint->joint)->EnableMotor(true);
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{
		((b2PrismaticJoint*)launcher_joint->joint)->EnableMotor(false);
	}
	
	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{
		((b2RevoluteJoint*)Left_flipper->joint)->SetMotorSpeed(-((b2RevoluteJoint*)Left_flipper->joint)->GetMotorSpeed());
	}
	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_UP)
	{
		((b2RevoluteJoint*)Left_flipper->joint)->SetMotorSpeed(-((b2RevoluteJoint*)Left_flipper->joint)->GetMotorSpeed());
	}

	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
	{
		((b2RevoluteJoint*)Right_flipper->joint)->SetMotorSpeed(-((b2RevoluteJoint*)Right_flipper->joint)->GetMotorSpeed());
	}
	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_UP)
	{
		((b2RevoluteJoint*)Right_flipper->joint)->SetMotorSpeed(-((b2RevoluteJoint*)Right_flipper->joint)->GetMotorSpeed());
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		Reset();
	}

	//Blit All ----

	App->renderer->Blit(background, 0, 0);


	for (p2List_item<PhysBody*>* new_obj = Bumpers.getFirst(); new_obj != NULL; new_obj = new_obj->next)
	{
		iPoint bumper_pos;
		new_obj->data->GetPosition(bumper_pos.x, bumper_pos.y);
		SDL_Rect bumper_rect = new_obj->data->anim.GetCurrentFrame().rect;
		App->renderer->Blit(Sprites, bumper_pos.x, bumper_pos.y, &bumper_rect);

	}

	for (p2List_item<PhysBody*>* new_obj = Lights.getFirst(); new_obj != NULL; new_obj = new_obj->next)
	{
		iPoint lights_pos;
		new_obj->data->GetPosition(lights_pos.x, lights_pos.y);
		SDL_Rect lights_rect = new_obj->data->anim.GetCurrentFrame().rect;
		App->renderer->Blit(Sprites, lights_pos.x + lights_rect.w / 2, lights_pos.y + lights_rect.h/2 , &lights_rect);

	}

	
	iPoint leftspeed_pos;
	SpeedBoosters.getFirst()->data->GetPosition(leftspeed_pos.x, leftspeed_pos.y);
	SDL_Rect leftspeed_rect = SpeedBoosters.getFirst()->data->anim.GetCurrentFrame().rect;
	App->renderer->Blit(Sprites, 50, 712, &leftspeed_rect,1,speedboosterleft);

	iPoint rightspeed_pos;
	SpeedBoosters.getLast()->data->GetPosition(rightspeed_pos.x, rightspeed_pos.y);
	SDL_Rect rightspeed_rect = SpeedBoosters.getLast()->data->anim.GetCurrentFrame().rect;
	App->renderer->Blit(Sprites, 515,712, &rightspeed_rect, 1, speedboosterright,true);

	iPoint ball_pos;
	Ball->GetPosition(ball_pos.x, ball_pos.y);
	App->renderer->CameraFollow(ball_pos);

	App->renderer->Blit(Sprites, ball_pos.x, ball_pos.y, &Ball->anim.GetCurrentFrame().rect);

	iPoint left_flipperpos;
	Left_flipper->body2->GetPosition(leftspeed_pos.x, leftspeed_pos.y);

	App->renderer->Blit(Sprites, leftspeed_pos.x + 45, leftspeed_pos.y, &Flipper.GetCurrentFrame().rect, 1.0f, Left_flipper->body2->body->GetAngle() * 57.2957795);

	iPoint right_flipperpos;
	Right_flipper->body2->GetPosition(rightspeed_pos.x, rightspeed_pos.y);

	App->renderer->Blit(Sprites, rightspeed_pos.x +45, rightspeed_pos.y, &Flipper.GetCurrentFrame().rect, 1.0f, Right_flipper->body2->body->GetAngle() * 57.2957795,true);



	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);

		b2Vec2 speed_vec = bodyA->body->GetLinearVelocity();
		if (bodyB->body_type == BUMPER)
		{
			App->audio->PlayFx(bonus_fx);						
			bodyB->anim.speed = 1;
			puntuatuion += 200;
		}
		else if (bodyB->body_type == LIGHTS)
		{
			bodyB->anim.speed = 1;
			App->audio->PlayFx(lights_fx);
			puntuatuion += 20;
		}
		else if (bodyB->body_type == SPEED_BOOSTER)
		{
			bodyA->body->ApplyLinearImpulse({ speed_vec.x * 1.5f , speed_vec.y * 1.5f  }, { 0,0 }, false);
			puntuatuion += 100;
		}
		else if (bodyB->body_type == SQUARED_BUMPER)
		{
			puntuatuion += 100;
		}
		else if (bodyB->body_type == END)
		{
			sensed = true;
			
		}
	}
}

