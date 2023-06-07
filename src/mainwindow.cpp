#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string.h>
#include "Widget_OpenGLDisplay.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iomanip>
#include <string>
#include <QTime>
#include <QMessageBox>

#include "breaktimedialog.h"

//***Define directory/folder for Subjects' experiment info***
//Make sure this directory exists before runtime or else the data will not save
//QString subjectDirectory = "C:/Users/Sam/Desktop/chai3dFingerMapping/Subjects/";
//QString subjectDirectory = "F:/FME_Subjects/";
//QString subjectDirectory = "./FME_Subject_Data/";

//Gloabal value to be shared with cMotorController
int mappingVal;

//Set Baud rate of the motor controller board and other default configs
int myBaudRate = 115200;
int defaultBitNum = 8;
int defaultParity = 0;
int defaultStopBit = 1;

//Set Device Desired Forces or positions:
//Dorsal desiered values:
QString device0X; //X
QString device0Y; //Y
QString device0Z; //Z
QString dev0Mag;
//Ventral desiered values:
QString device1X; //X
QString device1Y; //Y
QString device1Z; //Z
QString dev1Mag;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //#ifndef OCULUS
    //    //windowGLDisplay = new Widget_OpenGLDisplay(this->centralWidget());
    //    //Make the window that the environment appears the one that is editable in mainwindow.ui
    //    windowGLDisplay = new Widget_OpenGLDisplay(ui->openGLWidget);
    //    //Changes position and size of CHAI3D window:
    //    windowGLDisplay->setGeometry(QRect(0, 0, ui->openGLWidget->geometry().width(), ui->openGLWidget->geometry().height()));
    //#endif

    //Find available serial ports
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->openButton->setText(tr("Open\nSerial\nPort"));
            //Set Options to the drop down boxes
            ui->PortBox->addItem(serial.portName());
            ui->BaudBox->addItem(QString::number(myBaudRate));
            ui->BitNumBox->addItem(QString::number(defaultBitNum));
            ui->ParityBox->addItem(QString::number(defaultParity));
            ui->StopBox->addItem(QString::number(defaultStopBit));
            serial.close();
        }
        else
        {
            ui->openButton->setText(tr("Serial\nPort\nUnavailable"));
        }
    }
    //The setting COM drop-down menu display the 2nd (first-index) item by default
    ui->PortBox->setCurrentIndex(0);
    //All other drop-down menu displays the first (zero-index) item by default
    ui->BaudBox->setCurrentIndex(0);
    ui->BitNumBox->setCurrentIndex(0);
    ui->ParityBox->setCurrentIndex(0);
    ui->StopBox->setCurrentIndex(0);
    qDebug() << tr("The interface is set successfully!");

}

MainWindow::~MainWindow()
{
    p_CommonData->hapticsThreadActive = false;
    p_CommonData->VibroTactileThreadActive = false;
    delete ui;
}

QString MainWindow::mapFingersToDevices()
{
    //Normal Mapping
    if(p_CommonData->mapping == 1)
    {
        //Set Device Desired Forces:
        //Dorsal - index desired forces:
        device0X = QString::number(localForce0[0], 'f', 1); //localForce0[0] //X
        device0Y = QString::number(localForce0[1], 'f', 1); //localForce0[1] //Y
        device0Z = QString::number(localForce0[2], 'f', 1); //localForce0[2] //Z
        dev0Mag = QString::number(localForce0.norm(), 'f', 1);
        //Ventral - thumb desired forces:
        device1X = QString::number(localForce1[0], 'f', 1); //localForce1[0] //X
        device1Y = QString::number(localForce1[1], 'f', 1); //localForce1[1] //Y
        device1Z = QString::number(localForce1[2], 'f', 1); //localForce1[2] //Z
        dev1Mag = QString::number(localForce1.norm(), 'f', 1);
    }
    //Reverse Mapping
    if(p_CommonData->mapping == 2)
    {
        //Set Device Desired Forces:
        //Dorsal - thumb desired forces:
        device0X = QString::number(localForce1[0], 'f', 1); //localForce1[0] //X
        device0Y = QString::number(localForce1[1], 'f', 1); //localForce1[1] //Y
        device0Z = QString::number(localForce1[2], 'f', 1); //localForce1[2] //Z
        dev0Mag = QString::number(localForce1.norm(), 'f', 1);
        //Ventral - index desired forces:
        device1X = QString::number(localForce0[0], 'f', 1); //localForce0[0] //X
        device1Y = QString::number(localForce0[1], 'f', 1); //localForce0[1] //Y
        device1Z = QString::number(localForce0[2], 'f', 1); //localForce0[2] //Z
        dev1Mag = QString::number(localForce0.norm(), 'f', 1);
    }
    //Single Mapping
    if(p_CommonData->mapping == 3)
    {
        //Set Device Desired Forces:
        //Dorsal - index desired forces:
        device0X = QString::number(localForce0[0], 'f', 1); //localForce0[0] //X
        device0Y = QString::number(localForce0[1], 'f', 1); //localForce0[1] //Y
        device0Z = QString::number(localForce0[2], 'f', 1); //localForce0[2] //Z
        dev0Mag = QString::number(localForce0.norm(), 'f', 1);
        //Ventral - thumb desired forces:
        //localForce1[0] = 0.0;
        //localForce1[1] = 0.0;
        //localForce1[2] = 0.0;
        device1X = QString::number(0.0); //localForce1[0] //X
        device1Y = QString::number(0.0); //localForce1[1] //Y
        device1Z = QString::number(0.0); //localForce1[2] //Z
        dev1Mag = QString::number(0.0, 'f', 1);
    }
    //Averaged
    if(p_CommonData->mapping == 4)
    {
        //Set Device Desired Forces:
        //Dorsal - index desired forces:
        device0X = QString::number(0.5*(localForce0[0] + localForce1[0]), 'f', 1); //localForce0[0] //X
        device0Y = QString::number(0.5*(localForce0[1] + localForce1[1]), 'f', 1); //localForce0[1] //Y
        device0Z = QString::number(0.5*(localForce0[2] + localForce1[2]), 'f', 1); //localForce0[2] //Z
        dev0Mag = QString::number(0.5*(localForce0.norm() + localForce1.norm()), 'f', 1);
        //Ventral - thumb desired forces:
        device1X = QString::number(0.0); //localForce1[0] //X
        device1Y = QString::number(0.0); //localForce1[1] //Y
        device1Z = QString::number(0.0); //localForce1[2] //Z
        dev1Mag = QString::number(0.0, 'f', 1);
    }
    //Control - No Haptic Feedback
    if(p_CommonData->mapping == 5)
    {
        //Set Device Desired Forces:
        //Dorsal - index desired forces:
        device0X = QString::number(0.0); //localForce0[0] //X
        device0Y = QString::number(0.0); //localForce0[1] //Y
        device0Z = QString::number(0.0); //localForce0[2] //Z
        dev0Mag = QString::number(0.0, 'f', 1);
        //Ventral - thumb desired forces:
        device1X = QString::number(0.0); //localForce1[0] //X
        device1Y = QString::number(0.0); //localForce1[1] //Y
        device1Z = QString::number(0.0); //localForce1[2] //Z
        dev1Mag = QString::number(0.0, 'f', 1);
    }
    else
    {

    }

    //dev0Mag = QString::number(10.0, 'f', 1);
    //dev1Mag = QString::number(10.0, 'f', 1);
    //Dispay in GUI:

    ui->serialWrite1->setText("New: " + device0X + " | " + device0Y + " | " + device0Z + "N\r\n" + "Mag: " + dev0Mag + "N\r\n"); //device 0 //device 0 _prev
    ui->serialWrite2->setText("New: " + device1X + " | " + device1Y + " | " + device1Z + "N\r\n" + "Mag: " + dev1Mag + "N\r\n"); //device 1 //device 1_prev

    QString deviceData = device0X + " " + device0Y + " " + device0Z + " " + dev0Mag + " " + device1X + " " + device1Y + " " + device1Z + " " + dev1Mag + "\r\n";
    return deviceData;
}

//START SERIAL

//Serial data writing happens here:
void MainWindow::writeSerialData()
{
    QByteArray payloadBuffer;

    //Mappings:
    QString serialData = mapFingersToDevices();

    //Set limit for CrumblyCubeExp
    if (p_CommonData->currentDynamicObjectState == CrumblyCubeExperiment)
    {
//        if(localForce0.norm()+localForce1.norm() > FINGER_FORCE_LIMIT)
//        {
//            p_CommonData->manipForceTooHigh = true;
//        }
//        else
//        {
//            p_CommonData->manipForceTooHigh = false;
//        }
    }
    payloadBuffer = payloadBuffer.append(serialData);

    //qDebug() << payloadBuffer << " " << serial->isWritable();

    //qDebug() << p_CommonData->mapping;

    if(serial->isWritable())
    {
        //ui->serialWrite2->toPlainText().toLatin1();
        //Write data to board
        payloadBuffer = payloadBuffer.append(serialData);
        serial->write(payloadBuffer, payloadBuffer.size());
    }
}

//Read received data
void MainWindow::readSerialData()
{
    QByteArray buf;
    buf = serial->readAll();
    if(!buf.isEmpty())
    {
        QString str = ui->serialRead->toPlainText();
        str+=tr(buf);
        //ui->serialRead->clear();
        //ui->serialRead->append(str);
        ui->serialRead->setText(str);
    }
    buf.clear();
}

void MainWindow::on_openButton_clicked()
{
    if(ui->openButton->text()==tr("Open\nSerial\nPort"))
    {
        serial = new QSerialPort;
        //Set serial port name
        serial->setPortName(ui->PortBox->currentText());

        //Open serial port
        serial->open(QIODevice::ReadWrite);

        //set baud rate
        serial->setBaudRate(ui->BaudBox->currentText().toInt());

        //Set the number of data bits
        switch(ui->BitNumBox->currentIndex())
        {
        case 8: serial->setDataBits(QSerialPort::Data8); break;
        default: break;
        }

        //Set parity
        switch(ui->ParityBox->currentIndex())
        {
        case 0: serial->setParity(QSerialPort::NoParity); break;
        default: break;
        }

        //Set stop bit
        switch(ui->StopBox->currentIndex())
        {
        case 1: serial->setStopBits(QSerialPort::OneStop); break;
        case 2: serial->setStopBits(QSerialPort::TwoStop); break;
        default: break;
        }

        //set flow control
        serial->setFlowControl(QSerialPort::NoFlowControl);
        //Turn off settings menu enable
        ui->PortBox->setEnabled(false);
        ui->BaudBox->setEnabled(false);
        ui->BitNumBox->setEnabled(false);
        ui->ParityBox->setEnabled(false);
        ui->StopBox->setEnabled(false);
        ui->openButton->setText(tr("Close\nSerial\nPort"));
        //Connecting signal slot
        QObject::connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);

        QTimer *timer0 = new QTimer(this);
        connect(timer0, SIGNAL(timeout()), this, SLOT(writeSerialData()));

        timer0->start(10);
        //close PCI control:
#ifdef SENSORAY826
        S826_SystemClose();
        qDebug()<<"Closing PCI Control -- S826_SystemClose()";
#endif

        qDebug()<<"Open Serial Port Success~~";
    }
    else
    {
        //Close the serial port
        ui->openButton->setText(tr("Bye!\nBye!\n（っ＾▿＾）"));
        serial->clear();
        serial->close();
        serial->deleteLater();
        //Restore settings enable
        ui->PortBox->setEnabled(true);
        ui->BaudBox->setEnabled(true);
        ui->BitNumBox->setEnabled(true);
        ui->ParityBox->setEnabled(true);
        ui->StopBox->setEnabled(true);
        //Shutdown app gracefully lol
        //QThread::msleep(1500);
        //exit(EXIT_FAILURE);
    }
}

//END SERIAL
void MainWindow::Initialize()
{
    graphicsRateClock.reset();
    graphicsRateClock.setTimeoutPeriodSeconds(1.0);
    graphicsRateClock.start(true);

#ifdef OCULUS
    //--------------------------------------------------------------------------
    // SETUP OCULUS DISPLAY CONTEXT
    //--------------------------------------------------------------------------

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        cout << "failed initialization" << endl;
        QThread::msleep(1000);
    }

    if (!oculusVR.initVR())
    {
        cout << "failed to initialize Oculus" << endl;
        QThread::msleep(1000);
        SDL_Quit();
    }
    ovrSizei hmdResolution = oculusVR.getResolution();
    ovrSizei windowSize = { hmdResolution.w / 2, hmdResolution.h / 2 };

    renderContext.init("CHAI3D", 100, 100, windowSize.w, windowSize.h);
    SDL_ShowCursor(SDL_DISABLE);

    if (glewInit() != GLEW_OK)
    {
        oculusVR.destroyVR();
        renderContext.destroy();
        SDL_Quit();
    }

    if (!oculusVR.initVRBuffers(windowSize.w, windowSize.h))
    {
        oculusVR.destroyVR();
        renderContext.destroy();
        SDL_Quit();
    }
#endif
    
    // Set our current state
    p_CommonData->currentControlState = idleControl;
    //    p_CommonData->currentControlState = VRControlMode;
    
    
    //#ifndef OCULUS
    //    windowGLDisplay->p_CommonData = p_CommonData;
    //#endif

    connect(this->ui->Act1, SIGNAL(valueChanged(int)), this, SLOT(onGUIchanged()));
    connect(this->ui->Act2, SIGNAL(valueChanged(int)), this, SLOT(onGUIchanged()));

    connect(this->ui->equalBoxTouch, SIGNAL(stateChanged(int)), this, SLOT(onGUIchanged()));
    connect(this->ui->AdjustTrialNo, SIGNAL(stateChanged(int)), this, SLOT(onGUIchanged()));

    connect(this->ui->Condition1,  SIGNAL(clicked()), this, SLOT(onGUIchanged()));
    
    connect(this->ui->VRControl, SIGNAL(clicked()), this, SLOT(onGUIchanged()));
    
    //    this->Inertia_Scaling_Mass=.2;
    //    this->Inertia_Scaling_Factor=1;
    
    p_CommonData->indicatorRot = 0;
    //    p_CommonData->tissueRot = 0;
    //    p_CommonData->stiffness = 200;
    
    p_CommonData->adjustedDynamicForceReduction = 1.0;
    p_CommonData->adjustedForceToPosMult_Normal = 2.0; // this means 0.5 N/mm
    p_CommonData->adjustedForceToPosMult_Shear  = 5.0; // this means 0.2 N/mm

    p_CommonData->stiffness1 = 100;
    p_CommonData->stiffness2 = 300;
    p_CommonData->stiffness3 = 700;

    p_CommonData->mass1 = 0.3;
    p_CommonData->mass2 = 0.3;
    p_CommonData->mass3 = 0.3;
    p_CommonData->cond = 1;
    p_CommonData->direct = 1;
    p_CommonData->mapping = 1;

    p_CommonData->ActiveBox = 1;
    p_CommonData->lev1 = 1;
    p_CommonData->lev2 = 1;

    ui->equalBoxTouch->setChecked(true);
    ui->AdjustTrialNo->setChecked(false);

    p_CommonData->flagEqual = false;
    p_CommonData->flagEqualTouch = true;
    p_CommonData->flagReverse = false;

    ui->Condition1->setChecked(true);
    p_CommonData->flagMassExp = false;

    p_CommonData->device0Initing = false;
    p_CommonData->device1Initing = false;
    p_CommonData->resetBoxPosFlag = false;

    //Initialize Camera:
    //ui->cameraPos_BoxX->setValue(p_CommonData->cameraPos.get(0));
    ui->cameraPolar_Box->setValue(p_CommonData->polar);
    ui->cameraRadius_Box->setValue(p_CommonData->camRadius);
    ui->cameraAzimuth_Box->setValue(p_CommonData->azimuth);
    //Cartesian:

    // Save initial camera values for reset
    initOffset = p_CommonData->offset;
    initPolar = p_CommonData->polar;
    initCamRadius = p_CommonData->camRadius;
    initAzimuth = p_CommonData->azimuth;

    initCamPosX = 0.0;
    initCamPosY = 0.1;
    initCamPosZ = 0.1;

    p_CommonData->lookatPosX = initCamPosX;
    p_CommonData->lookatPosY = initCamPosY;
    p_CommonData->lookatPosZ = initCamPosZ;

    ui->cameraXPosBox->setValue(p_CommonData->lookatPosX);
    ui->cameraYPosBox->setValue(p_CommonData->lookatPosY);
    ui->cameraZPosBox->setValue(p_CommonData->lookatPosZ);
    //Initilize planar constraint box
    ui->planarCheckBox->setChecked(false);

    ui->SetTrialNoButton->setEnabled(false);

#ifdef QWT
    ///////////////
    // QWT INITS //
    ///////////////
    i = 0;
#endif

    GraphicsTimer.setInterval(1);
    GraphicsTimer.start();
    connect(&GraphicsTimer, SIGNAL(timeout()), this, SLOT(UpdateGUIInfo()));
    UpdateGUIInfo();
}

