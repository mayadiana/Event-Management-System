#include "controller.h"
#include <vector>       
#include <algorithm>    
#include <iostream>
#include <fstream>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h> // For ShellExecute on Windows
#endif

void AdminController::addEvent(const string& title, const string& description, const DateTime& dt, int attendees, const string& link)
{
    Validator::validateEvent(title, description, dt, attendees, link);
    Event newEvent(title, description, dt, attendees, link);
    repo.addEvent(newEvent);
}

void AdminController::removeEvent(const string& title, const DateTime& dt)
{
    Validator::validateTitle(title);
    Validator::validateDateTime(dt);
    repo.removeEvent(title, dt);
}

void AdminController::updateEvent(const string& oldTitle, const DateTime& oldDt, const string& newTitle, const string& newDescription, const DateTime& newDt, int newAttendees, const string& newLink)
{
    Validator::validateTitle(oldTitle);
    Validator::validateDateTime(oldDt);
    Validator::validateEvent(newTitle, newDescription, newDt, newAttendees, newLink);
    repo.updateEvent(oldTitle, oldDt, newTitle, newDescription, newDt, newAttendees, newLink);
}

const vector<Event>& AdminController::getAllEvents() const
{
    return repo.getAllEvents();
}


// CSV Storage
void UserEventListCSVStorage::write(const vector<Event>& events) const
{
    ofstream file(filename);
    if (!file.is_open())
        throw ControllerException("Error: Could not open CSV file '" + filename + "' for writing user list.");

    file << "Title,Description,Year,Month,Day,Hour,Minute,Attendees,Link\n";
    for (const auto& event : events)
        file << "\"" << event.getTitle() << "\"," << "\"" << event.getDescription() << "\"," << event.getDateTime().year << "," << event.getDateTime().month << "," << event.getDateTime().day << "," << event.getDateTime().hour << "," << event.getDateTime().minute << "," << event.getAttendees() << "," << "\"" << event.getLink() << "\"\n";

    file.close();
}

void UserEventListCSVStorage::display() const
{
    cout << "Displaying CSV file: " << filename << endl;

#ifdef _WIN32
    ShellExecuteA(NULL, "open", filename.c_str(), NULL, NULL, SW_SHOWNORMAL);
#endif
}


// HTML Storage
void UserEventListHTMLStorage::write(const vector<Event>& events) const
{
    ofstream file(filename);
    if (!file.is_open())
        throw ControllerException("Error: Could not open HTML file '" + filename + "' for writing user list.");

    file << "<!DOCTYPE html>\n<html>\n<head>\n<title>User Event List</title>\n" << "<style>\n" << "table { font-family: arial, sans-serif; border-collapse: collapse; width: 100%; }\n" << "td, th { border: 1px solid #dddddd; text-align: left; padding: 8px; }\n" << "tr:nth-child(even) { background-color: #f2f2f2; }\n" << "</style>\n" << "</head>\n<body>\n" << "<h2>User Event List</h2>\n" << "<table>\n" << "<tr><th>Title</th><th>Description</th><th>Date</th><th>Time</th><th>Attendees</th><th>Link</th></tr>\n";

    for (const auto& event : events)
        file << "<tr>" << "<td>" << event.getTitle() << "</td>" << "<td>" << event.getDescription() << "</td>" << "<td>" << event.getDateTime().year << "-" << event.getDateTime().month << "-" << event.getDateTime().day << "</td>" << "<td>" << event.getDateTime().hour << ":" << (event.getDateTime().minute < 10 ? "0" : "") << event.getDateTime().minute << "</td>" << "<td>" << event.getAttendees() << "</td>" << "<td><a href=\"" << event.getLink() << "\" target=\"_blank\">" << event.getLink() << "</a></td>" << "</tr>\n";

    file << "</table>\n</body>\n</html>\n";
    file.close();
}

void UserEventListHTMLStorage::display() const
{
    cout << "Displaying HTML file: " << filename << endl;
#ifdef _WIN32
    ShellExecuteA(NULL, "open", filename.c_str(), NULL, NULL, SW_SHOWNORMAL);
#endif
}

