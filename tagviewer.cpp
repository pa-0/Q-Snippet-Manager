#include "tagviewer.h"

TagViewer::TagViewer( QWidget *parent )
        : QTextEdit( parent )
{
    setReadOnly( true );
}

TagViewer::~TagViewer()
{
}

void TagViewer::generate( QMap< QString, int > tags ) {
    tagCount = tags;

    tagSelection.clear();
    clear();

    for( QMap< QString, int >::iterator i = tagCount.begin(); i != tagCount.end(); ++i ) {
        tagSelection[ i.key() ] = true;
        setFontPointSize( 5 + i.value() * 2 );
        insertPlainText( "  " + i.key() + "  " );
    }
}

QStringList TagViewer::selectedTags() {
    QStringList tags;

    for( QMap< QString, bool >::iterator i = tagSelection.begin(); i != tagSelection.end(); ++i )
        if( i.value() )
            tags.append( i.key() );

    return tags;
}

void TagViewer::mousePressEvent( QMouseEvent *e ) {
    QTextCursor textCursor = this->cursorForPosition( e->pos() );
    textCursor.select( QTextCursor::WordUnderCursor );
    QString tag = textCursor.selectedText();

    QTextCharFormat format;
    if( tagSelection[ tag ] == true ) {
        // then switch it off
        format.setForeground( QColor( Qt::gray ) );
        tagSelection[ tag ] = false;
    } else {
        // then switch it on
        format.setForeground( QColor( Qt::black ) );
        tagSelection[ tag ] = true;
    }
    textCursor.mergeCharFormat( format );

    emit tagSelectionChanged();
}

void TagViewer::mouseDoubleClickEvent( QMouseEvent *e ) {
    e->ignore();
}
