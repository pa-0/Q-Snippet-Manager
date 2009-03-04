/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Wed Mar 4 00:03:02 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *action_Category;
    QAction *action_Snippet;
    QAction *action_Save;
    QAction *actionSave_all;
    QAction *action_Close;
    QAction *actionClos_e_all;
    QAction *action_Main_category;
    QAction *action_Save_2;
    QAction *actionSave_snippets_as;
    QAction *action_Exit;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QMenuBar *menuBar;
    QMenu *menu_Add;
    QMenu *menu_Snippet;
    QMenu *menu_File;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *leftDockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QLineEdit *searchLineEdit;
    QTreeView *snippetTreeView;

    void setupUi(QMainWindow *MainWindowClass)
    {
    if (MainWindowClass->objectName().isEmpty())
        MainWindowClass->setObjectName(QString::fromUtf8("MainWindowClass"));
    MainWindowClass->resize(600, 400);
    action_Category = new QAction(MainWindowClass);
    action_Category->setObjectName(QString::fromUtf8("action_Category"));
    action_Snippet = new QAction(MainWindowClass);
    action_Snippet->setObjectName(QString::fromUtf8("action_Snippet"));
    action_Save = new QAction(MainWindowClass);
    action_Save->setObjectName(QString::fromUtf8("action_Save"));
    actionSave_all = new QAction(MainWindowClass);
    actionSave_all->setObjectName(QString::fromUtf8("actionSave_all"));
    action_Close = new QAction(MainWindowClass);
    action_Close->setObjectName(QString::fromUtf8("action_Close"));
    actionClos_e_all = new QAction(MainWindowClass);
    actionClos_e_all->setObjectName(QString::fromUtf8("actionClos_e_all"));
    action_Main_category = new QAction(MainWindowClass);
    action_Main_category->setObjectName(QString::fromUtf8("action_Main_category"));
    action_Save_2 = new QAction(MainWindowClass);
    action_Save_2->setObjectName(QString::fromUtf8("action_Save_2"));
    actionSave_snippets_as = new QAction(MainWindowClass);
    actionSave_snippets_as->setObjectName(QString::fromUtf8("actionSave_snippets_as"));
    action_Exit = new QAction(MainWindowClass);
    action_Exit->setObjectName(QString::fromUtf8("action_Exit"));
    centralWidget = new QWidget(MainWindowClass);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
    verticalLayout_2 = new QVBoxLayout(centralWidget);
    verticalLayout_2->setSpacing(2);
    verticalLayout_2->setMargin(2);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    tabWidget = new QTabWidget(centralWidget);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    tabWidget->setTabShape(QTabWidget::Rounded);
    widget = new QWidget();
    widget->setObjectName(QString::fromUtf8("widget"));
    horizontalLayout = new QHBoxLayout(widget);
    horizontalLayout->setSpacing(2);
    horizontalLayout->setMargin(2);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    tabWidget->addTab(widget, QString());

    verticalLayout_2->addWidget(tabWidget);

    MainWindowClass->setCentralWidget(centralWidget);
    menuBar = new QMenuBar(MainWindowClass);
    menuBar->setObjectName(QString::fromUtf8("menuBar"));
    menuBar->setGeometry(QRect(0, 0, 600, 25));
    menu_Add = new QMenu(menuBar);
    menu_Add->setObjectName(QString::fromUtf8("menu_Add"));
    menu_Snippet = new QMenu(menuBar);
    menu_Snippet->setObjectName(QString::fromUtf8("menu_Snippet"));
    menu_File = new QMenu(menuBar);
    menu_File->setObjectName(QString::fromUtf8("menu_File"));
    MainWindowClass->setMenuBar(menuBar);
    mainToolBar = new QToolBar(MainWindowClass);
    mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
    MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
    statusBar = new QStatusBar(MainWindowClass);
    statusBar->setObjectName(QString::fromUtf8("statusBar"));
    MainWindowClass->setStatusBar(statusBar);
    leftDockWidget = new QDockWidget(MainWindowClass);
    leftDockWidget->setObjectName(QString::fromUtf8("leftDockWidget"));
    leftDockWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
    leftDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
    dockWidgetContents = new QWidget();
    dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
    verticalLayout = new QVBoxLayout(dockWidgetContents);
    verticalLayout->setSpacing(2);
    verticalLayout->setMargin(2);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    searchLineEdit = new QLineEdit(dockWidgetContents);
    searchLineEdit->setObjectName(QString::fromUtf8("searchLineEdit"));

    verticalLayout->addWidget(searchLineEdit);

    snippetTreeView = new QTreeView(dockWidgetContents);
    snippetTreeView->setObjectName(QString::fromUtf8("snippetTreeView"));
    snippetTreeView->setEditTriggers(QAbstractItemView::EditKeyPressed);
    snippetTreeView->setDragEnabled(true);
    snippetTreeView->setDragDropMode(QAbstractItemView::DragDrop);
    snippetTreeView->setRootIsDecorated(true);
    snippetTreeView->setUniformRowHeights(true);
    snippetTreeView->setSortingEnabled(false);
    snippetTreeView->setHeaderHidden(true);

    verticalLayout->addWidget(snippetTreeView);

    leftDockWidget->setWidget(dockWidgetContents);
    MainWindowClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), leftDockWidget);

    menuBar->addAction(menu_File->menuAction());
    menuBar->addAction(menu_Snippet->menuAction());
    menuBar->addAction(menu_Add->menuAction());
    menu_Add->addAction(action_Main_category);
    menu_Add->addAction(action_Category);
    menu_Add->addAction(action_Snippet);
    menu_Snippet->addAction(action_Save);
    menu_Snippet->addAction(actionSave_all);
    menu_Snippet->addSeparator();
    menu_Snippet->addAction(action_Close);
    menu_Snippet->addAction(actionClos_e_all);
    menu_File->addAction(action_Save_2);
    menu_File->addAction(actionSave_snippets_as);
    menu_File->addSeparator();
    menu_File->addAction(action_Exit);

    retranslateUi(MainWindowClass);

    tabWidget->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
    MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "QSnippet", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    MainWindowClass->setToolTip(QString());
