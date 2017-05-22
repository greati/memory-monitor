#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <QMainWindow>

namespace Ui {
class MemoryMonitor;
}

class MemoryMonitor : public QMainWindow
{
    Q_OBJECT

public:
    explicit MemoryMonitor(QWidget *parent = 0);
    ~MemoryMonitor();

private:
    Ui::MemoryMonitor *ui;
};

#endif // MEMORYMONITOR_H
