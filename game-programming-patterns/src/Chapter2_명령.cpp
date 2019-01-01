#include "Chapter2_명령.h"
#pragma warning(disable : 4996)	// disable _sleep warning

void CHAPTER_2_COMMAND::ExampleClass::operator()()
{
	bool stopFlag{ false };

	std::thread inputThread = std::thread(
		[/* void */](bool& InStopFlag, InputHandler& InInputHandler)
		noexcept -> auto 
	{
		char charBuffer{};
	
		while (!InStopFlag)
		{
			std::cin >> charBuffer;
	
			if (charBuffer == 'q')
			{
				InStopFlag = true;
				break;
			}
	
			InInputHandler.GetPlayerInput(charBuffer);
		}
	}
		, std::reference_wrapper<bool>(stopFlag)
		, std::reference_wrapper<InputHandler>(inputhandler)
	);
	
	std::thread loadCommandTherad = std::thread(
		[/* void */](bool& InStopFlag, CommandManager& InCommandManager, InputHandler& InInputHandler)
		noexcept -> auto 
	{
		while (!InStopFlag)
		{
			std::cout << "[ Load ] Start!    ";
			InCommandManager.LoadCommand(InInputHandler);
			std::cout << "    End!\n";
			_sleep(1000);
		}
	}
		, std::reference_wrapper<bool>(stopFlag)
		, std::reference_wrapper<CommandManager>(commandManager)
		, std::reference_wrapper<InputHandler>(inputhandler)
	);
	
	std::thread executeCommandTherad = std::thread(
		[/* void */](bool& InStopFlag, CommandManager& InCommandManager)
		noexcept -> auto 
	{
		while (!InStopFlag)
		{
			std::cout << "[ Execute ] \n --- Start!    \n";
			InCommandManager.ExecuteCommand();
			std::cout << " --- End!\n";
			_sleep(3000);
		}
	}
		, std::reference_wrapper<bool>(stopFlag)
		, std::reference_wrapper<CommandManager>(commandManager)
	);
	
	inputThread.join();
	loadCommandTherad.join();
	executeCommandTherad.join();

	std::cout << "\n [ERROR]일부 메모리 누수가 발생할 가능성이 있습니다...ㅎ\n";
}




void CHAPTER_2_COMMAND::JumpCommand::Execute(GameActor& InActor) 
{
	InActor.Jump(); 
}

void CHAPTER_2_COMMAND::FireCommand::Execute(GameActor& InActor)
{
	InActor.Fire();
}

void CHAPTER_2_COMMAND::RunCommand::Execute(GameActor& InActor)
{
	InActor.Run();
}

void CHAPTER_2_COMMAND::SleepCommand::Execute(GameActor& InActor)
{
	InActor.Sleep();
}


CHAPTER_2_COMMAND::InputHandler::InputHandler() noexcept
	: Button_X('x'), Button_Y('y'), Button_A('a'), Button_B('b')
	, inputtedChar('c')
{
	buttonX = new JumpCommand();
	buttonY = new FireCommand();
	buttonA = new RunCommand();
	buttonB = new SleepCommand();
};

CHAPTER_2_COMMAND::InputHandler::~InputHandler()
{
	delete buttonX;
	delete buttonY;
	delete buttonA;
	delete buttonB;
}


void CHAPTER_2_COMMAND::InputHandler::GetPlayerInput(const char InChar)
{
	inputtedChar = InChar;
}

bool CHAPTER_2_COMMAND::InputHandler::isPressed(const char InChar)
{
	return inputtedChar == InChar
		? [ /* void */](char& InInputtedChar) 
		noexcept -> bool {InInputtedChar = 'c'; return true; }
		(std::ref(inputtedChar))
		: false;
}

CHAPTER_2_COMMAND::BaseCommand* CHAPTER_2_COMMAND::InputHandler::HandleInput()
{
	if (isPressed(Button_X)) { /* std::cout << "Return X\n"; */ return buttonX; }
	if (isPressed(Button_Y)) { /* std::cout << "Return Y\n"; */ return buttonY; }
	if (isPressed(Button_A)) { /* std::cout << "Return A\n"; */ return buttonA; }
	if (isPressed(Button_B)) { /* std::cout << "Return B\n"; */ return buttonB; }
	return nullptr;
}


void CHAPTER_2_COMMAND::CommandManager::LoadCommand(InputHandler& InInputHandler)
{
	BaseCommand* command = InInputHandler.HandleInput();
	
	if(command != nullptr)
		commandQueue.emplace_back(command);
}

void CHAPTER_2_COMMAND::CommandManager::ExecuteCommand()
{
	if (commandQueue.size())
	{
		if (BaseCommand* command = commandQueue.front(); command != nullptr)
		{
			command->Execute(actor);
			commandQueue.pop_front();
		}
	}
}