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
    void removeHighlight();
    void highlightSearchStr( const QString &searchStr, int light = 160 );
    void highlightSelectedBlock();

    int blockBegPos;
    int blockEndPos;
    int searchFromPos;

    bool firstRun;
    bool entireDocument;

    Ui::FindReplaceWidget *m_ui;

    QTextEdit *textEdit;

private slots:
    void on_pushButton_5_clicked();
    void on_replaceLineEdit_returnPressed();
    void resetSearch();
    void on_findLineEdit_textChanged(QString );
    void on_findLineEdit_returnPressed();
    void on_hideButton_clicked();
};

#endif // FINDREPLACEWIDGET_H
