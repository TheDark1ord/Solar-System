#include "BodyClass.h"

BodyModel::BodyModel(sf::Vector2<long double> pos, long double mass, long double rad)
	:pos(pos), vel({ 0, 0 }), mass(mass), rad(rad)
{}

BodyModel::BodyModel(sf::Vector2<long double> pos, sf::Vector2<long double> vel, long double mass, long double rad)
	: pos(pos), vel({ vel.x, vel.y }), mass(mass), rad(rad)
{}

void BodyModel::CalculateForce(BodyModel* body, long double time)
{
	// Time - time passed from last frame(in seconds)
// Calculate force and update velocity for BOTH bodyes

	long double dist_sqrd = (long double)(pos.x - body->pos.x) * (long double)(pos.x - body->pos.x)
		+ (long double)(pos.y - body->pos.y) * (long double)(pos.y - body->pos.y);
	long double Force = (long double)G * (long double)(long double(mass * body->mass) / (long double)dist_sqrd);

	sf::Vector2<long double> ForceVector;
	double angle = atan2(body->pos.y - pos.y, body->pos.x - pos.x);

	ForceVector.x = Force * cos(angle);
	ForceVector.y = Force * sin(angle);

	// Angle(in radians) from current body to the second body

	ApplyForce(ForceVector, time);
	body->ApplyForce({ -ForceVector.x, -ForceVector.y }, time);
}

void BodyModel::ApplyForce(sf::Vector2<long double> forceVector, long double time)
{
	vel.x += (long double)(forceVector.x / (long double)mass * (long double)time);
	vel.y += (long double)(forceVector.y / (long double)mass * (long double)time);
}

void BodyModel::UpdatePosition(long double time)
{
	pos.x += vel.x * time;
	pos.y += vel.y * time;
}

BodyRender::BodyRender(Panning* world, BodyModel* model, sf::Color fgColor, sf::Color pathColor, bool enable_path)
	:world(world), related_model(model), pathColor(pathColor), enable_path(enable_path)
{
	// Enable path - true if path for planet should be stored and rendered
	// Show path - if false, store path but do not render it
	show_path = enable_path;

	body.setFillColor(fgColor);
	body.setRadius(related_model->rad);

	path.setPrimitiveType(sf::PrimitiveType::LinesStrip);
	path.append(sf::Vertex(sf::Vector2f(related_model->pos), pathColor));
}

BodyRender::~BodyRender()
{
}

void BodyRender::Draw()
{
	// Update sprite position
	body.setPosition(sf::Vector2f(related_model->pos.x - related_model->rad, related_model->pos.y - related_model->rad));

	//Update path if the last point is outside of body
	if (enable_path)
	{
		sf::Vector2f lastPos = path[path.getVertexCount() - 1].position;
		long double distance2 = pow(lastPos.x - related_model->pos.x, 2) + pow(lastPos.y - related_model->pos.y, 2);
		if (distance2 >= pow(related_model->rad, 2))
			path.append(sf::Vertex(sf::Vector2f(related_model->pos), pathColor));
	}
	if (show_path)
		world->Draw(path);
	world->Draw(body);
}

void BodyRender::EnablePathRender()
{
	show_path = true;
}

void BodyRender::DisablePathRender()
{
	show_path = false;
}
