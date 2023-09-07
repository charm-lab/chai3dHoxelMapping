// This file defines the shared data structure which is shared with threads
// throughout the program.
#ifndef SHARED_DATA_H
#define SHARED_DATA_H

// defines indicating what physical hardware is present
#define MAGTRACKER
#define SENSORAY826
//#define OCULUS
#define VIBRATION
#define SIMPLE_VIB
//#define ACC

#define QWT

#include <qDebug>
#include <QVector>
#include <math.h>
#include "cMotorController.h"
#include "c3dofChaiDevice.h"
#include "c3DOFdevice.h"
#include <Eigen/Dense>
#include "chai3d.h"
#include "SimpleIni.h"
#include <QDebug>
#include <QMetaType>
#include <QString>
#include <string>
#include <iostream>
#include <ostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <vector>
#include <qfiledialog>
#include <qinputdialog>
#include "CODE.h"

//SERVO CHANNELS
#define CHANNEL_NUM0 0
#define CHANNEL_NUM1 1
//SERVO MAX TACTOR EXTENSION
#define MAX_TACTOR_EXTENSION 12.5 //Max extension of rack and pinion mechanism in mm

#define STIFFNESS_BASELINE 50
#define STIFFNESS_INCREMENT 10
#define E_VALUE 2.718
#define CAP_NORMAL_FORCE 0

#define CAP_LATERAL_DIRECTION 0

//#define EXPERIMENTSTIFFNESS 175
#define EXPERIMENTFRICTION 2.25
#define VIBRATE_TIME 0.3
// define the data structure that holds recorded data
typedef struct
{
    double time;
    double motorAngles0; //Eigen::Vector3d motorAngles0; //Eigen::Vector3d motorAngles0;
    double jointAngles0; //Eigen::Vector3d jointAngles0;
    double motorTorque0;//Eigen::Vector3d motorTorque0;
    double voltageOut0; //Eigen::Vector3d voltageOut0;
    double strokeOut0;
    double desiredStroke0;
    //    Eigen::Vector3d VRInteractionForce0;
    Eigen::Vector3d VRIntForce0;
    //    Eigen::Vector3d VRInteractionForceGlobal0;
    Eigen::Vector3d VRIntForceGlo0;
    Eigen::Vector3d pos0;
    Eigen::Vector3d pos1;
    Eigen::Vector3d desiredPos0;
    Eigen::Vector3d desiredPos1;
    Eigen::Vector3d desiredFor0;
    Eigen::Vector3d desiredFor1;
    // Added for Hoxel Mapping Experiment:
    //Local to fingers Normal Forces:
    Eigen::Vector3d desiredNormFor0;
    Eigen::Vector3d desiredNormFor1;
    //Local to fingers Tangential Forces:
    Eigen::Vector3d desiredTanFor0;
    Eigen::Vector3d desiredTanFor1;


    double motorAngles1; //Eigen::Vector3d motorAngles1; //Eigen::Vector3d motorAngles1;
    double jointAngles1; //Eigen::Vector3d jointAngles1;
    double motorTorque1;//Eigen::Vector3d motorTorque1;
    double voltageOut1; //Eigen::Vector3d voltageOut1;
    double strokeOut1;
    double desiredStroke1;


    double conditionNo;
    double strokeScale;
    double ref1;
    double ref2;
    bool success; //double success;
    double box1Stiffness;
    double box2Stiffness;
    double box3Stiffness;
    double box1Mass;
    double box2Mass;
    double box3Mass;
    double stiffness1;
    double stiffness2;
    double stiffness3;
    double mass1;
    double mass2;
    double mass3;
    double lev1;
    double lev2;
    double dir;

    //    Eigen::Vector3d VRInteractionForce1;
    Eigen::Vector3d VRIntForce1;
    Eigen::Vector3d VRIntForceGlo1;
    //Eigen::Vector3d VRInteractionForceGlobal1;
    chai3d::cVector3d magTrackerPos0;
    chai3d::cVector3d magTrackerPos1;
    bool indexContact;
    bool thumbContact;
    chai3d::cMatrix3d deviceRotation0;
    chai3d::cMatrix3d deviceRotation1;

    chai3d::cVector3d box1Pos;
    chai3d::cVector3d box2Pos;
    chai3d::cVector3d box3Pos;
    chai3d::cVector3d scaledBox1Pos;

    double boxMass;
    double CDRatio;
    int isRef;
    int isReversal;
    int pairNo;
    int subjResponse;
    int isUpperCurve;
    int InertiaScalingFactor;
    //    int blockNo;
    double subjectNo;
    double AdjustedTrialNo;
    double ReferenceMass;
    double ComparisonMass;

    double compMass;
    double compCD;
    double compInertia;
    double offset;
    //    double expMass;
    double expCD;
    //    double expInertia;
    int training;


    //    int CDVert;
    int referenceFirst;

    chai3d::cVector3d gravity;
    chai3d::cMatrix3d box1GlobalRotMat; //Modified for HME
    chai3d::cMatrix3d box1LocalRotMat; //Added for HME
    chai3d::cMatrix3d box2GlobalRotMat; //Modified for HME
    chai3d::cMatrix3d box2LocalRotMat; //Added for HME
    chai3d::cMatrix3d box3GlobalRotMat; //Modified for HME
    chai3d::cMatrix3d box3LocalRotMat; //Added for HME
    int Break_num;
    int ThroughHoop_num;
    int StartTime;

    int Fragility;
    int VT;
    int DoF3;

    int Acc;

    //Mappings For Jasmin's Finger Mapping Experiments
    int mapping;
    int boxInitParam;
    bool hoopSuccess;
    bool targetSuccess;
    bool trialSuccess;

    //For HME:
    int hmeExpType;
    bool manipForceTooHigh;
    chai3d::cVector3d boxInteractionForce;

    // the trial number of the experiment
    int trialNo;

} DataRecordStruct;

