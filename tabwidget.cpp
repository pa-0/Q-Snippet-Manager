#include "tabwidget.h"
#include "ui_tabwidget.h"

TabWidget::TabWidget( QTabWidget *parent, const QString &name, const QString &code, const QString &desc, const QString &tags ) :
    m_ui(new Ui::TabWidget)
{
    m_ui->setupUi(this);

    m_ui->codeFindWidget->hide();
    m_ui->descFindWidget->hide();
    m_ui->questionWidget->hide();

    m_ui->codeFindWidget->setTextEdit( m_ui->codeTextEdit );
    m_ui->descFindWidget->setTextEdit( m_ui->descTextEdit );

    this->parent = parent;
    this->name = name;
    this->tags = tags;

    codeHidden = false;
    descHidden = false;

    // next line has to be here, because of the way how on_codeTextEdit_textChanged() slot works
    modified = true;
    m_ui->codeTextEdit->setPlainText( code );
    m_ui->descTextEdit->setPlainText( desc );
    modified = false;

    m_ui->saveButton->setEnabled( false );

    tagsEditor.setTags( &this->tags );
    connect( &tagsEditor, SIGNAL( tagsModified() ), this, SLOT( on_codeTextEdit_textChanged() ) );
}

TabWidget::~TabWidget()
{
    delete m_ui;
}

void TabWidget::changeEvent(QEvent *e)
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

void TabWidget::on_codeTextEdit_textChanged()
{
    if( modified )
        return;

    modified = true;
    parent->setTabText( parent->indexOf( this ), name + "*" );
    m_ui->saveButton->setEnabled( true );
}

void TabWidget::on_descTextEdit_textChanged()
{
    on_codeTextEdit_textChanged();
}

void TabWidget::on_saveButton_clicked()
{
    modified = false;
    parent->setTabText( parent->indexOf( this ), name );
    m_ui->saveButton->setEnabled( false );

    emit snippetSaved( name, m_ui->codeTextEdit->toPlainText(), m_ui->descTextEdit->toPlainText(),
                       tags );
}

void TabWidget::on_hideCodeButton_clicked()
{
    if( !codeHidden ) {
        m_ui->codeTextEdit->hide();
        m_ui->hideCodeButton->setText( tr( "Show" ) );
        if( descHidden )
            on_hideDescButton_clicked();
    } else {
        m_ui->codeTextEdit->show();
        m_ui->hideCodeButton->setText( tr( "Hide" ) );
    }

    codeHidden = !codeHidden;
}

void TabWidget::on_hideDescButton_clicked()
{
    if( !descHidden ) {
        m_ui->descTextEdit->hide();
        m_ui->hideDescButton->setText( tr( "Show" ) );
        if( codeHidden )
            on_hideCodeButton_clicked();
    } else {
        m_ui->descTextEdit->show();
        m_ui->hideDescButton->setText( tr( "Hide" ) );
    }

    descHidden = !descHidden;
}

void TabWidget::on_pushButton_4_clicked()
{
    QApplication::clipboard()->setText( m_ui->codeTextEdit->toPlainText() );
}

void TabWidget::on_pushButton_6_clicked()
{
    QApplication::clipboard()->setText( m_ui->descTextEdit->toPlainText() );
}

void TabWidget::on_saveAsSnippetButton_clicked()
{
    QString newName = QInputDialog::getText( 0, tr( "Save as new snippet..." ),
                                             tr( "Enter new snippet's name:" ) );
    if( newName.size() != 0 )
        emit snippetSaved( newName, m_ui->codeTextEdit->toPlainText(), m_ui->descTextEdit->toPlainText(),
                           tags );
}

void TabWidget::onCloseRequested() {
    if( modified ) {
        m_ui->questionWidget->show();
        m_ui->pushButton_3->setFocus();
    } else
        parent->removeTab( parent->indexOf( this ) );
}

void TabWidget::on_pushButton_2_clicked()
{
    m_ui->questionWidget->hide();
}

void TabWidget::on_pushButton_clicked()
{
    parent->removeTab( parent->indexOf( this ) );
}

void TabWidget::on_pushButton_3_clicked()
{
    emit snippetSaved( name, m_ui->codeTextEdit->toPlainText(), m_ui->descTextEdit->toPlainText(),
                           tags );
    parent->removeTab( parent->indexOf( this ) );
}

void TabWidget::onShowFindDialog()
{
    if( m_ui->descTextEdit->hasFocus() )
        m_ui->descFindWidget->show();
    else
        m_ui->codeFindWidget->show();
}

void TabWidget::on_tagsButton_clicked()
{
    tagsEditor.show();
}

void TabWidget::rename( QString newname ) {
    name = newname;
}

bool TabWidget::isModified() {
    return modified;
}
