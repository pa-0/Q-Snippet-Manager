#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QtGui/QWidget>
#include <QAction>
#include <QClipboard>
#include <QInputDialog>
#include <QString>
#include <QTabWidget>

#include "tagseditor.h"

namespace Ui {
    class TabWidget;
}

class TabWidget : public QWidget {
    Q_OBJECT
public:
    TabWidget( QTabWidget *parent, const QString &name, const QString &code, const QString &desc, const QString &tags );
    ~TabWidget();

    bool isModified();
    void onCloseRequested();
    void onShowFindDialog();

public slots:
    void rename( QString newname );

protected:
    void changeEvent(QEvent *e);

private:
    Ui::TabWidget *m_ui;

    TagsEditor tagsEditor;

    bool codeHidden;
    bool descHidden;
    bool modified;
    QString name, tags;
    QTabWidget *parent;

private slots:
    void on_tagsButton_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_saveAsSnippetButton_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_4_clicked();
    void on_hideDescButton_clicked();
    void on_hideCodeButton_clicked();
    void on_saveButton_clicked();
    void on_descTextEdit_textChanged();
    void on_codeTextEdit_textChanged();

signals:
    void snippetSaved( const QString &name, const QString &code, const QString &desc, const QString &tags );
};

#endif // TABWIDGET_H
