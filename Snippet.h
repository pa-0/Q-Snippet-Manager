#ifndef SNIPPET_H
#define SNIPPET_H

#include <QDomElement>

#include "textedit.h"

class Snippet {
public:
    Snippet( const QDomElement& elem, bool isCat = false )
            : mod( false ), op( false ), ed( 0 ), cat( isCat ) {
        tit = elem.firstChildElement( "title" ).text();
        cd = elem.firstChildElement( "code" ).text();
        desc = elem.firstChildElement( "description" ).text();
    }
    Snippet( const QString& t, bool isCat = false )
            : mod( false ), op( false ), ed( 0 ), tit( t ), cd( "" ), desc( "" ), cat( isCat ) {}
    Snippet( const Snippet& snippet )
            : mod( snippet.mod ), op( snippet.mod ), ed( snippet.ed ), tit( snippet.tit ), cd( snippet.cd ),
            desc( snippet.desc ), tab( snippet.tab ), cat( snippet.cat ) {}
    ~Snippet() {}
    QString code() const { return cd; }
    QString description() const { return desc; }
    TextEdit* edit() const { return ed; }
    bool isCategory() { return cat; }
    bool isModified() { return mod; }
    bool isOpened() { return op; }
    void save( const QString& adesc = "" ) {
        cd = ed->toPlainText();
        desc = adesc;
    }
    void setDescription( const QString& adesc ) { desc = adesc; }
    void setEdit( TextEdit* edit ) { ed = edit; }
    void setOpened( bool val = true ) { op = val; }
    void setModified( bool val = true ) { mod = val; }
    void setTab( const int& t ) { tab = t; }
    void setTempDescription( const QString& atemp ) { tempDesc = atemp; }
    void setTitle( const QString& atitle ) { tit = atitle; }
    void setToolTip( const QString& atooltip ) { tooltip = atooltip; }
    int tabNumber() { return tab; }
    QString tempDescription() const { return tempDesc; }
    QString title() const { return tit; }
    QString toolTip() const { return tooltip; }
private:
    bool mod;
    bool op;
    TextEdit* ed;
    QString tit;
    QString cd;
    QString desc;
    QString tempDesc;
    QString tooltip;
    int tab;
    bool cat;
};

#endif // SNIPPET_H
