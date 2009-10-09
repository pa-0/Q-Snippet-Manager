#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      config( QSettings::UserScope, "QSnippetsManagerSettings" ),
      snippets( QSettings::IniFormat, QSettings::UserScope, "QSnippetsManager" )
{
    ui->setupUi(this);

    workModeDialog = new WorkModeDialog( this, ui->snippetsList, ui->searchLineEdit, config );
    connect( workModeDialog, SIGNAL( finished( int ) ), this, SLOT( show() ) );
    connect( this, SIGNAL( filterApplied( QStringList, QStringList, QStringList ) ),
             workModeDialog, SLOT( onFilterApplied( QStringList, QStringList, QStringList ) ) );
    connect( workModeDialog, SIGNAL( searchStringEntered( QString ) ),
             ui->searchLineEdit, SLOT( setText( QString ) ) );
    connect( ui->tagsWidget, SIGNAL( tagSelectionChanged() ), this, SLOT( applySearchFilter() ) );

    qsrand( time( 0 ) );

    setupActions();

    setupAll();

    QSize oldSize = config.value( "size" ).toSize();
    QPoint oldPos = config.value( "pos" ).toPoint();
    this->setGeometry( oldPos.x(), oldPos.y(), oldSize.width(), oldSize.height() );
}

MainWindow::~MainWindow()
{
    config.setValue( "size", size() );
    config.setValue( "pos", pos() );

    delete workModeDialog;
    delete ui;
}

void MainWindow::setupActions() {
    newSnippetAction = new QAction( tr( "New snippet" ), this );
    newSnippetAction->setShortcut( QKeySequence( "Ctrl+N" ) );
    newSnippetAction->setIcon( QIcon( ":/icons/filenew.png" ) );
    connect( newSnippetAction, SIGNAL( triggered() ), this, SLOT( newSnippet() ) );
    ui->mainToolBar->addAction( newSnippetAction );

    deleteSnippetAction = new QAction( tr( "Delete snippet" ), this );
    deleteSnippetAction->setShortcut( QKeySequence( "Delete" ) );
    deleteSnippetAction->setIcon( QIcon( ":/icons/edit_delete_shred.png" ) );
    connect( deleteSnippetAction, SIGNAL( triggered() ), this, SLOT( deleteSnippet() ) );
    ui->mainToolBar->addAction( deleteSnippetAction );

    closeSnippetAction = new QAction( tr( "Close snippet" ), this );
    closeSnippetAction->setShortcut( QKeySequence( "Ctrl+W" ) );
    closeSnippetAction->setIcon( QIcon( ":/icons/fileclose.png" ) );
    connect( closeSnippetAction, SIGNAL( triggered() ), this, SLOT( onCloseSnippetRequested() ) );
    ui->mainToolBar->addAction( closeSnippetAction );

    ui->mainToolBar->addSeparator();

    renameSnippetAction = new QAction( tr( "Rename..." ), this );
    renameSnippetAction->setShortcut( QKeySequence( "Ctrl+R" ) );
    renameSnippetAction->setIcon( QIcon( ":/icons/edit_rename.png" ) );
    connect( renameSnippetAction, SIGNAL( triggered() ), this, SLOT( renameSnippet() ) );
    ui->mainToolBar->addAction( renameSnippetAction );

    ui->mainToolBar->addSeparator();

    findReplaceAction = new QAction( tr( "Find/Replace" ), this );
    findReplaceAction->setShortcut( QKeySequence( "Ctrl+F" ) );
    findReplaceAction->setIcon( QIcon( ":/icons/edit_find.png" ) );
    connect( findReplaceAction, SIGNAL( triggered() ), this, SLOT( showFindReplaceWidget() ) );
    ui->mainToolBar->addAction( findReplaceAction );

    ui->mainToolBar->addSeparator();

    workModeAction = new QAction( tr( "Work Mode" ), this );
    workModeAction->setShortcut( QKeySequence( "F5" ) );
    workModeAction->setIcon( QIcon( ":/icons/favorites.png" ) );
    connect( workModeAction, SIGNAL( triggered() ), this, SLOT( showWorkModeDialog() ) );
    ui->mainToolBar->addAction( workModeAction );

//    ui->mainToolBar->addSeparator();
//
//    configAction = new QAction( tr( "Configuration" ), this );
//    configAction->setShortcut( QKeySequence( "F2" ) );
//    configAction->setIcon( QIcon( ":/icons/configure.png" ) );
//    connect( configAction, SIGNAL( triggered() ), this, SLOT( showConfigTab() ) );
//    ui->mainToolBar->addAction( configAction );
}

