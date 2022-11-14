#ifndef DIAGRAMITEMATOM_H
#define DIAGRAMITEMATOM_H

#include <QGraphicsProxyWidget>
#include <QLineEdit>

#include <atomblocksettings.h>
#include <diagramitem.h>

class DiagramItemAtom : public DiagramItem
{
public:
    int type() const override { return AtomItemType; }

    DiagramItemAtom( QMenu* context_menu, AtomBlockSettings* settings = new AtomBlockSettings(),
        QGraphicsItem* parent = nullptr );

    void setItemPolygon( QPolygonF& polygon );
    AtomBlockSettings* getSettings();
    void setText( const QString& text );
    QString getText();

private:
    QLineEdit* line_edit;
    QGraphicsProxyWidget* proxy_line_edit;
    QString type_block;
};

#endif // DIAGRAMITEMATOM_H