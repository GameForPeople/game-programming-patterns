#include "Chapter2_명령.h"
#pragma warning(disable : 4996)

void CHAPTER_2_COMMAND::ExampleClass::operator()()
{
	bool stopFlag{ false };

	std::thread inputThread = std::thread(
		[](bool& InStopFlag, InputHandler& InInputHandler) 
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
		, std::ref(stopFlag)
		, std::ref(inputhandler)
	);
	
	std::thread loadCommandTherad = std::thread(
		[](bool& InStopFlag, CommandManager& InCommandManager, InputHandler& InInputHandler)
	{
		while (!InStopFlag)
		{
			std::cout << "Load Start!\n";
			InCommandManager.LoadCommand(InInputHandler);
			std::cout << "Load End!\n";
			_sleep(1000);
		}
	}
		, std::ref(stopFlag)
		, std::ref(commandManager)
		, std::ref(inputhandler)
	);
	
	std::thread executeCommandTherad = std::thread(
		[](bool& InStopFlag, CommandManager& InCommandManager)
	{
		while (!InStopFlag)
		{
			std::cout << "Execute Start!\n";
			InCommandManager.ExecuteCommand();
			std::cout << "Execute End!\n";
			_sleep(3000);
		}
	}
		, std::ref(stopFlag)
		, std::ref(commandManager)
	);
	
	inputThread.join();
	loadCommandTherad.join();
	executeCommandTherad.join();

	std::cout << "\n [ERROR]일부 메모리 누수가 발생합니다..ㅎ\n";
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


CHAPTER_2_COMMAND::InputHandler::InputHandler() 
	: Button_X('x'), Button_Y('y'), Button_A('a'), Button_B('b')
	, inputtedChar('c')
{
	buttonX = new JumpCommand();
	buttonY = new FireCommand();
	buttonA = new RunCommand();
	buttonB = new SleepCommand();
};


void CHAPTER_2_COMMAND::InputHandler::GetPlayerInput(const char InChar)
{
	inputtedChar = InChar;
}

bool CHAPTER_2_COMMAND::InputHandler::isPressed(const char InChar)
{
	return inputtedChar == InChar
		? true
		: false;
}

CHAPTER_2_COMMAND::BaseCommand* CHAPTER_2_COMMAND::InputHandler::HandleInput()
{
	if (isPressed(Button_X)) return buttonX;
	if (isPressed(Button_Y)) return buttonY;
	if (isPressed(Button_A)) return buttonA;
	if (isPressed(Button_B)) return buttonB;
	return nullptr;
}


void CHAPTER_2_COMMAND::CommandManager::LoadCommand(InputHandler& InInputHandler)
{
	BaseCommand* command = InInputHandler.HandleInput();
	commandQueue.emplace_back(command);
}

void CHAPTER_2_COMMAND::CommandManager::ExecuteCommand()
{
	if (BaseCommand* command = commandQueue.front(); command != nullptr)
	{
		command->Execute(actor);
		commandQueue.pop_front();
	}
}