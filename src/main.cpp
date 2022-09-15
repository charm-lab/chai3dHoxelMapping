#include <QApplication>
#include "mainwindow.h"
#include "shared_data.h"
#include "haptics_thread.h"
#include "experiment_thread.h"
#include "magTracker_thread.h"

//#include "Vibrotactile_thread.h"
#include <boost/random.hpp>


// Variable Declarations ============================================
shared_data shared; //create the shared_data structure for sharing

// Testing Variable Declaration =====================================

// Function Declarations ============================================

// Create Threads ===================================================
haptics_thread hapticsThread;
Experiment_Thread experimentThread;
magTracker_thread magTrackerThread;
//Vibrotactile_thread VibrotactileThread;

// MAIN FUNCTION ====================================================
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;
    // Create a chai device for mag tracker
    shared.chaiMagDevice0 = chai3d::cGenericHapticDevicePtr((chai3d::cGenericHapticDevice *)(new chai3d::c3dofChaiDevice(0)));
    shared.chaiMagDevice1 = chai3d::cGenericHapticDevicePtr((chai3d::cGenericHapticDevice *)(new chai3d::c3dofChaiDevice(1)));

    // Create a haptic device
    shared.wearableDelta0 = new c3DOFDevice(0);     // create index
    shared.wearableDelta0->Init3DOFDeviceEnc();     // init index

    shared.wearableDelta1 = new c3DOFDevice(1);     // create thumb
    shared.wearableDelta1->Init3DOFDeviceEnc();     // init thumb

    // Initialize and set the haptics thread data pointer to the shared data
    hapticsThread.p_CommonData = &shared;
    experimentThread.p_CommonData = &shared;
    magTrackerThread.p_CommonData = &shared;
    //VibrotactileThread.p_CommonData = &shared;

    // Initialize the threads
    hapticsThread.initialize();
    qDebug()<<"hapticsThread initialized";

    experimentThread.initialize();
    qDebug()<<"experimentThread initialized";

    //magTrackerThread.initialize();
    qDebug()<<"magTrackerThread initialized";

    //serialPort.initialize();
    qDebug()<<"serialPort initialized";

    //VibrotactileThread.initialize();
    //qDebug()<<"VibrotactileThread initialized";

    // Initialize and set the window thread data pointer to the shared data
    window.p_CommonData = &shared;
    window.Initialize();
    window.show();
    qDebug()<<"window";


    hapticsThread.start();
    experimentThread.start();
    //magTrackerThread.start();
    //VibrotactileThread.start();

    return a.exec();
}



