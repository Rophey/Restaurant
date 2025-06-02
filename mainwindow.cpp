#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dish.h"
#include "order.h"
#include "table.h"
#include "waiter.h"
#include "neworderdialog.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QListWidget>
#include <QComboBox>
#include <QSpinBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Initialize models
    ordersModel = new RestaurantModel(RestaurantModel::OrdersModel, this);
    tablesModel = new RestaurantModel(RestaurantModel::TablesModel, this);
    dishesModel = new RestaurantModel(RestaurantModel::DishesModel, this);
    waitersModel = new RestaurantModel(RestaurantModel::WaitersModel, this);

    // Set models to views
    ui->ordersTableView->setModel(ordersModel);
    ui->tablesTableView->setModel(tablesModel);
    ui->menuTableView->setModel(dishesModel);
    ui->waitersTableView->setModel(waitersModel);

    // Setup initial data
    setupInitialData();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupInitialData() {
    // Add sample tables
    tablesModel->addTable(new Table(1, 4, this));
    tablesModel->addTable(new Table(2, 2, this));
    tablesModel->addTable(new Table(3, 6, this));
    tablesModel->addTable(new Table(4, 4, this));
    tablesModel->addTable(new Table(5, 8, this));

    // Add sample waiters
    waitersModel->addWaiter(new Waiter("Евгений", 101, this));
    waitersModel->addWaiter(new Waiter("Мила", 102, this));
    waitersModel->addWaiter(new Waiter("Илья", 103, this));

    // Add sample dishes using Factory Method
    DishFactory* mainCourseFactory = new MainCourseFactory();
    DishFactory* dessertFactory = new DessertFactory();
    DishFactory* drinkFactory = new DrinkFactory();

    dishesModel->addDish(mainCourseFactory->createDish("Стейк", 800, "Говядина с овощами, приготовленная на огне"));
    dishesModel->addDish(mainCourseFactory->createDish("Паста", 420, "Паста со сливочным сосусом и беконом"));
    dishesModel->addDish(dessertFactory->createDish("Чизкейк", 215, "Классический чизкейк Нью-Йорк"));
    dishesModel->addDish(dessertFactory->createDish("Тирамису", 250, "Итальянский десерт с кофейным вкусом"));
    dishesModel->addDish(drinkFactory->createDish("Апельсиновый фреш", 180, "Свежевыжатый апельсиновый сок"));
    dishesModel->addDish(drinkFactory->createDish("Кофе", 300, "Свежий сваренный кофе"));

    delete mainCourseFactory;
    delete dessertFactory;
    delete drinkFactory;
}

void MainWindow::on_btnNewOrder_clicked() {
    // Получаем список всех блюд
    QVector<Dish*> allDishes;
    for (int i = 0; i < dishesModel->rowCount(); ++i) {
        if (Dish* dish = dishesModel->getDish(i)) {
            allDishes.append(dish);
        }
    }

    NewOrderDialog dialog(allDishes, this);
    updateWaitersCombo(&dialog);

    if (dialog.exec() == QDialog::Accepted) {

        // Создаем заказ
        Order* newOrder = new Order(
            dialog.getTableNumber(),
            dialog.getWaiterName(),
            this
            );

        // Добавляем блюда
        for (Dish* dish : dialog.getSelectedDishes()) {
            newOrder->addDish(dish);
            qDebug() << "Добавлено блюдо:" << dish->getName();
        }

        // Добавляем в модель
        ordersModel->addOrder(newOrder);
        qDebug() << "Заказ добавлен. Всего заказов:" << ordersModel->rowCount();

        // Обновляем таблицу (если не обновляется автоматически)
        ui->ordersTableView->viewport()->update();
    }
}

void MainWindow::updateWaitersCombo(NewOrderDialog* dialog) {
    QComboBox* waiterCombo = dialog->findChild<QComboBox*>("waiterComboBox");
    waiterCombo->clear();
    waiterCombo->addItems(getWaiterNames());
}

void MainWindow::on_btnCompleteOrder_clicked() {
    QModelIndex currentIndex = ui->ordersTableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Warning", "Выбереите заказ для завершения");
        return;
    }

    Order* order = ordersModel->getOrder(currentIndex.row());
    if (order) {
        order->completeOrder();

        // Free the table
        for (int i = 0; i < tablesModel->rowCount(); ++i) {
            Table* table = tablesModel->getTable(i);
            if (table && table->getNumber() == order->getTableNumber()) {
                table->free();
                break;
            }
        }

        // Increment waiter's orders served count
        for (int i = 0; i < waitersModel->rowCount(); ++i) {
            Waiter* waiter = waitersModel->getWaiter(i);
            if (waiter && waiter->getName() == order->getWaiterName()) {
                waiter->incrementOrdersServed();
                break;
            }
        }
    }
}

void MainWindow::on_btnOccupyTable_clicked() {
    QModelIndex currentIndex = ui->tablesTableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Warning", "Выберите стол, чтобы занять его");
        return;
    }

    Table* table = tablesModel->getTable(currentIndex.row());
    if (table && !table->isOccupied()) {
        QStringList waiterNames = getWaiterNames();
        if (waiterNames.isEmpty()) {
            QMessageBox::warning(this, "Warning", "Нет свободных официантов");
            return;
        }

        bool ok;
        QString waiterName = QInputDialog::getItem(this, "Занятый стол",
                                                   "Выберите официанта:",
                                                   waiterNames,
                                                   0, false, &ok);
        if (ok && !waiterName.isEmpty()) {
            table->occupy(waiterName);
        }
    }
}

