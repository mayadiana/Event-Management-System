#include "ui.h"
#include <iostream> 
#include <string>
#include <vector>  
#include <limits>   
#include <ios>   
#include <algorithm> 
#include <cctype>
using namespace std;

void UI::displayMainMenu()
{
    cout << "\nMAIN MENU\n";
    cout << "Choose mode:\n";
    cout << "1. Administrator\n";
    cout << "2. User\n";
    cout << "0. Exit\n";
    //cout << "Enter choice: ";
}

void UI::displayAdminMenu()
{
    cout << "\nADMIN MENU\n";
    cout << "1. Add event\n";
    cout << "2. Remove event\n";
    cout << "3. Update event\n";
    cout << "4. List all events\n";
    cout << "0. Back to main menu\n";
    cout << "Choose an option: ";
}

void UI::displayUserMenu()
{
    cout << "\nUSER MENU\n";
    cout << "1. Choose format for event list (CSV/HTML)\n";
    cout << "2. Browse events by month (and add to list)\n";
    cout << "3. Delete event from my list\n";
    cout << "4. View my interest list (console)\n";
    cout << "5. Display my interest list (opens file)\n";
    cout << "0. Back to main menu\n";
    cout << "Choose an option: ";
}

int UI::readChoice(int min, int max, const string& prompt)
{
    int choice = -1;
    while (true)
    {
        cout << prompt;
        cin >> choice;
        if (cin.good() && choice >= min && choice <= max)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }
        cout << "Error: Invalid choice. Please enter a number between " << min << " and " << max << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int UI::readPositiveInteger(const string& prompt)
{
    int value = -1;
    while (true)
    {
        cout << prompt;
        cin >> value;
        if (value < 0 || cin.fail())
        {
            cout << "Error: Invalid input. Please enter a non-negative number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            value = -1;
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

string UI::readNonEmptyString(const string& prompt)
{
    string value = "";
    while (true)
    {
        cout << prompt;
        getline(cin >> ws, value);
        if (!value.empty())
            return value;
        else
            cout << "Error: Input cannot be empty.\n";
    }
}


bool UI::readDateTimeFromUser(DateTime& dt)
{
    cout << "Enter date and time for the event:\n";
    dt.year = readPositiveInteger("Year (> 0): ");
    dt.month = readPositiveInteger("Month (1-12): ");
    dt.day = readPositiveInteger("Day (1-31): ");
    dt.hour = readPositiveInteger("Hour (0-23): ");
    dt.minute = readPositiveInteger("Minute (0-59): ");

    try
    {
        Validator::validateDateTime(dt);
        return true;
    }
    catch (const ValidationException& e)
    {
        cout << "Date/Time validation error: " << e.what() << endl;
        return false;
    }
}

void UI::printEvent(const Event& event, int index)
{
    if (index != -1)
        cout << "\nEvent " << (index + 1) << ": " << endl;
    cout << event << endl;
}

void UI::handleAdminAdd()
{
    cout << "\nAdd New Event\n";
    string title = readNonEmptyString("Enter event title: ");
    string description = readNonEmptyString("Enter event description: ");
    DateTime dt;

    if (!readDateTimeFromUser(dt))
    {
        cout << "Aborting add operation due to invalid date/time.\n";
        return;
    }

    int attendees = readPositiveInteger("Enter initial number of attendees: ");
    string link = readNonEmptyString("Enter event link (e.g.: https://...): ");

    try
    {
        adminCtrl.addEvent(title, description, dt, attendees, link);
        cout << "Event added successfully.\n";
    }
    catch (const EventException& e)
    {
        cout << "Error adding event: " << e.what() << endl;
    }
    catch (const std::exception& e)
    {
        cout << "An unexpected error occurred: " << e.what() << endl;
    }
}

void UI::handleAdminRemove()
{
    cout << "\nRemove Event\n";
    string title = readNonEmptyString("Enter title of event to remove: ");
    DateTime dt;
    cout << "Enter date/time of event to remove.\n";
    if (!readDateTimeFromUser(dt))
    {
        cout << "Aborting remove operation due to invalid date/time input.\n";
        return;
    }

    try
    {
        adminCtrl.removeEvent(title, dt);
        cout << "Event removed successfully.\n";
    }
    catch (const EventException& e)
    {
        cout << "Error removing event: " << e.what() << endl;
    }
    catch (const exception& e)
    {
        cout << "An unexpected error occurred: " << e.what() << endl;
    }
}

void UI::handleAdminUpdate()
{
    cout << "\nUpdate Event\n";
    string oldTitle = readNonEmptyString("Enter current title: ");
    DateTime oldDt;
    cout << "Enter current date/time:\n";
    if (!readDateTimeFromUser(oldDt))
    {
        cout << "Aborting update operation due to invalid current date/time input.\n";
        return;
    }

    string newTitle = readNonEmptyString("Enter new event title: ");
    string newDescription = readNonEmptyString("Enter new event description: ");
    DateTime newDt;

    cout << "Enter new date/time:\n";
    if (!readDateTimeFromUser(newDt))
    {
        cout << "Aborting update operation due to invalid new date/time input.\n";
        return;
    }

    int newAttendees = readPositiveInteger("Enter new number of attendees: ");
    string newLink = readNonEmptyString("Enter new event link: ");

    try
    {
        adminCtrl.updateEvent(oldTitle, oldDt, newTitle, newDescription, newDt, newAttendees, newLink);
        cout << "Event updated successfully.\n";
    }
    catch (const EventException& e)
    {
        cout << "Error updating event: " << e.what() << endl;
    }
    catch (const std::exception& e)
    {
        cout << "An unexpected error occurred: " << e.what() << endl;
    }
}


void UI::handleAdminListAll()
{
    cout << "\nAll Events in Repository\n";
    try
    {
        const vector<Event>& events = adminCtrl.getAllEvents();
        if (events.empty())
        {
            cout << "The repository is currently empty.\n";
            return;
        }
        int index = 0;
        for (const auto& event : events)
            printEvent(event, index++);
    }
    catch (const exception& e)
    {
        cout << "Error retrieving events: " << e.what() << endl;
    }
}

void UI::handleUserSetFileFormat()
{
    cout << "\nChoose user event list file format\n";
    cout << "1. CSV (Comma Separated Values)\n";
    cout << "2. HTML (Web Page)\n";
    int choice = readChoice(1, 2, "Select file format: ");

    UserEventListStorage* strategy = nullptr;
    if (choice == 1)
    {
        strategy = new UserEventListCSVStorage(); //"user_events.csv"
        cout << "User event list will be saved in CSV format to user_events.csv.\n";
    }
    else
    {
        strategy = new UserEventListHTMLStorage(); //"user_events.html"
        cout << "User event list will be saved in HTML format to user_events.html.\n";
    }
    userCtrl.setUserListStorageStrategy(strategy);
}

void UI::handleUserBrowseEvents()
{
    cout << "\nBrowse Events\n";
    int month = readPositiveInteger("Enter month number (1-12) to filter by, or 0 for all months: ");
    if (month < 0 || month > 12)
    {
        cout << "Invalid month number. Showing all events.\n";
        month = 0;
    }

    try
    {
        vector<Event> eventsToShow = userCtrl.getEventsByMonth(month);

        if (eventsToShow.empty())
        {
            cout << "No events found for the specified criteria.\n";
            return;
        }

        size_t currentIndex = 0;
        char browseChoice;
        bool continueBrowsing = true;

        while (continueBrowsing)
        {
            const Event& currentEvent = eventsToShow[currentIndex];

            cout << "\nDisplaying Event " << (currentIndex + 1) << " of " << eventsToShow.size() << ":\n";
            printEvent(currentEvent);

            cout << "Event link: " << currentEvent.getLink() << endl;
            cout << "\nOptions: press a (add to my list), n (next event) or s (stop browsing): ";

            cin >> browseChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            browseChoice = tolower(browseChoice);

            switch (browseChoice)
            {
            case 'a':
                try
                {
                    userCtrl.addEventToUserList(currentEvent);
                    cout << "Event '" << currentEvent.getTitle() << "' added to your list.\n";
                }
                catch (const EventException& e)
                {
                    cout << "Error: " << e.what() << endl;
                }
                break;

            case 'n':
                currentIndex = (currentIndex + 1) % eventsToShow.size();
                break;

            case 's':
                continueBrowsing = false;
                break;

            default:
                cout << "Invalid option. Please choose 'a', 'n', or 's'.\n";
                break;
            }
        }
    }
    catch (const exception& e)
    {
        cout << "Error browsing events: " << e.what() << endl;
    }
}

void UI::handleUserDeleteFromList()
{
    cout << "\nRemove Event from Your List\n";
    cout << "Your current event list:\n";
    handleUserViewList();

    const vector<Event>& userList = userCtrl.getUserEventList();
    if (userList.empty())
        return;

    string title = readNonEmptyString("Enter title of event to remove from your list: ");
    DateTime dt;
    cout << "Enter date and time of event to remove from your list.\n";
    if (!readDateTimeFromUser(dt))
    {
        cout << "Aborting remove operation due to invalid date/time input.\n";
        return;
    }

    try
    {
        userCtrl.removeEventFromUserList(title, dt);
        cout << "Event removed from your list.\n";
    }
    catch (const EventException& e)
    {
        cout << "Error removing event from list: " << e.what() << endl;
    }
    catch (const std::exception& e)
    {
        cout << "An unexpected error occurred: " << e.what() << endl;
    }
}

void UI::handleUserViewList()
{
    cout << "\nEvents you are interested in:\n";
    try
    {
        const vector<Event>& events = userCtrl.getUserEventList();
        if (events.empty())
        {
            cout << "Your interest list is currently empty.\n";
            return;
        }
        int index = 0;
        for (const auto& event : events)
            printEvent(event, index++);
    }
    catch (const exception& e)
    {
        cout << "Error retrieving user event list: " << e.what() << endl;
    }
}

void UI::handleUserDisplayListFile()
{
    cout << "\nDisplay user event list file\n";
    try
    {
        userCtrl.displayUserListFile();
    }
    catch (const EventException& e)
    {
        cout << "Error: " << e.what() << endl;
    }
    catch (const exception& e)
    {
        cout << "An unexpected error occurred while trying to display the list: " << e.what() << endl;
    }
}

void UI::run()
{
    int modeChoice = -1;

    while (modeChoice != 0)
    {
        displayMainMenu();
        modeChoice = readChoice(0, 2, "Enter choice for mode (0-2): ");

        if (modeChoice == 1)
        {
            int adminChoice = -1;
            while (adminChoice != 0)
            {
                displayAdminMenu();

                adminChoice = readChoice(0, 4, "Admin menu choice (0-4): ");

                if (adminChoice == 1)
                    handleAdminAdd();
                else if (adminChoice == 2)
                    handleAdminRemove();
                else if (adminChoice == 3)
                    handleAdminUpdate();
                else if (adminChoice == 4)
                    handleAdminListAll();
                else if (adminChoice == 0)
                    break;
            }
        }
        else if (modeChoice == 2)
        {
            int userChoice = -1;

            while (userChoice != 0)
            {
                displayUserMenu();

                userChoice = readChoice(0, 5, "User menu choice (0-5): ");

                if (userChoice == 1)
                    handleUserSetFileFormat();
                if (userChoice == 2)
                    handleUserBrowseEvents();
                else if (userChoice == 3)
                    handleUserDeleteFromList();
                else if (userChoice == 4)
                    handleUserViewList();
                else if (userChoice == 5)
                    handleUserDisplayListFile();
                else if (userChoice == 0)
                    break;
            }
        }
        else if (modeChoice == 0)
            break;
        else
            cout << "Invalid choice.Try again!";
    }
}