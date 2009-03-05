#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>

class TextEdit : public QTextEdit
{
public:
    TextEdit( QWidget* aparent = 0 );
    QSize sizeHint() const;
};

#endif // TEXTEDIT_H
