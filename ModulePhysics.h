#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"
#include "Animation.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
enum Body_type
{
	BALL = 1,
	BUMPER,
	WALL,
	OTHERS
};
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;


public:
	int width, height;
	b2Body* body;
	Module* listener;
	Body_type body_type = OTHERS;
};

class Object
{
public:
	PhysBody* physbody;
	Animation anim;

};

class PhysJoint {
public:
	PhysJoint() : joint(NULL), type(b2JointType::e_unknownJoint), body1(NULL), body2(NULL)
	{}

	PhysJoint(const PhysJoint &og) : joint(og.joint), type(og.type), body1(og.body1), body2(og.body2)
	{}


public:
	b2Joint*		joint;
	PhysBody*		body1;
	PhysBody*		body2;
	b2JointType		type;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, bool dyn = false);
	PhysBody* CreateRectangle(int x, int y, int width, int height, bool dyn = false);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, int* points, int size, bool dyn = false);
	PhysJoint* CreateJoint(PhysBody *bodyA, PhysBody *bodyB, b2JointType type, float max_length, float speed = 1.0f,
		bool collide_connected = false, b2Vec2 axis = { 0.0f, 1.0f }, b2Vec2 anchor1 = { 1.0f, 1.0f }, b2Vec2 anchor2 = { 1.0f, 1.0f });

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
};