#include <Game/Application.h>
#include <stdexcept>
#include <iostream>

int main() {
	//Run in 32bit x86
	try {
		Application app;
		app.run();
	}
	catch (std::exception& e) {
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
	}
}
