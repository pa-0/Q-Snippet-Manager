#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass), model( this ), workModeDialog( this )
{
    ui->setupUi(this);
    ui->tabWidget->removeTab( 0 );
    ui->searchLineEdit->setFocus();

    // set up icons
    categoryIcon.addPixmap( style()->standardPixmap( QStyle::SP_DirClosedIcon ), QIcon::Normal, QIcon::Off );
    categoryIcon.addPixmap( style()->standardPixmap( QStyle::SP_DirOpenIcon ), QIcon::Normal, QIcon::On );
    snippetIcon.addPixmap( style()->standardPixmap( QStyle::SP_FileIcon ) );

    // set up actions
    ui->action_Save->setEnabled( false );
    ui->actionSave_all->setEnabled( false );
    ui->action_Close->setEnabled( false );
    ui->actionClos_e_all->setEnabled( false );
    ui->action_Cut->setEnabled( false );
    ui->action_Copy->setEnabled( false );
    // ui->action_Paste->setEnabled( false );
    ui->action_Undo->setEnabled( false );
    ui->action_Redo->setEnabled( false );
    // and their icons
    ui->actionSave_all->setIcon( style()->standardIcon( QStyle::SP_DialogSaveButton ) );
    ui->actionSave_snippets_as->setIcon( style()->standardIcon( QStyle::SP_DialogSaveButton ) );
    ui->action_Save->setIcon( style()->standardIcon( QStyle::SP_DialogSaveButton ) );
    ui->action_Save_2->setIcon( style()->standardIcon( QStyle::SP_DialogSaveButton ) );
    ui->action_Delete->setIcon( style()->standardIcon( QStyle::SP_TrashIcon ) );
    ui->action_Category->setIcon( style()->standardIcon( QStyle::SP_DirClosedIcon ) );
    ui->action_Main_category->setIcon( style()->standardIcon( QStyle::SP_DirClosedIcon ) );
    ui->action_Snippet->setIcon( style()->standardIcon( QStyle::SP_FileIcon ) );
    ui->action_Work->setIcon( style()->standardIcon( QStyle::SP_MediaPlay ) );

    // toolbar
    ui->mainToolBar->addAction( ui->action_Snippet );
    ui->mainToolBar->addAction( ui->action_Category );
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction( ui->action_Save );
    ui->mainToolBar->addAction( ui->actionSave_all );
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction( ui->action_Delete );
    ui->mainToolBar->addAction( ui->action_Undo );
    ui->mainToolBar->addAction( ui->action_Redo );
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction( ui->action_Cut );
    ui->mainToolBar->addAction( ui->action_Copy );
    ui->mainToolBar->addAction( ui->action_Paste );
    ui->mainToolBar->addAction( ui->actionPaste_as_new_snippet );
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction( ui->action_Work );

    // load snippets from snippets.xml and set up view
    loadSnippets();
    ui->snippetTreeView->setModel( &model );
    ui->snippetTreeView->expandAll();
    workModeDialog.m_ui->treeView->expandAll();

    // connections
    connect( ui->searchLineEdit, SIGNAL( textChanged(QString) ),
             this, SLOT(on_searchLineEdit_textChanged(QString)) );
    connect( &model, SIGNAL( itemChanged( QStandardItem* ) ),
             this, SLOT( updateSnippetsTitle( QStandardItem* ) ) );

    connect( ui->descTextEdit, SIGNAL(undoAvailable(bool)), ui->action_Undo, SLOT(setEnabled(bool)) );
    connect( ui->descTextEdit, SIGNAL(redoAvailable(bool)), ui->action_Redo, SLOT(setEnabled(bool)) );
    connect( ui->descTextEdit, SIGNAL( copyAvailable(bool) ), this, SLOT( onCopyAvailable(bool) ) );
    connect( qApp, SIGNAL( focusChanged(QWidget*,QWidget*) ), this, SLOT( focusChanged( QWidget*, QWidget* ) ) );
}

