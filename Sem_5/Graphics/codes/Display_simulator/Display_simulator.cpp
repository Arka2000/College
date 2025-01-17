#include <QApplication>
#include "sources/graph.h"
#include "sources/contolpanel.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget *window = new QWidget;
    window->setWindowTitle("Display simulation");

    Graph *graph = new Graph();
    ControlPanel *controlpanel = new ControlPanel();

    QLayout *parentlayout = new QHBoxLayout;
    parentlayout->addWidget(graph);
    parentlayout->addWidget(controlpanel);

    QObject::connect(controlpanel, SIGNAL(GraphResetSignal(int, int)), graph, SLOT(GraphResetSlot(int, int)));
    QObject::connect(graph, SIGNAL(pointSelect(pair<int, int>)), controlpanel, SLOT(getPointClicked(pair<int, int>)));
    QObject::connect(graph, SIGNAL(pointHover(pair<int, int>)), controlpanel, SLOT(getPointHover(pair<int, int>)));
    for(int i=0;i<7;i++)
    {
    	QObject::connect(graph, SIGNAL(pointSelect(pair<int, int>)), controlpanel->getAlgo(i), SLOT(receiveClickedPoint(pair<int, int>)));
    	QObject::connect(controlpanel->getAlgo(i), SIGNAL(paintPointSignal(pair<int, int>)), graph, SLOT(GraphPaintPointSlot(pair<int, int>)),Qt::DirectConnection);
    	QObject::connect(controlpanel->getAlgo(i), SIGNAL(paintPointSignal(pair<int, int>, QColor)), graph, SLOT(GraphPaintPointSlot(pair<int, int>, QColor)),Qt::DirectConnection);
    	QObject::connect(controlpanel->getAlgo(i), SIGNAL(unPaintPointSignal(pair<int, int>)), graph, SLOT(GraphUnPaintPointSlot(pair<int, int>)),Qt::DirectConnection);
    }
    window->setLayout(parentlayout);
    window->show();
    return app.exec();
}