//Updates Information Displays in the Window
void MainWindow::UpdateGUIInfo()
{
    graphicsRateCounter += 1;
    if(graphicsRateClock.timeoutOccurred())
    {
        //graphicsRateClock.stop();
        graphicsRateEstimate = graphicsRateCounter;
        graphicsRateCounter = 0;
        graphicsRateClock.reset();
        graphicsRateClock.start();
    }
#ifdef OCULUS
    // Handle Oculus events
    // handle key presses
    processEvents();

    // start rendering
    oculusVR.onRenderStart();

    // render frame for each eye
    for (int eyeIndex = 0; eyeIndex < ovrEye_Count; eyeIndex++)
    {
        // retrieve projection and modelview matrix from oculus
        cTransform projectionMatrix, modelViewMatrix;
        oculusVR.onEyeRender(eyeIndex, projectionMatrix, modelViewMatrix);

        p_CommonData->p_camera->m_useCustomProjectionMatrix = true;
        p_CommonData->p_camera->m_projectionMatrix = projectionMatrix;

        p_CommonData->p_camera->m_useCustomModelViewMatrix = true;
        p_CommonData->p_camera->m_modelViewMatrix = modelViewMatrix;

        // render world
        ovrSizei size = oculusVR.getEyeTextureSize(eyeIndex);
        p_CommonData->p_camera->renderView(size.w, size.h, 0, C_STEREO_LEFT_EYE, false);

        // finalize rendering
        oculusVR.onEyeRenderFinish(eyeIndex);
    }

    // update frames
    oculusVR.submitFrame();
    oculusVR.blitMirror();
    SDL_GL_SwapWindow(renderContext.window);
#endif

    if (p_CommonData->currentDynamicObjectState == manual)
        localOutputStrokes0 = p_CommonData->ref1;
    else
        localOutputStrokes0 = p_CommonData->wearableDelta0->ReadStrokeOutput();

    localDesiredPos0 = p_CommonData->wearableDelta0->ReadDesiredPos();
    localMotorAngles0 = p_CommonData->wearableDelta0->GetJointAngles();
    localForce0         = p_CommonData->desiredFor0;

    double localFor0_0     =  localForce0[0];
    double localFor0_1     =  localForce0[1];
    double localFor0_2     =  localForce0[2];

    if (p_CommonData->currentDynamicObjectState == manual)
        localOutputStrokes1 = p_CommonData->ref2;
    else
        localOutputStrokes1 = p_CommonData->wearableDelta1->ReadStrokeOutput();

    localDesiredPos1 = p_CommonData->wearableDelta1->ReadDesiredPos();
    localMotorAngles1 = p_CommonData->wearableDelta1->GetJointAngles();
    localForce1         = p_CommonData->desiredFor1;

    double localFor1_0     =  localForce1[0];
    double localFor1_1     =  localForce1[1];
    double localFor1_2     =  localForce1[2];

    if(p_CommonData->flagEqualTouch)
    {
        if (p_CommonData->fingerTouching == true && p_CommonData->thumbTouching == true)
        {
            localOutputStrokes0 = p_CommonData->wearableDelta0->ReadStrokeOutput();
            localOutputStrokes1 = p_CommonData->wearableDelta0->ReadStrokeOutput();
        }
    }

#ifdef QWT
    ////////////////////////////////////////////////////
    // update real time plotting
    ////////////////////////////////////////////////////
    //ui->qwtPlot->setAxisAutoScale(QwtPlot::yLeft);
    i += 1/updateHz;

    // assign values to first curve
    points1 << QPointF(i, p_CommonData->deviceComputedForce.z());
    if (points1.length() > 5*updateHz)
        points1.removeFirst();

    // assign values to second curve
    points2 << QPointF(i, p_CommonData->filteredDeviceComputedForce.z());
    if (points2.length() > 5*updateHz)
        points2.removeFirst();
#endif

    // index device - send local values
    ui->StrokeLCD1_0->display((localOutputStrokes0));
    ui->DesX0->display(localDesiredPos0[0]);
    ui->DesY0->display(localDesiredPos0[1]);
    ui->DesZ0->display(localDesiredPos0[2]);
    ui->MotorLCD_0->display((localMotorAngles0));
    ui->ForX0->display(localFor0_0);
    ui->ForY0->display(localFor0_1);
    ui->ForZ0->display(localFor0_2);

    //ui->StiX0->display(localStiffn0_0);
    //ui->StiY0->display(localStiffn0_1);
    //ui->StiZ0->display(localStiffn0_2);

    // thumb device - send local values
    ui->StrokeLCD1_1->display((localOutputStrokes1));
    ui->DesX1->display(localDesiredPos1[0]);
    ui->DesY1->display(localDesiredPos1[1]);
    ui->DesZ1->display(localDesiredPos1[2]);
    ui->MotorLCD_1->display((localMotorAngles1));
    ui->ForX1->display(localFor1_0);
    ui->ForY1->display(localFor1_1);
    ui->ForZ1->display(localFor1_2);

    //ui->strokeScale_show->display(p_CommonData->strokeScale);
    ui->Act1_show->display(p_CommonData->ref1);
    ui->Act2_show->display(p_CommonData->ref2);

    ui->stiff1Scale_show->display(p_CommonData->stiffness1);
    ui->stiff2Scale_show->display(p_CommonData->stiffness2);
    ui->stiff3Scale_show->display(p_CommonData->stiffness3);

    ui->mass1_show->display(p_CommonData->mass1);
    ui->mass2_show->display(p_CommonData->mass2);
    ui->mass3_show->display(p_CommonData->mass3);

    ui->condition->display(p_CommonData->cond);
    ui->direction->display(p_CommonData->direct);

    ui->lcdNumberHapticRate->display(p_CommonData->hapticRateEstimate);
    ui->lcdNumberGraphicsRate->display(graphicsRateEstimate);

    ui->trialNo->display(p_CommonData->trialNo);

    // Change to jake render if checked
    if(ui->JakeRenderCheckBox->isChecked())
        p_CommonData->jakeRender = true;
    else
        p_CommonData->jakeRender = false;

    p_CommonData->subjectNo = ui->subject_no->value();
    p_CommonData->AdjustedTrialNo = ui->SetTrialNo->value();

    //if Locked Camera is unchecked, allow adjustments:
    if(!ui->lockCameraBox->isChecked())
    {
        ui->cameraPolar_Box->setEnabled(true);
        ui->cameraRadius_Box->setEnabled(true);
        ui->cameraAzimuth_Box->setEnabled(true);

        ui->cameraXPosBox->setEnabled(true);
        ui->cameraYPosBox->setEnabled(true);
        ui->cameraZPosBox->setEnabled(true);

        //Set the new values to change the camera polar angles
        p_CommonData->polar = ui->cameraPolar_Box->value();
        p_CommonData->camRadius = ui->cameraRadius_Box->value();
        p_CommonData->azimuth = ui->cameraAzimuth_Box->value();

        //Set the new values to change the camera Cartesian coordiantes

        // Update camera Pos
        p_CommonData->lookatPosX = ui->cameraXPosBox->value();
        p_CommonData->lookatPosY = ui->cameraYPosBox->value();
        p_CommonData->lookatPosZ = ui->cameraZPosBox->value();
        //Rest of camera update occures in haptics_thread::UpdateVRGraphics()


    }
    else //disable boxes to lock camera
    {
        ui->cameraPolar_Box->setEnabled(false);
        ui->cameraRadius_Box->setEnabled(false);
        ui->cameraAzimuth_Box->setEnabled(false);

        ui->cameraXPosBox->setEnabled(false);
        ui->cameraYPosBox->setEnabled(false);
        ui->cameraZPosBox->setEnabled(false);
    }

    //Disable planar constraint if not using HME:
    if(!(p_CommonData->currentDynamicObjectState == HoxelMappingExperiment))
    {
        ui->planarCheckBox->setEnabled(false);
    }
    else
    {
        ui->planarCheckBox->setEnabled(true);
        //if planar constraint box checked, restrict motion to xz plane
        if(ui->planarCheckBox->isChecked())
        {
            p_CommonData->enablePlanarConstraint = true;
        }
        else
        {
            p_CommonData->enablePlanarConstraint = false;
        }
    }


    //Trial Adjustment if needed
    if(ui->AdjustTrialNo->isChecked())        //let haptics thread determine desired position
    {
        //qDebug() << "AdjustTrial";
        ui->SetTrialNoButton->setEnabled(true);
        //p_CommonData->trialNo = p_CommonData->AdjustedTrialNo;
        //qDebug() << "New TrialNo: " << p_CommonData->trialNo;

    }/*
    else
    {
        //qDebug() << "DONTAdjustTrial";
        ui->SetTrialNoButton->setEnabled(false);
        //p_CommonData->trialNo = -1;

    }
    */

    //Hide show all frames
    // update frames to show or not
    if (ui->showAllFramesCheckBox->isChecked())
    {
        //Remove existing frames first
        p_CommonData->p_world->setShowFrame(false, true);
        //Show all frames
        p_CommonData->p_world->setShowFrame(true, true);
        //set the size
        p_CommonData->p_world->setFrameSize(0.075, true);
        p_CommonData->adjustBox->setShowFrame(true, true);
    }
    else
    {
        //Remove all the frames
        p_CommonData->p_world->setShowFrame(false, true);
        p_CommonData->adjustBox->setShowFrame(false, true);
    }

    //calibrate if startup process over
    if(p_CommonData->calibClock.timeoutOccurred())
    {
        p_CommonData->calibClock.stop();
        p_CommonData->calibClock.reset();
    }

    //Set manip Boolean for CCE
    if(localForce0.norm()+localForce1.norm() > FINGER_FORCE_LIMIT)
    {
        p_CommonData->manipForceTooHigh = true;        
        if(p_CommonData->cceExpType == 3)
        {
            //Force Trial Progression if force limit is exceeded
            p_CommonData->trialNo = p_CommonData->trialNo++;
            if (readExpStuffIn())
            {
                qDebug()<<"readExpStuffIn() SUCCESS -- Forced Trial Prgoression complete";

                if(p_CommonData->TrialType == "break")
                {
                    // Change and then show message box
                    QString labelText = "<P><b><FONT COLOR='#7abfe4' FONT SIZE = 5>";
                    labelText.append("PRESS NEXT AFTER THE BREAK --");
                    labelText.append("</b></P></br>");

                    ui->text->setText(labelText);
                    qDebug()<<"BREAK "<<p_CommonData->trialNo;
                    // Pop up countdown timer:
                    showBreakTimeMessageBox(); // This has to remain in UpdateGUIInfo to act as an interrput
                }
                else
                {
                    qDebug() << "New TrialNo: " << p_CommonData->trialNo;
                    QString text = "New TrialNo: ";
                    text.append(QString::number(p_CommonData->trialNo));
                    text.append("\nNew Mapping: ");
                    text.append(QString::number(p_CommonData->mapping));
                    ui->text->setText(text);
                }

                //Mapping text
                QString mappingText = "<P><FONT COLOR='#0c88fb' FONT SIZE = 3>";
                mappingText.append(QString::number(p_CommonData->mapping));
                mappingText.append("</P></br>");
                mappingText.append("CCE Exp Type: ");
                mappingText.append(QString::number(p_CommonData->cceExpType));
                ui->mappingTextBox->setText(mappingText);
            }
            else
            {                
                qDebug() << "New TrialNo: " << p_CommonData->trialNo;
                QString text = "New TrialNo: ";
                text.append(QString::number(p_CommonData->trialNo));
                text.append("\nNew Mapping: ");
                text.append(QString::number(p_CommonData->mapping));
                ui->text->setText(text);

                //Mapping text
                QString mappingText = "<P><FONT COLOR='#0c88fb' FONT SIZE = 3>";
                mappingText.append(QString::number(p_CommonData->mapping));
                mappingText.append("</P></br>");
                mappingText.append("CCE Exp Type: ");
                mappingText.append(QString::number(p_CommonData->cceExpType));
                ui->mappingTextBox->setText(mappingText);
            }
        }
    }
    else
    {
        p_CommonData->manipForceTooHigh = false;
    }

}

void MainWindow::onGUIchanged()
{
    qDebug() << "onGUIchanged";
    if(p_CommonData->cond == 1)
    {
        p_CommonData->flagEqual = false;
        p_CommonData->flagReverse = false;
        qDebug()<<"cond 1";
    }

    else if(p_CommonData->cond == 2)
    {
        p_CommonData->flagEqual = false;
        p_CommonData->flagReverse = true;
        qDebug()<<"cond 2";
    }

    else if(p_CommonData->cond == 3)
    {
        p_CommonData->flagEqual = true;
        p_CommonData->flagReverse = false;
        qDebug()<<"cond 3";
    }

    ui->equalBox->setChecked(p_CommonData->flagEqual);
    ui->reverseBox->setChecked(p_CommonData->flagReverse);

    if(ui->VRControl->isChecked()) //let haptics thread determine desired position
    {
        p_CommonData->currentControlState = VRControlMode;
    }

    p_CommonData->ref1 = (ui->Act1->value());
    p_CommonData->ref2 = (ui->Act2->value());

    //qDebug()<<"am I now";

    UpdateGUIInfo();
}

void MainWindow::on_turnOff_clicked()
{
    p_CommonData->currentControlState = idleControl;
    ui->VRControl->setAutoExclusive(false);
    ui->VRControl->setChecked(false);
    ui->VRControl->setAutoExclusive(true);
}

void MainWindow::setRandomInitBoxPos(int param)
{
    qDebug()<<param;
    if (param == 1)
    {
        p_CommonData->randBoxPosX = 0.2;
        p_CommonData->randBoxPosY = 0.1;
    }
    else if (param == 2)
    {
        p_CommonData->randBoxPosX = 0.1;
        p_CommonData->randBoxPosY = -0.2;
    }
    else if (param == 3)
    {
        p_CommonData->randBoxPosX = -0.05;
        p_CommonData->randBoxPosY = 0.3;
    }
    else if (param == 4)
    {
        p_CommonData->randBoxPosX = 0.2;
        p_CommonData->randBoxPosY = -0.1;
    }
    else if (param == 5)
    {
        p_CommonData->randBoxPosX = -0.1;
        p_CommonData->randBoxPosY = 0.2;
    }
    else
    {
        p_CommonData->randBoxPosX = 0.0;
        p_CommonData->randBoxPosY = 0.0;
        qDebug()<<"~~else~~";
    }
}

