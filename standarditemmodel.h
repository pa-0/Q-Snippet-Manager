#ifndef STANDARDITEMMODEL_H
#define STANDARDITEMMODEL_H

#include <QMimeData>
#include <QStandardItemModel>

#include "Snippet.h"

class MainWindow;

class StandardItemModel : public QStandardItemModel
{
public:
    StandardItemModel( MainWindow* aparent = 0 );

    bool dropMimeData( const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent );
    QMimeData* mimeData( const QModelIndexList &indexes ) const;
    QStringList mimeTypes() const;

private:
    MainWindow* p;
};

#endif // STANDARDITEMMODEL_H
