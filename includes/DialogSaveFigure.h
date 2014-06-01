#ifndef DIALOGFIGURESAVE_H
#define DIALOGFIGURESAVE_H

#include <QFileDialog>

class DialogSaveFigure : public QFileDialog
{
    Q_OBJECT
public:
    DialogSaveFigure();
    virtual ~DialogSaveFigure() {}
    QString execToSave();
private slots:
    void filterChanged(const QString & _filter);

private:
    QStringList m_filters;

    static const QString s_conwayFigureSuffix;
};


#endif // DIALOGFIGURESAVE_H
