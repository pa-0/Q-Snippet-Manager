#include "mainwindow.h"
#include "treeview.h"

TreeView::TreeView( QWidget* aparent ) : QTreeView( aparent ) {}

QSize TreeView::sizeHint() const {
    return QSize( 200, this->height() );
}
