#include <iostream>
#include <ctime>

#include "main.h"

int main(int argc, char * argv[])
{
	clock_t startTime{}, endTime{};

	startTime = clock();
	//---------------------------

	CHAPTER_2_COMMAND::ExampleClass exampleClass;
	exampleClass();

	//----------------------------
	endTime = clock();

	printf("[�Լ� �ҿ� �ð�] : %.3f \n", static_cast<float>(endTime - startTime) / CLOCKS_PER_SEC);

	system("pause");
	return 0;
}