typedef enum
{
    idleExperiment,
    //    frictionTrial,
    sizeWeightTrial,
    InertialscalingTrial,
    CDTrial,
    palpationTrial,
    palpationLineTrial,
    palpationLineWritingToFile,
    palpationLineBreak,
    trialBreak,
    endExperiment,
    training,
    SubjectiveTrial,
    VtTrial,
    CDInertiaTrial
} experiment_states;

typedef enum
{
    idleControl,
    neutralPosControl,
    sliderControlMode,
    VRControlMode,
    sinControlMode,
    circControlMode,
    //initCalibControl
} control_states;

typedef enum
{
    none,
    //palpation,
    //twoFriction,
    //experimentFriction,
    //experimentPalpationLine,
    dynamicBodies,
    experimentInertia,
    experimentVt

} environment_states;

typedef enum
{
    standard,
    manual,
    dimension,
    //dynamicExperiment,
    dynamicInertiaExp,
    StiffnessExperiment,
    StiffnessMassExperiment,
    FingerMappingExperiment,
    HoxelMappingExperiment,
    WireGuideExperiment,
    CubeGuidanceExperiment,
    dynamicMagnitudeExp,
} dynamicObject_states;

typedef enum
{
    vt,
    impulse,
    None

} vt_feedback_states;

typedef enum
{
    Dof3,
    Dof2,
    Dof0

} Dof3_feedback_states;

