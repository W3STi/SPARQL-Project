#include "diagramexecutor.h"

#include <QGridLayout>
#include <QPushButton>

#include <diagramitembased.h>
#include <diagramitemsparql.h>

DiagramExecutor::DiagramExecutor( QWidget* parent )
    : QWidget( parent )
{
    createWindow();

    script_engine = new QScriptEngine( this );

    QScriptValue outputVal = script_engine->newQObject( text_edit_output );
    script_engine->globalObject().setProperty( "output", outputVal );

    // QScriptValue networkVal = engine->newQObject( network );
    // engine->globalObject().setProperty( "network", networkVal );
}

void DiagramExecutor::setScript( const QString& script )
{
    text_edit_script->setHtml( script );
}

namespace
{
QString getHtmlLine( QString line, QString style = "" )
{
    return "<span style=\"margin-top:0px; " + style + "\">" + line + "</span><br>";
}

QString CreateScriptForBlock( QVector<DiagramItem*>& block_list, int index )
{
    QString result = "";
    DiagramItem* diagram_item = block_list[index];

    result += getHtmlLine( "\nblocks_list.push( new Block( " );
    result += getHtmlLine( "\tfunction( x ) {" );

    switch ( diagram_item->type() )
    {
    case DiagramItem::BasedItemType:
        if ( ( static_cast<DiagramItemBased*>( diagram_item ) )->GetInputData() != "" )
            result += getHtmlLine( "\t\tvar input = " + ( static_cast<DiagramItemBased*>( diagram_item ) )->GetInputData() + ";" );
        break;

    case DiagramItem::SparqlItemType:
        result += getHtmlLine( "\t\tvar input = \"\";" );
        break;
    }

    result += getHtmlLine( "\t\tvar y = [];" );

    QStringList list;
    switch ( diagram_item->type() )
    {
    case DiagramItem::BasedItemType:
        list = ( static_cast<DiagramItemBased*>( diagram_item ) )->getSetting()->script.split( "\n" );
        break;

        // case DiagramItemSparql :
        //     list = ( static_cast<DiagramItemSparql*>( diagram_item ) )->getSetting()->ConvertToBasedBlockSetting()->script.split( "\n" );
        //     break;
    }

    foreach ( QString iter, list )
    {
        for ( int i = 0; i < iter.size(); i++ )
        {
            if ( iter[i] == "<" )
            {
                iter = iter.mid( 0, i ) + "&lt;" + iter.mid( i + 1, iter.size() );
            }
            if ( iter[i] == ">" )
            {
                iter = iter.mid( 0, i ) + "&gt;" + iter.mid( i + 1, iter.size() );
            }
        }
        result += getHtmlLine( "\t\t" + iter );
    }

    result += getHtmlLine( "\t\treturn y;" );
    result += getHtmlLine( "\t}," );

    result += getHtmlLine( "\t[ ]," );

    QString temp = "[ ";

    // foreach ( Arrow* arrow, api->block_list[index]->getArrows() )
    //{
    //     if ( arrow->startItem() != api->block_list[index] )
    //     {
    //         temp += QString::number( api->block_list.indexOf( arrow->startItem() ) );
    //         temp += ", ";
    //     }
    // }

    if ( temp[temp.size() - 2] == "," )
        temp.remove( temp.size() - 2, 1 );

    temp += "]";

    result += getHtmlLine( "\t" + temp );
    result += getHtmlLine( "));\n" );
    return result;
}
} // namespace

QString DiagramExecutor::ConvertDiagramItemToScript( QVector<DiagramItem*>& block_list )
{
    int end_block = -1;
    QString script = "<p style=\"white-space: pre-wrap;\">";
    for ( int i = 0; i < block_list.size(); i++ )
    {
        script += CreateScriptForBlock( block_list, i );
        if ( DiagramItem::BasedItemType == block_list[i]->type() && ( ( DiagramItemBased* )( block_list[i] ) )->getName() == "END" )
        {
            end_block = i;
        }
    }
    script += "</p>";
    return script;
}

void DiagramExecutor::createWindow()
{
    QGridLayout* grid_layout = new QGridLayout( this );
    text_edit_script = new QTextEdit( this );
    text_edit_output = new QTextEdit( this );
    text_edit_output->setMaximumHeight( 300 );
    QPushButton* button_exec = new QPushButton( "Execute", this );
    connect( button_exec, SIGNAL( clicked() ), this, SLOT( execute() ) );

    grid_layout->addWidget( text_edit_script, 0, 0 );
    grid_layout->addWidget( text_edit_output, 1, 0 );
    grid_layout->addWidget( button_exec, 2, 0 );
}

QString DiagramExecutor::loadScript( const QString& path )
{
    QFile f( path );
    QScriptValue result;
    if ( f.open( QIODevice::ReadOnly ) )
    {
        QString str = f.readAll();
        result = script_engine->evaluate( str, path );
        if ( result.isError() )
        {
            return result.toString();
        }
    }
    else
    {
        return "Failed load! (" + path + ")";
    }
    return "";
}

void DiagramExecutor::execute()
{
    script_engine->pushContext();
    text_edit_output->disconnect();
    text_edit_output->clear();
    QScriptValue result;

    QString temp = loadScript( ":/Sources/scripts/script.js" );
    if ( temp.length() )
    {
        text_edit_output->setText( "Defult script: " + temp );
    }

    temp = loadScript( ":/Sources/scripts/XMLHttpRequest.js" );
    if ( temp.length() )
    {
        text_edit_output->setText( "Defult script: " + temp );
    }

    temp = loadScript( ":/Sources/scripts/progress.js" );
    if ( temp.length() )
    {
        text_edit_output->setText( "Progress script: " + temp );
        return;
    }

    result = script_engine->evaluate( text_edit_script->toPlainText() );
    if ( result.isError() )
    {
        text_edit_output->setText( "User script: " + result.toString() );
        return;
    }

    // temp = loadScript("C:/Temp/Study/6 semestr/Kurs/Kurs/scripts/block_logic.js");
    temp = loadScript( ":/Sources/scripts/block_logic.js" );
    if ( temp.length() )
    {
        text_edit_output->setText( "Failed script: " + temp );
        return;
    }

    result = script_engine->evaluate( "indexOfEnd = " + QString::number( -1 ) + ";" );
    if ( result.isError() )
    {
        text_edit_output->setText( "Failed run script: " + result.toString() );
        return;
    }

    result = script_engine->evaluate( "main('run');" );

    if ( result.isError() )
    {
        text_edit_output->setText( "Failed run script: " + result.toString() );
        return;
    }
    script_engine->popContext();
}