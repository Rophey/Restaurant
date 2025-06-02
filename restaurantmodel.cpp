#include "restaurantmodel.h"

RestaurantModel::RestaurantModel(ModelType type, QObject* parent)
    : QAbstractTableModel(parent), modelType(type) {}

int RestaurantModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    switch(modelType) {
    case DishesModel: return dishes.size();
    case OrdersModel: return orders.size();
    case TablesModel: return tables.size();
    case WaitersModel: return waiters.size();
    }
    return 0;
}

int RestaurantModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    switch(modelType) {
    case DishesModel: return 4;
    case OrdersModel: return 5;
    case TablesModel: return 4;
    case WaitersModel: return 3;
    }
    return 0;
}

QVariant RestaurantModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    switch(modelType) {
    case DishesModel:
        if (index.row() >= dishes.size()) return QVariant();
        switch(index.column()) {
        case 0: return dishes[index.row()]->getName();
        case 1: return dishes[index.row()]->getType();
        case 2: return QString::number(dishes[index.row()]->getPrice(), 'f', 2);
        case 3: return dishes[index.row()]->getDescription();
        default: return QVariant();
        }
    case OrdersModel:
        if (index.row() >= orders.size()) return QVariant();
        switch(index.column()) {
        case 0: return orders[index.row()]->getTableNumber();
        case 1: return orders[index.row()]->getWaiterName();
        case 2: return orders[index.row()]->getOrderTime().toString("dd.MM.yyyy HH:mm");
        case 3: return orders[index.row()]->isCompleted() ? "Готов" : "В процессе";
        case 4: return QString::number(orders[index.row()]->getTotalPrice(), 'f', 2);
        default: return QVariant();
        }
    case TablesModel:
        if (index.row() >= tables.size()) return QVariant();
        switch(index.column()) {
        case 0: return tables[index.row()]->getNumber();
        case 1: return tables[index.row()]->getCapacity();
        case 2: return tables[index.row()]->isOccupied() ? "Занят" : "Свободен";
        case 3: return tables[index.row()]->getCurrentWaiter();
        default: return QVariant();
        }
    case WaitersModel:
        if (index.row() >= waiters.size()) return QVariant();
        switch(index.column()) {
        case 0: return waiters[index.row()]->getName();
        case 1: return waiters[index.row()]->getId();
        case 2: return waiters[index.row()]->getOrdersServed();
        default: return QVariant();
        }
    }
    return QVariant();
}

QVariant RestaurantModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    switch(modelType) {
    case DishesModel:
        switch(section) {
        case 0: return "Наименование";
        case 1: return "Тип";
        case 2: return "Цена";
        case 3: return "Описание";
        default: return QVariant();
        }
    case OrdersModel:
        switch(section) {
        case 0: return "Стол";
        case 1: return "Официант";
        case 2: return "Время";
        case 3: return "Статус";
        case 4: return "Итого";
        default: return QVariant();
        }
    case TablesModel:
        switch(section) {
        case 0: return "Номер";
        case 1: return "Количество мест";
        case 2: return "Статус";
        case 3: return "Официант";
        default: return QVariant();
        }
    case WaitersModel:
        switch(section) {
        case 0: return "Имя";
        case 1: return "ID";
        case 2: return "Заказов выполнено";
        default: return QVariant();
        }
    }
    return QVariant();
}

void RestaurantModel::addOrder(Order* order) {
    beginInsertRows(QModelIndex(), orders.size(), orders.size());
    orders.append(order);
    endInsertRows();
    connect(order, &Order::orderChanged, this, [this]() {
        QModelIndex topLeft = index(0, 0);
        QModelIndex bottomRight = index(rowCount() - 1, columnCount() - 1);
        emit dataChanged(topLeft, bottomRight);
    });
}

void RestaurantModel::addTable(Table* table) {
    beginInsertRows(QModelIndex(), tables.size(), tables.size());
    tables.append(table);
    endInsertRows();
    connect(table, &Table::tableStatusChanged, this, [this]() {
        QModelIndex topLeft = index(0, 0);
        QModelIndex bottomRight = index(rowCount() - 1, columnCount() - 1);
        emit dataChanged(topLeft, bottomRight);
    });
}

void RestaurantModel::addWaiter(Waiter* waiter) {
    beginInsertRows(QModelIndex(), waiters.size(), waiters.size());
    waiters.append(waiter);
    endInsertRows();
}

void RestaurantModel::addDish(Dish* dish) {
    beginInsertRows(QModelIndex(), dishes.size(), dishes.size());
    dishes.append(dish);
    endInsertRows();
}

Order* RestaurantModel::getOrder(int index) const {
    if (index >= 0 && index < orders.size())
        return orders[index];
    return nullptr;
}

Table* RestaurantModel::getTable(int index) const {
    if (index >= 0 && index < tables.size())
        return tables[index];
    return nullptr;
}

Waiter* RestaurantModel::getWaiter(int index) const {
    if (index >= 0 && index < waiters.size())
        return waiters[index];
    return nullptr;
}

Dish* RestaurantModel::getDish(int index) const {
    if (index >= 0 && index < dishes.size())
        return dishes[index];
    return nullptr;
}
int RestaurantModel::getLastId() {
    return getWaiter(waiters.length()-1)->getId();
}
bool RestaurantModel::removeRow(int row, const QModelIndex &parent) {
    Q_UNUSED(parent);

    if (row < 0 || row >= rowCount()) return false;

    beginRemoveRows(QModelIndex(), row, row);
    switch(modelType) {
    case DishesModel: dishes.remove(row); break;
    case OrdersModel: orders.remove(row); break;
    case TablesModel: tables.remove(row); break;
    case WaitersModel: waiters.remove(row); break;
    }
    endRemoveRows();
    return true;
}