MainWindow::~MainWindow()
{
    on_actionClos_e_all_activated();
    saveSnippets();
    delete ui;
}

void MainWindow::on_action_Category_activated() {
    bool ok;
    QString name = QInputDialog::getText( this, tr( "New category..." ), tr( "Enter the new category name. "
                                                                             "It will be added as a child of selected category." ),
                                          QLineEdit::Normal, "", &ok );

    if( ok && !name.isEmpty() ) {
        // check if snippet's not selected
        QStandardItem* parent = model.itemFromIndex( ui->snippetTreeView->currentIndex() );
        Snippet* snippet = snippetForItem.value( parent );

        if( !parent ) {
            parent = model.invisibleRootItem();
            snippet = snippetForItem.value( parent );
        }
        if( !snippet->isCategory() )
            parent = parent->parent();

        // model actions
        QStandardItem* item = new QStandardItem( categoryIcon, name );
        ui->snippetTreeView->setExpanded( parent->index(), true );
        snippetForItem.insert( item, new Snippet( name, true ) );
        insertItem( item, parent );
    }
}

bool MainWindow::on_action_Snippet_activated() {
    bool ok;
    QString name = QInputDialog::getText( this, tr( "New snippet..." ), tr( "Enter the new snippet name. "
                                                                             "It will be added as a child of selected category." ),
                                          QLineEdit::Normal, "", &ok );

    if( ok && !name.isEmpty() ) {
        // check if snippet's not selected
        QStandardItem* parent = model.itemFromIndex( ui->snippetTreeView->currentIndex() );
        Snippet* snippet = snippetForItem.value( parent );

        if( !parent ) {
            QMessageBox::warning( this, tr( "Error inserting snippet" ), tr( "A snippet cannot be added to the root of the tree." ) );
            return false;
        }
        if( !snippet->isCategory() )
            parent = parent->parent();

        // model actions
        QStandardItem* item = new QStandardItem( snippetIcon, name );
        ui->snippetTreeView->setExpanded( parent->index(), true );

        snippet = new Snippet( name );
        snippet->setToolTip( createToolTip( snippet ) );
        snippetForItem.insert( item, snippet );
        insertItem( item, parent );

        // open snippet for editing
        on_snippetTreeView_activated( item->index() );
    } else
        return false;

    return true;
}

void MainWindow::on_action_Save_activated() {
    Snippet* snippet = findSnippetByTab( ui->tabWidget->currentIndex() );
    snippet->save( ui->descTextEdit->toPlainText() );
    snippet->setToolTip( createToolTip( snippet ) );
    snippet->setModified( false );
    ui->tabWidget->setTabText( snippet->tabNumber(), ui->tabWidget->tabText( snippet->tabNumber() ).remove( 0, 1 ) );

    // set up actions
    ui->action_Save->setEnabled( false );
    ui->actionSave_all->setEnabled( false );
    ui->action_Close->setEnabled( true );
    ui->actionClos_e_all->setEnabled( true );
}

void MainWindow::loadSnippets() {
    model.setColumnCount( 1 );

    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument domDocument;
    QFile file( "snippets.xml" );
    if( !domDocument.setContent( &file, true, &errorStr, &errorLine, &errorColumn) ) {
        QMessageBox::information(window(), tr("Snippets XML file"), tr("Parse error at line %1, column %2:\n%3")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));
        return;
    }

    snippetForItem.insert( model.invisibleRootItem(), new Snippet( domDocument.documentElement(), true ) );

    QDomElement child = domDocument.documentElement().firstChildElement("category");
    while ( !child.isNull() ) {
        parseCategoryElement( child, model.invisibleRootItem() );
        child = child.nextSiblingElement( "category" );
    }
}

