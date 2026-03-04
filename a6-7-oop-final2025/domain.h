#pragma once
#ifndef DOMAIN_H
#define DOMAIN_H

#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <fstream>
using namespace std;

class Event;

struct DateTime
{
	int year = 0;
	int month = 0;
	int day = 0;
	int hour = 0;
	int minute = 0;

	DateTime() = default;

	DateTime(int inputYear, int inputMonth, int inputDay, int inputHour, int inputMinute);

	bool operator==(const DateTime& other) const;

	bool operator<(const DateTime& other) const;

	string toString() const;

	friend ostream& operator<<(ostream& os, const DateTime& dt);
	friend istream& operator>>(istream& is, DateTime& dt);
};

ostream& saveEventToFile(ostream& os, const Event& event);
istream& loadEventFromFile(istream& is, Event& event);

class Event
{
private:
	string title;
	string description;
	DateTime dateTime;
	int attendees;
	string link;

public:
	Event() : attendees(0) {}

	Event(string t, string desc, DateTime dt, int att, string l);

	const string& getTitle() const { return title; }
	const string& getDescription() const { return description; }
	const DateTime& getDateTime() const { return dateTime; }
	int getAttendees() const { return attendees; }
	const string& getLink() const { return link; }

	void setTitle(const string& t) { title = t; }
	void setDescription(const string& d) { description = d; }
	void setDateTime(const DateTime& dt) { dateTime = dt; }
	void setAttendees(int a) { attendees = a; }
	void setLink(const string& l) { link = l; }

	void incrementAttendees();
	void decrementAttendees();

	bool operator==(const Event& other) const;

	bool operator<(const Event& other) const;

	friend ostream& operator<<(ostream& os, const Event& event);
	friend istream& operator>>(istream& is, Event& event);
};

class EventException : public runtime_error
{
public:
	explicit EventException(const string& message) : runtime_error(message) {}
};


class ValidationException : public EventException
{
public:
	ValidationException(const string& message) : EventException(message) {}
};

class Validator
{
public:
	static void validateEvent(const string& title, const string& description, const DateTime& dt, int attendees, const string& link);

	static void validateTitle(const string& title);
	static void validateDescription(const string& description);
	static void validateDateTime(const DateTime& dt);
	static void validateAttendees(int attendees);
	static void validateLink(const string& link);
};

#endif