void MainWindow::on_btnFreeTable_clicked() {
    QModelIndex currentIndex = ui->tablesTableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Warning", "Выберите стол, чтобы его освободить");
        return;
    }

    Table* table = tablesModel->getTable(currentIndex.row());
    if (table && table->isOccupied()) {
        table->free();
    }
}

QStringList MainWindow::getWaiterNames() const {
    QStringList names;
    for (int i = 0; i < waitersModel->rowCount(); ++i) {
        Waiter* waiter = waitersModel->getWaiter(i);
        if (waiter) {
            names << waiter->getName();
        }
    }
    return names;
}

void MainWindow::on_btnAddDish_clicked() {
    // 1. Запрос типа блюда
    bool ok;
    QStringList dishTypes = {"Основное", "Десерт", "Напиток"};
    QString dishType = QInputDialog::getItem(this,
                                             "Добавить блюдо",
                                             "Выберите тип:",
                                             dishTypes,
                                             0,
                                             false,
                                             &ok);
    if (!ok || dishType.isEmpty()) return;

    // 2. Запрос названия блюда
    QString name = QInputDialog::getText(this,
                                         "Добавить блюдо",
                                         "Наименование:",
                                         QLineEdit::Normal,
                                         "",
                                         &ok);
    if (!ok || name.isEmpty()) {
        QMessageBox::warning(this, "Error", "Наименование не может быть пустым");
        return;
    }

    // 3. Запрос цены блюда
    double price = QInputDialog::getDouble(this,
                                           "Добавить блюдо",
                                           "Цена:",
                                           0.0,
                                           0.0,
                                           1000.0,
                                           2,
                                           &ok);
    if (!ok || price <= 0) {
        QMessageBox::warning(this, "Error", "Неверное значение цены");
        return;
    }

    // 4. Запрос описания блюда
    QString description = QInputDialog::getText(this,
                                                "Добавить блюдо",
                                                "Описание:",
                                                QLineEdit::Normal,
                                                "",
                                                &ok);
    if (!ok) return;

    // 5. Создание блюда с помощью Factory Method
    Dish* newDish = nullptr;
    DishFactory* factory = nullptr;

    try {
        if (dishType == "Основное") {
            factory = new MainCourseFactory();
        } else if (dishType == "Дессерт") {
            factory = new DessertFactory();
        } else if (dishType == "Напиток") {
            factory = new DrinkFactory();
        }

        if (factory) {
            newDish = factory->createDish(name, price, description);
            dishesModel->addDish(newDish);

            // Успешное добавление
            QMessageBox::information(this,
                                     "Success",
                                     QString("Блюдо '%1' успешно добавлено!").arg(name));
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Ошибка при создании блюда: %1").arg(e.what()));
        if (newDish) delete newDish;
    }

    // 6. Очистка памяти
    if (factory) delete factory;
}

void MainWindow::on_btnRemoveDish_clicked() {
    QModelIndex currentIndex = ui->menuTableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Warning", "Выберите блюда для удаления");
        return;
    }

    Dish* dish = dishesModel->getDish(currentIndex.row());
    if (dish) {
        // Check if dish is in any active order
        bool isInOrder = false;
        for (int i = 0; i < ordersModel->rowCount(); ++i) {
            Order* order = ordersModel->getOrder(i);
            if (order && !order->isCompleted()) {
                for (Dish* orderDish : order->getDishes()) {
                    if (orderDish == dish) {
                        isInOrder = true;
                        break;
                    }
                }
                if (isInOrder) break;
            }
        }

        if (isInOrder) {
            QMessageBox::warning(this, "Warning", "Нельзя удалить блюдо в активном заказе");
        } else {
            dishesModel->removeRow(currentIndex.row());
        }
    }
}

void MainWindow::on_btnAddWaiter_clicked() {
    bool ok;
    QString name = QInputDialog::getText(this, "Добавить официанта", "Имя:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;

    waitersModel->addWaiter(new Waiter(name, waitersModel->getLastId() + 1, this));
}

void MainWindow::on_btnRemoveWaiter_clicked() {
    QModelIndex currentIndex = ui->waitersTableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Warning", "Выберите официанта для удаления");
        return;
    }

    Waiter* waiter = waitersModel->getWaiter(currentIndex.row());
    if (waiter) {
        // Check if waiter is assigned to any table
        bool isAssigned = false;
        for (int i = 0; i < tablesModel->rowCount(); ++i) {
            Table* table = tablesModel->getTable(i);
            if (table && table->isOccupied() && table->getCurrentWaiter() == waiter->getName()) {
                isAssigned = true;
                break;
            }
        }

        if (isAssigned) {
            QMessageBox::warning(this, "Warning", "Нельзя удалить официанта, закреплённого за столом");
        } else {
            waitersModel->removeRow(currentIndex.row());
        }
    }
}
