#pragma once

#ifdef FLINPUTMANAGER_EXPORTS  
#define FLINPUTMANAGER_API __declspec(dllexport)   
#else  
#define FLINPUTMANAGER_API __declspec(dllimport)   
#endif 

#include "SDL.h"

#define SDL_main main

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include<map>
#include<vector>
#include<iostream>
#include<queue>
#include<thread>
#include<functional>


//* Vector2f Class for JoyStick values
class Vector2f
{
public:
	Vector2f(float _x, float _y) : x(_x), y(_y) {}
	Vector2f() : x(0.f), y(0.f) {}

	Vector2f operator*(float m) { x *= m; y *= m; return *this; }
	Vector2f operator+(const Vector2f v) { x += v.x; y += v.y; return *this; }
	float magnitude() { return sqrt(x * x + y * y); }
	float dotProduct(const Vector2f v2) { return (x * v2.x) + (y * v2.y); }

	Vector2f normalise() {
		if (magnitude() != 0) { return Vector2f((x / magnitude()), (y / magnitude())); }
		else { return Vector2f(0, 0); }
	}

	float x;
	float y;
};

//* Abstract class for input keys, input types, mouse and controller.
class EventListener
{
public:
	//* List of possible input events
	enum Event {
		UNKNOWN = 0,

		RETURN = '\r',
		ESCAPE = '\033',
		BACKSPACE = '\b',
		TAB = '\t',
		SPACE = ' ',
		EXCLAIM = '!',
		QUOTEDBL = '"',
		HASH = '#',
		PERCENT = '%',
		DOLLAR = '$',
		AMPERSAND = '&',
		QUOTE = '\'',
		LEFTPAREN = '(',
		RIGHTPAREN = ')',
		ASTERISK = '*',
		PLUS = '+',
		COMMA = ',',
		MINUS = '-',
		PERIOD = '.',
		SLASH = '/',
		NUM_0 = '0',
		NUM_1 = '1',
		NUM_2 = '2',
		NUM_3 = '3',
		NUM_4 = '4',
		NUM_5 = '5',
		NUM_6 = '6',
		NUM_7 = '7',
		NUM_8 = '8',
		NUM_9 = '9',
		COLON = ':',
		SEMICOLON = ';',
		LESS = '<',
		EQUALS = '=',
		GREATER = '>',
		QUESTION = '?',
		AT = '@',
		// Skip uppercase letters
		LEFTBRACKET = '[',
		BACKSLASH = '\\',
		RIGHTBRACKET = ']',
		CARET = '^',
		UNDERSCORE = '_',
		BACKQUOTE = '`',
		a = 'a',
		b = 'b',
		c = 'c',
		d = 'd',
		e = 'e',
		f = 'f',
		g = 'g',
		h = 'h',
		i = 'i',
		j = 'j',
		k = 'k',
		l = 'l',
		m = 'm',
		n = 'n',
		o = 'o',
		p = 'p',
		q = 'q',
		r = 'r',
		s = 's',
		t = 't',
		u = 'u',
		v = 'v',
		w = 'w',
		x = 'x',
		y = 'y',
		z = 'z',
		// Controller
		BUTTON_INVALID = 999,
		BUTTON_A = 1000,
		BUTTON_B,
		BUTTON_X,
		BUTTON_Y,
		BUTTON_BACK,
		BUTTON_GUIDE,
		BUTTON_START,
		BUTTON_LEFTSTICK,
		BUTTON_RIGHTSTICK,
		BUTTON_LEFTSHOULDER,
		BUTTON_RIGHTSHOULDER,
		BUTTON_DPAD_UP,
		BUTTON_DPAD_DOWN,
		BUTTON_DPAD_LEFT,
		BUTTON_DPAD_RIGHT,
		TRIGGER_SOFT_LEFT,
		TRIGGER_LEFT,
		TRIGGER_SOFT_RIGHT,
		TRIGGER_RIGHT,
		// Mouse
		MOUSE_LEFT = 2001, // A Space Odyssey
		MOUSE_RIGHT,
		MOUSE_MIDDLE,
		MOUSE_WHEEL_UP,
		MOUSE_WHEEL_DOWN,
		// Arrows
		ARROW_UP = 3001,
		ARROW_LEFT,
		ARROW_DOWN,
		ARROW_RIGHT

		//\\ Add your own events here
	};

	//* List of possible event types
	enum Type {
		Press,
		Release,
		Hold,
		Down,
		None

		//\\ Add your own events here
	};

	//* Pure virtual function that can be overriden inside any class that inherites from EventListener if needed
	//* Not required to execute keys, ideal for standard event dispatch/ listen procedure for custom events
	virtual void OnEvent(Event, Type) = 0;

	virtual ~EventListener();
};

