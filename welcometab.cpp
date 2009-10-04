#include "welcometab.h"
#include "ui_welcometab.h"

WelcomeTab::WelcomeTab(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::WelcomeTab)
{
    m_ui->setupUi(this);
}

WelcomeTab::~WelcomeTab()
{
    delete m_ui;
}

void WelcomeTab::changeEvent(QEvent *e)
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
