#pragma once
#ifndef REPOSITORY_H
#define REPOSITORY_H
#include "domain.h"       
#include <vector>
#include <string>
#include <stdexcept>  

class RepositoryException : public EventException
{
public:
	explicit RepositoryException(const string& message) : EventException(message) {}
};

class DuplicateEventException : public RepositoryException
{
public:
	explicit DuplicateEventException(const string& message = "Event already exists.") : RepositoryException(message) {}
};

class EventNotFoundException : public RepositoryException
{
public:
	explicit EventNotFoundException(const std::string& message = "Event not found.") : RepositoryException(message) {}
};


class EventRepository
{
private:
	vector<Event> events;
	string filename;

	void loadFromFile();
	void saveToFile() const;

	vector<Event>::iterator findEventIterator(const string& title, const DateTime& dt);

public:
	explicit EventRepository(const string& fname = "events.txt");
	void addEvent(const Event& event);
	void removeEvent(const string& title, const DateTime& dt);
	void updateEvent(const string& oldTitle, const DateTime& oldDt, const string& newTitle, const string& newDescription, const DateTime& newDt, int newAttendees, const string& newLink);
	Event& findEvent(const string& title, const DateTime& dt);
	const vector<Event>& getAllEvents() const;

	size_t getSize() const;
	EventRepository(const EventRepository&) = delete;
	EventRepository& operator=(const EventRepository&) = delete;
};

#endif


