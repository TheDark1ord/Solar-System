#include "pch.h"
#include "BodyClass.h"
#include "Panning.h"

#define HIDECONSOLE 0
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

	std::map<const char*, BodyModel*> bodyes;
	std::vector<BodyRender*> bodyesRender;

	bodyes.insert(std::pair<const char*, BodyModel*>("Sun", new BodyModel({ 0, 0 }, { 0, 0 }, 1.989e30, 696340km)));
	bodyes.insert(std::pair<const char*, BodyModel*>("Earth", new BodyModel({ -152.10e6km, 0 }, { 0, 29.29km }, 5.972e24, 6371km)));
	bodyes.insert(std::pair<const char*, BodyModel*>("Moon", new BodyModel({ -152.10e6km - 0.3633e6km, 0 }, {0, 29.29km + 0.970km}, 7.346e22, 1738km)));
	bodyes.insert(std::pair<const char*, BodyModel*>("Mercury", new BodyModel({ -0.466697au, 0 }, { 0, 43.46km }, 3.3011e23, 4880km)));
	bodyes.insert(std::pair<const char*, BodyModel*>("Venus", new BodyModel({ -0.7282au, 0 }, { 0, 34.78km }, 3.3011e23, 4880km)));

	bodyesRender.push_back(new BodyRender(world, bodyes["Sun"], sf::Color::Red, sf::Color::White, false));
	bodyesRender.push_back(new BodyRender(world, bodyes["Earth"], sf::Color::Blue, sf::Color(0, 0, 255, 125)));
	bodyesRender.push_back(new BodyRender(world, bodyes["Moon"], sf::Color(200, 200, 200), sf::Color::White, false));
	bodyesRender.push_back(new BodyRender(world, bodyes["Mercury"], sf::Color(200, 200, 200), sf::Color(255, 255, 255, 125)));
	bodyesRender.push_back(new BodyRender(world, bodyes["Venus"], sf::Color(255, 155, 155), sf::Color(255, 155, 155, 125)));

	sf::Font font;
	font.loadFromFile("times.ttf");

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

	while (window->isOpen())
	{
		sf::Event ev;
		while (window->pollEvent(ev))
		{
			world->HandleEvent(ev);
			HandleEvent(ev, window);
		}
		world->MainLoop();
		if (pause)
		{
			clock.restart();
			goto Render;
			continue;
		}

		for (int i = 0; i < 500; i++)
		{
			totalElapsedTime -= felapsed;
			felapsed += (long double)clock.getElapsedTime().asSeconds() * time_frame_array[current_time_frame];
			totalElapsedTime += felapsed;

			// Process planet motion in 15 minute time steps
			if (time_frame_array[current_time_frame] >= 12h)
			{
				while (felapsed >= 15min)
				{
					for (auto iti = bodyes.begin(); iti != bodyes.end(); iti++)
					{
						for (auto itj = iti; itj != bodyes.end(); itj++)
						{
							if (iti == itj)
								continue;
							iti->second->CalculateForce(itj->second, 15min);
						}
						iti->second->UpdatePosition(15min);
					}
					felapsed -= 15min;
				}
			}
			// Process planet motion in real time
			else
			{
				for (auto iti = bodyes.begin(); iti != bodyes.end(); iti++)
				{
					for (auto itj = iti; itj != bodyes.end(); itj++)
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

Render:
		window->clear();

		if (show_planets)
		{
			for (auto& i : bodyesRender)
			{
				if (i->related_model == bodyes["Sun"])
					i->Draw();
				else
					i->Draw(true);
			}
		}
		else
			for (auto& i : bodyesRender)
				i->Draw();

		window->draw(totalElapsedText);
		window->draw(time_frame_text);
		window->display();
	}

	delete window;
	window = nullptr;

	system("pause");
	return 0;
}