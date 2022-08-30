#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string.h>
#include "Widget_OpenGLDisplay.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iomanip>
#include <string>

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
//    windowGLDisplay->setGeometry(QRect(10, 10, 701, 471));
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

//    connect(this->ui->CDScale, SIGNAL(valueChanged(int)), this, SLOT(onGUIchanged()));
//    connect(this->ui->strokeScale, SIGNAL(valueChanged(int)), this, SLOT(onGUIchanged()));
    connect(this->ui->Act1, SIGNAL(valueChanged(int)), this, SLOT(onGUIchanged()));
    connect(this->ui->Act2, SIGNAL(valueChanged(int)), this, SLOT(onGUIchanged()));

    connect(this->ui->equalBoxTouch, SIGNAL(stateChanged(int)), this, SLOT(onGUIchanged()));

    connect(this->ui->Condition1,  SIGNAL(clicked()), this, SLOT(onGUIchanged()));
//    connect(this->ui->Condition2,  SIGNAL(clicked()), this, SLOT(onGUIchanged()));
//    connect(this->ui->Condition3,  SIGNAL(clicked()), this, SLOT(onGUIchanged()));

    connect(this->ui->VRControl, SIGNAL(clicked()), this, SLOT(onGUIchanged()));


    this->Inertia_Scaling_Mass=.2;
    this->Inertia_Scaling_Factor=1;

    p_CommonData->indicatorRot = 0;
    p_CommonData->tissueRot = 0;
    p_CommonData->stiffness = 200;

    p_CommonData->adjustedDynamicForceReduction = 1.0;
    p_CommonData->adjustedForceToPosMult_Normal = 2.0; // this means 0.5 N/mm
    p_CommonData->adjustedForceToPosMult_Shear  = 5.0; // this means 0.2 N/mm

     p_CommonData->stiffness1 = 300;
     p_CommonData->stiffness2 = 100;

     p_CommonData->mass1 = 0.2;
     p_CommonData->mass2 = 0.1;
     p_CommonData->mass3 = 0.05;
     p_CommonData->cond = 1;
     p_CommonData->direct = 1;

    ui->equalBoxTouch->setChecked(true);

//    p_CommonData->flagNormal = true;

//    p_CommonData->flagLateral = true;
    p_CommonData->flagEqual = false;
    p_CommonData->flagEqualTouch = true;
    p_CommonData->flagReverse = false;
//    p_CommonData->flagDorsal = true;
//    p_CommonData->flagVentral = true;

    ui->Condition1->setChecked(true);

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
//    qDebug()<<"m i here";

    double localFor0_0     =  localForce0[0];
    double localFor0_1     =  localForce0[1];
    double localFor0_2     =  localForce0[2];

//    qDebug()<<"m i here2";


//    int localStiffn0_0     =  (int) abs(localFor0_0*10/ localDesiredPos0[0]);
//    int localStiffn0_1     =  (int) abs(localFor0_1*10/ localDesiredPos0[1]);
//    int localStiffn0_2     =  (int) abs(localFor0_2*10/ localDesiredPos0[2]);


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
//    ui->qwtPlot->setAxisAutoScale(QwtPlot::yLeft);
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

//    ui->StiX0->display(localStiffn0_0);
//    ui->StiY0->display(localStiffn0_1);
//    ui->StiZ0->display(localStiffn0_2);

    // thumb device

    ui->StrokeLCD1_1->display((localOutputStrokes1));
    ui->DesX1->display(localDesiredPos1[0]);
    ui->DesY1->display(localDesiredPos1[1]);
    ui->DesZ1->display(localDesiredPos1[2]);
    ui->MotorLCD_1->display((localMotorAngles1));
    ui->ForX1->display(localFor1_0);
    ui->ForY1->display(localFor1_1);
    ui->ForZ1->display(localFor1_2);

//    ui->strokeScale_show->display(p_CommonData->strokeScale);
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
    ui->pairNo->display(p_CommonData->pairNo);
    ui->Mass_Val->display(p_CommonData->expMass);
    ui->CD_Val->display(p_CommonData->expCD);
    ui->Inertia_Val->display(p_CommonData->expInertia);