//Reads in experiment data from protocol ini files
bool MainWindow::readExpStuffIn()
{
    int trial;
    qDebug()<<"in readExpStuffIn()";

    trial = p_CommonData->trialNo;

    qDebug()<<"Trial: "<<trial; //<<p_CommonData->trialNo;
    //For Mine's Experiments
    if(p_CommonData->currentDynamicObjectState == StiffnessExperiment ||
            p_CommonData->currentDynamicObjectState == StiffnessMassExperiment)
    {
        p_CommonData->TrialType = p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(trial)).toStdString().c_str(), "type", NULL /*default*/);
    }
    //For Jasmin's Experiments:
    if(p_CommonData->currentDynamicObjectState == FingerMappingExperiment)
    {
        p_CommonData->TrialType = p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(trial)).toStdString().c_str(), "type", NULL /*default*/);
    }
    if(p_CommonData->currentDynamicObjectState == HoxelMappingExperiment)
    {
        p_CommonData->TrialType = p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(trial)).toStdString().c_str(), "type", NULL /*default*/);
    }
    if(p_CommonData->currentDynamicObjectState == CrumblyCubeExperiment)
    {
        p_CommonData->TrialType = p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(trial)).toStdString().c_str(), "type", NULL /*default*/);
    }
    if(p_CommonData->currentDynamicObjectState == CubeGuidanceExperiment)
    {
        p_CommonData->TrialType = p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(trial)).toStdString().c_str(), "type", NULL /*default*/);
    }

    qDebug()<<"TrialType"<< p_CommonData->TrialType;

    //Stiffness Experiment
    if(p_CommonData->currentDynamicObjectState == StiffnessExperiment)
    {
        qDebug() << "StiffnessExperiment";
        //Training Trial
        if (p_CommonData->TrialType=="training"){
            if (p_CommonData->trialNo > 0){
                p_CommonData->currentExperimentState = idleExperiment;
            }
            else{
                p_CommonData->currentExperimentState = trialBreak;
                p_CommonData->recordFlag = false;
            }

            p_CommonData->cond = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "condition", NULL /*default*/));
            p_CommonData->stiffness1 = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "stiffness1", NULL /*default*/));
            p_CommonData->stiffness2 = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "stiffness2", NULL /*default*/));
            p_CommonData->direct = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "dir", NULL /*default*/));

            //qDebug() << "train" << "st1" << p_CommonData->stiffness1 << "st2" << p_CommonData->stiffness2 << "cond" << p_CommonData->cond;
            onGUIchanged();
        }
        //Testing Trial
        else if (p_CommonData->TrialType=="testing"){
            p_CommonData->currentExperimentState = idleExperiment;
            p_CommonData->cond = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "condition", NULL /*default*/));
            p_CommonData->stiffness1 = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "stiffness1", NULL /*default*/));
            p_CommonData->stiffness2 = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "stiffness2", NULL /*default*/));
            p_CommonData->direct = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "dir", NULL /*default*/));
            qDebug() << "test" << "st1" << p_CommonData->stiffness1 << "st2" << p_CommonData->stiffness2 << "cond" << p_CommonData->cond;
            onGUIchanged();
        }
        //Trial Break
        else if (p_CommonData->TrialType=="break"){
            p_CommonData->currentExperimentState = trialBreak;
        }
        else if (p_CommonData->TrialType=="breakbreak"){
            p_CommonData->currentExperimentState = trialBreak;
        }
        //Trial Over
        else if (p_CommonData->TrialType=="end"){
            p_CommonData->currentExperimentState = endExperiment;
            //qDebug()<<"DONE!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
            return false;
        }
    }

    //Stiffness-Mass Experiment
    if(p_CommonData->currentDynamicObjectState == StiffnessMassExperiment)
    {
        qDebug() << "Reading In StiffnessMassExperiment Protocol";
        //Training trials
        if (p_CommonData->TrialType=="training"){
            if (p_CommonData->trialNo > 0){
                p_CommonData->currentExperimentState = idleExperiment;
            }
            else{
                p_CommonData->currentExperimentState = trialBreak;
                p_CommonData->recordFlag = false;
            }
            //for TrialMode, 1 means mass 2 means stiffness
            p_CommonData->TrialMode     = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mode", NULL /*default*/));
            p_CommonData->cond          = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "condition", NULL /*default*/));
            p_CommonData->stiffness1    = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "stiffness1", NULL /*default*/));
            p_CommonData->stiffness2    = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "stiffness2", NULL /*default*/));
            p_CommonData->mass1         = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mass1", NULL /*default*/));
            p_CommonData->mass2         = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mass2", NULL /*default*/));
            p_CommonData->direct        = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "dir", NULL /*default*/));

            if(p_CommonData->TrialMode == 1){
                if(p_CommonData->mass1 == 0.1) p_CommonData->lev1 = 1;
                if(p_CommonData->mass1 == 0.2) p_CommonData->lev1 = 2;
                if(p_CommonData->mass1 == 0.3) p_CommonData->lev1 = 3;
                if(p_CommonData->mass1 == 0.4) p_CommonData->lev1 = 4;
                if(p_CommonData->mass1 == 0.5) p_CommonData->lev1 = 5;

                if(p_CommonData->mass2 == 0.1) p_CommonData->lev2 = 1;
                if(p_CommonData->mass2 == 0.2) p_CommonData->lev2 = 2;
                if(p_CommonData->mass2 == 0.3) p_CommonData->lev2 = 3;
                if(p_CommonData->mass2 == 0.4) p_CommonData->lev2 = 4;
                if(p_CommonData->mass2 == 0.5) p_CommonData->lev2 = 5;
            }
            else if(p_CommonData->TrialMode == 2){
                if(p_CommonData->stiffness1 == 100) p_CommonData->lev1 = 1;
                if(p_CommonData->stiffness1 == 200) p_CommonData->lev1 = 2;
                if(p_CommonData->stiffness1 == 300) p_CommonData->lev1 = 3;
                if(p_CommonData->stiffness1 == 400) p_CommonData->lev1 = 4;
                if(p_CommonData->stiffness1 == 500) p_CommonData->lev1 = 5;

                if(p_CommonData->stiffness2 == 100) p_CommonData->lev2 = 1;
                if(p_CommonData->stiffness2 == 200) p_CommonData->lev2 = 2;
                if(p_CommonData->stiffness2 == 300) p_CommonData->lev2 = 3;
                if(p_CommonData->stiffness2 == 400) p_CommonData->lev2 = 4;
                if(p_CommonData->stiffness2 == 500) p_CommonData->lev2 = 5;
            }
            onGUIchanged();
        }
        //Testing trials
        else if (p_CommonData->TrialType=="testing"){
            p_CommonData->currentExperimentState = idleExperiment;
            p_CommonData->TrialMode     = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mode", NULL /*default*/));
            p_CommonData->cond          = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "condition", NULL /*default*/));
            p_CommonData->stiffness1    = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "stiffness1", NULL /*default*/));
            p_CommonData->stiffness2    = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "stiffness2", NULL /*default*/));
            p_CommonData->mass1         = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mass1", NULL /*default*/));
            p_CommonData->mass2         = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mass2", NULL /*default*/));
            p_CommonData->direct        = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "dir", NULL /*default*/));

            if(p_CommonData->TrialMode == 1){
                if(p_CommonData->mass1 == 0.1) p_CommonData->lev1 = 1;
                if(p_CommonData->mass1 == 0.2) p_CommonData->lev1 = 2;
                if(p_CommonData->mass1 == 0.3) p_CommonData->lev1 = 3;
                if(p_CommonData->mass1 == 0.4) p_CommonData->lev1 = 4;
                if(p_CommonData->mass1 == 0.5) p_CommonData->lev1 = 5;

                if(p_CommonData->mass2 == 0.1) p_CommonData->lev2 = 1;
                if(p_CommonData->mass2 == 0.2) p_CommonData->lev2 = 2;
                if(p_CommonData->mass2 == 0.3) p_CommonData->lev2 = 3;
                if(p_CommonData->mass2 == 0.4) p_CommonData->lev2 = 4;
                if(p_CommonData->mass2 == 0.5) p_CommonData->lev2 = 5;
            }
            else if(p_CommonData->TrialMode == 2){
                if(p_CommonData->stiffness1 == 100) p_CommonData->lev1 = 1;
                if(p_CommonData->stiffness1 == 200) p_CommonData->lev1 = 2;
                if(p_CommonData->stiffness1 == 300) p_CommonData->lev1 = 3;
                if(p_CommonData->stiffness1 == 400) p_CommonData->lev1 = 4;
                if(p_CommonData->stiffness1 == 500) p_CommonData->lev1 = 5;

                if(p_CommonData->stiffness2 == 100) p_CommonData->lev2 = 1;
                if(p_CommonData->stiffness2 == 200) p_CommonData->lev2 = 2;
                if(p_CommonData->stiffness2 == 300) p_CommonData->lev2 = 3;
                if(p_CommonData->stiffness2 == 400) p_CommonData->lev2 = 4;
                if(p_CommonData->stiffness2 == 500) p_CommonData->lev2 = 5;
            }
            //            qDebug() << "test" << "st1" << p_CommonData->stiffness1<< "st2" << p_CommonData->stiffness2 << "cond" << p_CommonData->cond;
            onGUIchanged();
        }
        //Trial Break
        else if (p_CommonData->TrialType=="break"){
            p_CommonData->currentExperimentState = trialBreak;
        }
        else if (p_CommonData->TrialType=="breakbreak"){
            p_CommonData->currentExperimentState = trialBreak;
        }
        //Trial Over
        else if (p_CommonData->TrialType=="end"){
            p_CommonData->currentExperimentState = endExperiment;
            //qDebug()<<"StiffnessMassExperiment DONE!!";
            return false;
        }
    }

    //For Jasmin's FingerMapping Experiment
    if(p_CommonData->currentDynamicObjectState == FingerMappingExperiment)
    {
        qDebug() << "Reading In FingerMappingExperiment Protocol";
        //Training trials
        if (p_CommonData->TrialType=="training")
        {
            if (p_CommonData->trialNo > 0)
            {
                p_CommonData->currentExperimentState = idleExperiment;
                qDebug()<<"idleExperiment";
            }
            else
            {
                p_CommonData->currentExperimentState = trialBreak;
                p_CommonData->recordFlag = false;
                qDebug()<<"trialBreak";
            }

            //Read protocal ini file info into the experiment environment
            //for TrialMode, 1 means mass 2 means stiffness
            p_CommonData->TrialMode     = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mode", NULL /*default*/));
            p_CommonData->cond          = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "condition", NULL /*default*/));
            p_CommonData->stiffness1    = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "stiffness1", NULL /*default*/));
            p_CommonData->mass1         = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mass1", NULL /*default*/));
            p_CommonData->direct        = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "dir", NULL /*default*/));
            p_CommonData->mapping       = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mapping", NULL /*default*/));


            if(p_CommonData->TrialMode == 1){
                if(p_CommonData->mass1 == 0.1) p_CommonData->lev1 = 1;
                if(p_CommonData->mass1 == 0.2) p_CommonData->lev1 = 2;
                if(p_CommonData->mass1 == 0.3) p_CommonData->lev1 = 3;
                if(p_CommonData->mass1 == 0.4) p_CommonData->lev1 = 4;
                if(p_CommonData->mass1 == 0.5) p_CommonData->lev1 = 5;
                qDebug()<<"TrialMode == 1";
            }
            else if(p_CommonData->TrialMode == 2){
                if(p_CommonData->stiffness1 == 100) p_CommonData->lev1 = 1;
                if(p_CommonData->stiffness1 == 200) p_CommonData->lev1 = 2;
                if(p_CommonData->stiffness1 == 300) p_CommonData->lev1 = 3;
                if(p_CommonData->stiffness1 == 400) p_CommonData->lev1 = 4;
                if(p_CommonData->stiffness1 == 500) p_CommonData->lev1 = 5;
                qDebug()<<"TrialMode == 2";
            }
            onGUIchanged();
        }
        //Testing trials
        else if (p_CommonData->TrialType=="testing"){
            p_CommonData->currentExperimentState = idleExperiment;
            p_CommonData->TrialMode     = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mode", NULL /*default*/));
            p_CommonData->cond          = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "condition", NULL /*default*/));
            p_CommonData->stiffness1    = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "stiffness1", NULL /*default*/));
            p_CommonData->mass1         = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mass1", NULL /*default*/));
            p_CommonData->direct        = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "dir", NULL /*default*/));
            p_CommonData->mapping       = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mapping", NULL /*default*/));

            if(p_CommonData->TrialMode == 1){
                if(p_CommonData->mass1 == 0.1) p_CommonData->lev1 = 1;
                if(p_CommonData->mass1 == 0.2) p_CommonData->lev1 = 2;
                if(p_CommonData->mass1 == 0.3) p_CommonData->lev1 = 3;
                if(p_CommonData->mass1 == 0.4) p_CommonData->lev1 = 4;
                if(p_CommonData->mass1 == 0.5) p_CommonData->lev1 = 5;
            }
            else if(p_CommonData->TrialMode == 2){
                if(p_CommonData->stiffness1 == 100) p_CommonData->lev1 = 1;
                if(p_CommonData->stiffness1 == 200) p_CommonData->lev1 = 2;
                if(p_CommonData->stiffness1 == 300) p_CommonData->lev1 = 3;
                if(p_CommonData->stiffness1 == 400) p_CommonData->lev1 = 4;
                if(p_CommonData->stiffness1 == 500) p_CommonData->lev1 = 5;
            }
            //qDebug() << "test" << "st1" << p_CommonData->stiffness1<< "st2" << p_CommonData->stiffness2 << "cond" << p_CommonData->cond;
            onGUIchanged();
        }
        //Trial Break
        else if (p_CommonData->TrialType=="break"){
            p_CommonData->currentExperimentState = trialBreak;
        }
        else if (p_CommonData->TrialType=="breakbreak"){
            p_CommonData->currentExperimentState = trialBreak;
        }
        //Trial Over
        else if (p_CommonData->TrialType=="end"){
            p_CommonData->currentExperimentState = endExperiment;
            qDebug()<<"StiffnessMassExperiment DONE!!";
            return false;
        }
        else if (p_CommonData->TrialType==""){
            qDebug()<<"THIS IS BROKEN -- YOU SHOULD NEVER BE HERE -- bad protocol file read";
        }
    }

    //For Jasmin's HoxelMapping Experiment
    if(p_CommonData->currentDynamicObjectState == HoxelMappingExperiment)
    {
        qDebug() << "Reading In HoxelMappingExperiment Protocol";
        //Training trials
        if (p_CommonData->TrialType=="training")
        {
            if (p_CommonData->trialNo > 0)
            {
                p_CommonData->currentExperimentState = idleExperiment;
                qDebug()<<"idleExperiment";
            }
            else
            {
                p_CommonData->currentExperimentState = trialBreak;
                p_CommonData->recordFlag = false;
                qDebug()<<"trialBreak";
            }

            //Read protocal ini file info into the experiment environment
            //for TrialMode, 1 means mass 2 means stiffness
            p_CommonData->TrialMode     = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mode", NULL /*default*/));
            p_CommonData->cond          = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "condition", NULL /*default*/));
            p_CommonData->stiffness1    = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "stiffness1", NULL /*default*/));
            p_CommonData->mass1         = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mass1", NULL /*default*/));
            p_CommonData->direct        = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "dir", NULL /*default*/));
            p_CommonData->mapping       = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mapping", NULL /*default*/));


            if(p_CommonData->TrialMode == 1)
            {
                if(p_CommonData->mass1 == 0.1) p_CommonData->lev1 = 1;
                if(p_CommonData->mass1 == 0.2) p_CommonData->lev1 = 2;
                if(p_CommonData->mass1 == 0.3) p_CommonData->lev1 = 3;
                if(p_CommonData->mass1 == 0.4) p_CommonData->lev1 = 4;
                if(p_CommonData->mass1 == 0.5) p_CommonData->lev1 = 5;
                qDebug()<<"TrialMode == 1";
            }
            else if(p_CommonData->TrialMode == 2)
            {
                if(p_CommonData->stiffness1 == 100) p_CommonData->lev1 = 1;
                if(p_CommonData->stiffness1 == 200) p_CommonData->lev1 = 2;
                if(p_CommonData->stiffness1 == 300) p_CommonData->lev1 = 3;
                if(p_CommonData->stiffness1 == 400) p_CommonData->lev1 = 4;
                if(p_CommonData->stiffness1 == 500) p_CommonData->lev1 = 5;
                qDebug()<<"TrialMode == 2";
            }
            onGUIchanged();
            return true;
        }
        //Testing trials
        else if (p_CommonData->TrialType=="testing")
        {
            p_CommonData->currentExperimentState = idleExperiment;
            p_CommonData->TrialMode     = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mode", NULL /*default*/));
            p_CommonData->cond          = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "condition", NULL /*default*/));
            p_CommonData->stiffness1    = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "stiffness1", NULL /*default*/));
            p_CommonData->mass1         = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mass1", NULL /*default*/));
            p_CommonData->direct        = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "dir", NULL /*default*/));
            p_CommonData->mapping       = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mapping", NULL /*default*/));

            if(p_CommonData->TrialMode == 1)
            {
                if(p_CommonData->mass1 == 0.1) p_CommonData->lev1 = 1;
                if(p_CommonData->mass1 == 0.2) p_CommonData->lev1 = 2;
                if(p_CommonData->mass1 == 0.3) p_CommonData->lev1 = 3;
                if(p_CommonData->mass1 == 0.4) p_CommonData->lev1 = 4;
                if(p_CommonData->mass1 == 0.5) p_CommonData->lev1 = 5;
            }
            else if(p_CommonData->TrialMode == 2)
            {
                if(p_CommonData->stiffness1 == 100) p_CommonData->lev1 = 1;
                if(p_CommonData->stiffness1 == 200) p_CommonData->lev1 = 2;
                if(p_CommonData->stiffness1 == 300) p_CommonData->lev1 = 3;
                if(p_CommonData->stiffness1 == 400) p_CommonData->lev1 = 4;
                if(p_CommonData->stiffness1 == 500) p_CommonData->lev1 = 5;
            }
            //qDebug() << "test" << "st1" << p_CommonData->stiffness1<< "st2" << p_CommonData->stiffness2 << "cond" << p_CommonData->cond;
            onGUIchanged();
            return true;
        }
        //Trial Break
        else if (p_CommonData->TrialType=="break"){
            p_CommonData->currentExperimentState = trialBreak;
            return true;
        }
        else if (p_CommonData->TrialType=="breakbreak"){
            p_CommonData->currentExperimentState = trialBreak;
            return true;
        }
        //Trial Over
        else if (p_CommonData->TrialType=="end"){
            p_CommonData->currentExperimentState = endExperiment;
            qDebug()<<"HoxelMappingExperiment DONE!!";
            return false;
        }
        else if (p_CommonData->TrialType==""){
            qDebug()<<"THIS IS BROKEN -- YOU SHOULD NEVER BE HERE -- bad protocol file read";
            return false;
        }
    }

    //For Jasmin's CrumblyCube Experiment
    if(p_CommonData->currentDynamicObjectState == CrumblyCubeExperiment)
    {
        qDebug() << "Reading In CrumblyCubeExperiment Protocol";
        //Training trials
        if (p_CommonData->TrialType=="training")
        {
            if (p_CommonData->trialNo > 0)
            {
                p_CommonData->currentExperimentState = idleExperiment;
                qDebug()<<"idleExperiment";
            }
            else
            {
                p_CommonData->currentExperimentState = trialBreak;
                p_CommonData->recordFlag = false;
                qDebug()<<"trialBreak";
            }

            //Read protocal ini file info into the experiment environment
            //for TrialMode, 1 means mass 2 means stiffness
            p_CommonData->TrialMode     = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mode", NULL /*default*/));
            p_CommonData->cond          = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "condition", NULL /*default*/));
            p_CommonData->stiffness1    = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "stiffness1", NULL /*default*/));
            p_CommonData->mass1         = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mass1", NULL /*default*/));
            p_CommonData->direct        = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "dir", NULL /*default*/));
            p_CommonData->mapping       = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mapping", NULL /*default*/));
            p_CommonData->cceExpType	= std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "cceExpType", NULL /*default*/));

            /*
            if(p_CommonData->TrialMode == 1)
            {
                if(p_CommonData->mass1 == 0.1) p_CommonData->lev1 = 1;
                if(p_CommonData->mass1 == 0.2) p_CommonData->lev1 = 2;
                if(p_CommonData->mass1 == 0.3) p_CommonData->lev1 = 3;
                if(p_CommonData->mass1 == 0.4) p_CommonData->lev1 = 4;
                if(p_CommonData->mass1 == 0.5) p_CommonData->lev1 = 5;
                qDebug()<<"TrialMode == 1";
            }
            else if(p_CommonData->TrialMode == 2)
            {
                if(p_CommonData->stiffness1 == 100) p_CommonData->lev1 = 1;
                if(p_CommonData->stiffness1 == 200) p_CommonData->lev1 = 2;
                if(p_CommonData->stiffness1 == 300) p_CommonData->lev1 = 3;
                if(p_CommonData->stiffness1 == 400) p_CommonData->lev1 = 4;
                if(p_CommonData->stiffness1 == 500) p_CommonData->lev1 = 5;
                qDebug()<<"TrialMode == 2";
            }
            */
            onGUIchanged();
            return true;
        }
        //Testing trials
        else if (p_CommonData->TrialType=="testing")
        {
            p_CommonData->currentExperimentState = idleExperiment;
            p_CommonData->TrialMode     = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mode", NULL /*default*/));
            p_CommonData->cond          = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "condition", NULL /*default*/));
            p_CommonData->stiffness1    = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "stiffness1", NULL /*default*/));
            p_CommonData->mass1         = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mass1", NULL /*default*/));
            p_CommonData->direct        = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "dir", NULL /*default*/));
            p_CommonData->mapping       = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mapping", NULL /*default*/));
            p_CommonData->cceExpType	= std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "cceExpType", NULL /*default*/));

            if(p_CommonData->TrialMode == 1)
            {
                if(p_CommonData->mass1 == 0.1) p_CommonData->lev1 = 1;
                if(p_CommonData->mass1 == 0.2) p_CommonData->lev1 = 2;
                if(p_CommonData->mass1 == 0.3) p_CommonData->lev1 = 3;
                if(p_CommonData->mass1 == 0.4) p_CommonData->lev1 = 4;
                if(p_CommonData->mass1 == 0.5) p_CommonData->lev1 = 5;
            }
            else if(p_CommonData->TrialMode == 2)
            {
                if(p_CommonData->stiffness1 == 100) p_CommonData->lev1 = 1;
                if(p_CommonData->stiffness1 == 200) p_CommonData->lev1 = 2;
                if(p_CommonData->stiffness1 == 300) p_CommonData->lev1 = 3;
                if(p_CommonData->stiffness1 == 400) p_CommonData->lev1 = 4;
                if(p_CommonData->stiffness1 == 500) p_CommonData->lev1 = 5;
            }
            //qDebug() << "test" << "st1" << p_CommonData->stiffness1<< "st2" << p_CommonData->stiffness2 << "cond" << p_CommonData->cond;
            onGUIchanged();
            return true;
        }
        //Trial Break
        else if (p_CommonData->TrialType=="break"){
            p_CommonData->currentExperimentState = trialBreak;
            p_CommonData->cceExpType = 0; // placeholder to stop code running through break transistions;
            return true;
        }
        else if (p_CommonData->TrialType=="breakbreak"){
            p_CommonData->currentExperimentState = trialBreak;
            return true;
        }
        //Trial Over
        else if (p_CommonData->TrialType=="end"){
            p_CommonData->currentExperimentState = endExperiment;
            qDebug()<<"CrumblyCubeExperiment DONE!!";
            return false;
        }
        else if (p_CommonData->TrialType==""){
            qDebug()<<"THIS IS BROKEN -- YOU SHOULD NEVER BE HERE -- bad protocol file read";
            return false;
        }
    }

    //For Jasmin's Cube Slide Experiment
    else if(p_CommonData->currentDynamicObjectState == CubeGuidanceExperiment)
    {
        qDebug() << "Reading In CubeGuidanceExperiment Protocol";
        //Training trials
        if (p_CommonData->TrialType=="training")
        {
            if (p_CommonData->trialNo > 0)
            {
                p_CommonData->currentExperimentState = idleExperiment;
                qDebug()<<"idleExperiment";
            }
            else
            {
                p_CommonData->currentExperimentState = trialBreak;
                p_CommonData->recordFlag = false;
                qDebug()<<"trialBreak";
            }

            //Read protocal ini file info into the experiment environment
            //for TrialMode, 1 means mass 2 means stiffness
            p_CommonData->TrialMode     = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mode", NULL /*default*/));
            p_CommonData->cond          = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "condition", NULL /*default*/));
            p_CommonData->stiffness1    = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "stiffness1", NULL /*default*/));
            p_CommonData->mass1         = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mass1", NULL /*default*/));
            p_CommonData->direct        = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "dir", NULL /*default*/));
            p_CommonData->mapping       = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mapping", NULL /*default*/));
            p_CommonData->boxInitRandPos  = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "boxInitParam", NULL /*default*/));

            if(p_CommonData->TrialMode == 1){
                if(p_CommonData->mass1 == 0.1) p_CommonData->lev1 = 1;
                if(p_CommonData->mass1 == 0.2) p_CommonData->lev1 = 2;
                if(p_CommonData->mass1 == 0.3) p_CommonData->lev1 = 3;
                if(p_CommonData->mass1 == 0.4) p_CommonData->lev1 = 4;
                if(p_CommonData->mass1 == 0.5) p_CommonData->lev1 = 5;
                qDebug()<<"TrialMode == 1";
            }
            else if(p_CommonData->TrialMode == 2){
                if(p_CommonData->stiffness1 == 100) p_CommonData->lev1 = 1;
                if(p_CommonData->stiffness1 == 200) p_CommonData->lev1 = 2;
                if(p_CommonData->stiffness1 == 300) p_CommonData->lev1 = 3;
                if(p_CommonData->stiffness1 == 400) p_CommonData->lev1 = 4;
                if(p_CommonData->stiffness1 == 500) p_CommonData->lev1 = 5;
                qDebug()<<"TrialMode == 2";
            }
            setRandomInitBoxPos(p_CommonData->boxInitRandPos);
            onGUIchanged();
        }
        //Testing trials
        else if (p_CommonData->TrialType=="testing"){
            p_CommonData->currentExperimentState = idleExperiment;
            p_CommonData->TrialMode     = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mode", NULL /*default*/));
            p_CommonData->cond          = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "condition", NULL /*default*/));
            p_CommonData->stiffness1    = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "stiffness1", NULL /*default*/));
            p_CommonData->mass1         = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mass1", NULL /*default*/));
            p_CommonData->direct        = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "dir", NULL /*default*/));
            p_CommonData->mapping       = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mapping", NULL /*default*/));
            p_CommonData->boxInitRandPos  = std::stod(p_CommonData->selectedProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "boxInitParam", NULL /*default*/));

            if(p_CommonData->TrialMode == 1){
                if(p_CommonData->mass1 == 0.1) p_CommonData->lev1 = 1;
                if(p_CommonData->mass1 == 0.2) p_CommonData->lev1 = 2;
                if(p_CommonData->mass1 == 0.3) p_CommonData->lev1 = 3;
                if(p_CommonData->mass1 == 0.4) p_CommonData->lev1 = 4;
                if(p_CommonData->mass1 == 0.5) p_CommonData->lev1 = 5;
            }
            else if(p_CommonData->TrialMode == 2){
                if(p_CommonData->stiffness1 == 100) p_CommonData->lev1 = 1;
                if(p_CommonData->stiffness1 == 200) p_CommonData->lev1 = 2;
                if(p_CommonData->stiffness1 == 300) p_CommonData->lev1 = 3;
                if(p_CommonData->stiffness1 == 400) p_CommonData->lev1 = 4;
                if(p_CommonData->stiffness1 == 500) p_CommonData->lev1 = 5;
            }
            //qDebug() << "test" << "st1" << p_CommonData->stiffness1<< "st2" << p_CommonData->stiffness2 << "cond" << p_CommonData->cond;
            setRandomInitBoxPos(p_CommonData->boxInitRandPos);
            onGUIchanged();
        }

        //Trial Break
        else if (p_CommonData->TrialType=="break"){
            p_CommonData->currentExperimentState = trialBreak;
        }
        else if (p_CommonData->TrialType=="breakbreak"){
            p_CommonData->currentExperimentState = trialBreak;
        }
        //Trial Over
        else if (p_CommonData->TrialType=="end"){
            p_CommonData->currentExperimentState = endExperiment;
            qDebug()<<"StiffnessMassExperiment DONE!!";
            return false;
        }
        else if (p_CommonData->TrialType==""){
            qDebug()<<"THIS IS BROKEN -- YOU SHOULD NEVER BE HERE -- bad protocol file read";
        }

    }
}

