#include "Panning.h"

Panning::Panning(sf::RenderWindow* window)
	:window(window), zoom(1.0), panx(-(Width / 2 / default_zoom)), pany(-(Height / 2 / default_zoom)), bounds(-1, -1, -1, -1)
{
	mouseHold = false;
}

Panning::Panning(sf::RenderWindow* window, sf::IntRect bounds)
	: window(window), zoom(1.0), panx(-(Width / 2 / default_zoom)), pany(-(Height / 2 / default_zoom)), bounds(bounds)
{
}

void Panning::SetZoom(long double new_zoom)
{
	zoom = new_zoom;
}

void Panning::SetPan(sf::Vector2<float> newPan)
{
	panx = newPan.x;
	pany = newPan.y;
}

float Panning::getZoom()
{
	return zoom;
}

sf::Vector2f Panning::getPan()
{
	return { panx, pany };
}

void Panning::HandleEvent(sf::Event msg)
{
	switch (msg.type)
	{
	case sf::Event::MouseButtonPressed:
		if (msg.mouseButton.button != sf::Mouse::Left)
			break;
		mouseHold = true;
		mousePrevPos = sf::Mouse::getPosition();
		break;
	case sf::Event::MouseButtonReleased:
		if (msg.mouseButton.button != sf::Mouse::Left)
			break;
		mouseHold = false;
		break;
	case sf::Event::MouseWheelScrolled:
		ZoomToMouse((float)msg.mouseWheelScroll.delta);
		break;
	default:
		break;
	}
}

void Panning::MainLoop()
{
	if (mouseHold)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition();
		panx += float(mousePrevPos.x - mousePos.x) / zoom;
		pany += float(mousePrevPos.y - mousePos.y) / zoom;
		mousePrevPos = mousePos;
	}
}

void Panning::Draw(sf::CircleShape drawable, const sf::RenderStates& renderStates, bool noZoom)
{
	sf::Vector2f newPos = drawable.getPosition();
	newPos.x = (newPos.x - panx) * zoom;
	newPos.y = (newPos.y - pany) * zoom;

	if (!noZoom)
		drawable.setScale(zoom, zoom);
	drawable.setPosition(newPos);

	drawable.setPointCount(std::min(std::max((int)drawable.getPointCount(), (int)((drawable.getRadius() * zoom) / 2)), 150));

	/*if (drawable.getPosition().x + drawable.getRadius() * drawable.getScale().x < 0)
		if (drawable.getPosition().y + drawable.getRadius() * drawable.getScale().y < 0)
			return;*/

	window->draw(drawable, renderStates);
}

void Panning::Draw(sf::ConvexShape drawable, const sf::RenderStates& renderStates)
{
	sf::Vector2f newPos = drawable.getPosition();
	newPos.x = (newPos.x - panx) * zoom;
	newPos.y = (newPos.y - pany) * zoom;
	drawable.setPosition(newPos);
	drawable.setScale(zoom, zoom);

	window->draw(drawable);
}

void Panning::Draw(sf::RectangleShape drawable, const sf::RenderStates& renderStates)
{
	sf::Vector2f newPos = drawable.getPosition();
	newPos.x = (newPos.x - panx) * zoom;
	newPos.y = (newPos.y - pany) * zoom;
	drawable.setPosition(newPos);
	drawable.setScale(zoom, zoom);

	window->draw(drawable);
}

void Panning::Draw(sf::Sprite drawable, const sf::RenderStates& renderStates)
{
	drawable.setOrigin({ panx, pany });
	drawable.setScale(zoom, zoom);
	window->draw(drawable, renderStates);
}

void Panning::Draw(sf::Text drawable, const sf::RenderStates& renderStates)
{
	drawable.setOrigin({ panx, pany });
	drawable.setScale(zoom, zoom);
	window->draw(drawable, renderStates);
}

void Panning::Draw(const sf::VertexBuffer vertexBuffer, const sf::RenderStates& renderStates)
{
}

void Panning::Draw(const sf::VertexBuffer vertexBuffer, std::size_t firstVertex, std::size_t vertexCount, const sf::RenderStates& renderStates)
{
}

void Panning::Draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& renderStates)
{
	sf::Vertex* outVertices = new sf::Vertex[vertexCount];

	for (int i = 0; i < vertexCount; i++)
	{
		outVertices[i] = vertices[i];
		outVertices[i].position.x = (outVertices[i].position.x - panx) * zoom;
		outVertices[i].position.y = (outVertices[i].position.y - pany) * zoom;
	}
	window->draw(outVertices, vertexCount, type, renderStates);

	delete outVertices;
	outVertices = nullptr;
}

void Panning::Draw(sf::VertexArray vertices, const sf::RenderStates& renderStates)
{
	for (int i = 0; i < vertices.getVertexCount(); i++)
	{
		vertices[i].position.x = (vertices[i].position.x - panx) * zoom;
		vertices[i].position.y = (vertices[i].position.y - pany) * zoom;
	}
	window->draw(vertices, renderStates);
}

void Panning::ZoomToMouse(int stepcount)
{
	// Mouse world pos before zoom
	sf::Vector2f mouseworldposp = (sf::Vector2f)sf::Mouse::getPosition(*window);
	mouseworldposp.x /= zoom;
	mouseworldposp.y /= zoom;
	mouseworldposp.x += panx;
	mouseworldposp.y += pany;

	/// Update zoom
	zoom *= 1 + ((float)stepcount * mousescrollmul);

	// Mouse world pos after zoom
	sf::Vector2f mouseworldpos = (sf::Vector2f)sf::Mouse::getPosition(*window);
	mouseworldpos.x /= zoom;
	mouseworldpos.y /= zoom;
	mouseworldpos.x += panx;
	mouseworldpos.y += pany;
	// Update panning to zoom to mouse
	panx += (mouseworldposp.x - mouseworldpos.x);
	pany += (mouseworldposp.y - mouseworldpos.y);
}