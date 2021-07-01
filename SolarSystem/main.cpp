#include "pch.h"
#include "BodyClass.h"
#include "Panning.h"

#define HIDECONSOLE 1
#if HIDECONSOLE
#pragma comment( linker, "/subsystem:windows" )
#else
#pragma comment( linker, "/subsystem:console" )
#endif

bool pause = true;
bool show_planets = false;

void HandleEvent(sf::Event msg, sf::RenderWindow* window)
{
	switch (msg.type)
	{
	case sf::Event::Closed:
		window->close();
		break;
	case sf::Event::KeyPressed:
		if (msg.key.code == sf::Keyboard::Right)
			current_time_frame = std::min(current_time_frame + 1, 7);
		else if (msg.key.code == sf::Keyboard::Left)
			current_time_frame = std::max(current_time_frame - 1, 0);
		else if (msg.key.code == sf::Keyboard::Space)
			pause = !pause;
		else if (msg.key.code == sf::Keyboard::S)
			show_planets = !show_planets;
		break;
	default:
		break;
	}
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = antialiasingLevel;

	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(Width, Height), WindowHeader, sf::Style::Default, settings);
	if (maxFPS != NULL)
		window->setFramerateLimit(maxFPS);
	sf::Clock clock;

	Panning* world = new Panning(window);
	world->SetZoom(default_zoom);

	std::map<const char*, BodyModel*> bodies;
	std::vector<BodyRender*> bodiesRender;

	std::srand(std::time(nullptr));

	/*bodyes.insert(std::pair<const char*, BodyModel*>("Sun", new BodyModel({ 0, 0 }, { 0, 0 }, 1.989e30, 696340km)));
	bodyes.insert(std::pair<const char*, BodyModel*>("Mercury", new BodyModel({ -0.466697au, 0 }, { 0, 43.46km }, 3.3011e23, 4880km)));
	bodyes.insert(std::pair<const char*, BodyModel*>("Venus", new BodyModel({ -0.7282au, 0 }, { 0, 34.78km }, 4.867E24, 6051km)));
	bodyes.insert(std::pair<const char*, BodyModel*>("Earth", new BodyModel({ -152.10e6km, 0 }, { 0, 29.29km }, 5.972e24, 6371km)));
	bodyes.insert(std::pair<const char*, BodyModel*>("Moon", new BodyModel({ -152.10e6km - 0.3633e6km, 0 }, {0, 29.29km + 0.970km}, 7.346e22, 1738km)));*/

	bodies.insert(std::pair<const char*, BodyModel*>("Sun", new BodyModel({ 0, 0 }, { 0, 0 }, 1.989e30, 696340km)));
	bodies.insert(std::pair<const char*, BodyModel*>("Mercury", new BodyModel(rand() % 360, 0.466697au, 43.46km, 3.3011e23, 4880km)));
	bodies.insert(std::pair<const char*, BodyModel*>("Venus", new BodyModel(rand() % 360, 0.7282au, 34.78km, 4.867E24, 6051km)));
	bodies.insert(std::pair<const char*, BodyModel*>("Earth", new BodyModel(rand() % 360, 152.10e6km, 29.29km, 5.972e24, 6371km)));
	bodies.insert(std::pair<const char*, BodyModel*>("Moon", new BodyModel(rand() % 360, 0.3633e6km, 0.970km, 7.346e22, 1738km, bodies["Earth"])));
	bodies.insert(std::pair<const char*, BodyModel*>("Mars", new BodyModel(rand() % 360, 1.6660au, 22.0km, 6.4171e23, 3396km)));
	bodies.insert(std::pair<const char*, BodyModel*>("Ivan", new BodyModel(rand() % 360, 3au, 30km, 5.972e24, 6371km)));

	Sun sun(world, bodies["Sun"], "sun.png");
	//bodiesRender.push_back(new BodyRender(world, bodies["Sun"], sf::Color::Red, sf::Color::White, false));
	bodiesRender.push_back(new BodyRender(world, bodies["Earth"], "earth.png", sf::Color(0, 0, 255, 125)));
	bodiesRender.push_back(new BodyRender(world, bodies["Moon"], sf::Color(200, 200, 200), sf::Color::White, false));
	bodiesRender.push_back(new BodyRender(world, bodies["Mercury"], "mercury.png", sf::Color(255, 255, 255, 125)));
	bodiesRender.push_back(new BodyRender(world, bodies["Venus"], "venus.png", sf::Color(255, 155, 155, 125)));
	bodiesRender.push_back(new BodyRender(world, bodies["Mars"], "mars.png", sf::Color(255, 100, 1005, 125)));
	bodiesRender.push_back(new BodyRender(world, bodies["Ivan"], "ivan.png", sf::Color(255, 100, 100)));

	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\times.ttf");

	float totalElapsedTime = 0;
	sf::Text totalElapsedText;
	totalElapsedText.setFont(font);
	totalElapsedText.setCharacterSize(25);
	totalElapsedText.setPosition(5, 5);
	totalElapsedText.setFillColor(sf::Color::White);
	totalElapsedText.setOutlineColor(sf::Color::Black);
	totalElapsedText.setOutlineThickness(1);

	sf::Text time_frame_text(totalElapsedText);
	time_frame_text.setPosition(5, 50);

	long double felapsed = 0;

	totalElapsedText.setString("Elapsed time:" + std::to_string((int)std::round((float)(totalElapsedTime / 1d))) + " days");

	while (window->isOpen())
	{
		sf::Event ev;
		while (window->pollEvent(ev))
		{
			world->HandleEvent(ev);
			HandleEvent(ev, window);
		}
		// Set time frame text:
		switch (current_time_frame)
		{
		case 0:
			time_frame_text.setString("Current simulation speed is 1s\\s");
			break;
		case 1:
			time_frame_text.setString("Current simulation speed is 1m\\s");
			break;
		case 2:
			time_frame_text.setString("Current simulation speed is 1h\\s");
			break;
		case 3:
			time_frame_text.setString("Current simulation speed is 12h\\s");
			break;
		case 4:
			time_frame_text.setString("Current simulation speed is 1d\\s");
			break;
		case 5:
			time_frame_text.setString("Current simulation speed is 5d\\s");
			break;
		case 6:
			time_frame_text.setString("Current simulation speed is 10d\\s");
			break;
		case 7:
			time_frame_text.setString("Current simulation speed is 20d\\s");
			break;
		}
		world->MainLoop();
		if (pause)
		{
			clock.restart();
			goto Render;
			continue;
		}

		for (int i = 0; i < 1; i++)
		{
			totalElapsedTime -= felapsed;
			felapsed += (long double)clock.getElapsedTime().asSeconds() * time_frame_array[current_time_frame];
			totalElapsedTime += felapsed;

			// Process planet motion in 15 minute time steps
			if (time_frame_array[current_time_frame] >= 12h)
			{
				while (felapsed >= 20min)
				{
					for (auto iti = bodies.begin(); iti != bodies.end(); iti++)
					{
						for (auto itj = iti; itj != bodies.end(); itj++)
						{
							if (iti == itj)
								continue;
							iti->second->CalculateForce(itj->second, 20min);
						}
						iti->second->UpdatePosition(20min);
					}
					felapsed -= 20min;
				}
			}
			// Process planet motion in real time
			else
			{
				for (auto iti = bodies.begin(); iti != bodies.end(); iti++)
				{
					for (auto itj = iti; itj != bodies.end(); itj++)
					{
						if (iti == itj)
							continue;
						iti->second->CalculateForce(itj->second, felapsed);
					}
					iti->second->UpdatePosition(felapsed);
				}
				felapsed = 0;
			}
			clock.restart();
		}
		totalElapsedText.setString("Elapsed time:" + std::to_string((int)std::round((float)(totalElapsedTime / 1d))) + " days");

	Render:
		window->clear();

		if (show_planets)
		{
			sun.Draw(16.0f);
			for (int i = 1; i < bodiesRender.size(); i++)
			{
				bodiesRender[i]->Draw(true);
			}
		}
		else
		{
			sun.Draw(16.0f);
			for (int i = 1; i < bodiesRender.size(); i++)
			{
				bodiesRender[i]->Draw();
			}
		}

		window->draw(totalElapsedText);
		window->draw(time_frame_text);
		window->display();
	}

	delete window;
	window = nullptr;

	system("pause");
	return 0;
}