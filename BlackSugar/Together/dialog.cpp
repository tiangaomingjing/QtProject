#include "dialog.h"
#include "ui_dialog.h"
#include "qttreemanager.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTreeWidgetItemIterator>
#include <QDir>

enum TEMP{
  TEMP_NAME = 0,
  TEMP_DOWN = 4,
  TEMP_UP = 5
};

enum TREE{
    TREE_NAME = 0,
    TREE_VALUE = 1,
    TREE_SCALE = 3,
    TREE_TYPE = 5,
    TREE_ADDRESS = 6,
    TREE_UP = 8,
    TREE_DOWN = 9
};

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->progressBar->setVisible(false);

    connect(ui->list, SIGNAL(currentRowChanged(int)), ui->stack, SLOT(setCurrentIndex(int)));
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(close()));

    m_allPaths = "";

    connect(ui->selectButton_1, SIGNAL(clicked()), this, SLOT(openFiles()));
    connect(ui->selectButton_2, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(this, SIGNAL(textChangedOne(QString)), ui->lineEdit_1, SLOT(setText(QString)));
    connect(this, SIGNAL(textChangedTwo(QString)), ui->lineEdit_2, SLOT(setText(QString)));
    connect(ui->fillButton, SIGNAL(clicked()), this, SLOT(fill()));

    ui->saveButton->setEnabled(false);
    ui->assignButton->setEnabled(false);

    connect(ui->openButton, SIGNAL(clicked()), this, SLOT(openAssignFile()));
    connect(this, SIGNAL(addrSelected()), this, SLOT(enable()));
    connect(ui->assignButton, SIGNAL(clicked()), this, SLOT(assign()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(save()));

    connect(ui->oldOpenButton, SIGNAL(clicked()), this, SLOT(openOld()));
    connect(ui->newOpenButton, SIGNAL(clicked()), this, SLOT(openNew()));
    connect(ui->compareButton, SIGNAL(clicked()), this, SLOT(compare()));
    connect(ui->updateButton, SIGNAL(clicked()), this, SLOT(update()));

    connect(ui->updateSaveButton, SIGNAL(clicked()), this, SLOT(saveNewFile()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::openFiles() {
    QStringList paths = QFileDialog::getOpenFileNames(NULL, tr("Open Files"),
                                                m_filePath, tr("XML Files(*.xml)"));
    if (!paths.isEmpty()) {
        QListIterator<QString> myIter(paths);
        QString str = "";
        while(myIter.hasNext()) {
            str = myIter.next();
            QFileInfo fileInfo;
            fileInfo.setFile(str);
            m_filePath = fileInfo.filePath()+"/";
            if (!m_allPaths.contains(fileInfo.fileName(), Qt::CaseSensitive)) {
                m_allPaths += fileInfo.fileName() + "; ";
                m_paths.append(str);
            }
        }
        emit textChangedOne(m_allPaths);
    } else {
        QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
    }
}

void Dialog::openFile() {
    m_path = QFileDialog::getOpenFileName(NULL, tr("Open File"),
                                                m_filePath, tr("XML Files(*.xml)"));
    if (!m_path.isNull()) {
        m_fileInfo.setFile(m_path);
        m_filePath = m_fileInfo.filePath() + "/";
        emit textChangedTwo(m_fileInfo.fileName() + "; ");
    } else {
        QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
    }
}

void Dialog::fill() {
    if (!m_path.isNull() && !m_paths.isEmpty()) {
        QTreeWidget *tree = QtTreeManager::createTreeWidgetFromXmlFile(m_path);
        QListIterator<QString> myIter(m_paths);
        ui->progressBar->show();
        ui->progressBar->setRange(0, m_paths.count());
        barCount = 0;
        while(myIter.hasNext()) {
            QString str = myIter.next();
            m_saveFileInfo.setFile(str);
            QTreeWidget *temp = QtTreeManager::createTreeWidgetFromXmlFile(str);
            writeLimit(tree, temp);
            ui->progressBar->setValue(++barCount);
            delete temp;
        }
        delete tree;
        ui->progressBar->hide();
    }
}

void Dialog::writeLimit(QTreeWidget *tree, QTreeWidget *temp) {
    QTreeWidgetItemIterator treeIter(tree);
    QString m_savePathTemp = m_saveFileInfo.path()+ "/Results/";
    QDir dir(m_savePathTemp);
    if(!dir.exists()) {
       dir.mkpath(m_savePathTemp);
    }
    m_savePath = m_savePathTemp + m_saveFileInfo.baseName() + "_2.xml";
    while (*treeIter) {
        QTreeWidgetItem *treeItem = (*treeIter);
        QString name = treeItem->text(TREE_NAME);
        QTreeWidgetItemIterator tempIter(temp);
        while (*tempIter) {
            QTreeWidgetItem *tempItem = (*tempIter);
            QString tempName = tempItem->text(TEMP_NAME);
            if (name == tempName) {
                QString up = treeItem->text(TREE_UP);
                QString down =treeItem->text(TREE_DOWN);
                tempItem->setText(TEMP_DOWN, down);
                tempItem->setText(TEMP_UP, up);
            }
            tempIter++;
        }
        treeIter++;
    }
    QtTreeManager::writeTreeWidgetToXmlFile(m_savePath, temp);
}

void Dialog::openAssignFile() {
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), m_filePath, tr("Files(*.ui)"));
    if (path.length() == 0) {
        QMessageBox::information(NULL, tr("Path"), tr("You did not select any files."));
    } else {
        QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
        m_fileInfo.setFile(path);
        m_filePath = m_fileInfo.filePath() + "/";
        ui->label_3->setText(m_fileInfo.fileName());
        emit Dialog::addrSelected();
        m_tree = QtTreeManager::createTreeWidgetFromXmlFile(path);
        //m_tree->show();
    }
}

void Dialog::enable() {
    ui->assignButton->setEnabled(true);
    ui->saveButton->setEnabled(true);
}

void Dialog::assign() {
    QTreeWidgetItem *node;
    int newAddress = 0;
    continueAssign = true;
    ui->progressBar->show();
    barCount = 0;
    ui->progressBar->setValue(barCount);
    for (int i = 0; i < m_tree->topLevelItemCount(); i++) {
        node = m_tree->topLevelItem(i);
        newAddress = changeAddress(node, newAddress);
    }
    ui->progressBar->hide();
    QMessageBox::information(this, "Assign", "Finish!", QMessageBox::Ok);
}

int Dialog::changeAddress(QTreeWidgetItem *node, int newAddress) {
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

int Dialog::writeAddress(QTreeWidgetItem *node, int newAddress) {
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
     barCount += 1;
     if (barCount % 10 == 0) {
         ui->progressBar->setValue(barCount % 100);
     }
     return newAddress;
}

void Dialog::save() {
    QString path = QFileDialog::getSaveFileName(this, tr("Save File"), m_filePath, tr("Files(*.ui)"));
    if (path.length() == 0) {
        QMessageBox::information(NULL, tr("Path"), tr("You did not select any files."));
    } else {
        QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
        m_fileInfo.setFile(path);
        m_filePath = m_fileInfo.filePath() + "/";
        QtTreeManager::writeTreeWidgetToXmlFile(path, m_tree);
    }
}

bool Dialog::loadTree(QString &path, QTreeWidget *treeWidget)
{
    QTreeWidget *tree = NULL;
    tree = QtTreeManager::createTreeWidgetFromXmlFile(path);;
    if(treeWidget == NULL)
        return false;
    else {
        QTreeWidgetItem *item;
        for(int i = 0;i < tree->topLevelItemCount(); i++)
        {
            item = tree->topLevelItem(i)->clone();
            treeWidget->addTopLevelItem(item);
        }
        item = tree->headerItem()->clone();
        treeWidget->setHeaderItem(item);
        tree->clear();
        delete tree;
        return true;
    }
}

void Dialog::openOld() {
    QString path = QFileDialog::getOpenFileName(this, tr("Open Old XML File"), m_filePath, tr("Files(*.xml)"));
    if (path.length() == 0) {
        QMessageBox::information(NULL, tr("Path"), tr("You did not select any files."));
    } else {
        QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
        m_fileInfo.setFile(path);
        m_filePath = m_fileInfo.filePath() + "/";
        loadTree(path, ui->oldTreeWidget);
        ui->oldTreeWidget->expandAll();
    }
}

void Dialog::openNew() {
    QString path = QFileDialog::getOpenFileName(this, tr("Open New XML File"), m_filePath, tr("Files(*.xml)"));
    if (path.length() == 0) {
        QMessageBox::information(NULL, tr("Path"), tr("You did not select any files."));
    } else {
        QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
        m_fileInfo.setFile(path);
        m_filePath = m_fileInfo.filePath() + "/";
        loadTree(path, ui->newTreeWidget);
        ui->newTreeWidget->expandAll();
    }
}

void Dialog::compare() {
    QTreeWidgetItem *oldNode = ui->oldTreeWidget->invisibleRootItem();
    QTreeWidgetItem *newNode = ui->newTreeWidget->invisibleRootItem();
    ui->progressBar->show();
    barCount = 0;
    ui->progressBar->setValue(barCount);
    compareNode(oldNode, newNode);
    ui->progressBar->hide();
    QMessageBox::information(this, "Compare", "Finish!", QMessageBox::Ok);
}

void Dialog::compareNode(QTreeWidgetItem *oldNode, QTreeWidgetItem *newNode) {
    barCount += 1;
    if (barCount % 10 == 0) {
        ui->progressBar->setValue(barCount % 100);
    }
    bool exist = false;
    for (int i = 0; i < newNode->childCount(); i++) {
        bool change = false;
        QString newNodeName = newNode->child(i)->text(TREE_NAME);
        for (int j = 0; j < oldNode->childCount(); j++) {
            QString oldNodeName = oldNode->child(j)->text(TREE_NAME);
            if (newNodeName.compare(oldNodeName) == 0) {
                exist = true;
                for (int k = 1; k < qMax(newNode->columnCount(), oldNode->columnCount()); k++) {
                    if (newNode->child(i)->text(k).compare(oldNode->child(j)->text(k)) != 0) {
                        change = true;
                        newNode->child(i)->setTextColor(k, Qt::red);
                        oldNode->child(j)->setTextColor(k, Qt::red);
                    }
                }
                if (change) {
                    for (int t = 0; t < oldNode->child(j)->columnCount(); t++) {
                        oldNode->child(j)->setBackgroundColor(t, Qt::yellow);
                    }
                    for (int t = 0; t < newNode->child(i)->columnCount(); t++) {
                        newNode->child(i)->setBackgroundColor(t, Qt::yellow);
                    }
                }
                compareNode(oldNode->child(j), newNode->child(i));
                break;
            }
        }
        if (!exist) {
            for (int t = 0; t < newNode->child(i)->columnCount(); t++) {
                newNode->child(i)->setBackgroundColor(t, Qt::green);
            }
        }
    }
    exist = false;
    for (int i = 0; i < oldNode->childCount(); i++) {
        QString oldNodeName = oldNode->child(i)->text(TREE_NAME);
        for (int j = 0; j < newNode->childCount(); j++) {
            QString newNodeName = newNode->child(j)->text(TREE_NAME);
            if (newNodeName.compare(oldNodeName) == 0) {
                exist = true;
                break;
            }
        }
        if (!exist) {
            for (int t = 0; t < oldNode->child(i)->columnCount(); t++) {
                oldNode->child(i)->setBackgroundColor(t, Qt::red);
            }
        }
    }
}

void Dialog::update() {
    QTreeWidgetItem *oldNode = ui->oldTreeWidget->invisibleRootItem();
    QTreeWidgetItem *newNode = ui->newTreeWidget->invisibleRootItem();
    ui->progressBar->show();
    barCount = 0;
    ui->progressBar->setValue(barCount);
    updateNode(oldNode, newNode);
    ui->progressBar->hide();
    QMessageBox::information(this, "Update", "Finish!", QMessageBox::Ok);
}

void Dialog::updateNode(QTreeWidgetItem *oldNode, QTreeWidgetItem *newNode) {
    barCount += 1;
    if (barCount % 10 == 0) {
        ui->progressBar->setValue(barCount % 100);
    }
    //bool exist = false;
    for (int i = 0; i < newNode->childCount(); i++) {
        QString newNodeName = newNode->child(i)->text(TREE_NAME);
        for (int j = 0; j < oldNode->childCount(); j++) {
            QString oldNodeName = oldNode->child(j)->text(TREE_NAME);
            if (newNodeName.compare(oldNodeName) == 0) {
                //exist = true;
                for (int k = 1; k < qMax(newNode->child(i)->columnCount(), oldNode->child(j)->columnCount()); k++) {
                    if (newNode->child(i)->text(k).compare(oldNode->child(j)->text(k)) != 0) {
                        oldNode->child(j)->setText(k, newNode->child(i)->text(k));
                    }
                }
                updateNode(oldNode->child(j), newNode->child(i));
                break;
            }
        }
        /*if (!exist) {
            newNode->child(i)->setTextColor(TREE_NAME, Qt::green);
        }*/
    }
    /*exist = false;
    for (int i = 0; i < oldNode->childCount(); i++) {
        QString oldNodeName = oldNode->child(i)->text(TREE_NAME);
        for (int j = 0; j < newNode->childCount(); j++) {
            QString newNodeName = newNode->child(j)->text(TREE_NAME);
            if (newNodeName.compare(oldNodeName) == 0) {
                exist = true;
                break;
            }
        }
        if (!exist) {
            oldNode->child(i)->setTextColor(TREE_NAME, Qt::red);
        }
    }*/
}

void Dialog::saveNewFile() {
    QString path = QFileDialog::getSaveFileName(this, tr("Save File"), m_filePath, tr("Files(*.xml)"));
    if (path.length() == 0) {
        QMessageBox::information(NULL, tr("Path"), tr("You did not select any files."));
    } else {
        QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
        m_fileInfo.setFile(path);
        m_filePath = m_fileInfo.filePath() + "/";
        QtTreeManager::writeTreeWidgetToXmlFile(path, ui->newTreeWidget);
    }
}
