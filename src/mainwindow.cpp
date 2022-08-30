#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string.h>
#include "Widget_OpenGLDisplay.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iomanip>
#include <string>

//***Define directory/folder for Subjects' experiment info***
//Make sure this directory exists before runtime or else the data will not save
//QString subjectDirectory = "C:/Users/Sam/Desktop/chai3dFingerMapping/Subjects/";
//QString subjectDirectory = "F:/FME_Subjects/";
QString subjectDirectory = "./FME_Subject_Data/";

//Gloabal value to be shared with cMotorController
int mappingVal;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
#ifndef OCULUS
    windowGLDisplay = new Widget_OpenGLDisplay(this->centralWidget());
    windowGLDisplay->setObjectName(QStringLiteral("windowGLDisplay"));
    //windowGLDisplay->setGeometry(QRect(10, 10, 1275, 1000));
    //windowGLDisplay->setGeometry(QRect(10, 10, 1275, 1500));
    //windowGLDisplay->setGeometry(QRect(10, 10, 701, 471));
    windowGLDisplay->setGeometry(QRect(10, 10, 1051, 706));
#endif
}

MainWindow::~MainWindow()
{
    p_CommonData->hapticsThreadActive = false;
    p_CommonData->VibroTactileThreadActive = false;
    delete ui;
}

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
    
    
#ifndef OCULUS
    windowGLDisplay->p_CommonData = p_CommonData;
