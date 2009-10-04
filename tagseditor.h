#ifndef TAGSEDITOR_H
#define TAGSEDITOR_H

#include <QtGui/QDialog>

namespace Ui {
    class TagsEditor;
}

class TagsEditor : public QDialog {
    Q_OBJECT
public:
    TagsEditor( QWidget *parent = 0 );
    ~TagsEditor();

    void setTags( QString *str );

protected:
    void changeEvent(QEvent *e);

private:
    Ui::TagsEditor *m_ui;

    QString *tags;

    void updateList();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

signals:
    void tagsModified();
};

#endif // TAGSEDITOR_H
