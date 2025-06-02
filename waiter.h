#ifndef WAITER_H
#define WAITER_H

#include <QObject>
#include <QString>

class Waiter : public QObject {
    Q_OBJECT
public:
    Waiter(const QString& name, int id, QObject* parent = nullptr);

    QString getName() const;
    int getId() const;
    int getOrdersServed() const;

    void incrementOrdersServed();

private:
    QString name;
    int id;
    int ordersServed;
};

#endif // WAITER_H
