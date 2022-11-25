#ifndef HAPTICS_THREAD_H
#define HAPTICS_THREAD_H

#include <QObject>
#include <QThread>
#include "shared_data.h"
#include "c3DOFdevice.h"
#include "chai3d.h"
#include <stdio.h>
#include <QDir>
#include <QCoreApplication>
#include "CODE.h"
#include "Win626.h"
#include "windows.h"
#include <qDebug>
#include <string>
#include <QString>
#include <ostream>
#include <istream>
#include <fstream>
#include <iostream>
#include <sstream>
#include "windows.h"
#include <time.h>

#define pi 3.14159
#define RANGE_10V 0x00 // Range code for ADC ±10V range.
#define RANGE_5V 0x10 // Range code for ADC ±5V range.
#define EOPL 0x80 // ADC end-of-poll-list marker.
#define CHANMASK 0x0F // ADC channel number mask.

class haptics_thread : public QThread
{
    Q_OBJECT


public:
    // Public Functions ============================================

    // CONSTRUCTOR
    explicit haptics_thread(QObject *parent = 0);

    // DESTRUCTOR
    ~haptics_thread();

    // METHODS
    void initialize();
    void RecordData();
    void ComputeVRDesiredDevicePos();
    void UpdateVRGraphics();
    void CommandSinPos(Eigen::Vector3d);
    void CommandCircPos(Eigen::Vector3d);

    void InitGeneralChaiStuff();
    void InitFingerAndTool();
    void InitEnvironments();
    //    void RenderPalpation();
    //    void RenderTwoFriction();
    void RenderHump();
    //    void RenderExpFriction();
    void WriteDataToFile();
    void InitDynamicBodies();
    void RenderDynamicBodies();
    void SetInitJointAngles();
    void UpdateScaledBoxes();
    void UpdateScaledCursors();
    void UpdateScaledFingers();
    void UpdateScaledTransparency();
    //    void SetDynEnvironMassExp();
    //    void SetDynEnvironSubjective();
    //    void SetDynEnvironCDExp();
    //    void SetDynEnvironInertiaExp();
    void SetDynEnvironCDInertiaExp();
    void SetDynEnvironStiffMassExp();
    void SetDynEnvironFingerMappingExp();
    void SetDynEnvironHoxelMappingExp();
    void SetDynEnvironMultiMassExp();
    void SetDynEnvironAdjust();
    void SetManualAdjust();
    //    void SetDynEnvironVtExp();
    //    void SetDynEnvironCDInertiaSubjective();

    void DeleteDynamicBodies();

    void rotateTissueLineDisp(double angle);
    void rotateTissueLine(double angle);
    //    void AddImpulseDisp(chai3d::cVector3d &indexForce, chai3d::cVector3d &thumbForce);
    //    void AddImpulseTorqueDisp(chai3d::cVector3d &indexForce, chai3d::cVector3d &thumbForce);


    // clocks
    chai3d::cPrecisionClock rateClock;
    chai3d::cPrecisionClock rateDisplayClock;

    double currTime;
    double timeInterval;
    double lastTime;

    // Public Variables ============================================
    shared_data* p_CommonData; //create a pointer to a shared_data struct

    //Chai3D variables
    chai3d::cSpotLight* light;
    chai3d::cToolCursor* m_tool0;
    chai3d::cToolCursor* m_tool1;
    chai3d::cShapeSphere* m_curSphere0;  // sphere to hover over tool and enable frames
    chai3d::cShapeSphere* m_curSphere1;
    chai3d::cMesh* ground;
    chai3d::cMesh* Right_Platform;
    chai3d::cMesh* Left_Platform;
    chai3d::cMultiMesh* finger;
    chai3d::cMultiMesh* thumb;
    chai3d::cMultiMesh* wire;    
    chai3d::cMultiMesh* boxWithHole;
    chai3d::cEffectSurface* newEffect;
    cODEGenericBody* ODEGPlane0;
    cODEGenericBody* ODEGPlane1;
    chai3d::cMesh* globe;

    chai3d::cMesh* wall;
    chai3d::cMesh* wall2;

    chai3d::cMesh* backWall; //Added for HME
    chai3d::cMesh* sideWall1; //Added for HME
    chai3d::cMesh* sideWall2 ; //Added for HME

    chai3d::cMesh* hoop1;
    chai3d::cMesh* hoop2;
    chai3d::cMesh* hoop3;
    chai3d::cMesh* hoop4;
    chai3d::cMesh* target1;
    chai3d::cMesh* target2;
    chai3d::cMesh* target3;

    chai3d::cMaterial matTarget1;
    chai3d::cMaterial matTarget2;
    chai3d::cMaterial matTarget3;
    chai3d::cMaterial matHoop1;
    chai3d::cMaterial matHoop2;
    chai3d::cMaterial matHoop3;


    chai3d::cMesh* dynamicHoop1; // -- addedd for HME
    chai3d::cMaterial matDynamicHoop1; // -- addedd for HME    
    chai3d::cMesh* dynamicHoop2; // -- addedd for HME
    chai3d::cMaterial matDynamicHoop2; // -- addedd for HME

    double targetRadius;

    chai3d::cVector3d groundPos;

    chai3d::cVector3d scaledBox1Pos;
    chai3d::cVector3d scaledBox2Pos;
    chai3d::cVector3d scaledBox3Pos;

    chai3d::cVector3d target1Pos;
    chai3d::cVector3d target2Pos;
    chai3d::cVector3d target3Pos;

