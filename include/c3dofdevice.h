/* This file controls the 3-DOF wearable device based on the "interaction force" from the
 * CHAI computed device */

#ifndef C3DOFDEVICE_H
#define C3DOFDEVICE_H

#include "cMotorController.h"
#include <QDebug>
#include <QVector>
#include <math.h>
#include <QGenericMatrix>
#include <QMatrix>
#include <Eigen/dense>
#include <iostream>
#include "chai3d.h"
#include <QMutex>

//SERIAL STUFF
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QList>


#define PI 3.14159265

//// Physical parameters of build agnostic [mm] =======================================
//#define SPRING_TORQUE 0.1               // 0.1 for stiff, .05 for weak [in-.lbs/180 deg], measured at parallel
//#define MOTRAD 1.5                      // radius of motor pulley [mm]
//#define HORIZOFFSET 6.25                // horizontal offset of motorshaft from base joint [mm]
//#define VERTOFFSET -9.5                 // vertical offset of motorshaft from base joint [mm]
//#define CALIBANGLE 0.785398             // Base joint angles during calibration procedure (45 [deg] to rad)
//// Physical parameters of build index [mm] =======================================
//#define ATTACHL_0 7.5                   // distance from base joint to tether attachment point [mm]
//#define L_UA_0 9                        // length upper arm [mm]
//#define L_LA_0 9                        // length lower arm [mm]
//#define L_BASE_0 15                     //length of base (center to joint)[mm]
//#define L_EE_0 15                       // length of end effector (center to joint)[mm]
//// Physical parameters of build thumb [mm] =======================================
//#define ATTACHL_1 10.5                  // distance from base joint to tether attachment point [mm]
//#define L_UA_1 9                        // length upper arm [mm]
//#define L_LA_1 12                       // length lower arm [mm]
//#define L_BASE_1 15                     //length of base (center to joint)[mm]
//#define L_EE_1 18                       // length of end effector (center to joint)[mm]


class c3DOFDevice
{

public:
    // Constructor of c3DOFDevice
    c3DOFDevice(int num);
    ~c3DOFDevice();

    // Instantiates all the motor controllers
    int Init3DOFDeviceEnc();

    // gets the end effector position (ZF method)
    Eigen::Vector3d GetCartesianPos();

    // determine the desired actuator stroke based on the dsired position. Gives actuator reference
    double CalcDesiredStroke(double BoxNo, double LevNo1, double LevNo2, double dir, bool ExpMass);

    // Set the desired pos (in mm)
    void SetDesiredPos(Eigen::Vector3d);

    // read the desired pos
    Eigen::Vector3d ReadDesiredPos();

    void SetStrokeOutput (double desiredStroke, bool equal);

    // read the output voltages
    double ReadStrokeOutput();
    double GetJointAngles(); //Eigen::Vector3d GetJointAngles();


//     //Commands joint angles to the motors on the device
//    void JointController();
    void SendStroke(bool man, bool equal, bool reverse, bool equalTouch, bool index, bool thumb, double ref, double BoxNo, double LevNo1, double LevNo2, double dir, bool MassExp);

    // Set motors to stop outputting
    void TurnOffControl();

    // allows setting individual joints on startup
//    void IndivJointController(Eigen::Vector3d desJointAnglesArg, double Kp, double Kd);

    // easy rotation functions
    chai3d::cMatrix3d rotX1(double a_angle);
    chai3d::cMatrix3d rotY1(double a_angle);
    chai3d::cMatrix3d rotZ1(double a_angle);

    // Public Vars ==========================================
    Eigen::Vector3d neutralPos;
    Eigen::Vector3d desiredPos; //desired pos of 3DoF device in x,y,z
    double desiredStroke;

    bool firstTimeThrough;


private:
    cMotorController* motor_1;
    int finger;

    //SERIAL STUFF
    QSerialPort *serial;

private slots:
//    //SERIAL STUFF
//    void open_serial_port();
//    void close_serial_port();
//    void Read_Data();
//    void Write_Data();
};

#endif // C3DOFDEVICE_H
