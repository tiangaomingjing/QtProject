#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileInfo>

class QTreeWidget;
class QTreeWidgetItem;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
signals:
    void textChangedOne(QString);
    void textChangedTwo(QString);
    void addrSelected();
private slots:
    void openFile();
    void openFiles();
    void openAssignFile();
    void fill();
    void enable();
    void assign();
    void save();
    void openOld();
    void openNew();
    void compare();
    void update();
    void saveNewFile();
private:
    Ui::Dialog *ui;
    void writeLimit(QTreeWidget *tree, QTreeWidget *temp);
    int changeAddress(QTreeWidgetItem *node, int newAddress);
    int writeAddress(QTreeWidgetItem *node, int newAddress);
    bool loadTree(QString &filename, QTreeWidget *treeWidget);
    void compareNode(QTreeWidgetItem *oldNode, QTreeWidgetItem *newNode);
    void updateNode(QTreeWidgetItem *oldNode, QTreeWidgetItem *newNode);
private:
    QStringList m_paths;
    QString m_path;
    QString m_filePath;
    QString m_allPaths;
    QString m_savePath;
    QFileInfo m_fileInfo;
    QFileInfo m_saveFileInfo;
    QTreeWidget *m_tree;
    bool continueAssign;
    int barCount;
};

#endif // DIALOG_H