//    ui->day_tracker->display(p_CommonData->day);

    // Change to jake render if checked
    if(ui->JakeRenderCheckBox->isChecked())
        p_CommonData->jakeRender = true;
    else
        p_CommonData->jakeRender = false;

    p_CommonData->cdVertScaleOn = ui->CD_Vertical_Box->isChecked();
//    p_CommonData->ImpulseAmp=ui->ImpulseAmp->value();
    p_CommonData->subjectNo = ui->subject_no->value();
//    p_CommonData->SDVibAmp=ui->SDVibAmp->value();
//    p_CommonData->SDVibFreq=ui->SDVibFreq->value();
//    p_CommonData->SDVibBeta=ui->SDVibBeta->value();

    //calibrate if startup process over
    if(p_CommonData->calibClock.timeoutOccurred())
    {
        p_CommonData->calibClock.stop();
        p_CommonData->calibClock.reset();
    }
}
void MainWindow::onGUIchanged()
{

    if(p_CommonData->cond == 1)
    {
//        p_CommonData->flagNormal = true;
//        p_CommonData->flagLateral = false;

        p_CommonData->flagEqual = false;
        p_CommonData->flagReverse = false;

//        p_CommonData->flagDorsal = true;
//        p_CommonData->flagVentral = true;

//        ui->Condition1->setChecked(true);
//        p_CommonData->cond = 1;
         qDebug()<<"cond 1";
    }

    else if(p_CommonData->cond == 2)
    {
//        p_CommonData->flagNormal = true;
//        p_CommonData->flagLateral = false;

        p_CommonData->flagEqual = false;
        p_CommonData->flagReverse = true;

//        p_CommonData->flagDorsal = true;
//        p_CommonData->flagVentral = true;

//        ui->Condition2->setChecked(true);
//        p_CommonData->cond = 2;
                 qDebug()<<"cond 2";
    }

    else if(p_CommonData->cond == 3)
    {
//        p_CommonData->flagNormal = true;
//        p_CommonData->flagLateral = false;

        p_CommonData->flagEqual = true;
        p_CommonData->flagReverse = false;

//        p_CommonData->flagDorsal = true;
//        p_CommonData->flagVentral = true;

//        ui->Condition3->setChecked(true);
//        p_CommonData->cond = 3;
                 qDebug()<<"cond 3";
    }



//        ui->normalBox->setChecked(p_CommonData->flagNormal);
//        ui->lateralBox->setChecked(p_CommonData->flagLateral);

        ui->equalBox->setChecked(p_CommonData->flagEqual);
        ui->reverseBox->setChecked(p_CommonData->flagReverse);

//        ui->dorsalBox->setChecked(p_CommonData->flagDorsal);
//        ui->ventralBox->setChecked(p_CommonData->flagVentral);


//    if(ui->equalBoxTouch->isChecked())
//    {
//        p_CommonData->flagEqualTouch = true;
//    }
//    else
//    {
//        p_CommonData->flagEqualTouch = false;
//    }

    if(ui->VRControl->isChecked())
    {
        //let haptics thread determine desired position
        p_CommonData->currentControlState = VRControlMode;
    }

    // set the display scale and the weight of the box
//    p_CommonData->strokeScale= (ui->strokeScale->value()*.1);

    p_CommonData->ref1 = (ui->Act1->value());
    p_CommonData->ref2 = (ui->Act2->value());

//    qDebug()<<"m i here";

    UpdateGUIInfo();

}

void MainWindow::on_turnOff_clicked()
{
    p_CommonData->currentControlState = idleControl;
//    ui->sliderControl->setAutoExclusive(false);
    ui->VRControl->setAutoExclusive(false);

//    ui->sliderControl->setChecked(false);
    ui->VRControl->setChecked(false);

//    ui->sliderControl->setAutoExclusive(true);
    ui->VRControl->setAutoExclusive(true);
}

bool MainWindow::readExpStuffIn(){
    int trial;
    qDebug()<<"good";

    trial = p_CommonData->trialNo;

    qDebug()<<"Trial: "<<p_CommonData->trialNo; //<<trial;
    p_CommonData->TrialType = p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(trial)).toStdString().c_str(), "type", NULL /*default*/);
    qDebug()<<"sad"<< p_CommonData->TrialType;

    p_CommonData->compCD = 1;
