#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qttreemanager.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QMenu>
#include <QKeySequence>
#include <QToolBar>
#include <QMenuBar>
#include <QMessageBox>
#include <QLabel>
#include <QStatusBar>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  m_openAction = new QAction(tr("Open"), this);
  m_openAction->setShortcut(QKeySequence::Open);
  m_openAction->setStatusTip(tr("Open a file."));
  connect(m_openAction, SIGNAL(triggered()), this, SLOT(open()));

  QMenu *file = menuBar()->addMenu(tr("&File"));
  file->addAction(m_openAction);

  m_assignAddr = new QAction(tr("Assign Address"), this);
  m_assignAddr->setShortcut(QKeySequence("A"));
  m_assignAddr->setStatusTip(tr("Assign address for the file."));
  m_assignAddr->setEnabled(false);
  connect(m_assignAddr, SIGNAL(triggered()), this, SLOT(assign()));

  file->addAction(m_assignAddr);

  m_saveAction = new QAction(tr("Save"), this);
  m_saveAction->setShortcut(QKeySequence::Save);
  m_saveAction->setStatusTip(tr("Save the file."));
  m_saveAction->setEnabled(false);
  connect(m_saveAction, SIGNAL(triggered()), this, SLOT(save()));

  connect(this, SIGNAL(addrSelected()), this, SLOT(enable()));

  file->addAction(m_saveAction);

  m_msgLabel = new QLabel;
  m_msgLabel->setMinimumSize(m_msgLabel->sizeHint());
  m_msgLabel->setAlignment(Qt::AlignHCenter);

  statusBar()->addWidget(m_msgLabel);
  statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}"));
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

void MainWindow::assign() {
    QTreeWidgetItem *node;
    int newAddress = 0;
    //tree->invisibleRootItem();
    for (int i = 0; i < m_tree->topLevelItemCount(); i++) {
        node = m_tree->topLevelItem(i);
        newAddress = changeAddress(node, newAddress);
    }
    m_tree->show();
}

void MainWindow::open() {
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Files(*.ui)"));
    if (path.length() == 0) {
        QMessageBox::information(NULL, tr("Path"), tr("You did not select any files."));
    } else {
        QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
        emit MainWindow::addrSelected();
        m_tree = QtTreeManager::createTreeWidgetFromXmlFile(path);
        m_tree->show();
    }
}

void MainWindow::save() {
    QString path = QFileDialog::getSaveFileName(this, tr("Save File"), ".", tr("Files(*.ui)"));
    if (path.length() == 0) {
        QMessageBox::information(NULL, tr("Path"), tr("You did not select any files."));
    } else {
        QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
        QtTreeManager::writeTreeWidgetToXmlFile(path, m_tree);
    }
}

void MainWindow::enable() {
    m_assignAddr->setEnabled(true);
    m_saveAction->setEnabled(true);
}
