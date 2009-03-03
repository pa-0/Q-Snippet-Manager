#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QHash>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QList>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QStandardItemModel>
#include <QTextStream>
#include <QTreeWidgetItem>

#include "Snippet.h"

namespace Ui
{
    class MainWindowClass;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindowClass *ui;

    QHash< QStandardItem*, Snippet* > snippetForItem;
    QIcon categoryIcon;
    QIcon snippetIcon;
    QStandardItemModel model;

private slots:
    void on_action_Main_category_activated();
    void on_actionClos_e_all_activated();
    void on_action_Close_activated();
    void on_actionSave_all_activated();
    Snippet* findSnippetByTab( int atab );
    void insertItem( QStandardItem* item, QStandardItem* parent );
    void loadSnippets();
    void on_action_Save_activated();
    void on_action_Snippet_activated();
    void on_action_Category_activated();
    void on_snippetTreeView_activated(QModelIndex index);
    void parseCategoryElement( const QDomElement &element, QStandardItem* parent );
    void parseModel( QStandardItem* parent, QString& xml );
    void restoreTabNumbers();
    void saveSnippets();
    void snippetsCodeModified();
};

#endif // MAINWINDOW_H
