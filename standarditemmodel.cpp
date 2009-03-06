#include "mainwindow.h"
#include "standarditemmodel.h"

StandardItemModel::StandardItemModel( MainWindow* aparent ) : p( aparent ) {
}

QStringList StandardItemModel::mimeTypes() const {
    QStringList types;
    types << "qsnip/itempointer";
    types << "text/plain";
    return types;
}

QMimeData* StandardItemModel::mimeData( const QModelIndexList &indexes ) const {
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream( &encodedData, QIODevice::WriteOnly );

    foreach( QModelIndex index, indexes ) {
        Snippet* snippet = p->snippetForItem.value( itemFromIndex( index ) );
        stream << QString::number( (long)itemFromIndex( index ) );
        if( !snippet->isCategory() )
            mimeData->setText( snippet->code() );
    }

    mimeData->setData( "qsnip/itempointer", encodedData );
    return mimeData;
}

bool StandardItemModel::dropMimeData( const QMimeData *data, Qt::DropAction action,
                                     int, int, const QModelIndex &parent ) {
    if( action == Qt::IgnoreAction )
        return true;

    QByteArray encodedData = data->data( "qsnip/itempointer" );
    QDataStream stream( &encodedData, QIODevice::ReadOnly );
    QStringList newItems;

    while( !stream.atEnd() ) {
        QString text;
        stream >> text;
        newItems << text;
    }

    QStandardItem* itemToMove = ( QStandardItem* )newItems.at( 0 ).toLong();
    Snippet* newParentSnippet = p->snippetForItem.value( itemFromIndex( parent ) );
    if( !newParentSnippet )
        newParentSnippet = p->snippetForItem.value( invisibleRootItem() );
    Snippet* snippet = p->snippetForItem.value( itemToMove );

    if( !snippet || !itemToMove )
        return false;
    if( itemToMove->parent() == itemFromIndex( parent ) )
        return false;
    if( !newParentSnippet->isCategory() )
        return false;

    if( !itemToMove->parent() )
        itemToMove = takeRow( itemToMove->index().row() ).at( 0 );
    else
        itemToMove = itemToMove->parent()->takeRow( itemToMove->index().row() ).at( 0 );

    p->insertItem( itemToMove, p->snippetForItem.key( newParentSnippet ) );

    return true;
}
