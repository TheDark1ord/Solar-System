#pragma once

#include "pch.h"
#include "Panning.h"

class BodyModel
{
public:
	// Mathematical model of a body
	sf::Vector2<long double> pos;
	sf::Vector2<long double> vel;
	long double mass;
	long double rad;
	BodyModel* parentPlanet;

	BodyModel(sf::Vector2<long double> pos, long double mass, long double rad, BodyModel* parentPlanet = nullptr);
	BodyModel(sf::Vector2<long double> pos, sf::Vector2<long double> vel, long double mass, long double rad, BodyModel* parentPlanet = nullptr);
	BodyModel(float angle, long double orbit_rad, long double orbital_vel, long double mass, long double rad, BodyModel* parentPlanet = nullptr);

	void CalculateForce(BodyModel* body, long double time);
	void ApplyForce(sf::Vector2<long double> forceVector, long double time);
	void UpdatePosition(long double time);
};

class BodyRender
{
public:
	// Class for rendering bodies
	sf::CircleShape body;

	// Supply model to set rad and position
	BodyModel* related_model;

	BodyRender(Panning* world, BodyModel* model, sf::Color fgColor, sf::Color pathColor, bool enable_path = true);
	BodyRender(Panning* world, BodyModel* model, const char* texture_name, sf::Color pathColor, bool enable_path = true);
	~BodyRender();

	virtual void Draw(bool show_planet = false);

	void EnablePathRender();
	void DisablePathRender();

protected:
	sf::Texture* LoadTexture(std::string textureName);

	Panning* world;

	sf::VertexArray path;
	const bool enable_path;
	bool show_path;

	sf::Color pathColor;
	sf::Texture* loaded_texture;
};

class Sun : public BodyRender
{
public:
	Sun(Panning* world, BodyModel* related_model, const char* texture_name);
	void Draw(long double time);

private:
	float textureRotation;
	float textureRotationSpeed = pi / 16;

	sf::RenderStates textureBlending;
};