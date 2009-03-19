#include "textedit.h"

TextEdit::TextEdit( QWidget* aparent ) : QTextEdit( aparent ) {
    this->setCurrentFont( QFont( "Monospace", 10 ) );
    this->setTabStopWidth( 32 );
}

QSize TextEdit::sizeHint() const {
    return QSize( 250, this->height() );
}
