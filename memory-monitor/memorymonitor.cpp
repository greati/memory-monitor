#include "memorymonitor.h"
#include "ui_memorymonitor.h"
#include "memory.cpp"

MemoryMonitor::MemoryMonitor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MemoryMonitor)
{
    ui->setupUi(this);

    general_time = 0;
    process_time = 0;
    selected_process = -1;

    QObject::connect(&m_timer,SIGNAL(timeout()), this, SLOT(handleTimeout()));
    connect(ui->proc_table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(processSelected(int, int)));

    m_timer.setInterval(500);

    // Main memory usage
    ui->gen_monitor->legend->setVisible(true);
    ui->gen_monitor->addGraph();
    ui->gen_monitor->graph(0)->setPen(QPen(Qt::blue));
    ui->gen_monitor->graph(0)->setData(memX,memY);
    ui->gen_monitor->graph(0)->setName("Main memory");
    ui->gen_monitor->xAxis->setVisible(false);

    ui->gen_monitor->addGraph();
    ui->gen_monitor->graph(1)->setPen(QPen(Qt::red));
    ui->gen_monitor->graph(1)->setData(genswapX,genswapX);
    ui->gen_monitor->graph(1)->setName("Swap");

    ui->gen_monitor->yAxis->setLabel("Usage (%)");
    ui->gen_monitor->xAxis->setAutoTickLabels(false);
    ui->gen_monitor->yAxis->setRange(0,100);
    ui->gen_monitor->xAxis->setRange(0,60);

    // Proc graph
    ui->proc_graph->legend->setVisible(true);

    ui->proc_graph->addGraph();
    ui->proc_graph->graph(0)->setPen(QPen(Qt::blue));
    ui->proc_graph->graph(0)->setData(memprocX,memprocY);
    ui->proc_graph->graph(0)->setName("Main memory");
    ui->proc_graph->xAxis->setVisible(false);


    ui->proc_graph->yAxis->setLabel("Usage (%)");
    ui->proc_graph->yAxis->setRange(0,100);
    ui->proc_graph->xAxis->setRange(0,60);

    // Proc Table
    QStringList table_proc_headers;
    table_proc_headers << "PID" << "Name" << "Memory (%)" << "Min Fault" << "Major Fault" << "Swap (KB)";
    ui->proc_table->setColumnCount(6);
    ui->proc_table->setHorizontalHeaderLabels(table_proc_headers);
    ui->proc_table->verticalHeader()->setVisible(false);
    m_timer.start();

}

void MemoryMonitor::handleTimeout(){

    // General monitor
    std::vector<double> geninfo = meminfo();

    qreal gx = general_time;
    qreal mgy = geninfo[0];
    ui->gen_mem->setText(QString::number(geninfo[0]) + "%");
    memY.append(mgy);
    memX.append(gx);
    ui->gen_monitor->graph(0)->setData(memX,memY);

    qreal mgsy = geninfo[1];
    genswapX.append(gx);
    genswapY.append(mgsy);
    ui->gen_monitor->graph(1)->setData(genswapX,genswapY);
    ui->gen_swap->setText(QString::number(geninfo[1]) + "%");

    ui->gen_monitor->replot();

    ui->gen_cache->setText(QString::number(geninfo[2]) + " MB");

    // Process table
    std::vector<process> processes = process_by_mem_usage();
    ui->proc_table->clearContents();
    ui->proc_table->setRowCount(0);
    ui->proc_table->setRowCount(processes.size());
    for (unsigned int i = 0; i < processes.size(); ++i) {
        ui->proc_table->setItem(i, 0, new QTableWidgetItem(QString::number(processes[i].pid)));
        ui->proc_table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(processes[i].proc_name)));
        ui->proc_table->setItem(i, 2, new QTableWidgetItem(QString::number(processes[i].mem_usage)));
        ui->proc_table->setItem(i, 3, new QTableWidgetItem(QString::number(processes[i].min_flt)));
        ui->proc_table->setItem(i, 4, new QTableWidgetItem(QString::number(processes[i].maj_flt)));
        ui->proc_table->setItem(i, 5, new QTableWidgetItem(QString::number(processes[i].swap)));
    }
    if (selected_process == -1)
        processSelected(0,0);

    // Process monitor
    qreal px = process_time;//ui->gen_monitor->width() / ui->gen_monitor->xAxis->tickStep();

    qreal mpy = 0.0;

    try {
        qreal mpy = mem_usage(selected_process);

        memprocY.append(mpy);
        memprocX.append(px);
        ui->proc_graph->graph(0)->setData(memprocX,memprocY);
        ui->proc_graph->replot();
        ui->proc_mem->setText(QString::number(mpy) + "%");
        ui->proc_swap->setText(QString::number(get_swap_proc(selected_process)) + " KB");
    } catch(std::exception & e){
        processSelected(0,0);
        process_time = 0;
    }

    if (general_time == 60) {
        general_time = 0;
        memX.clear(); memY.clear();
        genswapX.clear(); genswapX.clear();
        ui->gen_monitor->graph(0)->setData(memX, memY);
        ui->gen_monitor->graph(1)->setData(genswapX,genswapY);
    } else general_time += 1;

    if (process_time == 60) {
        process_time = 0;
        memprocX.clear(); memprocY.clear();
        ui->gen_monitor->graph(0)->setData(memprocX, memprocY);
    } else process_time += 1;

}

void MemoryMonitor::processSelected(int nrow, int ncol) {
    process_time = 0;
    selected_process = ui->proc_table->item(nrow, 0)->text().toInt();
    ui->proc_pid->setText(QString::number(selected_process));
    memprocX.clear(); memprocY.clear();
}

MemoryMonitor::~MemoryMonitor()
{
    delete ui;
}
