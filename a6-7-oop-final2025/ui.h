#ifndef UI_H
#define UI_H

#include "controller.h"
#include "domain.h" 
#include <string>

class UI
{
private:
	AdminController& adminCtrl;
	UserController& userCtrl;

	void displayMainMenu();
	void displayAdminMenu();
	void displayUserMenu();

	void handleAdminAdd();
	void handleAdminRemove();
	void handleAdminUpdate();
	void handleAdminListAll();

	void handleUserSetFileFormat();
	void handleUserBrowseEvents();
	void handleUserDeleteFromList();
	void handleUserViewList();
	void handleUserDisplayListFile();

	bool readDateTimeFromUser(DateTime& dt);

	int readPositiveInteger(const string& prompt);

	string readNonEmptyString(const string& prompt);

	int readChoice(int min, int max, const string& prompt);

	void printEvent(const Event& event, int index = -1);

public:
	UI(AdminController& ac, UserController& uc) : adminCtrl(ac), userCtrl(uc) {}

	void run();

	UI(const UI&) = delete;
	UI& operator=(const UI&) = delete;
};

#endif 