//    ui->CD_Vertical_Box->setChecked(true);
    p_CommonData->compInertia = 0.5;

    if(p_CommonData->currentDynamicObjectState == StiffnessExperiment)
    {
//        qDebug() << "am i really HERE?";
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

//            qDebug() << "train" << "st1" << p_CommonData->stiffness1 << "st2" << p_CommonData->stiffness2 << "cond" << p_CommonData->cond;

        onGUIchanged();
    }
    else if (p_CommonData->TrialType=="testing"){
            p_CommonData->currentExperimentState = idleExperiment;
            p_CommonData->cond = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "condition", NULL /*default*/));
            p_CommonData->stiffness1 = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "stiffness1", NULL /*default*/));
            p_CommonData->stiffness2 = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "stiffness2", NULL /*default*/));
            p_CommonData->direct = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "dir", NULL /*default*/));
            qDebug() << "test" << "st1" << p_CommonData->stiffness1 << "st2" << p_CommonData->stiffness2 << "cond" << p_CommonData->cond;
            onGUIchanged();
    }

    else if (p_CommonData->TrialType=="break"){
            p_CommonData->currentExperimentState = trialBreak;
//            qDebug()<<"PAUSE NOW";
    }
    else if (p_CommonData->TrialType=="breakbreak"){
            p_CommonData->currentExperimentState = trialBreak;
//            qDebug()<<"PAUSE NOW";
    }
    else if (p_CommonData->TrialType=="end"){
            p_CommonData->currentExperimentState = endExperiment;
//            qDebug()<<"DONE!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
            return false;
    }
    }

    else if(p_CommonData->currentDynamicObjectState == StiffnessMassExperiment)
    {
        qDebug() << "am i really HERE?";
    if (p_CommonData->TrialType=="training"){
            if (p_CommonData->trialNo > 0){
                p_CommonData->currentExperimentState = idleExperiment;
            }
            else{
                p_CommonData->currentExperimentState = trialBreak;
                p_CommonData->recordFlag = false;
            }

            qDebug() << "am i also HERE?";

            p_CommonData->cond          = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "condition", NULL /*default*/));
            p_CommonData->stiffness1    = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "stiffness1", NULL /*default*/));
            p_CommonData->stiffness2    = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "stiffness2", NULL /*default*/));
            p_CommonData->mass1         = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mass1", NULL /*default*/));
            p_CommonData->mass2         = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mass2", NULL /*default*/));
            p_CommonData->direct        = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "dir", NULL /*default*/));

//            qDebug() << "train" << "st1" << p_CommonData->stiffness1 << "st2" << p_CommonData->stiffness2 << "cond" << p_CommonData->cond;

            onGUIchanged();
    }
    else if (p_CommonData->TrialType=="testing"){
            p_CommonData->currentExperimentState = idleExperiment;
            p_CommonData->cond          = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "condition", NULL /*default*/));
            p_CommonData->stiffness1    = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "stiffness1", NULL /*default*/));
            p_CommonData->stiffness2    = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "stiffness2", NULL /*default*/));
            p_CommonData->mass1         = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mass1", NULL /*default*/));
            p_CommonData->mass2         = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "mass2", NULL /*default*/));
            p_CommonData->direct        = std::stod(p_CommonData->MineProtocolFile.GetValue((QString("trial ") + QString::number(p_CommonData->trialNo)).toStdString().c_str(), "dir", NULL /*default*/));

