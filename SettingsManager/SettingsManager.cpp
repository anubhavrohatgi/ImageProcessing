#include "SettingsManager.h"
#include <QFile>

namespace pil {


SettingsManager* SettingsManager::_instance = 0;


SettingsManager::SettingsManager(QObject *parent) : QObject(parent)
{
    this->sets = new QSettings(QCoreApplication::applicationDirPath()+"/config.ini", QSettings::IniFormat);
    qDebug()<<"[SettingsManager] Creating Settings Singleton";
}

SettingsManager *SettingsManager::instance()
{
    if (_instance == 0) _instance = new SettingsManager();
    return _instance;
}

QVariant SettingsManager::getValue(QString key)
{
    QSettings* config = instance()->sets;
    return config->value(key);
}

QVariant SettingsManager::getValue(QString section, QString key)
{
    QMutex m;
    m.lock();
    QSettings* config = instance()->sets;

    if(config->status() == QSettings::AccessError){
        qInfo()<<"[SettingsManager] Error Accessing File";
    } else if(!QFile(config->fileName()).exists()) {
        qInfo()<<"[SettingsManager] Error Accessing path";
    } else {
        qInfo()<<"[SettingsManager] Reading Config File Done";
    }


    config->beginGroup(section);
    QVariant returnValue = config->value(key);

    if(!config->contains(key))
        qInfo()<<"[SettingsManager] Key not found ["<<key<<"]";

    config->endGroup();

    m.unlock();
    return returnValue;
}

void SettingsManager::setValue(QString key, QVariant val)
{
    QSettings* config = instance()->sets;
    return config->setValue(key, val);
}

void SettingsManager::setValue(QString section, QString key, QVariant val)
{
    QSettings* config = instance()->sets;
    config->beginGroup(section);
    config->setValue(key, val);
    config->endGroup();
    config->sync();
}

SettingsManager::~SettingsManager()
{
    sets->deleteLater();
}


void SettingsManager::setdefaultSettings()
{
    QSettings* config = instance()->sets;
    config->beginGroup("logging");
    config->setValue("fileName", "../logs/PILlog.log");
    config->setValue("minLevel", 0);
    config->setValue("bufferSize", 100);
    config->setValue("maxSize", 1000000);
    config->setValue("maxBackups", 2);
    config->setValue("timestampFormat", "dd.MM.yyyy hh:mm:ss.zzz");
    config->setValue("msgFormat", "{timestamp} {typeNr} {type} {msg}\n  in line {line} function {function}");
    config->endGroup();

    config->sync();
}

}//end of namespace pil
