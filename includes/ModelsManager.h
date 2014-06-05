#ifndef MODELSMANAGER_H
#define MODELSMANAGER_H

#include "StringMap.h"

class Model;

class ModelsManager
{
public:
    enum Enabled
    {
        DoNotChange,
        SetTrue,
        SetFalse
    };
private:
    struct ModelData;
    typedef StringMap<ModelData> ModelsMap;

    struct ModelData
    {
        Model * model;
        bool mainEnabled;
        bool activeEnabled;

        ModelData( Model * _m, bool _mainEnabled = true, bool _activeEnabled = true );

        void setEnabled( Enabled _main = DoNotChange, Enabled _active = DoNotChange );

    };

public:
    ModelsManager();
    virtual ~ModelsManager();

    void addModel(const QString & _name, const Model * _model, Enabled _main = SetTrue, Enabled _active = SetTrue);
    void setEnabled(const QString & _name, Enabled _main = DoNotChange, Enabled _active = DoNotChange);

    void saveModel(const QString & _name) const;

    Model * getModel(const QString & _name) const;


    QStringList getActiveModels() const;
    QStringList getMainModels() const;

    static QString normalizeName(const QString & _str);

public:
    static const QString s_modelPenName;

private:
    ModelsMap m_modelsMap;

    static const QString s_modelsDirectory;
    static const QString s_extension;

};

#endif // MODELSMANAGER_H
