#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QLocale>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_rascet, &QPushButton::clicked, this, &MainWindow::on_calculateButton_clicked);
    connect(ui->pushButton_obrt_rascet, &QPushButton::clicked, this, &MainWindow::on_reverseCalculateButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calculateButton_clicked()
{
    // Получаем введенные данные
    double ves = ui->txtves->text().toDouble();
    double distance = ui->txtdist->text().toDouble();
    double pulse = ui->txtpuls->text().toDouble();
    double time = ui->txtvremya->text().toDouble();

    // Проверяем диапазон пульса
    if (pulse < 70 || pulse > 150)
    {
        QMessageBox::warning(this, "Проблемы со здоровьем", "У вас проблемы со здоровьем. Нельзя бегать.");
        return; // Stop further processing if pulse is out of range
    }

    // Проверяем вес пользователя
    if (ves > 150)
    {
        QMessageBox::warning(this, "Проблемы со здоровьем", "У вас проблемы со здоровьем. Травмоопасно заниматься бегом.");
        return; // Stop further processing if weight is too high
    }

    // Проводим расчет калорий (формулу можно изменить)
    double calories = ves * distance * pulse * time / 117;

    // Выводим результат на экран
    QMessageBox::information(this, "Результат расчета", QString("Количество затраченных калорий: %1").arg(calories));

    // Получаем путь к рабочему столу
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    // Сохраняем результат в файл на рабочем столе
    QFile file(desktopPath + "/reportsss.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");

        // Установка локали
        QLocale locale(QLocale::Russian);
        stream.setLocale(locale);
        stream << "Результат расчета калорий:\n";
        stream << "Вес: " << ves << " кг\n";
        stream << "Дистанция: " << distance << " км\n";
        stream << "Пульс: " << pulse << " уд/мин\n";
        stream << "Время бега: " << time << " ч\n";
        stream << "Количество затраченных калорий: " << calories << "\n";
        file.close();
    }
}

void MainWindow::on_reverseCalculateButton_clicked()
{
    // Получаем целевое количество калорий
    double targetCalories = ui->txtkolkolor->text().toDouble();

    // Получаем вес пользователя
    double ves = ui->txtves->text().toDouble();

    // Проверяем диапазон пульса
    if (ui->txtpuls->text().toDouble() < 70 || ui->txtpuls->text().toDouble() > 150)
    {
        QMessageBox::warning(this, "Проблемы со здоровьем", "У вас проблемы со здоровьем. Нельзя бегать.");
        return; // Stop further processing if pulse is out of range
    }

    // Проверяем вес пользователя
    if (ves > 150)
    {
        QMessageBox::warning(this, "Проблемы со здоровьем", "У вас проблемы со здоровьем. Травмоопасно заниматься бегом.");
        return; // Stop further processing if weight is too high
    }

    // Проводим обратный расчет дистанции
    double distance = targetCalories / (ves * ui->txtpuls->text().toDouble() * ui->txtvremya->text().toDouble() / 117);

    // Выводим результат на экран
    QMessageBox::information(this, "Результат обратного расчета", QString("Дистанция для достижения целевых калорий: %1 км").arg(distance));
}
