#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "restaurantmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class NewOrderDialog;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnNewOrder_clicked();
    void on_btnCompleteOrder_clicked();
    void on_btnOccupyTable_clicked();
    void on_btnFreeTable_clicked();
    void on_btnAddDish_clicked();
    void on_btnRemoveDish_clicked();
    void on_btnAddWaiter_clicked();
    void on_btnRemoveWaiter_clicked();

private:
    Ui::MainWindow* ui;
    RestaurantModel* ordersModel;
    RestaurantModel* tablesModel;
    RestaurantModel* dishesModel;
    RestaurantModel* waitersModel;

    void setupInitialData();
    void updateWaitersCombo(NewOrderDialog* dialog);
    QStringList getWaiterNames() const;
};

#endif // MAINWINDOW_H
