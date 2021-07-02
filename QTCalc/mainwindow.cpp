#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegularExpression>

double calculatedValue = 0.0;
bool divTrigger = false;
bool mulTrigger = false;
bool addTrigger = false;
bool subTrigger = false;
bool perTrigger = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //initalize everything
    ui->NumDisplay->setText(QString::number(calculatedValue));
    QPushButton *numButtons[10];

    for (int i = 0; i < 10; i++) {
        QString bName = "Button" + QString::number(i);

        //find the widget by name
        numButtons[i] = MainWindow::findChild<QPushButton *>(bName);
        //call numpressed slot when button is released
        connect(numButtons[i], SIGNAL(released()), this,
                SLOT(numPressed()));
    }

    //connect operators
    connect(ui->AddButtton, SIGNAL(released()), this,
            SLOT(opPressed()));
    connect(ui->SubtractButton, SIGNAL(released()), this,
            SLOT(opPressed()));
    connect(ui->DivideButton, SIGNAL(released()), this,
            SLOT(opPressed()));
    connect(ui->MultiplyButton, SIGNAL(released()), this,
            SLOT(opPressed()));

    //connect eq button
    connect(ui->EqualsButton, SIGNAL(released()), this,
            SLOT(eqPressed()));

    //connect modifiers
    connect (ui->PosNegButton, SIGNAL(released()), this,
             SLOT(signPressed()));
    connect(ui->ClearButton, SIGNAL(released()), this,
            SLOT(clearPressed()));
    connect (ui->PercentButton, SIGNAL(released()), this,
             SLOT(perPressed()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

//a number is pressed
void MainWindow::numPressed() {
    QPushButton *button = (QPushButton *)sender();
    QString bVal = button->text();
    QString displayVal = ui->NumDisplay->text();
    if ((displayVal.toDouble() == 0 || displayVal.toDouble() == 0.0)) {
        ui->NumDisplay->setText(bVal);
    }
    else {
        QString newVal = displayVal + bVal;
        double dblNewVal = newVal.toDouble();
        ui->NumDisplay->setText(QString::number(dblNewVal, 'g', 16));
    }
}

//a mathematial buton is pressed
void MainWindow::opPressed() {
    divTrigger = false;
    mulTrigger = false;
    addTrigger = false;
    subTrigger = false;
    perTrigger = false;

    QString displayVal = ui->NumDisplay->text();
    calculatedValue = displayVal.toDouble();
    QPushButton *button = (QPushButton *)sender();
    QString bVal = button->text();

    //figure out which operation was pressed
    if (QString::compare(bVal, "/", Qt::CaseInsensitive) == 0) {
        divTrigger = true;
    } else if (QString::compare(bVal, "X", Qt::CaseInsensitive) == 0) {
        mulTrigger = true;
    } else if (QString::compare(bVal, "+", Qt::CaseInsensitive) == 0) {
        addTrigger = true;
    } else if (QString::compare(bVal, "-", Qt::CaseInsensitive) == 0) {
        subTrigger = true;
    } else if (QString::compare(bVal, "%", Qt::CaseInsensitive) == 0) {
        perTrigger = true;
    }
    ui->NumDisplay->setText("");
}

//equals button pressed
void MainWindow::eqPressed() {
    double sol = 0.0;
    QString displayVal = ui->NumDisplay->text();
    double dblDispVal = displayVal.toDouble();

    //figure out which operation needs to be done and do it
    if (divTrigger || mulTrigger || addTrigger || subTrigger || perTrigger) {
        if (divTrigger) {
            sol = calculatedValue / dblDispVal;
        } else if (mulTrigger) {
            sol = calculatedValue * dblDispVal;
        } else if (addTrigger) {
            sol = calculatedValue + dblDispVal;
        } else if (subTrigger) {
            sol = calculatedValue - dblDispVal;
        }
    }
    ui->NumDisplay->setText(QString::number(sol));
}

//change sign button pressed
void MainWindow::signPressed() {
    QString val = ui->NumDisplay->text();
    QString Exp = "[-]?[0-9.]*";
    QRegularExpression re(QRegularExpression::anchoredPattern(Exp));
    QRegularExpressionMatch match = re.match(val);

    if (match.hasMatch()) {
        double dblDispVal = val.toDouble();
        double dblDispValSign = -1 * dblDispVal;
        ui->NumDisplay->setText(QString::number(dblDispValSign));
    }

}

//clear button pressed
void MainWindow::clearPressed() {
    calculatedValue = 0;
    ui->NumDisplay->setText("");
}


//percentage button pressed
void MainWindow::perPressed() {
    QString val = ui->NumDisplay->text();
    double numVal = val.toDouble();
    double percentage = numVal / 100;
    ui->NumDisplay->setText(QString::number(percentage));
}
