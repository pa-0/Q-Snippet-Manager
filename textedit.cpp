#include "textedit.h"

TextEdit::TextEdit( QWidget* aparent ) : QTextEdit( aparent ) {
#ifdef Q_WS_X11
    this->setCurrentFont( QFont( "Monospace", 9 ) );
#endif
#ifdef Q_WS_WIN
    this->setCurrentFont( QFont( "Courier New", 9 ) );
#endif
    this->currentFont().setFixedPitch( true );
    this->setTabStopWidth( 32 );
}

QSize TextEdit::sizeHint() const {
    return QSize( 250, this->height() );
}
