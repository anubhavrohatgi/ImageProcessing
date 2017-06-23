#ifndef SettingsManager_H
#define SettingsManager_H

#include <QCoreApplication>
#include <QObject>
#include <QSettings>
#include <QDebug>
#include <QMutex>

#ifndef DllCoreExport
#ifdef DLL_CORE_EXPORT
#define DllCoreExport Q_DECL_EXPORT
#else
#define DllCoreExport Q_DECL_IMPORT
#endif
#endif


namespace pil {

class DllCoreExport SettingsManager : public QObject
{
    Q_OBJECT
private:
    explicit SettingsManager(QObject *parent = 0);
    static SettingsManager* _instance;
    QSettings*   sets;

public:
    static SettingsManager*   instance();
    static QVariant           getValue(QString key);
    static QVariant           getValue(QString section, QString key);
    static void               setValue(QString key, QVariant val);
    static void               setValue(QString section, QString key ,QVariant val);

    void setdefaultSettings();

    ~SettingsManager();

signals:

public slots:
};


}//end of namespace pil

#endif // SettingsManager_H
