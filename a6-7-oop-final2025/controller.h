#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "repository.h" 
#include "domain.h"     
#include <string>
#include <vector>      
#include <stdexcept>   
#include <algorithm>

class ControllerException : public EventException
{
public:
	explicit ControllerException(const string& message) : EventException(message) {}
};

class AdminController
{
private:
	EventRepository& repo;

public:
	explicit AdminController(EventRepository& r) : repo(r) {}
	void addEvent(const string& title, const string& description, const DateTime& dt, int attendees, const string& link);
	void removeEvent(const string& title, const DateTime& dt);
	void updateEvent(const string& oldTitle, const DateTime& oldDt, const string& newTitle, const string& newDescription, const DateTime& newDt, int newAttendees, const string& newLink);
	const vector<Event>& getAllEvents() const;
	AdminController(const AdminController&) = delete;
	AdminController& operator=(const AdminController&) = delete;
};

class UserEventListStorage
{
public:
	virtual ~UserEventListStorage() = default;
	virtual void write(const vector<Event>& events) const = 0;
	virtual string getFilePath() const = 0;
	virtual void display() const = 0;
};

class UserEventListCSVStorage : public UserEventListStorage
{
private:
	string filename;
public:
	explicit UserEventListCSVStorage(const string& fname = "user_events.csv") : filename(fname) {}
	void write(const vector<Event>& events) const override;
	string getFilePath() const override { return filename; }
	void display() const override;
};

class UserEventListHTMLStorage : public UserEventListStorage
{
private:
	string filename;
public:
	explicit UserEventListHTMLStorage(const string& fname = "user_events.html") : filename(fname) {}
	void write(const vector<Event>& events) const override;
	string getFilePath() const override { return filename; }
	void display() const override;
};

class UserController
{
private:
	EventRepository& repo;
	vector<Event> userEventList;
	UserEventListStorage* listStorageStrategy; //pointer to storage array

public:
	explicit UserController(EventRepository& r);
	~UserController();

	void setUserListStorageStrategy(UserEventListStorage* strategy);

	vector<Event> getEventsByMonth(int month);
	void addEventToUserList(const Event& eventToAdd);
	void removeEventFromUserList(const string& title, const DateTime& dt);
	const vector<Event>& getUserEventList() const;

	void saveUserListToFile() const;
	void displayUserListFile() const;

	UserController(const UserController&) = delete;
	UserController& operator=(const UserController&) = delete;
};

#endif 