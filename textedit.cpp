#include "mainwindow.h"
#include "textedit.h"

TextEdit::TextEdit( QWidget* aparent ) : QTextEdit( aparent ) {
#ifdef Q_WS_X11
    font = QFont( "Monospace", 9 );
#endif
#ifndef Q_WS_X11
    font = QFont( "Courier New", 9 );
#endif
    this->setCurrentFont( font );
    this->currentFont().setFixedPitch( true );
    this->setTabStopWidth( 32 );
    this->setAcceptRichText( false );
}

QSize TextEdit::sizeHint() const {
    return QSize( 250, this->height() );
}