void MainWindow::parseCategoryElement( const QDomElement &element, QStandardItem* parent ) {
    // create item with category's title
    QStandardItem* titleItem = new QStandardItem( categoryIcon, element.firstChildElement("title").text() );
    parent->setChild( parent->rowCount(), 0, titleItem );
    snippetForItem.insert( titleItem, new Snippet( element, true ) );

    // iterate over children of the element
    QDomElement child = element.firstChildElement();
    while( !child.isNull() ) {
        if( child.tagName() == "category" ) {
            parseCategoryElement( child, titleItem );
        } else if( child.tagName() == "snippet" ) {
            // create item with snippet's title
            QStandardItem* title = new QStandardItem( snippetIcon, child.firstChildElement("title").text() );

            // insert item and snippet into the hash and the model
            Snippet* snippet = new Snippet( child );
            snippet->setToolTip( createToolTip( snippet ) );
            snippetForItem.insert( title, snippet );
            titleItem->setChild( titleItem->rowCount(), 0, title );
        }
        child = child.nextSiblingElement();
    }
}

void MainWindow::on_snippetTreeView_activated( QModelIndex index ) {
    Snippet* snippet = snippetForItem.value( model.itemFromIndex( index ) );
    if( !snippet->isCategory() ) {
        if( snippet->isOpened() ) {
            ui->tabWidget->setCurrentIndex( snippet->tabNumber() );
            ui->descTextEdit->setEnabled( true );
            if( snippet->isModified() ) {
                ui->descTextEdit->setText( snippet->tempDescription() );
                ui->action_Save->setEnabled( true );
                ui->actionSave_all->setEnabled( true );
            } else
                ui->descTextEdit->setText( snippet->description() );
            return;
        } else if( ui->descDockWidget->isHidden() && !snippet->description().isEmpty() )
            ui->statusBar->showMessage( tr( "Description available." ) );

        TextEdit* edit = new TextEdit();
        connect( edit, SIGNAL(undoAvailable(bool)), ui->action_Undo, SLOT(setEnabled(bool)) );
        connect( edit, SIGNAL(redoAvailable(bool)), ui->action_Redo, SLOT(setEnabled(bool)) );
        connect( edit, SIGNAL( copyAvailable(bool) ), this, SLOT( onCopyAvailable(bool) ) );
        edit->setText( snippet->code() );
        connect( edit, SIGNAL( textChanged() ), this, SLOT( snippetsCodeModified() ) );
        snippet->setEdit( edit );
        snippet->setOpened();
        snippet->setTab( ui->tabWidget->addTab( edit, snippet->title() ) );
        ui->tabWidget->setCurrentIndex( snippet->tabNumber() );
        ui->descTextEdit->setText( snippet->description() );
        ui->descTextEdit->setEnabled( true );

        if( snippet->isModified() ) {
            ui->action_Save->setEnabled( true );
            ui->actionSave_all->setEnabled( true );
        }

        // set up actions
        ui->action_Close->setEnabled( true );
        ui->actionClos_e_all->setEnabled( true );
    }
}

void MainWindow::insertItem( QStandardItem* item, QStandardItem* parent ) {
    int i;
    if( snippetForItem.value( item )->isCategory() ) {
        for( i = 0; i < parent->rowCount() && snippetForItem.value( parent->child( i, 0 ) )->isCategory()
            && QString::compare( parent->child( i, 0 )->text(), item->text(), Qt::CaseInsensitive ) < 0; i++ ) ;
        parent->insertRow( i, item );
    } else {
        for( i = 0; i < parent->rowCount() &&
            ( ( snippetForItem.value( parent->child( i, 0 ) )->isCategory() ) ||
            ( !snippetForItem.value( parent->child( i, 0 ) )->isCategory() && QString::compare( parent->child( i, 0 )->text(), item->text(), Qt::CaseInsensitive ) < 0 ) ); i++ ) ;
        parent->insertRow( i, item );
    }
    ui->snippetTreeView->setExpanded( parent->index(), true );
}

void MainWindow::snippetsCodeModified() {
    Snippet* snippet = findSnippetByTab( ui->tabWidget->currentIndex() );
    if( !snippet->isModified() ) {
        snippet->setModified();
        ui->tabWidget->setTabText( snippet->tabNumber(),
                                   QString( "*" ) + ui->tabWidget->tabText( snippet->tabNumber() ) );
        ui->action_Save->setEnabled( true );
        ui->actionSave_all->setEnabled( true );
    }
}

