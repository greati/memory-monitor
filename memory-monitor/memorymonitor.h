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

private:
    QTimer m_timer;
    Ui::MemoryMonitor *ui;
    QVector<double> memX, memY;
    double v;

};

#endif // MEMORYMONITOR_H
