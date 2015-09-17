#ifndef PLAYERMODULE_H
#define PLAYERMODULE_H

#include <QQuickItem>

class PlayerModule : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(PlayerModule)

public:
    PlayerModule(QQuickItem *parent = 0);
    ~PlayerModule();
};

#endif // PLAYERMODULE_H

