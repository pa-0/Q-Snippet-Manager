#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QListWidget>
#include <QMouseEvent>

class ListWidget : public QListWidget
{
//    Q_OBJECT
public:
    ListWidget( QWidget *parent = 0 );
    ~ListWidget();

protected:
    void mousePressEvent( QMouseEvent *event );
};

#endif // LISTWIDGET_H
