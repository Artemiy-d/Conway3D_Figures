#include "DialogSaveFigure.h"
#include "LanguageManager.h"

const QString DialogSaveFigure::s_conwayFigureSuffix = tr("cf");

DialogSaveFigure::DialogSaveFigure() :
    QFileDialog()
{
    m_filters += LNG["figure_files"] +  tr(" (*.") + s_conwayFigureSuffix + ")";
    m_filters += LNG["all_files"] + tr(" (*)");
    setNameFilters(m_filters);
    connect(this, SIGNAL(filterSelected(const QString&) ), this, SLOT(filterChanged(const QString&) ) );
    setDefaultSuffix( s_conwayFigureSuffix );
}

QString DialogSaveFigure::execToSave()
{
    this->setAcceptMode(QFileDialog::AcceptSave);
    if (QFileDialog::exec() != QDialog::Accepted)
        return QString();
    return selectedFiles().first();
}

void DialogSaveFigure::filterChanged(const QString& _filter)
{
    if (_filter == m_filters.first())
        this->setDefaultSuffix( s_conwayFigureSuffix );
    else
        this->setDefaultSuffix( QString() );
}
