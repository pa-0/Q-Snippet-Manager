#include "workmodedialog.h"
#include "ui_workmodedialog.h"

WorkModeDialog::WorkModeDialog( QWidget *parent, QListWidget *pListWidget, QLineEdit *pLineEdit, QSettings &set ) :
    QDialog(parent),
    m_ui(new Ui::WorkModeDialog),    
    parentLineEdit( pLineEdit ),
    parentListWidget( pListWidget ),
    config( set )
{
    m_ui->setupUi(this);

    connect( m_ui->lineEdit, SIGNAL( mouseOver() ), this, SLOT( showList() ) );

    QSize oldSize = config.value( "WorkModeDialog/size" ).toSize();
    QPoint oldPos = config.value( "WorkModeDialog/pos" ).toPoint();
    this->setGeometry( oldPos.x(), oldPos.y(), oldSize.width(), 400 );
}

WorkModeDialog::~WorkModeDialog()
{
    config.setValue( "WorkModeDialog/size", QSize( this->width(), 0 ) );
    config.setValue( "WorkModeDialog/pos", pos() );
    delete m_ui;
}

void WorkModeDialog::changeEvent(QEvent *e)
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

void WorkModeDialog::showList() {
    setFixedHeight( 400 );
    m_ui->listWidget->show();
}

void WorkModeDialog::hideList() {
    m_ui->listWidget->hide();
    setFixedHeight( 20 );
}

void WorkModeDialog::enterEvent( QEvent * ) {
    showList();
}

void WorkModeDialog::leaveEvent( QEvent * ) {
    hideList();
}

void WorkModeDialog::showEvent( QShowEvent * ) {
    hideList();
}

void WorkModeDialog::onFilterApplied( QStringList shownSnippets, QStringList toolTips, QStringList codeList ) {
    m_ui->listWidget->clear();
    m_ui->listWidget->addItems( shownSnippets );

    for( int i = 0; i < toolTips.size(); ++i ) {
        m_ui->listWidget->item( i )->setToolTip( toolTips.at( i ) );
        m_ui->listWidget->item( i )->setData( Qt::UserRole, codeList.at( i ) );
        qDebug( codeList.at( i ).toAscii() );
    }
}

void WorkModeDialog::on_lineEdit_textChanged( QString searchStr )
{
    emit searchStringEntered( searchStr );
}