Snippet* MainWindow::findSnippetByTab( int atab ) {
    QHash< QStandardItem*, Snippet* >::iterator i;
    for( i = snippetForItem.begin(); i != snippetForItem.end(); i++ )
        if( i.value()->isOpened() && i.value()->tabNumber() == atab )
            return i.value();
    return 0;
}

void MainWindow::saveSnippets( const QString& fileName ) {
    QFile file;
    if( fileName.isEmpty() )
        file.setFileName( "snippets.xml" );
    else
        file.setFileName( fileName );

    if( !file.open( QIODevice::WriteOnly | QIODevice::Truncate ) ) {
        QMessageBox::critical( this, tr( "Error" ), tr( "Cannot open file snippets.xml for writing." ) );
        return;
    }
    QString xml( "<snippets>\n" );
    parseModel( model.invisibleRootItem(), xml );
    xml += "</snippets>";

    QTextStream out( &file );
    out << xml;
}

void MainWindow::parseModel( QStandardItem* parent, QString& xml ) {
    for( int i = 0; i < parent->rowCount(); i++ ) {
        Snippet* snippet = snippetForItem.value( parent->child( i, 0 ) );
        if( snippet->isCategory() ) {
            xml += "<category><title>" + toValidXml( snippet->title() ) + "</title>\n";
            parseModel( parent->child( i, 0 ), xml );
            xml += "</category>\n";
        } else {
            xml += "<snippet><title>" + toValidXml( snippet->title() ) + "</title>\n";
            xml += "<code>" + toValidXml( snippet->code() ) + "</code>\n";
            xml += "<description>" + toValidXml( snippet->description() ) + "</description></snippet>\n";
        }
    }
}

void MainWindow::on_actionSave_all_activated() {
    QHash< QStandardItem*, Snippet* >::iterator i;
    int prevTab = ui->tabWidget->currentIndex();
    for( i = snippetForItem.begin(); i != snippetForItem.end(); i++ )
        if( i.value()->isOpened() ) {
            ui->tabWidget->setCurrentIndex( i.value()->tabNumber() );
            on_action_Save_activated();
        }
    ui->tabWidget->setCurrentIndex( prevTab );
}

void MainWindow::on_action_Close_activated() {
    Snippet* snippet = findSnippetByTab( ui->tabWidget->currentIndex() );

    if( snippet->isModified() ) {
        QMessageBox::StandardButton answer = QMessageBox::question( this, tr( "Save?" ),
                               tr( "Snippet %1 has been modified." ).arg( snippet->title() ),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save );
        if( answer == QMessageBox::Save )
            on_action_Save_activated();
        else if( answer != QMessageBox::Discard )
            return;
    }

    ui->tabWidget->removeTab( snippet->tabNumber() );
    snippet->setEdit( 0 );
    snippet->setOpened( false );

    restoreTabNumbers();

    snippet = findSnippetByTab( ui->tabWidget->currentIndex() );
    if( snippet )
        on_snippetTreeView_activated( snippetForItem.key( snippet )->index() );

    // set up actions
    if( !ui->tabWidget->count() ) {
        ui->action_Close->setEnabled( false );
        ui->actionClos_e_all->setEnabled( false);
        ui->descTextEdit->clear();
        ui->descTextEdit->setEnabled( false );
    }
}

void MainWindow::on_actionClos_e_all_activated() {
    QHash< QStandardItem*, Snippet* >::iterator i;
    for( i = snippetForItem.begin(); i != snippetForItem.end(); i++ )
        if( i.value()->isOpened() ) {
            ui->tabWidget->setCurrentIndex( i.value()->tabNumber() );
            on_action_Close_activated();
        }
}

