#include "dish.h"

// Реализация базового класса Dish
Dish::Dish(const QString& name, double price, const QString& description)
    : name(name), price(price), description(description) {}

QString Dish::getName() const { return name; }
double Dish::getPrice() const { return price; }
QString Dish::getDescription() const { return description; }

// Реализация конкретных классов блюд
MainCourse::MainCourse(const QString& name, double price, const QString& description)
    : Dish(name, price, description) {}
QString MainCourse::getType() const { return "Основное"; }

Dessert::Dessert(const QString& name, double price, const QString& description)
    : Dish(name, price, description) {}
QString Dessert::getType() const { return "Десерт"; }

Drink::Drink(const QString& name, double price, const QString& description)
    : Dish(name, price, description) {}
QString Drink::getType() const { return "Напиток"; }

// Реализация фабрик
Dish* MainCourseFactory::createDish(const QString& name, double price, const QString& description) {
    return new MainCourse(name, price, description);
}

Dish* DessertFactory::createDish(const QString& name, double price, const QString& description) {
    return new Dessert(name, price, description);
}

Dish* DrinkFactory::createDish(const QString& name, double price, const QString& description) {
    return new Drink(name, price, description);
}
