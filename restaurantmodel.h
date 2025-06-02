#ifndef RESTAURANTMODEL_H
#define RESTAURANTMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include "order.h"
#include "table.h"
#include "waiter.h"

class RestaurantModel : public QAbstractTableModel {
    Q_OBJECT
public:
    enum ModelType {
        DishesModel,
        OrdersModel,
        TablesModel,
        WaitersModel
    };

    RestaurantModel(ModelType type, QObject* parent = nullptr);

    QVector<Dish*> getAllDishes() const { return dishes; }
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void addOrder(Order* order);
    void addTable(Table* table);
    void addWaiter(Waiter* waiter);
    void addDish(Dish* dish);

    Order* getOrder(int index) const;
    Table* getTable(int index) const;
    Waiter* getWaiter(int index) const;
    Dish* getDish(int index) const;
    int getLastId ();

private:
    ModelType modelType;
    QVector<Order*> orders;
    QVector<Table*> tables;
    QVector<Waiter*> waiters;
    QVector<Dish*> dishes;
};

#endif // RESTAURANTMODEL_H
