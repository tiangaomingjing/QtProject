#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>

class QLabel;
class QTreeWidget;
class QTreeWidgetItem;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

signals:
  void addrSelected();
private slots:
  void open();
  void assign();
  void save();
  void enable();
  //void test();
private:
  int writeAddress(QTreeWidgetItem *node, int newAddress);
  int changeAddress(QTreeWidgetItem *node, int newAddress);
  void writeOutFile(QString filePath);

private:
  Ui::MainWindow *ui;
  QAction *m_openAction;
  QAction *m_assignAddr;
  QAction *m_saveAction;
  QLabel *m_msgLabel;
  QTreeWidget *m_tree;
  bool continueAssign;
};

#endif // MAINWINDOW_H
