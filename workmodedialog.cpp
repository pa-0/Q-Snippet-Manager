#include "mainwindow.h"
#include "workmodedialog.h"
#include "ui_workmodedialog.h"

WorkModeDialog::WorkModeDialog(QWidget *parent) :
    QDialog( parent ),
    m_ui(new Ui::WorkModeDialog)
{
    m_ui->setupUi(this);
    setWindowFlags( Qt::WindowStaysOnTopHint | Qt::Tool );

    p = ( MainWindow* )parent;

    m_ui->treeView->setModel( &p->model );
    m_ui->treeView->expandAll();

    connect( m_ui->searchLineEdit, SIGNAL( textChanged( QString ) ),
             p, SLOT( on_searchLineEdit_textChanged( QString ) ) );

    toEditMode = new QAction( this );
    toEditMode->setShortcut( QKeySequence( "F5" ) );
    toEditMode->setEnabled( true );

    hideSnippets();
}

WorkModeDialog::~WorkModeDialog()
{
    delete m_ui;
}

void WorkModeDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void WorkModeDialog::hideSnippets() {
    m_ui->treeView->hide();
    this->setFixedHeight( m_ui->searchLineEdit->height() + 2 );
}

void WorkModeDialog::showSnippets() {
    m_ui->treeView->show();
    this->setFixedHeight( m_ui->searchLineEdit->height() + 250 );
}

void WorkModeDialog::enterEvent( QEvent* ) {
    showSnippets();
}

void WorkModeDialog::leaveEvent( QEvent* ) {
    hideSnippets();
}
