#ifndef ORDER_H
#define ORDER_H

#include <QObject>
#include <QDateTime>
#include <QVector>
#include "dish.h"

class Order : public QObject {
    Q_OBJECT
public:
    Order(int tableNumber, const QString& waiterName, QObject* parent = nullptr);

    void addDish(Dish* dish);
    void removeDish(int index);
    void completeOrder();

    int getTableNumber() const;
    QString getWaiterName() const;
    QDateTime getOrderTime() const;
    bool isCompleted() const;
    QVector<Dish*> getDishes() const;
    double getTotalPrice() const;

signals:
    void orderChanged();

private:
    int tableNumber;
    QString waiterName;
    QDateTime orderTime;
    bool completed;
    QVector<Dish*> dishes;
};

#endif // ORDER_H
