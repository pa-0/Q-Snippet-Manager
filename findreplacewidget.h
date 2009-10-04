#ifndef FINDREPLACEWIDGET_H
#define FINDREPLACEWIDGET_H

#include <QtGui/QWidget>
#include <QShortcut>
#include <QTextCursor>
#include <QTextEdit>

namespace Ui {
    class FindReplaceWidget;
}

class FindReplaceWidget : public QWidget {
    Q_OBJECT
public:
    FindReplaceWidget(QWidget *parent = 0);
    ~FindReplaceWidget();

    void setTextEdit( QTextEdit *te );

protected:
    void changeEvent(QEvent *e);
    void showEvent( QShowEvent *e );

private:
    Ui::FindReplaceWidget *m_ui;

    QTextEdit *textEdit;

private slots:
    void on_findLineEdit_returnPressed();
    void on_hideButton_clicked();
};

#endif // FINDREPLACEWIDGET_H
