//===========================================================================
/*
    This file controls defines a class "MotorControl" that interfaces
    with the Sensoray 626 to allow control of a motor with encoder.
*/
//===========================================================================

#ifndef CMOTORCONTROLLER_H
#define CMOTORCONTROLLER_H

// List all includes ==========================================

//#include "826api.h"
#include <QMutex>
#include <Eigen/dense>
#include "windows.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

// DEFINES =================================================================
#define PCI_BOARD 0            // only 1 826board //0
#define NUM_ENC 6               // 3 per wearable device
#define MODE_ENC  0x00000070    // for quadrature-encoded device using x4 clock multiplier
#define MAXCOUNT  0xFFFFFFFF    // maximum number of counts for 32-bit counter channel
#define T_WAIT    0             // maximum time to wait for data [usec]
#define MAXSETPNT 0xFFFF        // maximum analog output level (0x0000 to 0xFFFF covers output voltage range)
//#define VOLTRANGE 2             // mode that assigns +-5V range
#define VOLTRANGE 0             // mode that assigns 0/+5V range

// OLD DEFINES FROM 626
// Define used for max Sensoray Encoder Count
#define MAX_COUNT 16777215

// Define used for DAC Voltage Conversion
//#define DAC_VSCALAR 819.1

// Define the encoder counts per revolution
#define ENCODER_CPR 360 // for pololu motor (12*30:1)
//#define ENCODER_CPR 12800 // for micromo 50*256:1
//#define ENCODER_CPR 12800 // for micromo 50*4*64:1

// Define used for Encoder count to angle in radian
//#define ENCCOUNT_TO_RAD 2*3.1415926535897932384/(ENCODER_CPR)
#define ENCCOUNT_TO_RAD 20/(ENCODER_CPR)


// Define the torque constant torque = I*KT
#define KT 78.44 //mNm/A for pololu
//#define KT 501.76 //mNm/A for micromo 256:1
//#define KT 125.44 //mNm/A for micromo 64:1

// Define the amplifier output to voltage input ratio I = I_RATIO*V (defined by current amplifier)
#define AMPS_TO_VOLTS 10 //Amps*10 = # Volts, Amps = Volts/10

// Define the max amperage of the motor (continuous)
#define MAX_AMPS 0.096

class cMotorController
{
public:
    // Constructor of cMotorControl
    cMotorController(int);

    // Open and Access Sensoray 826, configure for encoder
    int open();
    int close();
    void InitEncoder();
    void InitDACOut();
    void SetOffsetAngle();
    double GetMotorAngle();
    void SetOutputTorque(double);
    void SetOutputStroke(double, bool, bool, bool, bool, bool);

    // Destructor of cMotorControl
    virtual ~cMotorController();

    // used to look at voltage output
    double voltageOutput;
    double strokeOutput;



private:
    QMutex m_mutex;
    int channelNum;
    int sl;
    double offsetAngle;
    short PotReading_iold;
    short PotReading_told;
    short PotReading_i;
    short PotReading_t;
};



#endif // CMOTORCONTROL_H