//            qDebug() << "test" << "st1" << p_CommonData->stiffness1 << "st2" << p_CommonData->stiffness2 << "cond" << p_CommonData->cond;

            onGUIchanged();
    }

    else if (p_CommonData->TrialType=="break"){
            p_CommonData->currentExperimentState = trialBreak;
//            qDebug()<<"PAUSE NOW";
    }
    else if (p_CommonData->TrialType=="breakbreak"){
            p_CommonData->currentExperimentState = trialBreak;
//            qDebug()<<"PAUSE NOW";
    }
    else if (p_CommonData->TrialType=="end"){
            p_CommonData->currentExperimentState = endExperiment;
//            qDebug()<<"DONE!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
            return false;
    }
    }
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

    if(a_event->key() == Qt::Key_I)
    {
        p_CommonData->camRadius = p_CommonData->camRadius - 0.05;
    }
    if(a_event->key() == Qt::Key_O)
    {
        p_CommonData->camRadius = p_CommonData->camRadius + 0.05;
    }

    //Vt Experiment Buttons

    if(a_event->key() == Qt::Key_H) //progressing to next pair
    {

        qDebug()<<"start trying";

        qDebug()<<p_CommonData->pairNo;

        if (p_CommonData->currentDynamicObjectState == StiffnessExperiment)
        {

            qDebug("pleaseadvance");
            if(CheckFingers()&& (p_CommonData->fingerTouching == false && p_CommonData->thumbTouching == false))
            {
                p_CommonData->pairNo = 1;

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
                        if(p_CommonData->zone1Responded == true)
                        {
                            p_CommonData->trialNo++;
                        }
                    }
                    else
                            p_CommonData->trialNo++;


                    qDebug("recorded");
                }

                if (readExpStuffIn()){
                    qDebug()<<"successful read" << p_CommonData->compCD;
                }

                    p_CommonData->expCD = 1;
                    p_CommonData->tempCD = p_CommonData->compCD;
                    p_CommonData->diffStiffSubjective = p_CommonData->diffStiffSubjectiveRead;
                    p_CommonData->diffFrictSubjective = p_CommonData->diffFrictSubjectiveRead;
                    qDebug()<<"changeh?";
                p_CommonData->isRef = !p_CommonData->isRef;
                p_CommonData->environmentChange = true;
                QThread::msleep(200);
//            }

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

            qDebug("pleaseadvance");
            if(CheckFingers()&& (p_CommonData->fingerTouching == false && p_CommonData->thumbTouching == false))
            {
                p_CommonData->pairNo = 1;

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
                        if(p_CommonData->answerComplete == true)
                        {
                            p_CommonData->trialNo++;
                        }
                    }
                    else
                            p_CommonData->trialNo++;


                    qDebug("recorded");
                }

                if (readExpStuffIn()){
                    qDebug()<<"successful read" << p_CommonData->compCD;
                }

                    p_CommonData->expCD = 1;
                    p_CommonData->tempCD = p_CommonData->compCD;
                    p_CommonData->diffStiffSubjective = p_CommonData->diffStiffSubjectiveRead;
                    p_CommonData->diffFrictSubjective = p_CommonData->diffFrictSubjectiveRead;
                    qDebug()<<"changeh?";
                p_CommonData->isRef = !p_CommonData->isRef;
                p_CommonData->environmentChange = true;
                QThread::msleep(200);
