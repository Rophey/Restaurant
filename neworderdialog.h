#ifndef NEWORDERDIALOG_H
#define NEWORDERDIALOG_H

#include <QDialog>
#include <QVector>
#include "dish.h"

namespace Ui {
class NewOrderDialog;
}

class NewOrderDialog : public QDialog {
    Q_OBJECT

public:
    explicit NewOrderDialog(const QVector<Dish*>& dishes, QWidget *parent = nullptr);
    ~NewOrderDialog();

    int getTableNumber() const;
    QString getWaiterName() const;
    QVector<Dish*> getSelectedDishes() const;

private slots:
    void on_addDishButton_clicked();
    void on_removeDishButton_clicked();
    void on_btnCreateOrder_clicked();

private:
    Ui::NewOrderDialog *ui;
    const QVector<Dish*>& availableDishes;
    QVector<Dish*> selectedDishes;

    void setupTables();
};

#endif // NEWORDERDIALOG_H
