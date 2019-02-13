//#include "stdafx.h"
#include "FLInputManager.h"

//* Set initial instance to be nullptr
InputManager* InputManager::inputManagerInstance = nullptr;

//* Default Constructor
InputManager::InputManager()
{
	auto e = SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2

	if (e != 0) 
	{
		// problem with SDL?...
		std::cout << "Could not init SDL: " << SDL_GetError() << std::endl;
	}

	//* Create map of Keys and Events for Logger
	createKeyMap();
}

//* Default Deconstructor
InputManager::~InputManager()
{
}

//* Used to get the Class Instance
InputManager* InputManager::GetInstance()
{
	if (inputManagerInstance == nullptr)
	{
		inputManagerInstance = new InputManager();
	}
	//* Used to return the same instance of the Class
	return inputManagerInstance;
}

//* Create an EventListener object
void InputManager::AddListener(EventListener::Event evt, EventListener *listener)
{
	if (listeners.find(evt) == listeners.end())
	{
		listeners[evt] = new std::vector<EventListener*>();
	}

	if (listeners.find(evt) != listeners.end())
	{
		listeners[evt]->push_back(listener);
	}
}

//* Find a specific Event listener in the listeners dictionary, and call its onEvent() function
void InputManager::Dispatch(EventListener::Type type, EventListener::Event evt)
{
	if (listeners.find(evt) != listeners.end())
	{
		for (auto const &listener : *listeners[evt])
		{
			listener->OnEvent(evt, type);
		}
	}

	//* After a key is dispatched, see if it was previously held
	CheckPrevious(type, evt);
}

//* Set the hold for a specific Event in the beingHeld dictionary 
void InputManager::SetPrevious(EventListener::Event evt, bool isHeld)
{
	//* Set Event in map hold to be true or false
	beingHeld[evt] = isHeld;
}

//* Check to see if a specific Event is being held down
void InputManager::CheckPrevious(EventListener::Type type, EventListener::Event evt)
{
	//* Set Held to false for Event if the key was released
	if (type == EventListener::Type::Release)
	{
		SetPrevious(evt, false);
	}

	if (type == EventListener::Type::Down)
	{
		Execute(type, evt);
	}

	//* If Events hold is true, set Event Type to Hold
	if (beingHeld[evt])
	{
		type = EventListener::Type::Hold;
	}

	//* Run the Events Execute, now that type has been determined
	Execute(type, evt);
}

//* Create a Command object
void InputManager::AddCommand(EventListener::Event evt, Command *command)
{
	if (commands.find(evt) == commands.end())
	{
		commands[evt] = new std::vector<Command*>();
	}

	commands[evt]->push_back(command);
}

//* Find a Command object in the commands dictionary, and call its execute function based on Event Type
void InputManager::Execute(EventListener::Type type, EventListener::Event evt)
{
	//* Log Input
	if (evt != 0)
	{
		if (type != EventListener::Type::Hold && type != EventListener::Type::Down)
		{
			std::string newLine =
				keyNames[evt] + ","
				+ keyTypes[type] + ","
				+ "" + ","
				+ GetTimeStamp(false);
			logEvent(newLine);
			previousEvent = evt;
		}
		else if (type == EventListener::Type::Hold || type == EventListener::Type::Down)
		{
			holdDuration++;
			logFileValues.pop_back();
			std::string newLine =
				keyNames[evt] + ","
				+ keyTypes[type] + ","
				+ std::to_string(holdDuration / 20) + ","
				+ GetTimeStamp(false);
			logEvent(newLine);
			previousEvent = evt;
		}
		if (previousEvent == evt && type == EventListener::Type::Release)
		{
			holdDuration = 0;
		}
	}


	if (commands.find(evt) != commands.end())
	{
		for (auto const &command : *commands[evt])
		{
			//// Execution calls for firing custom function sent in
			if (type == EventListener::Type::Press) { command->executePress(); }
			else if (type == EventListener::Type::Release) { command->executeRelease(); }
			else if (type == EventListener::Type::Hold) { command->executeHold(); }
			else if (type == EventListener::Type::Down) { command->executeDown(); }
			else { command->execute(); }
			//std::cout << keyNames[evt] << " : " << keyTypes[type] << std::endl;
		}
	}

	//* Set Held to true for Event if the key was released, based on type
	if (type == EventListener::Type::Press)
	{
		SetPrevious(evt, true);
	}
}

//* Used to create a key event
void InputManager::AddKey(EventListener::Event evt, Command* command, EventListener *listener)
{
	//* This shit is LIT yo!
	//* Get a Command* and set Reference to that Command* 
	Command*& toBind = bindCommand(evt);
	//* get the referenced Command*, and assign the desired Command*
	toBind = command;

	//* Create a Listener for this Event
	AddListener(evt, listener);

	//* Create a Command for this Event
	AddCommand(evt, toBind);
}

//* Used to create a key event
void InputManager::ResetKey(EventListener::Event evt)
{
	commands[evt]->clear();
	listeners[evt]->clear();
}

//* Used to delete all key comands, and empty listeners
void InputManager::EmptyKeys()
{
	for (auto& commandVector : commands)
	{
		for (auto command : *commandVector.second)
		{
			delete command;
		}

		delete commandVector.second;
	}
	commands.erase(commands.begin(), commands.end());

	for (auto& listenerVector : listeners)
	{
		delete listenerVector.second;
	}
	listeners.erase(listeners.begin(), listeners.end());
}

// Used to delete a specific Key
void InputManager::EmptyKey(EventListener::Event evt)
{
	if (commands.count(evt) == 1)
	{
		for (auto command : *commands[evt])
		{
			delete command;
		}

		delete commands[evt];
	}
	commands.erase(evt);
}