void MainWindow::progressPickAndPlaceExperiment(bool mistake)
{
    if(CheckFingers() && (p_CommonData->fingerTouching == false && p_CommonData->thumbTouching == false))
    {
        //PRE-TRIAL******
        if(p_CommonData->trialNo < 1)
        {
            qDebug()<< "Trial# < 1";
            p_CommonData->trialNo = 1;
            p_CommonData->recordFlag = true;

            //Read in protocol file and check if the read is successful
            if (readExpStuffIn())
            {
                qDebug()<<"readExpStuffIn() SUCCESS -- Pre-Trials";
            }

            //GUI Stuff for each trial type transitioning from pre-trial to trial 1
            //GUI should show in light blue to distinguish that these
            //are for the trial that transitioned from pre-trial
            if (p_CommonData->TrialType == "training")
            {
                QString labelText = "<P><FONT COLOR='#7abfe4' FONT SIZE = 5>";
                labelText.append("TRAINING\n");
                labelText.append("</P></br>");
                labelText.append("<P><FONT COLOR='#7abfe4' FONT SIZE = 2>");
                labelText.append("Pick up the cube,\n"
                                 "bring it through the hoop,\n"
                                 "Place it in the target area");
                labelText.append("</P></br>");
                ui->text->setText(labelText);
                qDebug()<<"TRAINING "<<p_CommonData->trialNo;
            }
            else if (p_CommonData->TrialType == "testing")
            {
                QString labelText = "<P><FONT COLOR='#7abfe4' FONT SIZE = 5>";
                labelText .append("TRIAL -- ");
                labelText .append("</P></br>");
                ui->text->setText(labelText);
                qDebug()<<"RUNNING TRIAL "<<p_CommonData->trialNo;
            }
            else if (p_CommonData->TrialType == "break")
            {
                QString labelText = "<P><b><FONT COLOR='#7abfe4' FONT SIZE = 5>";
                labelText .append("PRESS NEXT AFTER THE BREAK --");
                labelText .append("</b></P></br>");
                ui->text->setText(labelText);
                qDebug()<<"BREAK "<<p_CommonData->trialNo;

            }
            else if (p_CommonData->TrialType == "breakbreak")
            {
                QString labelText = "<P><b><FONT COLOR='#7abfe4' FONT SIZE = 5>";
                labelText .append("TIME TO CHANGE THE GROUNDING --");
                labelText .append("</b></P></br>");
                ui->text->setText(labelText);
                qDebug()<<"BREAK "<<p_CommonData->trialNo;
            }
            else if (p_CommonData->TrialType == "end")
            {
                QString labelText = "<P><b><FONT COLOR='#7abfe4' FONT SIZE = 5>";
                labelText .append("END OF THE EXPERIMENT --");
                labelText .append("</b></P></br>");
                ui->text->setText(labelText);
                qDebug()<<"EXPERIMENT OVER "<<p_CommonData->trialNo;
            }
            p_CommonData->environmentChange = true;

            qDebug()<<"Progress to Pre-Trial#"<<p_CommonData->trialNo<<"  Type "<< p_CommonData->TrialType;
        }
        //ACTUAL TRIAL*******************************
        else
        {
            qDebug()<<"Trial# >= 1";
            if(p_CommonData->recordFlag)
            {
                p_CommonData->dataRecordMutex.lock();
                localDataRecorderVector = p_CommonData->dataRecorderVector;
                p_CommonData->dataRecorderVector.clear();
                p_CommonData->dataRecordMutex.unlock();
                WriteDataToFile();
                p_CommonData->recordFlag = false;
            }

            //if in training or testing trials
            if(p_CommonData->TrialType == "training" || p_CommonData->TrialType == "testing")
            {
                //If cube passed hoop and target, advance trial
                if(p_CommonData->target1Complete && p_CommonData->hoopSuccess)//(p_CommonData->target1Complete && p_CommonData->hoop1Complete)
                {
                    //advance
                    p_CommonData->trialNo++;
                    //reset mistakeCunter
                    p_CommonData->mistakeCounter = 0;
                    //then read in
                    if (readExpStuffIn())
                    {
                        qDebug()<<"_readExpStuffIn() SUCCESS 2_";
                    }

                    //GUI Indication of completed trial
                    if (p_CommonData->TrialType == "training")
                    {
                        QString labelText = "<P><FONT COLOR='#000000' FONT SIZE = 5>";
                        labelText.append("TRAINING...\n");
                        labelText.append("</P></br>");
                        labelText.append("<P><FONT COLOR='#000000' FONT SIZE = 2>");
                        labelText.append("Pick up the cube,\n"
                                         "Bring it through the hoop,\n"
                                         "Place it in the target area");
                        labelText.append("</P></br>");
                        ui->text->setText(labelText);
                        qDebug()<<"TRAINING "<<p_CommonData->trialNo;
                    }
                    else if (p_CommonData->TrialType == "testing")
                    {
                        QString labelText = "<P><FONT COLOR='#000000' FONT SIZE = 5>";
                        labelText .append("TESTING...");
                        labelText .append("</P></br>");
                        ui->text->setText(labelText);
                        qDebug()<<"RUNNING TRIAL "<<p_CommonData->trialNo;
                    }
                    else if (p_CommonData->TrialType == "break")
                    {
                        QString labelText = "<P><b><FONT COLOR='#0000ff' FONT SIZE = 5>";
                        labelText .append("PRESS NEXT AFTER THE BREAK --");
                        labelText .append("</b></P></br>");
                        ui->text->setText(labelText);
                        qDebug()<<"BREAK "<<p_CommonData->trialNo;

                        // Pop up countdown timer:
                        showBreakTimeMessageBox();
                    }
                    else if (p_CommonData->TrialType == "breakbreak")
                    {
                        QString labelText = "<P><b><FONT COLOR='#0000ff' FONT SIZE = 5>";
                        labelText .append("TIME TO CHANGE THE GROUNDING --");
                        labelText .append("</b></P></br>");
                        ui->text->setText(labelText);
                        qDebug()<<"BREAK "<<p_CommonData->trialNo;
                    }
                    else if (p_CommonData->TrialType == "end")
                    {
                        QString labelText = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 5>";
                        labelText.append("END OF THE EXPERIMENT");
                        labelText.append("</b></P></br>");
                        labelText.append("<P><b><FONT COLOR='#b00be5' FONT SIZE = 2>");
                        labelText.append("~~Thanks :)");
                        labelText.append("</b></P></br>");
                        ui->text->setText(labelText);
                        qDebug()<<"EXPERIMENT OVER "<<p_CommonData->trialNo;
                    }
                    qDebug()<<"Progress to FME Trial #"<<p_CommonData->trialNo<<"  Type "<< p_CommonData->TrialType;
                    p_CommonData->environmentChange = true;
                }

                //If cube has not passed *both* hoop and target
                else
                {
                    if(p_CommonData->target1Complete){//REVALUATE THIS CONDITION

                        qDebug()<< "TrialComplete!!";

                        QString labelText = "<P><b><FONT COLOR='#4f0080' FONT SIZE = 5>";
                        labelText.append("DONE w/ move!! for:  ");
                        labelText.append("</b></P></br>");
                        ui->text->setText(labelText);
                        mistake = true;
                        p_CommonData->mistakeCounter++;
                        //qDebug()<<"___"<<p_CommonData->trialNo;

                        //p_CommonData->targetSuccess = 1;

                    }
                    //If object is not in the final area
                    else if(!p_CommonData->target1Complete)
                    {
                        //If the object has gone through the hoop but not target
                        if(p_CommonData->hoopSuccess) //(p_CommonData->hoop1Complete)
                        {
                            //qDebug()<< "Through the Blue Hoop but not in target area yet";
                            QString labelText1 = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 5>";
                            labelText1.append("CANNOT ADVANCE");
                            labelText1.append("</b></P></br>");
                            labelText1.append("<P><b><FONT COLOR='#000000' FONT SIZE = 2>");
                            labelText1.append("Pick up the cube, "
                                              "Place it in the target area");
                            labelText1.append("</b></P></br>");
                            ui->text->setText(labelText1);
                            mistake = true;
                            p_CommonData->mistakeCounter++;
                            //qDebug()<<"___"<<p_CommonData->trialNo;
                        }
                        //If the object hit neither the target or hoop
                        else if(!p_CommonData->hoopSuccess) //
                        {
                            //qDebug()<< "Did not hit target area and did not pass through Blue Hoop";
                            QString labelText1 = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 5>";
                            labelText1.append("CANNOT ADVANCE");
                            labelText1.append("</b></P></br>");
                            labelText1.append("<P><b><FONT COLOR='#000000' FONT SIZE = 2>");
                            labelText1.append("Pick up the cube, "
                                              "Bring it through the hoop, "
                                              "Place it in the target area");
                            labelText1.append("</b></P></br>");
                            ui->text->setText(labelText1);
                            mistake = true;
                            p_CommonData->mistakeCounter++;
                            //qDebug()<<"___"<<p_CommonData->trialNo;
                        }
                    }
                }
            }

            //if not in training or testing trials
            //This area is called when coming back from break
            else
            {
                //advance
                p_CommonData->trialNo++;
                //reset mistakeCounter
                p_CommonData->mistakeCounter = 0;
                //then read in
                if (readExpStuffIn())
                {
                    qDebug()<<"successful read -- back from break";
                }
                //GUI Stuff
                //Logic after break
                if (p_CommonData->TrialType == "training")
                {
                    QString labelText = "<P><FONT COLOR='#000000' FONT SIZE = 5>";
                    labelText.append("Training -- back from break");
                    labelText.append("</P></br>");
                    ui->text->setText(labelText);
                    //qDebug()<<"___"<<p_CommonData->trialNo;
                }

                else if (p_CommonData->TrialType == "testing")
                {
                    QString labelText = "<P><FONT COLOR='#000000' FONT SIZE = 5>";
                    labelText.append("Testing -- back from break");
                    labelText.append("</P></br>");
                    ui->text->setText(labelText);
                    //qDebug()<<"___"<<p_CommonData->trialNo;
                }

                else if (p_CommonData->TrialType == "break")
                {
                    QString labelText = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                    labelText.append("PRESS NEXT AFTER THE BREAK -- back from break");
                    labelText.append("</b></P></br>");
                    ui->text->setText(labelText);

                    // Pop up countdown timer:
                    showBreakTimeMessageBox();
                }

                else if (p_CommonData->TrialType == "breakbreak")
                {
                    QString labelText = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                    labelText.append("TIME TO CHANGE THE GROUNDING -- back from break");
                    labelText.append("</b></P></br>");
                    ui->text->setText(labelText);
                    // qDebug()<<"___"<<p_CommonData->trialNo;
                }

                else if (p_CommonData->TrialType == "end")
                {
                    QString labelText = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                    labelText.append("END OF THE EXPERIMENT -- back from break");
                    labelText.append("</b></P></br>");
                    ui->text->setText(labelText);
                    //qDebug()<<"___"<<p_CommonData->trialNo;
                }
                qDebug()<<"Progress to Trial#"<<p_CommonData->trialNo<<"  Type "<< p_CommonData->TrialType;
                p_CommonData->environmentChange = true;
            }
        }
    }

    if(p_CommonData->currentDynamicObjectState == FingerMappingExperiment ||
            p_CommonData->currentDynamicObjectState == HoxelMappingExperiment ||
            p_CommonData->currentDynamicObjectState == CrumblyCubeExperiment)
    {

        //Set Mapping Text
        QString mappingText = "<P><FONT COLOR='#0c88fb' FONT SIZE = 3> Mapping #";
        mappingText.append(QString::number(p_CommonData->mapping));
        mappingText.append("</P></br>\n");
        mappingText.append("CCE Exp Type: ");
        mappingText.append(QString::number(p_CommonData->cceExpType));
        ui->mappingTextBox->setText(mappingText);
    }

    if(p_CommonData->currentDynamicObjectState == CubeGuidanceExperiment)
    {
        //Set Mapping Text
        QString mappingText = "<P><FONT COLOR='#0c88fb' FONT SIZE = 3>";
        mappingText.append(QString::number(p_CommonData->mapping));
        mappingText.append("</P></br>rPos-");
        mappingText.append(QString::number(p_CommonData->boxInitRandPos));
        mappingText.append(" | X0:");
        mappingText.append(QString::number(p_CommonData->randBoxPosX));
        mappingText.append(" Y0:");
        mappingText.append(QString::number(p_CommonData->randBoxPosY));
        ui->mappingTextBox->setText(mappingText);
    }
}

void MainWindow::showBreakTimeMessageBox()
{
    BreakTimeDialog dialog(&windowGLDisplay);
    dialog.exec();
    // Perform any necessary actions to continue using the application after the dialog is closed

    //qDebug()<<"NOW I'M CLOSED";
}