    chai3d::cVector3d hoop1Pos;
    chai3d::cVector3d hoop2Pos;
    chai3d::cVector3d box1InitPos;    
    chai3d::cVector3d box2InitPos;
    chai3d::cVector3d box3InitPos;

    chai3d::cVector3d dynamicHoop1Pos; // -- addedd for HME    
    chai3d::cVector3d dynamicHoop2Pos; // -- addedd for HME

    chai3d::cVector3d target1Dist;
    chai3d::cVector3d target2Dist;
    chai3d::cVector3d target3Dist;

    // objects for doing display scaling
    chai3d::cMultiMesh* scaledFinger;
    chai3d::cMultiMesh* scaledThumb;

    chai3d::cShapeSphere* m_dispScaleCurSphere0;
    chai3d::cShapeSphere* m_dispScaleCurSphere1;

    // dyn environment params
    double boxSize1, boxSize2, boxSize3;
    double mass1, mass2, mass3;
    double friction1, friction2, friction3;
    double dynFriction1, dynFriction2, dynFriction3;
    double stiffness1, stiffness2, stiffness3;
    double latStiffness1, latStiffness2, latStiffness3;

    // offsets for haptic tool from finger models
    chai3d::cVector3d fingerOffset;
    chai3d::cVector3d thumbOffset;

    // vectors for scaling
    chai3d::cVector3d curCenter;
    chai3d::cVector3d scaledCurCenter;
    chai3d::cVector3d centToFingCur;
    chai3d::cVector3d centToThumbCur;

    //texture for globe
    chai3d::cTexture2dPtr textureSpace;

    double boxSize;
    double groundLength;
    double groundThickness;


    double wallLength;
    double wallHeight;
    double innerWallHeight; //Added for HME
    double outerWallHeight; //Added for HME
    double wallThickness;

    // ODE Module variables
    cODEWorld* ODEWorld;

    // vars for storing computed impulse forces
    chai3d::cVector3d indexImpulse;
    chai3d::cVector3d thumbImpulse;
    chai3d::cVector3d indexTorqueImpulse;
    chai3d::cVector3d thumbTorqueImpulse;

    // vars for computed forces
    double toolRadius;
    chai3d::cVector3d  filteredDeviceForce0;
    chai3d::cVector3d computedForce0;
    chai3d::cVector3d deviceComputedForce0;
    Eigen::Vector3d deviceForceRecord0;
    Eigen::Vector3d globalForceRecord0;
    chai3d::cVector3d  lastFilteredDeviceForce0;
    Eigen::Vector3d lastFilteForce0;
    chai3d::cVector3d estimatedVel0;

    chai3d::cVector3d filteredDeviceForce1;
    chai3d::cVector3d computedForce1;
    chai3d::cVector3d deviceComputedForce1;
    Eigen::Vector3d deviceForceRecord1;
    Eigen::Vector3d globalForceRecord1;
    chai3d::cVector3d lastFilteredDeviceForce1;
    chai3d::cVector3d estimatedVel1;

    // tracker rotation variables
    chai3d::cVector3d position0;
    chai3d::cMatrix3d rotation0;
    chai3d::cMatrix3d fingerRotation0;
    chai3d::cMatrix3d deviceRotation0;

    chai3d::cVector3d position1;
    chai3d::cMatrix3d rotation1;
    chai3d::cMatrix3d fingerRotation1;
    chai3d::cMatrix3d deviceRotation1;

    // ints for display counters
    int rateDisplayCounter;
    int recordDataCounter;

    // variables for handling ADC read in
    BYTE poll_list[16]; // List of items to be digitized.
    SHORT databuf[16]; // Buffer to receive digitized data.

    chai3d::cShapeLine* force1_show;
    chai3d::cShapeLine* force2_show;
    chai3d::cVector3d lastforce1;
    chai3d::cVector3d lastforce2;

    //Line -- Added for HME
     chai3d::cShapeLine* boxToWireLine;

    //Length of straight wire stl
    double wireLengthInM;
    chai3d::cVector3d wireEnd1;
    chai3d::cVector3d wireEnd2;

    //    chai3d::cShapeLine* hoop1;
    //    chai3d::cShapeLine* hoop2;
    //    chai3d::cShapeLine* hoop3;
    //    chai3d::cShapeLine* hoop4;

    chai3d::cVector3d hoop_point1;
    chai3d::cVector3d hoop_point2;
    chai3d::cVector3d hoop_point3;
    chai3d::cVector3d hoop_point4;

    int Impulse_Max0=30;
    int Impulse_Counter0=0;
    //bool Impulse0=false;
    bool Playing_Impulse0=false;
    int Impulse_Max1=30;
    int Impulse_Counter1=0;
    //bool Impulse1=false;
    bool Playing_Impulse1=false;

    int SDVib_Max0=30;
    int SDVib_Counter0=0;
    bool SDVib0=false;
    bool Playing_SDVib0=false;
    int SDVib_Max1=30;
    int SDVib_Counter1=0;
    bool SDVib1=false;
    bool Playing_SDVib1=false;
    double SDVib_StartTime0;
    double SDVib_CurrentTime0;
    double SDVib_StartTime1;
    double SDVib_CurrentTime1;
    double Fragile;

    chai3d::cMaterial mat1;
    chai3d::cMaterial mat2;
    chai3d::cMaterial mat3;
    bool Break0;
    bool Break1;

    chai3d::cMaterial mat_left;
    chai3d::cMaterial mat_right;

    bool  PreventVibOnRelease0;
    bool  PreventVibOnRelease1;

    int PreventCounter0;
    int PreventCounter1;

protected:
    void run();
};

#endif // HAPTICS_THREAD_H