void MainWindow::updateSnippetsList() {
    ui->snippetsList->clear();
    for( int i = 0; i < shownSnippets.size(); ++i )
        ui->snippetsList->insertItem( ui->snippetsList->count(), shownSnippets.at( i ) );
}

void MainWindow::on_snippetsList_activated(QModelIndex index)
{
    if( !index.isValid() )
        return;

    openSnippet( ui->snippetsList->currentItem()->text() );
}

void MainWindow::openSnippet( const QString &name ) {
    for( int i = 0; i < ui->tabWidget->count(); ++i )
        if( ui->tabWidget->tabText( i ) == name
            || ui->tabWidget->tabText( i ) == name + '*' ) {
            ui->tabWidget->setCurrentIndex( i );
            return;
        }

    snippets.beginGroup( name );

        QString code = snippets.value( "code" ).toString();
        QString desc = snippets.value( "desc" ).toString();

        QStringList childGroups = snippets.allKeys();
        QStringList tagsList;
        for( int i = 0; i < childGroups.size(); ++i )
            if( childGroups.at( i ).indexOf( "tag" ) == 0 )
                tagsList.append( snippets.value( childGroups.at( i ) ).toString() );

    snippets.endGroup();

    TabWidget *tab = new TabWidget( ui->tabWidget, name, code, desc, tagsList.join( ";" ) );
    ui->tabWidget->addTab( tab, name );
    ui->tabWidget->setCurrentWidget( tab );

    connect( tab, SIGNAL( snippetSaved( QString, QString, QString, QString ) ),
             this, SLOT( onSnippetSaved( QString, QString, QString, QString ) ) );
}

void MainWindow::onSnippetSaved( QString name, QString code, QString desc, QString tags ) {
    QString n = name;
    QString c = code;
    QString d = desc;
    QString t = tags;

    snippets.beginGroup( name );

        snippets.remove( QString() );

        snippets.setValue( "code", code );
        snippets.setValue( "desc", desc );

        QStringList tagList = tags.split( ";", QString::SkipEmptyParts );
        for( int i = 0; i < tagList.size(); ++i )
            snippets.setValue( "tag" + QString::number( i ), tagList.at( i ) );

    snippets.endGroup();

    setupAll();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    TabWidget *widget = qobject_cast< TabWidget* >( ui->tabWidget->widget( index ) );
    if( widget )
        widget->onCloseRequested();
    else
        ui->tabWidget->removeTab( index );
}

void MainWindow::showFindReplaceWidget() {
    TabWidget *widget = qobject_cast< TabWidget* >( ui->tabWidget->currentWidget() );
    if( widget )
        widget->onShowFindDialog();
}

void MainWindow::newSnippet() {
    QString newName = QInputDialog::getText( 0, tr( "Create new snippet..." ),
                                             tr( "Enter new snippet's name:" ) );
    if( newName.size() == 0 )
        return;

    if( snippetsList.contains( newName ) ) {
        QMessageBox::information( this, tr( "QSnippetsManager" ),
                                  tr( "Snippet named %1 already exists. Please specify other name." )
                                    .arg( newName ) );
        newSnippet();
    } else {
        snippets.beginGroup( newName );
            snippets.setValue( "code", "" );
            snippets.setValue( "desc", "" );
        snippets.endGroup();

        setupAll();
    }
}

