#ifndef FILLWINDOW_H
#define FILLWINDOW_H

#include <QMainWindow>

namespace Ui {
class FillWindow;
}

class FillWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit FillWindow(QWidget *parent = 0);
  ~FillWindow();

private:
  Ui::FillWindow *ui;
};

#endif // FILLWINDOW_H
