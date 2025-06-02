#include "neworderdialog.h"
#include "ui_neworderdialog.h"
#include <QTableWidgetItem>
#include <QMessageBox>

NewOrderDialog::NewOrderDialog(const QVector<Dish*>& dishes, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewOrderDialog),
    availableDishes(dishes)
{
    ui->setupUi(this);
    setupTables();
}

NewOrderDialog::~NewOrderDialog() {
    delete ui;
}

void NewOrderDialog::setupTables() {
    // Очищаем таблицы
    ui->availableDishesTable->clear();
    ui->selectedDishesTable->clear();

    // Настраиваем колонки
    QStringList headers = {"Название", "Тип", "Цена"};
    ui->availableDishesTable->setColumnCount(3);
    ui->availableDishesTable->setHorizontalHeaderLabels(headers);
    ui->selectedDishesTable->setColumnCount(3);
    ui->selectedDishesTable->setHorizontalHeaderLabels(headers);

    // Заполняем доступные блюда
    for (int i = 0; i < availableDishes.size(); ++i) {
        Dish* dish = availableDishes[i];
        ui->availableDishesTable->insertRow(i);
        ui->availableDishesTable->setItem(i, 0, new QTableWidgetItem(dish->getName()));
        ui->availableDishesTable->setItem(i, 1, new QTableWidgetItem(dish->getType()));
        ui->availableDishesTable->setItem(i, 2, new QTableWidgetItem(QString::number(dish->getPrice(), 'f', 2)));
    }
}
void NewOrderDialog::on_addDishButton_clicked() {
    if (!ui->availableDishesTable || !ui->selectedDishesTable) return;

    int currentRow = ui->availableDishesTable->currentRow();
    if (currentRow >= 0 && currentRow < availableDishes.size()) {
        if (Dish* selectedDish = availableDishes[currentRow]) {
            selectedDishes.append(selectedDish);

            int row = ui->selectedDishesTable->rowCount();
            ui->selectedDishesTable->insertRow(row);
            ui->selectedDishesTable->setItem(row, 0, new QTableWidgetItem(selectedDish->getName()));
            ui->selectedDishesTable->setItem(row, 1, new QTableWidgetItem(selectedDish->getType()));
            ui->selectedDishesTable->setItem(row, 2, new QTableWidgetItem(QString::number(selectedDish->getPrice(), 'f', 2)));
        }
    }
}

void NewOrderDialog::on_removeDishButton_clicked() {
    if (!ui->selectedDishesTable) return;

    int currentRow = ui->selectedDishesTable->currentRow();
    if (currentRow >= 0 && currentRow < selectedDishes.size()) {
        selectedDishes.remove(currentRow);
        ui->selectedDishesTable->removeRow(currentRow);
    }
}

void NewOrderDialog::on_btnCreateOrder_clicked() {
    // Проверяем, что выбран хотя бы один элемент
    if (selectedDishes.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите хотя бы одно блюдо!");
        return;
    }

    // Проверяем, что выбран столик и официант
    if (ui->tableNumberSpinBox->value() == 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите номер столика!");
        return;
    }

    if (ui->waiterComboBox->currentText().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите официанта!");
        return;
    }

    // Если все проверки пройдены, закрываем диалог с Accepted
    this->accept();
}

int NewOrderDialog::getTableNumber() const {
    return ui->tableNumberSpinBox ? ui->tableNumberSpinBox->value() : 0;
}

QString NewOrderDialog::getWaiterName() const {
    return ui->waiterComboBox ? ui->waiterComboBox->currentText() : QString();
}

QVector<Dish*> NewOrderDialog::getSelectedDishes() const {
    return selectedDishes;
}
