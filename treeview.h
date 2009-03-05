#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QTreeView>

class TreeView : public QTreeView
{
public:
    TreeView( QWidget* aparent = 0 );
    QSize sizeHint() const;
};

#endif // TREEVIEW_H
