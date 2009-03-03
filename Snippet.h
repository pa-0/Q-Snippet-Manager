#ifndef SNIPPET_H
#define SNIPPET_H

#include <QDomElement>
#include <QPlainTextEdit>

class Snippet {
public:
    Snippet( const QDomElement& elem, bool isCat = false )
            : mod( false ), op( false ), ed( 0 ), cat( isCat ) {
        tit = elem.firstChildElement( "title" ).text();
        cd = elem.firstChildElement( "code" ).text();
    }
    Snippet( const QString& t, bool isCat = false )
            : mod( false ), op( false ), ed( 0 ), tit( t ), cd( "" ), cat( isCat ) {}
    Snippet( const Snippet& snippet )
            : mod( snippet.mod ), op( snippet.mod ), ed( snippet.ed ), tit( snippet.tit ), cd( snippet.cd ),
            tab( snippet.tab ), cat( snippet.cat ) {}
    ~Snippet() { qDebug( "oops" ); }
    QString code() { return cd; }
    QPlainTextEdit* edit() { return ed; }
    bool isCategory() { return cat; }
    bool isModified() { return mod; }
    bool isOpened() { return op; }
    void save() { cd = ed->toPlainText(); }
    void setEdit( QPlainTextEdit* edit ) { ed = edit; }
    void setOpened( bool val = true ) { op = val; }
    void setModified( bool val = true ) { mod = val; }
    void setTab( const int& t ) { tab = t; }
    int tabNumber() { return tab; }
    QString title() { return tit; }
private:
    bool mod;
    bool op;
    QPlainTextEdit* ed;
    QString tit;
    QString cd;
    int tab;
    bool cat;
};

// Q_DECLARE_METATYPE( Snippet );

#endif // SNIPPET_H
