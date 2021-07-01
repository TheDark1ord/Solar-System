#include "BodyClass.h"

BodyModel::BodyModel(sf::Vector2<long double> pos, long double mass, long double rad, BodyModel* parentPlanet)
	:pos(pos), vel({ 0, 0 }), mass(mass), rad(rad)
{
	this->parentPlanet = parentPlanet;
	if (parentPlanet != nullptr)
	{
		vel.x += parentPlanet->vel.x;
		vel.y += parentPlanet->vel.y;

		pos.x += parentPlanet->pos.x;
		pos.y += parentPlanet->pos.y;
	}
}

BodyModel::BodyModel(sf::Vector2<long double> pos, sf::Vector2<long double> vel, long double mass, long double rad, BodyModel* parentPlanet)
	: pos(pos), vel({ vel.x, vel.y }), mass(mass), rad(rad)
{
	this->parentPlanet = parentPlanet;
	if (parentPlanet != nullptr)
	{
		vel.x += parentPlanet->vel.x;
		vel.y += parentPlanet->vel.y;

		pos.x += parentPlanet->pos.x;
		pos.y += parentPlanet->pos.y;
	}
}

BodyModel::BodyModel(float angle, long double orbit_rad, long double orbital_vel, long double mass, long double rad, BodyModel* parentPlanet)
{
	// Construct using Cartesian coordinates
	// Angle - angle to the (0, 0) position(Sun)
	this->pos = { std::cos((360 - angle) * pi / 180) * orbit_rad, std::sin((360 - angle) * pi / 180) * orbit_rad };
	this->vel = { std::cos((360 - angle + 90.0f) * pi / 180) * orbital_vel, std::sin((360 - angle + 90.0f) * pi / 180) * orbital_vel };
	this->mass = mass;
	this->rad = rad;

	this->parentPlanet = parentPlanet;
	if (parentPlanet != nullptr)
	{
		vel.x += parentPlanet->vel.x;
		vel.y += parentPlanet->vel.y;

		pos.x += parentPlanet->pos.x;
		pos.y += parentPlanet->pos.y;
	}
}

void BodyModel::CalculateForce(BodyModel* body, long double time)
{
	// Time - time passed from last frame(in seconds)
// Calculate force and update velocity for BOTH bodies

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
	:world(world), related_model(model), pathColor(pathColor), enable_path(enable_path), loaded_texture(nullptr)
{
	// Enable path - true if path for planet should be stored and rendered
	// Show path - if false, store path but do not render it
	show_path = enable_path;

	body.setFillColor(fgColor);

	if (enable_path)
	{
		path.setPrimitiveType(sf::PrimitiveType::LinesStrip);
		path.append(sf::Vertex(sf::Vector2f(related_model->pos), pathColor));
	}
}

BodyRender::BodyRender(Panning* world, BodyModel* model, const char* texture_name, sf::Color pathColor, bool enable_path)
	:world(world), related_model(model), pathColor(pathColor), enable_path(enable_path)
{
	show_path = enable_path;

	if (enable_path)
	{
		path.setPrimitiveType(sf::PrimitiveType::LinesStrip);
		path.append(sf::Vertex(sf::Vector2f(related_model->pos), pathColor));
	}

	loaded_texture = LoadTexture(texture_name);
	loaded_texture->setSmooth(true);
	loaded_texture->generateMipmap();
	body.setTexture(loaded_texture);
}

BodyRender::~BodyRender()
{
	if (loaded_texture != nullptr)
	{
		delete loaded_texture;
		loaded_texture = nullptr;
	}
}

void BodyRender::Draw(bool show_planet)
{
	//TODO: set fgColor to avg color of the texture
	// and disable texture if the planet shape is less than a sertain treshold;
	// Enlarge the planet, so it will be visible from far away
	// The radius is set to the radius of the Sun
	if (show_planet && related_model->parentPlanet == nullptr && world->getZoom() <= 5.56248e-07)
		body.setRadius(696340km / world->getZoom() / (1 / default_zoom) * 1.25);
	else
		body.setRadius(related_model->rad);
	// Update sprite position
	body.setPosition(sf::Vector2f(related_model->pos.x - body.getRadius(), related_model->pos.y - body.getRadius()));

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

sf::Texture* BodyRender::LoadTexture(std::string textureName)
{
	std::string texturePath;

	texturePath = DevPath + textureName;
	if (!std::filesystem::exists(texturePath))
		texturePath = ReleasePath + textureName;
	if (!std::filesystem::exists((texturePath)))
		return nullptr;

	sf::Texture* retTexture = new sf::Texture;
	retTexture->loadFromFile(texturePath);
	return retTexture;
}

Sun::Sun(Panning* world, BodyModel* related_model, const char* texture_name)
	:BodyRender(world, related_model, texture_name, sf::Color::Black, false), textureRotation(0.0f)
{
	textureBlending = sf::BlendMode(sf::BlendMode::One, sf::BlendMode::One, sf::BlendMode::Add);
}

void Sun::Draw(long double time)
{
	//Time passed since last frame in milliseconds
	textureRotation += (textureRotationSpeed * time * 0.001f);

	if (textureRotation >= 2 * pi)
		textureRotation = 0.0f;

	sf::CircleShape topShape = BodyRender::body;

	BodyRender::body.setRadius(related_model->rad);
	BodyRender::body.setPosition(sf::Vector2f(related_model->pos.x - body.getRadius(), related_model->pos.y - body.getRadius()));

	topShape.setRadius(related_model->rad);
	topShape.setPosition(sf::Vector2f(related_model->pos.x - body.getRadius(), related_model->pos.y - body.getRadius()));

	sf::Vector2f newPos = { topShape.getPosition().x + topShape.getRadius(), topShape.getPosition().y + topShape.getRadius() };
	newPos.x = (newPos.x - world->getPan().x) * world->getZoom();
	newPos.y = (newPos.y - world->getPan().y) * world->getZoom();

	sf::Transform transform;
	transform.rotate(textureRotation * 180 / pi, newPos);

	sf::Transform top_transform;
	top_transform.rotate(360 - textureRotation * 180 / pi, newPos);

	textureBlending.transform = top_transform;

	world->Draw(BodyRender::body, transform);
	world->Draw(topShape, textureBlending);
}