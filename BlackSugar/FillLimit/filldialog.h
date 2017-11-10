#ifndef FILLDIALOG_H
#define FILLDIALOG_H

#include <QDialog>
#include <QFileDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QTreeWidget;
class QFileInfo;
class QProgressBar;

namespace Ui {
class FillDialog;
}

class FillDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FillDialog(QWidget *parent = 0);
    ~FillDialog();
signals:
    void textChangedOne(QString);
    void textChangedTwo(QString);
private slots:
    void openFiles();
    void openFile();
    void write();
private:
    Ui::FillDialog *ui;
    void createDialog();
    void writeLimit(QTreeWidget *tree, QTreeWidget *temp);
private:
    QLabel *m_labelOne;
    QLineEdit *m_lineEditOne;
    QLabel *m_labelTwo;
    QLineEdit *m_lineEditTwo;
    QPushButton *m_selectButtonOne;
    QPushButton *m_selectButtonTwo;
    QPushButton *m_cancelButton;
    QPushButton *m_workButton;
    QStringList m_paths;
    QString m_path;
    QString m_filePath;
    QString m_allPaths;
    QString m_savePath;
    QFileInfo m_fileInfo;
    QFileInfo m_saveFileInfo;
    QProgressBar *m_progressBar;
};

#endif // FILLDIALOG_H
