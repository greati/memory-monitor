#include "memorymonitor.h"
#include "ui_memorymonitor.h"

MemoryMonitor::MemoryMonitor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MemoryMonitor)
{
    ui->setupUi(this);

    v = 0;

    QObject::connect(&m_timer,SIGNAL(timeout()), this, SLOT(handleTimeout()));

    m_timer.setInterval(1000);



    // Main memory usage
    ui->gen_monitor->addGraph();
    ui->gen_monitor->graph(0)->setPen(QPen(Qt::blue));
    ui->gen_monitor->graph(0)->setData(memX,memY);

    ui->gen_monitor->xAxis->setLabel("Usage");
    ui->gen_monitor->yAxis->setLabel("Time");
    ui->gen_monitor->yAxis->setRange(0,100);
    ui->gen_monitor->xAxis->setRange(0,60);

    // Proc graph
    ui->proc_graph->addGraph();
    ui->proc_graph->graph(0)->setPen(QPen(Qt::blue));
    ui->proc_graph->graph(0)->setData(memX,memY);

    ui->proc_graph->xAxis->setLabel("Usage");
    ui->proc_graph->yAxis->setLabel("Time");
    ui->proc_graph->yAxis->setRange(0,100);
    ui->proc_graph->xAxis->setRange(0,60);

    m_timer.start();

}

void MemoryMonitor::handleTimeout(){
    qreal x = v;//ui->gen_monitor->width() / ui->gen_monitor->xAxis->tickStep();
    qreal y = 50;
    memY.append(y);
    memX.append(x);
    ui->gen_monitor->graph(0)->setData(memX,memY);
    ui->gen_monitor->replot();
    v += 1;
}

MemoryMonitor::~MemoryMonitor()
{
    delete ui;
}
