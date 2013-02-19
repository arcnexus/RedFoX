#include "agendaform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AgendaForm w;
    w.show();
    
    return a.exec();
}