void MainWindow::keyPressEvent(QKeyEvent *a_event)
{
    if (a_event->key() == Qt::Key_B)
    {
       // showBreakTimeMessageBox();

    }
    /***Environment Adjustment Buttons***/
    //Hide/Show finger interaction forces
    if (a_event->key() == Qt::Key_Y)
    {
        p_CommonData->show_forces = !p_CommonData->show_forces;
    }
    //Hide/Show finger reference frames
    if (a_event->key() == Qt::Key_F)
    {
        p_CommonData->showCursorFrames = !p_CommonData->showCursorFrames;
    }
    //Update GUI Info Manually
    if (a_event->key() == Qt::Key_Y)
    {
        UpdateGUIInfo();
    }
    //Stop Recording Data
    if (a_event->key() == Qt::Key_G)
    {
        qDebug()<<"Stopped Recording";
        if(p_CommonData->recordFlag == true)
        {
            p_CommonData->dataRecordMutex.lock();
            localDataRecorderVector = p_CommonData->dataRecorderVector;
            p_CommonData->dataRecorderVector.clear();
            p_CommonData->dataRecordMutex.unlock();
            WriteDataToFile();
            p_CommonData->recordFlag = false;
        }
        //assuming this is switching between states without progressing trial
    }

    /***Pick-and-Place Trial Progression Buttons***/
    /***For FME, HME, & Stiffness-Mass and Mass Discrimination***/
    //progressing to next trial
    if(a_event->key() == Qt::Key_H)
    {
        bool mistake;
        qDebug()<<"H Key pressed -- progressing to next trial/state";

        //p_CommonData->hoopSuccess = 0;
        //p_CommonData->targetSuccess = 0;

        if (p_CommonData->currentDynamicObjectState == StiffnessExperiment)
        {
            qDebug("pleaseadvance");
            if(CheckFingers()&& (p_CommonData->fingerTouching == false && p_CommonData->thumbTouching == false))
            {
                if(p_CommonData->trialNo<1)
                {
                    p_CommonData->trialNo = 1;
                    p_CommonData->recordFlag = true;
                }

                else {
                    if(p_CommonData->recordFlag == true){
                        p_CommonData->dataRecordMutex.lock();
                        localDataRecorderVector = p_CommonData->dataRecorderVector;
                        p_CommonData->dataRecorderVector.clear();
                        p_CommonData->dataRecordMutex.unlock();
                        WriteDataToFile();
                        p_CommonData->recordFlag=false;
                    }

                    if(p_CommonData->TrialType == "training" || p_CommonData->TrialType == "testing")
                    {
                        if(p_CommonData->target1Responded == true)
                        {
                            p_CommonData->trialNo++;
                        }
                    }

                    else{
                        p_CommonData->trialNo++;
                    }

                    qDebug("recorded");
                }

                if (readExpStuffIn()){
                    qDebug()<<"successful read"; // << p_CommonData->compCD;
                }

                qDebug()<<"changeh?";
                p_CommonData->environmentChange = true;
                QThread::msleep(200);

                if(p_CommonData->recordFlag == true){
                    p_CommonData->dataRecordMutex.lock();
                    localDataRecorderVector = p_CommonData->dataRecorderVector;
                    p_CommonData->dataRecorderVector.clear();
                    p_CommonData->dataRecordMutex.unlock();
                    WriteDataToFile();
                    p_CommonData->recordFlag=false;
                }

                qDebug("recorded");

                if (p_CommonData->TrialType == "training")
                {
                    QString labelText = "<P><FONT COLOR='#000000' FONT SIZE = 5>";
                    labelText .append("training");
                    labelText .append("</P></br>");
                    ui->text->setText(labelText);
                }

                else if (p_CommonData->TrialType == "testing")
                {
                    QString labelText = "<P><FONT COLOR='#000000' FONT SIZE = 5>";
                    labelText .append("trial");
                    labelText .append("</P></br>");
                    ui->text->setText(labelText);
                }

                else if (p_CommonData->TrialType == "break")
                {
                    QString labelText = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                    labelText .append("PRESS NEXT AFTER THE BREAK");
                    labelText .append("</b></P></br>");
                    ui->text->setText(labelText);



                }

                else if (p_CommonData->TrialType == "breakbreak")
                {
                    QString labelText = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                    labelText .append("TIME TO CHANGE THE GROUNDING");
                    labelText .append("</b></P></br>");
                    ui->text->setText(labelText);
                }

                else if (p_CommonData->TrialType == "end")
                {
                    QString labelText = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                    labelText .append("END OF THE EXPERIMENT");
                    labelText .append("</b></P></br>");
                    ui->text->setText(labelText);
                }
            }
        }

        if (p_CommonData->currentDynamicObjectState == StiffnessMassExperiment)
        {
            //mistake boolean for incase the user presses "H" prematurely
            mistake = false;
            p_CommonData->flagMassExp = true;

            qDebug("advance StiffnessMassExperiment");
            if(CheckFingers()&& (p_CommonData->fingerTouching == false && p_CommonData->thumbTouching == false))
            {
                if(p_CommonData->trialNo<1)
                {
                    p_CommonData->trialNo = 1;
                    p_CommonData->recordFlag = true;

                    //Read in protocol file and check if the read is successful
                    if (readExpStuffIn())
                    {
                        qDebug()<<"readExpStuffIn() SUCCESS"; // << p_CommonData->compCD;
                    }/*
                    else
                    {
                        qDebug()<<"FAILED TO READ PROTOCOL FILE";
                    }*/

                    if (p_CommonData->TrialType == "training")
                    {
                        QString labelText = "<P><FONT COLOR='#000000' FONT SIZE = 5>";
                        labelText .append("training");
                        labelText .append("</P></br>");
                        ui->text->setText(labelText);
                    }

                    else if (p_CommonData->TrialType == "testing")
                    {
                        QString labelText = "<P><FONT COLOR='#000000' FONT SIZE = 5>";
                        labelText .append("trial");
                        labelText .append("</P></br>");
                        ui->text->setText(labelText);
                    }

                    else if (p_CommonData->TrialType == "break")
                    {
                        QString labelText = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                        labelText .append("PRESS NEXT AFTER THE BREAK");
                        labelText .append("</b></P></br>");
                        ui->text->setText(labelText);
                    }

                    else if (p_CommonData->TrialType == "breakbreak")
                    {
                        QString labelText = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                        labelText .append("TIME TO CHANGE THE GROUNDING");
                        labelText .append("</b></P></br>");
                        ui->text->setText(labelText);
                    }

                    else if (p_CommonData->TrialType == "end")
                    {
                        QString labelText = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                        labelText .append("END OF THE EXPERIMENT");
                        labelText .append("</b></P></br>");
                        ui->text->setText(labelText);
                    }

                    qDebug("recorded");
                    p_CommonData->environmentChange = true;

                }

                else {
                    if(p_CommonData->recordFlag)
                    {
                        p_CommonData->dataRecordMutex.lock();
                        localDataRecorderVector = p_CommonData->dataRecorderVector;
                        p_CommonData->dataRecorderVector.clear();
                        p_CommonData->dataRecordMutex.unlock();
                        WriteDataToFile();
                        p_CommonData->recordFlag = false;
                    }

                    if(p_CommonData->TrialType=="training" || p_CommonData->TrialType=="testing")
                    {
                        if(p_CommonData->answerComplete == true){
                            p_CommonData->trialNo++;
                            if (readExpStuffIn()){
                                qDebug()<<"successful read";
                            }

                            if (p_CommonData->TrialType == "training"){
                                QString labelText = "<P><FONT COLOR='#000000' FONT SIZE = 5>";
                                labelText .append("training");
                                labelText .append("</P></br>");
                                ui->text->setText(labelText);
                            }

                            else if (p_CommonData->TrialType == "testing"){
                                QString labelText = "<P><FONT COLOR='#000000' FONT SIZE = 5>";
                                labelText .append("trial");
                                labelText .append("</P></br>");
                                ui->text->setText(labelText);
                            }

                            else if (p_CommonData->TrialType == "break"){
                                QString labelText = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                                labelText .append("PRESS NEXT AFTER THE BREAK");
                                labelText .append("</b></P></br>");
                                ui->text->setText(labelText);
                            }

                            else if (p_CommonData->TrialType == "breakbreak"){
                                QString labelText = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                                labelText .append("TIME TO CHANGE THE GROUNDING");
                                labelText .append("</b></P></br>");
                                ui->text->setText(labelText);
                            }

                            else if (p_CommonData->TrialType == "end"){
                                QString labelText = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                                labelText .append("END OF THE EXPERIMENT");
                                labelText .append("</b></P></br>");
                                ui->text->setText(labelText);
                            }

                            qDebug("recorded");
                            p_CommonData->environmentChange = true;
                        }

                        else if(p_CommonData->answerComplete == false){

                            if(p_CommonData->target1Complete && p_CommonData->target2Complete){

                                qDebug()<< "WHY THE HELL I'M NOT HERE?!";

                                QString labelText = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                                labelText .append("PLEASE ANSWER");
                                labelText .append("</b></P></br>");
                                ui->text->setText(labelText);
                                mistake = true;
                            }

                            else if(!p_CommonData->target1Complete  && p_CommonData->target2Complete){

                                if(p_CommonData->hoop1Complete){
                                    QString labelText1 = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                                    labelText1 .append("PLEASE INSERT BLUE BOX TO BLUE ZONE");
                                    labelText1 .append("</b></P></br>");
                                    ui->text->setText(labelText1);
                                    mistake = true;
                                }

                                else if(!p_CommonData->hoop1Complete){
                                    QString labelText1 = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                                    labelText1 .append("PLEASE PASS BLUE BOX FROM BLUE HOOP");
                                    labelText1 .append("</b></P></br>");
                                    ui->text->setText(labelText1);
                                    mistake = true;
                                }
                            }

                            else if(p_CommonData->target1Complete  && !p_CommonData->target2Complete){

                                if(p_CommonData->hoop2Complete){
                                    QString labelText1 = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                                    labelText1 .append("PLEASE INSERT YELLOW BOX TO YELLOW ZONE");
                                    labelText1 .append("</b></P></br>");
                                    ui->text->setText(labelText1);
                                    mistake = true;
                                }

                                else if(!p_CommonData->hoop2Complete){
                                    QString labelText1 = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                                    labelText1 .append("PLEASE PASS YELLOW BOX FROM YELLOW HOOP");
                                    labelText1 .append("</b></P></br>");
                                    ui->text->setText(labelText1);
                                    mistake = true;
                                }
                            }

                            else if(!p_CommonData->target1Complete  && !p_CommonData->target2Complete){

                                if(p_CommonData->hoop1Complete && p_CommonData->hoop2Complete ){
                                    QString labelText1 = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                                    labelText1 .append("PLEASE INSERT BOXES TO TARGET ZONES");
                                    labelText1 .append("</b></P></br>");
                                    ui->text->setText(labelText1);
                                    mistake = true;
                                }

                                else if(p_CommonData->hoop1Complete && !p_CommonData->hoop2Complete){
                                    QString labelText1 = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                                    labelText1 .append("PLEASE INSERT BLUE BOX TO BLUE ZONE AND PASS YELLOW BOX FROM YELLOW HOOP");
                                    labelText1 .append("</b></P></br>");
                                    ui->text->setText(labelText1);
                                    mistake = true;
                                }

                                else if(!p_CommonData->hoop1Complete && p_CommonData->hoop2Complete){
                                    QString labelText1 = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                                    labelText1 .append("PLEASE PASS BLUE BOX FROM BLUE HOOP AND INSERT YELLOW BOX TO YELLOW ZONE");
                                    labelText1 .append("</b></P></br>");
                                    ui->text->setText(labelText1);
                                    mistake = true;
                                }

                                else if(!p_CommonData->hoop1Complete && !p_CommonData->hoop2Complete){
                                    QString labelText1 = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                                    labelText1 .append("PLEASE PASS BOXES FROM TARGET HOOPS");
                                    labelText1 .append("</b></P></br>");
                                    ui->text->setText(labelText1);
                                    mistake = true;
                                }
                            }
                        }
                    }

                    else {
                        p_CommonData->trialNo++;

                        if (readExpStuffIn()){
                            qDebug()<<"successful read";
                        }
                        if (p_CommonData->TrialType == "training"){
                            QString labelText = "<P><FONT COLOR='#000000' FONT SIZE = 5>";
                            labelText .append("training");
                            labelText .append("</P></br>");
                            ui->text->setText(labelText);
                        }

                        else if (p_CommonData->TrialType == "testing"){
                            QString labelText = "<P><FONT COLOR='#000000' FONT SIZE = 5>";
                            labelText .append("trial");
                            labelText .append("</P></br>");
                            ui->text->setText(labelText);
                        }

                        else if (p_CommonData->TrialType == "break"){
                            QString labelText = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                            labelText .append("PRESS NEXT AFTER THE BREAK");
                            labelText .append("</b></P></br>");
                            ui->text->setText(labelText);
                        }

                        else if (p_CommonData->TrialType == "breakbreak"){
                            QString labelText = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                            labelText .append("TIME TO CHANGE THE GROUNDING");
                            labelText .append("</b></P></br>");
                            ui->text->setText(labelText);
                        }

                        else if (p_CommonData->TrialType == "end"){
                            QString labelText = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                            labelText .append("END OF THE EXPERIMENT");
                            labelText .append("</b></P></br>");
                            ui->text->setText(labelText);
                        }

                        qDebug("recorded");

                        qDebug()<<"changeh?";

                        p_CommonData->environmentChange = true;
                    }
                }
            }
        }

        if (p_CommonData->currentDynamicObjectState == FingerMappingExperiment)
        {
            //mistake boolean for incase the user presses "H" prematurely
            mistake = false;
            p_CommonData->flagMassExp = false;

            qDebug("advance FingerMappingExp");
            progressPickAndPlaceExperiment(mistake);
        }

        if (p_CommonData->currentDynamicObjectState == HoxelMappingExperiment)
        {
            //mistake boolean for incase the user presses "H" prematurely
            mistake = false;
            p_CommonData->flagMassExp = false;

            qDebug("advance HoxelMappingExp");
            progressPickAndPlaceExperiment(mistake);
        }

        if (p_CommonData->currentDynamicObjectState == CrumblyCubeExperiment)
        {
            //mistake boolean for incase the user presses "H" prematurely
            mistake = false;
            p_CommonData->flagMassExp = false;

            qDebug("advance HoxelMappingExp");
            progressPickAndPlaceExperiment(mistake);
        }

        else if (p_CommonData->currentDynamicObjectState == CubeGuidanceExperiment)
        {
            //mistake boolean for incase the user presses "H" prematurely
            mistake = false;
            p_CommonData->flagMassExp = false;

            qDebug("advance CubeGuidanceExp");

            progressPickAndPlaceExperiment(mistake);
            qDebug() << "mistake -- " << mistake << " -- numMistakes:" << p_CommonData->mistakeCounter;
        }
    }

    /***Stiffness-Mass and Mass Discrimination Buttons***/
    // choosing box1
    if(a_event->key() == Qt::Key_J)
    {
        // this means the user thinks box 1 is heavier or stiffer
        p_CommonData->answer1 = true;
    }
    // choosing box2
    if(a_event->key() == Qt::Key_K)
    {
        // this means the user thinks box 2 is heavier or stiffer
        p_CommonData->answer2 = true;
    }

    /***Tissue Study Buttons***/
    double angle = 10.0;
    //Rotate Tissue - (CW)
    if (a_event->key() == Qt::Key_Z)
    {
        rotateTissueLineDisp(-angle);
        p_CommonData->p_indicator->setTransparencyLevel(1, true);
    }
    //Rotate Tissue + (CCW)
    if (a_event->key() == Qt::Key_X)
    {
        rotateTissueLineDisp(angle);
        p_CommonData->p_indicator->setTransparencyLevel(1, true);
    }
}

//Determine the folder for subject data depending on the experiment being run
QString MainWindow::getSubjectDirectory()
{
    if(p_CommonData->currentDynamicObjectState == StiffnessExperiment ||
            p_CommonData->currentDynamicObjectState == StiffnessMassExperiment)
    {
        return "./Subjects";
    }
    if (p_CommonData->currentDynamicObjectState == FingerMappingExperiment)
    {
        return "./FME_Subject_Data_v2/";
    }
    if (p_CommonData->currentDynamicObjectState == HoxelMappingExperiment)
    {
        return  "./HME_Subject_Data/";
    }
    if (p_CommonData->currentDynamicObjectState == CrumblyCubeExperiment)
    {
        if(p_CommonData->cceExpType == 1)
        {
            return  "./CCE_Subject_Data/CCE_ExpType1/";
        }
        if(p_CommonData->cceExpType == 2)
        {
            return  "./CCE_Subject_Data/CCE_ExpType2/";
        }
        if(p_CommonData->cceExpType == 3)
        {
            return  "./CCE_Subject_Data/CCE_ExpType3/";
        }
        // This shouldn't happen but just in case:
        else
        {
            return  "./CCE_Subject_Data/";
        }
    }
    else if (p_CommonData->currentDynamicObjectState == CubeGuidanceExperiment)
    {
        return "./CGE_Subject_Data/";
    }
}

