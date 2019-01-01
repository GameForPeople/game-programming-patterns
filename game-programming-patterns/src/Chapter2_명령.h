#pragma once

/*
	game-programming-patterns 
	Chapter2 명령

	"명령 패턴은 메서드 호출을 실체화한 것이다."
	"명령 패턴은 콜백을 객체지향적으로 표현한 것"

	2.1 입력키 변경, 2.2 액터에게 지시하기 예제 구현.
*/

#include <iostream>

#include <thread>

#include <deque>

namespace CHAPTER_2_COMMAND
{
	class BaseCommand;

	class GameActor
	{
	public:
		void Jump() { std::cout << " Jump!\n"; }
		void Fire() { std::cout << " Fire!\n"; }
		void Run() { std::cout << " Run!\n"; }
		void Sleep() { std::cout << " Sleep!\n"; }
	};

	class InputHandler
	{
		BaseCommand*		buttonX;
		BaseCommand*		buttonY;
		BaseCommand*		buttonA;
		BaseCommand*		buttonB;

		char				inputtedChar;

		char				Button_X;
		char				Button_Y;
		char				Button_A;
		char				Button_B;
		
	public:
		InputHandler() noexcept;
		~InputHandler();

	public:
		BaseCommand* HandleInput();
		void GetPlayerInput(const char InChar);

	private:
		bool isPressed(const char InChar);
	};

	class CommandManager
	{
		std::deque<BaseCommand*>	commandQueue;

		GameActor					actor;
	public:
		void LoadCommand(InputHandler& InInputHandler);
		void ExecuteCommand();
	};

	class ExampleClass
	{
	private:
		CommandManager			commandManager;
		InputHandler			inputhandler;

	public:
		void operator()();
	};

#pragma region [ COMMAND ]
	class BaseCommand 
	{
	public:
		BaseCommand() = default;
		virtual ~BaseCommand() = 0 {};

	public:
		virtual void Execute(GameActor& InActor) = 0;
	};

	class JumpCommand : public BaseCommand
	{
	public:
		virtual void Execute(GameActor& InActor) override;
	};

	class FireCommand : public BaseCommand
	{
	public:
		virtual void Execute(GameActor& InActor) override;
	};

	class RunCommand : public BaseCommand
	{
	public:
		virtual void Execute(GameActor& InActor) override; 
	};

	class SleepCommand : public BaseCommand
	{
	public:
		virtual void Execute(GameActor& InActor) override;
	};
#pragma endregion

}