void MainWindow::restoreTabNumbers() {
    QHash< QStandardItem*, Snippet* >::iterator i;
    for( i = snippetForItem.begin(); i != snippetForItem.end(); i++ )
        if( i.value()->isOpened() ) {
            for( int k = 0; k < ui->tabWidget->count(); k++ )
                if( i.value()->title() == ui->tabWidget->tabText( k )
                    || "*" + i.value()->title() == ui->tabWidget->tabText( k ) ) {
                    i.value()->setTab( k );
                    break;
                }
        }
}

void MainWindow::on_action_Main_category_activated() {
    bool ok;
    QString name = QInputDialog::getText( this, tr( "New category..." ), tr( "Enter the new language name. "
                                                                             "It will be added to the root node." ),
                                          QLineEdit::Normal, "", &ok );

    if( ok && !name.isEmpty() ) {
        QStandardItem* parent = model.invisibleRootItem();

        // model actions
        QStandardItem* item = new QStandardItem( categoryIcon, name );
        ui->snippetTreeView->setExpanded( parent->index(), true );
        snippetForItem.insert( item, new Snippet( name, true ) );
        insertItem( item, parent );
    }
}

void MainWindow::on_action_Save_2_activated() {
    saveSnippets();
}

void MainWindow::on_actionSave_snippets_as_activated() {
    QString fileName = QFileDialog::getSaveFileName( this, tr( "Save as..." ) );
    if( !fileName.isEmpty() )
        saveSnippets( fileName );
}

void MainWindow::on_action_Exit_activated() {
    on_actionClos_e_all_activated();
    qApp->quit();
}

void MainWindow::on_searchLineEdit_textChanged( QString searchString ) {
    // check if we're in edit or work mode
    if( workModeDialog.isVisible() )
        ui->searchLineEdit->setText( workModeDialog.m_ui->searchLineEdit->text() );
    else
        workModeDialog.m_ui->searchLineEdit->setText( ui->searchLineEdit->text() );

    workModeDialog.showSnippets();

    showAllSnippets( model.invisibleRootItem() );
    if( !searchString.isEmpty() )
        searchModelForString( searchString, model.invisibleRootItem() );

    if( !workModeDialog.underMouse() )
        QTimer::singleShot( 3000, &workModeDialog, SLOT( hideSnippets() ) );
}

// returns false of all children of a parent were hidden during parse
bool MainWindow::searchModelForString( const QString &searchString, QStandardItem* parent ) {
    int hiddenCount = 0;

    for( int i = 0; i < parent->rowCount(); i++ ) {
        QStandardItem* child = parent->child( i, 0 );
        Snippet* snippet = snippetForItem.value( child );

        if( ui->snippetTreeView->isRowHidden( i, parent->index() ) )
            hiddenCount++;
        else if( snippet->isCategory() && !child->text().contains( searchString, Qt::CaseInsensitive ) ) {
            if( child->rowCount() ) {
                if( !searchModelForString( searchString, child ) ) {
                    ui->snippetTreeView->setRowHidden( i, parent->index(), true );
                    workModeDialog.m_ui->treeView->setRowHidden( i, parent->index(), true );
                    hiddenCount++;
                }
            } else {
                ui->snippetTreeView->setRowHidden( i, parent->index(), true );
                workModeDialog.m_ui->treeView->setRowHidden( i, parent->index(), true );
                hiddenCount++;
            }
        } else if( !snippet->isCategory()
                   && ( !child->text().contains( searchString, Qt::CaseInsensitive ) && !snippet->code().contains( searchString, Qt::CaseInsensitive ) ) ) {
            ui->snippetTreeView->setRowHidden( i, parent->index(), true );
            workModeDialog.m_ui->treeView->setRowHidden( i, parent->index(), true );
            hiddenCount++;
        }
    }

    if( hiddenCount == parent->rowCount() )
        return false;

    return true;
}

void MainWindow::showAllSnippets( QStandardItem* parent ) {
    for( int i = 0; i < parent->rowCount(); i++ ) {
        ui->snippetTreeView->setRowHidden( i, parent->index(), false );
        workModeDialog.m_ui->treeView->setRowHidden( i, parent->index(), false );
        showAllSnippets( parent->child( i, 0 ) );
    }
}