//Writes the experiment data to txt File in Subject Directory
void MainWindow::WriteDataToFile()
{
    qDebug()<<"WriteDataToFile()";
    p_CommonData->recordFlag = false;

    char trialBuffer[33];
    //char cdBuffer[33];
    //char inertiaBuffer[33];

    //double file_number;
    //file_number = p_CommonData->trialNo;

    itoa(p_CommonData->trialNo, trialBuffer, 10);
    //itoa((int)(p_CommonData->compCD*10),cdBuffer,10);
    //itoa((int)(p_CommonData->compInertia*10),inertiaBuffer,10);

    QString directory = getSubjectDirectory() + QString::number(p_CommonData->subjectNo);

    //Create folder for data
    if(p_CommonData->currentDynamicObjectState == StiffnessExperiment){
        directory = getSubjectDirectory() + "StiffnessExperiment" + QString::number(p_CommonData->subjectNo);
    }
    if(p_CommonData->currentDynamicObjectState == StiffnessMassExperiment){
        directory = getSubjectDirectory() + "StiffnessMassExperiment" + QString::number(p_CommonData->subjectNo);
    }
    if(p_CommonData->currentDynamicObjectState == FingerMappingExperiment ||
            p_CommonData->currentDynamicObjectState == HoxelMappingExperiment ||
            p_CommonData->currentDynamicObjectState == CrumblyCubeExperiment ||
            p_CommonData->currentDynamicObjectState == CubeGuidanceExperiment){
        if(p_CommonData->TrialType == "training")
        {
            directory = getSubjectDirectory() + "TrainingTrialData";
            qDebug()<<"Saved data to: "<<directory;
        }
        else
        {
            directory = getSubjectDirectory();
            qDebug()<<"Saved data to: "<<directory;
        }
    }

    //Create file object (needed for later parsing)
    std::ofstream file;

    //File Name for Mine's Experiments
    if (p_CommonData->currentDynamicObjectState == StiffnessExperiment ||
            p_CommonData->currentDynamicObjectState == StiffnessMassExperiment)
    {
        //Sort data by trialName
        QString trialName;
        if (p_CommonData->direct == 1){
            trialName = "vertical_"; // + QString::number(p_CommonData->day)+"_";
        }
        else if (p_CommonData->direct == 2){
            trialName = "horizontal_"; // + QString::number(p_CommonData->day)+"_";
        }

        //Sort data by trialMode
        QString trialMode;
        if (p_CommonData->TrialMode == 1){
            trialMode = "mass_"; // + QString::number(p_CommonData->day)+"_";
        }
        else if (p_CommonData->TrialMode == 2){
            trialMode = "stiffness_"; // + QString::number(p_CommonData->day)+"_";
        }    //std::ofstream file;

        //Open file to write data:
        file.open(directory.toStdString() + "/" + trialName.toStdString() + trialMode.toStdString() + trialBuffer + "trialRun.csv");
    }
    //File names for Jasmin's Experiments
    else if (p_CommonData->currentDynamicObjectState == FingerMappingExperiment ||
             p_CommonData->currentDynamicObjectState == HoxelMappingExperiment ||
             p_CommonData->currentDynamicObjectState == CrumblyCubeExperiment||
             p_CommonData->currentDynamicObjectState == CubeGuidanceExperiment )
    {
        //Sort data by trialName
        QString trialNum = QString::number(p_CommonData->trialNo);

        //std::ofstream file;

        //Formatting to help with post-processing
        if (p_CommonData->trialNo < 10)
        {
            //Open file to write data:
            //if no mistakes due to premature presses of H:
            if(p_CommonData->mistakeCounter == 0)
            {
                file.open(directory.toStdString()
                          +"/Subject" + QString::number(p_CommonData->subjectNo).toStdString()
                          + "_Trial0" + trialNum.toStdString()
                          + "_Mapping" + QString::number(p_CommonData->mapping).toStdString()
                          + ".csv");
            }
            else //append mistakes number so that files can be combined in post-processing
            {
                file.open(directory.toStdString()
                          +"/Subject" + QString::number(p_CommonData->subjectNo).toStdString()
                          + "_Trial0" + trialNum.toStdString()
                          + "_Mapping" + QString::number(p_CommonData->mapping).toStdString()
                          + "-MISTAKE-NUM-" + QString::number(p_CommonData->mistakeCounter).toStdString()
                          + ".csv");
            }
        }
        else
        {
            //Open file to write data:
            //if no mistakes due to premature presses of H:
            if(p_CommonData->mistakeCounter == 0)
            {
                file.open(directory.toStdString()
                          +"/Subject" + QString::number(p_CommonData->subjectNo).toStdString()
                          + "_Trial" + trialNum.toStdString()
                          + "_Mapping" + QString::number(p_CommonData->mapping).toStdString()
                          + ".csv");
            }
            else //append mistakes number so that files can be combined in post-processing
            {
                file.open(directory.toStdString()
                          +"/Subject" + QString::number(p_CommonData->subjectNo).toStdString()
                          + "_Trial" + trialNum.toStdString()
                          + "_Mapping" + QString::number(p_CommonData->mapping).toStdString()
                          + "-MISTAKE-NUM-" + QString::number(p_CommonData->mistakeCounter).toStdString()
                          + ".csv");
            }
        }
    }

    //write data to file when we are done

#ifdef ACC
    p_CommonData->dir= "./VTACC_Subject_Data/VTExp/Subjects/VTACC";//"C:/Users/Sam/Desktop/Subjects/VTExp/Subjects/VTACC";
    p_CommonData->fileName = "VTACC_CAL";
#endif


    //Create Headers for files depending on experiment
    if(p_CommonData->currentDynamicObjectState == StiffnessExperiment ||
            p_CommonData->currentDynamicObjectState == StiffnessMassExperiment)
    {
        //none for now
    }
    /*
    if(p_CommonData->currentDynamicObjectState == FingerMappingExperiment)
    {
        //These ~MUST~ match the order of variables saved below:
        file <<std::setprecision(9)
            << "time" << "," << " " //time in seconds

            << "realDorsalTactorPos" << "," << " "//in mm

            << "realVentralTactorPos" << "," << " " //in mm

               //boxPos is a vector and will need 3 headers
            << "boxPosX" << "," << " " //in m
            << "boxPosY" << "," << " " //in m
            << "boxPosZ" << "," << " " //in m

               //magTrackerPos vectors will need 3 headers each
            << "indexPosX" << "," << " " //in m
            << "indexPosY" << "," << " " //in m
            << "indexPosZ" << "," << " " //in m
            << "indexContact" << "," << " " //bool
            << "indexForceX" << "," << " " //in N
            << "indexForceY" << "," << " " //in N
            << "indexForceZ" << "," << " " //in N
            << "indexForceGlobalX" << "," << " " //in N
            << "indexForceGlobalY" << "," << " " //in N
            << "indexForceGlobalZ" << "," << " " //in N

               //magTrackerPos vectors will need 3 headers each
            << "thumbPosX" << "," << " " //in m
            << "thumbPosY" << "," << " " //in m
            << "thumbPosZ" << "," << " " //in m
            << "thumbContact" << "," << " " //bool
            << "thumbForceX" << "," << " " //in N
            << "thumbForceY" << "," << " " //in N
            << "thumbForceZ" << "," << " " //in N
            << "thumbForceGlobalX" << "," << " " //in N
            << "thumbForceGlobalY" << "," << " " //in N
            << "thumbForceGlobalZ" << "," << " " //in N

            << "hoopSuccess" << "," << " " //bool
            << "targetSuccess" << "," << " " //bool
            << "trialSuccess" << "," << " " //bool

            << std::endl;
    }
    */
    if (p_CommonData->currentDynamicObjectState == FingerMappingExperiment ||
            p_CommonData->currentDynamicObjectState == HoxelMappingExperiment)
    {
        //These *MUST* match the order of variables saved below:
        file <<std::setprecision(9)
            << "time" << "," << " " //time in seconds

            << "realDorsalTactorPos" << "," << " "//in mm

            << "realVentralTactorPos" << "," << " " //in mm

               //boxPos is a vector and will need 3 headers per box
            << "boxPosX" << "," << " " //in m
            << "boxPosY" << "," << " " //in m
            << "boxPosZ" << "," << " " //in m

               //box1 local rotation Matrix
            << "boxLocalRot_11" << "," << " "
            << "boxLocalRot_12" << "," << " "
            << "boxLocalRot_13" << "," << " "
            << "boxLocalRot_21" << "," << " "
            << "boxLocalRot_22" << "," << " "
            << "boxLocalRot_23" << "," << " "
            << "boxLocalRot_31" << "," << " "
            << "boxLocalRot_32" << "," << " "
            << "boxLocalRot_33" << "," << " "
               //box1 global rotation Matrix
            << "boxGlobalRot_11" << "," << " "
            << "boxGlobalRot_12" << "," << " "
            << "boxGlobalRot_13" << "," << " "
            << "boxGlobalRot_21" << "," << " "
            << "boxGlobalRot_22" << "," << " "
            << "boxGlobalRot_23" << "," << " "
            << "boxGlobalRot_31" << "," << " "
            << "boxGlobalRot_32" << "," << " "
            << "boxGlobalRot_33" << "," << " "

               //interaction forces in local coordinates
            << "indexForceX" << "," << " " //in N
            << "indexForceY" << "," << " " //in N
            << "indexForceZ" << "," << " " //in N
               //interaction force in global coordinates
            << "indexForceGlobalX" << "," << " " //in N
            << "indexForceGlobalY" << "," << " " //in N
            << "indexForceGlobalZ" << "," << " " //in N

               //Contact boolean
            << "indexContact" << "," << " " //bool

               //magTrackerPos vectors will need 3 headers each
            << "indexPosX" << "," << " " //in m
            << "indexPosY" << "," << " " //in m
            << "indexPosZ" << "," << " " //in m

               //Tracker orientation:
            << "indexRot_11" << "," << " "
            << "indexRot_12" << "," << " "
            << "indexRot_13" << "," << " "
            << "indexRot_21" << "," << " "
            << "indexRot_22" << "," << " "
            << "indexRot_23" << "," << " "
            << "indexRot_31" << "," << " "
            << "indexRot_32" << "," << " "
            << "indexRot_33" << "," << " "

               //interaction forces in local coordinates
            << "thumbForceX" << "," << " " //in N
            << "thumbForceY" << "," << " " //in N
            << "thumbForceZ" << "," << " " //in N

               //interaction force in global coordinates
            << "thumbForceGlobalX" << "," << " " //in N
            << "thumbForceGlobalY" << "," << " " //in N
            << "thumbForceGlobalZ" << "," << " " //in N

               //Contact boolean
            << "thumbContact" << "," << " " //bool

               //magTrackerPos vectors will need 3 headers each
            << "thumbPosX" << "," << " " //in m
            << "thumbPosY" << "," << " " //in m
            << "thumbPosZ" << "," << " " //in m

               //Tracker orientation:

            << "thumbRot_11" << "," << " "
            << "thumbRot_12" << "," << " "
            << "thumbRot_13" << "," << " "
            << "thumbRot_21" << "," << " "
            << "thumbRot_22" << "," << " "
            << "thumbRot_23" << "," << " "
            << "thumbRot_31" << "," << " "
            << "thumbRot_32" << "," << " "
            << "thumbRot_33" << "," << " "

            << "mapping" << "," << " "
            << "trialSuccess" << "," << " " //bool

            << std::endl;
    }
    if (p_CommonData->currentDynamicObjectState == CubeGuidanceExperiment)
    {
        //These *MUST* match the order of variables saved below:
        file <<std::setprecision(9)
            << "time" << "," << " " //time in seconds

            << "realDorsalTactorPos" << "," << " "//in mm

            << "realVentralTactorPos" << "," << " " //in mm

               //boxPos is a vector and will need 3 headers per box
            << "boxPosX" << "," << " " //in m
            << "boxPosY" << "," << " " //in m
            << "boxPosZ" << "," << " " //in m

               //box1 local rotation Matrix
            << "boxLocalRot_11" << "," << " "
            << "boxLocalRot_12" << "," << " "
            << "boxLocalRot_13" << "," << " "
            << "boxLocalRot_21" << "," << " "
            << "boxLocalRot_22" << "," << " "
            << "boxLocalRot_23" << "," << " "
            << "boxLocalRot_31" << "," << " "
            << "boxLocalRot_32" << "," << " "
            << "boxLocalRot_33" << "," << " "
               //box1 global rotation Matrix
            << "boxGlobalRot_11" << "," << " "
            << "boxGlobalRot_12" << "," << " "
            << "boxGlobalRot_13" << "," << " "
            << "boxGlobalRot_21" << "," << " "
            << "boxGlobalRot_22" << "," << " "
            << "boxGlobalRot_23" << "," << " "
            << "boxGlobalRot_31" << "," << " "
            << "boxGlobalRot_32" << "," << " "
            << "boxGlobalRot_33" << "," << " "

               //interaction forces in local coordinates
            << "indexForceX" << "," << " " //in N
            << "indexForceY" << "," << " " //in N
            << "indexForceZ" << "," << " " //in N
               //interaction force in global coordinates
            << "indexForceGlobalX" << "," << " " //in N
            << "indexForceGlobalY" << "," << " " //in N
            << "indexForceGlobalZ" << "," << " " //in N

               //Contact boolean
            << "indexContact" << "," << " " //bool

               //magTrackerPos vectors will need 3 headers each
            << "indexPosX" << "," << " " //in m
            << "indexPosY" << "," << " " //in m
            << "indexPosZ" << "," << " " //in m

               //Tracker orientation:
            << "indexRot_11" << "," << " "
            << "indexRot_12" << "," << " "
            << "indexRot_13" << "," << " "
            << "indexRot_21" << "," << " "
            << "indexRot_22" << "," << " "
            << "indexRot_23" << "," << " "
            << "indexRot_31" << "," << " "
            << "indexRot_32" << "," << " "
            << "indexRot_33" << "," << " "

               //interaction forces in local coordinates
            << "thumbForceX" << "," << " " //in N
            << "thumbForceY" << "," << " " //in N
            << "thumbForceZ" << "," << " " //in N

               //interaction force in global coordinates
            << "thumbForceGlobalX" << "," << " " //in N
            << "thumbForceGlobalY" << "," << " " //in N
            << "thumbForceGlobalZ" << "," << " " //in N

               //Contact boolean
            << "thumbContact" << "," << " " //bool

               //magTrackerPos vectors will need 3 headers each
            << "thumbPosX" << "," << " " //in m
            << "thumbPosY" << "," << " " //in m
            << "thumbPosZ" << "," << " " //in m

               //Tracker orientation:

            << "thumbRot_11" << "," << " "
            << "thumbRot_12" << "," << " "
            << "thumbRot_13" << "," << " "
            << "thumbRot_21" << "," << " "
            << "thumbRot_22" << "," << " "
            << "thumbRot_23" << "," << " "
            << "thumbRot_31" << "," << " "
            << "thumbRot_32" << "," << " "
            << "thumbRot_33" << "," << " "

            << "mapping" << "," << " "
            << "boxInitParam" << "," << " "
            << "trialSuccess" << "," << " " //bool

            << std::endl;
    }

    else if (p_CommonData->currentDynamicObjectState == CrumblyCubeExperiment)
    {
        //These *MUST* match the order of variables saved below:
        file <<std::setprecision(9)
            << "time" << "," << " " //time in seconds

            << "realDorsalTactorPos" << "," << " "//in mm

            << "realVentralTactorPos" << "," << " " //in mm

               //boxPos is a vector and will need 3 headers per box
            << "box1PosX" << "," << " " //in m
            << "box1PosY" << "," << " " //in m
            << "box1PosZ" << "," << " " //in m
               /*
            << "box2PosX" << "," << " " //in m
            << "box2PosY" << "," << " " //in m
            << "box2PosZ" << "," << " " //in m
            << "box3PosX" << "," << " " //in m
            << "box3PosY" << "," << " " //in m
            << "box3PosZ" << "," << " " //in m
               */

               //box1 local rotation Matrix
            << "box1LocalRot_11" << "," << " "
            << "box1LocalRot_12" << "," << " "
            << "box1LocalRot_13" << "," << " "
            << "box1LocalRot_21" << "," << " "
            << "box1LocalRot_22" << "," << " "
            << "box1LocalRot_23" << "," << " "
            << "box1LocalRot_31" << "," << " "
            << "box1LocalRot_32" << "," << " "
            << "box1LocalRot_33" << "," << " "
               //box1 global rotation Matrix
            << "box1GlobalRot_11" << "," << " "
            << "box1GlobalRot_12" << "," << " "
            << "box1GlobalRot_13" << "," << " "
            << "box1GlobalRot_21" << "," << " "
            << "box1GlobalRot_22" << "," << " "
            << "box1GlobalRot_23" << "," << " "
            << "box1GlobalRot_31" << "," << " "
            << "box1GlobalRot_32" << "," << " "
            << "box1GlobalRot_33" << "," << " "
            /*
               //box2 local rotation Matrix
            << "box2LocalRot_11" << "," << " "
            << "box2LocalRot_12" << "," << " "
            << "box2LocalRot_13" << "," << " "
            << "box2LocalRot_21" << "," << " "
            << "box2LocalRot_22" << "," << " "
            << "box2LocalRot_23" << "," << " "
            << "box2LocalRot_31" << "," << " "
            << "box2LocalRot_32" << "," << " "
            << "box2LocalRot_33" << "," << " "
               //box2 global rotation Matrix
            << "box2GlobalRot_11" << "," << " "
            << "box2GlobalRot_12" << "," << " "
            << "box2GlobalRot_13" << "," << " "
            << "box2GlobalRot_21" << "," << " "
            << "box2GlobalRot_22" << "," << " "
            << "box2GlobalRot_23" << "," << " "
            << "box2GlobalRot_31" << "," << " "
            << "box2GlobalRot_32" << "," << " "
            << "box2GlobalRot_33" << "," << " "

               //box3 local rotation Matrix
            << "box3LocalRot_11" << "," << " "
            << "box3LocalRot_12" << "," << " "
            << "box3LocalRot_13" << "," << " "
            << "box3LocalRot_21" << "," << " "
            << "box3LocalRot_22" << "," << " "
            << "box3LocalRot_23" << "," << " "
            << "box3LocalRot_31" << "," << " "
            << "box3LocalRot_32" << "," << " "
            << "box3LocalRot_33" << "," << " "
               //box3 global rotation Matrix
            << "box3GlobalRot_11" << "," << " "
            << "box3GlobalRot_12" << "," << " "
            << "box3GlobalRot_13" << "," << " "
            << "box3GlobalRot_21" << "," << " "
            << "box3GlobalRot_22" << "," << " "
            << "box3GlobalRot_23" << "," << " "
            << "box3GlobalRot_31" << "," << " "
            << "box3GlobalRot_32" << "," << " "
            << "box3GlobalRot_33" << "," << " "
            */
               //interaction forces in local coordinates
            << "indexForceX" << "," << " " //in N
            << "indexForceY" << "," << " " //in N
            << "indexForceZ" << "," << " " //in N
               //interaction force in global coordinates
            << "indexForceGlobalX" << "," << " " //in N
            << "indexForceGlobalY" << "," << " " //in N
            << "indexForceGlobalZ" << "," << " " //in N

               //Contact boolean
            << "indexContact" << "," << " " //bool

               //magTrackerPos vectors will need 3 headers each
            << "indexPosX" << "," << " " //in m
            << "indexPosY" << "," << " " //in m
            << "indexPosZ" << "," << " " //in m

               //Tracker orientation:
            << "indexRot_11" << "," << " "
            << "indexRot_12" << "," << " "
            << "indexRot_13" << "," << " "
            << "indexRot_21" << "," << " "
            << "indexRot_22" << "," << " "
            << "indexRot_23" << "," << " "
            << "indexRot_31" << "," << " "
            << "indexRot_32" << "," << " "
            << "indexRot_33" << "," << " "

               //interaction forces in local coordinates
            << "thumbForceX" << "," << " " //in N
            << "thumbForceY" << "," << " " //in N
            << "thumbForceZ" << "," << " " //in N

               //interaction force in global coordinates
            << "thumbForceGlobalX" << "," << " " //in N
            << "thumbForceGlobalY" << "," << " " //in N
            << "thumbForceGlobalZ" << "," << " " //in N

               //Contact boolean
            << "thumbContact" << "," << " " //bool

               //magTrackerPos vectors will need 3 headers each
            << "thumbPosX" << "," << " " //in m
            << "thumbPosY" << "," << " " //in m
            << "thumbPosZ" << "," << " " //in m

               //Tracker orientation:

            << "thumbRot_11" << "," << " "
            << "thumbRot_12" << "," << " "
            << "thumbRot_13" << "," << " "
            << "thumbRot_21" << "," << " "
            << "thumbRot_22" << "," << " "
            << "thumbRot_23" << "," << " "
            << "thumbRot_31" << "," << " "
            << "thumbRot_32" << "," << " "
            << "thumbRot_33" << "," << " "

            << "manipForceTooHigh" << "," << " " //bool
            << "cceExpType" << "," << " "
            << "mapping" << "," << " "
            << "trialSuccess" << "," << " " //bool

            << std::endl;
    }

    //Parse through localDataVector to more easily readable form in .txt or .csv file
    for (int i = 0; i < localDataRecorderVector.size(); i++)
    {
        //[0] is distal finger, [1] is toward middle finger, [2] is away from finger pad
#ifndef ACC
        //The comma and the space helps you to read the data easily on MATLAB later
        //Choose Check which experiment you are interested in and commnet/uncomment desired parameters
        if(p_CommonData->currentDynamicObjectState == StiffnessExperiment ||
                p_CommonData->currentDynamicObjectState == StiffnessMassExperiment)
        {
            file <<std::setprecision(9)<< ""  //"trial = " << localDataRecorderVector[i].time << "," << " "
                   //              "time = "
                << localDataRecorderVector[i].time << "," << " "
                   //              "des0X = "
                << localDataRecorderVector[i].desiredPos0[0] << "," << " "
                   //              "des0Y = "
                << localDataRecorderVector[i].desiredPos0[1] << "," << " "
                   //              "des0Z = "
                << localDataRecorderVector[i].desiredPos0[2] << "," << " "
                   //              "real0 = "
                << localDataRecorderVector[i].strokeOut0 << "," << " "
                   //              "des0 = "
                << localDataRecorderVector[i].desiredStroke0 << "," << " "
                   //"fingerpose_x = "
                   //              <<localDataRecorderVector[i].magTrackerPos0[0] << "," << " "
                   //"fingerpose_y = "
                   //            <<localDataRecorderVector[i].magTrackerPos0[1] << "," << " "
                   //"fingerpose_z = "
                   //            <<localDataRecorderVector[i].magTrackerPos0[2] << "," << " "
                   //"des1X = "
                << localDataRecorderVector[i].desiredPos1[0] << "," << " "
                   //              "des1Y = "
                << localDataRecorderVector[i].desiredPos1[1] << "," << " "
                   //              "des1Z = "
                << localDataRecorderVector[i].desiredPos1[2] << "," << " "
                   //              "real1 = "
                << localDataRecorderVector[i].strokeOut1 << "," << " "
                   //              "des1 = "
                << localDataRecorderVector[i].desiredStroke1 << "," << " "
                   //              "fingerpose_x = "
                   //            <<localDataRecorderVector[i].magTrackerPos1[0] << "," << " "
                   //              "fingerpose_y = "
                   //            <<localDataRecorderVector[i].magTrackerPos1[1] << "," << " "
                   //              "fingerpose_z = "
                   //             <<localDataRecorderVector[i].magTrackerPos1[2] << "," << " "
                   //              "cond = "
                << localDataRecorderVector[i].conditionNo<< "," << " "
                   //              "scale = "
                << localDataRecorderVector[i].strokeScale<< "," << " "
                   //              "success = "
                << localDataRecorderVector[i].success<< "," << " "
                   //              "stiff1 = "
                << localDataRecorderVector[i].box1Stiffness << "," << " "
                   //              "stiff2 = "
                << localDataRecorderVector[i].box2Stiffness << "," << " "
                << localDataRecorderVector[i].dir << "," << " "
                << localDataRecorderVector[i].magTrackerPos0 << "," << " "
                << localDataRecorderVector[i].magTrackerPos1 << "," << " "
                << localDataRecorderVector[i].indexContact << "," << " "
                << localDataRecorderVector[i].thumbContact << "," << " "

                << std::endl;
        }
        /*
        if(p_CommonData->currentDynamicObjectState == FingerMappingExperiment)
        {
            file <<std::setprecision(9)<< ""  //"trial = " << localDataRecorderVector[i].time << "," << " "
                   //"time = "
                << localDataRecorderVector[i].time << "," << " "

                   //"des0 = "
                << localDataRecorderVector[i].desiredStroke0 << "," << " "
                   //"real0 = "
                   //<< localDataRecorderVector[i].strokeOut0 << "," << " " //Giving motor angles N/A for FME
                   //"des1 = "
                << localDataRecorderVector[i].desiredStroke1 << "," << " "
                   //"real1 = "
                   //<< localDataRecorderVector[i].strokeOut1 << "," << " "//Giving motor angles N/A for FME

                   //Cube position
                <<localDataRecorderVector[i].box1Pos << "," << " "

                  //"cond = "
                  //<< localDataRecorderVector[i].conditionNo<< "," << " "

                  //INDEX:
               << localDataRecorderVector[i].magTrackerPos0 << "," << " " //magTrackerPos vectors will need 3 headers each
               << localDataRecorderVector[i].indexContact << "," << " " //contact boolean
                  // last force on tool0:
               << localDataRecorderVector[i].VRIntForce0[0]<< "," << " "
               << localDataRecorderVector[i].VRIntForce0[1]<< "," << " "
               << localDataRecorderVector[i].VRIntForce0[2]<< "," << " "
                  // last force on tool0 in global coords
               << localDataRecorderVector[i].VRIntForceGlo0[0]<< "," << " "
               << localDataRecorderVector[i].VRIntForceGlo0[1]<< "," << " "
               << localDataRecorderVector[i].VRIntForceGlo0[2]<< "," << " "

                  //THUMB
               << localDataRecorderVector[i].magTrackerPos1 << "," << " " //magTrackerPos vectors will need 3 headers each
               << localDataRecorderVector[i].thumbContact << "," << " "//contact boolean
                  // last force on tool1:
               << localDataRecorderVector[i].VRIntForce1[0]<< "," << " "
               << localDataRecorderVector[i].VRIntForce1[1]<< "," << " "
               << localDataRecorderVector[i].VRIntForce1[2]<< "," << " "
                  // last force on tool1 in global coords
               << localDataRecorderVector[i].VRIntForceGlo1[0]<< "," << " "
               << localDataRecorderVector[i].VRIntForceGlo1[1]<< "," << " "
               << localDataRecorderVector[i].VRIntForceGlo1[2]<< "," << " "

                  //Touch hoop, target, trial success booleans
               << localDataRecorderVector[i].hoopSuccess << "," << " "
               << localDataRecorderVector[i].targetSuccess << "," << " "
               << localDataRecorderVector[i].trialSuccess << "," << " "

               << std::endl;
        }
        */
        if(p_CommonData->currentDynamicObjectState == FingerMappingExperiment ||
                p_CommonData->currentDynamicObjectState == HoxelMappingExperiment)
        {
            file <<std::setprecision(9)<< ""  //"trial = " << localDataRecorderVector[i].time << "," << " "
                   //"time = "
                << localDataRecorderVector[i].time << "," << " "

                   //"des0 = "
                << localDataRecorderVector[i].desiredStroke0 << "," << " "
                   //"real0 = "
                   //<< localDataRecorderVector[i].strokeOut0 << "," << " " //Giving motor angles N/A for FME
                   //"des1 = "
                << localDataRecorderVector[i].desiredStroke1 << "," << " "
                   //"real1 = "
                   //<< localDataRecorderVector[i].strokeOut1 << "," << " "//Giving motor angles N/A for FME

                   //Cube position
                << localDataRecorderVector[i].box1Pos << "," << " " //Each needs 3 headers

                   //Box1 -- Cube local orientation
                << localDataRecorderVector[i].box1LocalRotMat(0,0) << "," << " "  //Matrix 1st row
                << localDataRecorderVector[i].box1LocalRotMat(0,1) << "," << " "
                << localDataRecorderVector[i].box1LocalRotMat(0,2) << "," << " "
                << localDataRecorderVector[i].box1LocalRotMat(1,0) << "," << " "  //Matrix 2nd row
                << localDataRecorderVector[i].box1LocalRotMat(1,1) << "," << " "
                << localDataRecorderVector[i].box1LocalRotMat(1,2) << "," << " "
                << localDataRecorderVector[i].box1LocalRotMat(2,0) << "," << " "  //Matrix 3rd row
                << localDataRecorderVector[i].box1LocalRotMat(2,1) << "," << " "
                << localDataRecorderVector[i].box1LocalRotMat(2,2) << "," << " "

                   //Box1 --Cube Global orientation
                << localDataRecorderVector[i].box1GlobalRotMat(0,0) << "," << " "  //Matrix 1st row
                << localDataRecorderVector[i].box1GlobalRotMat(0,1) << "," << " "
                << localDataRecorderVector[i].box1GlobalRotMat(0,2) << "," << " "
                << localDataRecorderVector[i].box1GlobalRotMat(1,0) << "," << " "  //Matrix 2nd row
                << localDataRecorderVector[i].box1GlobalRotMat(1,1) << "," << " "
                << localDataRecorderVector[i].box1GlobalRotMat(1,2) << "," << " "
                << localDataRecorderVector[i].box1GlobalRotMat(2,0) << "," << " "  //Matrix 3rd row
                << localDataRecorderVector[i].box1GlobalRotMat(2,1) << "," << " "
                << localDataRecorderVector[i].box1GlobalRotMat(2,2) << "," << " "

                   //"cond = "
                   //<< localDataRecorderVector[i].conditionNo<< "," << " "

                   //INDEX:
                   // last force on tool0:
                << localDataRecorderVector[i].VRIntForce0[0]<< "," << " "
                << localDataRecorderVector[i].VRIntForce0[1]<< "," << " "
                << localDataRecorderVector[i].VRIntForce0[2]<< "," << " "
                   // last force on tool0 in global coords
                << localDataRecorderVector[i].VRIntForceGlo0[0]<< "," << " "
                << localDataRecorderVector[i].VRIntForceGlo0[1]<< "," << " "
                << localDataRecorderVector[i].VRIntForceGlo0[2]<< "," << " "

                << localDataRecorderVector[i].indexContact << "," << " " //contact boolean
                << localDataRecorderVector[i].magTrackerPos0 << "," << " " //magTrackerPos vectors will need 3 headers each

                   //Index tracker orientation:
                << localDataRecorderVector[i].deviceRotation0(0,0) << "," << " "  //Matrix 1st row
                << localDataRecorderVector[i].deviceRotation0(0,1) << "," << " "
                << localDataRecorderVector[i].deviceRotation0(0,2) << "," << " "
                << localDataRecorderVector[i].deviceRotation0(1,0) << "," << " "  //Matrix 2nd row
                << localDataRecorderVector[i].deviceRotation0(1,1) << "," << " "
                << localDataRecorderVector[i].deviceRotation0(1,2) << "," << " "
                << localDataRecorderVector[i].deviceRotation0(2,0) << "," << " "  //Matrix 3rd row
                << localDataRecorderVector[i].deviceRotation0(2,1) << "," << " "
                << localDataRecorderVector[i].deviceRotation0(2,2) << "," << " "

                   //THUMB
                   // last force on tool1:
                << localDataRecorderVector[i].VRIntForce1[0]<< "," << " "
                << localDataRecorderVector[i].VRIntForce1[1]<< "," << " "
                << localDataRecorderVector[i].VRIntForce1[2]<< "," << " "
                   //last force on tool1 in global coords
                << localDataRecorderVector[i].VRIntForceGlo1[0]<< "," << " "
                << localDataRecorderVector[i].VRIntForceGlo1[1]<< "," << " "
                << localDataRecorderVector[i].VRIntForceGlo1[2]<< "," << " "

                << localDataRecorderVector[i].thumbContact << "," << " "//contact boolean
                << localDataRecorderVector[i].magTrackerPos1 << "," << " " //magTrackerPos vectors will need 3 headers each

                   //Thumb tracker orientation:
                << localDataRecorderVector[i].deviceRotation1(0,0) << "," << " "  //Matrix 1st row
                << localDataRecorderVector[i].deviceRotation1(0,1) << "," << " "
                << localDataRecorderVector[i].deviceRotation1(0,2) << "," << " "
                << localDataRecorderVector[i].deviceRotation1(1,0) << "," << " "  //Matrix 2nd row
                << localDataRecorderVector[i].deviceRotation1(1,1) << "," << " "
                << localDataRecorderVector[i].deviceRotation1(1,2) << "," << " "
                << localDataRecorderVector[i].deviceRotation1(2,0) << "," << " "  //Matrix 3rd row
                << localDataRecorderVector[i].deviceRotation1(2,1) << "," << " "
                << localDataRecorderVector[i].deviceRotation1(2,2) << "," << " "

                   //mapping
                << localDataRecorderVector[i].mapping << "," << " "
                   //Trial success booleans
                << localDataRecorderVector[i].trialSuccess << "," << " "

                << std::endl;
        }
        if(p_CommonData->currentDynamicObjectState == CubeGuidanceExperiment)
        {
            file <<std::setprecision(9)<< ""  //"trial = " << localDataRecorderVector[i].time << "," << " "
                   //"time = "
                << localDataRecorderVector[i].time << "," << " "

                   //"des0 = "
                << localDataRecorderVector[i].desiredStroke0 << "," << " "
                   //"real0 = "
                   //<< localDataRecorderVector[i].strokeOut0 << "," << " " //Giving motor angles N/A for FME
                   //"des1 = "
                << localDataRecorderVector[i].desiredStroke1 << "," << " "
                   //"real1 = "
                   //<< localDataRecorderVector[i].strokeOut1 << "," << " "//Giving motor angles N/A for FME

                   //Cube position
                << localDataRecorderVector[i].box1Pos << "," << " " //Each needs 3 headers

                   //Box1 -- Cube local orientation
                << localDataRecorderVector[i].box1LocalRotMat(0,0) << "," << " "  //Matrix 1st row
                << localDataRecorderVector[i].box1LocalRotMat(0,1) << "," << " "
                << localDataRecorderVector[i].box1LocalRotMat(0,2) << "," << " "
                << localDataRecorderVector[i].box1LocalRotMat(1,0) << "," << " "  //Matrix 2nd row
                << localDataRecorderVector[i].box1LocalRotMat(1,1) << "," << " "
                << localDataRecorderVector[i].box1LocalRotMat(1,2) << "," << " "
                << localDataRecorderVector[i].box1LocalRotMat(2,0) << "," << " "  //Matrix 3rd row
                << localDataRecorderVector[i].box1LocalRotMat(2,1) << "," << " "
                << localDataRecorderVector[i].box1LocalRotMat(2,2) << "," << " "

                   //Box1 --Cube Global orientation
                << localDataRecorderVector[i].box1GlobalRotMat(0,0) << "," << " "  //Matrix 1st row
                << localDataRecorderVector[i].box1GlobalRotMat(0,1) << "," << " "
                << localDataRecorderVector[i].box1GlobalRotMat(0,2) << "," << " "
                << localDataRecorderVector[i].box1GlobalRotMat(1,0) << "," << " "  //Matrix 2nd row
                << localDataRecorderVector[i].box1GlobalRotMat(1,1) << "," << " "
                << localDataRecorderVector[i].box1GlobalRotMat(1,2) << "," << " "
                << localDataRecorderVector[i].box1GlobalRotMat(2,0) << "," << " "  //Matrix 3rd row
                << localDataRecorderVector[i].box1GlobalRotMat(2,1) << "," << " "
                << localDataRecorderVector[i].box1GlobalRotMat(2,2) << "," << " "

                   //"cond = "
                   //<< localDataRecorderVector[i].conditionNo<< "," << " "

                   //INDEX:
                   // last force on tool0:
                << localDataRecorderVector[i].VRIntForce0[0]<< "," << " "
                << localDataRecorderVector[i].VRIntForce0[1]<< "," << " "
                << localDataRecorderVector[i].VRIntForce0[2]<< "," << " "
                   // last force on tool0 in global coords
                << localDataRecorderVector[i].VRIntForceGlo0[0]<< "," << " "
                << localDataRecorderVector[i].VRIntForceGlo0[1]<< "," << " "
                << localDataRecorderVector[i].VRIntForceGlo0[2]<< "," << " "

                << localDataRecorderVector[i].indexContact << "," << " " //contact boolean
                << localDataRecorderVector[i].magTrackerPos0 << "," << " " //magTrackerPos vectors will need 3 headers each

                   //Index tracker orientation:
                << localDataRecorderVector[i].deviceRotation0(0,0) << "," << " "  //Matrix 1st row
                << localDataRecorderVector[i].deviceRotation0(0,1) << "," << " "
                << localDataRecorderVector[i].deviceRotation0(0,2) << "," << " "
                << localDataRecorderVector[i].deviceRotation0(1,0) << "," << " "  //Matrix 2nd row
                << localDataRecorderVector[i].deviceRotation0(1,1) << "," << " "
                << localDataRecorderVector[i].deviceRotation0(1,2) << "," << " "
                << localDataRecorderVector[i].deviceRotation0(2,0) << "," << " "  //Matrix 3rd row
                << localDataRecorderVector[i].deviceRotation0(2,1) << "," << " "
                << localDataRecorderVector[i].deviceRotation0(2,2) << "," << " "

                   //THUMB
                   // last force on tool1:
                << localDataRecorderVector[i].VRIntForce1[0]<< "," << " "
                << localDataRecorderVector[i].VRIntForce1[1]<< "," << " "
                << localDataRecorderVector[i].VRIntForce1[2]<< "," << " "
                   //last force on tool1 in global coords
                << localDataRecorderVector[i].VRIntForceGlo1[0]<< "," << " "
                << localDataRecorderVector[i].VRIntForceGlo1[1]<< "," << " "
                << localDataRecorderVector[i].VRIntForceGlo1[2]<< "," << " "

                << localDataRecorderVector[i].thumbContact << "," << " "//contact boolean
                << localDataRecorderVector[i].magTrackerPos1 << "," << " " //magTrackerPos vectors will need 3 headers each

                   //Thumb tracker orientation:
                << localDataRecorderVector[i].deviceRotation1(0,0) << "," << " "  //Matrix 1st row
                << localDataRecorderVector[i].deviceRotation1(0,1) << "," << " "
                << localDataRecorderVector[i].deviceRotation1(0,2) << "," << " "
                << localDataRecorderVector[i].deviceRotation1(1,0) << "," << " "  //Matrix 2nd row
                << localDataRecorderVector[i].deviceRotation1(1,1) << "," << " "
                << localDataRecorderVector[i].deviceRotation1(1,2) << "," << " "
                << localDataRecorderVector[i].deviceRotation1(2,0) << "," << " "  //Matrix 3rd row
                << localDataRecorderVector[i].deviceRotation1(2,1) << "," << " "
                << localDataRecorderVector[i].deviceRotation1(2,2) << "," << " "

                   //mapping
                << localDataRecorderVector[i].mapping << "," << " "
                   //boxInitPosParam
                << localDataRecorderVector[i].boxInitParam << "," << " "
                   //Trial success booleans
                << localDataRecorderVector[i].trialSuccess << "," << " "

                << std::endl;
        }

        else if(p_CommonData->currentDynamicObjectState == CrumblyCubeExperiment)
        {
            file <<std::setprecision(9)<< ""  //"trial = " << localDataRecorderVector[i].time << "," << " "
                   //"time = "
                << localDataRecorderVector[i].time << "," << " "

                   //"des0 = "
                << localDataRecorderVector[i].desiredStroke0 << "," << " "
                   //"real0 = "
                   //<< localDataRecorderVector[i].strokeOut0 << "," << " " //Giving motor angles N/A for FME
                   //"des1 = "
                << localDataRecorderVector[i].desiredStroke1 << "," << " "
                   //"real1 = "
                   //<< localDataRecorderVector[i].strokeOut1 << "," << " "//Giving motor angles N/A for FME

                   //Cube position
                << localDataRecorderVector[i].box1Pos << "," << " " //Each needs 3 headers
                //<< localDataRecorderVector[i].box2Pos << "," << " " //Each needs 3 headers
                //<< localDataRecorderVector[i].box3Pos << "," << " " //Each needs 3 headers

                   //Box1 -- Cube local orientation
                << localDataRecorderVector[i].box1LocalRotMat(0,0) << "," << " "  //Matrix 1st row
                << localDataRecorderVector[i].box1LocalRotMat(0,1) << "," << " "
                << localDataRecorderVector[i].box1LocalRotMat(0,2) << "," << " "
                << localDataRecorderVector[i].box1LocalRotMat(1,0) << "," << " "  //Matrix 2nd row
                << localDataRecorderVector[i].box1LocalRotMat(1,1) << "," << " "
                << localDataRecorderVector[i].box1LocalRotMat(1,2) << "," << " "
                << localDataRecorderVector[i].box1LocalRotMat(2,0) << "," << " "  //Matrix 3rd row
                << localDataRecorderVector[i].box1LocalRotMat(2,1) << "," << " "
                << localDataRecorderVector[i].box1LocalRotMat(2,2) << "," << " "

                   //Box1 --Cube Global orientation
                << localDataRecorderVector[i].box1GlobalRotMat(0,0) << "," << " "  //Matrix 1st row
                << localDataRecorderVector[i].box1GlobalRotMat(0,1) << "," << " "
                << localDataRecorderVector[i].box1GlobalRotMat(0,2) << "," << " "
                << localDataRecorderVector[i].box1GlobalRotMat(1,0) << "," << " "  //Matrix 2nd row
                << localDataRecorderVector[i].box1GlobalRotMat(1,1) << "," << " "
                << localDataRecorderVector[i].box1GlobalRotMat(1,2) << "," << " "
                << localDataRecorderVector[i].box1GlobalRotMat(2,0) << "," << " "  //Matrix 3rd row
                << localDataRecorderVector[i].box1GlobalRotMat(2,1) << "," << " "
                << localDataRecorderVector[i].box1GlobalRotMat(2,2) << "," << " "
                /*
                   //Box2 --Cube local orientation
                << localDataRecorderVector[i].box2LocalRotMat(0,0) << "," << " "  //Matrix 1st row
                << localDataRecorderVector[i].box2LocalRotMat(0,1) << "," << " "
                << localDataRecorderVector[i].box2LocalRotMat(0,2) << "," << " "
                << localDataRecorderVector[i].box2LocalRotMat(1,0) << "," << " "  //Matrix 2nd row
                << localDataRecorderVector[i].box2LocalRotMat(1,1) << "," << " "
                << localDataRecorderVector[i].box2LocalRotMat(1,2) << "," << " "
                << localDataRecorderVector[i].box2LocalRotMat(2,0) << "," << " "  //Matrix 3rd row
                << localDataRecorderVector[i].box2LocalRotMat(2,1) << "," << " "
                << localDataRecorderVector[i].box2LocalRotMat(2,2) << "," << " "

                   //Box2 --Cube Global orientation
                << localDataRecorderVector[i].box2GlobalRotMat(0,0) << "," << " "  //Matrix 1st row
                << localDataRecorderVector[i].box2GlobalRotMat(0,1) << "," << " "
                << localDataRecorderVector[i].box2GlobalRotMat(0,2) << "," << " "
                << localDataRecorderVector[i].box2GlobalRotMat(1,0) << "," << " "  //Matrix 2nd row
                << localDataRecorderVector[i].box2GlobalRotMat(1,1) << "," << " "
                << localDataRecorderVector[i].box2GlobalRotMat(1,2) << "," << " "
                << localDataRecorderVector[i].box2GlobalRotMat(2,0) << "," << " "  //Matrix 3rd row
                << localDataRecorderVector[i].box2GlobalRotMat(2,1) << "," << " "
                << localDataRecorderVector[i].box2GlobalRotMat(2,2) << "," << " "

                   //Box3 --Cube local orientation
                << localDataRecorderVector[i].box3LocalRotMat(0,0) << "," << " "  //Matrix 1st row
                << localDataRecorderVector[i].box3LocalRotMat(0,1) << "," << " "
                << localDataRecorderVector[i].box3LocalRotMat(0,2) << "," << " "
                << localDataRecorderVector[i].box3LocalRotMat(1,0) << "," << " "  //Matrix 2nd row
                << localDataRecorderVector[i].box3LocalRotMat(1,1) << "," << " "
                << localDataRecorderVector[i].box3LocalRotMat(1,2) << "," << " "
                << localDataRecorderVector[i].box3LocalRotMat(2,0) << "," << " "  //Matrix 3rd row
                << localDataRecorderVector[i].box3LocalRotMat(2,1) << "," << " "
                << localDataRecorderVector[i].box3LocalRotMat(2,2) << "," << " "

                   //Box3 --Cube Global orientation
                << localDataRecorderVector[i].box3GlobalRotMat(0,0) << "," << " "  //Matrix 1st row
                << localDataRecorderVector[i].box3GlobalRotMat(0,1) << "," << " "
                << localDataRecorderVector[i].box3GlobalRotMat(0,2) << "," << " "
                << localDataRecorderVector[i].box3GlobalRotMat(1,0) << "," << " "  //Matrix 2nd row
                << localDataRecorderVector[i].box3GlobalRotMat(1,1) << "," << " "
                << localDataRecorderVector[i].box3GlobalRotMat(1,2) << "," << " "
                << localDataRecorderVector[i].box3GlobalRotMat(2,0) << "," << " "  //Matrix 3rd row
                << localDataRecorderVector[i].box3GlobalRotMat(2,1) << "," << " "
                << localDataRecorderVector[i].box3GlobalRotMat(2,2) << "," << " "
                */
                   //"cond = "
                   //<< localDataRecorderVector[i].conditionNo<< "," << " "

                   //INDEX:
                   // last force on tool0:
                << localDataRecorderVector[i].VRIntForce0[0]<< "," << " "
                << localDataRecorderVector[i].VRIntForce0[1]<< "," << " "
                << localDataRecorderVector[i].VRIntForce0[2]<< "," << " "
                   // last force on tool0 in global coords
                << localDataRecorderVector[i].VRIntForceGlo0[0]<< "," << " "
                << localDataRecorderVector[i].VRIntForceGlo0[1]<< "," << " "
                << localDataRecorderVector[i].VRIntForceGlo0[2]<< "," << " "

                << localDataRecorderVector[i].indexContact << "," << " " //contact boolean
                << localDataRecorderVector[i].magTrackerPos0 << "," << " " //magTrackerPos vectors will need 3 headers each

                   //Index tracker orientation:
                << localDataRecorderVector[i].deviceRotation0(0,0) << "," << " "  //Matrix 1st row
                << localDataRecorderVector[i].deviceRotation0(0,1) << "," << " "
                << localDataRecorderVector[i].deviceRotation0(0,2) << "," << " "
                << localDataRecorderVector[i].deviceRotation0(1,0) << "," << " "  //Matrix 2nd row
                << localDataRecorderVector[i].deviceRotation0(1,1) << "," << " "
                << localDataRecorderVector[i].deviceRotation0(1,2) << "," << " "
                << localDataRecorderVector[i].deviceRotation0(2,0) << "," << " "  //Matrix 3rd row
                << localDataRecorderVector[i].deviceRotation0(2,1) << "," << " "
                << localDataRecorderVector[i].deviceRotation0(2,2) << "," << " "

                   //THUMB
                   // last force on tool1:
                << localDataRecorderVector[i].VRIntForce1[0]<< "," << " "
                << localDataRecorderVector[i].VRIntForce1[1]<< "," << " "
                << localDataRecorderVector[i].VRIntForce1[2]<< "," << " "
                   //last force on tool1 in global coords
                << localDataRecorderVector[i].VRIntForceGlo1[0]<< "," << " "
                << localDataRecorderVector[i].VRIntForceGlo1[1]<< "," << " "
                << localDataRecorderVector[i].VRIntForceGlo1[2]<< "," << " "

                << localDataRecorderVector[i].thumbContact << "," << " "//contact boolean
                << localDataRecorderVector[i].magTrackerPos1 << "," << " " //magTrackerPos vectors will need 3 headers each

                   //Thumb tracker orientation:
                << localDataRecorderVector[i].deviceRotation1(0,0) << "," << " "  //Matrix 1st row
                << localDataRecorderVector[i].deviceRotation1(0,1) << "," << " "
                << localDataRecorderVector[i].deviceRotation1(0,2) << "," << " "
                << localDataRecorderVector[i].deviceRotation1(1,0) << "," << " "  //Matrix 2nd row
                << localDataRecorderVector[i].deviceRotation1(1,1) << "," << " "
                << localDataRecorderVector[i].deviceRotation1(1,2) << "," << " "
                << localDataRecorderVector[i].deviceRotation1(2,0) << "," << " "  //Matrix 3rd row
                << localDataRecorderVector[i].deviceRotation1(2,1) << "," << " "
                << localDataRecorderVector[i].deviceRotation1(2,2) << "," << " "

                   //ManipForceTooHigh Threshold Bool
                << localDataRecorderVector[i].manipForceTooHigh << "," << " "
                   //CCE Experiment Type
                << localDataRecorderVector[i].cceExpType << "," << " "
                   //mapping
                << localDataRecorderVector[i].mapping << "," << " "
                   //Trial success booleans
                << localDataRecorderVector[i].trialSuccess << "," << " "

                << std::endl;
        }
#endif
#ifdef ACC
        file <<std::setprecision(9)<< localDataRecorderVector[i].time << "," << " "
            << localDataRecorderVector[i].Acc << "," << " "
            << std::endl;
#endif
    }
    file.close();
    //if(i!=(localDataRecorderVector.size())||localDataRecorderVector[localDataRecorderVector.size()-1].ThroughHoop_num==0){}
    localDataRecorderVector.clear();
}

