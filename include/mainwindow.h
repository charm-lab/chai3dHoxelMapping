#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "shared_data.h"
#include <QTimer>
#include <windows.h>
#include <iostream>
#include <sstream> // Required for stringstreams
#include <string.h>
#include <QString>
#include <ostream>
#include <istream>
#include <fstream>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <qfiledialog.h>
#include <qinputdialog.h>
#include <QKeyEvent>
#include <QThread>
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "OVRRenderContext.h"
#include "OVRDevice.h"
#include "Widget_OpenGLDisplay.h"
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <Eigen/dense>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>


using namespace chai3d;
using namespace std;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // functions
    void Initialize();
    void processEvents();

    // vars
    //Widget_OpenGLDisplay* windowGLDisplay;
    shared_data* p_CommonData;

    double updateHz = 100;

    // duplicate data record vector to limit thread locking
    std::vector<DataRecordStruct> localDataRecorderVector;

    // clock for display rate
    cPrecisionClock graphicsRateClock;
    int graphicsRateCounter;
    int graphicsRateEstimate;

    // qwtVars
    QwtPlotCurve *curve1;
    QwtPlotCurve *curve2;
    QwtPlotCurve *curve3;
    QwtPlotGrid *grid;
    QPolygonF points1;
    QPolygonF points2;
    QPolygonF points3;
    QPointF point;
    double i;

    double localOutputStrokes0; //Eigen::Vector3d localOutputVoltages0;
    Eigen::Vector3d localDesiredPos0;
    //double localDesiredJointAngle0; //Eigen::Vector3d localDesiredJointAngle0;
    double localMotorAngles0;
    Eigen::Vector3d localForce0;
    Eigen::Vector3d localForce1;

    double localOutputStrokes1; //Eigen::Vector3d localOutputVoltages0;
    Eigen::Vector3d localDesiredPos1;
    //double localDesiredJointAngle1; //Eigen::Vector3d localDesiredJointAngle1;
    double localMotorAngles1;

    //double Inertia_Scaling_Mass;
    //double Inertia_Scaling_Factor;

    // Oculus Rift
    // display context
    chai3d::cOVRRenderContext renderContext;
    // oculus device
    chai3d::cOVRDevice oculusVR;

    //Prev values needed for IIR filter:
    Eigen::Vector3d localDesiredPos0_prev;
    Eigen::Vector3d localDesiredPos1_prev;

    Eigen::Vector3d localForce0_prev;
    Eigen::Vector3d localForce1_prev;


    //Set Device Desired Forces or positions:
    //Dorsal desiered values:
    QString device0X; //X
    QString device0Y; //Y
    QString device0Z; //Z
    QString device0X_prev; //X
    QString device0Y_prev; //Y
    QString device0Z_prev; //Z
    QString dev0Mag;
    //Ventral desiered values:
    QString device1X; //X
    QString device1Y; //Y
    QString device1Z; //Z
    QString device1X_prev; //X
    QString device1Y_prev; //Y
    QString device1Z_prev; //Z
    QString dev1Mag;

    //Initial Camera Variables:
    double initOffset;
    double initPolar;
    double initCamRadius;
    double initAzimuth;

    //------------------------------------------------------------------------------
    // DECLARED MACROS
    //------------------------------------------------------------------------------

    // convert to resource path
#define RESOURCE_PATH(p)    (char*)((resourceRoot+string(p)).c_str())


private:
    Ui::MainWindow *ui;
    QSerialPort *serial;

    // a timer for updating the gui
    QTimer GraphicsTimer;

    void keyPressEvent(QKeyEvent* a_event);

private slots:
    void UpdateGUIInfo();
    //void on_CalibratePushButton_clicked();
    //void on_ZeroSliders_clicked();
    void onGUIchanged();
    //void on_startSin_clicked();
    //void on_setDirectory_clicked();
    void on_turnOff_clicked();
    //void on_palpationButton_clicked();
    //void on_frictionButton_clicked();
    //void on_startCircle_clicked();
    //void on_loadProtocol_clicked();
    //void on_startExperiment_clicked();
    QString getSubjectDirectory();
    void WriteDataToFile();
    //void on_setNeutral_clicked();
    //void on_setTrial_clicked();
    void on_dynamicEnvironment_clicked();
    void rotateTissueLineDisp(double angle);
    //void rotateTissueLine(double angle);
    //void on_startExperiment_3_clicked();
    //void on_loadProtocol_3_clicked();
    //void on_pushButton_clicked();
    //void on_AllDown0_clicked();
    //void on_AllDown1_clicked();
    //void on_Mass_clicked();
    //void on_Friction_clicked();
    //void on_Size_clicked();
    //void on_Stiffness_clicked();
    //void on_loadProtocol_2_clicked();
    //void on_startExperiment_2_clicked();
    bool CheckFingers();
    //void on_impulseForward_clicked();
    //void on_impulseBackward_clicked();
    //void on_impulseRight_clicked();
    //void on_impulseLeft_clicked();
    //void on_impulseUp_clicked();
    //void on_impulseDown_clicked();
    //void on_scaleUp_clicked();
    //void on_scaleDown_clicked();
    //void on_impulseOff_clicked();
    //void on_impulseTorquezNeg_clicked();
    //void on_impulseTorquex_clicked();
    //void on_impulseTorquexNeg_clicked();
    //void on_impulseTorquey_clicked();
    //void on_impulseTorqueyNeg_clicked();
    //void on_impulseTorquez_clicked();
    //void on_StartCD_clicked();

    //void ProgressCDExpParams();
    void RecordCDInertiaData();

    //void updateCDInertia();

    //void on_setCDDirectory_clicked();
    //void ResetDynamicEnviron();

    //void on_pushButton_2_clicked();
    //void on_AllDown01_clicked();
    //void on_adjustParamsButton_clicked();

    //void on_InertialMassSlider_valueChanged(int value);
    //void on_InertialFactorSlider_valueChanged(int value);
    //void on_StartInertiaTrial_clicked();
    //void on_SetIDir_clicked();
    //void WhatMassAreWeDisplaying();
    bool readExpStuffIn();

    //void on_ContinueInertiaTrial_clicked();

    //void on_Subjective_Exp_clicked();

    //void on_stiffnessSlider_valueChanged(int value);
    //void on_VibrationButton_clicked();
    //void on_pushButton_3_clicked();
    //void on_pushButton_4_clicked();

    //void on_cdInertia_clicked();

    void on_StiffnessExp_clicked();
    void on_StiffnMassCombined_clicked();
    void on_FingerMappingExp_clicked();
    void on_HoxelMappingExp_clicked();
    void on_MultiMassExp_clicked();
    void on_Manual_clicked();
    void on_resetCameraButton_clicked();

    //SERIAL:
    void on_openButton_clicked();
    void readSerialData();
    void writeSerialData();
};

#endif // MAINWINDOW_H
