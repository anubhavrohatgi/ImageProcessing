#include <QCoreApplication>
#include "SettingsManager.h"
#include <QDebug>
#include <QTime>


class test
{
public:
    test() {}
    ~test(){}

    void build() {
        mgr = pil::SettingsManager::instance();
        mgr->setValue(mName,"blackbox",true);
    }

private:
    pil::SettingsManager* mgr;
    const QString mName = "Test";

};


int main(int argc, char** argv)
{
    QCoreApplication::setApplicationName("Star Runner");
    QCoreApplication::setOrganizationName("MySoft");
    QCoreApplication::setOrganizationDomain("mysoft.com");
    QCoreApplication app(argc, (char**)argv);

    pil::SettingsManager* mgr = pil::SettingsManager::instance();
    mgr->setdefaultSettings();
    mgr->setValue("Anubhav","Hello","123");
    mgr->setValue("Anubhav","Check","1222");
    mgr->setValue("Anubhav","Ben","123");


    test t;
    t.build();


    qDebug()<<pil::SettingsManager::getValue("Anubhav","Ben").toString();


//    app.exec();
    return 0;
}
