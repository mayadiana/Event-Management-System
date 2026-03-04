#include "domain.h"
#include <iostream>
using namespace std;
#include <string>
#include <sstream>

DateTime::DateTime(int inputYear, int inputMonth, int inputDay, int inputHour, int inputMinute)
{
	if (inputYear < 0 || inputMonth < 1 || inputMonth > 12 || inputDay < 1 || inputDay > 31 || inputHour < 0 || inputHour > 23 || inputMinute < 0 || inputMinute > 59)
	{
		throw ValidationException("Invalid date or time");
	}
	year = inputYear;
	month = inputMonth;
	day = inputDay;
	hour = inputHour;
	minute = inputMinute;
}

bool DateTime::operator==(const DateTime& other) const
{
	return year == other.year && month == other.month && day == other.day && hour == other.hour && minute == other.minute;
}

bool DateTime::operator<(const DateTime& other) const
{
	if (year != other.year) return year < other.year;
	if (month != other.month) return month < other.month;
	if (day != other.day) return day < other.day;
	if (hour != other.hour) return hour < other.hour;
	return minute < other.minute;
}

string DateTime::toString() const
{
	string result = to_string(year);
	result += "-";

	if (month < 10)
		result += "0";
	result += to_string(month);

	result += "-";
	if (day < 10)
		result += "0";
	result += to_string(day);

	result += " ";
	if (hour < 10)
		result += "0";
	result += to_string(hour);

	result += ":";
	if (minute < 10)
		result += "0";
	result += to_string(minute);

	return result;
}

ostream& operator<<(ostream& os, const DateTime& dt)
{
	os << dt.year << " " << dt.month << " " << dt.day << " " << dt.hour << " " << dt.minute;
	return os;
}

istream& operator>>(istream& is, DateTime& dt)
{
	is >> dt.year >> dt.month >> dt.day >> dt.hour >> dt.minute;
	return is;
}

Event::Event(string t, string desc, DateTime dt, int att, string l)
{
	this->title = move(t);
	this->description = move(desc);
	this->dateTime = dt;
	this->attendees = att;
	this->link = move(l);
}

void Event::incrementAttendees()
{
	attendees++;
}

void Event::decrementAttendees()
{
	if (attendees > 0)
		attendees--;
}

bool Event::operator==(const Event& other) const
{
	return title == other.title && dateTime == other.dateTime;
}

bool Event::operator<(const Event& other) const
{
	return dateTime < other.dateTime;
}

ostream& operator<<(ostream& os, const Event& event)
{
	os << "Title: " << event.title << "\n" << "Description: " << event.description << "\n" << "Date/Time: " << event.dateTime.toString() << "\n" << "Attendees: " << event.attendees << "\n" << "Link: " << event.link;
	return os;
}

ostream& saveEventToFile(ostream& os, const Event& event)
{
	os << event.getTitle() << "\n" << event.getDescription() << "\n" << event.getDateTime() << "\n" << event.getAttendees() << "\n" << event.getLink() << "\n";
	return os;
}

istream& loadEventFromFile(istream& is, Event& event)
{
	string title, description, link;
	DateTime dt;
	int attendees;

	getline(is >> ws, title); //ws consumes leading whitespace
	if (title.empty() && is.eof()) return is;

	getline(is, description);
	is >> dt;
	is >> attendees;
	is.ignore(numeric_limits<streamsize>::max(), '\n'); // Consume rest of attendees line
	getline(is, link);

	if (is)
	{
		event.setTitle(title);
		event.setDescription(description);
		event.setDateTime(dt);
		event.setAttendees(attendees);
		event.setLink(link);
	}
	return is;
}

istream& operator>>(istream& is, Event& event)
{
	return loadEventFromFile(is, event);
}

void Validator::validateEvent(const string& title, const string& description, const DateTime& dt, int attendees, const string& link)
{
	validateTitle(title);
	validateDescription(description);
	validateDateTime(dt);
	validateAttendees(attendees);
	validateLink(link);
}

void Validator::validateTitle(const string& title)
{
	if (title.empty())
		throw ValidationException("Event title cannot be empty.");
}

void Validator::validateDescription(const string& description)
{
	// if (description.empty()) 
	//     throw ValidationException("Event description cannot be empty.");
}

void Validator::validateDateTime(const DateTime& dt)
{
	if (dt.year <= 0)
		throw ValidationException("Invalid year.");

	if (dt.month < 1 || dt.month > 12)
		throw ValidationException("Invalid month (must be 1-12).");

	int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	if ((dt.year % 4 == 0 && dt.year % 100 != 0) || (dt.year % 400 == 0))
		daysInMonth[2] = 29;

	if (dt.day < 1 || dt.day > daysInMonth[dt.month])
	{
		stringstream ss;
		ss << "Invalid day (" << dt.day << ") for month " << dt.month << " in year " << dt.year << "/n Max days: " << daysInMonth[dt.month];
		throw ValidationException(ss.str());
	}

	if (dt.hour < 0 || dt.hour > 23)
		throw ValidationException("Invalid hour (must be 0-23).");

	if (dt.minute < 0 || dt.minute > 59)
		throw ValidationException("Invalid minute (must be 0-59).");
}

void Validator::validateAttendees(int attendees)
{
	if (attendees < 0)
		throw ValidationException("Number of attendees cannot be negative.");
}

void Validator::validateLink(const string& link)
{
	if (link.empty())
		throw ValidationException("Event link cannot be empty.");

	if (link.rfind("http://", 0) != 0 && link.rfind("https://", 0) != 0)
	{
		// rfind checks if the string *starts with* "http://" or "https://"
		if (link.length() < 8 && (link.rfind("http://", 0) == 0 || link.rfind("https://", 0) == 0))
			throw ValidationException("Link seems too short for it tot be a valid URL.");
	}
}