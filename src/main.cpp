#include <QApplication>
#include "mainwindow.h"
#include "mychai3dwindow.h"
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
    QApplication app(argc, argv);
    MainWindow controlWindow;
    myCHAI3DWindow chai3DWindow;
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

    magTrackerThread.initialize();
    qDebug()<<"magTrackerThread initialized";/**/

    //VibrotactileThread.initialize();
    //qDebug()<<"VibrotactileThread initialized";

    // Initialize and set the controlWindow thread data pointer to the shared data
    controlWindow.p_CommonData = &shared;
    controlWindow.Initialize();
    //Move control screen over, then maximze to guarantee control window is on screen2
    controlWindow.move(2540,0);
    //controlWindow.show();
    controlWindow.showMaximized();

    //controlWindow.setScreen(screen1);

    qDebug()<<"controlWindow Initialized";

    // Initialize and set the chai3DWindow thread data pointer to the shared data
    chai3DWindow.p_CommonData = &shared;
    chai3DWindow.Initialize();
    //chai3DWindow.show();
    chai3DWindow.showMaximized();
    //chai3DWindow.showFullScreen();
    qDebug()<<"CHAI3D Window Initialized";

    hapticsThread.start();
    experimentThread.start();
    magTrackerThread.start();
    //VibrotactileThread.start();

    return app.exec();
}



