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

	BodyModel(sf::Vector2<long double> pos, long double mass, long double rad);
	BodyModel(sf::Vector2<long double> pos, sf::Vector2<long double> vel, long double mass, long double rad);

	void CalculateForce(BodyModel* body, long double time);
	void ApplyForce(sf::Vector2<long double> forceVector, long double time);
	void UpdatePosition(long double time);
};

class BodyRender
{
public:
	// Struct for rendering bodyes
	sf::CircleShape body;

	// Supply model to set rad and position
	BodyModel* related_model;

	BodyRender(Panning* world, BodyModel* model, sf::Color fgColor, sf::Color pathColor, bool enable_path = true);
	~BodyRender();

	void Draw(bool show_planet = false);
	
	void EnablePathRender();
	void DisablePathRender();

private:
	Panning* world;

	sf::VertexArray path;
	const bool enable_path;
	bool show_path;

	sf::Color pathColor;
};