void MainWindow::on_action_Delete_activated() {
    if( ui->snippetTreeView->currentIndex().isValid() ) {
        QStandardItem* item = model.itemFromIndex( ui->snippetTreeView->currentIndex() );

        QMessageBox::StandardButton answer = QMessageBox::question( this, tr( "Delete" ),
                                                                    tr( "Do you really want to delete element %1 and all its subelements?" ).arg( item->text() ),
                                                                    QMessageBox::Yes | QMessageBox::No );
        if( answer == QMessageBox::Yes )
            deleteChildItems( item );
    }
}

void MainWindow::deleteChildItems( QStandardItem* parent ) {
    while( parent->hasChildren() )
        deleteChildItems( parent->child( 0, 0 ) );

    if( snippetForItem.value( parent )->isOpened() ) {
        Snippet* prev = findSnippetByTab( ui->tabWidget->currentIndex() );
        ui->tabWidget->setCurrentIndex( snippetForItem.value( parent )->tabNumber() );
        snippetForItem.value( parent )->setModified( false );
        on_action_Close_activated();
        if( prev )
            ui->tabWidget->setCurrentIndex( prev->tabNumber() );
    }

    delete snippetForItem.value( parent );
    snippetForItem.remove( parent );
    if( !parent->parent() )
        model.removeRow( parent->index().row() );
    else
        parent->parent()->removeRow( parent->index().row() );
}

void MainWindow::on_action_Work_activated() {
    this->hide();
    setToolTips();
    workModeDialog.show();

    ui->action_Normal->setEnabled( true );
    ui->action_Work->setEnabled( false );

    mainWindowGeometry = this->saveGeometry();
    workModeDialog.restoreGeometry( workModeDialogGeometry );

    qApp->setActiveWindow( &workModeDialog );
    workModeDialog.m_ui->searchLineEdit->setFocus();
}

void MainWindow::on_action_Normal_activated() {
    resetToolTips();
    this->show();
    workModeDialog.hide();

    ui->action_Normal->setEnabled( false );
    ui->action_Work->setEnabled( true );

    workModeDialogGeometry = workModeDialog.saveGeometry();
    this->restoreGeometry( mainWindowGeometry );
}

void MainWindow::on_WorkModeDialog_finished( int ) {
    // ignore the result
   on_action_Normal_activated();
}

void MainWindow::updateSnippetsTitle( QStandardItem* item ) {
    Snippet* snippet = snippetForItem.value( item );
    snippet->setTitle( item->text() );
}

QString MainWindow::toValidXml( QString string ) {
    QString temp( string );

    temp.replace( "&", "&amp;" );
    temp.replace( "<", "&lt;" );
    temp.replace( ">", "&gt;" );

    return temp;
}

void MainWindow::on_actionHide_categories_activated() {
    if( ui->leftDockWidget->isVisible() )
        ui->leftDockWidget->hide();
    else
        ui->leftDockWidget->show();
}

void MainWindow::on_actionHide_description_activated() {
    if( ui->descDockWidget->isVisible() )
        ui->descDockWidget->hide();
    else
        ui->descDockWidget->show();
}

void MainWindow::on_descTextEdit_textChanged() {
    if( ui->descTextEdit->toPlainText().isEmpty() )
        return;

    findSnippetByTab( ui->tabWidget->currentIndex() )->setTempDescription( ui->descTextEdit->toPlainText() );

    if( findSnippetByTab( ui->tabWidget->currentIndex() )->description() != ui->descTextEdit->toPlainText() )
        snippetsCodeModified();
}

void MainWindow::on_tabWidget_currentChanged( int index ) {
    QStandardItem* item = snippetForItem.key( findSnippetByTab( index ) );
    if( item ) {
        on_snippetTreeView_activated( item->index() );
    }
}

void MainWindow::on_action_About_activated() {
    QMessageBox::about( this, tr( "About QSnippetManager" ),
                        tr( "Author: mslupny ( mslupny@gmail.com )\n"
                            "Use http://repo.or.cz/w/qsnippetsmanager.git if you'd like to contribute.\n"
                            "License: GPLv3." ) );
}

