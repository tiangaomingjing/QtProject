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
enum TREE{
    TREE_NAME = 0,
    TREE_VALUE = 1,
    TREE_SCALE = 3,
    TREE_TYPE = 5,
    TREE_ADDRESS = 6,
};

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

  //connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(test()));
}

MainWindow::~MainWindow()
{
  delete ui;
  delete m_tree;
}

int MainWindow::writeAddress(QTreeWidgetItem *node, int newAddress) {
     QString str_type = node->text(TREE_TYPE); // type
     QString str_address = node->text(TREE_ADDRESS); // address
     QString str_value = node->text(TREE_VALUE); // value
     QString str_scale = node->text(TREE_SCALE); // scale
     QString str_name = node->text(TREE_NAME);
     bool ok;
     int increment = 0;
     if (str_type == "Uint8") {
         increment = 1;
     } else if (str_type == "Uint16") {
         increment = 2;
     } else if (str_type == "Uint32") {
         increment = 4;
     }
     double temp = str_value.toDouble(&ok) * str_scale.toDouble(&ok);
     if (ok && str_address != "-1") {
         if (temp < -pow(256, increment) || temp > pow(256, increment)) {
             QMessageBox::warning(this, "Assign", "Out of range on " + str_name, QMessageBox::Ok);
             continueAssign = false;
             return 0;
         }
     }
     if (str_address == "1" || str_address == "-2" || str_address == "-3") {
         if (str_value == "") {
             node->setText(1, "0");
         }
         node->setText(TREE_ADDRESS, QString("0x%1").arg(newAddress, 4, 16, QLatin1Char('0')));
     } else if (str_address == "-1") {
         return newAddress;
     }
     newAddress += increment;
     return newAddress;
}

int MainWindow::changeAddress(QTreeWidgetItem *node, int newAddress) {
    if (continueAssign) {
        QString str_type = node->text(TREE_TYPE); // type
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
    }
    return newAddress;
}

void MainWindow::assign() {
    /*QTreeWidgetItem *node1;
    QTreeWidgetItem *node2;
    QTreeWidgetItem *node3;*/
    QTreeWidgetItem *node;
    int newAddress = 0;
    //int newAddress = 1;
    //tree->invisibleRootItem();
    continueAssign = true;
    for (int i = 0; i < m_tree->topLevelItemCount(); i++) {
        node = m_tree->topLevelItem(i);
        newAddress = changeAddress(node, newAddress);
    }
    //m_tree->show();
    /*node1 = m_tree->invisibleRootItem()->child(1)->child(0)->child(0)->child(0)->child(0);
    node2 = m_tree->invisibleRootItem()->child(1)->child(0)->child(1)->child(0)->child(0);
    node3 = m_tree->invisibleRootItem()->child(1)->child(0)->child(1)->child(0)->child(1);
    newAddress = changeAddress(node1, newAddress);
    newAddress = 1;
    newAddress = changeAddress(node2, newAddress);
    newAddress = 1;
    newAddress = changeAddress(node3, newAddress);
    //m_tree->show();*/;
    QMessageBox::information(this, "Assign", "Finish!", QMessageBox::Ok);
}

void MainWindow::open() {
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), "../../BlackSugar/AssignAddrTree_NEW", tr("Files(*.ui)"));
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
    QString path = QFileDialog::getSaveFileName(this, tr("Save File"), "../../BlackSugar/AssignAddrTree_NEW", tr("Files(*.ui)"));
    if (path.length() == 0) {
        QMessageBox::information(NULL, tr("Path"), tr("You did not select any files."));
    } else {
        QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
        /*QTreeWidgetItem *node = node = m_tree->invisibleRootItem()->child(1)->child(0)->child(0)->child(0)->child(0)->child(0)->child(1)->child(0)->child(1);
        qDebug()<<node->text(TREE_NAME);
        qDebug()<<node->text(9);*/
        QtTreeManager::writeTreeWidgetToXmlFile(path, m_tree);
        m_tree->show();
    }
}

void MainWindow::enable() {
    m_assignAddr->setEnabled(true);
    m_saveAction->setEnabled(true);
}

/*void MainWindow::test() {
    QTreeWidget *tree;
    tree = QtTreeManager::createTreeWidgetFromXmlFile("../../BlackSugar/AssignAddrTree_NEW/PowerBoard.ui");
    tree->show();
    qDebug()<<"0 "<<tree->topLevelItem(0)->columnCount();
    qDebug()<<"1 "<<tree->topLevelItem(1)->columnCount();
    QtTreeManager::writeTreeWidgetToXmlFile("../../BlackSugar/AssignAddrTree_NEW/PowerBoard_3.ui", tree);
}*/
