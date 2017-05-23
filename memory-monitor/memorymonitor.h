#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <QMainWindow>
#include <QtCore/QTimer>

namespace Ui {
class MemoryMonitor;
}

class MemoryMonitor : public QMainWindow
{
    Q_OBJECT

public:
    explicit MemoryMonitor(QWidget *parent = 0);
    ~MemoryMonitor();

public slots:
    void handleTimeout();
    void processSelected(int, int);

private:
    QTimer m_timer;
    Ui::MemoryMonitor *ui;
    QVector<double> memX, memY;

    QVector<double> memprocX, memprocY;

    double general_time, process_time;

    int selected_process;
};

#endif // MEMORYMONITOR_H
