#include "lineedit.h"

LineEdit::LineEdit( QWidget *parent )
        : QLineEdit( parent )
{
}

LineEdit::~LineEdit()
{
}

void LineEdit::mouseMoveEvent( QMouseEvent * ) {
    emit mouseOver();
}

void LineEdit::focusOutEvent( QFocusEvent *e ) {
    QLineEdit::focusOutEvent( e );

    if( e->reason() == Qt::TabFocusReason )
        emit lostFocusByTab();
}
