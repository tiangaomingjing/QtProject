#include "fillwindow.h"
#include "ui_fillwindow.h"
#include "filldialog.h"
#include <QtGui>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QKeySequence>
#include <QToolBar>

FillWindow::FillWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FillWindow)
{
    ui->setupUi(this);

    m_startAction = new QAction(tr("&Start"), this);
    m_startAction->setStatusTip(tr("Start"));

    QMenu *file = menuBar()->addMenu(tr("&File"));
    file->addAction(m_startAction);

    connect(m_startAction, SIGNAL(triggered()), this, SLOT(start()));
}

FillWindow::~FillWindow()
{
    delete ui;
}

void FillWindow::start() {
    FillDialog *dialog = new FillDialog(this);
    dialog->show();
}
