#include "repository.h" 
#include "controller.h" 
#include "ui.h"        
#include <iostream>     
#include <exception>   
using namespace std;

int main()
{
	try
	{
		EventRepository repo("events.txt");
		AdminController adminCtrl(repo);
		UserController userCtrl(repo);
		UI ui(adminCtrl, userCtrl);
		ui.run();
	}
	catch (const EventException& e)
	{
		cerr << "A critical application error occurred: " << e.what() << endl;
		return 1;
	}
	catch (const std::exception& e)
	{
		cerr << "An unexpected standard library error occurred: " << e.what() << endl;
		return 1;
	}
	catch (...)
	{
		cerr << "An unknown critical error occurred." << endl;
		return 1;
	}
	return 0;
}