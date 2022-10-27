#ifndef DIAGRAMITEMSETTINGS_H
#define DIAGRAMITEMSETTINGS_H

#include <QJsonValue>
#include <QPixmap>
#include <QString>

class DiagramItemSettings
{
public:
    virtual int type() = 0;
    DiagramItemSettings();

    virtual void setSettingFromJson( const QJsonValue& value ) = 0;
    virtual QJsonObject getJsonFromSetting() = 0;

    void setSettingFromString( const QString& str );

    QJsonValue jsonValFromPixmap( const QPixmap& p );
    QPixmap pixmapFrom( const QJsonValue& val );
};

#endif // DIAGRAMITEMSETTINGS_H