//* Abstract class for input commands
//* Override examples are at the bottom of FLInputManager.h
class Command
{
public:
	//* Constructor for Custom/ once off calls
	Command(std::function<void()> function) : m_function(function) {}
	//* Constructor for Keys with Event Type
	Command(std::function<void()> function, EventListener::Type type) : m_function(function), m_type(type) { m_functions.push_back(m_function); }
	//* Default Destructor
	virtual ~Command() {}

	//* Function Object for executing custom function
	std::function<void()> m_function;
	//* Vector of functions for multiple functions per Key
	std::vector<std::function<void()>> m_functions;

	//* Type of Event used to differentiate commands
	EventListener::Type m_type;

	//// Execution calls for firing custom function sent in, based on type
	virtual void execute() {};
	virtual void executePress() {};
	virtual void executeRelease() {};
	virtual void executeHold() {};
	virtual void executeDown() {};

	void clearKeys()
	{
		m_functions.erase(m_functions.begin(), m_functions.end());
	}
};

//* Singleton class for input management
class InputManager
{
public:
	//* Default Constructor
	InputManager();
	//* Default Deconstructor
	~InputManager();

	//// Dictionaries
private:
	//* Dictionary holding a list of litener objects for each event
	std::map<EventListener::Event, std::vector<EventListener*>*> listeners; //* Pointer to vector of EventListeners

    //* Dictionary holding a bool for each event
	std::map<EventListener::Event, bool> beingHeld; //* Bool used to detect if the desired Event was previously being held

    //* Dictionary holding a list of command objects for each event
	std::map<EventListener::Event, std::vector<Command*>*> commands; //* Pointer to vector of Commands

    //* Dictionary holding a bool for each event
	std::map<EventListener::Event, bool> controllerHeld; //* Bool used to detect if the desired Event was previously being held

    //// Instance Variables
public:
	//* Used to get the Class Instance
	static InputManager* GetInstance();
private:
	//* Used to return the same instance of the Class
	static InputManager* inputManagerInstance;

public:
	//* Required to update the input
	void ProcessInput();
	void ConstantInput();

	//* Create an EventListener object
	void AddListener(EventListener::Event, EventListener*);

	void EmptyKeys();

	void EmptyKey(EventListener::Event evt);

	//* Used to create a key event
	void AddKey(EventListener::Event, Command*, EventListener*);
	//* Reset a Key and remove its Commands
	void ResetKey(EventListener::Event);

	//// Input Functions
private:
	//* Find a specific Event listener in the listeners dictionary, and call its onEvent() function
	void Dispatch(EventListener::Type, EventListener::Event);

	//* Set the hold for a specific Event in the beingHeld dictionary 
	void SetPrevious(EventListener::Event, bool);
	//* Check to see if a specific Event is being held down
	void CheckPrevious(EventListener::Type, EventListener::Event);

	//* Create a Command object
	void AddCommand(EventListener::Event, Command*);
	//* Find a Command object in the commands dictionary, and call its execute function based on Event Type
	void Execute(EventListener::Type, EventListener::Event);

	//* Combine a Command* and an EventListener to work together
	Command*& bindCommand(EventListener::Event);

	//// Controller Variables
private:
	//* Controller Timers
	int countedButtonFrames = 0;
	int countedTriggerFrames = 0;
	int controllerButtonDelay = 500;
	int controllerTriggerDelay = 500;

	//* Controller
	SDL_GameController* mGameController = SDL_GameControllerOpen(0);
	SDL_Joystick* mJoyStick = SDL_JoystickOpen(0);

	//* Used to get current Controller event for the map and see if it's being held
	EventListener::Event mControllerButton;
	bool mIsConnected = false;
	bool mIsPressed = false;

	//* Add Contoller if Detected
	void AddController(int id);
	//* Disconnect Controller
	void RemoveController(int id);

public:
	//* Set Delay of Controller Update
	void SetControllerButtonDelay(int delayInMilliseconds);
	void SetControllerTriggerDelay(int delayInMilliseconds);

	//// Controller Sticks
private:
	//* Joy Stick Axis
	int stick_Dead_Zone = 4000;

	//* Left Stick Variables
	float stick_Left_X = 0;
	float stick_Left_Y = 0;
	float stick_Left_T = 0;

	//* Right Stick Variables
	float stick_Right_X = 0;
	float stick_Right_Y = 0;
	float stick_Right_T = 0;

public:
	//* Set the size of the Stick Dead Zone
	void SetStickDeadZone(int deadZone);

	//* Left Stick Functions
	Vector2f GetLeftStickVector();
	Vector2f GetLeftStickVectorNormal();
	float GetLeftStickAngle();
	float GetLeftTrigger();

	//* Left Stick Functions
	Vector2f GetRightStickVector();
	Vector2f GetRightStickVectorNormal();
	float GetRightStickAngle();
	float GetRightTrigger();

public:
	SDL_Point GetMousePos();


	//// Logger
private:
	//* Log File Name
	std::string logFileName;
	//* Log File Values
	std::vector<std::string> logFileValues;

