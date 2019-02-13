#include "Scene_Splash.h"


SplashScreen::SplashScreen()
{
	Init();
}


SplashScreen::~SplashScreen()
{
}

void SplashScreen::Init()
{
	// Art
	scaleFactor = sf::Vector2f(1, 1);

	if (!m_logoTexture.loadFromFile("content\\logo.png")){ std::cout << "Logo Art Error" << std::endl; }
	m_logoTexture.setSmooth(true);
	m_logoSprite.setTexture(m_logoTexture);
	// logoSprite.setScale(scaleFactor);
	m_logoSprite.setOrigin(sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
	m_logoSprite.setPosition(sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
	m_logoSprite.setColor(sf::Color(255, 255, 255, 0));

	logoAlphaFade = 0;
	logoFadeMultiply = 1.0f;

	// Writing
	pressAnyString = "Press Any Button to Continue!";
	currentLetter = 0;

	pressAnyText.setFont(ResourceManager::GetInstance()->FONT);
	pressAnyText.setString("");
	// pressAnyText.setScale(scaleFactor);
	// pressAnyText.setOrigin(0.5f, 0.5f); // Did not noticably move text, still needed to be divided by 2 later
	pressAnyText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	pressAnyText.setCharacterSize(18);
	pressAnyText.setOutlineThickness(2);

	printed = false;
	textAlphaFade = FULL_COLOUR;
	fading = false;

	m_currentScene = GameScene::SPLASH;
}

GameScene SplashScreen::Update()
{
	Fade(); // Fade in image

	return m_currentScene;
}

void SplashScreen::Draw(sf::RenderWindow &window)
{
	window.draw(m_logoSprite);
	window.draw(pressAnyText);
}

void SplashScreen::Input(sf::Event Event)
{
	if (Event.type == sf::Event::KeyPressed)
	{
		ResourceManager::GetInstance()->s_enter.play();
		std::cout << Event.key.code << std::endl;
		m_currentScene = GameScene::MAINMENU;
	}
}

void SplashScreen::Print(const std::string& str, const int delay) 
{
	// Start printing
	if (clock.getElapsedTime().asMilliseconds() > delay && currentLetter < str.size())
	{
		clock.restart();
		currentLetter++;
		pressAnyText.setString(sf::String(str.substr(0, currentLetter)));
		pressAnyText.setPosition((SCREEN_WIDTH / 2) - pressAnyText.getLocalBounds().width / 2.0f,
								(SCREEN_HEIGHT / 2) - pressAnyText.getLocalBounds().height / 2.0f);
	}

	// Finished
	if (currentLetter >= str.size())
	{
		printed = true;
		fading = true;
	}
}

void SplashScreen::Fade()
{
	if (logoAlphaFade < FULL_COLOUR)
	{
		m_logoSprite.setColor(sf::Color(FULL_COLOUR, FULL_COLOUR, FULL_COLOUR, logoAlphaFade));
		logoAlphaFade += logoFadeMultiply;
		logoFadeMultiply *= 1.007f; // 1.98333 seconds - License to kill
	}

	// Check if string has finished printing
	if (printed == false) 
	{ 
		Print(pressAnyString, TEXT_SPEED);
	}

	// If string has printed, change its alpha
	if (printed == true)
	{
		pressAnyText.setFillColor(sf::Color(FULL_COLOUR, FULL_COLOUR, FULL_COLOUR, textAlphaFade)); // Set Alpha for string
		//pressAnyText.setOutlineColor(sf::Color(std::rand() % 255, std::rand() % 255, std::rand() % 255, textAlphaFade)); // Set Alpha for string

		// Decrease alpha - fade out
		if (fading == true && textAlphaFade >= 50) 
		{ 
			textAlphaFade -= 3; 
		}
		else 
		{ 
			fading = false; 
		}

		// Increase alpha - fade in
		if (fading == false && textAlphaFade <= FULL_COLOUR - 5) 
		{ 
			textAlphaFade += 5; 
		}
		else 
		{ 
			fading = true; 
		}
	}
}
