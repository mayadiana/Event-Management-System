#include "repository.h"
#include <iostream>
#include <fstream>
#include <algorithm>

void EventRepository::loadFromFile()
{
	ifstream file(filename);
	if (!file.is_open())
		return;

	Event event;
	while (file >> event)
	{
		events.push_back(event);
	}

	if (file.bad())
		cerr << "Error: I/O error while reading from event data file '" << filename << "'." << endl;

	file.close();
}

void EventRepository::saveToFile() const
{
	ofstream file(filename);

	if (!file.is_open())
		throw RepositoryException("Error: Could not open event data file '" + filename + "' for saving.");

	for (const auto& event : events)
		saveEventToFile(file, event);

	if (file.bad())
		throw RepositoryException("Error: I/O error while writing to event data file '" + filename + "'.");

	file.close();
}

vector<Event>::iterator EventRepository::findEventIterator(const string& title, const DateTime& dt)
{
	return find_if(events.begin(), events.end(), [&](const Event& e) //find_if + lambda function
		{
			return e.getTitle() == title && e.getDateTime() == dt;
		});
}

EventRepository::EventRepository(const string& fname) : filename(fname)
{
	loadFromFile();

	if (events.empty() && fname == "events.txt")
	{
		addEvent(Event("C++ Workshop", "Learn advanced C++ features", DateTime(2024, 5, 10, 18, 0), 15, "https://example.com/cpp_workshop"));
		addEvent(Event("Python for Beginners", "Introductory Python course", DateTime(2024, 5, 12, 10, 0), 25, "https://example.com/python_intro"));
		addEvent(Event("Web Technologies", "Discuss latest web technologies", DateTime(2024, 5, 15, 19, 30), 40, "https://example.com/web_meetup"));
		addEvent(Event("Data Science Seminar", "Exploring algorithms", DateTime(2024, 6, 1, 14, 0), 30, "https://example.com/ds_seminar"));
		addEvent(Event("Game Developing", "Start developing a game in 48h", DateTime(2024, 6, 7, 18, 0), 50, "https://example.com/game_jam"));
		addEvent(Event("Art Exhibition Opening", "Local artists showcase", DateTime(2024, 6, 10, 17, 0), 60, "https://example.com/art_exhibit"));
		addEvent(Event("Live Music Night", "Live performances of local artists", DateTime(2024, 6, 15, 20, 0), 75, "https://example.com/music_night"));
		addEvent(Event("Book Club Meeting", "Discussing 'Dune'", DateTime(2024, 6, 20, 19, 0), 10, "https://example.com/book_club"));
		addEvent(Event("Outdoor Yoga", "Morning yoga session in the park", DateTime(2024, 7, 5, 8, 0), 22, "https://example.com/yoga_park"));
		addEvent(Event("Tech Conference", "Annual tech industry conference", DateTime(2024, 7, 18, 9, 0), 150, "https://example.com/tech_conf"));
		addEvent(Event("Cooking Class", "Italian Cuisine", DateTime(2024, 7, 22, 18, 30), 12, "https://example.com/cooking_italian"));
	}

	saveToFile();
}

void EventRepository::addEvent(const Event& event)
{
	auto it = findEventIterator(event.getTitle(), event.getDateTime());
	if (it != events.end())
		throw DuplicateEventException("Cannot add event: An event with the same title and date/time already exists.");
	events.push_back(event);
	saveToFile();
}

void EventRepository::removeEvent(const string& title, const DateTime& dt)
{
	auto it = findEventIterator(title, dt);
	if (it == events.end())
		throw EventNotFoundException("Cannot remove event: Event not found.");
	events.erase(it);
	saveToFile();
}

void EventRepository::updateEvent(const string& oldTitle, const DateTime& oldDt, const string& newTitle, const string& newDescription, const DateTime& newDt, int newAttendees, const string& newLink)
{
	auto it = findEventIterator(oldTitle, oldDt);
	if (it == events.end())
		throw EventNotFoundException("Cannot update event: Original event not found.");

	if (!(newTitle == oldTitle && newDt == oldDt))
	{
		auto conflictIt = find_if(events.begin(), events.end(), [&](const Event& e)
			{
				return (&e != &(*it)) && e.getTitle() == newTitle && e.getDateTime() == newDt;
			});

		if (conflictIt != events.end())
			throw DuplicateEventException("Cannot update event: Another event with the new title and date/time already exists.");
	}

	it->setTitle(newTitle);
	it->setDescription(newDescription);
	it->setDateTime(newDt);
	it->setAttendees(newAttendees);
	it->setLink(newLink);
	saveToFile();
}

Event& EventRepository::findEvent(const string& title, const DateTime& dt)
{
	auto it = findEventIterator(title, dt);
	if (it == events.end())
		throw EventNotFoundException("Event not found.");
	return *it;
}

const vector<Event>& EventRepository::getAllEvents() const
{
	return events;
}

size_t EventRepository::getSize() const
{
	return events.size();
}