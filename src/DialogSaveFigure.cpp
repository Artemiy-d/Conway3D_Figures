#include "DialogSaveFigure.h"
#include "LanguageManager.h"

const QString DialogSaveFigure::s_conwayFigureSuffix = QString("cf");

DialogSaveFigure::DialogSaveFigure() :
    QFileDialog()
{
    m_filters += LNG["figure_files"] +  QString(" (*.") + s_conwayFigureSuffix + ")";
    m_filters += LNG["all_files"] + QString(" (*)");
    setNameFilters(m_filters);
    connect(this, SIGNAL(filterSelected(const QString&) ), this, SLOT(filterChanged(const QString&) ) );
    setDefaultSuffix( s_conwayFigureSuffix );
}

QString DialogSaveFigure::execToSave(const QString & _lastFileName)
{
    QFileInfo fInfo(_lastFileName);

    if ( fInfo.exists() )
    {
        if ( fInfo.isDir() )
        {
            setDirectory( _lastFileName );
        }
        else
        {
            setDirectory( fInfo.absolutePath() );
            selectFile( fInfo.fileName() );
        }
    }
    setAcceptMode(QFileDialog::AcceptSave);

    if (QFileDialog::exec() != QDialog::Accepted)
        return QString();
    return selectedFiles().first();
}

void DialogSaveFigure::filterChanged(const QString& _filter)
{
    if (_filter == m_filters.first())
        setDefaultSuffix( s_conwayFigureSuffix );
    else
        setDefaultSuffix( QString() );
}
