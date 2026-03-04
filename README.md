# Event Management System (C++)

**A professional C++ application designed to manage local events, demonstrating a transition from low-level memory management to modern software architectural patterns.**

This project was developed as part of the Object-Oriented Programming curriculum, focusing on layered architecture, code reusability, and robust error handling.

## 🌟 Key Technical Features

* **Layered Architecture:** Strict separation of concerns across four distinct layers:
    * **Domain:** Defines the `Event` entity with attributes like title, description, date/time, attendance count, and source link.
    * **Repository:** Manages data persistence and CRUD operations.
    * **Service/Controller:** Implements business logic and coordinates data flow.
    * **UI:** Console-based interface for administrative and user modes.
* **Memory Management Evolution:** * Initial versions featured a custom-built templated `DynamicVector` to manage memory manually.
    * Final version utilizes **STL vectors** and **smart pointers** for enhanced safety and efficiency.
* **Modern C++ & STL:** Extensive use of STL algorithms (e.g., `std::copy_if`, `std::count_if`) and range-based for loops to replace traditional iterations.
* **Data Persistence & Export:** * Permanent storage in text files (`events.txt`) using `iostream` operator overloading.
    * Polymorphic export system allowing users to save their agendas in **CSV** or **HTML** formats.
* **Robust Exception Handling:** Implementation of custom exception hierarchies to handle validation errors and repository conflicts gracefully.

## 🚀 Functionalities

### Administrator Mode
* Full CRUD (Create, Read, Update, Delete) operations for the event database.
* Data validation ensures every event has a valid date, positive capacity, and reachable link.

### User Mode
* **Discovery:** Browse events filtered by month or view all events ordered chronologically.
* **Agenda Management:** Add events to a personal "interest list" and automatically open event links in the default web browser.
* **Persistence:** Export the personal agenda to CSV (Excel) or HTML (Browser) for external viewing.

## 💻 Tech Stack
* **Language:** C++ (Standard 14/17)
* **Tools:** Visual Studio IDE
* **Concepts:** Encapsulation, Inheritance, Polymorphism, Templates, STL, File I/O.

---
*Developed as a comprehensive study of Software Engineering principles in C++.*
