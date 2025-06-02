#include "table.h"

Table::Table(int number, int capacity, QObject* parent)
    : QObject(parent), number(number), capacity(capacity), occupied(false) {}

int Table::getNumber() const { return number; }
int Table::getCapacity() const { return capacity; }
bool Table::isOccupied() const { return occupied; }
QString Table::getCurrentWaiter() const { return currentWaiter; }

void Table::occupy(const QString& waiterName) {
    occupied = true;
    currentWaiter = waiterName;
    emit tableStatusChanged();
}

void Table::free() {
    occupied = false;
    currentWaiter.clear();
    emit tableStatusChanged();
}
