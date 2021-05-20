#pragma once

#include "pch.h"

class Panning
{
public:
	Panning(sf::RenderWindow* window);
	Panning(sf::RenderWindow* window, sf::IntRect bounds);

	void SetZoom(long double new_zoom);
	void SetPan(sf::Vector2<float> newPan);

	float getZoom();
	sf::Vector2f getPan();

	void HandleEvent(sf::Event msg);
	void MainLoop();

	void Draw(sf::CircleShape drawable, const sf::RenderStates& renderStates = sf::RenderStates::Default, bool noZoom = false);
	void Draw(sf::ConvexShape drawable, const sf::RenderStates& renderStates = sf::RenderStates::Default);
	void Draw(sf::RectangleShape drawable, const sf::RenderStates& renderStates = sf::RenderStates::Default);
	void Draw(sf::Sprite drawable, const sf::RenderStates& renderStates = sf::RenderStates::Default);
	void Draw(sf::Text drawable, const sf::RenderStates& renderStates = sf::RenderStates::Default);
	void Draw(const sf::VertexBuffer vertexBuffer, const sf::RenderStates& renderStates = sf::RenderStates::Default);
	void Draw(const sf::VertexBuffer vertexBuffer, std::size_t firstVertex, std::size_t vertexCount, const sf::RenderStates& renderStates = sf::RenderStates::Default);
	void Draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& renderStates = sf::RenderStates::Default);
	void Draw(sf::VertexArray vertices, const sf::RenderStates& renderStates = sf::RenderStates::Default);
private:
	const double mousescrollmul = 0.05;

	float panx;
	float pany;
	float zoom;

	sf::RenderWindow* window;
	const sf::IntRect bounds;

	sf::Vector2i mousePrevPos;
	bool mouseHold;

	void ZoomToMouse(int stepcount);
};