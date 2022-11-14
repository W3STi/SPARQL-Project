#ifndef SPARQLBLOCKWINDOW_H
#define SPARQLBLOCKWINDOW_H

#include <diagramitematom.h>
#include <sgraphicsview.h>
#include <sparqlblocksettings.h>

class SparqlBlockWindow : public SGraphicsView
{
public:
    enum
    {
        DiagramMode = 10
    };

    explicit SparqlBlockWindow( QWidget* parent = nullptr );

    void setSettings( SparqlBlockSettings* settings = new SparqlBlockSettings() );

    SparqlBlockSettings* getSettings();

public slots:
    void slotOnCreateButtonClicked() override;
    void slotOnSaveButtonClicked() override;
    void slotOnOpenButtonClicked() override;

private:
    QWidget* addCustomWidget() override;
    QWidget* addCustomBotWidget() override;

    QLineEdit* line_name_block;
    void createDefaultcScene();
    void clearScene();
    bool CheckCollisionArea( DiagramItemAtom* item, DiagramItemAtom* area );
};

#endif // SPARQLBLOCKWINDOW_H