// define the data structure that contains data that we share between threads
typedef struct
{
    // camera vars
    chai3d::cCamera* p_camera;
    chai3d::cWorld * p_world;
    chai3d::cVector3d cameraPos;
    chai3d::cVector3d lookatPos;
    chai3d::cVector3d upVector;

    // camera vars for polar camera movement
    double azimuth;
    double polar;
    double camRadius;
    double offset;

    //camere vars for cartesian camera movement
    double lookatPosX;
    double lookatPosY;
    double lookatPosZ;

    QMutex sharedMutex;
    QMutex dataRecordMutex;
    QMutex resetRenderMutex;

    // the delta mechanism class
    c3DOFDevice* wearableDelta0;
    c3DOFDevice* wearableDelta1;

    // the delta mechanism chai class
    chai3d::cGenericHapticDevicePtr chaiMagDevice0; // a pointer to the current haptic device
    chai3d::cGenericHapticDevicePtr chaiMagDevice1; // support two mag sensors

    //clock for all threads
    chai3d::cPrecisionClock overallClock;

    //clock for showing temp transparent, then moving to next trial
    chai3d::cPrecisionClock palpPostTrialClock;

    //clock for doing the startup trajectory
    chai3d::cPrecisionClock calibClock;

    //clock for impulse function
    chai3d::cPrecisionClock impulseClock;

    //clock for torque impulse function
    chai3d::cPrecisionClock impulseTorqueClock;

    //clock for gap between impulses
    chai3d::cPrecisionClock impulseDelayClock;

    //clock for gap between impulses torques
    chai3d::cPrecisionClock impulseTorqueDelayClock;

    // determine start time for bandwidth sin
    double sinStartTime;

    // determine start time for circ
    double circStartTime;

    // Declare the control state that we are in
    control_states currentControlState;

    // Declare the environment state that we are in
    environment_states currentEnvironmentState;

    // The experiment state we are in
    experiment_states currentExperimentState;

    // Which one of the dynamic object states we are in
    dynamicObject_states currentDynamicObjectState;

    // Which vt feedback condition we are in
    vt_feedback_states current_vt_feedback_state;
    int current_vt_feedback_state_num;

    // Which 3dof feedback condition we are in
    Dof3_feedback_states current_3Dof_feedback_state;
    int current_3Dof_feedback_state_num;

    // circle drawing radius
    double circRadius;

    // check whether to record
    bool recordFlag;

    bool hapticsThreadActive;
    double hapticRateEstimate;

    // have window thread tell haptics thread to render transparent temporarily
    bool tempTransparentFlag;

    // our data storage variable
    DataRecordStruct dataRecorder;
    std::vector<DataRecordStruct> dataRecorderVector;

    // bandwidth sin variables
    double bandSinAmpDisp;
    double bandSinFreqDisp;
    double bandSinAmp;
    double bandSinFreq;

    // position controller variables
    double Kp;
    double Kd;

    // vars for plotting real time force and filtered force from haptics thread
    chai3d::cVector3d deviceComputedForce;
    chai3d::cVector3d filteredDeviceComputedForce;

    // joint controller variables
    double jointKp;
    double jointKd;

    // device initing flags
    bool device0Initing;
    bool device1Initing;

    // the trial number of the experiment
    int trialNo;
    // are we doing the first or second of the comparison
    int pairNo;
    // Is this trial reference or comparison first
    bool referenceFirst;
    // for reading in the friction values
    double comparisonFriction;
    double referenceFriction;
    // for reading in the experiment feedback condition
    int tactileFeedback;

    bool jakeRender;

    // answer to which was stiffer
    int subjectAnswer;

    // which sizeWeight box are we using
    int sizeWeightBox;
    double sizeWeightBoxMass;
    double sizeWeightStandardMass;

    // what does the participant respond
    double subjectResponseWeight;

    // workspace scale factor
    double workspaceScaleFactor;

    // Which block are we display 1 = Reference, 0 = Comparison
    int Displaying;

    // If the refence comes first =1, if comp comes first = 0;
    int ReferenceFirst;

    // added displacement from impulse
    chai3d::cVector3d globalImpulseDir;
    chai3d::cVector3d localImpulseTorque;

    // protocol loading
    QString frictionProtocolLocation;

    //Protocol loading For Mine's Stiffness and Stiffness-Mass Experiments
    QString MineProtocolLocation;
    CSimpleIniA MineProtocolFile;

    //Protocol loading For Jasmin's Experiments
    QString protocolFileLocation;
    CSimpleIniA selectedProtocolFile;

    QString palpationProtocolLocation;
    QString palpationLineProtocolLocation;
    QString sizeWeightProtocolLocation;
    CSimpleIniA frictionProtocolFile;
    CSimpleIniA palpationProtocolFile;
    CSimpleIniA palpationLineProtocolFile;
    CSimpleIniA sizeWeightProtocolFile;
    CSimpleIniA InertiaProtocolFile;

    // haptics thread objects for palpation environment
    chai3d::cMultiMesh* p_table;
    chai3d::cMultiMesh* p_petriDish;
    chai3d::cMultiMesh* p_tissueOne;
    chai3d::cMultiMesh* p_tissueTwo;
    chai3d::cMultiMesh* p_tissueThree;
    chai3d::cMultiMesh* p_tissueFour;
    chai3d::cMultiMesh* p_tissueFive;
    chai3d::cMultiMesh* p_tissueSix;
    chai3d::cMultiMesh* p_tissueSeven;
    chai3d::cMultiMesh* p_tissueEight;
    chai3d::cMultiMesh* p_tissueNine;
    chai3d::cMultiMesh* p_tissueTen;
    chai3d::cMultiMesh* p_tissueEleven;
    chai3d::cMultiMesh* p_tissueTwelve;
    chai3d::cMultiMesh* p_indicator;

    //haptics thread objects for friction environment
    //chai3d::cMesh* p_frictionBox1; //mesh for left friction surface
    //chai3d::cMesh* p_frictionBox2; //mesh for right friction surface

    // testing textures
    //chai3d::cMultiMesh* p_textureBox;

    // haptics thread objects for friction experiment
    chai3d::cMesh* p_expFrictionBox;

    // haptics thread objects for palpation experiment
    chai3d::cMesh* p_tissueCyl;
    chai3d::cMesh* p_tissueLump;
    chai3d::cMesh* p_tissueLumpCenter;
    chai3d::cMesh* p_tissueLumpCenter1;
    chai3d::cMesh* p_tissueLumpCenter2;
    chai3d::cMesh* p_tissueLumpCenter3;
    chai3d::cMesh* p_tissueBox;

    // rotation of line in tissue
    //double tissueRot;
    double indicatorRot;

    // haptics thread dynamic object for demoing adjustability
    cODEGenericBody* ODEAdjustBody; // dynamic object for adjust box
    chai3d::cMesh* adjustBox; // mesh for adjust box
    cODEGenericBody* ODEAdjustBody1; // dynamic object for adjust box
    chai3d::cMesh* adjustBox1; // mesh for adjust box

    // parameters for the adjusted dynamic box demo
    double adjustedMass;
    double adjustedDynamicFriction;
    double adjustedStaticFriction;
    double adjustedStiffness;
    double adjustedDynamicForceReduction;
    double norm;
    double lat;
    double adjustedForceToPosMult_Normal;
    double adjustedForceToPosMult_Shear;

    bool showCursorFrames;

    // haptics thread objects for Dynamic (ODE) environments
    cODEGenericBody* ODEBody1; //ODE body for box 1
    cODEGenericBody* ODEBody2; //ODE body for box 2
    cODEGenericBody* ODEBody3; //ODE body for box 3
    cODEGenericBody* ODEBody4; //ODE body for box 4
    cODEGenericBody* ODEHoop1; //ODE body for dynamic hoop 1 -- added for HME
    cODEGenericBody* ODEHoop2; //ODE body for dynamic hoop 1 -- added for HME
    cODEGenericBody* ODEWall; //ODE body for center wall -- added for HME

    // haptics thread objects for visual representation of dynamic objects
    chai3d::cMesh* p_dynamicBox1; // mesh for box 1
    chai3d::cMesh* p_dynamicBox2; // mesh for box 2
    chai3d::cMesh* p_dynamicBox3; // mesh for box 3
    chai3d::cMesh* p_dynamicBox4; // mesh for box 4
    chai3d::cMesh* p_dynamicHoop1; // mesh for hoop 1 -- added for HME
    chai3d::cMesh* p_dynamicHoop2; // mesh for hoop 1 -- added for HME
    chai3d::cMultiMesh* p_boxWithHole; // mesh for p_boxWithHole -- added for HME


    //Walls:
    chai3d::cMesh* p_wall; // mesh for center wall
    chai3d::cMesh* p_backWall; // mesh for backWall -- Added for HME
    chai3d::cMesh* p_sideWall1; // mesh for sideWall1 -- Added for HME
    chai3d::cMesh* p_sideWall2; // mesh for sideWall2 -- Added for HME

    // meshes for display scaled objects
    chai3d::cMesh* p_dynamicScaledBox1;
    chai3d::cMesh* p_dynamicScaledBox2;
    chai3d::cMesh* p_dynamicScaledBox3;
    chai3d::cMesh* p_dynamicScaledBox4;

    chai3d::cMultiMesh* oneModel;
    chai3d::cMultiMesh* twoModel;

    // initial position of boxes (center of warping for display scaled boxes)
    chai3d::cVector3d box1InitPos;
    chai3d::cVector3d box2InitPos;
    chai3d::cVector3d box3InitPos;

    chai3d::cVector3d box1PostInitCenter;
    chai3d::cVector3d box2PostInitCenter;
    chai3d::cVector3d box3PostInitCenter;


    // flag for reseting box position during experiment if the standard reset causes issue
    bool resetBoxPosFlag;

    // flag for determining if display scaling is clutched
    bool fingerTouching;
    bool thumbTouching;
    bool fingerTouchingLast;
    bool thumbTouchingLast;

    double ActiveBox;

    // center position of current display scale warping for finger or box
    chai3d::cVector3d boxScaleCenter;
    chai3d::cVector3d fingerScalePoint;
    chai3d::cVector3d clutchedOffset;

    // storing how much we are display scaling
    double box1displayScale;
    double fingerDisplayScale;
    double sliderWeight;
    int scaledDispTransp;
    //int expDone;


    // vars for tracking CD exp params
    int isUpperCurve; // 1 upper, 0 lower
    int isRef; // 1 ref, 0 compare
    double upperCurveMass;
    double lowerCurveMass;
    int upperCurveReversals;
    int lowerCurveReversals;
    int reversalReductionUpper;
    int reversalReductionLower;
    int CDSet;
    int currChoice;
    int maxReversals;
    int isReversal;

    double upperCurveIncrement;
    double lowerCurveIncrement;

    int lastUpperCurveRefHeavier; // record whether the last choice for this curve was ref or compare, to know whether we had a "reversal"
    int lastLowerCurveRefHeavier;

    double refCD;
    double compareCD;
    double strokeScale;
    Eigen::Vector3d desiredFor0;
    Eigen::Vector3d desiredFor1;
    // Added for Hoxel Mapping Experiment:
    //Local to fingers Normal Forces:
    Eigen::Vector3d desiredNormFor0;
    Eigen::Vector3d desiredNormFor1;
    //Local to fingers Tangential Forces:
    Eigen::Vector3d desiredTanFor0;
    Eigen::Vector3d desiredTanFor1;

    double ref1;
    double ref2;
    //double stiffness;
    double stiffness1;
    double stiffness2;
    double stiffness3;
    double mass1;
    double mass2;
    double mass3;
    double lev1;
    double lev2;
    double EXPERIMENTSTIFFNESS;
    double cond;
    int direct;

    //double refMass; //compare mass set by upper and lower curves
    double refInertia;

    double compCD;
    double compMass;
    double compInertia;

    //double tempCD; //CD messes up on restart - keep variable in a temp for consistency

    double expCD;
    //double expInertia;
    //double expMass;
    //int day;

    //bool diffStiffSubjective;
    //bool diffFrictSubjective;
    bool diffStiffSubjectiveRead;
    bool diffFrictSubjectiveRead;

    bool target1Responded;
    bool target2Responded;

    bool target1Complete;
    bool target2Complete;
    bool target3Complete;

    bool hoop1Complete;
    bool hoop2Complete;

    bool cubeLiftComplete; //Added for Jasmin's Cube ID experiment

    bool explorationComplete;
    bool answerComplete;

    bool answer1;
    bool answer2;

    //bool cdVertScaleOn;
    // tiger vars for tracking CD inertia subjective values
    // int isRef; // 1 ref, 0 compare
    //int currChoice;
    double subjCDArr[3];
    double subjInertiaArr[3];

    int    randList[36];
    double magCDArr[144];
    double magMassArr[144];
    double magInertiaArr[144];

    int cdInertiaSubjState;
    int magCounter;

    //bool trainingOn;

    // flags for environment change and tissue transparency
    bool environmentChange;
    bool m_flagTissueTransparent;

    // recording variables
    QString dir;
    QString fileName;

    // init joint angles
    double desJointInits0; //Eigen::Vector3d desJointInits0;
    double desJointInits1; //Eigen::Vector3d desJointInits1;

    // flags for only normal or only lateral control
    //bool flagLateral;
    //bool flagNormal;
    bool flagEqual;
    bool flagEqualTouch;
    bool flagReverse;
    bool flagDorsal;
    bool flagVentral;
    bool flagMassExp;

    bool flagStiff1;
    bool flagStiff2;

    double Inertial_Scaling_Factor;
    double Inertial_Scaling_Mass;
    double ReferenceMass;
    double ComparisonMass;
    //double InertiaBlockMass;
    //double blockNo;
    double blockSize;
    double subjectNo;
    double AdjustedTrialNo;
    double CDInertial_Scaling_Factor;

    QString TrialType;
    double TrialMode;

    chai3d::cVector3d gravity;

    //Subjective Inertia
    double Subjective_Pair_Num;

    //Vibraitons active
    bool VibroTactileThreadActive;

    //Vibration Monitoring

    bool playVib0;
    bool playingVib0;

    bool playVib1;
    bool playingVib1;

    bool show_forces;
    int texture;

    bool ImpulseVib;
    bool SDVib;

    double ImpulseAmp;
    double SDVibAmp;
    double SDVibFreq;
    double SDVibBeta;

    //bool JustVib;

    bool Break;

    bool Decaying;

    chai3d::cVector3d Velocity0;
    chai3d::cVector3d Velocity1;

    double Mass; //for Vt trials
    double Fragility; //for VT trials

    chai3d::cVector3d last_pos;
    bool ThroughHoop;

    int ThroughHoop_Num;
    int Break_Num;

    bool VT_Feedback;

    int StartTime;

    double Acc;

    bool Impulse0;
    bool Impulse1;

    double settable_inertial_scaling;

    //bool resetCDFlag;
    bool pauseTrial;

    //Mappings For Jasmin's Finger Mapping Experiments
    int mapping;
    bool hoopSuccess;
    bool targetSuccess;
    bool trialSuccess;

    //Counter for number of times the user presses "H" prematurely
    int mistakeCounter;

    //Boolean for enabling/disabling planar motion constraints for HME
    bool enablePlanarConstraint;

    //Parameter for deciding which pseudo random start position the box in CGE starts at
    int boxInitRandPos;
    //X and Y positions for randoms box initial positions
    double randBoxPosX;
    double randBoxPosY;

    //For HME to see if grasp force is too high
    bool manipForceTooHigh;
    int hmeExpType;
    chai3d::cVector3d boxInteractionForce;

    //For HME: deciding when to remind subject of experiment conditions
    bool remindSubject;

    bool showFingerProxyAlgPoints;
    chai3d::cVector3d indexProxyRelPos;
    chai3d::cVector3d thumbProxyRelPos;

    //Parameters for WHC Demo
    bool whcDemo;
    QString cubeID0;
    QString cubeID1;

} shared_data;

#endif // SHARED_DATA_H

