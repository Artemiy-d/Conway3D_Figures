/* 
 * File:   Languages.cpp
 * Author: artyom
 * 
 * Created on 14 Ноябрь 2011 г., 18:05
 */


#include "Languages.h"



Languages::Languages() : QObject()
{
    BaseLang.AddItem("language_name",tr("English"));
    BaseLang.AddItem("start",tr("Start"));
    BaseLang.AddItem("stop",tr("Stop"));
    BaseLang.AddItem("step",tr("Step"));
    BaseLang.AddItem("speed",tr("Speed"));
    BaseLang.AddItem("settings",tr("Settings"));
    BaseLang.AddItem("file",tr("File"));
    BaseLang.AddItem("new_figure",tr("New Figure"));
    BaseLang.AddItem("exit",tr("Exit"));
    BaseLang.AddItem("open",tr("Open"));
    BaseLang.AddItem("save",tr("Save"));
    BaseLang.AddItem("edit",tr("Edit"));
    BaseLang.AddItem("full_screen",tr("Full screen"));
    BaseLang.AddItem("clear_map",tr("Clear map"));
    BaseLang.AddItem("create_random_map",tr("Create random map"));
    BaseLang.AddItem("create_agar",tr("Create Agar"));
    BaseLang.AddItem("modeling",tr("Modeling"));
    BaseLang.AddItem("conway_game",tr("Conway's game"));
    BaseLang.AddItem("file",tr("File"));
    BaseLang.AddItem("settings_panel",tr("Settings panel"));
    BaseLang.AddItem("view",tr("View"));
    BaseLang.AddItem("help",tr("Help"));
    BaseLang.AddItem("grid",tr("Grid"));
    BaseLang.AddItem("animation",tr("Animation"));
    BaseLang.AddItem("axes",tr("Axes"));
    BaseLang.AddItem("statistic",tr("Statistic"));
    BaseLang.AddItem("drawing",tr("Drawing"));
    BaseLang.AddItem("modeling_settings",tr("Modeling settings"));
    BaseLang.AddItem("apply",tr("apply"));
    BaseLang.AddItem("probabilities_revival",tr("Probabilities of revival"));
    BaseLang.AddItem("probabilities_death",tr("Probabilities of death"));
    BaseLang.AddItem("set_default",tr("Set default"));
    BaseLang.AddItem("cancel",tr("Cancel"));
    BaseLang.AddItem("size",tr("size"));
    BaseLang.AddItem("creating_figure",tr("Creating figure"));
    BaseLang.AddItem("type_figure",tr("Type of figure"));
    BaseLang.AddItem("ellipsoid",tr("Ellipsoid"));
    BaseLang.AddItem("torus",tr("Torus"));
    BaseLang.AddItem("parallelepiped",tr("Parallelepiped"));
    BaseLang.AddItem("offset",tr("Offset"));
    BaseLang.AddItem("create",tr("Create"));
    BaseLang.AddItem("geometry",tr("Geometry"));
    BaseLang.AddItem("default_settings",tr("Default settings"));
    BaseLang.AddItem("random_map",tr("Random map"));
    BaseLang.AddItem("about",tr("About"));
    BaseLang.AddItem("languages",tr("Languages"));
    BaseLang.AddItem("ok",tr("OK"));
    BaseLang.AddItem("plane",tr("Surface"));
    BaseLang.AddItem("open_finded",tr("Open finded files"));
    BaseLang.AddItem("save_as",tr("Save as"));
    BaseLang.AddItem("templates",tr("Templates"));
    BaseLang.AddItem("current_template",tr("Current template") );
    BaseLang.AddItem("template_name",tr("Template name") );
    BaseLang.AddItem("size_template",tr("Size") );
    BaseLang.AddItem("type_of_draw",tr("Type of draw") );
    BaseLang.AddItem("all_templates",tr("All templates"));
    BaseLang.AddItem("active_templates",tr("Active templates"));
    BaseLang.AddItem("file_not_correct",tr("File is not correct"));
    BaseLang.AddItem("cannot_open_this_file",tr("Cannot open this file"));
    BaseLang.AddItem("add_to_active",tr("Add to active"));
    BaseLang.AddItem("input_name",tr("Input name of template"));
    BaseLang.AddItem("empty_name",tr("Empty name"));
    BaseLang.AddItem("replace_",tr("Replace?"));
    BaseLang.AddItem("name_prezent",tr("Name is prezent"));
    BaseLang.AddItem("save_template",tr("Save template"));
    CurrentLang = &BaseLang;
    lang_list += CurrentLang;

    QDir dir(tr("languages") );
    QStringList sl = dir.entryList(QDir::Files);
    QString basefile = tr("BaseLang.txt");
    if (sl.count())
    {
        foreach (QString s, sl)
        {
            if (s == basefile)
            {
                QString sum = dir.absoluteFilePath(s);
                AddLang(sum, true);
            }
        }
        foreach (QString s, sl)
        {
            if (s != basefile)
            {
                QString sum = dir.absoluteFilePath(s);
                AddLang(sum);
            }
        }
        foreach (Action_lang * a_l, lang_list)
        {
            a_l->setText();
            connect(a_l,SIGNAL(triggered()),this,SLOT(act_click()));
        }
    }


}


Languages::~Languages()
{

}


QString& Languages::operator [] (char * key)
{
    return (*CurrentLang)[key];
}


void Languages::AddLang(QString &filename, bool base_lang)
{
    wchar_t * str = NULL, * strValue, * key;
    QFile file(filename);
    if (file.open(QIODevice::Text | QIODevice::ReadOnly))
    {
        QTextStream ts(&file);
        QString S = ts.readAll();
        file.close();
        if (S.length()<2) return;
        Action_lang * A_L = base_lang?&BaseLang:new Action_lang(*(MyCollection<QString>*)&BaseLang);
        str = new wchar_t[S.length()+1];
        str[S.length()] = 0;
        S.toWCharArray(str);
        while (*str!=0)
        {
            while (*(str) == ' ' || *(str)==10 || *(str)==13) str++;
            if (*(key = str) == 0) break;

            while (*str != ' ' && *str!=0) str++;
            *(str++) = 0;

            while (*str!=34 && *str!=0) str++;
            if (*str == 0) break;
            strValue = ++str;
            while (*str!=34 && *str!=0)
            {
                if (*str == '@') *str = 10;
                str++;
            }
            if (*str!=0) *(str++)=0;

            QString V = QString::fromWCharArray(strValue);
            A_L->AddItem(key,V);
        }

      //  delete str;
       if (!base_lang) lang_list+=A_L;
    }
}

void Languages::act_click()
{
    CurrentLang = (Action_lang*)QObject::sender();
    emit set_lang();
}

void Languages::CreateActions(QMenu * menu, QObject * parent)
{
    menu->addActions( *(reinterpret_cast<QList<QAction*>*>(&lang_list)) );

}

Languages appLangs;
