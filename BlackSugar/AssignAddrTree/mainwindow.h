#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
  Ui::MainWindow *ui;
  int writeAddress(QTreeWidgetItem *node, int newAddress);
  int changeAddress(QTreeWidgetItem *node, int newAddress);
};

#endif // MAINWINDOW_H