void MainWindow::on_searchLineEdit_textChanged( QString )
{
    applySearchFilter();
    updateSnippetsList();
}

void MainWindow::generateTagBoard() {
    ui->tagsWidget->generate( tags );
}

void MainWindow::generateTags() {
    // assume snippets list is up to date

    tags.clear();

    for( int i = 0; i < snippetsList.size(); ++i ) {
        snippets.beginGroup( snippetsList.at( i ) );

            QStringList tagList = snippets.allKeys();

            foreach( QString key, tagList )
                if( key.indexOf( QRegExp( "tag\\d+" ) ) == 0 )
                    tags[ snippets.value( key ).toString() ]++;

        snippets.endGroup();
    }
}

void MainWindow::generateSnippetsList() {
    snippetsList = snippets.allKeys();

    for( int i = 0; i < snippetsList.size(); ++i ) {
        int truncPos = snippetsList.at( i ).indexOf( '/' );
        snippetsList[ i ].truncate( truncPos );
    }

    snippetsList.removeDuplicates();
}

void MainWindow::applySearchFilter() {
    shownSnippets.clear();

    // apply tag filter first
    for( int i = 0; i < snippetsList.size(); ++i ) {
        snippets.beginGroup( snippetsList.at( i ) );

            QStringList currentTags = snippets.allKeys();
            for( int k = 0; k < currentTags.size(); ++k )
                if( currentTags.at( k ).indexOf( QRegExp( "tag\\d+" ) ) != 0 )
                    currentTags.removeAt( k-- );

            if( currentTags.size() == 0 ) {
                shownSnippets.append( snippetsList.at( i ) );
                snippets.endGroup();
                continue;
            }

            for( int k = 0; k < currentTags.size(); ++k )
                currentTags[ k ] = snippets.value( currentTags.at( k ) ).toString();

            QStringList selectedTags = ui->tagsWidget->selectedTags();
            for( int j = 0; j < selectedTags.size(); ++j ) {
                if( currentTags.contains( selectedTags.at( j ) ) ) {
                    shownSnippets.append( snippetsList.at( i ) );
                    break;
                }
            }

        snippets.endGroup();
    }

    // then search for a search string (if any)
    QString searchStr = ui->searchLineEdit->text();
    if( !searchStr.isEmpty() ) {
        for( int i = 0; i < shownSnippets.size(); ++i ) {
            QString currentSnippet = shownSnippets.at( i );
            if( currentSnippet.contains( searchStr ) )
                continue;

            snippets.beginGroup( currentSnippet );

            QString code = snippets.value( "code" ).toString();
            QString desc = snippets.value( "desc" ).toString();
            if( !code.contains( searchStr ) && !desc.contains( searchStr ) )
                shownSnippets.removeAt( i-- );

            snippets.endGroup();
        }
    }

    emit filterApplied( shownSnippets, generateToolTips(), generateCode() );
    updateSnippetsList();
}

void MainWindow::onTagButtonClicked() {
    on_searchLineEdit_textChanged( QString() );
}

void MainWindow::deleteSnippet() {
    if( !ui->snippetsList->currentItem() )
        return;

    QString name = ui->snippetsList->currentItem()->text();

    QMessageBox::StandardButton answer = QMessageBox::question( this, tr( "Are you sure?" ),
                                                   tr( "You are trying to delete snippet %1. Continue? ")
                                                       .arg( name ),
                                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No );
    if( answer == QMessageBox::Yes ) {
        snippets.remove( name );
        snippetsList.removeOne( name );

        for( int i = 0; i < ui->tabWidget->count(); ++i )
            if( ui->tabWidget->tabText( i ) == name ) {
                ui->tabWidget->removeTab( i );
                break;
            }

        setupAll();
    }
}