//* Combine a Command* and an EventListener to work together
Command*& InputManager::bindCommand(EventListener::Event evt)
{
	//* Return a Command* based on the Event
	switch (evt)
	{
	case SDLK_UNKNOWN:		return  Key_UNKNOWN;		break;
	case SDLK_RETURN:		return	Key_RETURN;			break;
	case SDLK_ESCAPE:		return	Key_ESCAPE;			break;
	case SDLK_BACKSPACE:	return	Key_BACKSPACE;		break;
	case SDLK_TAB:			return	Key_TAB;			break;
	case SDLK_SPACE:		return	Key_SPACE;			break;
	case SDLK_EXCLAIM:		return	Key_EXCLAIM;		break;
	case SDLK_QUOTEDBL:		return	Key_QUOTEDBL;		break;
	case SDLK_HASH:			return	Key_HASH;			break;
	case SDLK_PERCENT:		return	Key_PERCENT;		break;
	case SDLK_DOLLAR:		return	Key_DOLLAR;			break;
	case SDLK_AMPERSAND:	return	Key_AMPERSAND;		break;
	case SDLK_QUOTE:		return	Key_QUOTE;			break;
	case SDLK_LEFTPAREN:	return	Key_LEFTPAREN;		break;
	case SDLK_RIGHTPAREN:	return	Key_RIGHTPAREN;		break;
	case SDLK_ASTERISK:		return	Key_ASTERISK;		break;
	case SDLK_PLUS:			return	Key_PLUS;			break;
	case SDLK_COMMA:		return	Key_COMMA;			break;
	case SDLK_MINUS:		return	Key_MINUS;			break;
	case SDLK_PERIOD:		return	Key_PERIOD;			break;
	case SDLK_SLASH:		return	Key_SLASH;			break;
	case SDLK_0:			return	Key_NUM_0;			break;
	case SDLK_1:			return	Key_NUM_1;			break;
	case SDLK_2:			return	Key_NUM_2;			break;
	case SDLK_3:			return	Key_NUM_3;			break;
	case SDLK_4:			return	Key_NUM_4;			break;
	case SDLK_5:			return	Key_NUM_5;			break;
	case SDLK_6:			return	Key_NUM_6;			break;
	case SDLK_7:			return	Key_NUM_7;			break;
	case SDLK_8:			return	Key_NUM_8;			break;
	case SDLK_9:			return	Key_NUM_9;			break;
	case SDLK_COLON:		return	Key_COLON;			break;
	case SDLK_SEMICOLON:	return	Key_SEMICOLON;		break;
	case SDLK_LESS:			return	Key_LESS;			break;
	case SDLK_EQUALS:		return	Key_EQUALS;			break;
	case SDLK_GREATER:		return	Key_GREATER;		break;
	case SDLK_QUESTION:		return	Key_QUESTION;		break;
	case SDLK_AT:			return	Key_AT;				break;
	case SDLK_LEFTBRACKET:	return	Key_LEFTBRACKET;	break;
	case SDLK_BACKSLASH:	return	Key_BACKSLASH;		break;
	case SDLK_RIGHTBRACKET: return	Key_RIGHTBRACKET;	break;
	case SDLK_CARET:		return	Key_CARET;			break;
	case SDLK_UNDERSCORE:	return	Key_UNDERSCORE;		break;
	case SDLK_BACKQUOTE:	return	Key_BACKQUOTE;		break;
	case SDLK_a:			return	Key_a;				break;
	case SDLK_b:			return	Key_b;				break;
	case SDLK_c:			return	Key_c;				break;
	case SDLK_d:			return	Key_d;				break;
	case SDLK_e:			return	Key_e;				break;
	case SDLK_f:			return	Key_f;				break;
	case SDLK_g:			return	Key_g;				break;
	case SDLK_h:			return	Key_h;				break;
	case SDLK_i:			return	Key_i;				break;
	case SDLK_j:			return	Key_j;				break;
	case SDLK_k:			return	Key_k;				break;
	case SDLK_l:			return	Key_l;				break;
	case SDLK_m:			return	Key_m;				break;
	case SDLK_n:			return	Key_n;				break;
	case SDLK_o:			return	Key_o;				break;
	case SDLK_p:			return	Key_p;				break;
	case SDLK_q:			return	Key_q;				break;
	case SDLK_r:			return	Key_r;				break;
	case SDLK_s:			return	Key_s;				break;
	case SDLK_t:			return	Key_t;				break;
	case SDLK_u:			return	Key_u;				break;
	case SDLK_v:			return	Key_v;				break;
	case SDLK_w:			return	Key_w;				break;
	case SDLK_x:			return	Key_x;				break;
	case SDLK_y:			return	Key_y;				break;
	case SDLK_z:			return	Key_z;				break;

		//\\ Add new Command* to return for custom Input Library Events here

	default: break;
	}

	//* Controller Buttons
	if (evt == EventListener::Event::BUTTON_INVALID) { return Button_INVALID; }
	if (evt == EventListener::Event::BUTTON_A) { return Button_A; }
	if (evt == EventListener::Event::BUTTON_B) { return Button_B; }
	if (evt == EventListener::Event::BUTTON_X) { return Button_X; }
	if (evt == EventListener::Event::BUTTON_Y) { return Button_Y; }
	if (evt == EventListener::Event::BUTTON_BACK) { return Button_BACK; }
	if (evt == EventListener::Event::BUTTON_GUIDE) { return Button_GUIDE; }
	if (evt == EventListener::Event::BUTTON_START) { return Button_START; }
	if (evt == EventListener::Event::BUTTON_LEFTSTICK) { return Button_LEFTSTICK; }
	if (evt == EventListener::Event::BUTTON_RIGHTSTICK) { return Button_RIGHTSTICK; }
	if (evt == EventListener::Event::BUTTON_LEFTSHOULDER) { return Button_LEFTSHOULDER; }
	if (evt == EventListener::Event::BUTTON_RIGHTSHOULDER) { return Button_RIGHTSHOULDER; }
	if (evt == EventListener::Event::BUTTON_DPAD_UP) { return Button_DPAD_UP; }
	if (evt == EventListener::Event::BUTTON_DPAD_DOWN) { return Button_DPAD_DOWN; }
	if (evt == EventListener::Event::BUTTON_DPAD_LEFT) { return Button_DPAD_LEFT; }
	if (evt == EventListener::Event::BUTTON_DPAD_RIGHT) { return Button_DPAD_RIGHT; }

	//* Controller Triggers
	if (evt == EventListener::Event::TRIGGER_SOFT_LEFT) { return Button_DPAD_UP; }
	if (evt == EventListener::Event::TRIGGER_LEFT) { return Button_DPAD_DOWN; }
	if (evt == EventListener::Event::TRIGGER_SOFT_RIGHT) { return Button_DPAD_LEFT; }
	if (evt == EventListener::Event::TRIGGER_RIGHT) { return Button_DPAD_RIGHT; }

	//* Mouse Buttons
	if (evt == EventListener::Event::MOUSE_LEFT) { return  Mouse_Left; }
	if (evt == EventListener::Event::MOUSE_RIGHT) { return  Mouse_Right; }
	if (evt == EventListener::Event::MOUSE_MIDDLE) { return  Mouse_Middle; }
	if (evt == EventListener::Event::MOUSE_WHEEL_UP) { return  Mouse_Wheel_Up; }
	if (evt == EventListener::Event::MOUSE_WHEEL_DOWN) { return  Mouse_Wheel_Down; }

	//* Arrows
	if (evt == EventListener::Event::ARROW_LEFT) { return  Arrow_Left; }
	if (evt == EventListener::Event::ARROW_RIGHT) { return  Arrow_Right; }
	if (evt == EventListener::Event::ARROW_UP) { return  Arrow_Up; }
	if (evt == EventListener::Event::ARROW_DOWN) { return  Arrow_Down; }

	return Key_UNKNOWN;
}

