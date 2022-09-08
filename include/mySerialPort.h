#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#include <QSerialPort>
#include <QSerialPortInfo>


class MySerialPort
{
public:
    MySerialPort();
    // CONSTRUCTOR



    // DESTRUCTOR
    ~MySerialPort();

    //METHODS
   // void initialize();
   void openSerialPort();
   void readData();
   void writeData(const QByteArray);

};

#endif // MYSERIALPORT_H
