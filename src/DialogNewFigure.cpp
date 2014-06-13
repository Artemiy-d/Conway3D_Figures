#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QDebug>

#include "Scene3D.h"
#include "DialogNewFigure.h"
#include "LanguageManager.h"




DialogNewFigure::DialogNewFigure(Scene3D * _s3d)
    : QDialog(),
      m_s3d(_s3d),
      m_result(false)
{
    m_widgetsCount = 0;
    m_widgets[m_widgetsCount++] = m_labelCombo = new QLabel(this);
    m_labelCombo->move(10, 10);
    
    m_widgets[m_widgetsCount++] = m_combo = new QComboBox(this);
    m_combo->move(10, 30);
    connect(m_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(selectingFigure(int)) );

    m_widgets[m_widgetsCount++] = m_checkBoxDefaultSettings = new QCheckBox(this);

    m_options = NULL;

    m_widgets[m_widgetsCount++] = m_buttonExit = new QPushButton(this);
    
    
    connect(m_buttonExit, SIGNAL(pressed()), this, SLOT(close()));

    m_widgets[m_widgetsCount++] = m_buttonOK = new QPushButton(this);
    
    connect(m_buttonOK, SIGNAL(pressed()), this, SLOT(pressOK()));
    
    connect(&LNG, SIGNAL(set_lang()), this, SLOT(setLang()) );

    const std::list<std::string> & types = FiguresFactory::getInstance().getTypes();
    for ( std::list<std::string>::const_iterator it = types.begin();
                  it != types.end();
                  ++it)
    {
        qDebug() << QString::fromStdString( *it );
        m_combo->addItem( QString(), QVariant( QString::fromStdString( *it ) ) );
    }

    m_checkBoxDefaultSettings->move(m_combo->pos().x(), m_combo->pos().y() + m_combo->height() + 10);
    m_buttonExit->move(15, 260);
    m_buttonOK->move(20 + m_buttonExit->width(), m_buttonExit->y());

    setLang();
    setFixedSize(250, 300);
    m_combo->setCurrentIndex(0);
    selectingFigure(0);
}   

void DialogNewFigure::setLang()
{
    setWindowTitle(LNG["creating_figure"]);
    m_labelCombo->setText(LNG["type_figure"]);
    //m_combo->clear();

    const std::list<std::string> & types = FiguresFactory::getInstance().getTypes();
    int i = 0;
    for ( std::list<std::string>::const_iterator it = types.begin();
                  it != types.end() && i < m_combo->count();
                  ++it, ++i )
    {
        m_combo->setItemText( i, LNG[ QString::fromStdString( *it ) ] );
    }

    m_checkBoxDefaultSettings->setText(LNG["default_settings"]);

    m_buttonExit->setText(LNG["exit"]);
    m_buttonOK->setText(LNG["create"]);

    for (int i = 0; i<m_widgetsCount; i++)
        m_widgets[i]->adjustSize();
}

void DialogNewFigure::selectingFigure(int _index)
{
    if ( _index < 0 )
        return;

    using namespace NewFigureOptions;

    OptionsBase * options = FiguresFactory::getInstance().createFigureCreator<OptionsBase>( m_combo->itemData( _index ).toString().toStdString() );
    if ( options )
    {
        qDebug() << _index << m_combo->itemData( _index ).toString();
        if ( m_options )
        {
            m_options->setVisible( false );
            m_options->setParent( NULL );
        }
        m_options = options;
        m_options->setParent(this);
        m_options->show();

        m_options->setGeometry( m_combo->x(), m_checkBoxDefaultSettings->y() + m_checkBoxDefaultSettings->height() + 10, width() - 2 * m_combo->x(), 150);
    }
}

void DialogNewFigure::pressOK()
{
    if (m_combo->currentIndex() < 0)
        return;
    m_s3d->setFigure( m_options->createFigure() );

    m_result = true;
    close();
}
DialogNewFigure::~DialogNewFigure()
{
    m_options->setParent( NULL );
    FiguresFactory::getInstance().reset();
}

int DialogNewFigure::exec()
{
    QDialog::exec();
    return (int)m_result;
}

