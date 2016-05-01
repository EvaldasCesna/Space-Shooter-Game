#include <Game/Application.h>
#include <Database/Database.h>
#include <stdexcept>
#include <iostream>

int main() {
	//Run in 32bit x86
	try {
		Application app;
		open();
		createTable();
		//insert();
		select();
		app.run();
	}
	catch (std::exception& e) {
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
	}
}