void MainWindow::on_dynamicEnvironment_clicked() // Needs to be clicked 1st
{
    qDebug()<<"Dynamic Environment Button clicked";
    //on_adjustParamsButton_clicked();
    p_CommonData->environmentChange = true;
    p_CommonData->currentDynamicObjectState = standard;
    p_CommonData->currentEnvironmentState = dynamicBodies;

    ui->VRControl->setChecked(true);
    ui->JakeRenderCheckBox->setChecked(false);
    onGUIchanged();
    qDebug()<<"Dynamic Environment Button finished";
}

//tiger cd
void MainWindow::RecordCDInertiaData()
{
    p_CommonData->recordFlag = false;
    p_CommonData->sharedMutex.lock();

    /////////////////////////////////////
    // Write data from this trial to file
    /////////////////////////////////////
    p_CommonData->dataRecordMutex.lock();
    p_CommonData->dataRecorder.subjResponse = p_CommonData->currChoice;
    p_CommonData->dataRecorder.referenceFirst = p_CommonData->referenceFirst;
    qDebug()<<"ref first ? " << p_CommonData->referenceFirst;
    p_CommonData->dataRecorderVector.push_back(p_CommonData->dataRecorder);
    localDataRecorderVector = p_CommonData->dataRecorderVector;
    p_CommonData->dataRecorderVector.clear();
    p_CommonData->dataRecordMutex.unlock();
    WriteDataToFile();

    p_CommonData->fingerDisplayScale = 1.0;
    p_CommonData->sharedMutex.unlock();
    p_CommonData->recordFlag = true;
}

