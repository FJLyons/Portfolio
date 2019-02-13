#pragma once

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

const int MAIN_MENU_INDEX = 3;

const int FULL_COLOUR = 255;
const int TEXT_SPEED = 100;

const sf::Vector2f SCREEN_SIZE = sf::Vector2f((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
const sf::Vector2f SCREEN_SIZE_HALF = sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

const int RUNNING = 0;
const int SUCCESS = 1;
const int FAIL = 2;
const int EVALUATE = 3;

const sf::Color Grey = sf::Color(125, 125, 125, 255);
const sf::Color DarkGreen = sf::Color(0, 125, 0, 255);

const int MAX_LENGHT = 1000000;

const int CASTLE_BODY_SIZE = 125;
const int WALL_WIDTH = 45;
const float WALL_LENGTH = SCREEN_HEIGHT / 3;

const float SITETOP = SCREEN_HEIGHT * 0.000f + WALL_LENGTH / 2;
const float SITEMID = SCREEN_HEIGHT * 0.334f + WALL_LENGTH / 2;
const float SITEBOT = SCREEN_HEIGHT * 0.667f + WALL_LENGTH / 2;

const float LEFTSITEBACK = SCREEN_WIDTH * 0.2f - WALL_WIDTH / 2;
const float LEFTSITEMID = SCREEN_WIDTH * 0.3f - WALL_WIDTH / 2;
const float LEFTSITEFRONT = SCREEN_WIDTH * 0.4f - WALL_WIDTH / 2;

const float RIGHTSITEBACK = SCREEN_WIDTH * 0.8f + WALL_WIDTH / 2;
const float RIGHTSITEMID = SCREEN_WIDTH * 0.7f + WALL_WIDTH / 2;
const float RIGHTSITEFRONT = SCREEN_WIDTH * 0.6f + WALL_WIDTH / 2;

const float TargetOffset = 50;

const sf::Vector2f LeftBackUpper = sf::Vector2f(LEFTSITEBACK, SITETOP);
const sf::Vector2f LeftBackMiddle = sf::Vector2f(LEFTSITEBACK, SITEMID);
const sf::Vector2f LeftBackLower = sf::Vector2f(LEFTSITEBACK, SITEBOT);

const sf::Vector2f LeftMidUpper  = sf::Vector2f(LEFTSITEMID, SITETOP);
const sf::Vector2f LeftMidMiddle = sf::Vector2f(LEFTSITEMID, SITEMID);
const sf::Vector2f LeftMidLower = sf::Vector2f(LEFTSITEMID, SITEBOT);

const sf::Vector2f LeftFrontUpper = sf::Vector2f(LEFTSITEFRONT, SITETOP);
const sf::Vector2f LeftFrontMiddle = sf::Vector2f(LEFTSITEFRONT, SITEMID);
const sf::Vector2f LeftFrontLower = sf::Vector2f(LEFTSITEFRONT, SITEBOT);


const sf::Vector2f RightBackUpper = sf::Vector2f(RIGHTSITEBACK, SITETOP);
const sf::Vector2f RightBackMiddle = sf::Vector2f(RIGHTSITEBACK, SITEMID);
const sf::Vector2f RightBackLower = sf::Vector2f(RIGHTSITEBACK, SITEBOT);
				   
const sf::Vector2f RightMidUpper = sf::Vector2f(RIGHTSITEMID, SITETOP);
const sf::Vector2f RightMidMiddle = sf::Vector2f(RIGHTSITEMID, SITEMID);
const sf::Vector2f RightMidLower = sf::Vector2f(RIGHTSITEMID, SITEBOT);
				   
const sf::Vector2f RightFrontUpper = sf::Vector2f(RIGHTSITEFRONT, SITETOP);
const sf::Vector2f RightFrontMiddle = sf::Vector2f(RIGHTSITEFRONT, SITEMID);
const sf::Vector2f RightFrontLower = sf::Vector2f(RIGHTSITEFRONT, SITEBOT);

const sf::Vector2f LEFTCASTLE = sf::Vector2f(125, SCREEN_SIZE_HALF.y);
const sf::Vector2f RIGHTCASTLE = sf::Vector2f(SCREEN_SIZE.x - 125, SCREEN_SIZE_HALF.y);

const float ARCHER_TIME = 3500.0f;
const float SOLDIER_TIME = 2500.0f;
const float BUILDER_TIME = 1500.0f;

const float ARCHER_VALUE = 35.0f;
const float SOLDIER_VALUE = 25.0f;
const float BUILDER_VALUE = 10.0f;
const float WALL_VALUE = 45.0f;

const float CLOCK_TICK = 100.0f;
const float MILIS = 1000.0f;
const float MINUTE = 60.0f;

const int MAX_BUILDERS = 6;
const int MAX_SOLDIERS = 10;
const int MAX_ARCHERS = 8;
const int MAX_WALLS = 9;

const int GAME_LENGTH = 5.0f * 60 * 1000; // minutes - seconds - miliseconds

const float WALL_HP = 80.0f;

const int UIS = 6;
const int UI_BAR = 300;

const int INSUI = 10;

const int DEBUG_NONE = 0;
const int DEBUG_TEXT = 1;
const int DEBUG_CIRCLES = 2;

const int UNIT_BODY_SIZE = 20;
const int UNIT_POS_LOWER = 400;
const int UNIT_POS_UPPER = 340;