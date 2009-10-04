#ifndef WORKMODEDIALOG_H
#define WORKMODEDIALOG_H

#include <QtGui/QDialog>
#include <QEvent>
#include <QLineEdit>
#include <QListWidget>
#include <QMouseEvent>
#include <QSettings>

namespace Ui {
    class WorkModeDialog;
}

class WorkModeDialog : public QDialog {
    Q_OBJECT
public:
    WorkModeDialog( QWidget *parent, QListWidget *pListWidget, QLineEdit *pLineEdit, QSettings &settings );
    ~WorkModeDialog();

public slots:
    void onFilterApplied( QStringList, QStringList, QStringList );

protected:
    void changeEvent(QEvent *e);
    void enterEvent( QEvent *event );
    void leaveEvent( QEvent *event );
    void showEvent( QShowEvent *e );

private:
    void hideList();
    void showList();

    Ui::WorkModeDialog *m_ui;

    QLineEdit *parentLineEdit;
    QListWidget *parentListWidget;

    QSettings &config;

private slots:
    void on_lineEdit_textChanged(QString );

signals:
    void searchStringEntered( QString );
};

#endif // WORKMODEDIALOG_H