//* Required to update the input
void InputManager::ProcessInput()
{
	//* New Input Library Event
	SDL_Event evn;

	//* New Event Listener Type
	EventListener::Type type;

	while (SDL_PollEvent(&evn) != 0)
	{
		//* Print Key Code on Press
		if (evn.type == SDL_KEYDOWN) 
		{ 
			//std::cout << evn.key.keysym.sym << std::endl; 
		}
		else if (evn.type == SDL_CONTROLLERBUTTONDOWN) 
		{ 
			//std::cout << std::to_string(evn.cbutton.button) << std::endl; 
		}
		else if (evn.type == SDL_MOUSEBUTTONDOWN) 
		{
			//std::cout << std::to_string(evn.button.button) << std::endl;
		}


		//* Get Event Listener Type
		switch (evn.type)
		{
		case SDL_KEYDOWN: type = EventListener::Type::Press;	break;
		case SDL_KEYUP: type = EventListener::Type::Release;	break;

		default: type = EventListener::Type::None;				break;
		}

		//* Get Key Event and call Input Manager Dispatch for that key
		switch (evn.key.keysym.sym)
		{
		case SDLK_UNKNOWN:		Dispatch(type, EventListener::Event::UNKNOWN);			break;
		case SDLK_RETURN:		Dispatch(type, EventListener::Event::RETURN);			break;
		case SDLK_ESCAPE:		Dispatch(type, EventListener::Event::ESCAPE);			break;
		case SDLK_BACKSPACE:	Dispatch(type, EventListener::Event::BACKSPACE);		break;
		case SDLK_TAB:			Dispatch(type, EventListener::Event::TAB);				break;
		case SDLK_SPACE:		Dispatch(type, EventListener::Event::SPACE);			break;
		case SDLK_EXCLAIM:		Dispatch(type, EventListener::Event::EXCLAIM);			break;
		case SDLK_QUOTEDBL:		Dispatch(type, EventListener::Event::QUOTEDBL);			break;
		case SDLK_HASH:			Dispatch(type, EventListener::Event::HASH);				break;
		case SDLK_PERCENT:		Dispatch(type, EventListener::Event::PERCENT);			break;
		case SDLK_DOLLAR:		Dispatch(type, EventListener::Event::DOLLAR);			break;
		case SDLK_AMPERSAND:	Dispatch(type, EventListener::Event::AMPERSAND);		break;
		case SDLK_QUOTE:		Dispatch(type, EventListener::Event::QUOTE);			break;
		case SDLK_LEFTPAREN:	Dispatch(type, EventListener::Event::LEFTPAREN);		break;
		case SDLK_RIGHTPAREN:	Dispatch(type, EventListener::Event::RIGHTPAREN);		break;
		case SDLK_ASTERISK:		Dispatch(type, EventListener::Event::ASTERISK);			break;
		case SDLK_PLUS:			Dispatch(type, EventListener::Event::PLUS);				break;
		case SDLK_COMMA:		Dispatch(type, EventListener::Event::COMMA);			break;
		case SDLK_MINUS:		Dispatch(type, EventListener::Event::MINUS);			break;
		case SDLK_PERIOD:		Dispatch(type, EventListener::Event::PERIOD);			break;
		case SDLK_SLASH:		Dispatch(type, EventListener::Event::SLASH);			break;
		case SDLK_0:			Dispatch(type, EventListener::Event::NUM_0);			break;
		case SDLK_1:			Dispatch(type, EventListener::Event::NUM_1);			break;
		case SDLK_2:			Dispatch(type, EventListener::Event::NUM_2);			break;
		case SDLK_3:			Dispatch(type, EventListener::Event::NUM_3);			break;
		case SDLK_4:			Dispatch(type, EventListener::Event::NUM_4);			break;
		case SDLK_5:			Dispatch(type, EventListener::Event::NUM_5);			break;
		case SDLK_6:			Dispatch(type, EventListener::Event::NUM_6);			break;
		case SDLK_7:			Dispatch(type, EventListener::Event::NUM_7);			break;
		case SDLK_8:			Dispatch(type, EventListener::Event::NUM_8);			break;
		case SDLK_9:			Dispatch(type, EventListener::Event::NUM_9);			break;
		case SDLK_COLON:		Dispatch(type, EventListener::Event::COLON);			break;
		case SDLK_SEMICOLON:	Dispatch(type, EventListener::Event::SEMICOLON);		break;
		case SDLK_LESS:			Dispatch(type, EventListener::Event::LESS);				break;
		case SDLK_EQUALS:		Dispatch(type, EventListener::Event::EQUALS);			break;
		case SDLK_GREATER:		Dispatch(type, EventListener::Event::GREATER);			break;
		case SDLK_QUESTION:		Dispatch(type, EventListener::Event::QUESTION);			break;
		case SDLK_AT:			Dispatch(type, EventListener::Event::AT);				break;
		case SDLK_LEFTBRACKET:	Dispatch(type, EventListener::Event::LEFTBRACKET);		break;
		case SDLK_BACKSLASH:	Dispatch(type, EventListener::Event::BACKSLASH);		break;
		case SDLK_RIGHTBRACKET: Dispatch(type, EventListener::Event::RIGHTBRACKET);		break;
		case SDLK_CARET:		Dispatch(type, EventListener::Event::CARET);			break;
		case SDLK_UNDERSCORE:	Dispatch(type, EventListener::Event::UNDERSCORE);		break;
		case SDLK_BACKQUOTE:	Dispatch(type, EventListener::Event::BACKQUOTE);		break;
		case SDLK_a:			Dispatch(type, EventListener::Event::a);				break;
		case SDLK_b:			Dispatch(type, EventListener::Event::b);				break;
		case SDLK_c:			Dispatch(type, EventListener::Event::c);				break;
		case SDLK_d:			Dispatch(type, EventListener::Event::d);				break;
		case SDLK_e:			Dispatch(type, EventListener::Event::e);				break;
		case SDLK_f:			Dispatch(type, EventListener::Event::f);				break;
		case SDLK_g:			Dispatch(type, EventListener::Event::g);				break;
		case SDLK_h:			Dispatch(type, EventListener::Event::h);				break;
		case SDLK_i:			Dispatch(type, EventListener::Event::i);				break;
		case SDLK_j:			Dispatch(type, EventListener::Event::j);				break;
		case SDLK_k:			Dispatch(type, EventListener::Event::k);				break;
		case SDLK_l:			Dispatch(type, EventListener::Event::l);				break;
		case SDLK_m:			Dispatch(type, EventListener::Event::m);				break;
		case SDLK_n:			Dispatch(type, EventListener::Event::n);				break;
		case SDLK_o:			Dispatch(type, EventListener::Event::o);				break;
		case SDLK_p:			Dispatch(type, EventListener::Event::p);				break;
		case SDLK_q:			Dispatch(type, EventListener::Event::q);				break;
		case SDLK_r:			Dispatch(type, EventListener::Event::r);				break;
		case SDLK_s:			Dispatch(type, EventListener::Event::s);				break;
		case SDLK_t:			Dispatch(type, EventListener::Event::t);				break;
		case SDLK_u:			Dispatch(type, EventListener::Event::u);				break;
		case SDLK_v:			Dispatch(type, EventListener::Event::v);				break;
		case SDLK_w:			Dispatch(type, EventListener::Event::w);				break;
		case SDLK_x:			Dispatch(type, EventListener::Event::x);				break;
		case SDLK_y:			Dispatch(type, EventListener::Event::y);				break;
		case SDLK_z:			Dispatch(type, EventListener::Event::z);				break;
		case SDLK_UP:			Dispatch(type, EventListener::Event::ARROW_UP);			break;
		case SDLK_LEFT:			Dispatch(type, EventListener::Event::ARROW_LEFT);		break;
		case SDLK_DOWN:			Dispatch(type, EventListener::Event::ARROW_DOWN);		break;
		case SDLK_RIGHT:		Dispatch(type, EventListener::Event::ARROW_RIGHT);		break;

		default: break;
		}

		//* Controller
		//* Get Event Listener Type
		switch (evn.type)
		{
			//* Controller Connect / Disconnect
		case SDL_CONTROLLERDEVICEADDED: AddController(evn.cdevice.which);	break;
		case SDL_CONTROLLERDEVICEREMOVED: RemoveController(evn.cdevice.which); break;

		default: type = EventListener::Type::None;				break;
		}

		//* Controller is connected
		if (mIsConnected)
		{
			//* Controller Button Pressed
			if (evn.type == SDL_CONTROLLERBUTTONDOWN)
			{
				//* Set Type
				type = EventListener::Type::Press;
				//* Find Button
				if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_INVALID) { mControllerButton = EventListener::Event::BUTTON_INVALID; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_A) { mControllerButton = EventListener::Event::BUTTON_A; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_B) { mControllerButton = EventListener::Event::BUTTON_B; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_X) { mControllerButton = EventListener::Event::BUTTON_X; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_Y) { mControllerButton = EventListener::Event::BUTTON_Y; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_BACK) { mControllerButton = EventListener::Event::BUTTON_BACK; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_GUIDE) { mControllerButton = EventListener::Event::BUTTON_GUIDE; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_START) { mControllerButton = EventListener::Event::BUTTON_START; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSTICK) { mControllerButton = EventListener::Event::BUTTON_LEFTSTICK; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSTICK) { mControllerButton = EventListener::Event::BUTTON_RIGHTSTICK; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER) { mControllerButton = EventListener::Event::BUTTON_LEFTSHOULDER; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) { mControllerButton = EventListener::Event::BUTTON_RIGHTSHOULDER; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP) { mControllerButton = EventListener::Event::BUTTON_DPAD_UP; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN) { mControllerButton = EventListener::Event::BUTTON_DPAD_DOWN; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT) { mControllerButton = EventListener::Event::BUTTON_DPAD_LEFT; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) { mControllerButton = EventListener::Event::BUTTON_DPAD_RIGHT; }
				//* Set Pressed in map
				controllerHeld[mControllerButton] = true;
				//* Dispatch
				Dispatch(type, mControllerButton);
				//* Used to Delay Hold
				countedButtonFrames = -controllerButtonDelay * 8;
			}

			else if (evn.type == SDL_CONTROLLERBUTTONUP)
			{
				//* Set Type
				type = EventListener::Type::Release;
				//* Find Button
				if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_INVALID) { mControllerButton = EventListener::Event::BUTTON_INVALID; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_A) { mControllerButton = EventListener::Event::BUTTON_A; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_B) { mControllerButton = EventListener::Event::BUTTON_B; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_X) { mControllerButton = EventListener::Event::BUTTON_X; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_Y) { mControllerButton = EventListener::Event::BUTTON_Y; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_BACK) { mControllerButton = EventListener::Event::BUTTON_BACK; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_GUIDE) { mControllerButton = EventListener::Event::BUTTON_GUIDE; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_START) { mControllerButton = EventListener::Event::BUTTON_START; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSTICK) { mControllerButton = EventListener::Event::BUTTON_LEFTSTICK; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSTICK) { mControllerButton = EventListener::Event::BUTTON_RIGHTSTICK; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER) { mControllerButton = EventListener::Event::BUTTON_LEFTSHOULDER; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) { mControllerButton = EventListener::Event::BUTTON_RIGHTSHOULDER; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP) { mControllerButton = EventListener::Event::BUTTON_DPAD_UP; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN) { mControllerButton = EventListener::Event::BUTTON_DPAD_DOWN; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT) { mControllerButton = EventListener::Event::BUTTON_DPAD_LEFT; }
				else if (evn.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) { mControllerButton = EventListener::Event::BUTTON_DPAD_RIGHT; }
				//* Set Pressed in map
				controllerHeld[mControllerButton] = false;
				//* Dispatch
				Dispatch(type, mControllerButton);
				//* Used to Delay Hold
				countedButtonFrames = -controllerButtonDelay * 8;
			}

			//* Joystick
			//* Get If Axis Is Moving
			if (evn.type == SDL_JOYAXISMOTION)
			{
				//* Left Stick X Axis
				if (evn.jaxis.axis == 0) { stick_Left_X = evn.jaxis.value; }
				//* Left Stick Y Axis	   
				if (evn.jaxis.axis == 1) { stick_Left_Y = evn.jaxis.value; }
				//* Left Trigger Axis	   
				if (evn.jaxis.axis == 2) { stick_Left_T = evn.jaxis.value; }
				//* Right Stick X Axis	   
				if (evn.jaxis.axis == 3) { stick_Right_X = evn.jaxis.value; }
				//* Right Stick Y Axis	   
				if (evn.jaxis.axis == 4) { stick_Right_Y = evn.jaxis.value; }
				//* Right Trigger Axis	   
				if (evn.jaxis.axis == 5) { stick_Right_T = evn.jaxis.value; }

				// Left Trigger Press/ Relaase
				if (evn.jaxis.axis == 2) {
					if (GetLeftTrigger() > 32000) { Dispatch(EventListener::Type::Press, EventListener::Event::TRIGGER_LEFT); }
					if (GetLeftTrigger() < -32000) { Dispatch(EventListener::Type::Release, EventListener::Event::TRIGGER_LEFT); }
				}

				// Right Trigger Press/ Relaase
				if (evn.jaxis.axis == 5) {
					if (GetRightTrigger() > 32000) { Dispatch(EventListener::Type::Hold, EventListener::Event::TRIGGER_RIGHT); }
					if (GetRightTrigger() < -32000) { Dispatch(EventListener::Type::Hold, EventListener::Event::TRIGGER_RIGHT); }
				}
			}
		}

		//* Mouse
		if (evn.type == SDL_MOUSEBUTTONDOWN)
		{
			type = EventListener::Type::Press;
			if (evn.button.button == SDL_BUTTON_LEFT) { Dispatch(type, EventListener::Event::MOUSE_LEFT); }
			else if (evn.button.button == SDL_BUTTON_RIGHT) { Dispatch(type, EventListener::Event::MOUSE_RIGHT); }
			else if (evn.button.button == SDL_BUTTON_MIDDLE) { Dispatch(type, EventListener::Event::MOUSE_MIDDLE); }
		}
		if (evn.type == SDL_MOUSEBUTTONUP)
		{
			type = EventListener::Type::Release;
			if (evn.button.button == SDL_BUTTON_LEFT) { Dispatch(type, EventListener::Event::MOUSE_LEFT); }
			else if (evn.button.button == SDL_BUTTON_RIGHT) { Dispatch(type, EventListener::Event::MOUSE_RIGHT); }
			else if (evn.button.button == SDL_BUTTON_MIDDLE) { Dispatch(type, EventListener::Event::MOUSE_MIDDLE); }
		}

		if (evn.type == SDL_MOUSEWHEEL)
		{
			type = EventListener::Type::Press;
			if (evn.wheel.y > 0)
			{
				Dispatch(type, EventListener::Event::MOUSE_WHEEL_UP);
				SetPrevious(EventListener::Event::MOUSE_WHEEL_UP, false);
			}
			else if (evn.wheel.y < 0)
			{
				Dispatch(type, EventListener::Event::MOUSE_WHEEL_DOWN);
				SetPrevious(EventListener::Event::MOUSE_WHEEL_DOWN, false);
			}
		}

		//\\ Add new Input Library Events here for custom detection

	}

	//* Controller is connected
	if (mIsConnected)
	{
		//* Controller Button Hold Update
		countedButtonFrames++;
		if (countedButtonFrames > controllerButtonDelay)
		{
			//* For Each Button in the Map
			for (auto const &button : controllerHeld)
			{
				if (button.second) { Dispatch(EventListener::Type::Hold, button.first); }
				else { controllerHeld[button.first] = false; }
			}
			countedButtonFrames = 0;
		}

		//* Controller Trigger Hold Update
		countedTriggerFrames++;
		if (countedTriggerFrames > controllerButtonDelay)
		{
			if (GetLeftTrigger() > 0) { Dispatch(EventListener::Type::Hold, EventListener::Event::TRIGGER_LEFT); }
			else if (GetLeftTrigger() > -30000 && GetLeftTrigger() < 0) { Dispatch(EventListener::Type::Hold, EventListener::Event::TRIGGER_SOFT_LEFT); }

			if (GetRightTrigger() > 0) { Dispatch(EventListener::Type::Hold, EventListener::Event::TRIGGER_RIGHT); }
			else if (GetRightTrigger() > -30000 && GetRightTrigger() < 0) { Dispatch(EventListener::Type::Hold, EventListener::Event::TRIGGER_SOFT_RIGHT); }

			countedTriggerFrames = 0;
		}
	}
}

