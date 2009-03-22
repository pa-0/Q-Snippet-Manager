#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QFont>
#include <QTextEdit>

class TextEdit : public QTextEdit
{
public:
    TextEdit( QWidget* aparent = 0 );
    QSize sizeHint() const;

    QFont font;
};

#endif // TEXTEDIT_H