void MainWindow::rotateTissueLineDisp(double angle)
{
    p_CommonData->p_indicator->rotateAboutLocalAxisDeg(0,0,-1,angle);

    p_CommonData->indicatorRot = p_CommonData->indicatorRot + angle;
}

void MainWindow::processEvents()
{
#ifdef OCULUS
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {

SDL_KEYDOWN:
        // esc
        if (event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_ESCAPE)
        {
            close();
            break;
        }

        // spacebar
        if (event.key.keysym.sym == SDLK_SPACE)
        {
            // oculusVR.recenterPose();
            break;
        }

        break;

        case SDL_QUIT:
            close();
            break;

        default:
            break;
        }
    }
#endif
}

bool MainWindow::CheckFingers()
{
    // returns true if fingers out of the way, false if they're in the way
    chai3d::cVector3d fingerPos, thumbPos;
    p_CommonData->chaiMagDevice0->getPosition(fingerPos);
    p_CommonData->chaiMagDevice1->getPosition(thumbPos);

    // it returns in meters
    bool logicFinger, logicThumb;
    logicFinger = (abs(fingerPos.x()) > 0.07) || (abs(fingerPos.y()) > 0.07);
    logicThumb = (abs(thumbPos.x()) > 0.07) || (abs(thumbPos.y()) > 0.07);
    if(logicFinger && logicThumb)
        return true;
    else
        return false;
}

//Mine Stiffness Experiment
void MainWindow::on_StiffnessExp_clicked()
{
    QString temp = QFileDialog::getOpenFileName();
    p_CommonData->MineProtocolLocation = temp;
    int error = p_CommonData->MineProtocolFile.LoadFile(temp.toStdString().c_str());
    //qDebug() << "error" << error << p_CommonData->MineProtocolLocation;

    p_CommonData->trialNo       = -1;

    p_CommonData->environmentChange         = true;
    p_CommonData->currentDynamicObjectState = StiffnessExperiment;
    p_CommonData->currentExperimentState    = idleExperiment;
    p_CommonData->currentEnvironmentState   = dynamicBodies;
    p_CommonData->recordFlag                = false;
    ui->VRControl->setChecked(true);
    ui->JakeRenderCheckBox->setChecked(true);

    //Disable all other buttons:
    ui->dynamicEnvironment->setEnabled(false);
    //ui->StiffnessExp->setEnabled(false);
    ui->StiffnMassCombined->setEnabled(false);
    ui->FingerMappingExp->setEnabled(false);
    ui->HoxelMappingExp->setEnabled(false);
    ui->CrumblyCubeExp->setEnabled(false);
    ui->CubeGuidanceExp->setEnabled(false);
    ui->Manual->setEnabled(false);

    qDebug()<<"Stiffness Button finished";
    QThread::msleep(200);
    onGUIchanged();
}

//Mine Stiffness-Mass Experiment
void MainWindow::on_StiffnMassCombined_clicked()
{
    QString temp = QFileDialog::getOpenFileName(); //click desired protocol ini file when fiole explorer opens
    p_CommonData->MineProtocolLocation = temp;
    int error = p_CommonData->MineProtocolFile.LoadFile(temp.toStdString().c_str());
    //qDebug() << "error" << error << p_CommonData->MineProtocolLocation;
    /*
    if(ui->AdjustTrialNo->isChecked())        //let haptics thread determine desired position
    {
        qDebug() << "AdjustTrial";
        p_CommonData->trialNo = p_CommonData->AdjustedTrialNo;
    }else{
        qDebug() << "DONTAdjustTrial";
        p_CommonData->trialNo       = -1;
    }
*/
    p_CommonData->environmentChange         = true;
    p_CommonData->currentDynamicObjectState = StiffnessMassExperiment;
    p_CommonData->currentExperimentState    = idleExperiment;
    p_CommonData->currentEnvironmentState   = dynamicBodies;
    p_CommonData->recordFlag                = false;
    ui->VRControl->setChecked(true);
    ui->JakeRenderCheckBox->setChecked(true);

    //Disable all other buttons:
    ui->dynamicEnvironment->setEnabled(false);
    ui->StiffnessExp->setEnabled(false);
    //ui->StiffnMassCombined->setEnabled(false);
    ui->FingerMappingExp->setEnabled(false);
    ui->HoxelMappingExp->setEnabled(false);
    ui->CrumblyCubeExp->setEnabled(false);
    ui->CubeGuidanceExp->setEnabled(false);
    ui->Manual->setEnabled(false);

    qDebug()<<"StiffMass Button finished";
    QThread::msleep(200);
    onGUIchanged();
}

//Jasmin Finger Mapping Experiment -- 1-DoF Servo Device
void MainWindow::on_FingerMappingExp_clicked()
{
    QString protocolFolder = "./FingerMappingProtocols/";
    qDebug() << protocolFolder;
    QString temp = QFileDialog::getOpenFileName(this, tr("Choose a Protocol File"), protocolFolder); //click desired protocol ini file when file explorer opens
    p_CommonData->protocolFileLocation = temp;
    int error = p_CommonData->selectedProtocolFile.LoadFile(temp.toStdString().c_str()); //DO NOT COMMENT OUT THIS LINE it will cause protocol reading to fail
    qDebug() << "error" << error << p_CommonData->protocolFileLocation;

    if(ui->AdjustTrialNo->isChecked())        //let haptics thread determine desired position
    {
        qDebug() << "AdjustTrial";
        ui->SetTrialNoButton->setEnabled(true);
        p_CommonData->trialNo = p_CommonData->AdjustedTrialNo;
        qDebug() << "New TrialNo: " << p_CommonData->trialNo;

    }
    else
    {
        qDebug() << "DONTAdjustTrial";
        ui->SetTrialNoButton->setEnabled(false);
        p_CommonData->trialNo = -1;
    }
    /**/
    p_CommonData->environmentChange         = true;
    p_CommonData->currentDynamicObjectState = FingerMappingExperiment;
    p_CommonData->currentExperimentState    = idleExperiment;
    p_CommonData->currentEnvironmentState   = dynamicBodies;
    p_CommonData->recordFlag                = false;
    ui->VRControl->setChecked(true);
    //ui->JakeRenderCheckBox->setChecked(true);
    ui->JakeRenderCheckBox->setChecked(false);
    qDebug()<<"FingerMapping Button finished";

    //**GUI Prompt****
    QString labelText = "<P><FONT COLOR='#000000' FONT SIZE = 5>";
    labelText.append("Pre-Training Stage\n");
    labelText.append("</P></br>");
    labelText.append("<P><FONT COLOR='#000000' FONT SIZE = 2>");
    labelText.append("Press the 'Next' button");
    labelText.append("</P></br>");
    ui->text->setText(labelText);
    //****************

    //Disable all other buttons:
    ui->dynamicEnvironment->setEnabled(false);
    ui->StiffnessExp->setEnabled(false);
    ui->StiffnMassCombined->setEnabled(false);
    //ui->FingerMappingExp->setEnabled(false);
    ui->HoxelMappingExp->setEnabled(false);
    ui->CrumblyCubeExp->setEnabled(false);
    ui->CubeGuidanceExp->setEnabled(false);
    ui->Manual->setEnabled(false);

    QThread::msleep(200);
    onGUIchanged();
}

//Jasmin Hoxel Mapping Experiment
void MainWindow::on_HoxelMappingExp_clicked()
{
    QString protocolFolder = "./HoxelMappingProtocols/";
    qDebug() << protocolFolder;
    QString temp = QFileDialog::getOpenFileName(this, tr("Choose a Protocol File"), protocolFolder); //click desired protocol ini file when file explorer opens
    p_CommonData->protocolFileLocation = temp;
    int error = p_CommonData->selectedProtocolFile.LoadFile(temp.toStdString().c_str()); //DO NOT COMMENT OUT THIS LINE it will cause protocol reading to fail
    qDebug() << "error" << error << p_CommonData->protocolFileLocation;
    /*
    if(ui->AdjustTrialNo->isChecked())        //let haptics thread determine desired position
    {
        qDebug() << "AdjustTrial";
        p_CommonData->trialNo = p_CommonData->AdjustedTrialNo;
    }
    else
    {
        qDebug() << "DONTAdjustTrial";
        p_CommonData->trialNo = -1;
    }
*/
    p_CommonData->environmentChange         = true;
    p_CommonData->currentDynamicObjectState = HoxelMappingExperiment;
    p_CommonData->currentExperimentState    = idleExperiment;
    p_CommonData->currentEnvironmentState   = dynamicBodies;
    p_CommonData->recordFlag                = false;
    ui->VRControl->setChecked(true);
    //ui->JakeRenderCheckBox->setChecked(true);
    ui->JakeRenderCheckBox->setChecked(false);
    qDebug()<<"HoxelMapping Button finished";

    //**GUI Prompt****
    QString labelText = "<P><FONT COLOR='#000000' FONT SIZE = 5>";
    labelText.append("Pre-Training Stage\n");
    labelText.append("</P></br>");
    labelText.append("<P><FONT COLOR='#000000' FONT SIZE = 2>");
    labelText.append("Press the 'Next' button");
    labelText.append("</P></br>");
    ui->text->setText(labelText);
    //****************

    //Disable all other buttons:
    ui->dynamicEnvironment->setEnabled(false);
    ui->StiffnessExp->setEnabled(false);
    ui->StiffnMassCombined->setEnabled(false);
    ui->FingerMappingExp->setEnabled(false);
    //ui->HoxelMappingExp->setEnabled(false);
    ui->CrumblyCubeExp->setEnabled(false);
    ui->CubeGuidanceExp->setEnabled(false);
    ui->Manual->setEnabled(false);

    QThread::msleep(200);
    onGUIchanged();
}

void MainWindow::on_SetTrialNoButton_clicked()
{
    p_CommonData->trialNo = p_CommonData->AdjustedTrialNo;
    if (readExpStuffIn())
    {
        qDebug()<<"readExpStuffIn() SUCCESS -- Adjusted Trial No Set";
    }
    qDebug() << "New TrialNo: " << p_CommonData->trialNo;
    QString text = "New TrialNo: ";
    text.append(QString::number(p_CommonData->trialNo));
    text.append("\nNew Mapping: ");
    text.append(QString::number(p_CommonData->mapping));
    ui->text->setText(text);

    //Mapping text
    QString mappingText = "<P><FONT COLOR='#0c88fb' FONT SIZE = 3>";
    mappingText.append(QString::number(p_CommonData->mapping));
    mappingText.append("</P></br>");
    ui->mappingTextBox->setText(mappingText);

}

void MainWindow::on_CrumblyCubeExp_clicked()
{
    QString protocolFolder = "./CrumblyCubeProtocols/";
    qDebug() << protocolFolder;
    QString temp = QFileDialog::getOpenFileName(this, tr("Choose a Protocol File"), protocolFolder); //click desired protocol ini file when file explorer opens
    p_CommonData->protocolFileLocation = temp;
    int error = p_CommonData->selectedProtocolFile.LoadFile(temp.toStdString().c_str()); //DO NOT COMMENT OUT THIS LINE it will cause protocol reading to fail
    qDebug() << "error" << error << p_CommonData->protocolFileLocation;
    /*
    if(ui->AdjustTrialNo->isChecked())        //let haptics thread determine desired position
    {
        qDebug() << "AdjustTrial";
        p_CommonData->trialNo = p_CommonData->AdjustedTrialNo;
    }
    else
    {
        qDebug() << "DONTAdjustTrial";
        p_CommonData->trialNo = -1;
    }
*/
    p_CommonData->environmentChange         = true;
    p_CommonData->currentDynamicObjectState = CrumblyCubeExperiment;
    p_CommonData->currentExperimentState    = idleExperiment;
    p_CommonData->currentEnvironmentState   = dynamicBodies;
    p_CommonData->recordFlag                = false;
    ui->VRControl->setChecked(true);
    //ui->JakeRenderCheckBox->setChecked(true);
    ui->JakeRenderCheckBox->setChecked(false);
    qDebug()<<"CrumblyCubeExp Button finished";

    //**GUI Prompt****
    QString labelText = "<P><FONT COLOR='#000000' FONT SIZE = 5>";
    labelText.append("Pre-Training Stage\n");
    labelText.append("</P></br>");
    labelText.append("<P><FONT COLOR='#000000' FONT SIZE = 2>");
    labelText.append("Press the 'Next' button");
    labelText.append("</P></br>");
    ui->text->setText(labelText);
    //****************

    QThread::msleep(200);
    onGUIchanged();
}

void MainWindow::on_CubeGuidanceExp_clicked()
{
    QString protocolFolder = "./CubeGuidanceProtocols/";
    qDebug() << protocolFolder;
    QString temp = QFileDialog::getOpenFileName(this, tr("Choose a Protocol File"), protocolFolder); //click desired protocol ini file when file explorer opens
    p_CommonData->protocolFileLocation = temp;
    int error = p_CommonData->selectedProtocolFile.LoadFile(temp.toStdString().c_str()); //DO NOT COMMENT OUT THIS LINE it will cause protocol reading to fail
    qDebug() << "error" << error << p_CommonData->protocolFileLocation;

    if(ui->AdjustTrialNo->isChecked())        //let haptics thread determine desired position
    {
        qDebug() << "AdjustTrial";
        ui->SetTrialNoButton->setEnabled(true);
        p_CommonData->trialNo = p_CommonData->AdjustedTrialNo;
        qDebug() << "New TrialNo: " << p_CommonData->trialNo;
    }
    else
    {
        qDebug() << "DONTAdjustTrial";
        ui->SetTrialNoButton->setEnabled(false);
        p_CommonData->trialNo = -1;
    }
    /**/
    p_CommonData->environmentChange         = true;
    p_CommonData->currentDynamicObjectState = CubeGuidanceExperiment;
    p_CommonData->currentExperimentState    = idleExperiment;
    p_CommonData->currentEnvironmentState   = dynamicBodies;
    p_CommonData->recordFlag                = false;
    ui->VRControl->setChecked(true);
    //ui->JakeRenderCheckBox->setChecked(true);
    ui->JakeRenderCheckBox->setChecked(false);
    qDebug()<<"CubeGuidance Button finished";

    //**GUI Prompt****
    QString labelText = "<P><FONT COLOR='#000000' FONT SIZE = 5>";
    labelText.append("Pre-Training Stage\n");
    labelText.append("</P></br>");
    labelText.append("<P><FONT COLOR='#000000' FONT SIZE = 2>");
    labelText.append("Press the 'Next' button");
    labelText.append("</P></br>");
    ui->text->setText(labelText);
    //****************

    //Disable all other buttons:
    ui->dynamicEnvironment->setEnabled(false);
    ui->StiffnessExp->setEnabled(false);
    ui->StiffnMassCombined->setEnabled(false);
    ui->FingerMappingExp->setEnabled(false);
    ui->HoxelMappingExp->setEnabled(false);
    ui->CrumblyCubeExp->setEnabled(false);
    //ui->CubeGuidanceExp->setEnabled(false);
    ui->Manual->setEnabled(false);

    QThread::msleep(200);
    onGUIchanged();
}

//for creating manually adjusted environment parameters during runtime
void MainWindow::on_Manual_clicked()
{
    p_CommonData->environmentChange         = true;
    p_CommonData->currentDynamicObjectState = manual;
    p_CommonData->currentEnvironmentState   = dynamicBodies;
    ui->VRControl->setChecked(true);
    ui->JakeRenderCheckBox->setChecked(false);

    //Disable all other buttons:
    ui->dynamicEnvironment->setEnabled(false);
    ui->StiffnessExp->setEnabled(false);
    ui->StiffnMassCombined->setEnabled(false);
    ui->FingerMappingExp->setEnabled(false);
    ui->HoxelMappingExp->setEnabled(false);
    ui->CrumblyCubeExp->setEnabled(false);
    ui->CubeGuidanceExp->setEnabled(false);
    //ui->Manual->setEnabled(false);

    onGUIchanged();
    qDebug()<<"Manual Button finished";
}

void MainWindow::on_resetCameraButton_clicked()
{
    ui->cameraPolar_Box->setValue(initPolar);
    ui->cameraRadius_Box->setValue(initCamRadius);
    ui->cameraAzimuth_Box->setValue(initAzimuth);
    ui->cameraXPosBox->setValue(initCamPosX);
    ui->cameraYPosBox->setValue(initCamPosY);
    ui->cameraZPosBox->setValue(initCamPosZ);

}
