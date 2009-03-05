/********************************************************************************
** Form generated from reading ui file 'workmodedialog.ui'
**
** Created: Wed Mar 4 22:31:22 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_WORKMODEDIALOG_H
#define UI_WORKMODEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_WorkModeDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *searchLineEdit;
    QTreeView *treeView;

    void setupUi(QDialog *WorkModeDialog)
    {
    if (WorkModeDialog->objectName().isEmpty())
        WorkModeDialog->setObjectName(QString::fromUtf8("WorkModeDialog"));
    WorkModeDialog->setWindowModality(Qt::ApplicationModal);
    WorkModeDialog->resize(193, 233);
    WorkModeDialog->setModal(true);
    verticalLayout = new QVBoxLayout(WorkModeDialog);
    verticalLayout->setSpacing(2);
    verticalLayout->setMargin(2);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    searchLineEdit = new QLineEdit(WorkModeDialog);
    searchLineEdit->setObjectName(QString::fromUtf8("searchLineEdit"));

    verticalLayout->addWidget(searchLineEdit);

    treeView = new QTreeView(WorkModeDialog);
    treeView->setObjectName(QString::fromUtf8("treeView"));
    treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeView->setDragDropOverwriteMode(true);
    treeView->setDragDropMode(QAbstractItemView::DragOnly);
    treeView->setHeaderHidden(true);

    verticalLayout->addWidget(treeView);


    retranslateUi(WorkModeDialog);

    QMetaObject::connectSlotsByName(WorkModeDialog);
    } // setupUi

    void retranslateUi(QDialog *WorkModeDialog)
    {
    WorkModeDialog->setWindowTitle(QApplication::translate("WorkModeDialog", "QSnip Work Mode", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(WorkModeDialog);
    } // retranslateUi

};

namespace Ui {
    class WorkModeDialog: public Ui_WorkModeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WORKMODEDIALOG_H
