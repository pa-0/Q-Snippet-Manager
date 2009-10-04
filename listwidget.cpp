#include "listwidget.h"

ListWidget::ListWidget( QWidget *parent )
        : QListWidget( parent )
{
}

ListWidget::~ListWidget()
{
}

void ListWidget::mousePressEvent( QMouseEvent *event ) {
    QListWidgetItem *itemAtCursor = itemAt( event->x(), event->y() );
    if( !itemAtCursor )
        return;

    if( currentItem() != itemAtCursor )
        this->setCurrentItem( itemAtCursor );

    QMimeData *data = new QMimeData();
    data->setText( currentItem()->data( Qt::UserRole ).toString() );

    QDrag *drag = new QDrag( this );
    drag->setMimeData( data );
    drag->exec( Qt::CopyAction );
}
