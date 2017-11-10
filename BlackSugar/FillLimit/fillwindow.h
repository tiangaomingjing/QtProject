#ifndef FILLWINDOW_H
#define FILLWINDOW_H

#include <QMainWindow>

class QAction;


namespace Ui {
class FillWindow;
}

class FillWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit FillWindow(QWidget *parent = 0);
  ~FillWindow();


private slots:
    void start();
private:
    Ui::FillWindow *ui;
private:
    QAction *m_startAction;

};

#endif // FILLWINDOW_H
