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
 
	background = App->textures->Load("Sprites/background1.png");

	Bumper.PushBack({ 405,0,60,60 });
	Bumper.PushBack({ 341,0,60,60 });
	Bumper.speed = 0;
	Bumper.loop = false;
	bonus_fx = App->audio->LoadFx("Game/pinball/bonus.wav");
	Sprites = App->textures->Load("Sprites/Sprite sheet.png");

	end_game_sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);


	App->physics->CreateChain(0, 0, structure_chain, 120, false );
	PhysBody *a = App->physics->CreateRectangle(200, 600, 1, 1);
	PhysBody *b = App->physics->CreateRectangle(200, 650, 20, 10, true);
	launcher_joint = App->physics->CreateJoint(a, b, e_prismaticJoint, 5.0f, -10.0f, false);
	
	Object* aux_obj = new Object();
	aux_obj->physbody = App->physics->CreateCircle(262, 394, 30, false);
	Bumpers.add(aux_obj);
	aux_obj->physbody = App->physics->CreateCircle(420, 394, 30, false);
	Bumpers.add(aux_obj);


	p2List_item<Object*>* new_obj = Bumpers.getFirst();

	for (int i = 0; i < Bumpers.count(); i++)
	{
		
		new_obj->data->physbody->body_type = BUMPER;
		new_obj->data->anim = Bumper;

		new_obj = new_obj->next;

	}

	Ball = App->physics->CreateCircle(262/ 2, 304 / 2 , 15,true);
	Ball->listener = this;

	

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
		/*ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();*/

		((b2PrismaticJoint*)launcher_joint->joint)->EnableMotor(false);
	}
	
	p2List_item<Object*>* new_obj = Bumpers.getFirst();

	for (uint i = 0; i < Bumpers.count(); i++)
	{
		iPoint bumper_pos;
		new_obj->data->physbody->GetPosition(bumper_pos.x, bumper_pos.y);
		SDL_Rect bumper_rect = new_obj->data->anim.GetCurrentFrame().rect;
		App->renderer->Blit(Sprites, bumper_pos.x, bumper_pos.y, &bumper_rect);
		new_obj = new_obj->next;
	}
	

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
		if (bodyB->body_type == BUMPER)
		{
			App->audio->PlayFx(bonus_fx);
			Bumper.speed = 1.0f;
			b2Vec2 speed_vec = bodyA->body->GetLinearVelocity();
			b2Vec2 Normal_vec = { bodyA->body->GetPosition().x + bodyA->width / 2 - bodyB->body->GetPosition().x + bodyB->width / 2, bodyA->body->GetPosition().y + bodyA->height / 2 - bodyB->body->GetPosition().y + bodyB->height / 2 };
			bodyA->body->ApplyLinearImpulse({ speed_vec.x - (0.5f * Normal_vec.x ), speed_vec.y - (0.5f * Normal_vec.y) }, { 0,0 }, false);
		}
	}
	}