//            }

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


    }

    if(a_event->key() == Qt::Key_Y)
    {
          UpdateGUIInfo();
    }

    if(a_event->key() == Qt::Key_J) // choosing box1
    {
        if(p_CommonData->explorationComplete){
            // this means the user thinks box 1 is heavier or stiffer
            p_CommonData->answer1 = true;
            }
//        else{
//            // this means the user pressed by accident, update the task from the beginning
//        }
    }

    if(a_event->key() == Qt::Key_K) // choosing box2
    {
        if(p_CommonData->explorationComplete){
            p_CommonData->answer2 = true;
        }
//        else{
//            // this means the user pressed by accident, update the task from the beginning
//        }

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
        if(p_CommonData->recordFlag==true){
            p_CommonData->dataRecordMutex.lock();
            localDataRecorderVector = p_CommonData->dataRecorderVector;
            p_CommonData->dataRecorderVector.clear();
            p_CommonData->dataRecordMutex.unlock();
            WriteDataToFile();
        p_CommonData->recordFlag=false;
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

void MainWindow::WriteDataToFile()
{
//    qDebug()<<"writing";
    p_CommonData->recordFlag = false;

    char trialBuffer[33];
    char cdBuffer[33];
    char inertiaBuffer[33];

    double file_number;
//    if(!p_CommonData->currentDynamicObjectState == dynamicMagnitudeExp)
//    {
//        file_number= p_CommonData->trialNo % 51;
//    }
//    else
//    {
        file_number = p_CommonData->trialNo;
//}

    itoa(file_number,trialBuffer,10);
    itoa((int)(p_CommonData->compCD*10),cdBuffer,10);
    itoa((int)(p_CommonData->compInertia*10),inertiaBuffer,10);

    QString directory = "C:/Users/Sam/Desktop/MineWrist_Sept24/TactileDevice/Subjects"+QString::number(p_CommonData->subjectNo);

    if(p_CommonData->currentDynamicObjectState == StiffnessExperiment){
        directory = "C:/Users/Sam/Desktop/MineWrist_Sept24/TactileDevice/Subjects/SubjectStiffn"+QString::number(p_CommonData->subjectNo);
    }
    else if(p_CommonData->currentDynamicObjectState == StiffnessMassExperiment){
        directory = "C:/Users/Sam/Desktop/MineWrist_Sept24/TactileDevice/Subjects/SubjectStiffnMass"+QString::number(p_CommonData->subjectNo);
    }

    QString trialName;
    if (p_CommonData->direct == 1){
        trialName = "vertical_"; // + QString::number(p_CommonData->day)+"_";
//        qDebug() << "Vertical";
    }
    if (p_CommonData->direct == 0){
        trialName = "horizontal_"; // + QString::number(p_CommonData->day)+"_";
//        qDebug() << "Horizontal";
    }    std::ofstream file;

    file.open(directory.toStdString() + "/" + trialName.toStdString() + trialBuffer + "deneme.txt");

    //write data to file when we are done

#ifdef ACC
    p_CommonData->dir="C:/Users/Sam/Desktop/Subjects/VTExp/Subjects/VTACC";
    p_CommonData->fileName = "VTACC_CAL";
#endif

    int i=0;
    for (; i < localDataRecorderVector.size(); i++)
    {
        //[0] is distal finger, [1] is toward middle finger, [2] is away from finger pad
#ifndef ACC
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
//                             "fingerpose_x = "
//                           <<localDataRecorderVector[i].magTrackerPos0[0] << "," << " "
//               //              "fingerpose_y = "
//                           <<localDataRecorderVector[i].magTrackerPos0[1] << "," << " "
//               //              "fingerpose_z = "
//                           <<localDataRecorderVector[i].magTrackerPos0[2] << "," << " "


//                             "des1X = "
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
//                           <<localDataRecorderVector[i].magTrackerPos1[0] << "," << " "
//               //              "fingerpose_y = "
//                           <<localDataRecorderVector[i].magTrackerPos1[1] << "," << " "
//               //              "fingerpose_z = "
//                           <<localDataRecorderVector[i].magTrackerPos1[2] << "," << " "


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


                           <<localDataRecorderVector[i].magTrackerPos0 << "," << " "
                           <<localDataRecorderVector[i].magTrackerPos1 << "," << " "
                           <<localDataRecorderVector[i].index_contact << "," << " "
                           <<localDataRecorderVector[i].thumb_contact << "," << " "




                << std::endl;
#endif
#ifdef ACC
        file <<std::setprecision(9)<< localDataRecorderVector[i].time << "," << " "
        << localDataRecorderVector[i].Acc << "," << " "
        << std::endl;
#endif
    }
    file.close();
    if(i!=(localDataRecorderVector.size())||localDataRecorderVector[localDataRecorderVector.size()-1].ThroughHoop_num==0){

//        qDebug()<<"FUCK YOUR LIFE";
    }
    localDataRecorderVector.clear();
}

void MainWindow::on_dynamicEnvironment_clicked() // USED
{
    qDebug()<<"Button clicked";
//    on_adjustParamsButton_clicked();
    p_CommonData->environmentChange = true;
    p_CommonData->currentDynamicObjectState = standard;
    p_CommonData->currentEnvironmentState = dynamicBodies;

    p_CommonData->InertiaBlockMass  = 0.2;
    p_CommonData->expCD             = 1;

    ui->VRControl->setChecked(true);
    ui->JakeRenderCheckBox->setChecked(false);
    onGUIchanged();
    qDebug()<<"Button finished";
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

void MainWindow::on_StiffnessExp_clicked() //WHAT I USE
{
    QString temp = QFileDialog::getOpenFileName();
    p_CommonData->MineProtocolLocation = temp;
    int error = p_CommonData->MineProtocolFile.LoadFile(temp.toStdString().c_str());
    qDebug() << "error" << error << p_CommonData->MineProtocolLocation;

    p_CommonData->expMass       = 0.2;
    p_CommonData->compMass      = 0.2;
    p_CommonData->refMass       = 0.200;
    p_CommonData->compCD        = 1;
    p_CommonData->compInertia   = 1;

    p_CommonData->expCD         = 1;
    p_CommonData->expInertia    = 1;

    p_CommonData->refCD         = 1;
//    p_CommonData->refInertia    = 1;

    // initialize subjective variables
//    p_CommonData->cdInertiaSubjState = -1;
    p_CommonData->pairNo        = 1;
    p_CommonData->trialNo       = -1;

    p_CommonData->environmentChange         = true;
    p_CommonData->currentDynamicObjectState = StiffnessExperiment;
    p_CommonData->currentExperimentState    = idleExperiment;
    p_CommonData->currentEnvironmentState   = dynamicBodies;
    p_CommonData->scaledDispTransp          = 2;
    p_CommonData->recordFlag                = false;
    ui->VRControl->setChecked(true);
    ui->JakeRenderCheckBox->setChecked(true);
    //qDebug("god help us");
    QThread::msleep(200);
    onGUIchanged();
}

void MainWindow::on_StiffnMassCombined_clicked() //WHAT I USE 2
{
    QString temp = QFileDialog::getOpenFileName();
    p_CommonData->MineProtocolLocation = temp;
    int error = p_CommonData->MineProtocolFile.LoadFile(temp.toStdString().c_str());
    qDebug() << "error" << error << p_CommonData->MineProtocolLocation;

    p_CommonData->expMass       = 0.2;
    p_CommonData->compMass      = 0.2;
    p_CommonData->refMass       = 0.200;
    p_CommonData->compCD        = 1;
    p_CommonData->compInertia   = 1;

    p_CommonData->expCD         = 1;
    p_CommonData->expInertia    = 1;

    p_CommonData->refCD         = 1;
//    p_CommonData->refInertia    = 1;

    // initialize subjective variables
//    p_CommonData->cdInertiaSubjState = -1;
    p_CommonData->pairNo        = 1;
    p_CommonData->trialNo       = -1;

    p_CommonData->environmentChange         = true;
    p_CommonData->currentDynamicObjectState = StiffnessMassExperiment;
    p_CommonData->currentExperimentState    = idleExperiment;
    p_CommonData->currentEnvironmentState   = dynamicBodies;
    p_CommonData->scaledDispTransp          = 2;
    p_CommonData->recordFlag                = false;
    ui->VRControl->setChecked(true);
    ui->JakeRenderCheckBox->setChecked(true);
    //qDebug("god help us");
    QThread::msleep(200);
    onGUIchanged();
}


void MainWindow::on_Manual_clicked() //WHAT I TRY TO CREATE
{

    qDebug()<<"Manual?";
//    on_adjustParamsButton_clicked();
    p_CommonData->environmentChange         = true;
    p_CommonData->currentDynamicObjectState = manual;

    qDebug()<<"Manual v2?";

    p_CommonData->currentEnvironmentState   = dynamicBodies;

    p_CommonData->InertiaBlockMass  = 0.2;
    p_CommonData->expCD             = 1;

    qDebug()<<"Manual 3?";

    ui->VRControl->setChecked(true);
    ui->JakeRenderCheckBox->setChecked(false);
    onGUIchanged();
    qDebug()<<"Button finished";
}

//void MainWindow::on_day1_clicked()
//{
//    p_CommonData->day = 1;
//}

//void MainWindow::on_day2_clicked()
//{
//    p_CommonData->day = 2;
//}

//void MainWindow::on_day3_clicked()
//{
//    p_CommonData->day = 3;
//}
