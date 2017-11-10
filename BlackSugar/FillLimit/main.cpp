
#include "fillwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  FillWindow w;
  w.show();

  return a.exec();
}
