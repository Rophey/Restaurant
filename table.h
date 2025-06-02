#ifndef TABLE_H
#define TABLE_H

#include <QObject>

class Table : public QObject {
    Q_OBJECT
public:
    Table(int number, int capacity, QObject* parent = nullptr);

    int getNumber() const;
    int getCapacity() const;
    bool isOccupied() const;
    QString getCurrentWaiter() const;

    void occupy(const QString& waiterName);
    void free();

signals:
    void tableStatusChanged();

private:
    int number;
    int capacity;
    bool occupied;
    QString currentWaiter;
};

#endif // TABLE_H
