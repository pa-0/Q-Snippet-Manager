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
