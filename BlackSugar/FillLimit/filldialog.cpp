#include "filldialog.h"
#include "ui_filldialog.h"
#include "qttreemanager.h"

#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QPushButton>
#include <QTreeWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QLinkedList>
#include <QFileInfo>
#include <QTreeWidgetItemIterator>
#include <QDir>
#include <QDebug>
#include <QProgressBar>

enum TEMP{
  TEMP_NAME = 0,
  TEMP_DOWN = 4,
  TEMP_UP = 5
};
enum TREE{
  TREE_NAME = 0,
  TREE_UP = 2,
  TREE_DOWN = 3
};

FillDialog::FillDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FillDialog),
    m_filePath(".")
{
    ui->setupUi(this);
    createDialog();
}

FillDialog::~FillDialog()
{
    delete ui;
}

void FillDialog::createDialog() {
    m_labelOne = new QLabel(tr("Select written files:"));
    m_lineEditOne = new QLineEdit;
    m_labelOne->setBuddy(m_lineEditOne);
    m_lineEditOne->setMinimumWidth(150);
    m_lineEditOne->setReadOnly(true);

    m_labelTwo = new QLabel(tr("Select Catalog:"));
    m_lineEditTwo = new QLineEdit;
    m_labelTwo->setBuddy(m_lineEditTwo);
    m_lineEditTwo->setReadOnly(true);

    m_cancelButton = new QPushButton(tr("Cancel"));
    m_workButton = new QPushButton(tr("Write"));
    m_selectButtonOne = new QPushButton(tr("..."));
    m_selectButtonTwo = new QPushButton(tr("..."));
    m_progressBar = new QProgressBar;

    m_allPaths = "";

    connect(m_selectButtonOne, SIGNAL(clicked()), this, SLOT(openFiles()));
    connect(m_selectButtonTwo, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(this, SIGNAL(textChangedOne(QString)), m_lineEditOne, SLOT(setText(QString)));
    connect(this, SIGNAL(textChangedTwo(QString)), m_lineEditTwo, SLOT(setText(QString)));
    connect(m_workButton, SIGNAL(clicked()), this, SLOT(write()));
    connect(m_cancelButton, SIGNAL(clicked(bool)), this, SLOT(close()));

    QHBoxLayout *topUpLayout = new QHBoxLayout;
    topUpLayout->addWidget(m_labelOne);
    topUpLayout->addWidget(m_lineEditOne);
    topUpLayout->addWidget(m_selectButtonOne);

    QHBoxLayout *topDownLayout = new QHBoxLayout;
    topDownLayout->addWidget(m_labelTwo);
    topDownLayout->addWidget(m_lineEditTwo);
    topDownLayout->addWidget(m_selectButtonTwo);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(m_workButton);
    bottomLayout->addWidget(m_cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topUpLayout);
    mainLayout->addLayout(topDownLayout);
    mainLayout->addWidget(m_progressBar);
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);

    setWindowTitle(tr("Fill limit"));
//    setFixedHeight(sizeHint().height());
//    setFixedWidth(800);
}

void FillDialog::openFiles() {
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
            //qDebug()<<m_paths.count();
        }
        emit textChangedOne(m_allPaths);
    } else {
        QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
    }
}

void FillDialog::openFile() {
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

void FillDialog::write() {
    if (!m_path.isNull() && !m_paths.isEmpty()) {
        QTreeWidget *tree = QtTreeManager::createTreeWidgetFromXmlFile(m_path);
        //tree->show();
        QListIterator<QString> myIter(m_paths);
        m_progressBar->setRange(0, m_paths.count());
        int i = 0;
        while(myIter.hasNext()) {
            QString str = myIter.next();
            m_saveFileInfo.setFile(str);
            QTreeWidget *temp = QtTreeManager::createTreeWidgetFromXmlFile(str);
            writeLimit(tree, temp);
            m_progressBar->setValue(++i);
            delete temp;
        }
        delete tree;
    }
}

void FillDialog::writeLimit(QTreeWidget *tree, QTreeWidget *temp) {
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
    //temp->show();
    QtTreeManager::writeTreeWidgetToXmlFile(m_savePath, temp);
}
