#include "fillwindow.h"
#include "ui_fillwindow.h"

FillWindow::FillWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::FillWindow)
{
  ui->setupUi(this);
}

FillWindow::~FillWindow()
{
  delete ui;
}
