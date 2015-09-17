#ifndef PLAYERMODULES_PLUGIN_H
#define PLAYERMODULES_PLUGIN_H

#include <QQmlExtensionPlugin>

class PlayerModulesPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif // PLAYERMODULES_PLUGIN_H

