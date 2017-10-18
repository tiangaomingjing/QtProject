#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qttreemanager.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  QTreeWidget *tree = QtTreeManager::createTreeWidgetFromXmlFile("F:/QtTreeManager/SD41P003.ui");
  QTreeWidgetItem *node;
  int newAddress = 0;
  //tree->invisibleRootItem();
  for (int i = 0; i < tree->topLevelItemCount(); i++)
  {
      node = tree->topLevelItem(i);
      newAddress = changeAddress(node, newAddress);
  }
  QtTreeManager::writeTreeWidgetToXmlFile("Result.ui", tree);
  tree->show();
}

MainWindow::~MainWindow()
{
  delete ui;
}

int MainWindow::writeAddress(QTreeWidgetItem *node, int newAddress) {
     QString str_type = node->text(5); // type
     QString str_address = node->text(6); // address
     QString str_value = node->text(1); // value
     if (str_address == "1" || str_address == "-2" || str_address == "-3") {
         if (str_value == "") {
             node->setText(1, "0");
         }
         node->setText(6, QString("0x%1").arg(newAddress, 4, 16, QLatin1Char('0')));
     } else if (str_address == "-1") {
         return newAddress;
     }
     int increment = 0;
     if (str_type == "Uint8") {
         increment = 1;
     } else if (str_type == "Uint16") {
         increment = 2;
     } else if (str_type == "Uint32") {
         increment = 4;
     }
     newAddress += increment;
     return newAddress;
}

int MainWindow::changeAddress(QTreeWidgetItem *node, int newAddress) {
    QString str_type = node->text(5); // type
    newAddress = writeAddress(node, newAddress);
    if (str_type == "Case") {
        int currentAddress = newAddress;
        for (int i = 0; i < node->childCount(); i++) {
            newAddress = changeAddress(node->child(i), currentAddress);
        }
    } else {
        for (int i = 0; i < node->childCount(); i++) {
            newAddress = changeAddress(node->child(i), newAddress);
        }
    }
    return newAddress;
}
