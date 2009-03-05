#include "Snippet.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_workmodedialog.h"

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

void MainWindow::on_action_Snippet_activated() {
    bool ok;
    QString name = QInputDialog::getText( this, tr( "New snippet..." ), tr( "Enter the new snippet name. "
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
        QStandardItem* item = new QStandardItem( snippetIcon, name );
        ui->snippetTreeView->setExpanded( parent->index(), true );

        snippet = new Snippet( name );
        snippetForItem.insert( item, snippet );
        insertItem( item, parent );

        // open snippet for editing
        on_snippetTreeView_activated( item->index() );
    }
}

void MainWindow::on_action_Save_activated() {
    Snippet* snippet = findSnippetByTab( ui->tabWidget->currentIndex() );
    snippet->save();
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
            return;
        }
        QPlainTextEdit* edit = new QPlainTextEdit( snippet->code() );
        connect( edit, SIGNAL( textChanged() ), this, SLOT( snippetsCodeModified() ) );
        snippet->setEdit( edit );
        snippet->setOpened();
        snippet->setTab( ui->tabWidget->addTab( edit, snippet->title() ) );
        ui->tabWidget->setCurrentIndex( snippet->tabNumber() );

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

    if( !file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ) ) {
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
            xml += "<category><title>" + snippet->title() + "</title>\n";
            parseModel( parent->child( i, 0 ), xml );
            xml += "</category>\n";
        } else
            xml += "<snippet><title>" + snippet->title() + "</title>\n<code>" + snippet->code() + "</code>\n</snippet>\n";
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

    // set up actions
    if( ui->tabWidget->count() ) {
        ui->action_Close->setEnabled( true );
        ui->actionClos_e_all->setEnabled( true );

        if( findSnippetByTab( ui->tabWidget->currentIndex() )->isModified() ) {
            ui->action_Save->setEnabled( true );
            ui->actionSave_all->setEnabled( true );
        }
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
                if( i.value()->title() == ui->tabWidget->tabText( k ) ) {
                    i.value()->setTab( k );
                    break;
                }
        }
}

void MainWindow::on_action_Main_category_activated() {
    bool ok;
    QString name = QInputDialog::getText( this, tr( "New category..." ), tr( "Enter the new category name. "
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

    delete snippetForItem.value( parent );
    snippetForItem.remove( parent );
    if( !parent->parent() )
        model.removeRow( parent->index().row() );
    else
        parent->parent()->removeRow( parent->index().row() );
}

void MainWindow::on_action_Work_activated() {
    this->hide();
    workModeDialog.show();

    mainWindowGeometry = this->saveGeometry();
    workModeDialog.restoreGeometry( workModeDialogGeometry );

    workModeDialog.setFocus();
}

void MainWindow::on_action_Normal_activated() {
    this->show();
    workModeDialog.hide();

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