void InputManager::ConstantInput()
{
	const Uint8 *keystates = SDL_GetKeyboardState(NULL);

	EventListener::Type type = EventListener::Type::Down;

	SDL_PumpEvents();

	//* Get Key Event and call Input Manager Dispatch for that key
	if (keystates[SDL_GetScancodeFromKey(SDLK_UNKNOWN)]) { Dispatch(type, EventListener::Event::UNKNOWN);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_RETURN)]) { Dispatch(type, EventListener::Event::RETURN);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_ESCAPE)]) { Dispatch(type, EventListener::Event::ESCAPE);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_BACKSPACE)]) { Dispatch(type, EventListener::Event::BACKSPACE);		}
	if (keystates[SDL_GetScancodeFromKey(SDLK_TAB)]) { Dispatch(type, EventListener::Event::TAB);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_SPACE)]) { Dispatch(type, EventListener::Event::SPACE);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_EXCLAIM)]) { Dispatch(type, EventListener::Event::EXCLAIM);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_QUOTEDBL)]) { Dispatch(type, EventListener::Event::QUOTEDBL);		}
	if (keystates[SDL_GetScancodeFromKey(SDLK_HASH)]) { Dispatch(type, EventListener::Event::HASH);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_PERCENT)]) { Dispatch(type, EventListener::Event::PERCENT);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_DOLLAR)]) { Dispatch(type, EventListener::Event::DOLLAR);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_AMPERSAND)]) { Dispatch(type, EventListener::Event::AMPERSAND);		}
	if (keystates[SDL_GetScancodeFromKey(SDLK_QUOTE)]) { Dispatch(type, EventListener::Event::QUOTE);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_LEFTPAREN)]) { Dispatch(type, EventListener::Event::LEFTPAREN);		}
	if (keystates[SDL_GetScancodeFromKey(SDLK_RIGHTPAREN)]) { Dispatch(type, EventListener::Event::RIGHTPAREN);		}
	if (keystates[SDL_GetScancodeFromKey(SDLK_ASTERISK)]) { Dispatch(type, EventListener::Event::ASTERISK);		}
	if (keystates[SDL_GetScancodeFromKey(SDLK_PLUS)]) { Dispatch(type, EventListener::Event::PLUS);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_COMMA)]) { Dispatch(type, EventListener::Event::COMMA);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_MINUS)]) { Dispatch(type, EventListener::Event::MINUS);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_PERIOD)]) { Dispatch(type, EventListener::Event::PERIOD);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_SLASH)]) { Dispatch(type, EventListener::Event::SLASH);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_0)]) { Dispatch(type, EventListener::Event::NUM_0);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_1)]) { Dispatch(type, EventListener::Event::NUM_1);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_2)]) { Dispatch(type, EventListener::Event::NUM_2);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_3)]) { Dispatch(type, EventListener::Event::NUM_3);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_4)]) { Dispatch(type, EventListener::Event::NUM_4);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_5)]) { Dispatch(type, EventListener::Event::NUM_5);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_6)]) { Dispatch(type, EventListener::Event::NUM_6);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_7)]) { Dispatch(type, EventListener::Event::NUM_7);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_8)]) { Dispatch(type, EventListener::Event::NUM_8);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_9)]) { Dispatch(type, EventListener::Event::NUM_9);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_COLON)]) { Dispatch(type, EventListener::Event::COLON);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_SEMICOLON)]) { Dispatch(type, EventListener::Event::SEMICOLON);		}
	if (keystates[SDL_GetScancodeFromKey(SDLK_LESS)]) { Dispatch(type, EventListener::Event::LESS);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_EQUALS)]) { Dispatch(type, EventListener::Event::EQUALS);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_GREATER)]) { Dispatch(type, EventListener::Event::GREATER);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_QUESTION)]) { Dispatch(type, EventListener::Event::QUESTION);		}
	if (keystates[SDL_GetScancodeFromKey(SDLK_AT)]) { Dispatch(type, EventListener::Event::AT);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_LEFTBRACKET)]) { Dispatch(type, EventListener::Event::LEFTBRACKET);		}
	if (keystates[SDL_GetScancodeFromKey(SDLK_BACKSLASH)]) { Dispatch(type, EventListener::Event::BACKSLASH);		}
	if (keystates[SDL_GetScancodeFromKey(SDLK_RIGHTBRACKET)]) { Dispatch(type, EventListener::Event::RIGHTBRACKET);	}
	if (keystates[SDL_GetScancodeFromKey(SDLK_CARET)]) { Dispatch(type, EventListener::Event::CARET);			}
	if (keystates[SDL_GetScancodeFromKey(SDLK_UNDERSCORE)]) { Dispatch(type, EventListener::Event::UNDERSCORE);		}
	if (keystates[SDL_GetScancodeFromKey(SDLK_BACKQUOTE)]) { Dispatch(type, EventListener::Event::BACKQUOTE);		}
	if (keystates[SDL_GetScancodeFromKey(SDLK_a)]) { Dispatch(type, EventListener::Event::a);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_b)]) { Dispatch(type, EventListener::Event::b);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_c)]) { Dispatch(type, EventListener::Event::c);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_d)]) { Dispatch(type, EventListener::Event::d);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_e)]) { Dispatch(type, EventListener::Event::e);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_f)]) { Dispatch(type, EventListener::Event::f);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_g)]) { Dispatch(type, EventListener::Event::g);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_h)]) { Dispatch(type, EventListener::Event::h);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_i)]) { Dispatch(type, EventListener::Event::i);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_j)]) { Dispatch(type, EventListener::Event::j);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_k)]) { Dispatch(type, EventListener::Event::k);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_l)]) { Dispatch(type, EventListener::Event::l);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_m)]) { Dispatch(type, EventListener::Event::m);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_n)]) { Dispatch(type, EventListener::Event::n);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_o)]) { Dispatch(type, EventListener::Event::o);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_p)]) { Dispatch(type, EventListener::Event::p);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_q)]) { Dispatch(type, EventListener::Event::q);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_r)]) { Dispatch(type, EventListener::Event::r);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_s)]) { Dispatch(type, EventListener::Event::s);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_t)]) { Dispatch(type, EventListener::Event::t);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_u)]) { Dispatch(type, EventListener::Event::u);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_v)]) { Dispatch(type, EventListener::Event::v);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_w)]) { Dispatch(type, EventListener::Event::w);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_x)]) { Dispatch(type, EventListener::Event::x);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_y)]) { Dispatch(type, EventListener::Event::y);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_z)]) { Dispatch(type, EventListener::Event::z);				}
	if (keystates[SDL_GetScancodeFromKey(SDLK_UP)]) { Dispatch(type, EventListener::Event::ARROW_UP);		}
	if (keystates[SDL_GetScancodeFromKey(SDLK_LEFT)]) { Dispatch(type, EventListener::Event::ARROW_LEFT);		}
	if (keystates[SDL_GetScancodeFromKey(SDLK_DOWN)]) { Dispatch(type, EventListener::Event::ARROW_DOWN);		}
	if (keystates[SDL_GetScancodeFromKey(SDLK_RIGHT)]) { 	Dispatch(type, EventListener::Event::ARROW_RIGHT);	}

}