void MainWindow::on_actionAbout_Qt_activated() {
    QMessageBox::aboutQt( this, tr( "About Qt" ) );
}

QString MainWindow::createToolTip( const Snippet* snippet ) {
    QString toolTip;
    if( !snippet->description().isEmpty() ) {
        toolTip += tr( "<b>Description:<br></b>" ) + toValidXml( snippet->description() );
        if( !snippet->code().isEmpty() )
            toolTip += "<br><br>";
    }

    if( !snippet->code().isEmpty() ) {
        toolTip += tr( "<b>Code:</b><br>" );
        QString temp( snippet->code() );
        bool changed = false;
        while( temp.count( '\n' ) > 40 ) {
            temp.truncate( temp.size() / 2 );
            changed = true;
        }
        toolTip += toValidXml( temp );
        if( changed )
            toolTip += "...";
    }

    toolTip.replace( '\n', "<br>" );
    toolTip.replace( '\t', "&nbsp;&nbsp;&nbsp;&nbsp;" );

    return toolTip;
}

void MainWindow::resetToolTips() {
    for( QHash< QStandardItem*, Snippet* >::const_iterator i = snippetForItem.constBegin();
         i != snippetForItem.constEnd(); ++i )
        i.key()->setToolTip( "" );
}

void MainWindow::setToolTips() {
    for( QHash< QStandardItem*, Snippet* >::iterator i = snippetForItem.begin();
         i != snippetForItem.end(); ++i )
        if( !i.value()->isCategory() )
            i.key()->setToolTip( i.value()->toolTip() );
}

void MainWindow::on_action_Find_activated() {
    ui->searchLineEdit->setFocus();
}

void MainWindow::on_action_Cut_activated() {
    if( ui->descTextEdit->hasFocus() )
        ui->descTextEdit->cut();
    else {
        Snippet* snippet = findSnippetByTab( ui->tabWidget->currentIndex() );
        if( snippet )
            snippet->edit()->cut();
    }
}

void MainWindow::on_action_Copy_activated() {
    if( ui->descTextEdit->hasFocus() )
        ui->descTextEdit->copy();
    else {
        Snippet* snippet = findSnippetByTab( ui->tabWidget->currentIndex() );
        if( snippet )
            snippet->edit()->copy();
    }
}

void MainWindow::on_action_Paste_activated() {
    if( ui->descTextEdit->hasFocus() ) {
        ui->descTextEdit->paste();
    }
    else {
        Snippet* snippet = findSnippetByTab( ui->tabWidget->currentIndex() );
        if( snippet )
            snippet->edit()->paste();
    }
}

void MainWindow::onCopyAvailable( bool atrue ) {
    ui->action_Cut->setEnabled( atrue );
    ui->action_Copy->setEnabled( atrue );
}

void MainWindow::focusChanged( QWidget*, QWidget* now ) {
    ui->action_Cut->setEnabled( false );
    ui->action_Copy->setEnabled( false );
    QTextEdit* edit = dynamic_cast< TextEdit* >( now );

    Snippet* snippet = findSnippetByTab( ui->tabWidget->currentIndex() );
    if( snippet ) {
        if( edit == snippet->edit() ) {
            ui->action_Undo->setEnabled( edit->document()->isUndoAvailable() );
            ui->action_Redo->setEnabled( edit->document()->isRedoAvailable() );
        } else if( edit == ui->descTextEdit ) {
            ui->action_Undo->setEnabled( edit->document()->isUndoAvailable() );
            ui->action_Redo->setEnabled( edit->document()->isRedoAvailable() );
        }
    }
}

void MainWindow::on_actionPaste_as_new_snippet_activated() {
    if( this->on_action_Snippet_activated() )
        findSnippetByTab( ui->tabWidget->currentIndex() )->edit()->setPlainText( qApp->clipboard()->text() );
}