#endif // QT_NO_TOOLTIP

    action_Category->setText(QApplication::translate("MainWindowClass", "&Category...", 0, QApplication::UnicodeUTF8));
    action_Category->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+M", 0, QApplication::UnicodeUTF8));
    action_Snippet->setText(QApplication::translate("MainWindowClass", "&Snippet...", 0, QApplication::UnicodeUTF8));
    action_Snippet->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+N", 0, QApplication::UnicodeUTF8));
    action_Save->setText(QApplication::translate("MainWindowClass", "&Save", 0, QApplication::UnicodeUTF8));
    action_Save->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+S", 0, QApplication::UnicodeUTF8));
    actionSave_all->setText(QApplication::translate("MainWindowClass", "Save &all", 0, QApplication::UnicodeUTF8));
    actionSave_all->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+Shift+S", 0, QApplication::UnicodeUTF8));
    action_Close->setText(QApplication::translate("MainWindowClass", "&Close", 0, QApplication::UnicodeUTF8));
    action_Close->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+W", 0, QApplication::UnicodeUTF8));
    actionClos_e_all->setText(QApplication::translate("MainWindowClass", "Clos&e all", 0, QApplication::UnicodeUTF8));
    actionClos_e_all->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+Shift+W", 0, QApplication::UnicodeUTF8));
    action_Main_category->setText(QApplication::translate("MainWindowClass", "&Main category...", 0, QApplication::UnicodeUTF8));
    action_Main_category->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+Shift+M", 0, QApplication::UnicodeUTF8));
    action_Save_2->setText(QApplication::translate("MainWindowClass", "&Save snippets", 0, QApplication::UnicodeUTF8));
    action_Save_2->setShortcut(QApplication::translate("MainWindowClass", "Alt+S", 0, QApplication::UnicodeUTF8));
    actionSave_snippets_as->setText(QApplication::translate("MainWindowClass", "Save snippets &as...", 0, QApplication::UnicodeUTF8));
    actionSave_snippets_as->setShortcut(QApplication::translate("MainWindowClass", "Alt+Shift+S", 0, QApplication::UnicodeUTF8));
    action_Exit->setText(QApplication::translate("MainWindowClass", "&Exit", 0, QApplication::UnicodeUTF8));
    action_Exit->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(widget), QString());
    menu_Add->setTitle(QApplication::translate("MainWindowClass", "&Add", 0, QApplication::UnicodeUTF8));
    menu_Snippet->setTitle(QApplication::translate("MainWindowClass", "&Snippet", 0, QApplication::UnicodeUTF8));
    menu_File->setTitle(QApplication::translate("MainWindowClass", "&File", 0, QApplication::UnicodeUTF8));
    leftDockWidget->setWindowTitle(QApplication::translate("MainWindowClass", "Categories", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