//// Controller  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//* Add Contoller if Detected
void InputManager::AddController(int id)
{
	//* Assign Current Controller
	if (SDL_IsGameController(id))
	{
		mGameController = SDL_GameControllerOpen(id);
		std::cout << SDL_GameControllerMapping(mGameController) << std::endl;
		mIsConnected = true;

		if (mGameController)
		{
			//* Assign Joysticks
			mJoyStick = SDL_JoystickOpen(id);
			int instanceID = SDL_JoystickInstanceID(mJoyStick);
		}
	}
}

//* Disconnect Controller
void InputManager::RemoveController(int id)
{
	SDL_GameControllerClose(mGameController);
	mIsConnected = false;
}

//* Set Delay of Controller Update
void InputManager::SetControllerButtonDelay(int delay)
{
	controllerButtonDelay = delay;
}

//* Set Delay of Controller Update
void InputManager::SetControllerTriggerDelay(int delay)
{
	controllerTriggerDelay = delay;
}

//* Set the size of the Stick Dead Zone
void InputManager::SetStickDeadZone(int deadZone)
{
	stick_Dead_Zone = deadZone;
}

//* Return X, Y for Left Stick Vector
Vector2f InputManager::GetLeftStickVector()
{
	if (Vector2f(stick_Left_X, stick_Left_Y).magnitude() > stick_Dead_Zone)
	{
		return Vector2f(stick_Left_X, stick_Left_Y);
	}
	else
	{
		return Vector2f(0, 0);
	}
}