#endif

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

    p_CommonData->stiffness1 = 300;
    p_CommonData->stiffness2 = 100;

    p_CommonData->mass1 = 0.3;
    p_CommonData->mass2 = 0.3;
    p_CommonData->mass3 = 0.05;
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

    if(p_CommonData->flagEqualTouch){
        if (p_CommonData->fingerTouching == true && p_CommonData->thumbTouching == true){
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
    points1 << QPointF( i , p_CommonData->deviceComputedForce.z());
    if (points1.length() > 5*updateHz)
        points1.removeFirst();

    // assign values to second curve
    points2 << QPointF( i , p_CommonData->filteredDeviceComputedForce.z());
    if (points2.length() > 5*updateHz)
        points2.removeFirst();

#endif

    // index device
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

    // thumb device
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

    ui->mass1_show->display(p_CommonData->mass1);
    ui->mass2_show->display(p_CommonData->mass2);

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

    //calibrate if startup process over
    if(p_CommonData->calibClock.timeoutOccurred())
    {
        p_CommonData->calibClock.stop();
        p_CommonData->calibClock.reset();
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

//Reads in experiment data from protocol ini files
bool MainWindow::readExpStuffIn(){
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
    //For Jasmin's FingerMapping Experiment
    else if(p_CommonData->currentDynamicObjectState == FingerMappingExperiment)
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
    else if(p_CommonData->currentDynamicObjectState == FingerMappingExperiment)
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
}

//Mataches text to mapping type to be displayed for FingerMapping Experiment
QString getMappingText(int mappingVal)
{
    QString textVal;

    if(mappingVal == 1)
    {
        //textVal = "Normal";
        textVal = "A";
    }
    else if(mappingVal == 2)
    {
        //textVal = "Reversed";
        textVal = "B";
    }
    else if(mappingVal == 3)
    {
        //textVal = "Single";
        textVal = "C";
    }
    else if(mappingVal == 4)
    {
        //textVal = "Averaged";
        textVal = "D";
    }
    else if(mappingVal == 5)
    {
        //textVal = "Control";
        textVal = "E";
    }
    else
    {
        textVal = "FAILED MAPPING";
    }
    return textVal;
}

void MainWindow::keyPressEvent(QKeyEvent *a_event)
{
    if (a_event->key() == Qt::Key_Y)
    {
        p_CommonData->show_forces = !p_CommonData->show_forces;
    }
    if (a_event->key() == Qt::Key_F)
    {
        p_CommonData->showCursorFrames = !p_CommonData->showCursorFrames;
    }
    if (a_event->key() == Qt::Key_I)
    {
        p_CommonData->camRadius = p_CommonData->camRadius - 0.05;
    }
    if (a_event->key() == Qt::Key_O)
    {
        p_CommonData->camRadius = p_CommonData->camRadius + 0.05;
    }

    /***Vt Experiment Buttons***/
    //progressing to next trial
    if(a_event->key() == Qt::Key_H)
    {
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
                    p_CommonData->recordFlag==true;
                }

                else {
                    if(p_CommonData->recordFlag==true){
                        p_CommonData->dataRecordMutex.lock();
                        localDataRecorderVector = p_CommonData->dataRecorderVector;
                        p_CommonData->dataRecorderVector.clear();
                        p_CommonData->dataRecordMutex.unlock();
                        WriteDataToFile();
                        p_CommonData->recordFlag=false;
                    }

                    if(p_CommonData->TrialType=="training" || p_CommonData->TrialType=="testing")
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

                if(p_CommonData->recordFlag==true){
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
            bool mistake = false;
            p_CommonData->flagMassExp = true;

            qDebug("advance StiffnessMassExperiment");
            if(CheckFingers()&& (p_CommonData->fingerTouching == false && p_CommonData->thumbTouching == false))
            {
                if(p_CommonData->trialNo<1)
                {
                    p_CommonData->trialNo = 1;
                    p_CommonData->recordFlag==true;

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
                    if(p_CommonData->recordFlag){
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

        else if (p_CommonData->currentDynamicObjectState == FingerMappingExperiment)
        {
            bool mistake = false;
            p_CommonData->flagMassExp = false;

            qDebug("advance FingerMappingExp");
            if(CheckFingers()&& (p_CommonData->fingerTouching == false && p_CommonData->thumbTouching == false))
            {
                //PRE-TRIAL******
                if(p_CommonData->trialNo < 1)
                {
                    qDebug()<<"Trial# < 1";
                    p_CommonData->trialNo = 1;
                    p_CommonData->recordFlag == true;

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

                    qDebug()<<"Recorded Pre-Trial#"<<p_CommonData->trialNo<<"  Type "<< p_CommonData->TrialType;
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
                    if(p_CommonData->TrialType=="training" || p_CommonData->TrialType=="testing")
                    {
                        //If cube passed hoop and target, advance trial
                        if(p_CommonData->target1Complete && p_CommonData->hoop1Complete)
                        {
                            //ADVANCE to next trial
                            p_CommonData->trialNo++;
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
                            qDebug()<<"Recorded Trial#"<<p_CommonData->trialNo<<"  Type "<< p_CommonData->TrialType;
                            p_CommonData->environmentChange = true;


                            //p_CommonData->hoopSuccess = 1;
                            //p_CommonData->targetSuccess = 1;
                            //p_CommonData->trialSuccess = 1;
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
                                //qDebug()<<"___"<<p_CommonData->trialNo;

                                //p_CommonData->targetSuccess = 1;

                            }
                            //If object is not in the final area
                            else if(!p_CommonData->target1Complete)
                            {
                                //If the object has gone through the hoop but not target
                                if(p_CommonData->hoop1Complete)
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
                                    //qDebug()<<"___"<<p_CommonData->trialNo;

                                    //p_CommonData->hoopSuccess = 1;
                                }
                                //If the object hit neither the target or hoop
                                else if(!p_CommonData->hoop1Complete)
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
                                    //qDebug()<<"___"<<p_CommonData->trialNo;

                                    //p_CommonData->hoopSuccess = 0;
                                }

                                //p_CommonData->targetSuccess = 0;
                            }

                           //p_CommonData->trialSuccess = 0;
                        }
                    }

                    //if not in training or testing trials
                    //This area is called when coming back from break
                    else
                    {
                        p_CommonData->trialNo++;
                        //GUI Stuff
                        if (readExpStuffIn())
                        {
                            qDebug()<<"successful read -- back from break";
                        }
                        if (p_CommonData->TrialType == "training")
                        {
                            QString labelText = "<P><FONT COLOR='#000000' FONT SIZE = 5>";
                            labelText.append("Training -- back from break");
                            labelText.append("</P></br>");
                            ui->text->setText(labelText);
                            //qDebug()<<"___"<<p_CommonData->trialNo;
                        }

                        else if (p_CommonData->TrialType == "testing"){
                            QString labelText = "<P><FONT COLOR='#000000' FONT SIZE = 5>";
                            labelText.append("Testing -- back from break");
                            labelText.append("</P></br>");
                            ui->text->setText(labelText);
                            //qDebug()<<"___"<<p_CommonData->trialNo;
                        }

                        else if (p_CommonData->TrialType == "break"){
                            QString labelText = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                            labelText.append("PRESS NEXT AFTER THE BREAK -- back from break");
                            labelText.append("</b></P></br>");
                            ui->text->setText(labelText);
                        }

                        else if (p_CommonData->TrialType == "breakbreak"){
                            QString labelText = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                            labelText.append("TIME TO CHANGE THE GROUNDING -- back from break");
                            labelText.append("</b></P></br>");
                            ui->text->setText(labelText);
                           // qDebug()<<"___"<<p_CommonData->trialNo;
                        }

                        else if (p_CommonData->TrialType == "end"){
                            QString labelText = "<P><b><FONT COLOR='#ff0000' FONT SIZE = 10>";
                            labelText.append("END OF THE EXPERIMENT -- back from break");
                            labelText.append("</b></P></br>");
                            ui->text->setText(labelText);
                           //qDebug()<<"___"<<p_CommonData->trialNo;
                        }
                        qDebug()<<"Recorded Trial#"<<p_CommonData->trialNo<<"  Type "<< p_CommonData->TrialType;
                        p_CommonData->environmentChange = true;
                    }
                }
            }

            //Set Mapping Text
            if (p_CommonData->currentDynamicObjectState == FingerMappingExperiment)
            {
                mappingVal = p_CommonData->mapping;
                QString mappingText = "<P><FONT COLOR='#0c88fb' FONT SIZE = 3>";
                //mappingText.append("Mapping " + QString::number(p_CommonData->mapping) +":\n");
                //mappingText.append("</P></br>");
                //mappingText.append("<P><FONT COLOR='#0c88fb' FONT SIZE = 3>");
                mappingText.append(getMappingText(p_CommonData->mapping));
                mappingText.append("</P></br>");
                ui->mappingTextBox->setText(mappingText);
            }

        }
    }

    if(a_event->key() == Qt::Key_Y)
    {
        UpdateGUIInfo();
    }
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

    double degInc = 5.0;
    double radInc = 0.05;
    if (a_event->key() == Qt::Key_W)
    {
        p_CommonData->polar = p_CommonData->polar + degInc;
        qDebug() << "pol" << p_CommonData->polar;
    }
    if (a_event->key() == Qt::Key_S)
    {
        p_CommonData->polar = p_CommonData->polar - degInc;
        qDebug() << "pol" << p_CommonData->polar;
    }
    if (a_event->key() == Qt::Key_A)
    {
        p_CommonData->azimuth = p_CommonData->azimuth + degInc;
        qDebug() << "azim" << p_CommonData->azimuth;
    }
    if (a_event->key() == Qt::Key_D)
    {
        p_CommonData->azimuth = p_CommonData->azimuth - degInc;
        qDebug() << "azim" << p_CommonData->azimuth;
    }
    if (a_event->key() == Qt::Key_C)
    {
        p_CommonData->camRadius = p_CommonData->camRadius - radInc;
    }
    if (a_event->key() == Qt::Key_V)
    {
        p_CommonData->camRadius = p_CommonData->camRadius + radInc;
    }
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
    }
    //assuming this is switching between states without progressing trial


    double angle = 10.0;
    if (a_event->key() == Qt::Key_Z)
    {
        rotateTissueLineDisp(-angle);
        p_CommonData->p_indicator->setTransparencyLevel(1, true);
    }
    if (a_event->key() == Qt::Key_X)
    {
        rotateTissueLineDisp(angle);
        p_CommonData->p_indicator->setTransparencyLevel(1, true);
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

    QString directory = subjectDirectory + QString::number(p_CommonData->subjectNo);

    //Create folder for data
    if(p_CommonData->currentDynamicObjectState == StiffnessExperiment){
        directory = subjectDirectory + "StiffnessExperiment" + QString::number(p_CommonData->subjectNo);
    }
    if(p_CommonData->currentDynamicObjectState == StiffnessMassExperiment){
        directory = subjectDirectory + "StiffnessMassExperiment" + QString::number(p_CommonData->subjectNo);
    }
    else if(p_CommonData->currentDynamicObjectState == FingerMappingExperiment){
        if(p_CommonData->TrialType == "training")
        {
            directory = subjectDirectory + "TrainingTrialData";
            qDebug()<<"Saved data to: "<<directory;
        }
        else
        {
            directory = subjectDirectory;
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
    else if (p_CommonData->currentDynamicObjectState == FingerMappingExperiment)
    {
        //Sort data by trialName
        QString trialNum = QString::number(p_CommonData->trialNo);

        //std::ofstream file;

        //Formatting to help with post-processing
        if (p_CommonData->trialNo < 10)
        {
            //Open file to write data:
            file.open(directory.toStdString()
                      +"/Subject" + QString::number(p_CommonData->subjectNo).toStdString()
                      + "_Trial0" + trialNum.toStdString()
                      + "_Mapping" + QString::number(p_CommonData->mapping).toStdString()
                      + ".csv");
        }
        else
        {
        //Open file to write data:
        file.open(directory.toStdString()
                  +"/Subject" + QString::number(p_CommonData->subjectNo).toStdString()
                  + "_Trial" + trialNum.toStdString()
                  + "_Mapping" + QString::number(p_CommonData->mapping).toStdString()
                  + ".csv");
        }
    }

    //write data to file when we are done

#ifdef ACC
    p_CommonData->dir="C:/Users/Sam/Desktop/Subjects/VTExp/Subjects/VTACC";
    p_CommonData->fileName = "VTACC_CAL";
#endif


    //Create Headers for files depending on experiment
    if(p_CommonData->currentDynamicObjectState == StiffnessExperiment ||
            p_CommonData->currentDynamicObjectState == StiffnessMassExperiment)
    {
        //none for now
    }
    else if(p_CommonData->currentDynamicObjectState == FingerMappingExperiment)
    {
        //These *MUST* match the order of variables saved below:
        file <<std::setprecision(9)
            << "time" << "," << " " //time in seconds

            << "realDorsalTactorPos" << "," << " "//in mm

            << "realVentralTactorPos" << "," << " " //in mm

            //boxPos is a vector and will need 3 headers
            << "boxPosX" << "," << " " //in cm? - unsure
            << "boxPosY" << "," << " " //in cm? - unsure
            << "boxPosZ" << "," << " " //in cm? - unsure

            //magTrackerPos vectors will need 3 headers each
            << "indexPosX" << "," << " " //in cm? - unsure
            << "indexPosY" << "," << " " //in cm? - unsure
            << "indexPosZ" << "," << " " //in cm? - unsure
            << "indexContact" << "," << " " //bool
            << "indexForceX" << "," << " " //in N? - unsure
            << "indexForceY" << "," << " " //in N? - unsure
            << "indexForceZ" << "," << " " //in N? - unsure
            << "indexForceGlobalX" << "," << " " //in N? - unsure
            << "indexForceGlobalY" << "," << " " //in N? - unsure
            << "indexForceGlobalZ" << "," << " " //in N? - unsure

            //magTrackerPos vectors will need 3 headers each
            << "thumbPosX" << "," << " " //in cm? - unsure
            << "thumbPosY" << "," << " " //in cm? - unsure
            << "thumbPosZ" << "," << " " //in cm? - unsure
            << "thumbContact" << "," << " " //bool
            << "thumbForceX" << "," << " " //in N? - unsure
            << "thumbForceY" << "," << " " //in N? - unsure
            << "thumbForceZ" << "," << " " //in N? - unsure
            << "thumbForceGlobalX" << "," << " " //in N? - unsure
            << "thumbForceGlobalY" << "," << " " //in N? - unsure
            << "thumbForceGlobalZ" << "," << " " //in N? - unsure

            << "hoopSuccess" << "," << " " //bool
            << "targetSuccess" << "," << " " //bool
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
                << localDataRecorderVector[i].index_contact << "," << " "
                << localDataRecorderVector[i].thumb_contact << "," << " "

                << std::endl;
        }
        else if(p_CommonData->currentDynamicObjectState == FingerMappingExperiment)
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
                << localDataRecorderVector[i].index_contact << "," << " " //contact boolean
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
                << localDataRecorderVector[i].thumb_contact << "," << " "//contact boolean
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

    if(ui->AdjustTrialNo->isChecked())        //let haptics thread determine desired position
    {
        qDebug() << "AdjustTrial";
        p_CommonData->trialNo = p_CommonData->AdjustedTrialNo;
    }else{
        qDebug() << "DONTAdjustTrial";
        p_CommonData->trialNo       = -1;
    }

    p_CommonData->environmentChange         = true;
    p_CommonData->currentDynamicObjectState = StiffnessMassExperiment;
    p_CommonData->currentExperimentState    = idleExperiment;
    p_CommonData->currentEnvironmentState   = dynamicBodies;
    p_CommonData->recordFlag                = false;
    ui->VRControl->setChecked(true);
    ui->JakeRenderCheckBox->setChecked(true);
    qDebug()<<"StiffMass Button finished";
    QThread::msleep(200);
    onGUIchanged();
}

//Jasmin Finger Mapping Experiment
void MainWindow::on_FingerMappingExp_clicked()
{
    QString protocolFolder = "./FingerMappingProtocols/";

    QString temp = QFileDialog::getOpenFileName(this, tr("Choose a Protocol File"), protocolFolder); //click desired protocol ini file when file explorer opens
    //QString temp = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
    //                                                 "C:/Users/Sam/Desktop/chai3dFingerMapping/FingerMappingProtocols/");//click desired protocol ini file when file explorer opens
    //"C:/Users/Sam/Desktop/chai3dFingerMapping/FingerMappingProtocols/"
    p_CommonData->protocolFileLocation = temp;
    int error = p_CommonData->selectedProtocolFile.LoadFile(temp.toStdString().c_str()); //DO NOT COMMENT OUT THIS LINE it will cause protocol reading to fail
    //qDebug() << "error" << error << p_CommonData->protocolFileLocation;

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

    p_CommonData->environmentChange         = true;
    p_CommonData->currentDynamicObjectState = FingerMappingExperiment;
    p_CommonData->currentExperimentState    = idleExperiment;
    p_CommonData->currentEnvironmentState   = dynamicBodies;
    p_CommonData->recordFlag                = false;
    ui->VRControl->setChecked(true);
    ui->JakeRenderCheckBox->setChecked(true);
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
    onGUIchanged();
    qDebug()<<"Manual Button finished";
}
