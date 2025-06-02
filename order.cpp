#include "order.h"

Order::Order(int tableNumber, const QString& waiterName, QObject* parent)
    : QObject(parent), tableNumber(tableNumber), waiterName(waiterName),
    orderTime(QDateTime::currentDateTime()), completed(false) {}

void Order::addDish(Dish* dish) {
    dishes.append(dish);
    emit orderChanged();
}

void Order::removeDish(int index) {
    if (index >= 0 && index < dishes.size()) {
        dishes.remove(index);
        emit orderChanged();
    }
}

void Order::completeOrder() {
    completed = true;
    emit orderChanged();
}

int Order::getTableNumber() const { return tableNumber; }
QString Order::getWaiterName() const { return waiterName; }
QDateTime Order::getOrderTime() const { return orderTime; }
bool Order::isCompleted() const { return completed; }
QVector<Dish*> Order::getDishes() const { return dishes; }

double Order::getTotalPrice() const {
    double total = 0.0;
    for (const auto& dish : dishes) {
        total += dish->getPrice();
    }
    return total;
}