//* Return X, Y for Left Stick Vector Normal
Vector2f InputManager::GetLeftStickVectorNormal()
{
	if (Vector2f(stick_Left_X, stick_Left_Y).magnitude() > stick_Dead_Zone)
	{
		return Vector2f(stick_Left_X, stick_Left_Y).normalise();
	}
	else
	{
		return Vector2f(0, 0);
	}
}

//* Return X, Y for Left Stick Angle
float InputManager::GetLeftStickAngle()
{
	return (180 - (180 * (atan2(stick_Left_X, stick_Left_Y)) / M_PI));
}

//* Return Left Trigger Value
float InputManager::GetLeftTrigger()
{
	return stick_Left_T;
}

//* Return X, Y for Right Stick Vector
Vector2f InputManager::GetRightStickVector()
{
	if (Vector2f(stick_Right_X, stick_Right_Y).magnitude() > stick_Dead_Zone)
	{
		return Vector2f(stick_Right_X, stick_Right_Y);
	}
	else
	{
		return Vector2f(0, 0);
	}
}

//* Return X, Y for Right Stick Vector Normal
Vector2f InputManager::GetRightStickVectorNormal()
{
	if (Vector2f(stick_Right_X, stick_Right_Y).magnitude() > stick_Dead_Zone)
	{
		return Vector2f(stick_Right_X, stick_Right_Y);
	}
	else
	{
		return Vector2f(0, 0);
	}
}

