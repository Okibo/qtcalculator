#include "calculator.h"
#include "./ui_calculator.h"

double calcVal = 0.0;
double solution = 0.0;
double memoryVal = 0;
bool memoryTriggered = false;
Calculator::MATH_TRIGGERS TRIGGER = Calculator::MATH_TRIGGERS::NONE;

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);
    ui->display->setText(QString::number(calcVal));

    QPushButton *numButtons[10];
    for(int i=0; i<10; i++){
        QString butName = "button" + QString::number(i);
        numButtons[i] = Calculator::findChild<QPushButton *>(butName);
        connect(numButtons[i], SIGNAL(released()), this, SLOT(NumPressed()));
    }

    connect(ui->buttonAdd, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->buttonDivide, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->ButtonSubstract, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->buttonMultiply, SIGNAL(released()), this, SLOT(MathButtonPressed()));

    connect(ui->buttonChangeSign, SIGNAL(released()), this, SLOT(ChangeNumberSign()));

    connect(ui->buttonEquals, SIGNAL(released()), this, SLOT(EqualButton()));

    connect(ui->buttonClear, SIGNAL(released()), this, SLOT(ClearDisplay()));

    connect(ui->buttonRetriveMem, SIGNAL(released()), this, SLOT(CalcMemory()));
    connect(ui->buttonEraseMem, SIGNAL(released()), this, SLOT(CalcMemory()));
    connect(ui->buttoneAddMem, SIGNAL(released()), this, SLOT(CalcMemory()));
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::NumPressed(){
    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();
    QString displayVal = ui->display->text();
    if((displayVal.toDouble() == 0) || (displayVal.toDouble() == 0.0)){
        ui->display->setText(butVal);
    }else{
        QString newVal = displayVal + butVal;
        double dblNewVal = newVal.toDouble();
        ui->display->setText(QString::number(dblNewVal, 'g', 16));
    }
}

void Calculator::MathButtonPressed(){
    TRIGGER = Calculator::MATH_TRIGGERS::NONE;

    QString displayVal = ui->display->text();
    calcVal = displayVal.toDouble();
    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();
    if(QString::compare(butVal, "/", Qt::CaseInsensitive) == 0){
        TRIGGER = Calculator::MATH_TRIGGERS::DIVIDE;
    }else if(QString::compare(butVal, "*", Qt::CaseInsensitive) == 0){
        TRIGGER = Calculator::MATH_TRIGGERS::MULTIPLY;
    }else if(QString::compare(butVal, "+", Qt::CaseInsensitive) == 0){
        TRIGGER = Calculator::MATH_TRIGGERS::ADD;
    }else if(QString::compare(butVal, "-", Qt::CaseInsensitive) == 0){
        TRIGGER = Calculator::MATH_TRIGGERS::SUBSTRACT;
    }
    ui->display->setText("");
}

void Calculator::EqualButton(){
    QString displayVal = ui->display->text();
    double dblDisplayVal = displayVal.toDouble();
    if(TRIGGER != Calculator::MATH_TRIGGERS::NONE){
        if(TRIGGER == Calculator::MATH_TRIGGERS::ADD){
            solution = calcVal + dblDisplayVal;
        }else if(TRIGGER == Calculator::MATH_TRIGGERS::DIVIDE){
            solution = calcVal / dblDisplayVal;
        }else if(TRIGGER == Calculator::MATH_TRIGGERS::MULTIPLY){
            solution = calcVal * dblDisplayVal;
        }else if(TRIGGER == Calculator::MATH_TRIGGERS::SUBSTRACT){
            solution = calcVal - dblDisplayVal;
        }
    }
    TRIGGER = Calculator::MATH_TRIGGERS::NONE;
    ui->display->setText(QString::number(solution));
}

void Calculator::ChangeNumberSign(){
    QString displayVal = ui->display->text();
    double dblDisplayVal = displayVal.toDouble();
    dblDisplayVal *= -1;
    ui->display->setText(QString::number(dblDisplayVal));
}

void Calculator::ClearDisplay(){
    calcVal = 0;
    solution = 0;
    TRIGGER = Calculator::MATH_TRIGGERS::NONE;
    ui->display->setText(QString::number(calcVal));
}

void Calculator::CalcMemory(){
    QPushButton *button = (QPushButton *)sender();
    QString text = button->text();
    if(QString::compare(text, "M+", Qt::CaseInsensitive) == 0){
        memoryTriggered = true;
        QString displayVal = ui->display->text();
        memoryVal = displayVal.toDouble();
    }else if(QString::compare(text, "M-", Qt::CaseInsensitive) == 0){
        memoryTriggered = false;
    }else if(QString::compare(text, "M", Qt::CaseInsensitive) == 0){
        if(memoryTriggered){
            ui->display->setText(QString::number(memoryVal, 'g', 16));
        }
    }
}
