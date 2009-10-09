#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QFocusEvent>
#include <QLineEdit>

class LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    LineEdit( QWidget *parent = 0);
    ~LineEdit();

protected:
    void focusOutEvent( QFocusEvent *e );
    void mouseMoveEvent( QMouseEvent * );

signals:
    void lostFocusByTab();
    void mouseOver();
};

#endif // LINEEDIT_H
