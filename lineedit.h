#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>

class LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    LineEdit( QWidget *parent = 0);
    ~LineEdit();

protected:
    void mouseMoveEvent( QMouseEvent * );

signals:
    void mouseOver();
};

#endif // LINEEDIT_H
