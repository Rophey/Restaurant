#include "waiter.h"

Waiter::Waiter(const QString& name, int id, QObject* parent)
    : QObject(parent), name(name), id(id), ordersServed(0) {}

QString Waiter::getName() const { return name; }
int Waiter::getId() const { return id; }
int Waiter::getOrdersServed() const { return ordersServed; }

void Waiter::incrementOrdersServed() {
    ordersServed++;
}