//* Return X, Y for Right Stick Angle
float InputManager::GetRightStickAngle()
{
	return (180 - (180 * (atan2(stick_Right_X, stick_Right_Y)) / M_PI));
}

//* Return Right Trigger Value
float InputManager::GetRightTrigger()
{
	return stick_Right_T;
}

//* Mouse
SDL_Point InputManager::GetMousePos()
{
	int x;
	int y;
	SDL_GetMouseState(&x, &y);

	return SDL_Point{ x, y };
}


//* Logger //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//* Add Event to output Log
void InputManager::logEvent(std::string str)
{
	logFileValues.push_back(str);
}

//* Save the Log File
void InputManager::saveFile()
{
	if (saved == false)
	{
		logFileName = "Logs/Input Log = "
			+ GetTimeStamp(true)
			+ ".csv";

		std::string log;

		for (int i = 0; i < logFileValues.size(); i++)
		{
			log += logFileValues[i] + "\n";
		}

		std::ofstream output_file(logFileName);
		output_file << log;
		logFileValues.clear();
		log = "";

		saved = true;
	}
}

//* Return the Current System Time
std::string InputManager::GetTimeStamp(bool save)
{
	time_t t = time(0);
	struct tm now;

	localtime_s(&now, &t);

	std::string timeStamp;

	if (save == true)
	{
		timeStamp = ""
			+ std::to_string(now.tm_mday) + "-"
			+ std::to_string(now.tm_mon + 1) + "-"
			+ std::to_string(now.tm_year + 1900) + "_"
			+ std::to_string(now.tm_hour) + "-"
			+ std::to_string(now.tm_min) + "-"
			+ std::to_string(now.tm_sec);
	}
	else
	{
		timeStamp = ""
			+ std::to_string(now.tm_mday) + "/"
			+ std::to_string(now.tm_mon + 1) + "/"
			+ std::to_string(now.tm_year + 1900) + " - "
			+ std::to_string(now.tm_hour) + ":"
			+ std::to_string(now.tm_min) + ":"
			+ std::to_string(now.tm_sec);
	}

	return timeStamp;
}

