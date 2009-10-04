#ifndef TAGVIEWER_H
#define TAGVIEWER_H

#include <QMap>
#include <QMouseEvent>
#include <QTextEdit>

class TagViewer : public QTextEdit
{
    Q_OBJECT
public:
    TagViewer( QWidget *parent = 0 );
    ~TagViewer();

    void generate( QMap< QString, int > tags );
    QStringList selectedTags();

protected:
    void mouseDoubleClickEvent( QMouseEvent *e );
    void mousePressEvent( QMouseEvent *e );

private:
    QMap< QString, bool > tagSelection;
    QMap< QString, int > tagCount;

signals:
    void tagSelectionChanged();
};

#endif // TAGVIEWER_H
