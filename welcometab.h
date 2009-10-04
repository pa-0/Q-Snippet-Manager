#ifndef WELCOMETAB_H
#define WELCOMETAB_H

#include <QtGui/QWidget>

namespace Ui {
    class WelcomeTab;
}

class WelcomeTab : public QWidget {
    Q_OBJECT
public:
    WelcomeTab(QWidget *parent = 0);
    ~WelcomeTab();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WelcomeTab *m_ui;
};

#endif // WELCOMETAB_H