//* Fill the map for Key Logger
void InputManager::createKeyMap()
{
	logFileValues.push_back("Key, Type, Duration, Time Stamp");

	keyNames[EventListener::Event::RETURN] = "Return";
	keyNames[EventListener::Event::ESCAPE] = "Escape";
	keyNames[EventListener::Event::BACKSPACE] = "BackSpace";
	keyNames[EventListener::Event::TAB] = "Tab";
	keyNames[EventListener::Event::SPACE] = "Space";
	keyNames[EventListener::Event::EXCLAIM] = "!";
	keyNames[EventListener::Event::QUOTEDBL] = "Half Quote";
	keyNames[EventListener::Event::HASH] = "#";
	keyNames[EventListener::Event::PERCENT] = "%";
	keyNames[EventListener::Event::DOLLAR] = "$";
	keyNames[EventListener::Event::AMPERSAND] = "&";
	keyNames[EventListener::Event::QUOTE] = "Quote";
	keyNames[EventListener::Event::LEFTPAREN] = "(";
	keyNames[EventListener::Event::RIGHTPAREN] = ")";
	keyNames[EventListener::Event::ASTERISK] = "*";
	keyNames[EventListener::Event::PLUS] = "+";
	keyNames[EventListener::Event::COMMA] = "Comma";
	keyNames[EventListener::Event::MINUS] = "-";
	keyNames[EventListener::Event::PERIOD] = ".";
	keyNames[EventListener::Event::SLASH] = "/";
	keyNames[EventListener::Event::NUM_0] = "0";
	keyNames[EventListener::Event::NUM_1] = "1";
	keyNames[EventListener::Event::NUM_2] = "2";
	keyNames[EventListener::Event::NUM_3] = "3";
	keyNames[EventListener::Event::NUM_4] = "4";
	keyNames[EventListener::Event::NUM_5] = "5";
	keyNames[EventListener::Event::NUM_6] = "6";
	keyNames[EventListener::Event::NUM_7] = "7";
	keyNames[EventListener::Event::NUM_8] = "8";
	keyNames[EventListener::Event::NUM_9] = "9";
	keyNames[EventListener::Event::COLON] = "Colon";
	keyNames[EventListener::Event::SEMICOLON] = "Semi Colon";
	keyNames[EventListener::Event::LESS] = "<";
	keyNames[EventListener::Event::EQUALS] = "=";
	keyNames[EventListener::Event::GREATER] = ">";
	keyNames[EventListener::Event::QUESTION] = "?";
	keyNames[EventListener::Event::AT] = "@";

	// Skip uppercase letters
	keyNames[EventListener::Event::LEFTBRACKET] = "[";
	keyNames[EventListener::Event::BACKSLASH] = "\\";
	keyNames[EventListener::Event::RIGHTBRACKET] = "]";
	keyNames[EventListener::Event::CARET] = "^";
	keyNames[EventListener::Event::UNDERSCORE] = "_";
	keyNames[EventListener::Event::BACKQUOTE] = "`";
	keyNames[EventListener::Event::a] = "a";
	keyNames[EventListener::Event::b] = "b";
	keyNames[EventListener::Event::c] = "c";
	keyNames[EventListener::Event::d] = "d";
	keyNames[EventListener::Event::e] = "e";
	keyNames[EventListener::Event::f] = "f";
	keyNames[EventListener::Event::g] = "g";
	keyNames[EventListener::Event::h] = "h";
	keyNames[EventListener::Event::i] = "i";
	keyNames[EventListener::Event::j] = "j";
	keyNames[EventListener::Event::k] = "k";
	keyNames[EventListener::Event::l] = "l";
	keyNames[EventListener::Event::m] = "m";
	keyNames[EventListener::Event::n] = "n";
	keyNames[EventListener::Event::o] = "o";
	keyNames[EventListener::Event::p] = "p";
	keyNames[EventListener::Event::q] = "q";
	keyNames[EventListener::Event::r] = "r";
	keyNames[EventListener::Event::s] = "s";
	keyNames[EventListener::Event::t] = "t";
	keyNames[EventListener::Event::u] = "u";
	keyNames[EventListener::Event::v] = "v";
	keyNames[EventListener::Event::w] = "w";
	keyNames[EventListener::Event::x] = "x";
	keyNames[EventListener::Event::y] = "y";
	keyNames[EventListener::Event::z] = "z";

	// Controller
	keyNames[EventListener::Event::BUTTON_INVALID] = "Invalid";
	keyNames[EventListener::Event::BUTTON_A] = "Controller A";
	keyNames[EventListener::Event::BUTTON_B] = "Controller B";
	keyNames[EventListener::Event::BUTTON_X] = "Controller X";
	keyNames[EventListener::Event::BUTTON_Y] = "Controller Y";
	keyNames[EventListener::Event::BUTTON_BACK] = "Controller Back";
	keyNames[EventListener::Event::BUTTON_GUIDE] = "Controller Guide";
	keyNames[EventListener::Event::BUTTON_START] = "Controller Start";
	keyNames[EventListener::Event::BUTTON_LEFTSTICK] = "Controller Left Stick";
	keyNames[EventListener::Event::BUTTON_RIGHTSTICK] = "Controller Right Stick";
	keyNames[EventListener::Event::BUTTON_LEFTSHOULDER] = "Controller Left Shoulder";
	keyNames[EventListener::Event::BUTTON_RIGHTSHOULDER] = "Controller Right Shoulder";
	keyNames[EventListener::Event::BUTTON_DPAD_UP] = "D Pad Up";
	keyNames[EventListener::Event::BUTTON_DPAD_DOWN] = "D Pad Down";
	keyNames[EventListener::Event::BUTTON_DPAD_LEFT] = "D Pad Left";
	keyNames[EventListener::Event::BUTTON_DPAD_RIGHT] = "D Pad Right";
	keyNames[EventListener::Event::TRIGGER_SOFT_LEFT] = "Trigger Soft Left";
	keyNames[EventListener::Event::TRIGGER_LEFT] = "Trigger Hard Left";
	keyNames[EventListener::Event::TRIGGER_SOFT_RIGHT] = "Trigger Soft Right";
	keyNames[EventListener::Event::TRIGGER_RIGHT] = "Trigger Hard Right";

	// Mouse
	keyNames[EventListener::Event::MOUSE_LEFT] = "Mouse Left";
	keyNames[EventListener::Event::MOUSE_RIGHT] = "Mouse Right";
	keyNames[EventListener::Event::MOUSE_MIDDLE] = "Mouse Middle";
	keyNames[EventListener::Event::MOUSE_WHEEL_UP] = "Mouse Wheel Up";
	keyNames[EventListener::Event::MOUSE_WHEEL_DOWN] = "Mouse Wheel Down";

	// Types
	keyTypes[EventListener::Type::Press] = "Press";
	keyTypes[EventListener::Type::Release] = "Release";
	keyTypes[EventListener::Type::Hold] = "Hold";
	keyTypes[EventListener::Type::Down] = "Down";
}

EventListener::~EventListener()
{
}