UserController::UserController(EventRepository& r) : repo(r), listStorageStrategy(nullptr) {}

UserController::~UserController()
{
    delete listStorageStrategy;
}

void UserController::setUserListStorageStrategy(UserEventListStorage* strategy)
{
    delete listStorageStrategy;
    listStorageStrategy = strategy;
}

vector<Event> UserController::getEventsByMonth(int month)
{
    const vector<Event>& allEvents = repo.getAllEvents();
    vector<Event> filteredEvents;

    if (month == 0)
        filteredEvents = allEvents;
    else
    {
        copy_if(allEvents.begin(), allEvents.end(), back_inserter(filteredEvents), [month](const Event& e)
            {
                return e.getDateTime().month == month;
            });
    }

    sort(filteredEvents.begin(), filteredEvents.end());
    return filteredEvents;
}

void UserController::addEventToUserList(const Event& eventToAdd)
{
    auto it = find(userEventList.begin(), userEventList.end(), eventToAdd);
    if (it != userEventList.end())
        throw ControllerException("Event is already in your interest list.");

    try
    {
        Event& repoEvent = repo.findEvent(eventToAdd.getTitle(), eventToAdd.getDateTime());
        userEventList.push_back(eventToAdd);
        repoEvent.incrementAttendees();
        repo.updateEvent(repoEvent.getTitle(), repoEvent.getDateTime(), repoEvent.getTitle(), repoEvent.getDescription(), repoEvent.getDateTime(), repoEvent.getAttendees(), repoEvent.getLink());
        if (listStorageStrategy)
            saveUserListToFile();
    }
    catch (const EventNotFoundException& e)
    {
        throw ControllerException("Error: Could not find the event in the main repository to update attendees.");
    }
    catch (const RepositoryException& e)
    {
        throw ControllerException("Repository error while adding event to user list: " + string(e.what()));
    }
}


void UserController::removeEventFromUserList(const string& title, const DateTime& dt)
{
    Event eventToFind;
    eventToFind.setTitle(title);
    eventToFind.setDateTime(dt);

    auto it = find(userEventList.begin(), userEventList.end(), eventToFind);
    if (it == userEventList.end()) {
        throw ControllerException("Event not found in your interest list.");
    }
    userEventList.erase(it);

    try
    {
        Event& repoEvent = repo.findEvent(title, dt);
        repoEvent.decrementAttendees();
        repo.updateEvent(repoEvent.getTitle(), repoEvent.getDateTime(), repoEvent.getTitle(), repoEvent.getDescription(), repoEvent.getDateTime(), repoEvent.getAttendees(), repoEvent.getLink());
        if (listStorageStrategy)
            saveUserListToFile();
    }
    catch (const EventNotFoundException& e)
    {
        cerr << "Warning: Event removed from user list but not found in repository for attendee update: " << title << endl;
    }
    catch (const RepositoryException& e)
    {
        throw ControllerException("Repository error while removing event from user list: " + string(e.what()));
    }
}

const vector<Event>& UserController::getUserEventList() const
{
    return userEventList;
}

void UserController::saveUserListToFile() const
{
    if (!listStorageStrategy)
        throw ControllerException("No storage format selected for user list.");

    try
    {
        listStorageStrategy->write(userEventList);
    }
    catch (const std::exception& e)
    {
        throw ControllerException("Error saving user event list: " + string(e.what()));
    }
}

void UserController::displayUserListFile() const
{
    if (!listStorageStrategy)
        throw ControllerException("No storage format selected for user list. Cannot display.");

    if (userEventList.empty())
    {
        cout << "Your event list is empty. Nothing to display." << endl;
        return;
    }
    try
    {
        listStorageStrategy->write(userEventList);
        listStorageStrategy->display();
    }
    catch (const std::exception& e)
    {
        throw ControllerException("Error displaying user event list: " + string(e.what()));
    }
}