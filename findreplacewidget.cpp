#include "findreplacewidget.h"
#include "ui_findreplacewidget.h"

FindReplaceWidget::FindReplaceWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::FindReplaceWidget)
{
    m_ui->setupUi(this);

    m_ui->hideButton->setIcon( QIcon( ":/icons/dialog_close.png" ) );

    connect( m_ui->findLineEdit, SIGNAL( lostFocusByTab() ),
             m_ui->replaceLineEdit, SLOT( setFocus() ) );
    connect( m_ui->replaceLineEdit, SIGNAL( lostFocusByTab() ),
             m_ui->findLineEdit, SLOT( setFocus() ) );
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
    removeHighlight();
    firstRun = true;

    textEdit->setFocus();
    hide();
}

void FindReplaceWidget::setTextEdit( QTextEdit *te ) {
    textEdit = te;

    firstRun = true;

    connect( textEdit, SIGNAL( selectionChanged() ), this, SLOT( resetSearch() ) );
}

void FindReplaceWidget::on_findLineEdit_returnPressed()
{
    textEdit->blockSignals( true );

    if( m_ui->findLineEdit->text().isEmpty() )
        return;

    QString text = textEdit->toPlainText();
    QString searchStr = m_ui->findLineEdit->text();
    QString selectedText = textEdit->textCursor().selectedText();

    if( firstRun ) {
        if( selectedText.isEmpty() || selectedText == searchStr ) {
            entireDocument = true;
            blockBegPos = 0;
            blockEndPos = text.size();

        } else {
            entireDocument = false;
            blockBegPos = textEdit->textCursor().selectionStart();
            blockEndPos = textEdit->textCursor().selectionEnd();
        }
        searchFromPos = blockBegPos;

        firstRun = false;
    }

    if( !text.contains( searchStr ) )
        return;

    // extra selections
    removeHighlight();
    if( !entireDocument ) {
        highlightSelectedBlock();
        QString block = text.mid( blockBegPos, blockEndPos - blockBegPos );

        if( !block.contains( searchStr ) ) {
            QTextCursor cursor = textEdit->textCursor();
            cursor.setPosition( blockEndPos );
            textEdit->setTextCursor( cursor );

            return;
        }
    }
    highlightSearchStr( searchStr, 130 );

    // select string
    searchFromPos = text.indexOf( searchStr, searchFromPos );
    if( searchFromPos == -1 || searchFromPos > blockEndPos - searchStr.size() ) {
        searchFromPos = blockBegPos;
        on_findLineEdit_returnPressed();
    } else {
        QTextCursor cursor = textEdit->textCursor();
        cursor.setPosition( searchFromPos );
        cursor.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor, searchStr.size() );
        searchFromPos += searchStr.size();

        textEdit->setTextCursor( cursor );
    }

    textEdit->blockSignals( false );
}

void FindReplaceWidget::showEvent( QShowEvent * ) {
    m_ui->findLineEdit->setFocus();
}

void FindReplaceWidget::on_findLineEdit_textChanged( QString str )
{
    removeHighlight();
    highlightSearchStr( str );
    firstRun = true;
}

void FindReplaceWidget::resetSearch() {
    firstRun = true;
}

void FindReplaceWidget::on_replaceLineEdit_returnPressed()
{
    textEdit->blockSignals( true );

    if( m_ui->replaceLineEdit->text().isEmpty() )
        return;

    if( m_ui->findLineEdit->text().isEmpty() ) {
        m_ui->findLineEdit->setFocus();
        return;
    }

    if( firstRun ) {
        on_findLineEdit_returnPressed();
        return;
    }

    textEdit->blockSignals( false );

    QTextCursor textCursor = textEdit->textCursor();
    if( textCursor.selectedText().isEmpty() )
        return;

    textEdit->blockSignals( false );

    textCursor.removeSelectedText();
    textCursor.insertText( m_ui->replaceLineEdit->text() );
    textEdit->setTextCursor( textCursor );

    int delta = m_ui->replaceLineEdit->text().size() - m_ui->findLineEdit->text().size();
    blockEndPos += delta;

    firstRun = false;

    on_findLineEdit_returnPressed();
}

void FindReplaceWidget::highlightSearchStr( const QString &searchStr, int light ) {
    if( searchStr.size() == 0 )
            return;

    QList< QTextEdit::ExtraSelection > extraSelections = textEdit->extraSelections();

    QString text = textEdit->toPlainText();

    int pos = 0;
    while( ( pos = text.indexOf( searchStr, pos ) ) != -1 && pos <= text.size() - searchStr.size() ) {
        QTextEdit::ExtraSelection es;
        es.cursor = textEdit->textCursor();
        es.cursor.clearSelection();
        es.cursor.setPosition( pos, QTextCursor::MoveAnchor );
        es.cursor.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor, searchStr.size() );
        es.format.setBackground( QColor( Qt::yellow ).lighter( light ) );

        extraSelections.append( es );

        pos += searchStr.size();
    }

    textEdit->setExtraSelections( extraSelections );
}

void FindReplaceWidget::removeHighlight() {
    textEdit->setExtraSelections( QList< QTextEdit::ExtraSelection >() );
}

void FindReplaceWidget::highlightSelectedBlock() {
    QList< QTextEdit::ExtraSelection > extraSelections = textEdit->extraSelections();

    QTextEdit::ExtraSelection es;
    es.cursor = textEdit->textCursor();
    es.cursor.setPosition( blockBegPos );
    es.cursor.setPosition( blockEndPos, QTextCursor::KeepAnchor );
    es.format.setBackground( QColor( Qt::gray ).lighter( 141 ) );
    extraSelections.append( es );

    textEdit->setExtraSelections( extraSelections );
}
