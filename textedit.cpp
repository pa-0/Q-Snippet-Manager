#include "textedit.h"

TextEdit::TextEdit( QWidget* aparent ) : QTextEdit( aparent ) {}

QSize TextEdit::sizeHint() const {
    return QSize( 150, this->height() );
}