	//* Add Event to Output Log
	void logEvent(std::string str);
	//* Fill the Map for Key Logger
	void createKeyMap();

	//* Maps of Strings from Event Names
	std::map<EventListener::Event, std::string> keyNames;
	std::map<EventListener::Type, std::string> keyTypes;

	//* Track Previous Event for Hold
	EventListener::Event previousEvent;
	//* Count Hold Time
	float holdDuration = 0;

	//* Detect Previous saved
	bool saved = false;

public:
	//* Save the Log File
	void saveFile();
	//* Return the Current System Time
	std::string GetTimeStamp(bool save);

	//// Commands
public:
	// Keys
	Command* Key_UNKNOWN;
	Command* Key_RETURN;
	Command* Key_ESCAPE;
	Command* Key_BACKSPACE;
	Command* Key_TAB;
	Command* Key_SPACE;
	Command* Key_EXCLAIM;
	Command* Key_QUOTEDBL;
	Command* Key_HASH;
	Command* Key_PERCENT;
	Command* Key_DOLLAR;
	Command* Key_AMPERSAND;
	Command* Key_QUOTE;
	Command* Key_LEFTPAREN;
	Command* Key_RIGHTPAREN;
	Command* Key_ASTERISK;
	Command* Key_PLUS;
	Command* Key_COMMA;
	Command* Key_MINUS;
	Command* Key_PERIOD;
	Command* Key_SLASH;
	Command* Key_NUM_0;
	Command* Key_NUM_1;
	Command* Key_NUM_2;
	Command* Key_NUM_3;
	Command* Key_NUM_4;
	Command* Key_NUM_5;
	Command* Key_NUM_6;
	Command* Key_NUM_7;
	Command* Key_NUM_8;
	Command* Key_NUM_9;
	Command* Key_COLON;
	Command* Key_SEMICOLON;
	Command* Key_LESS;
	Command* Key_EQUALS;
	Command* Key_GREATER;
	Command* Key_QUESTION;
	Command* Key_AT;
	Command* Key_LEFTBRACKET;
	Command* Key_BACKSLASH;
	Command* Key_RIGHTBRACKET;
	Command* Key_CARET;
	Command* Key_UNDERSCORE;
	Command* Key_BACKQUOTE;
	Command* Key_a;
	Command* Key_b;
	Command* Key_c;
	Command* Key_d;
	Command* Key_e;
	Command* Key_f;
	Command* Key_g;
	Command* Key_h;
	Command* Key_i;
	Command* Key_j;
	Command* Key_k;
	Command* Key_l;
	Command* Key_m;
	Command* Key_n;
	Command* Key_o;
	Command* Key_p;
	Command* Key_q;
	Command* Key_r;
	Command* Key_s;
	Command* Key_t;
	Command* Key_u;
	Command* Key_v;
	Command* Key_w;
	Command* Key_x;
	Command* Key_y;
	Command* Key_z;

	// Controller
	Command* Button_INVALID;
	Command* Button_A;
	Command* Button_B;
	Command* Button_X;
	Command* Button_Y;
	Command* Button_BACK;
	Command* Button_GUIDE;
	Command* Button_START;
	Command* Button_LEFTSTICK;
	Command* Button_RIGHTSTICK;
	Command* Button_LEFTSHOULDER;
	Command* Button_RIGHTSHOULDER;
	Command* Button_DPAD_UP;
	Command* Button_DPAD_DOWN;
	Command* Button_DPAD_LEFT;
	Command* Button_DPAD_RIGHT;
	Command* Trigger_SOFT_LEFT;
	Command* Trigger_LEFT;
	Command* Trigger_SOFT_RIGHT;
	Command* Trigger_RIGHT;

	// Mouse
	Command* Mouse_Left;
	Command* Mouse_Right;
	Command* Mouse_Middle;
	Command* Mouse_Wheel_Up;
	Command* Mouse_Wheel_Down;

	// Arrows
	Command* Arrow_Up;
	Command* Arrow_Left;
	Command* Arrow_Down;
	Command* Arrow_Right;


	//\\ Add new Command* here for custom inputs
};

// Input
class InputCommand : public Command
{
public:
	InputCommand(std::function<void()> function, EventListener::Type type) : Command(function, type) {}

	~InputCommand() override
	{
		clearKeys();
	}

	virtual void executePress()
	{
		for (int i = 0; m_type == EventListener::Type::Press && i < m_functions.size(); i++)
			m_functions[i]();
	}

	virtual void executeRelease()
	{
		for (int i = 0; m_type == EventListener::Type::Release && i < m_functions.size(); i++)
			m_functions[i]();
	}

	virtual void executeHold()
	{
		for (int i = 0; m_type == EventListener::Type::Hold && i < m_functions.size(); i++)
			m_functions[i]();
	}

	virtual void executeDown()
	{
		for (int i = 0; m_type == EventListener::Type::Down && i < m_functions.size(); i++)
			m_functions[i]();
	}
};