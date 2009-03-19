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
#include <QStandardItemModel>
#include <QTextStream>
#include <QTimer>
#include <QTreeWidgetItem>

#include "Snippet.h"
#include "standarditemmodel.h"
#include "textedit.h"
#include "workmodedialog.h"

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

    QByteArray mainWindowGeometry;
    QByteArray workModeDialogGeometry;
    QHash< QStandardItem*, Snippet* > snippetForItem;
    QIcon categoryIcon;
    QIcon snippetIcon;
    StandardItemModel model;
    WorkModeDialog workModeDialog;

private slots:
    void on_actionAbout_Qt_activated();
    void on_action_About_activated();
    void on_tabWidget_currentChanged(int index);
    void on_descTextEdit_textChanged();
    void on_actionHide_description_activated();
    void on_actionHide_categories_activated();
    void on_WorkModeDialog_finished(int result);
    void on_action_Normal_activated();
    void on_action_Work_activated();
    void on_action_Delete_activated();
    void on_searchLineEdit_textChanged(QString );
    void on_action_Exit_activated();
    void on_actionSave_snippets_as_activated();
    void on_action_Save_2_activated();
    void on_action_Main_category_activated();
    void on_actionClos_e_all_activated();
    void on_action_Close_activated();
    void on_actionSave_all_activated();
    void on_action_Save_activated();
    void on_action_Snippet_activated();
    void on_action_Category_activated();
    void on_snippetTreeView_activated(QModelIndex index);

    void deleteChildItems( QStandardItem* parent );
    Snippet* findSnippetByTab( int atab );
    void insertItem( QStandardItem* item, QStandardItem* parent );
    void loadSnippets();
    void parseCategoryElement( const QDomElement &element, QStandardItem* parent );
    void parseModel( QStandardItem* parent, QString& xml );
    void restoreTabNumbers();
    void saveSnippets( const QString& fileName = "" );
    bool searchModelForString( const QString &searchString, QStandardItem* parent );
    void showAllSnippets( QStandardItem* parent );
    void snippetsCodeModified();
    QString createToolTip( const Snippet* snippet );
    QString toValidXml( QString string );
    void updateSnippetsTitle( QStandardItem* item );

// friends
    friend class StandardItemModel;
    friend class WorkModeDialog;
};

#endif // MAINWINDOW_H
