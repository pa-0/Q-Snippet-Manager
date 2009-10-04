#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <ctime>
#include <cmath>

#include <QtGui/QMainWindow>
#include <QAction>
#include <QList>
#include <QMap>
#include <QMessageBox>
#include <QMenu>
#include <QModelIndex>
#include <QPushButton>
#include <QSettings>
#include <QString>
#include <QStringList>

#include "tabwidget.h"
#include "workmodedialog.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent( QCloseEvent *event );

private:
    void generateSnippetsList();
    void generateTagBoard();
    void generateTags();
    QStringList generateAttribList( QString key );
    QStringList generateCode();
    QStringList generateToolTips();
    void openSnippet( const QString &name );
    void setupActions();
    void setupAll();
    void updateSnippetsList();

    Ui::MainWindow *ui;

    WorkModeDialog *workModeDialog;

    QAction *closeSnippetAction;
    QAction *configAction;
    QAction *deleteSnippetAction;
    QAction *editModeAction;
    QAction *findReplaceAction;
    QAction *newSnippetAction;
    QAction *renameSnippetAction;
    QAction *workModeAction;

    QMap< QString, int > tags;
    QStringList shownSnippets;
    QStringList snippetsList;

    QSettings config;
    QSettings snippets;

private slots:
    void applySearchFilter();
    void deleteSnippet();
    void showWorkModeDialog();
    void on_searchLineEdit_textChanged(QString );
    void newSnippet();
    void onCloseSnippetRequested();
    void on_tabWidget_tabCloseRequested(int index);
    void on_snippetsList_activated(QModelIndex index);
    void onSnippetSaved( QString name, QString code, QString desc, QString tags );
    void onTagButtonClicked();
    void renameSnippet();
    void showFindReplaceWidget();

signals:
    void filterApplied( QStringList shownSnippets, QStringList toolTips, QStringList code );
};

#endif // MAINWINDOW_H
