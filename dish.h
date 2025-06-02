#ifndef DISH_H
#define DISH_H

#include <QString>

// Абстрактный класс блюда
class Dish {
public:
    Dish(const QString& name, double price, const QString& description);
    virtual ~Dish() = default;

    QString getName() const;
    double getPrice() const;
    QString getDescription() const;
    virtual QString getType() const = 0;

protected:
    QString name;
    double price;
    QString description;
};

// Конкретные классы блюд
class MainCourse : public Dish {
public:
    MainCourse(const QString& name, double price, const QString& description);
    QString getType() const override;
};

class Dessert : public Dish {
public:
    Dessert(const QString& name, double price, const QString& description);
    QString getType() const override;
};

class Drink : public Dish {
public:
    Drink(const QString& name, double price, const QString& description);
    QString getType() const override;
};

// Фабрика блюд (Factory Method)
class DishFactory {
public:
    virtual Dish* createDish(const QString& name, double price, const QString& description) = 0;
    virtual ~DishFactory() = default;
};

class MainCourseFactory : public DishFactory {
public:
    Dish* createDish(const QString& name, double price, const QString& description) override;
};

class DessertFactory : public DishFactory {
public:
    Dish* createDish(const QString& name, double price, const QString& description) override;
};

class DrinkFactory : public DishFactory {
public:
    Dish* createDish(const QString& name, double price, const QString& description) override;
};

#endif // DISH_H