void MainWindow::setupAll() {
    generateSnippetsList();
    generateTags();
    generateTagBoard();
    applySearchFilter();
    updateSnippetsList();
}

void MainWindow::showWorkModeDialog() {
    workModeDialog->show();
    this->hide();
}

// this function generates tooltips for shown snippets only
QStringList MainWindow::generateToolTips() {
    QStringList code = generateAttribList( "code" );
    QStringList desc = generateAttribList( "desc" );

    QStringList toolTips;
    for( int i = 0; i < code.size(); ++i ) {
        QString toolTip;
        toolTip.append( "<b>Description:</b>" );

        if( desc.at( i ).isEmpty() )
            toolTip.append( " (none)" );
        else
            toolTip.append( "<br>" + desc.at( i ) );

        toolTip.append( "<br><br><b>Code:</b>" );

        if( code.at( i ).isEmpty() )
            toolTip.append( " (none)" );
        else
            toolTip.append( "<br>" + code.at( i ) );

        toolTips.append( toolTip );
    }

    return toolTips;
}

QStringList MainWindow::generateCode() {
    return generateAttribList( "code" );
}

void MainWindow::renameSnippet() {
    if( !ui->snippetsList->currentItem() )
        return;

    QString oldName = ui->snippetsList->currentItem()->text();

    QString newName = QInputDialog::getText( this, tr( "Enter new name..." ),
                                             tr( "Rename snippet %1 to:" ).arg( oldName ) );
    if( !newName.isEmpty() ) {
        for( int i = 0; i < ui->tabWidget->count(); ++i ) {
            QString tabText = ui->tabWidget->tabText( i );
            bool modified = false;
            if( tabText.at( tabText.size() - 1 ) == '*' ) {
                tabText.chop( 1 );
                modified = true;
            }

            if( tabText == oldName ) {
                if( modified )
                    ui->tabWidget->setTabText( i, newName + "*" );
                else
                    ui->tabWidget->setTabText( i, newName );

                TabWidget *widget = qobject_cast< TabWidget* >( ui->tabWidget->widget( i ) );
                if( widget )
                    widget->rename( newName );

                break;
            }
        }

        snippets.beginGroup( oldName );
            QStringList keys = snippets.allKeys();
            QStringList values;
            for( int i = 0; i < keys.size(); ++i )
                values.append( snippets.value( keys.at( i ) ).toString() );
        snippets.endGroup();

        snippets.remove( oldName );

        snippets.beginGroup( newName );
            for( int i = 0; i < keys.size(); ++i )
                snippets.setValue( keys.at( i ), values.at( i ) );
        snippets.endGroup();

        generateSnippetsList();
        applySearchFilter();
        updateSnippetsList();
    }
}

QStringList MainWindow::generateAttribList( QString key ) {
    QStringList toolTips;

    foreach( QString snippet, shownSnippets ) {
        snippets.beginGroup( snippet );

            toolTips.append( snippets.value( key ).toString() );

        snippets.endGroup();
    }

    return toolTips;
}

void MainWindow::onCloseSnippetRequested() {
    TabWidget *widget = qobject_cast< TabWidget* >( ui->tabWidget->currentWidget() );

    if( widget )
        on_tabWidget_tabCloseRequested( ui->tabWidget->currentIndex() );
    else
        ui->tabWidget->removeTab( ui->tabWidget->currentIndex() );
}

void MainWindow::closeEvent( QCloseEvent *event ) {
    bool canCloseApp = true;

    for( int i = 0; i < ui->tabWidget->count(); ++i ) {
        TabWidget *widget = qobject_cast< TabWidget* >( ui->tabWidget->widget( i ) );
        if( widget ) {
            bool wasModified = widget->isModified();
            on_tabWidget_tabCloseRequested( i );
            if( !wasModified )
                --i;
            else
                canCloseApp = false;
        } else {
            ui->tabWidget->removeTab( i-- );
        }
    }

    if( canCloseApp )
        event->accept();
    else
        event->ignore();
}
