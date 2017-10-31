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
	ray_on = false;
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

	Lights_anim.PushBack({ 495,0,18,18 });
	Lights_anim.PushBack({ 514,0,18,18 });
	Lights_anim.speed = 0;
	Lights_anim.loop = false;

	bonus_fx = App->audio->LoadFx("FX/bonus.wav");
	lights_fx = App->audio->LoadFx("FX/bonus 2.wav");
	Sprites = App->textures->Load("Sprites/Sprite sheet.png");

	end_game_sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);


	Chain = App->physics->CreateChain(0, 0, structure_chain, 120, false );

	Rare_thing_left = App->physics->CreateChain(0, 0, rare_thing_left, 24, false);
	Rare_thing_right = App->physics->CreateChain(0, 0, rare_thing_right, 24, false);
	Curve_left = App->physics->CreateChain(0, 0, curve_left, 16, false);
	Curve_right = App->physics->CreateChain(0, 0, curve_right, 16, false);
	Center_chain = App->physics->CreateChain(0, 0, center_chain, 24, false);
	Left_bumper_chain = App->physics->CreateChain(0, 0, left_bumper_chain, 20);
	Right_bumper_chain = App->physics->CreateChain(0, 0, right_bumper_chain, 18);


	PhysBody *a = App->physics->CreateRectangle(610, 1155, 1, 1);
	PhysBody *b = App->physics->CreateRectangle(610, 1190, 20, 10, true);
	launcher_joint = App->physics->CreateJoint(a, b, e_prismaticJoint, 40.0f, -60.0f, false);

	PhysBody *static_part = App->physics->CreateCircle(191, 1195, 1);
	PhysBody *dynamic_part = App->physics->CreateRectangle(191 + 20, 1195, 30, 10, true);

	Left_flipper = App->physics->CreateJoint(static_part, dynamic_part, e_revoluteJoint, 0, 10.0f,true);




	//Upper part sticks

	App->physics->CreateRectangle(166 + 6, 246 + 25 ,5, 50, false);
	App->physics->CreateRectangle(230 + 4, 239 + 25, 5, 50, false);
	App->physics->CreateRectangle(294 + 4, 238 + 25, 5, 50, false);
	App->physics->CreateRectangle(359 + 2, 240 + 25, 5, 50, false);;
	App->physics->CreateRectangle(422 + 2, 245 + 25, 5, 50, false);
	App->physics->CreateRectangle(58 + 2, 868 + 55, 5, 126, false);
	App->physics->CreateRectangle(538 + 2, 868 + 55, 5, 126, false);

	
	//Circle Bumpers
	PhysBody* aux_obj = new PhysBody();
	aux_obj = App->physics->CreateCircle(262+ 35, 394 + 35, 30, false);
	aux_obj->body_type = BUMPER;
	aux_obj->anim = Bumper;
	aux_obj->listener = this;
	Bumpers.add(aux_obj);


	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateCircle(171 + 35, 496 + 35, 30, false);
	aux_obj->body_type = BUMPER;
	aux_obj->anim = Bumper;
	aux_obj->listener = this;
	Bumpers.add(aux_obj);

	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateCircle(356 + 35, 496 + 35, 30, false);
	aux_obj->body_type = BUMPER;
	aux_obj->anim = Bumper;
	aux_obj->listener = this;
	Bumpers.add(aux_obj);

	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateCircle(262 + 35, 582 + 35, 30, false);
	aux_obj->body_type = BUMPER;
	aux_obj->anim = Bumper;
	aux_obj->listener = this;
	Bumpers.add(aux_obj);

	//Ball

	Ball = App->physics->/*CreateRectangle(262 / 2, 304 / 2, 15*2, 15*2, true, 90.0f);*/CreateCircle(600, 1140 , 15,true);
	Ball->body_type = BALL;
	Ball->anim = Ball_anim;
	Ball->listener = this;

	//Speedboosters

	//Squared bumpers

	aux_obj = new PhysBody();
	aux_obj = App->physics->CreateRectangleSensor(200, 400, 50,50);
	aux_obj->body_type = SQUARED_BUMPER;
	aux_obj->anim = Bumper;
	aux_obj->listener = this;
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

	return ret;
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
	App->renderer->Blit(background, 0, 0);


	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		/*ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();*/

		((b2PrismaticJoint*)launcher_joint->joint)->EnableMotor(true);
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{
		((b2PrismaticJoint*)launcher_joint->joint)->EnableMotor(false);
	}
	
	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{
		/*ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();*/

		((b2PrismaticJoint*)Left_flipper->joint)->EnableMotor(false);
		
	
	}

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

	iPoint ball_pos;
	Ball->GetPosition(ball_pos.x, ball_pos.y);
	App->renderer->CameraFollow(ball_pos);

	App->renderer->Blit(Sprites, ball_pos.x, ball_pos.y, &Ball->anim.GetCurrentFrame().rect);


	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		
	}

	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{

		App->physics->CreateCircle(mouse.x, mouse.y, 15, true);

	}
	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

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
			b2Vec2 Normal_vec = { bodyA->body->GetPosition().x + bodyA->width / 2 - bodyB->body->GetPosition().x + bodyB->width / 2, bodyA->body->GetPosition().y + bodyA->height / 2 - bodyB->body->GetPosition().y + bodyB->height / 2 };
			bodyA->body->ApplyLinearImpulse({ speed_vec.x - (0.5f * Normal_vec.x ), speed_vec.y - (0.5f * Normal_vec.y) }, { 0,0 }, false);
			
			bodyB->anim.speed = 1;
		}
		else if (bodyB->body_type == LIGHTS)
		{
			bodyB->anim.speed = 1;
			App->audio->PlayFx(lights_fx);
		}
		else if (bodyB->body_type == SPEED_BOOSTER)
		{
			bodyA->body->ApplyLinearImpulse({ speed_vec.x * 1.5f , speed_vec.y * 1.5f  }, { 0,0 }, false);
		}
	}
}

