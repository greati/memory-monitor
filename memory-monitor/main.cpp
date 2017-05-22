#include "memorymonitor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MemoryMonitor w;
    w.show();

    return a.exec();
}
