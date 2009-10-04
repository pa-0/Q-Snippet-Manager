#include "tagseditor.h"
#include "ui_tagseditor.h"

TagsEditor::TagsEditor( QWidget *parent ) :
    QDialog(parent),
    m_ui(new Ui::TagsEditor)
{
    m_ui->setupUi(this);
}

TagsEditor::~TagsEditor()
{
    delete m_ui;
}

void TagsEditor::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void TagsEditor::setTags( QString *t ) {
    tags = t;

    updateList();
}

void TagsEditor::updateList() {
    QStringList tagList = tags->split( ';', QString::SkipEmptyParts );
    tagList.removeDuplicates();

    m_ui->listWidget->clear();
    for( int i = 0; i < tagList.size(); ++i )
        m_ui->listWidget->insertItem( m_ui->listWidget->count(), tagList.at( i ) );

    *tags = tagList.join( ";" );
}

void TagsEditor::on_pushButton_clicked()
{
    QString tag = m_ui->lineEdit->text();
    tag.replace( " ", ";" );

    tags->append( ";" + tag.remove( ";" ) );

    updateList();

    emit tagsModified();

    m_ui->lineEdit->setText( QString() );
}

void TagsEditor::on_pushButton_2_clicked()
{
    if( !m_ui->listWidget->currentItem() )
        return;

    QStringList tagList = tags->split( ';', QString::SkipEmptyParts );
    tagList.removeAll( m_ui->listWidget->currentItem()->text() );
    *tags = tagList.join( ";" );
    updateList();

    emit tagsModified();
}
