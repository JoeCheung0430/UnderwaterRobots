
#include "hcrov.h"
#include <QtWidgets/QApplication>
#include <QDebug>
#include <iostream>
#include "client.h"

hcrov* HCROV;
Client* client;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    client = new Client();
    HCROV = new hcrov();
    //ctrl = new Control();

    client->init();
    HCROV->show();
    return a.exec();
}
