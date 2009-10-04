#include "findreplacewidget.h"
#include "ui_findreplacewidget.h"

FindReplaceWidget::FindReplaceWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::FindReplaceWidget)
{
    m_ui->setupUi(this);

    m_ui->hideButton->setIcon( QIcon( ":/icons/dialog_close.png" ) );
}

FindReplaceWidget::~FindReplaceWidget()
{
    delete m_ui;
}

void FindReplaceWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void FindReplaceWidget::on_hideButton_clicked()
{
    textEdit->setFocus();
    hide();
}

void FindReplaceWidget::setTextEdit( QTextEdit *te ) {
    textEdit = te;
}

void FindReplaceWidget::on_findLineEdit_returnPressed()
{
    QString searchStr = m_ui->findLineEdit->text();
    if( searchStr.isEmpty() )
        return;

    QString selectedText = textEdit->textCursor().selectedText();

    if( selectedText.isEmpty() || selectedText == searchStr ) {
        QList< QTextEdit::ExtraSelection > extraSelections;

        QString text = textEdit->toPlainText();
        int pos = 0;

        while( ( pos = text.indexOf( searchStr, pos ) ) != -1 ) {
            QTextEdit::ExtraSelection es;
            es.cursor = QTextCursor( textEdit->document() );
            es.cursor.clearSelection();
            es.cursor.setPosition( pos, QTextCursor::MoveAnchor );
            es.cursor.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor, searchStr.size() );
            es.format.setBackground( QColor( Qt::yellow ).lighter( 160 ) );

            extraSelections.append( es );

            pos += searchStr.size();
        }

        textEdit->setExtraSelections( extraSelections );
        textEdit->find( searchStr );
    }
}

void FindReplaceWidget::showEvent( QShowEvent * ) {
    m_ui->findLineEdit->setFocus();
}
