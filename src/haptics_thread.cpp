#include "haptics_thread.h"
using namespace chai3d;

haptics_thread::haptics_thread(QObject *parent) : QThread(parent)
{
}

haptics_thread::~haptics_thread()
{
}

void haptics_thread::initialize()
{
    InitGeneralChaiStuff();
    InitFingerAndTool();
    //InitEnvironments();
    InitDynamicBodies();

    // GENERAL HAPTICS INITS=================================
    // Ensure the device is not controlling to start
    p_CommonData->wearableDelta0->TurnOffControl();
    p_CommonData->wearableDelta1->TurnOffControl();

    p_CommonData->wearableDelta0->SetDesiredPos(p_CommonData->wearableDelta0->neutralPos); // kinematic neutral position
    p_CommonData->wearableDelta1->SetDesiredPos(p_CommonData->wearableDelta1->neutralPos); // kinematic neutral position

    // set flag that says haptics thread is running
    p_CommonData->hapticsThreadActive = true;
    p_CommonData->environmentChange = false;

    // set to provide feedback when running VR control mode
    p_CommonData->tactileFeedback = true;

    // set up palpation post trial clock
    p_CommonData->palpPostTrialClock.reset();
    p_CommonData->palpPostTrialClock.setTimeoutPeriodSeconds(2.0);

    // Set the clock that controls haptic rate
    //rateClock.reset();
    rateClock.setTimeoutPeriodSeconds(0.000001);
    rateClock.start(true);

    // setup the clock that will enable display of the haptic rate
    rateDisplayClock.reset();
    rateDisplayClock.setTimeoutPeriodSeconds(1.0);
    rateDisplayClock.start(true);

    // setup the overall program time clock
    p_CommonData->overallClock.reset();
    p_CommonData->overallClock.start(true);

    // setup the calibration clock
    p_CommonData->calibClock.reset();
    qDebug()<<"error check 2 pass";
    // init values for first time through on filter
    lastFilteredDeviceForce0.set(0,0,0);
    lastFilteredDeviceForce1.set(0,0,0);
    p_CommonData->deviceComputedForce.set(0,0,0);
    p_CommonData->filteredDeviceComputedForce.set(0,0,0);
    qDebug()<<"error check 3 pass";
    //p_CommonData->workspaceScaleFactor = 1;

    currTime = 0;
    lastTime = 0;

    //init counters to 0
    rateDisplayCounter = 0;
    recordDataCounter = 0;

    // Start off not recording
    p_CommonData->recordFlag = false;

    p_CommonData->currentControlState = idleControl;

    p_CommonData->fingerDisplayScale = 1;
    p_CommonData->box1displayScale = 1;

    p_CommonData->fingerScalePoint.set(0,0,0);

    fingerOffset.set(0.0,-0.006,.003); // finger axis are not at fingerpad, so we want a translation outward on fingertip
    thumbOffset.set(0.0,-0.009,.003); // finger axis are not at fingerpad, so we want a translation outward on fingertip

    // initial box positions
    //tiger edit

    qDebug()<<"error check 4 pass";
    p_CommonData->box1InitPos.set(0.05, 0.0, -0.025);
    p_CommonData->box2InitPos.set(1.0, 0.0, 0.0);
    p_CommonData->box3InitPos.set(1.0, -0.1, 0.0);

    p_CommonData->box1PostInitCenter.set(0.05,0,-0.025);

    p_CommonData->fingerTouching = false; //reset before we check
    p_CommonData->thumbTouching = false;
    p_CommonData->fingerTouchingLast = false;
    p_CommonData->thumbTouchingLast = false;
    p_CommonData->clutchedOffset.set(0,0,0);
    p_CommonData->fingerDisplayScale = 1.0; //will get changed in dynsim if necessary
    p_CommonData->show_forces = false;
    p_CommonData->ImpulseAmp=0;
    p_CommonData->SDVibAmp=0;
    p_CommonData->SDVibFreq=5;
    p_CommonData->SDVibBeta=10;
}

void haptics_thread::run()
{
    while(p_CommonData->hapticsThreadActive)
    {
        //qDebug() << p_CommonData->mapping;
        // if clock controlling haptic rate times out
        if(rateClock.timeoutOccurred())
        {
            // stop clock while we perform haptic calcs
            //rateClock.stop();

            //Eigen::Vector3d inputAxis(0,1,0); // input axis for sin control and circ control modes
            switch(p_CommonData->currentControlState)
            {

            case idleControl:
                UpdateVRGraphics();
                p_CommonData->wearableDelta0->TurnOffControl();
                p_CommonData->wearableDelta1->TurnOffControl();
                break;

            case VRControlMode:
                p_CommonData->sharedMutex.lock();
                UpdateVRGraphics();
                p_CommonData->sharedMutex.unlock();
                ComputeVRDesiredDevicePos();
                bool man = 0;
                if(p_CommonData->currentDynamicObjectState == manual)
                    man = 1;
                else
                    man = 0;

                //qDebug() << "try1" << p_CommonData->ActiveBox << p_CommonData->lev1 << p_CommonData->lev2;
                p_CommonData->wearableDelta0->SendStroke(man, p_CommonData->flagEqual, p_CommonData->flagReverse, p_CommonData->flagEqualTouch, p_CommonData->fingerTouching, p_CommonData->thumbTouching, p_CommonData->ref1, p_CommonData->ActiveBox, p_CommonData->lev1, p_CommonData->lev2, p_CommonData->direct, p_CommonData->flagMassExp);
                p_CommonData->wearableDelta1->SendStroke(man, p_CommonData->flagEqual, p_CommonData->flagReverse, p_CommonData->flagEqualTouch, p_CommonData->fingerTouching, p_CommonData->thumbTouching, p_CommonData->ref2, p_CommonData->ActiveBox, p_CommonData->lev1, p_CommonData->lev2, p_CommonData->direct, p_CommonData->flagMassExp);
                break;
            }

            // update our rate estimate every second
            rateDisplayCounter++;
            if(rateDisplayClock.timeoutOccurred())
            {
                //rateDisplayClock.stop();
                p_CommonData->hapticRateEstimate = rateDisplayCounter;
                rateDisplayCounter = 0;
                rateDisplayClock.reset();
                rateDisplayClock.start();
            }

            // record only on every 10 haptic loops
            recordDataCounter++;
            if(recordDataCounter == 5)
            {
                recordDataCounter = 0;
                if(p_CommonData->recordFlag == true)
                {
                    RecordData();
                }
            }

            // restart rateClock
            rateClock.reset();
            rateClock.start();
        }
    }

    // If we are terminating, delete the haptic device to set outputs to 0
    delete p_CommonData->wearableDelta0;
    delete p_CommonData->wearableDelta1;
}

void haptics_thread::UpdateVRGraphics()
{
#ifndef OCULUS
    // Update camera Pos
    double xPos = p_CommonData->camRadius*cos(p_CommonData->azimuth*PI/180.0)*sin(p_CommonData->polar*PI/180.0);
    double yPos = p_CommonData->camRadius*sin(p_CommonData->azimuth*PI/180.0)*sin(p_CommonData->polar*PI/180.0);
    double zPos = p_CommonData->camRadius*cos(p_CommonData->polar*PI/180.0);
    p_CommonData->cameraPos.set(xPos-0.02, yPos+0.04, zPos-0.10);
#endif

#ifdef OCULUS
    p_CommonData->lookatPos.set(0, .0, p_CommonData->cameraPos.z()+0.2);
    //        p_CommonData->lookatPos.set(0.4, .08, p_CommonData->cameraPos.z() + 0.07);
    //    p_CommonData->cameraPos.set(0.298198, 0.04, -0.348198);
    //qDebug()<<"hi "<<p_CommonData->lookatPos.x() << " " <<p_CommonData->lookatPos.y() << " " <<p_CommonData->lookatPos.z();

#endif

#ifndef OCULUS
    p_CommonData->lookatPos.set(0.0, 0.1, 0.1);
#endif
    // update camera parameters
    p_CommonData->p_camera->set( p_CommonData->cameraPos,
                                 p_CommonData->lookatPos,
                                 p_CommonData->upVector);
    p_CommonData->p_camera->setUseMultipassTransparency(true);
    if(p_CommonData->environmentChange == true)
    {
        p_CommonData->environmentChange = false;
        switch(p_CommonData->currentEnvironmentState)
        {
        case none:
            p_CommonData->p_world->clearAllChildren();
            p_CommonData->p_world->addChild(m_tool0);
            p_CommonData->p_world->addChild(m_tool1);
            p_CommonData->p_world->addChild(finger);
            p_CommonData->p_world->addChild(thumb);
            break;

        case dynamicBodies:
            p_CommonData->p_world->clearAllChildren();
            RenderDynamicBodies();
            break;
       case experimentInertia: //addded to avoid warnings
            qDebug() << "Experiment State not Implemented";
            p_CommonData->p_world->clearAllChildren();
            break;

       case experimentVt: //addded to avoid warnings
            qDebug() << "Experiment State not Implemented";
            p_CommonData->p_world->clearAllChildren();
            break;
        }
    }

    // compute global reference frames for each object
    p_CommonData->p_world->computeGlobalPositions(true);

    // update position and orientation of tool 0(and sphere that represents tool)
    m_tool0->updateFromDevice();
    position0 = m_tool0->m_hapticPoint->getGlobalPosGoal(); // get position and rotation of the haptic point (and delta mechanism) (already transformed from magTracker)
    p_CommonData->chaiMagDevice0->getRotation(rotation0);

    // update position and orientation of tool 1
    m_tool1->updateFromDevice();
    position1 = m_tool1->m_hapticPoint->getGlobalPosGoal();
    p_CommonData->chaiMagDevice1->getRotation(rotation1);

    // update position of finger0 to stay on proxy point
    fingerRotation0 = rotation0;
    fingerRotation0.rotateAboutLocalAxisDeg(0,0,1,90);
    fingerRotation0.rotateAboutLocalAxisDeg(1,0,0,90);
    finger->setLocalRot(fingerRotation0);
    finger->setLocalPos(m_tool0->m_hapticPoint->getGlobalPosProxy() + fingerRotation0*fingerOffset); //this offset isn't for computation of forces, just to align finger model
    m_curSphere0->setLocalPos(position0); // set the sphere visual representation to match
    rotation0.rotateAboutLocalAxisDeg(0,0,1,180); // this is the rotation matrix deviceToWorld
    m_curSphere0->setLocalRot(rotation0);
    m_tool0->computeInteractionForces();

    // update position of finger1 to stay on proxy  point
    fingerRotation1 = rotation1;
    fingerRotation1.rotateAboutLocalAxisDeg(0,0,1,90);
    fingerRotation1.rotateAboutLocalAxisDeg(1,0,0,90);
    fingerRotation1 = fingerRotation1;
    thumb->setLocalRot(fingerRotation1);
    thumb->setLocalPos(m_tool1->m_hapticPoint->getGlobalPosProxy() + fingerRotation1*thumbOffset);
    m_curSphere1->setLocalPos(position1);
    rotation1.rotateAboutLocalAxisDeg(0,0,1,180); // this is the rotation matrix deviceToWorld
    m_curSphere1->setLocalRot(rotation1);
    m_tool1->computeInteractionForces();

    // update frames to show or not
    if (p_CommonData->showCursorFrames)
    {
        m_curSphere0->setShowFrame(true);
        m_curSphere1->setShowFrame(true);
        p_CommonData->adjustBox->setShowFrame(true);
    }
    else
    {
        m_curSphere0->setShowFrame(false);
        m_curSphere1->setShowFrame(false);
        p_CommonData->adjustBox->setShowFrame(false);
    }

    //check for applying scaling based on whether we were touching last time
    p_CommonData->fingerTouchingLast = p_CommonData->fingerTouching;
    p_CommonData->thumbTouchingLast = p_CommonData->thumbTouching;

    chai3d::cVector3d gravity_force1(0.0, 0.0, 0.0);
    chai3d::cVector3d gravity_force2(0.0, 0.0, 0.0);    
    chai3d::cVector3d gravity_force3(0.0, 0.0, 0.0);

    // set fingers to non initially touching
    p_CommonData->fingerTouching = false;
    p_CommonData->thumbTouching = false;

    // find loop time interval
    currTime = p_CommonData->overallClock.getCurrentTimeSeconds();
    timeInterval = currTime - lastTime;
    if(timeInterval > 0.001)
    {
        timeInterval = 0.001;
    }

    double forceLimit = 20.0;

    // perform our dynamic body updates if we are in a dynamic environment
    if(p_CommonData->currentEnvironmentState == dynamicBodies)
    {
        if(timeInterval > 0.01) timeInterval = 0.01;
        //---------------------------------------------------
        // Implement Dynamic simulation
        //---------------------------------------------------
        // for each interaction point of the tool we look for any contact events
        // with the environment and apply forces accordingly
        int numInteractionPoints = m_tool0->getNumHapticPoints();

        for (int j=0; j<numInteractionPoints; j++)
        {
            // get pointer to next interaction point of tool
            chai3d::cHapticPoint* interactionPoint = m_tool0->getHapticPoint(j);

            // check all contact points
            int numContacts = interactionPoint->getNumCollisionEvents();
            for (int k=0; k<numContacts; k++)
            {
                chai3d::cCollisionEvent* collisionEvent = interactionPoint->getCollisionEvent(k);

                // given the mesh object we may be touching, we search for its owner which
                // could be the mesh itself or a multi-mesh object. Once the owner found, we
                // look for the parent that will point to the ODE object itself.
                chai3d::cGenericObject* object = collisionEvent->m_object->getOwner()->getOwner();

                //if finger touching box, note it
                p_CommonData->fingerTouching = true;

                // cast to ODE object
                cODEGenericBody* ODEobject = dynamic_cast<cODEGenericBody*>(object);
                if (ODEobject==p_CommonData->ODEBody1){
                    p_CommonData->ActiveBox = 1;
                }
                else if(ODEobject==p_CommonData->ODEBody2){
                    p_CommonData->ActiveBox = 2;
                }

                // if ODE object, we apply interaction forces
                if (ODEobject != NULL){

                    ODEobject->addExternalForceAtPoint(-p_CommonData->adjustedDynamicForceReduction * interactionPoint->getLastComputedForce(),
                                                       collisionEvent->m_globalPos);

                    if((interactionPoint->getLastComputedForce().length() > forceLimit) & (computedForce0.length() > forceLimit))
                        p_CommonData->resetBoxPosFlag = false; //true;

                }
            }
        }
        numInteractionPoints = m_tool1->getNumHapticPoints();
        for (int j=0; j<numInteractionPoints; j++)
        {
            // get pointer to next interaction point of tool
            chai3d::cHapticPoint* interactionPoint = m_tool1->getHapticPoint(j);

            // check all contact points
            int numContacts = interactionPoint->getNumCollisionEvents();
            for (int k=0; k<numContacts; k++)
            {
                chai3d::cCollisionEvent* collisionEvent = interactionPoint->getCollisionEvent(k);

                // given the mesh object we may be touching, we search for its owner which
                // could be the mesh itself or a multi-mesh object. Once the owner found, we
                // look for the parent that will point to the ODE object itself.
                chai3d::cGenericObject* object = collisionEvent->m_object->getOwner()->getOwner();

                //if thumb touching box, note it
                p_CommonData->thumbTouching = true;

                // cast to ODE object
                cODEGenericBody* ODEobject = dynamic_cast<cODEGenericBody*>(object);
                if (ODEobject==p_CommonData->ODEBody1){
                    p_CommonData->ActiveBox = 1;
                }

                else if(ODEobject==p_CommonData->ODEBody2){
                    p_CommonData->ActiveBox = 2;
                }

                // if ODE object, we apply interaction forces
                if (ODEobject != NULL)
                {
                    ODEobject->addExternalForceAtPoint(-p_CommonData->adjustedDynamicForceReduction * interactionPoint->getLastComputedForce(),
                                                       collisionEvent->m_globalPos);
                    if((interactionPoint->getLastComputedForce().length() > forceLimit) & (computedForce1.length() > forceLimit))
                        p_CommonData->resetBoxPosFlag = false; //true;

                }
            }
        }

        // tiger added inertia to cd
        double original_mass1 = p_CommonData->mass1;
        double original_mass2 = p_CommonData->mass2;
        double original_mass3 = p_CommonData->mass3;

        //For Mine's Experiments --> 2boxes
        if (p_CommonData->currentDynamicObjectState == StiffnessExperiment
                || p_CommonData->currentDynamicObjectState == StiffnessMassExperiment){

            p_CommonData->ODEBody1->setMass(p_CommonData->mass1);
            p_CommonData->ODEBody2->setMass(p_CommonData->mass2);

            gravity_force1.set(0,0,original_mass1*9.81);
            gravity_force2.set(0,0,original_mass2*9.81);

            p_CommonData->ODEBody1->addExternalForce(gravity_force1);
            p_CommonData->ODEBody2->addExternalForce(gravity_force2);
        }

        //For Jasmin's Experiments - Only 1 box
        if (p_CommonData->currentDynamicObjectState == FingerMappingExperiment){

            p_CommonData->ODEBody1->setMass(p_CommonData->mass1);

            gravity_force1.set(0.0, 0.0, original_mass1*9.81);

            p_CommonData->ODEBody1->addExternalForce(gravity_force1);
        }

        if (p_CommonData->currentDynamicObjectState == HoxelMappingExperiment){

            p_CommonData->ODEBody1->setMass(p_CommonData->mass1);
            //p_CommonData->ODEBody2->setMass(p_CommonData->mass2);

            gravity_force1.set(0,0,original_mass1*9.81);
            //gravity_force2.set(0,0,original_mass2*9.81);

            p_CommonData->ODEBody1->addExternalForce(gravity_force1);
            //p_CommonData->ODEBody2->addExternalForce(gravity_force2);

            //qDebug()<< "BoxGlobalX: " << p_CommonData->ODEBody1->getGlobalPos().x()<< "Hoop: " << p_CommonData->ODEHoop1->getGlobalPos().x() << "targetRad: " << targetRadius;
            //Keep motion to the yz plane only when box is checked
            if (p_CommonData->enablePlanarConstraint == true)
            {
                //Stricly on the plane:
                chai3d::cVector3d thisPos = p_CommonData->ODEBody1->getLocalPos();
                //p_CommonData->ODEBody1->setLocalPos(box1InitPos.x(), thisPos.get(1), thisPos.get(2));
                //qDebug()<< "x: " << thisPos.get(0) << " | y: " << thisPos.get(1) << " | z: " << thisPos.get(2);

                //within a range around the yz plane:
                double boundary = targetRadius;
                //If exceeding max allowable val:
                if(thisPos.x() > box1InitPos.x()+boundary)
                {
                    p_CommonData->ODEBody1->setLocalPos(box1InitPos.x()+boundary, thisPos.get(1), thisPos.get(2));
                    qDebug() << "lim1";
                }
                //If exceeding min allowable val:
                if(thisPos.x() < box1InitPos.x()-boundary)
                {
                    p_CommonData->ODEBody1->setLocalPos(box1InitPos.x()-boundary, thisPos.get(1), thisPos.get(2));
                    qDebug() << "lim2";
                }
                else
                {
                    //move normally
                    qDebug() << "normal";
                }
            }
            //else -- move normally without planar constraint
        }

        if (p_CommonData->currentDynamicObjectState == MultiMassExperiment){

            p_CommonData->ODEBody1->setMass(p_CommonData->mass1);
            p_CommonData->ODEBody2->setMass(p_CommonData->mass2);
            p_CommonData->ODEBody3->setMass(p_CommonData->mass3);

            gravity_force1.set(0,0,original_mass1*9.81);
            gravity_force2.set(0,0,original_mass2*9.81);
            gravity_force3.set(0,0,original_mass3*9.81);

            p_CommonData->ODEBody1->addExternalForce(gravity_force1);
            p_CommonData->ODEBody2->addExternalForce(gravity_force2);
            p_CommonData->ODEBody3->addExternalForce(gravity_force3);


        }

        if(p_CommonData->show_forces)
        {
            if(m_tool1->getDeviceGlobalForce().length()>0)
            {

                force1_show->m_pointA = m_tool1->m_hapticPoint->getGlobalPosProxy();
                force1_show->m_pointB = m_tool1->m_hapticPoint->getGlobalPosProxy()+(((lastforce1*0.95)+(m_tool1->getDeviceGlobalForce()*0.05))*0.05);
                force2_show->m_pointA = m_tool0->m_hapticPoint->getGlobalPosProxy();
                force2_show->m_pointB = m_tool0->m_hapticPoint->getGlobalPosProxy()+(((lastforce2*0.95)+(m_tool0->getDeviceGlobalForce()*0.05))*0.05);

                lastforce1=(lastforce1*0.95)+(m_tool1->getDeviceGlobalForce()*0.05);
                lastforce2=(lastforce2*0.95)+(m_tool0->getDeviceGlobalForce()*0.05);
            }
            else
            {
                force1_show->m_pointA = m_tool1->m_hapticPoint->getGlobalPosProxy();
                force1_show->m_pointB = m_tool1->m_hapticPoint->getGlobalPosProxy();
                force2_show->m_pointA = m_tool0->m_hapticPoint->getGlobalPosProxy();
                force2_show->m_pointB = m_tool0->m_hapticPoint->getGlobalPosProxy();
                lastforce1=chai3d::cVector3d(0,0,0);
                lastforce2=chai3d::cVector3d(0,0,0);
            }
        }
        else
        {
            force1_show->m_pointA = chai3d::cVector3d(0,0,0);
            force1_show->m_pointB = chai3d::cVector3d(0,0,0);
            force2_show->m_pointA = chai3d::cVector3d(0,0,0);
            force2_show->m_pointB = chai3d::cVector3d(0,0,0);
        }

        // update simulation
        ODEWorld->updateDynamics(timeInterval);

        // update display scaling and scaling center if we just grabbed a box or are pushing and need to scale and center
        if (p_CommonData->fingerTouching | p_CommonData->thumbTouching)
        {
            // need this to only run the first time they're both touching
            if(!p_CommonData->fingerTouchingLast & !p_CommonData->thumbTouchingLast)
            {
                p_CommonData->fingerScalePoint = curCenter;
                p_CommonData->fingerDisplayScale = 1.0;
            }
        }
    }

    lastTime = currTime;

    // update scaled positions
    UpdateScaledCursors();
    UpdateScaledFingers();
    UpdateScaledBoxes();

    //chai3d::cVector3d Box2Pos = p_CommonData->ODEBody2->getLocalPos();
    //chai3d::cVector3d Box1Pos = p_CommonData->ODEBody1->getLocalPos();
    chai3d::cVector3d box1Pos = p_CommonData->ODEBody1->getLocalPos();
    chai3d::cVector3d box2Pos = p_CommonData->ODEBody2->getLocalPos();

    //VR Updates for Mine's StiffnessExperiment
    if(p_CommonData->currentDynamicObjectState == StiffnessExperiment)
    {
        if(!p_CommonData->target1Complete)
        {
            //Find distance between box2 and target1
            chai3d::cVector3d err12 = box2Pos - target1Pos;
            //Find distance between box1 and target1
            chai3d::cVector3d err11 = box1Pos - target1Pos;

            if(p_CommonData->stiffness2 > p_CommonData->stiffness1)
            {
                if(err11.length()< targetRadius)
                {
                    p_CommonData->target1Responded = true;
                    p_CommonData->target1Complete = false;
                    p_CommonData->targetSuccess = 0;
                    if (p_CommonData->TrialType=="training"){
                        matTarget1.setRed();
                        target1->setMaterial(matTarget1);
                        target1->setTransparencyLevel(0.35, true);
                    }
                    else if (p_CommonData->TrialType=="testing"){
                        matTarget1.setPurple();
                        target1->setMaterial(matTarget1);
                        target1->setTransparencyLevel(1, true);
                    }
                }
                if(err12.length()< targetRadius)
                {
                    p_CommonData->target1Responded = true;
                    p_CommonData->target1Complete = true;
                    p_CommonData->targetSuccess = 1;
                    if (p_CommonData->TrialType=="training"){
                        matTarget1.setGreen();
                        target1->setMaterial(matTarget1);
                        target1->setTransparencyLevel(0.35, true);
                    }
                    else if (p_CommonData->TrialType=="testing"){
                        matTarget1.setPurple();
                        target1->setMaterial(matTarget1);
                        target1->setTransparencyLevel(1, true);
                    }
                }
            }

            if(p_CommonData->stiffness1 > p_CommonData->stiffness2)
            {
                if(err11.length()< targetRadius)
                {
                    p_CommonData->target1Responded = true;
                    p_CommonData->target1Complete = true;
                    p_CommonData->targetSuccess = 1;
                    if (p_CommonData->TrialType=="training")
                    {
                        matTarget1.setGreen();
                        target1->setMaterial(matTarget1);
                        target1->setTransparencyLevel(0.35, true);
                    }
                    else if (p_CommonData->TrialType=="testing")
                    {
                        matTarget1.setPurple();
                        target1->setMaterial(matTarget1);
                        target1->setTransparencyLevel(1, true);
                    }
                }
                if(err12.length()< targetRadius)
                {
                    p_CommonData->target1Responded = true;
                    p_CommonData->target1Complete = false;
                    p_CommonData->targetSuccess = 0;
                    if (p_CommonData->TrialType=="training"){
                        matTarget1.setRed();
                        target1->setMaterial(matTarget1);
                        target1->setTransparencyLevel(0.35, true);
                    }
                    else if (p_CommonData->TrialType=="testing")
                    {
                        matTarget1.setPurple();
                        target1->setMaterial(matTarget1);
                        target1->setTransparencyLevel(1, true);
                    }
                }
            }
        }
    }

    //VR Updates for Mine's StiffnessMassExperiment
    if(p_CommonData->currentDynamicObjectState == StiffnessMassExperiment)
    {
        if(!p_CommonData->target1Complete && !p_CommonData->hoop1Complete)
        {
            //Find distance between box1 and hoop1
            chai3d::cVector3d err11 = box1Pos - hoop1Pos;

            if(err11.length() < targetRadius)
            {
                p_CommonData->hoop1Complete = true;
                hoop1->setMaterial(matHoop1);
                hoop1->setTransparencyLevel(0.85, true);
            }
        }
        else if(!p_CommonData->target1Complete && p_CommonData->hoop1Complete)
        {
            //Find distance between box1 and target1
            chai3d::cVector3d err12 = box1Pos - target1Pos;

            if(err12.length()< targetRadius)
            {
                p_CommonData->target1Complete = true;
                target1->setMaterial(matTarget1);
                target1->setTransparencyLevel(0.85, true);
            }
        }

        if(!p_CommonData->target2Complete && !p_CommonData->hoop2Complete)
        {
            //Find distance between box 2 and hoop2
            chai3d::cVector3d err22 = box2Pos - hoop2Pos;
            if(err22.length()< targetRadius)
            {
                p_CommonData->hoop2Complete = true;
                hoop2->setMaterial(matHoop2);
                hoop2->setTransparencyLevel(0.85, true);
            }
        }
        else if(!p_CommonData->target2Complete && p_CommonData->hoop2Complete)
        {
            //find distance between box2 and target2
            chai3d::cVector3d err22 = box2Pos - target2Pos;
            if(err22.length()< targetRadius)
            {
                p_CommonData->target2Complete = true;
                target2->setMaterial(matTarget2);
                target2->setTransparencyLevel(0.85, true);
            }
        }

        if(p_CommonData->target1Complete && p_CommonData->target2Complete)
        {
            p_CommonData->explorationComplete = true;
            if (p_CommonData->lev1 > p_CommonData->lev2)
            {
                if(p_CommonData->answer1)
                {
                    p_CommonData->targetSuccess = 1;
                    if (p_CommonData->TrialType=="training")
                    {
                        matTarget1.setGreen();
                        target1->setMaterial(matTarget1);
                        target1->setTransparencyLevel(0.85, true);
                        matHoop1.setGreen();
                        hoop1->setMaterial(matHoop1);
                        hoop1->setTransparencyLevel(0.85, true);
                        p_CommonData->answerComplete = true;
                    }
                    else if (p_CommonData->TrialType=="testing")
                    {
                        matTarget1.setGray();
                        target1->setMaterial(matTarget1);
                        target1->setTransparencyLevel(1, true);
                        matHoop1.setGray();
                        hoop1->setMaterial(matHoop1);
                        hoop1->setTransparencyLevel(0.85, true);
                        p_CommonData->answerComplete = true;
                    }
                }
                else if(p_CommonData->answer2)
                {
                    p_CommonData->targetSuccess = 0;
                    if (p_CommonData->TrialType=="training")
                    {
                        matTarget2.setRed();
                        target2->setMaterial(matTarget2);
                        target2->setTransparencyLevel(1, true);
                        matHoop2.setRed();
                        hoop2->setMaterial(matHoop2);
                        hoop2->setTransparencyLevel(0.85, true);
                        p_CommonData->answerComplete = true;
                    }
                    else if (p_CommonData->TrialType=="testing")
                    {
                        matTarget2.setGray();
                        target2->setMaterial(matTarget2);
                        target2->setTransparencyLevel(1, true);
                        matHoop2.setGray();
                        hoop2->setMaterial(matHoop2);
                        hoop2->setTransparencyLevel(0.85, true);
                        p_CommonData->answerComplete = true;
                    }
                }
            }
            else if (p_CommonData->lev1 < p_CommonData->lev2)
            {
                if(p_CommonData->answer1)
                {
                    p_CommonData->targetSuccess = 0;
                    if (p_CommonData->TrialType=="training")
                    {
                        matTarget1.setRed();
                        target1->setMaterial(matTarget1);
                        target1->setTransparencyLevel(1, true);
                        matHoop1.setRed();
                        hoop1->setMaterial(matHoop1);
                        hoop1->setTransparencyLevel(0.85, true);
                        p_CommonData->answerComplete = true;
                    }
                    else if (p_CommonData->TrialType=="testing")
                    {
                        matTarget1.setGray();
                        target1->setMaterial(matTarget1);
                        target1->setTransparencyLevel(1, true);
                        matHoop1.setGray();
                        hoop1->setMaterial(matHoop1);
                        hoop1->setTransparencyLevel(0.85, true);
                        p_CommonData->answerComplete = true;
                    }
                }
                else if(p_CommonData->answer2)
                {
                    p_CommonData->targetSuccess = 1;
                    if (p_CommonData->TrialType=="training")
                    {
                        matTarget2.setGreen();
                        target2->setMaterial(matTarget2);
                        target2->setTransparencyLevel(1, true);
                        matHoop2.setGreen();
                        hoop2->setMaterial(matHoop2);
                        hoop2->setTransparencyLevel(0.85, true);
                        p_CommonData->answerComplete = true;
                    }
                    else if (p_CommonData->TrialType=="testing")
                    {
                        matTarget2.setGray();
                        target2->setMaterial(matTarget2);
                        target2->setTransparencyLevel(1, true);
                        matHoop2.setGray();
                        hoop2->setMaterial(matHoop2);
                        hoop2->setTransparencyLevel(0.85, true);
                        p_CommonData->answerComplete = true;
                    }
                }
            }
        }
    }

    //VR Updates for Jasmin's FingerMapping Experiment
    if(p_CommonData->currentDynamicObjectState == FingerMappingExperiment ||
            p_CommonData->currentDynamicObjectState == HoxelMappingExperiment)
    {
        //If netiher the target or hoop are completed
        if(!p_CommonData->target1Complete && !p_CommonData->hoop1Complete)
        {
            //qDebug()<<"target + hoop not complete";
            //Find distance between box1 and hoop1
            chai3d::cVector3d err11 = box1Pos - hoop1Pos;

            if(err11.length() < targetRadius)
            {
                p_CommonData->hoop1Complete = true;
                hoop1->setMaterial(matHoop1);
                hoop1->setTransparencyLevel(0.85, true);
                //qDebug()<<"hoop completed";

                p_CommonData->hoopSuccess = 1;
            }
        }
        //If the target is not completed but the hoop is completed
        else if(!p_CommonData->target1Complete && p_CommonData->hoop1Complete)
        {
            //qDebug()<<"target not complete, hoop completed earlier";
            //Find distance between box1 and target1
            chai3d::cVector3d err12 = box1Pos - target1Pos;

            if(err12.length()< targetRadius)
            {
                p_CommonData->target1Complete = true;
                target1->setMaterial(matTarget1);
                target1->setTransparencyLevel(0.85, true);
                //qDebug()<<"target now completed in addition to hoop";

                p_CommonData->targetSuccess = 1;
                p_CommonData->trialSuccess = 1;
            }
        }

        //        //less than to accomodate coordinate differences
        //        if(box1Pos.z() <= -0.03)
        //        {
        //            //make invisible
        //            p_CommonData->p_dynamicBox1->setTransparencyLevel(0.0, true);
        //        }
        //        else
        //        {
        //            //make visible again
        //            p_CommonData->p_dynamicBox1->setTransparencyLevel(1.0, true);
        //        }
        //        qDebug()<< "X: " << box1Pos.x() << "Y: " << box1Pos.y() << "Z: " << box1Pos.z();
    }

    // if fingers reset in box, fix it and reset the environment again
    if(p_CommonData->resetBoxPosFlag)
    {
        qDebug() << "resetting";
        p_CommonData->p_world->computeGlobalPositions(true);
        // set position of box back to starting point
        chai3d::cMatrix3d eyeMat(1,0,0,0,1,0,0,0,1);

        p_CommonData->p_world->removeChild(m_tool0);
        p_CommonData->p_world->removeChild(m_tool1);
        ODEWorld->updateDynamics(timeInterval);

        p_CommonData->p_world->addChild(m_tool0);
        p_CommonData->p_world->addChild(m_tool1);
        m_tool0->updateFromDevice();
        m_tool1->updateFromDevice();
        p_CommonData->p_world->computeGlobalPositions(true);
        m_tool0->updateFromDevice();
        m_tool1->updateFromDevice();

        p_CommonData->p_world->removeChild(force1_show);
        p_CommonData->p_world->removeChild(force2_show);
        p_CommonData->p_world->addChild(force1_show);
        p_CommonData->p_world->addChild(force2_show);

        //p_CommonData->p_world->removeChild(hoop1);
        //p_CommonData->p_world->removeChild(hoop2);
        //p_CommonData->p_world->removeChild(hoop3);
        //p_CommonData->p_world->removeChild(hoop4);
        //p_CommonData->p_world->addChild(hoop1);
        //p_CommonData->p_world->addChild(hoop2);
        //p_CommonData->p_world->addChild(hoop3);
        //p_CommonData->p_world->addChild(hoop4);

        p_CommonData->p_world->computeGlobalPositions(true);
        p_CommonData->ODEBody1->setLocalPos(p_CommonData->box1InitPos);
        p_CommonData->ODEBody1->setLocalRot(eyeMat);
        ODEWorld->updateDynamics(timeInterval);

        p_CommonData->fingerTouching = false; //reset before we check
        p_CommonData->thumbTouching = false;
        p_CommonData->fingerTouchingLast = false;
        p_CommonData->thumbTouchingLast = false;
        //p_CommonData->scaledDispTransp = 1;
        p_CommonData->clutchedOffset.set(0,0,0);
        p_CommonData->fingerDisplayScale = 1.0;

        p_CommonData->resetBoxPosFlag = false;
    }

    /*
    //set scaled stuff to show or not show
    UpdateScaledTransparency();

    //Remove environment if experiments done
    if(p_CommonData->expDone)
    {
      p_CommonData->environmentChange = true;
      p_CommonData->currentEnvironmentState = none;
      p_CommonData->expDone = false;
    }
    */
}

void haptics_thread::UpdateScaledCursors()
{
    curCenter = (m_tool0->m_hapticPoint->getGlobalPosProxy() + m_tool1->m_hapticPoint->getGlobalPosProxy())/2.0;
    centToFingCur = m_tool0->m_hapticPoint->getGlobalPosProxy() - curCenter;
    centToThumbCur = m_tool1->m_hapticPoint->getGlobalPosProxy() - curCenter;

    //    if(p_CommonData->fingerTouching || p_CommonData->thumbTouching)
    //    {
    //        p_CommonData->fingerDisplayScale = 1.0/p_CommonData->expCD;
    //    }
    //    else
    //    {
    p_CommonData->fingerDisplayScale = 1.0;
    //    }

    //    if(!p_CommonData->cdVertScaleOn)
    //    {
    scaledCurCenter = p_CommonData->fingerDisplayScale*(curCenter - p_CommonData->fingerScalePoint) + p_CommonData->fingerScalePoint + p_CommonData->clutchedOffset;
    //    }
    //    else
    //    {
    //        scaledCurCenter.x(((curCenter - p_CommonData->fingerScalePoint) + p_CommonData->fingerScalePoint + p_CommonData->clutchedOffset).x());
    //        scaledCurCenter.y(((curCenter - p_CommonData->fingerScalePoint) + p_CommonData->fingerScalePoint + p_CommonData->clutchedOffset).y());
    //        scaledCurCenter.z((p_CommonData->fingerDisplayScale*(curCenter - p_CommonData->fingerScalePoint) + p_CommonData->fingerScalePoint
    //                           + p_CommonData->clutchedOffset).z());
    //    }
    m_dispScaleCurSphere0->setLocalPos(scaledCurCenter + centToFingCur);
    m_dispScaleCurSphere1->setLocalPos(scaledCurCenter + centToThumbCur);
}

void haptics_thread::UpdateScaledFingers()
{
    scaledFinger->setLocalRot(fingerRotation0);
    scaledFinger->setLocalPos(m_dispScaleCurSphere0->getLocalPos() + fingerRotation0*fingerOffset);

    // update position of thumb to stay on scaled cursor (which is scaled relative to proxy point)
    scaledThumb->setLocalRot(fingerRotation1);
    scaledThumb->setLocalPos(m_dispScaleCurSphere1->getLocalPos() + fingerRotation1*thumbOffset);
}

void haptics_thread::UpdateScaledBoxes()
{
    p_CommonData->box1displayScale = 1.0; ///p_CommonData->expCD;

    //    chai3d::cVector3d scaledBox1Pos;
    //    if(!p_CommonData->cdVertScaleOn)
    //    {
    scaledBox1Pos = p_CommonData->box1PostInitCenter + (p_CommonData->ODEBody1->getLocalPos()-p_CommonData->box1PostInitCenter)*p_CommonData->box1displayScale;
    //    }
    //    else
    //    {
    //        scaledBox1Pos.x((p_CommonData->box1PostInitCenter + (p_CommonData->ODEBody1->getLocalPos()-p_CommonData->box1PostInitCenter)).x());
    //        scaledBox1Pos.y((p_CommonData->box1PostInitCenter + (p_CommonData->ODEBody1->getLocalPos()-p_CommonData->box1PostInitCenter)).y());
    //        scaledBox1Pos.z((p_CommonData->box1PostInitCenter + (p_CommonData->ODEBody1->getLocalPos()-p_CommonData->box1PostInitCenter)*p_CommonData->box1displayScale).z());
    //    }
    p_CommonData->p_dynamicScaledBox1->setLocalPos(scaledBox1Pos);
    p_CommonData->p_dynamicScaledBox1->setLocalRot(p_CommonData->ODEBody1->getLocalRot());
}

void haptics_thread::ComputeVRDesiredDevicePos()
{
    // perform transformation to get "device forces"
    computedForce0 = m_tool0->getDeviceGlobalForce();
    computedForce1 = m_tool1->getDeviceGlobalForce();

    //qDebug()<< computedForce0.x() << computedForce0.y() << computedForce0.z();
    //qDebug()<< computedForce1.x() << computedForce1.y() << computedForce1.z();

    // rotation of delta mechanism in world frame from cursor updates(originally from mag tracker, but already rotated the small bend angle of finger)
    rotation0.trans(); //from deviceToWorld to worldToDevice
    rotation1.trans();

    // this are the forces in the device frames
    deviceComputedForce0 = rotation0*computedForce0; // rotation between force in world and delta frames
    deviceComputedForce1 = rotation1*computedForce1; // rotation between force in world and delta frames

    //qDebug()<<deviceComputedForce0.z()<<" "<<deviceComputedForce1.z();

    if(p_CommonData->jakeRender)
    {
        chai3d::cMatrix3d boxRot_boxToWorld;
        if(p_CommonData->currentDynamicObjectState == standard)
            boxRot_boxToWorld = p_CommonData->ODEBody1->getLocalRot();
        else if(p_CommonData->currentDynamicObjectState == manual)
            boxRot_boxToWorld = p_CommonData->ODEBody1->getLocalRot();
        else if(p_CommonData->currentDynamicObjectState == StiffnessExperiment
                || p_CommonData->currentDynamicObjectState == StiffnessMassExperiment
                || p_CommonData->currentDynamicObjectState == FingerMappingExperiment
                || p_CommonData->currentDynamicObjectState == HoxelMappingExperiment
                || p_CommonData->currentDynamicObjectState == MultiMassExperiment) // || p_CommonData->currentDynamicObjectState == dynamicMagnitudeExp)
            boxRot_boxToWorld = p_CommonData->ODEBody1->getLocalRot();
        //        else if(p_CommonData->currentDynamicObjectState == dynamicSubjectiveExp)
        //            boxRot_boxToWorld = p_CommonData->ODEBody1->getLocalRot();

        // get current rotation of adjust box
        chai3d::cMatrix3d boxRot_worldToBox; boxRot_boxToWorld.transr(boxRot_worldToBox);
        chai3d::cMatrix3d rotation0_deviceToWorld; rotation0.transr(rotation0_deviceToWorld); // find rotation of index delta device in world coordinates
        chai3d::cMatrix3d rotation1_deviceToWorld; rotation1.transr(rotation1_deviceToWorld); // find rotation of thumb delta device in world coordinates

        // compute angle between box back normal and index normal
        chai3d::cVector3d boxIndexSideNorm(-1,0,0); // index side vector normal to surface (in box frame)
        chai3d::cVector3d boxThumbSideNorm(1,0,0); // thumb side vector normalto surface(in box frame)
        chai3d::cVector3d fingerpadNorm(0,0,-1); // vector normal to surface of fingerpad (in finger frame)
        chai3d::cVector3d thumbpadNorm(0,0,-1); // vector normal to surface of fingerpad (in thumb frame)

        chai3d::cVector3d boxIndexSideNormGlobal = boxRot_boxToWorld*boxIndexSideNorm; // index side normal to surface expressed in world coords
        chai3d::cVector3d boxThumbSideNormGlobal = boxRot_boxToWorld*boxThumbSideNorm; // thumb side normal to surface expressed in world coords
        chai3d::cVector3d fingerpadNormGlobal = rotation0_deviceToWorld*fingerpadNorm; // fingerpad normal vector expressed in world coords
        chai3d::cVector3d thumbpadNormGlobal = rotation1_deviceToWorld*thumbpadNorm; // thumbpad normal vector expressed in world coords

        chai3d::cVector3d crossAxis0; boxIndexSideNormGlobal.crossr(fingerpadNormGlobal, crossAxis0); //cross axis is global frames
        chai3d::cVector3d crossAxis1; boxThumbSideNormGlobal.crossr(thumbpadNormGlobal, crossAxis1);// cross axis in global frames
        crossAxis0.normalize(); crossAxis1.normalize();

        // find dot product of box norm and finger norm expressed in global coords (for finding angle)
        double dotProduct0 = boxIndexSideNormGlobal.dot(fingerpadNormGlobal);
        double magBoxIndexSideNormGlobal = boxIndexSideNormGlobal.length();
        double magFingerpadNormGlobal = fingerpadNormGlobal.length();
        double angle0 = acos(dotProduct0/(magBoxIndexSideNormGlobal*magFingerpadNormGlobal));

        double dotProduct1 = boxThumbSideNormGlobal.dot(thumbpadNormGlobal);
        double magBoxThumbSideNormGlobal = boxThumbSideNormGlobal.length();
        double magThumbpadNormGlobal = thumbpadNormGlobal.length();
        double angle1 = acos(dotProduct1/(magBoxThumbSideNormGlobal*magThumbpadNormGlobal));


        chai3d::cMatrix3d angleRotationMatrix0; double ux=crossAxis0.x(); double uy=crossAxis0.y(); double uz=crossAxis0.z();
        angleRotationMatrix0.set( cos(angle0)+pow(ux,2)*(1-cos(angle0)), ux*uy*(1-cos(angle0))-uz*sin(angle0), ux*uz*(1-cos(angle0))+uy*sin(angle0),
                                  uy*ux*(1-cos(angle0))+uz*sin(angle0), cos(angle0)+pow(uy,2)*(1-cos(angle0)), uy*uz*(1-cos(angle0))-ux*sin(angle0),
                                  uz*ux*(1-cos(angle0))-uy*sin(angle0), uz*uy*(1-cos(angle0))+ux*sin(angle0), cos(angle0)+pow(uz,2)*(1-cos(angle0)) );

        chai3d::cVector3d forceToIndexGlobal = angleRotationMatrix0*computedForce0; // rotate global force by angle about crossAxis
        chai3d::cVector3d forceToIndexLocal = forceToIndexGlobal; //rotation0*forceToIndexGlobal; // express that in the device frame

        chai3d::cMatrix3d angleRotationMatrix1; ux=crossAxis1.x(); uy=crossAxis1.y(); uz=crossAxis1.z();
        angleRotationMatrix1.set( cos(angle1)+pow(ux,2)*(1-cos(angle1)), ux*uy*(1-cos(angle1))-uz*sin(angle1), ux*uz*(1-cos(angle1))+uy*sin(angle1),
                                  uy*ux*(1-cos(angle1))+uz*sin(angle1), cos(angle1)+pow(uy,2)*(1-cos(angle1)), uy*uz*(1-cos(angle1))-ux*sin(angle1),
                                  uz*ux*(1-cos(angle1))-uy*sin(angle1), uz*uy*(1-cos(angle1))+ux*sin(angle1), cos(angle1)+pow(uz,2)*(1-cos(angle1)) );

        chai3d::cVector3d forceToThumbGlobal = angleRotationMatrix1*computedForce1; // rotate global force by angle about crossAxis
        chai3d::cVector3d forceToThumbLocal = forceToThumbGlobal; //rotation1*forceToThumbGlobal; // express that in the device frame


        //        if(CAP_NORMAL_FORCE){ //caps normal force at 1 N
        //            if(abs(forceToIndexLocal.z())>2){
        //                forceToIndexLocal.set(forceToIndexLocal.x(),forceToIndexLocal.y(),((0.0 < forceToIndexLocal.z()) - (forceToIndexLocal.z() < 0.0))*2.0);
        //            }
        //            if(abs(forceToThumbLocal.z())>2){
        //                forceToThumbLocal.set(forceToThumbLocal.x(),forceToThumbLocal.y(),((0.0 < forceToThumbLocal.z()) - (forceToThumbLocal.z() < 0.0))*2.0);
        //            }
        //        }

        //        if(CAP_LATERAL_DIRECTION){ //caps normal force at 0 N
        //            if(forceToIndexLocal.y()<0){
        //                forceToIndexLocal.set(forceToIndexLocal.x(),0, forceToIndexLocal.z());
        //            }
        //            if(forceToThumbLocal.y()>0){
        //                forceToThumbLocal.set(forceToThumbLocal.x(),0, forceToThumbLocal.z());
        //            }
        //        }

        //        qDebug() << " Index " << forceToIndexLocal.x() << forceToIndexLocal.y() << forceToIndexLocal.z();
        //        qDebug() << " Thumb " << forceToThumbLocal.x() << forceToThumbLocal.y() << forceToThumbLocal.z();

        deviceComputedForce0 = forceToIndexLocal;
        deviceComputedForce1 = forceToThumbLocal;
    }

    // write down the most recent device and world forces for recording
    deviceForceRecord0 << deviceComputedForce0.x(),deviceComputedForce0.y(),deviceComputedForce0.z();
    globalForceRecord0 << computedForce0.x(), computedForce0.y(), computedForce0.z();
    deviceForceRecord1 << deviceComputedForce1.x(),deviceComputedForce1.y(),deviceComputedForce1.z();
    globalForceRecord1 << computedForce1.x(), computedForce1.y(), computedForce1.z();


    if(deviceComputedForce0.length() > 12)
    {
        //        qDebug()<<deviceComputedForce0.x()<<" "<<deviceComputedForce0.y()<<" "<<deviceComputedForce0.z();
        //deviceComputedForce0.set(0,0,0);
    }
    if(deviceComputedForce1.length() > 12)
    {
        //        qDebug()<<deviceComputedForce1.x()<<" "<<deviceComputedForce1.y()<<" "<<deviceComputedForce1.z();
        //deviceComputedForce1.set(0,0,0);
    }
    // filter param
    double fc = 5.0;
    double RC = 1.0/(fc*2.0*PI);
    double alpha = (timeInterval)/(RC + timeInterval);
    // get filtered force
    filteredDeviceForce0 = alpha*deviceComputedForce0 + (1-alpha)*lastFilteredDeviceForce0;
    filteredDeviceForce1 = alpha*deviceComputedForce1 + (1-alpha)*lastFilteredDeviceForce1;

    //convert device "force" to a mapped position
    double forceToPosMult = 0;

    // For the device with normal and tangential directions, stifness values should be different.
    // If direct is 1, the device applies normal force
    // If direct is 0, the device applies shear force

    if (p_CommonData->direct == 1)
        forceToPosMult = p_CommonData->adjustedForceToPosMult_Normal;
    else
        forceToPosMult = p_CommonData->adjustedForceToPosMult_Shear;

    double forceToPosMultThumb = forceToPosMult;

    // Pos movements in delta mechanism frame (index)
    chai3d::cVector3d desiredPosMovement0 = forceToPosMult*(filteredDeviceForce0 + indexImpulse + indexTorqueImpulse); //this is only for lateral if we override normal later
    chai3d::cVector3d desiredPosMovement1 = forceToPosMultThumb*(filteredDeviceForce1 + thumbImpulse + thumbTorqueImpulse); //this is only for lateral if we override normal later

    //qDebug()<<"deviceforce"<<filteredDeviceForce0.x()<<filteredDeviceForce0.y()<<filteredDeviceForce0.z();
    //qDebug()<<"indexImpulse"<<indexImpulse.x()<<indexImpulse.y()<<indexImpulse.z();
    //qDebug()<<"indexTorqueImpulse"<<indexTorqueImpulse.x()<<indexTorqueImpulse.y()<<indexTorqueImpulse.z();

    //<< "indexImpulse" << indexImpulse << "indextorque" << indexTorqueImpulse;

    // don't allow the tactor to move away from finger when computing VR interaction
    double vertPosMovement0 = desiredPosMovement0.z();
    double vertPosMovement1 = desiredPosMovement1.z();
    //if(vertPosMovement0 > 0)
    //    vertPosMovement0 = 0;
    //if(vertPosMovement1 > 0)
    //    vertPosMovement1 = 0;

    Eigen::Vector3d neutralPos0 = p_CommonData->wearableDelta0->neutralPos;
    Eigen::Vector3d desiredPos0(3);
    desiredPos0 << desiredPosMovement0.x()+neutralPos0[0], desiredPosMovement0.y()+neutralPos0[1], vertPosMovement0+neutralPos0[2];
    Eigen::Vector3d neutralPos1 = p_CommonData->wearableDelta1->neutralPos;
    Eigen::Vector3d desiredPos1(3);
    desiredPos1 << desiredPosMovement1.x()+neutralPos1[0], desiredPosMovement1.y()+neutralPos1[1], vertPosMovement1+neutralPos1[2];

    // if the experimental condition is no feedback, tell it to move to neutral pos
    if(p_CommonData->tactileFeedback == 0)
    {
        desiredPos0 << neutralPos0[0], neutralPos0[1], neutralPos0[2];
        desiredPos1 << neutralPos1[0], neutralPos1[1], neutralPos1[2];
    }
    //float radius_no_nan = 4.4;

    //if(sqrt(pow(desiredPos0.x(),2)+pow(desiredPos0.y(),2))>radius_no_nan){
    //    float desired_fixed_angle=atan2(desiredPos0.y(),desiredPos0.x());
    //    desiredPos0 << radius_no_nan*cos(desired_fixed_angle),4.3*sin(desired_fixed_angle),desiredPos0.z();
    //}
    //if(sqrt(pow(desiredPos1.x(),2)+pow(desiredPos1.y(),2))>radius_no_nan){
    //    float desired_fixed_angle=atan2(desiredPos1.y(),desiredPos1.x());
    //    desiredPos1 << radius_no_nan*cos(desired_fixed_angle),radius_no_nan*sin(desired_fixed_angle),desiredPos1.z();
    //}

    // Perform control based on desired position
    p_CommonData->wearableDelta0->SetDesiredPos(desiredPos0);
    p_CommonData->wearableDelta1->SetDesiredPos(desiredPos1);

    p_CommonData->desiredFor0 << filteredDeviceForce0.x(),filteredDeviceForce0.y(),filteredDeviceForce0.z();
    p_CommonData->desiredFor1 << filteredDeviceForce1.x(),filteredDeviceForce1.y(),filteredDeviceForce1.z();


    lastFilteredDeviceForce0 = filteredDeviceForce0;
    lastFilteredDeviceForce1 = filteredDeviceForce1;
}

//The data varibles we want to store:
void haptics_thread::RecordData()
{
    p_CommonData->dataRecordMutex.lock();

    // modified for new experiment, now with both devices
    recordDataCounter = 0;
    p_CommonData->dataRecorder.time = p_CommonData->overallClock.getCurrentTimeSeconds();

    //Choose Check which experiment you are interested in and commnet/uncomment desired parameters
    if(p_CommonData->currentDynamicObjectState == StiffnessExperiment ||
            p_CommonData->currentDynamicObjectState == StiffnessMassExperiment)
    {
        //For sensor0/finger0
        p_CommonData->dataRecorder.desiredPos0      = p_CommonData->wearableDelta0->ReadDesiredPos(); // desired vector for index
        p_CommonData->dataRecorder.strokeOut0       = p_CommonData->wearableDelta0->GetJointAngles();
        p_CommonData->dataRecorder.desiredStroke0   = p_CommonData->wearableDelta0->ReadStrokeOutput();
        p_CommonData->dataRecorder.VRIntForce0      = deviceForceRecord0; // last force on tool0
        p_CommonData->dataRecorder.VRIntForceGlo0   = globalForceRecord0; // last force on tool0 in global coords
        p_CommonData->dataRecorder.magTrackerPos0   = position0;
        p_CommonData->dataRecorder.indexContact    = p_CommonData->fingerTouching;

        //For sensor0/finger1
        p_CommonData->dataRecorder.desiredPos1      = p_CommonData->wearableDelta1->ReadDesiredPos();
        p_CommonData->dataRecorder.strokeOut1       = p_CommonData->wearableDelta1->GetJointAngles();
        p_CommonData->dataRecorder.desiredStroke1   = p_CommonData->wearableDelta1->ReadStrokeOutput();
        p_CommonData->dataRecorder.VRIntForce1      = deviceForceRecord1; // last force on tool0
        p_CommonData->dataRecorder.VRIntForceGlo1   = globalForceRecord1; // last force on tool0 in global coords
        p_CommonData->dataRecorder.magTrackerPos1   = position1;
        p_CommonData->dataRecorder.thumbContact    = p_CommonData->thumbTouching;

        //Box mass/stiffness properties
        p_CommonData->dataRecorder.box1Stiffness    = stiffness1;
        p_CommonData->dataRecorder.box2Stiffness    = stiffness2;
        p_CommonData->dataRecorder.box1Mass         = mass1;
        p_CommonData->dataRecorder.box2Mass         = mass2;
        p_CommonData->dataRecorder.dir              = p_CommonData->direct;

        //Rotation matrices of trackers
        p_CommonData->dataRecorder.deviceRotation0  = rotation0;
        p_CommonData->dataRecorder.deviceRotation1  = rotation1;

        //Box positions
        p_CommonData->dataRecorder.box1Pos          = p_CommonData->ODEBody1->getLocalPos();
        p_CommonData->dataRecorder.box2Pos          = p_CommonData->ODEBody2->getLocalPos();

        //Experiment information
        //p_CommonData->dataRecorder.pairNo           = p_CommonData->pairNo;
        p_CommonData->dataRecorder.conditionNo      = p_CommonData->cond;
        p_CommonData->dataRecorder.strokeScale      = p_CommonData->strokeScale;
        p_CommonData->dataRecorder.success          = p_CommonData->targetSuccess;
        p_CommonData->dataRecorder.box1GlobalRotMat      = p_CommonData->ODEBody1->getGlobalRot();

        //p_CommonData->dataRecorder.compMass         = p_CommonData->compMass;
        p_CommonData->dataRecorder.compCD           = p_CommonData->compCD;
        //p_CommonData->dataRecorder.compInertia      = p_CommonData->compInertia;

        //p_CommonData->dataRecorder.CDVert           = p_CommonData->cdVertScaleOn;

        //p_CommonData->dataRecorder.training         = p_CommonData->trainingOn;
        //p_CommonData->dataRecorder.expMass          = p_CommonData->expMass;
        //p_CommonData->dataRecorder.expCD            = p_CommonData->expCD;
        //p_CommonData->dataRecorder.expInertia       = p_CommonData->expInertia;
    }

    if(p_CommonData->currentDynamicObjectState == FingerMappingExperiment ||
            p_CommonData->currentDynamicObjectState == HoxelMappingExperiment)
    {
        //For sensor0/finger0
        p_CommonData->dataRecorder.desiredPos0      = p_CommonData->wearableDelta0->ReadDesiredPos(); // desired vector for index
        p_CommonData->dataRecorder.strokeOut0       = p_CommonData->wearableDelta0->GetJointAngles(); //for Mine's experiments
        p_CommonData->dataRecorder.desiredStroke0   = p_CommonData->wearableDelta0->ReadStrokeOutput();
        p_CommonData->dataRecorder.VRIntForce0      = deviceForceRecord0; // last force on tool0
        p_CommonData->dataRecorder.VRIntForceGlo0   = globalForceRecord0; // last force on tool0 in global coords
        p_CommonData->dataRecorder.indexContact    = p_CommonData->fingerTouching;

        //For sensor0/finger1
        p_CommonData->dataRecorder.desiredPos1      = p_CommonData->wearableDelta1->ReadDesiredPos();
        p_CommonData->dataRecorder.strokeOut1       = p_CommonData->wearableDelta1->GetJointAngles();
        p_CommonData->dataRecorder.desiredStroke1   = p_CommonData->wearableDelta1->ReadStrokeOutput();
        p_CommonData->dataRecorder.VRIntForce1      = deviceForceRecord1; // last force on tool0
        p_CommonData->dataRecorder.VRIntForceGlo1   = globalForceRecord1; // last force on tool0 in global coords
        p_CommonData->dataRecorder.thumbContact    = p_CommonData->thumbTouching;

        //Box mass/stiffness properties
        p_CommonData->dataRecorder.box1Stiffness    = stiffness1;
        p_CommonData->dataRecorder.box1Mass         = mass1;
        p_CommonData->dataRecorder.dir              = p_CommonData->direct;

        //Rotation matrices of trackers
        p_CommonData->dataRecorder.deviceRotation0  = rotation0;
        p_CommonData->dataRecorder.deviceRotation1  = rotation1;


        //Position vectors of trackers
        p_CommonData->dataRecorder.magTrackerPos0   = position0;
        p_CommonData->dataRecorder.magTrackerPos1   = position1;


        //Box positions
        p_CommonData->dataRecorder.box1Pos          = p_CommonData->ODEBody1->getLocalPos();

        //Experiment information
        //p_CommonData->dataRecorder.pairNo           = p_CommonData->pairNo;
        p_CommonData->dataRecorder.conditionNo      = p_CommonData->cond;
        p_CommonData->dataRecorder.strokeScale      = p_CommonData->strokeScale;
        //p_CommonData->dataRecorder.success          = p_CommonData->targetSuccess; //for Mine's old experiment (not for Jasmin's Jasmin's Experiments)
        p_CommonData->dataRecorder.box1GlobalRotMat      = p_CommonData->ODEBody1->getGlobalRot();
        p_CommonData->dataRecorder.box1LocalRotMat      = p_CommonData->ODEBody1->getLocalRot();

        //p_CommonData->dataRecorder.compMass         = p_CommonData->compMass;
        p_CommonData->dataRecorder.compCD           = p_CommonData->compCD;
        //p_CommonData->dataRecorder.compInertia      = p_CommonData->compInertia;

        //p_CommonData->dataRecorder.CDVert           = p_CommonData->cdVertScaleOn;

        //p_CommonData->dataRecorder.training         = p_CommonData->trainingOn;
        //p_CommonData->dataRecorder.expMass          = p_CommonData->expMass;
        //p_CommonData->dataRecorder.expCD            = p_CommonData->expCD;
        //p_CommonData->dataRecorder.expInertia       = p_CommonData->expInertia;

        //Added variables for Jasmin's Experiments
        p_CommonData->dataRecorder.hoopSuccess        = p_CommonData->hoopSuccess;
        p_CommonData->dataRecorder.targetSuccess      = p_CommonData->targetSuccess;
        p_CommonData->dataRecorder.trialSuccess       = p_CommonData->trialSuccess;
    }
    else if(p_CommonData->currentDynamicObjectState == MultiMassExperiment)
    {
        //For sensor0/finger0
        p_CommonData->dataRecorder.desiredPos0      = p_CommonData->wearableDelta0->ReadDesiredPos(); // desired vector for index
        p_CommonData->dataRecorder.strokeOut0       = p_CommonData->wearableDelta0->GetJointAngles(); //for Mine's experiments
        p_CommonData->dataRecorder.desiredStroke0   = p_CommonData->wearableDelta0->ReadStrokeOutput();
        p_CommonData->dataRecorder.VRIntForce0      = deviceForceRecord0; // last force on tool0
        p_CommonData->dataRecorder.VRIntForceGlo0   = globalForceRecord0; // last force on tool0 in global coords
        p_CommonData->dataRecorder.indexContact     = p_CommonData->fingerTouching;

        //For sensor0/finger1
        p_CommonData->dataRecorder.desiredPos1      = p_CommonData->wearableDelta1->ReadDesiredPos();
        p_CommonData->dataRecorder.strokeOut1       = p_CommonData->wearableDelta1->GetJointAngles();
        p_CommonData->dataRecorder.desiredStroke1   = p_CommonData->wearableDelta1->ReadStrokeOutput();
        p_CommonData->dataRecorder.VRIntForce1      = deviceForceRecord1; // last force on tool0
        p_CommonData->dataRecorder.VRIntForceGlo1   = globalForceRecord1; // last force on tool0 in global coords
        p_CommonData->dataRecorder.thumbContact     = p_CommonData->thumbTouching;

        //Box mass/stiffness properties
        p_CommonData->dataRecorder.box1Stiffness    = stiffness1;
        p_CommonData->dataRecorder.box2Stiffness    = stiffness2;
        p_CommonData->dataRecorder.box3Stiffness    = stiffness3;
        p_CommonData->dataRecorder.box1Mass         = mass1;
        p_CommonData->dataRecorder.box2Mass         = mass2;
        p_CommonData->dataRecorder.box3Mass         = mass3;
        //p_CommonData->dataRecorder.dir              = p_CommonData->direct;

        //Rotation matrices of trackers
        p_CommonData->dataRecorder.deviceRotation0  = rotation0;
        p_CommonData->dataRecorder.deviceRotation1  = rotation1;

        //Position vectors of trackers
        p_CommonData->dataRecorder.magTrackerPos0   = position0;
        p_CommonData->dataRecorder.magTrackerPos1   = position1;

        //Box positions
        p_CommonData->dataRecorder.box1Pos          = p_CommonData->ODEBody1->getLocalPos();
        p_CommonData->dataRecorder.box2Pos          = p_CommonData->ODEBody2->getLocalPos();
        p_CommonData->dataRecorder.box3Pos          = p_CommonData->ODEBody3->getLocalPos();

        //Experiment information
        //p_CommonData->dataRecorder.pairNo           = p_CommonData->pairNo;
        p_CommonData->dataRecorder.conditionNo          = p_CommonData->cond;
        p_CommonData->dataRecorder.strokeScale          = p_CommonData->strokeScale;
        //p_CommonData->dataRecorder.success          = p_CommonData->targetSuccess; //for Mine's old experiment (not for Jasmin's Experiments)
        p_CommonData->dataRecorder.box1GlobalRotMat     = p_CommonData->ODEBody1->getGlobalRot();
        p_CommonData->dataRecorder.box1LocalRotMat      = p_CommonData->ODEBody1->getLocalRot();
        p_CommonData->dataRecorder.box2GlobalRotMat     = p_CommonData->ODEBody2->getGlobalRot();
        p_CommonData->dataRecorder.box2LocalRotMat      = p_CommonData->ODEBody2->getLocalRot();
        p_CommonData->dataRecorder.box3GlobalRotMat     = p_CommonData->ODEBody3->getGlobalRot();
        p_CommonData->dataRecorder.box3LocalRotMat      = p_CommonData->ODEBody3->getLocalRot();

        //p_CommonData->dataRecorder.compMass         = p_CommonData->compMass;
        p_CommonData->dataRecorder.compCD               = p_CommonData->compCD;
        //p_CommonData->dataRecorder.compInertia      = p_CommonData->compInertia;

        //p_CommonData->dataRecorder.CDVert           = p_CommonData->cdVertScaleOn;

        //p_CommonData->dataRecorder.training         = p_CommonData->trainingOn;
        //p_CommonData->dataRecorder.expMass          = p_CommonData->expMass;
        //p_CommonData->dataRecorder.expCD            = p_CommonData->expCD;
        //p_CommonData->dataRecorder.expInertia       = p_CommonData->expInertia;

        //Added variables for Jasmin's Experiments
        //p_CommonData->dataRecorder.hoopSuccess        = p_CommonData->hoopSuccess;
        //p_CommonData->dataRecorder.targetSuccess      = p_CommonData->targetSuccess;
        p_CommonData->dataRecorder.trialSuccess         = p_CommonData->trialSuccess;
    }


#ifdef ACC
    p_CommonData->dataRecorder.Acc = p_CommonData->Acc;
#endif
    //p_CommonData->dataRecorder.isReversal //handled by mainwindow thread manually at end of each trial

    p_CommonData->dataRecorderVector.push_back(p_CommonData->dataRecorder);
    p_CommonData->dataRecordMutex.unlock();
}

void haptics_thread::InitGeneralChaiStuff()
{
    //--------------------------------------------------------------------------
    // WORLD - CAMERA - LIGHTING
    //--------------------------------------------------------------------------
    // Create a new world
    p_CommonData->p_world = new chai3d::cWorld();

    // create a camera and insert it into the virtual world
    p_CommonData->p_world->setBackgroundColor(0, 0, 0);
    p_CommonData->p_world->m_backgroundColor.setWhite();

    // create a camera and insert it into the virtual world
    p_CommonData->p_camera = new chai3d::cCamera(p_CommonData->p_world);
    p_CommonData->p_world->addChild(p_CommonData->p_camera);

    // Position and orientate the camera
    // X is toward camera, pos Y is to right, pos Z is up

    //p_CommonData->cameraPos.set(0.320, 0.0, -0.400);
    p_CommonData->cameraPos.set(0.67, 0.0, -0.58);
    //p_CommonData->cameraPos.set(0.3, -0.3, -0.400);
    p_CommonData->lookatPos.set(0.0, 0.0, 0.0);
    p_CommonData->upVector.set(0.0, 0.0, -1.0);
    p_CommonData->p_camera->set( p_CommonData->cameraPos,    //(0.25, 0.0, -0.25),    // camera position (eye)
                                 p_CommonData->lookatPos,    // lookat position (target)
                                 p_CommonData->upVector);    // direction of the "up" vector

    // set the near and far clipping planes of the camera
    // anything in front/behind these clipping planes will not be rendered
    p_CommonData->p_camera->setClippingPlanes(0.01, 20.0);

#ifndef OCULUS
    // the camera is updated to a position based on these params
    p_CommonData->azimuth = -10.0; //5; //0.0;
    p_CommonData->polar = 115.0; // 140.0; //higher number puts us higher in the air
    //    p_CommonData->camRadius = 0.45;
    //    p_CommonData->camRadius = 0.75;
    p_CommonData->camRadius = 0.8;
#endif

    // create a light source and attach it to the camera
    light = new chai3d::cSpotLight(p_CommonData->p_world);
    p_CommonData->p_world->addChild(light);   // insert light source inside world
    light->setEnabled(true);                   // enable light source
    light->setDir(-0.2, -0.2, 0.5);  // define the direction of the light beam
    light->setLocalPos(0.2, 0.2, -0.5);
    light->setCutOffAngleDeg(120);
}

void haptics_thread::InitFingerAndTool()
{
    //--------------------------------------------------------------------------
    // HAPTIC DEVICES / TOOLS
    //--------------------------------------------------------------------------
    m_tool0 = new chai3d::cToolCursor(p_CommonData->p_world); // create a 3D tool
    p_CommonData->p_world->addChild(m_tool0); //insert the tool into the world
    toolRadius = 0.002; // set tool radius
    m_tool0->setRadius(toolRadius);
    m_tool0->setHapticDevice(p_CommonData->chaiMagDevice0); // connect the haptic device to the tool
    m_tool0->setShowContactPoints(false, false, chai3d::cColorf(0,0,0)); // show proxy and device position of finger-proxy algorithm
    m_tool0->enableDynamicObjects(true);
    m_tool0->setWaitForSmallForce(true);
    m_tool0->start();

    //uncomment this if we want to use 2 tools
    m_tool1 = new chai3d::cToolCursor(p_CommonData->p_world); // create a 3D tool
    p_CommonData->p_world->addChild(m_tool1); //insert the tool into the world
    m_tool1->setRadius(toolRadius);
    m_tool1->setHapticDevice(p_CommonData->chaiMagDevice1); // connect the haptic device to the tool
    m_tool1->setShowContactPoints(false, false, chai3d::cColorf(0,0,0)); // show proxy and device position of finger-proxy algorithm
    m_tool1->enableDynamicObjects(true);
    m_tool1->setWaitForSmallForce(true);
    m_tool1->start();

    // Can use this to show frames on tool if so desired
    //create a sphere to represent the tool
    m_curSphere0 = new chai3d::cShapeSphere(toolRadius);
    p_CommonData->p_world->addChild(m_curSphere0);
    m_curSphere0->m_material->setGrayDarkSlate();
    m_curSphere0->setShowFrame(true);
    m_curSphere0->setFrameSize(0.05);

    m_curSphere1 = new chai3d::cShapeSphere(toolRadius);
    p_CommonData->p_world->addChild(m_curSphere1);
    m_curSphere1->m_material->setBlueAqua();
    m_curSphere1->setShowFrame(true);
    m_curSphere1->setFrameSize(0.05);

    // add display scaled visual representations
    m_dispScaleCurSphere0 = new chai3d::cShapeSphere(toolRadius);
    p_CommonData->p_world->addChild(m_dispScaleCurSphere0);
    m_dispScaleCurSphere0->m_material->setGrayDarkSlate();
    m_dispScaleCurSphere0->setShowFrame(false);
    m_dispScaleCurSphere0->setFrameSize(0.05);

    m_dispScaleCurSphere1 = new chai3d::cShapeSphere(toolRadius);
    p_CommonData->p_world->addChild(m_dispScaleCurSphere1);
    m_dispScaleCurSphere1->m_material->setBlueAqua();
    m_dispScaleCurSphere1->setShowFrame(false);
    m_dispScaleCurSphere1->setFrameSize(0.05);


    //--------------------------------------------------------------------------
    // CREATING OBJECTS
    //--------------------------------------------------------------------------
    // create a finger object
    finger = new chai3d::cMultiMesh(); // create a virtual mesh
    p_CommonData->p_world->addChild(finger); // add object to world
    finger->setShowFrame(false);
    finger->setFrameSize(0.05);
    finger->setLocalPos(0.0, 0.0, 0.0);

    thumb = new chai3d::cMultiMesh(); //create the thumb
    p_CommonData->p_world->addChild(thumb);
    thumb->setShowFrame(false);
    thumb->setFrameSize(0.05);
    thumb->setLocalPos(0.0, 0.0, 0.0);

    // load an object file
    if(cLoadFileOBJ(finger, "./Resources/FingerModel.obj")){
        qDebug() << "finger file loaded";
    }
    if(cLoadFileOBJ(thumb, "./Resources/FingerModelThumb.obj")){
        qDebug() << "thumb file loaded";
    }

    // set params for finger
    finger->setShowEnabled(true);
    finger->setUseVertexColors(true);
    chai3d::cColorf fingerColor;
    fingerColor.setBrownSandy();
    finger->setVertexColor(fingerColor);
    //    finger->m_material->m_ambient.set(0.1, 0.1, 0.1);
    //    finger->m_material->m_diffuse.set(0.3, 0.3, 0.3);
    finger->m_material->m_specular.set(1.0, 1.0, 1.0);
    finger->setUseMaterial(true);
    finger->setHapticEnabled(false);

    // set params for thumb
    thumb->setShowEnabled(true);
    thumb->setUseVertexColors(true);
    chai3d::cColorf thumbColor;
    thumbColor.setBrownSandy();
    thumb->setVertexColor(thumbColor);
    //    thumb->m_material->m_ambient.set(0.1, 0.1, 0.1);
    //    thumb->m_material->m_diffuse.set(0.3, 0.3, 0.3);
    thumb->m_material->m_specular.set(1.0, 1.0, 1.0);
    thumb->setUseMaterial(true);
    thumb->setHapticEnabled(false);

    ////////////////////////////////////////////
    // CREATE POSITION SCALED OBJECTS
    /////////////////////////////////////////////
    scaledFinger = new chai3d::cMultiMesh(); // create a virtual mesh
    p_CommonData->p_world->addChild(scaledFinger); // add object to world
    scaledFinger->setShowFrame(false);
    scaledFinger->setFrameSize(0.05);
    scaledFinger->setLocalPos(0.0, 0.0, 0.0);

    scaledThumb = new chai3d::cMultiMesh(); //create the scaledThumb
    p_CommonData->p_world->addChild(scaledThumb);
    scaledThumb->setShowFrame(false);
    scaledThumb->setFrameSize(0.05);
    scaledThumb->setLocalPos(0.0, 0.0, 0.0);

    if(cLoadFileOBJ(scaledFinger, "./Resources/FingerModel.obj")){
        qDebug() << "finger file loaded";
    }
    if(cLoadFileOBJ(scaledThumb, "./Resources/FingerModelThumb.obj")){
        qDebug() << "thumb file loaded";
    }

    // set params for scaledFinger
    scaledFinger->setShowEnabled(true);
    scaledFinger->setUseVertexColors(true);
    chai3d::cColorf scaledFingerColor;
    scaledFingerColor.setBrownSandy();
    scaledFinger->setVertexColor(scaledFingerColor);
    scaledFinger->m_material->m_ambient.set(0.1, 0.1, 0.1);
    scaledFinger->m_material->m_diffuse.set(0.3, 0.3, 0.3);
    scaledFinger->m_material->m_specular.set(1.0, 1.0, 1.0);
    scaledFinger->setUseMaterial(true);
    scaledFinger->setHapticEnabled(false);

    // set params for scaledThumb
    scaledThumb->setShowEnabled(true);
    scaledThumb->setUseVertexColors(true);
    chai3d::cColorf scaledThumbColor;
    scaledThumbColor.setBrownSandy();
    scaledThumb->setVertexColor(scaledThumbColor);
    scaledThumb->m_material->m_ambient.set(0.1, 0.1, 0.1);
    scaledThumb->m_material->m_diffuse.set(0.3, 0.3, 0.3);
    scaledThumb->m_material->m_specular.set(1.0, 1.0, 1.0);
    scaledThumb->setUseMaterial(true);
    scaledThumb->setHapticEnabled(false);
}

void haptics_thread::InitEnvironments()
{
    //    p_CommonData->p_tissueOne = new chai3d::cMultiMesh();
    //    p_CommonData->p_tissueTwo = new chai3d::cMultiMesh();
    //    p_CommonData->p_tissueThree = new chai3d::cMultiMesh();
    //    p_CommonData->p_tissueFour = new chai3d::cMultiMesh();
    //    p_CommonData->p_tissueFive = new chai3d::cMultiMesh();
    //    p_CommonData->p_tissueSix = new chai3d::cMultiMesh();
    //    p_CommonData->p_tissueSeven = new chai3d::cMultiMesh();
    //    p_CommonData->p_tissueEight = new chai3d::cMultiMesh();
    //    p_CommonData->p_tissueNine = new chai3d::cMultiMesh();
    //    p_CommonData->p_tissueTen = new chai3d::cMultiMesh();
    //    p_CommonData->p_tissueEleven = new chai3d::cMultiMesh();
    //    p_CommonData->p_tissueTwelve = new chai3d::cMultiMesh();
    p_CommonData->p_indicator = new chai3d::cMultiMesh();
    //    p_CommonData->p_tissueOne->rotateAboutLocalAxisDeg(1,0,0,180);
    //    p_CommonData->p_tissueTwo->rotateAboutLocalAxisDeg(1,0,0,180);
    //    p_CommonData->p_tissueThree->rotateAboutLocalAxisDeg(1,0,0,180);
    //    p_CommonData->p_tissueFour->rotateAboutLocalAxisDeg(1,0,0,180);
    //    p_CommonData->p_tissueFive->rotateAboutLocalAxisDeg(1,0,0,180);
    //    p_CommonData->p_tissueSix->rotateAboutLocalAxisDeg(1,0,0,180);
    //    p_CommonData->p_tissueSeven->rotateAboutLocalAxisDeg(1,0,0,180);
    //    p_CommonData->p_tissueEight->rotateAboutLocalAxisDeg(1,0,0,180);
    //    p_CommonData->p_tissueNine->rotateAboutLocalAxisDeg(1,0,0,180);
    //    p_CommonData->p_tissueTen->rotateAboutLocalAxisDeg(1,0,0,180);
    //    p_CommonData->p_tissueEleven->rotateAboutLocalAxisDeg(1,0,0,180);
    //    p_CommonData->p_tissueTwelve->rotateAboutLocalAxisDeg(1,0,0,180);
    p_CommonData->p_indicator->rotateAboutLocalAxisDeg(1,0,0,180);
    p_CommonData->p_expFrictionBox = new chai3d::cMesh();
}

//Must add new body here or else environment *will* crash
void haptics_thread::InitDynamicBodies()
{
    //--------------------------------------------------------------------------
    // CREATING ODE World and Objects
    //--------------------------------------------------------------------------
    // create an ODE world to simulate dynamic bodies
    ODEWorld = new cODEWorld(p_CommonData->p_world);

    // Create the ODE objects for the blocks and cup
    p_CommonData->ODEBody1 = new cODEGenericBody(ODEWorld);
    p_CommonData->ODEAdjustBody = new cODEGenericBody(ODEWorld);
    p_CommonData->ODEAdjustBody1 = new cODEGenericBody(ODEWorld);
    p_CommonData->ODEBody2 = new cODEGenericBody(ODEWorld);
    p_CommonData->ODEBody3 = new cODEGenericBody(ODEWorld);
    //p_CommonData->ODEBody4 = new cODEGenericBody(ODEWorld);
    p_CommonData->ODEHoop1 = new cODEGenericBody(ODEWorld); // added for HME
    p_CommonData->ODEHoop2 = new cODEGenericBody(ODEWorld); // added for HME
    p_CommonData->ODEWall = new cODEGenericBody(ODEWorld); // added for HME

    // create a virtual mesh that will be used for the geometry representation of the dynamic body
    p_CommonData->p_dynamicBox1 = new chai3d::cMesh();
    p_CommonData->p_dynamicBox2 = new chai3d::cMesh();
    p_CommonData->p_dynamicBox3 = new chai3d::cMesh();
    //p_CommonData->p_dynamicBox4 = new chai3d::cMesh();
    p_CommonData->adjustBox = new chai3d::cMesh();
    p_CommonData->adjustBox1 = new chai3d::cMesh();
    p_CommonData->p_wall = new chai3d::cMesh();
    p_CommonData->p_dynamicHoop1 = new chai3d::cMesh(); // added for HME
    p_CommonData->p_dynamicHoop2 = new chai3d::cMesh(); // added for HME
    p_CommonData->p_boxWithHole = new chai3d::cMultiMesh(); // added for HME

    // create the scaled virtual objects
    p_CommonData->p_dynamicScaledBox1 = new chai3d::cMesh();
    //p_CommonData->p_dynamicScaledBox2 = new chai3d::cMesh();
    //p_CommonData->p_dynamicScaledBox3 = new chai3d::cMesh();
    //p_CommonData->p_dynamicScaledBox4 = new chai3d::cMesh();

    //--------------------------------------------------------------------------
    // CREATING ODE INVISIBLE WALLS
    //--------------------------------------------------------------------------
    ODEGPlane0 = new cODEGenericBody(ODEWorld);

    //create ground
    ground = new chai3d::cMesh();
    wall = new chai3d::cMesh();

    //create background mesh
    globe = new chai3d::cMesh();

    ////////////////////////////////////////////
    // CREATE 1 and 2 MODELS FOR EXPERIMENT DISPLAY
    /////////////////////////////////////////////
    //init one and two meshes
    //    p_CommonData->oneModel = new chai3d::cMultiMesh();
    //    p_CommonData->twoModel = new chai3d::cMultiMesh();
    p_CommonData->p_world->addChild(p_CommonData->oneModel); // add object to world
    p_CommonData->p_world->addChild(p_CommonData->twoModel); // add object to world
    //    p_CommonData->oneModel->setShowEnabled(false);
    //    p_CommonData->twoModel->setShowEnabled(false);

    // load an object file
    //    if(cLoadFileOBJ(p_CommonData->oneModel, "./Resources/One.obj")){
    //        //qDebug() << "'One' loaded";
    //    }
    //    if(cLoadFileOBJ(p_CommonData->twoModel, "./Resources/Two.obj")){
    //        //qDebug() << "'Two' loaded";
    //    }

    //    p_CommonData->oneModel->setUseVertexColors(true);
    //    chai3d::cColorf oneColor;
    //    oneColor.setRedCrimson();
    //    p_CommonData->oneModel->setVertexColor(oneColor);

    //    p_CommonData->twoModel->setUseVertexColors(true);
    //    chai3d::cColorf twoColor;
    //    twoColor.setRedCrimson();
    //    p_CommonData->twoModel->setVertexColor(twoColor);

    //    p_CommonData->oneModel->rotateAboutLocalAxisDeg(chai3d::cVector3d(0,0,1), -90);
    //    p_CommonData->twoModel->rotateAboutLocalAxisDeg(chai3d::cVector3d(0,0,1), -90);
    //    p_CommonData->oneModel->rotateAboutLocalAxisDeg(chai3d::cVector3d(1,0,0), -90);
    //    p_CommonData->twoModel->rotateAboutLocalAxisDeg(chai3d::cVector3d(1,0,0), -90);

    //Define lines for showforces
    chai3d::cColorf LineColor;
    LineColor.setGreenLawn();
    force1_show = new chai3d::cShapeLine(chai3d::cVector3d(0,0,0),
                                         chai3d::cVector3d(0,0,0));
    force2_show = new chai3d::cShapeLine(chai3d::cVector3d(0,0,0),
                                         chai3d::cVector3d(0,0,0));
    lastforce1 = chai3d::cVector3d(0,0,0);
    lastforce2 = chai3d::cVector3d(0,0,0);
    force1_show->setLineWidth(5);
    force2_show->setLineWidth(5);

    force1_show->m_colorPointA=LineColor;
    force1_show->m_colorPointB=LineColor;
    LineColor.setBlueSky();
    force2_show->m_colorPointA=LineColor;
    force2_show->m_colorPointB=LineColor;
    p_CommonData->p_world->addChild(force1_show);
    p_CommonData->p_world->addChild(force2_show);
}

void haptics_thread::DeleteDynamicBodies()
{
    //For Mine's Experiemnts
    if(p_CommonData->currentDynamicObjectState == StiffnessExperiment ||
            p_CommonData->currentDynamicObjectState == StiffnessMassExperiment)
    {
        //    qDebug("is this happening");
        delete ODEWorld;
        //    delete p_CommonData->oneModel;
        //    qDebug("is this happening 2");
        //    delete p_CommonData->twoModel;
        delete p_CommonData->ODEAdjustBody;
        delete p_CommonData->ODEAdjustBody1;
        //    qDebug("is this happening 3");
        delete p_CommonData->ODEBody1;
        delete p_CommonData->ODEBody2;
        //    delete p_CommonData->ODEBody3;
        //    delete p_CommonData->ODEBody4;
        //    qDebug("is this happening 4");
        delete p_CommonData->adjustBox;
        delete p_CommonData->adjustBox1;
        delete p_CommonData->p_dynamicBox1;
        delete p_CommonData->p_dynamicScaledBox1;
        delete p_CommonData->p_dynamicBox2;
        //    delete p_CommonData->p_dynamicBox3;
        delete p_CommonData->p_dynamicBox4;
        //    qDebug("is this happening 5");

        delete wall;
        delete hoop1;
        delete hoop2;
        //    delete hoop3;
        //    delete hoop4;
        //    qDebug("is this happening 6");

        delete target1;
        delete target2;
        //    delete target3;
        //    qDebug("is this happening 7");

        delete p_CommonData->p_wall;
        delete ODEGPlane0;
        delete ground;
        delete Right_Platform;
        delete Left_Platform;
        delete globe;

        //    qDebug("is this happening 5");

        p_CommonData->p_world->removeChild(p_CommonData->p_dynamicBox1);
        p_CommonData->p_world->removeChild(p_CommonData->p_dynamicScaledBox1);
        p_CommonData->p_world->removeChild(ODEWorld);
        p_CommonData->p_world->removeChild(ground);
        p_CommonData->p_world->removeChild(wall);
        p_CommonData->p_world->removeChild(hoop1);
        p_CommonData->p_world->removeChild(hoop2);
        p_CommonData->p_world->removeChild(hoop3);
        p_CommonData->p_world->removeChild(hoop4);
        p_CommonData->p_world->removeChild(target1);
        p_CommonData->p_world->removeChild(target2);
        p_CommonData->p_world->removeChild(target3);
        p_CommonData->p_world->removeChild(Right_Platform);
        p_CommonData->p_world->removeChild(Left_Platform);
        p_CommonData->p_world->removeChild(m_tool0);
        p_CommonData->p_world->removeChild(m_tool1);
        p_CommonData->p_world->removeChild(finger);
        p_CommonData->p_world->removeChild(thumb);
        p_CommonData->p_world->removeChild(globe);
        p_CommonData->p_world->removeChild(force1_show);
        p_CommonData->p_world->removeChild(force1_show);

        //    qDebug("is this happening 6");


        // add scaled bodies for altering display ratio
        p_CommonData->p_world->removeChild(m_dispScaleCurSphere0);
        p_CommonData->p_world->removeChild(m_dispScaleCurSphere1);
        p_CommonData->p_world->removeChild(scaledFinger);
        p_CommonData->p_world->removeChild(scaledThumb);
        //    qDebug("it just happened!");
    }
    //For Jasmin's Experiments
    if (p_CommonData->currentDynamicObjectState == FingerMappingExperiment)
    {
        delete ODEWorld;
        //delete p_CommonData->oneModel;
        //delete p_CommonData->twoModel;
        delete p_CommonData->ODEAdjustBody;
        delete p_CommonData->ODEAdjustBody1;

        delete p_CommonData->ODEBody1;
        //delete p_CommonData->ODEBody2;
        //delete p_CommonData->ODEBody3;
        //delete p_CommonData->ODEBody4;

        delete p_CommonData->adjustBox;
        delete p_CommonData->adjustBox1;
        delete p_CommonData->p_dynamicBox1;
        //delete p_CommonData->p_dynamicScaledBox1;
        //delete p_CommonData->p_dynamicBox2;
        //delete p_CommonData->p_dynamicBox3;
        //delete p_CommonData->p_dynamicBox4;

        delete wall;
        delete hoop1;
        //delete hoop2;
        //delete hoop3;
        //delete hoop4;

        delete target1;
        //delete target2;
        //delete target3;
        //qDebug("is this happening 7");

        delete p_CommonData->p_wall;
        delete ODEGPlane0;
        delete ground;
        delete Right_Platform;
        delete Left_Platform;
        delete globe;

        p_CommonData->p_world->removeChild(p_CommonData->p_dynamicBox1);
        p_CommonData->p_world->removeChild(p_CommonData->p_dynamicScaledBox1);
        p_CommonData->p_world->removeChild(ODEWorld);
        p_CommonData->p_world->removeChild(ground);
        p_CommonData->p_world->removeChild(wall);
        p_CommonData->p_world->removeChild(hoop1);
        //p_CommonData->p_world->removeChild(hoop2);
        p_CommonData->p_world->removeChild(hoop3);
        p_CommonData->p_world->removeChild(hoop4);
        p_CommonData->p_world->removeChild(target1);
        //p_CommonData->p_world->removeChild(target2);
        p_CommonData->p_world->removeChild(target3);
        p_CommonData->p_world->removeChild(Right_Platform);
        p_CommonData->p_world->removeChild(Left_Platform);
        p_CommonData->p_world->removeChild(m_tool0);
        p_CommonData->p_world->removeChild(m_tool1);
        p_CommonData->p_world->removeChild(finger);
        p_CommonData->p_world->removeChild(thumb);
        p_CommonData->p_world->removeChild(globe);
        p_CommonData->p_world->removeChild(force1_show);
        p_CommonData->p_world->removeChild(force1_show);

        // add scaled bodies for altering display ratio
        p_CommonData->p_world->removeChild(m_dispScaleCurSphere0);
        p_CommonData->p_world->removeChild(m_dispScaleCurSphere1);
        p_CommonData->p_world->removeChild(scaledFinger);
        p_CommonData->p_world->removeChild(scaledThumb);
    }
    if (p_CommonData->currentDynamicObjectState == HoxelMappingExperiment)
    {
        delete ODEWorld;
        delete p_CommonData->ODEAdjustBody;
        delete p_CommonData->ODEAdjustBody1;

        delete p_CommonData->ODEBody1;
        delete p_CommonData->ODEBody2;
        //delete p_CommonData->ODEBody3;
        //delete p_CommonData->ODEBody4;

        delete p_CommonData->adjustBox;
        delete p_CommonData->adjustBox1;
        delete p_CommonData->p_dynamicBox1;
        delete p_CommonData->p_dynamicBox2;
        delete p_CommonData->p_boxWithHole;

        delete wall;
        delete hoop1;
        delete target1;

        delete p_CommonData->p_wall;
        delete ODEGPlane0;
        delete ground;
        delete Right_Platform;
        delete Left_Platform;
        delete globe;

        p_CommonData->p_world->removeChild(p_CommonData->p_dynamicBox1);
        p_CommonData->p_world->removeChild(p_CommonData->p_boxWithHole);
        p_CommonData->p_world->removeChild(p_CommonData->p_dynamicScaledBox1);
        p_CommonData->p_world->removeChild(ODEWorld);
        p_CommonData->p_world->removeChild(ground);
        p_CommonData->p_world->removeChild(wall);
        p_CommonData->p_world->removeChild(hoop1);
        //p_CommonData->p_world->removeChild(hoop2);
        p_CommonData->p_world->removeChild(hoop3);
        p_CommonData->p_world->removeChild(hoop4);
        p_CommonData->p_world->removeChild(target1);
        p_CommonData->p_world->removeChild(m_tool0);
        p_CommonData->p_world->removeChild(m_tool1);
        p_CommonData->p_world->removeChild(finger);
        p_CommonData->p_world->removeChild(thumb);
        p_CommonData->p_world->removeChild(globe);
        p_CommonData->p_world->removeChild(force1_show);
        p_CommonData->p_world->removeChild(force1_show);

        // add scaled bodies for altering display ratio
        p_CommonData->p_world->removeChild(m_dispScaleCurSphere0);
        p_CommonData->p_world->removeChild(m_dispScaleCurSphere1);
        p_CommonData->p_world->removeChild(scaledFinger);
        p_CommonData->p_world->removeChild(scaledThumb);
    }
    else if (p_CommonData->currentDynamicObjectState == MultiMassExperiment)
    {
        delete ODEWorld;
        delete p_CommonData->ODEAdjustBody;
        delete p_CommonData->ODEAdjustBody1;

        delete p_CommonData->ODEBody1;
        delete p_CommonData->ODEBody2;
        delete p_CommonData->ODEBody3;
        //delete p_CommonData->ODEBody4;

        delete p_CommonData->adjustBox;
        delete p_CommonData->adjustBox1;
        delete p_CommonData->p_dynamicBox1;
        delete p_CommonData->p_dynamicBox2;
        delete p_CommonData->p_dynamicBox3;

        delete ODEGPlane0;
        delete ground;
        delete Right_Platform;
        delete Left_Platform;
        delete globe;

        p_CommonData->p_world->removeChild(p_CommonData->p_dynamicBox1);
        p_CommonData->p_world->removeChild(p_CommonData->p_dynamicBox2);
        p_CommonData->p_world->removeChild(p_CommonData->p_dynamicBox3);
        p_CommonData->p_world->removeChild(p_CommonData->p_dynamicScaledBox1);
        p_CommonData->p_world->removeChild(ODEWorld);
        p_CommonData->p_world->removeChild(ground);
        p_CommonData->p_world->removeChild(wall);
        p_CommonData->p_world->removeChild(hoop1);
        //p_CommonData->p_world->removeChild(hoop2);
        p_CommonData->p_world->removeChild(hoop3);
        p_CommonData->p_world->removeChild(hoop4);
        p_CommonData->p_world->removeChild(target1);
        p_CommonData->p_world->removeChild(m_tool0);
        p_CommonData->p_world->removeChild(m_tool1);
        p_CommonData->p_world->removeChild(finger);
        p_CommonData->p_world->removeChild(thumb);
        p_CommonData->p_world->removeChild(globe);
        p_CommonData->p_world->removeChild(force1_show);
        p_CommonData->p_world->removeChild(force1_show);

        // add scaled bodies for altering display ratio
        p_CommonData->p_world->removeChild(m_dispScaleCurSphere0);
        p_CommonData->p_world->removeChild(m_dispScaleCurSphere1);
        p_CommonData->p_world->removeChild(scaledFinger);
        p_CommonData->p_world->removeChild(scaledThumb);
    }
}

void haptics_thread::RenderDynamicBodies()
{
    DeleteDynamicBodies();
    //Sleep(10);
    InitDynamicBodies();
    ODEWorld->deleteAllChildren();

    //----------------------------- ---------------------------------------------
    // CREATING ODE World and Objects
    //--------------------------------------------------------------------------

    // create an ODE world to simulate dynamic bodies
    p_CommonData->p_world->addChild(ODEWorld);

    // give world gravity
    //if (p_CommonData->currentDynamicObjectState == dynamicInertiaExp || p_CommonData->currentDynamicObjectState == dynamicSubjectiveExp || p_CommonData->currentDynamicObjectState == dynamicCDInertiaExp){
    //    ODEWorld->setGravity(chai3d::cVector3d(0.0, 0.0, 0));
    //}
    //else {
    ODEWorld->setGravity(chai3d::cVector3d(0.0, 0.0, 9.81));
    //    }
    // define damping properties
    ODEWorld->setAngularDamping(0.02);
    ODEWorld->setLinearDamping(0.007);

    //create a plane
    groundSize = 0.3;
    groundThickness = 0.01;
    //--------------------------------------------------------------------------
    // CREATING ODE INVISIBLE WALLS
    //--------------------------------------------------------------------------
    ODEGPlane0->createStaticPlane(chai3d::cVector3d(0.0, 0.0, 0.0), chai3d::cVector3d(0.0, 0.0 ,-1.0));

    chai3d::cCreateBox(ground, groundSize*1.75, 2.5*groundSize, groundThickness);
    ground->setLocalPos(0.05, 0.1, groundThickness*0.5);

    if(p_CommonData->currentDynamicObjectState == StiffnessMassExperiment)
    {
        wallHeight = 0.1;
        wallThickness = 0.01;

        chai3d::cCreateBox(wall, 1.75*0.3, wallThickness, wallHeight);
        wall->setLocalPos(0.05, 0.05, -0.05);
    }

    if(p_CommonData->currentDynamicObjectState == FingerMappingExperiment)
    {
        wallHeight = 0.1;
        wallThickness = 0.01;

        chai3d::cCreateBox(wall, 1.75*0.3, wallThickness, wallHeight);
        wall->setLocalPos(0.05, 0.085, -0.05);
    }

    if(p_CommonData->currentDynamicObjectState == HoxelMappingExperiment)
    {
        wallHeight = 0.1;
        wallThickness = 0.01;

        chai3d::cCreateBox(wall, 1.75*0.3, wallThickness, wallHeight);
        wall->setLocalPos(0.05, 0.085, -0.05);
    }

    //create globe
    chai3d::cCreateSphere(globe, 30, 30, 30); //(globe, 10, 30, 30);
    globe->setUseDisplayList(true);
    globe->deleteCollisionDetector();
    globe->setShowFrame(true); //Show globe frame
    chai3d::cMaterial matGlobe;
    matGlobe.setWhite();
    globe->setMaterial(matGlobe);

    //create a texture
    //textureSpace = chai3d::cTexture2d::create();
    //textureSpace->loadFromFile("./Resources/sky.jpg");

    //globe->setTexture(textureSpace, true); //apply texture to globe
    //globe->setUseCulling(false, true);     // Since we don't need to see our polygons from both sides, we enable culling.

    // define some material properties for ground
    chai3d::cMaterial matGround;
    chai3d::cMaterial matWall;
    matGround.setBrownSandy();
    matWall.setBrownSandy();
    ground->setMaterial(matGround);
    wall->setMaterial(matWall);

    // choose which type of dynamic object environment to render
    switch(p_CommonData->currentDynamicObjectState)
    {
    case standard:
        boxSize1 = 0.04; boxSize2 = 0.04; boxSize3 = 0.04;
        friction1 = 2; friction2 = 2; friction3 = 2; //EXPERIMENTFRICTION;
        mass1 = 0.2; mass2 = 0.2; mass3 = 0.2;
        //stiffness1 = 100; stiffness2 = 300; stiffness3 = 700;
        stiffness1 =  p_CommonData->stiffness1; stiffness2 =  p_CommonData->stiffness2; stiffness3 = p_CommonData->stiffness3;
        break;
    case StiffnessExperiment:  // Mine Stiffness Experiment
        boxSize1 = 0.04; boxSize2 = 0.04; boxSize3 = 0.04;
        friction1 = EXPERIMENTFRICTION; friction2 = EXPERIMENTFRICTION; friction3 = EXPERIMENTFRICTION;
        mass1 = 0.2; mass2 = 0.2; mass3 = 0.2;
        stiffness1 =  p_CommonData->stiffness1; stiffness2 =  p_CommonData->stiffness2; stiffness3 = 500;
        break;
    case StiffnessMassExperiment:  // Mine Stiffness-Mass Experiment
        boxSize1 = 0.04; boxSize2 = 0.04; boxSize3 = 0.04;
        friction1 = EXPERIMENTFRICTION; friction2 = EXPERIMENTFRICTION; friction3 = EXPERIMENTFRICTION;
        mass1 = p_CommonData->mass1; mass2 = p_CommonData->mass2; mass3 = p_CommonData->mass3;
        stiffness1 =  p_CommonData->stiffness1; stiffness2 =  p_CommonData->stiffness2; stiffness3 = 500;
        break;
    case FingerMappingExperiment:  // Jasmin FingerMapping Experiment
        boxSize1 = 0.04; boxSize2 = 0.04; boxSize3 = 0.04;
        friction1 = EXPERIMENTFRICTION; friction2 = EXPERIMENTFRICTION; friction3 = EXPERIMENTFRICTION;
        mass1 = p_CommonData->mass1; mass2 = p_CommonData->mass2; mass3 = p_CommonData->mass3;
        stiffness1 =  p_CommonData->stiffness1; stiffness2 =  p_CommonData->stiffness2; stiffness3 = p_CommonData->stiffness3;
        break;
    case HoxelMappingExperiment:  // Jasmin HoxelMapping Experiment
        boxSize1 = 0.04; boxSize2 = 0.04; boxSize3 = 0.04;
        friction1 = EXPERIMENTFRICTION; friction2 = EXPERIMENTFRICTION; friction3 = EXPERIMENTFRICTION;        
        mass1 = p_CommonData->mass1; mass2 = p_CommonData->mass2; mass3 = p_CommonData->mass3;
        stiffness1 =  p_CommonData->stiffness1; stiffness2 =  p_CommonData->stiffness2; stiffness3 = p_CommonData->stiffness3;
        break;
    case MultiMassExperiment:  // Jasmin MultiMass Experiment
        boxSize1 = 0.04; boxSize2 = 0.04; boxSize3 = 0.04;
        friction1 = EXPERIMENTFRICTION; friction2 = EXPERIMENTFRICTION; friction3 = EXPERIMENTFRICTION;
        mass1 = p_CommonData->mass1; mass2 = p_CommonData->mass2; mass3 = p_CommonData->mass3;
        stiffness1 =  p_CommonData->stiffness1; stiffness2 =  p_CommonData->stiffness2; stiffness3 = p_CommonData->stiffness3;
        break;
    }

    //assign the params dependent on the others
    latStiffness1 = stiffness1*1.5;
    latStiffness2 = stiffness2*1.5;
    latStiffness3 = stiffness3*1.5;

    dynFriction1 = 0.9*friction1;
    dynFriction2 = 0.9*friction2;
    dynFriction3 = 0.9*friction3;

    if(p_CommonData->currentDynamicObjectState == StiffnessExperiment)
    {
        SetDynEnvironCDInertiaExp(); //tiger - using same environment for this part
    }

    if(p_CommonData->currentDynamicObjectState == StiffnessMassExperiment)
    {
        SetDynEnvironStiffMassExp();
    }

    if(p_CommonData->currentDynamicObjectState == FingerMappingExperiment)
    {
        SetDynEnvironFingerMappingExp();
    }

    if(p_CommonData->currentDynamicObjectState == HoxelMappingExperiment)
    {
        SetDynEnvironHoxelMappingExp();
    }

    if(p_CommonData->currentDynamicObjectState == MultiMassExperiment)
    {
        SetDynEnvironMultiMassExp();
    }

    // if just rendering dynamic environments without an experiment
    if (p_CommonData->currentDynamicObjectState == standard)
    {
        SetDynEnvironAdjust();
    }

    else if (p_CommonData->currentDynamicObjectState == manual)
    {
        SetManualAdjust();
    }

    //set position of backgroundObject
    globe->setLocalPos(0.0, 0.0, 0.0);

    // setup tools for dynamic interaction
    m_tool0->enableDynamicObjects(true);
    m_tool1->enableDynamicObjects(true);

    //add objects to world
    p_CommonData->p_world->addChild(ODEWorld);
    p_CommonData->p_world->addChild(ground);
    p_CommonData->p_world->addChild(m_tool0);
    p_CommonData->p_world->addChild(m_tool1);
    p_CommonData->p_world->addChild(finger);
    p_CommonData->p_world->addChild(thumb);
    p_CommonData->p_world->addChild(globe);
    p_CommonData->p_world->addChild(m_curSphere0);
    p_CommonData->p_world->addChild(m_curSphere1);

    // add scaled bodies for altering display ratio
    m_dispScaleCurSphere0->setHapticEnabled(false);
    m_dispScaleCurSphere1->setHapticEnabled(false);
    scaledFinger->setHapticEnabled(false);
    scaledThumb->setHapticEnabled(false);
    p_CommonData->p_world->addChild(m_dispScaleCurSphere0);
    p_CommonData->p_world->addChild(m_dispScaleCurSphere1);
    p_CommonData->p_world->addChild(scaledFinger);
    p_CommonData->p_world->addChild(scaledThumb);

    p_CommonData->clutchedOffset.set(0,0,0);
    p_CommonData->fingerScalePoint.set(0,0,0);

    m_tool0->setTransparencyLevel(0);
    m_tool1->setTransparencyLevel(0);
    m_curSphere0->setTransparencyLevel(0);
    m_curSphere1->setTransparencyLevel(0);
    m_dispScaleCurSphere0->setTransparencyLevel(0);
    m_dispScaleCurSphere1->setTransparencyLevel(0);
}

/*Change Environment for each particular setup*/
//tiger cd inertia function
void haptics_thread::SetDynEnvironCDInertiaExp()   // Mine Stiffness Experiment
{
    // set part 1/2 indicators to be able to show
    //    p_CommonData->oneModel->setShowEnabled(false);
    //    p_CommonData->twoModel->setShowEnabled(false);

    // create the visual boxes on the dynamicbox meshes
    //    cCreateBox(p_CommonData->p_dynamicBox1, boxSize1*p_CommonData->expCD, boxSize1*p_CommonData->expCD, boxSize1*p_CommonData->expCD); // make mesh a box
    cCreateBox(p_CommonData->p_dynamicBox1, boxSize1, boxSize1, boxSize1); // make mesh a box

    // create the visual for the position scaled dynamic boxes
    cCreateBox(p_CommonData->p_dynamicScaledBox1, boxSize1, boxSize1, boxSize3); // make mesh a box

    // setup collision detectorsfor the dynamic objects
    p_CommonData->p_dynamicBox1->createAABBCollisionDetector(toolRadius);

    // define material properties for box 1 - invisible
    chai3d::cMaterial mat1;
    mat1.setBlueMediumTurquoise();
    mat1.setStiffness(stiffness1);
    //    mat1.setLateralStiffness(latStiffness1);
    mat1.setDynamicFriction(dynFriction1);
    mat1.setStaticFriction(friction1);
    mat1.setUseHapticFriction(true);
    p_CommonData->p_dynamicBox1->setMaterial(mat1);
    p_CommonData->p_dynamicBox1->setUseMaterial(true);


    // define material properties for box 2 - visible
    chai3d::cMaterial mat2;
    //    if (p_CommonData->pairNo == 0)
    //    {
    //        mat2.setBlueMediumTurquoise();
    //    }
    //    else if (p_CommonData->pairNo==1){
    //      mat2.setBlueMediumTurquoise();
    //    }
    //    else{
    //      qDebug()<<p_CommonData->pairNo;
    mat2.setBlueMediumTurquoise();
    //    }

    p_CommonData->p_dynamicScaledBox1->setMaterial(mat2);
    p_CommonData->p_dynamicScaledBox1->setUseMaterial(true);
    p_CommonData->p_dynamicScaledBox1->setHapticEnabled(false);

    // add mesh to ODE object
    p_CommonData->ODEBody1->setImageModel(p_CommonData->p_dynamicBox1);

    // create a dynamic model of the ODE object
    p_CommonData->ODEBody1->createDynamicBox(boxSize1, boxSize1, boxSize3);

    // set mass of box
    //p_CommonData->ODEBody1->setMass(p_CommonData->expMass);
    p_CommonData->ODEBody1->setMass(mass1);

    //qDebug()<<"ugh"<<p_CommonData->expMass<<" "<< p_CommonData->tempCD << " " << p_CommonData->expInertia<< p_CommonData->cdVertScaleOn;
    //qDebug()<<"further ugh" << mat1.getStiffness()<< mat1.getLateralStiffness()<< mat1.getDynamicFriction()<<mat1.getStaticFriction();
    //p_CommonData->CDInertial_Scaling_Factor = 1; //resets to one everytime
    //p_CommonData->expCD=1; // resets to one everytime - we reset to one initialization

    // set position of box
    //p_CommonData->ODEBody1->setLocalPos(0.25,0,-.1);; //(p_CommonData->box1InitPos);


    // create the visual boxes on the dynamicbox meshes
    //cCreateBox(p_CommonData->p_dynamicBox1, boxSize1, boxSize1, boxSize1); // make mesh a box
    //cCreateBox(p_CommonData->p_dynamicBox2, boxSize1*.9, boxSize1*1.05, boxSize1*.9); // make mesh a box
    cCreateBox(p_CommonData->p_dynamicBox2, boxSize2, boxSize2, boxSize3); // make mesh a box
    //cCreateBox(p_CommonData->p_dynamicBox3, boxSize3, boxSize3, boxSize3); // make mesh a box


    // setup collision detectorsfor the dynamic objects
    //p_CommonData->p_dynamicBox1->createAABBCollisionDetector(toolRadius);
    p_CommonData->p_dynamicBox2->createAABBCollisionDetector(toolRadius);
    //p_CommonData->p_dynamicBox3->createAABBCollisionDetector(toolRadius);

    // define material properties for box 2
    //chai3d::cMaterial mat2;
    mat2.setBlueMediumTurquoise();
    mat2.setStiffness(stiffness2);
    //mat2.setLateralStiffness(latStiffness2);
    mat2.setDynamicFriction(dynFriction2);
    mat2.setStaticFriction(friction2);
    mat2.setUseHapticFriction(true);
    p_CommonData->p_dynamicBox2->setMaterial(mat2);
    p_CommonData->p_dynamicBox2->setUseMaterial(true);

    // define material properties for box 3
    //chai3d::cMaterial mat3;
    //mat3.setRedSalmon();
    //mat3.setStiffness(stiffness3);
    //mat3.setLateralStiffness(latStiffness3);
    //mat3.setDynamicFriction(dynFriction3);
    //mat3.setStaticFriction(friction3);
    //mat3.setUseHapticFriction(true);
    //p_CommonData->p_dynamicBox3->setMaterial(mat3);
    //p_CommonData->p_dynamicBox3->setUseMaterial(true);

    // add mesh to ODE object
    //p_CommonData->ODEBody1->setImageModel(p_CommonData->p_dynamicBox1);
    p_CommonData->ODEBody2->setImageModel(p_CommonData->p_dynamicBox2);
    //p_CommonData->ODEBody3->setImageModel(p_CommonData->p_dynamicBox3);

    // create a dynamic model of the ODE object
    //p_CommonData->ODEBody1->createDynamicBox(boxSize1, boxSize1, boxSize1);
    p_CommonData->ODEBody2->createDynamicBox(boxSize2, boxSize2, boxSize3);
    //p_CommonData->ODEBody3->createDynamicBox(boxSize3, boxSize3, boxSize3);

    // set mass of box
    //p_CommonData->ODEBody1->setMass(mass1);
    p_CommonData->ODEBody2->setMass(mass2);
    //p_CommonData->ODEBody3->setMass(mass3);

    // set position of box
    p_CommonData->ODEBody1->setLocalPos(0.2,.05,-.02);
    p_CommonData->ODEBody2->setLocalPos(0.2,-.05,-.02);
    //p_CommonData->ODEBody3->setLocalPos(0.25,0,-.1);

    double hoopX = 0.35;
    double hoopY = 0.65;
    double hoopZ = -0.35;

    hoop1 =new chai3d::cMesh();
    chai3d::cCreateEllipsoid(hoop1, 0.08, 0.01, 0.08);
    //    hoop1->setLocalPos(hoopX, hoopY, hoopZ);
    hoop1->setLocalPos(hoopX, hoopY, hoopZ);
    //chai3d::cCreateEllipsoid(hoop1, 0.08, 0.08, 0.08);
    //hoop1->setLocalPos(0, 0, 0.00);

    chai3d::cMaterial matHoop1;
    matHoop1.setRed();
    hoop1->setMaterial(matHoop1);

    p_CommonData->p_world->addChild(hoop1);

    hoop2 =new chai3d::cMesh();
    chai3d::cCreateEllipsoid(hoop2, 0.07, 0.01, 0.07);
    hoop2->setLocalPos(hoopX, hoopY - 0.0015, hoopZ);
    chai3d::cMaterial matHoop2;
    matHoop2.setWhite();
    hoop2->setMaterial(matHoop2);
    p_CommonData->p_world->addChild(hoop2);

    hoop3 =new chai3d::cMesh();
    chai3d::cCreateEllipsoid(hoop3, 0.06, 0.01, 0.06);
    hoop3->setLocalPos(hoopX, hoopY - 0.0025, hoopZ);
    chai3d::cMaterial matHoop3;
    matHoop3.setBlue();
    hoop3->setMaterial(matHoop3);
    p_CommonData->p_world->addChild(hoop3);

    hoop4 =new chai3d::cMesh();
    chai3d::cCreateEllipsoid(hoop4, 0.055, 0.01, 0.055);
    hoop4->setLocalPos(hoopX, hoopY - 0.0028, hoopZ);
    chai3d::cMaterial matHoop4;
    matHoop4.setWhite();
    hoop4->setMaterial(matHoop4);
    p_CommonData->p_world->addChild(hoop4);

    targetRadius = 0.08;

    target1 =new chai3d::cMesh();
    chai3d::cCreateEllipsoid(target1, targetRadius, targetRadius, targetRadius);
    target1Pos = chai3d::cVector3d(0.05,0,0); //(0.05, 0, -0.24);
    target1->setLocalPos(target1Pos.x(), target1Pos.y(), target1Pos.z());
    matTarget1.setPurple();
    target1->setMaterial(matTarget1);
    target1->setUseCulling(true);
    target1->setUseTransparency(true);
    target1->setTransparencyLevel(0.35, true);
    p_CommonData->p_world->addChild(target1);

    target2 =new chai3d::cMesh();
    chai3d::cCreateEllipsoid(target2, 0.02, 0.02, 0.01);
    target2Pos = chai3d::cVector3d(0.25,0,0); //(0.05, 0, -0.24);
    target2->setLocalPos(target2Pos.x(), target2Pos.y(), target2Pos.z());
    matTarget2.setYellow();
    target2->setMaterial(matTarget2);
    target2->setUseCulling(true);
    target2->setUseTransparency(true);
    target2->setTransparencyLevel(1, true);
    p_CommonData->p_world->addChild(target2);

    p_CommonData->target1Complete = false;
    p_CommonData->target1Responded = false;
    //p_CommonData->target2Complete = false;
    p_CommonData->targetSuccess = 0;
    //p_CommonData->target3Complete = false;

    //add one and two indicators
    //p_CommonData->p_world->addChild(p_CommonData->oneModel);
    //p_CommonData->p_world->addChild(p_CommonData->twoModel);

    p_CommonData->p_world->addChild(p_CommonData->p_dynamicBox1);
    p_CommonData->p_world->addChild(p_CommonData->p_dynamicBox2);

    //p_CommonData->p_world->addChild(p_CommonData->p_dynamicBox3);

    p_CommonData->p_world->addChild(p_CommonData->p_dynamicScaledBox1);

    //p_CommonData->p_world->addChild(p_CommonData->p_dynamicBox1);


    //p_CommonData->oneModel->setLocalPos(1, 0.5, 0);
    //p_CommonData->oneModel->setLocalPos(0.22, 0.2, 0);
    //p_CommonData->twoModel->setLocalPos(0.22, 0.2, 0);
}

void haptics_thread::SetDynEnvironStiffMassExp()   // Mine Stiffness-Mass Experiment
{
    // set part 1/2 indicators to be able to show
    //p_CommonData->oneModel->setShowEnabled(false);
    //p_CommonData->twoModel->setShowEnabled(true);

    // create the visual boxes on the dynamicbox meshes
    cCreateBox(p_CommonData->p_dynamicBox1, boxSize1, boxSize1, boxSize1); // make mesh a box

    // create the visual for the position scaled dynamic boxes
    cCreateBox(p_CommonData->p_dynamicScaledBox1, boxSize2, boxSize2, boxSize3); // make mesh a box

    // setup collision detectorsfor the dynamic objects
    p_CommonData->p_dynamicBox1->createAABBCollisionDetector(toolRadius);

    // define material properties for box 1 - invisible
    chai3d::cMaterial mat1;
    mat1.setBlue();
    mat1.setStiffness(stiffness1);
    //mat1.setLateralStiffness(latStiffness1);
    mat1.setDynamicFriction(dynFriction1);
    mat1.setStaticFriction(friction1);
    mat1.setUseHapticFriction(true);
    p_CommonData->p_dynamicBox1->setMaterial(mat1);
    p_CommonData->p_dynamicBox1->setUseMaterial(true);

    // define material properties for box 2 - visible
    chai3d::cMaterial mat2;
    mat2.setBlue();

    p_CommonData->p_dynamicScaledBox1->setMaterial(mat2);
    p_CommonData->p_dynamicScaledBox1->setUseMaterial(true);
    p_CommonData->p_dynamicScaledBox1->setHapticEnabled(false);

    // add mesh to ODE object
    p_CommonData->ODEBody1->setImageModel(p_CommonData->p_dynamicBox1);

    // create a dynamic model of the ODE object
    p_CommonData->ODEBody1->createDynamicBox(boxSize1, boxSize1, boxSize3);

    // set mass of box
    //    p_CommonData->ODEBody1->setMass(p_CommonData->expMass);
    p_CommonData->ODEBody1->setMass(mass1);

    // create the visual boxes on the dynamicbox meshes
    cCreateBox(p_CommonData->p_dynamicBox2, boxSize2, boxSize2, boxSize3); // make mesh a box

    // setup collision detectorsfor the dynamic objects
    p_CommonData->p_dynamicBox2->createAABBCollisionDetector(toolRadius);

    // define material properties for box 2
    //chai3d::cMaterial mat2;
    mat2.setYellow();
    mat2.setStiffness(stiffness2);
    //mat2.setLateralStiffness(latStiffness2);
    mat2.setDynamicFriction(dynFriction2);
    mat2.setStaticFriction(friction2);
    mat2.setUseHapticFriction(true);
    //mat2->setTransparencyLevel(1, true);

    p_CommonData->p_dynamicBox2->setMaterial(mat2);
    p_CommonData->p_dynamicBox2->setUseMaterial(true);

    // add mesh to ODE object
    p_CommonData->ODEBody2->setImageModel(p_CommonData->p_dynamicBox2);

    // create a dynamic model of the ODE object
    p_CommonData->ODEBody2->createDynamicBox(boxSize2, boxSize2, boxSize3);

    // set mass of box
    p_CommonData->ODEBody2->setMass(mass2);

    // set position of box
    p_CommonData->ODEBody1->setLocalPos(0.15, -.2, -.02); //(0.1,-.1,-.02);
    p_CommonData->ODEBody2->setLocalPos(0.15,-.1,-.02);

    p_CommonData->ODEBody1->rotateAboutLocalAxisDeg(0,0,1,45);
    p_CommonData->ODEBody2->rotateAboutLocalAxisDeg(0,0,1,45);

    targetRadius = 0.05;

    hoop2 =new chai3d::cMesh();
    chai3d::cCreateRing(hoop2, 0.005, targetRadius);
    hoop2->rotateAboutLocalAxisDeg(1,0,0,90);
    hoop2Pos = chai3d::cVector3d(0.25,0.05,-0.2);
    hoop2->setLocalPos(hoop2Pos.x(), hoop2Pos.y(), hoop2Pos.z());
    matHoop2.setYellow();
    hoop2->setMaterial(matHoop2);
    hoop2->setTransparencyLevel(0.2, true);
    p_CommonData->p_world->addChild(hoop2);

    hoop1 =new chai3d::cMesh();
    chai3d::cCreateRing(hoop1, 0.005, targetRadius);
    hoop1->rotateAboutLocalAxisDeg(1,0,0,90);
    hoop1Pos = chai3d::cVector3d(0.1,0.05,-0.2);
    hoop1->setLocalPos(hoop1Pos.x(), hoop1Pos.y(), hoop1Pos.z());
    matHoop1.setBlue();
    hoop1->setMaterial(matHoop1);
    hoop1->setTransparencyLevel(0.2, true);
    p_CommonData->p_world->addChild(hoop1);


    //wall = new chai3d::cMesh();
    //create a plane
    //wallHeight = 0.1;
    //wallThickness = 0.01;

    //chai3d::cCreateBox(wall, 1.75*0.3, wallThickness, wallHeight);
    //wall->setLocalPos(0.05, 0.05, -0.05);

    p_CommonData->p_world->addChild(wall);

    target1 =new chai3d::cMesh();
    chai3d::cCreateEllipsoid(target1, targetRadius, targetRadius, targetRadius);
    target1Pos = chai3d::cVector3d(0.1,0.2, 0.0); //(0.05, 0.0, -0.24);  (0.1,-0.05,-0.02)
    target1->setLocalPos(target1Pos.x(), target1Pos.y(), target1Pos.z());
    matTarget1.setBlue();
    target1->setMaterial(matTarget1);
    target1->setUseCulling(true);
    target1->setUseTransparency(true);
    target1->setTransparencyLevel(0.2, true);
    p_CommonData->p_world->addChild(target1);

    target2 =new chai3d::cMesh();
    chai3d::cCreateEllipsoid(target2, targetRadius, targetRadius, targetRadius);
    target2Pos = chai3d::cVector3d(0.25,0.2, 0.0); //(0.05, 0.0, -0.24);  (0.1,-0.05,-0.02)
    target2->setLocalPos(target2Pos.x(), target2Pos.y(), target2Pos.z());
    matTarget2.setYellow();
    target2->setMaterial(matTarget2);
    target2->setUseCulling(true);
    target2->setUseTransparency(true);
    target2->setTransparencyLevel(0.2, true);
    p_CommonData->p_world->addChild(target2);

    p_CommonData->target1Complete = false;
    p_CommonData->target2Complete = false;
    p_CommonData->hoop1Complete = false;
    p_CommonData->hoop2Complete = false;

    p_CommonData->explorationComplete = false;

    p_CommonData->answer1 = false;
    p_CommonData->answer2 = false;
    p_CommonData->answerComplete = false;

    p_CommonData->targetSuccess = 0;

    //add one and two indicators
    //p_CommonData->p_world->addChild(p_CommonData->oneModel);
    //p_CommonData->p_world->addChild(p_CommonData->twoModel);

    p_CommonData->p_world->addChild(p_CommonData->p_dynamicBox1);
    p_CommonData->p_world->addChild(p_CommonData->p_dynamicBox2);


    p_CommonData->p_world->addChild(p_CommonData->p_dynamicScaledBox1);

    //p_CommonData->oneModel->setLocalPos(1.0, 0.5, 0.0);
    //p_CommonData->oneModel->setLocalPos(0.22, 0.2, 0.0);
    //p_CommonData->twoModel->setLocalPos(0.22, 0.2, 0.0);
    qDebug()<<"Finished SME Setup";
}

void haptics_thread::SetDynEnvironFingerMappingExp()   // Jasmin FingerMapping Experiment
{
    qDebug() << "start SetDynEnvironFingerMappingExp()";
    // create the visual boxes on the dynamic box meshes
    cCreateBox(p_CommonData->p_dynamicBox1, boxSize1, boxSize1, boxSize1); // make mesh a box

    // setup collision detectorsfor the dynamic objects
    p_CommonData->p_dynamicBox1->createAABBCollisionDetector(toolRadius);

    // define material properties for box 1 - invisible
    mat1.setBlue();
    mat1.setStiffness(stiffness1);
    //mat1.setLateralStiffness(latStiffness1);
    mat1.setDynamicFriction(dynFriction1);
    mat1.setStaticFriction(friction1);
    mat1.setUseHapticFriction(true);
    p_CommonData->p_dynamicBox1->setMaterial(mat1);
    p_CommonData->p_dynamicBox1->setUseMaterial(true);

    // add mesh to ODE object
    p_CommonData->ODEBody1->setImageModel(p_CommonData->p_dynamicBox1);

    // create a dynamic model of the ODE object - box1
    p_CommonData->ODEBody1->createDynamicBox(boxSize1, boxSize1, boxSize3);

    // set mass of box1
    p_CommonData->ODEBody1->setMass(mass1);

    // set position of box
    p_CommonData->ODEBody1->setLocalPos(0.15, -0.2, -0.02); //(0.1,-0.1,-.02);

    //Set orientation of box
    p_CommonData->ODEBody1->rotateAboutLocalAxisDeg(0, 0, 1, 45);

    targetRadius = 0.05;

    //Create box1 hoop
    hoop1 = new chai3d::cMesh();
    chai3d::cCreateRing(hoop1, 0.005, targetRadius);
    hoop1->rotateAboutLocalAxisDeg(1, 0, 0, 90);
    hoop1Pos = chai3d::cVector3d(0.1, 0.05, -0.2);
    hoop1->setLocalPos(hoop1Pos.x(), hoop1Pos.y(), hoop1Pos.z());
    matHoop1.setBlue();
    hoop1->setMaterial(matHoop1);
    hoop1->setTransparencyLevel(0.2, true);
    p_CommonData->p_world->addChild(hoop1);

    /*
    //wall = new chai3d::cMesh();
    ////create a plane
    //wallHeight = 0.1;
    //wallThickness = 0.01;

    //chai3d::cCreateBox(wall, 1.75*0.3, wallThickness, wallHeight);
    //wall->setLocalPos(0.05, 0.05, -0.05);
    */
    //Add vertical Wall to world
    p_CommonData->p_world->addChild(wall);

    //Create Box1 Target Area
    target1 = new chai3d::cMesh();
    chai3d::cCreateEllipsoid(target1, targetRadius, targetRadius, targetRadius);
    target1Pos = chai3d::cVector3d(0.1, 0.2, 0.0); //(0.05, 0.0, -0.24);  (0.1,-0.05,-0.02)
    target1->setLocalPos(target1Pos.x(), target1Pos.y(), target1Pos.z());
    matTarget1.setBlue();
    target1->setMaterial(matTarget1);
    target1->setUseCulling(true);
    target1->setUseTransparency(true);
    target1->setTransparencyLevel(0.2, true);
    p_CommonData->p_world->addChild(target1);

    p_CommonData->target1Complete = false;
    p_CommonData->hoop1Complete = false;

    p_CommonData->explorationComplete = false;

    p_CommonData->answer1 = false;
    p_CommonData->answerComplete = false;

    //Reset trial succes markers whenever environment is created
    p_CommonData->targetSuccess = 0;
    p_CommonData->hoopSuccess = 0;
    p_CommonData->trialSuccess = 0;

    //add one and two indicators
    //p_CommonData->p_world->addChild(p_CommonData->oneModel);
    //p_CommonData->p_world->addChild(p_CommonData->twoModel);

    //Add dynamic boxes to the world
    p_CommonData->p_world->addChild(p_CommonData->p_dynamicBox1);

    //p_CommonData->oneModel->setLocalPos(1.0, 0.5, 0.0);
    //p_CommonData->oneModel->setLocalPos(0.22, 0.2, 0.0);
    //p_CommonData->twoModel->setLocalPos(0.22, 0.2, 0.0);
    qDebug()<<"Finished FME Setup";
}

void haptics_thread::SetDynEnvironHoxelMappingExp()   // Jasmin HoxelMapping Experiment
{   
    qDebug() << "start SetDynEnvironHoxelMappingExp()";
    targetRadius = 0.05;    
    double scaleFactorMMToM = 0.001; //scale stl file from mm unit to meter scale of the environment

    //Create the box with hole object
    boxWithHole = new chai3d::cMultiMesh(); // create a virtual mesh
    //boxWithHole->setShowFrame(true);
    //boxWithHole->setFrameSize(0.075);

    //Load stl model of the object
    if(cLoadFileSTL(boxWithHole, "./Resources/boxWithHole.stl")){
        qDebug() << "boxWithHole loaded";
    }
    boxWithHole->scale(scaleFactorMMToM);

    // setup collision detectors for the dynamic box with finger only
    boxWithHole->createAABBCollisionDetector(toolRadius);

    // define material properties for box 1 - invisible
    mat1.setRed();
    mat1.setStiffness(stiffness1);
    //mat1.setLateralStiffness(latStiffness1);
    mat1.setDynamicFriction(dynFriction1);
    mat1.setStaticFriction(friction1);
    mat1.setUseHapticFriction(true);
    boxWithHole->setMaterial(mat1);
    boxWithHole->setUseMaterial(true);
    boxWithHole->setTransparencyLevel(0.75, true);

    // add mesh to ODE object
    p_CommonData->ODEBody1->setImageModel(boxWithHole);
    // create a dynamic model of the ODE object - box1
    p_CommonData->ODEBody1->createDynamicBox(boxSize1, boxSize1, boxSize3);
    // set mass of box1
    p_CommonData->ODEBody1->setMass(mass1);
    // set position of box
    box1InitPos = chai3d::cVector3d(0.1, hoop1Pos.y()-0.2, -0.02); //(0.15, -0.2, -0.02);
    p_CommonData->ODEBody1->setLocalPos(box1InitPos);
    //Set orientation of box
    p_CommonData->ODEBody1->rotateAboutLocalAxisDeg(0, 0, 1, 45);
    //Add object to the world
    p_CommonData->p_world->addChild(boxWithHole);

    //Create box1 hoop -- visual only
    hoop1 = new chai3d::cMesh();
    chai3d::cCreateRing(hoop1, 0.005, targetRadius);
    hoop1->rotateAboutLocalAxisDeg(1, 0, 0, 90);
    hoop1Pos = chai3d::cVector3d(0.1, 0.085, -0.2);
    hoop1->setLocalPos(hoop1Pos.x(), hoop1Pos.y(), hoop1Pos.z());
    matHoop1.setRed();
    hoop1->setMaterial(matHoop1);
    hoop1->setTransparencyLevel(0.2, true);
    //Add object to the world
    p_CommonData->p_world->addChild(hoop1);

    //Create Box1 Target Area
    target1 = new chai3d::cMesh();
    chai3d::cCreateEllipsoid(target1, targetRadius, targetRadius, targetRadius);
    target1Pos = chai3d::cVector3d(0.1, hoop1Pos.y()+0.2, 0.0); //(0.05, 0.0, -0.24);  (0.1,-0.05,-0.02);
    target1->setLocalPos(target1Pos.x(), target1Pos.y(), target1Pos.z());
    matTarget1.setRed();
    target1->setMaterial(matTarget1);
    target1->setUseCulling(true);
    target1->setUseTransparency(true);
    target1->setTransparencyLevel(0.2, true);
    p_CommonData->p_world->addChild(target1);

    //Make fingers collide with wall
    wall->createAABBCollisionDetector(toolRadius);
    //Add object to the world
    p_CommonData->p_world->addChild(wall);

    //Create the wire object
    wire = new chai3d::cMultiMesh(); // create a virtual mesh
    if(cLoadFileSTL(wire, "./Resources/wire_Jasmin.stl")){
        qDebug() << "wire loaded";
    }
    //Add object to the world
    p_CommonData->p_world->addChild(wire);
    //wire->setShowFrame(true);
    //wire->setFrameSize(0.1);
    wire->rotateAboutLocalAxisDeg(0,0,1,0);
    wire->setLocalPos(hoop1Pos);
    wire->scale(scaleFactorMMToM);
    //Set params for wire
    wire->setShowEnabled(true);
    wire->setUseVertexColors(true);
    chai3d::cColorf wireColor;
    wireColor.setBlack();
    wire->setVertexColor(wireColor);
    wire->m_material->m_specular.set(1.0, 1.0, 1.0);
    wire->setUseMaterial(true);
    wire->setHapticEnabled(true);

    p_CommonData->target1Complete = false;
    p_CommonData->hoop1Complete = false;

    p_CommonData->explorationComplete = false;

    p_CommonData->answer1 = false;
    p_CommonData->answerComplete = false;

    //Reset trial succes markers whenever environment is created
    p_CommonData->targetSuccess = 0;
    p_CommonData->hoopSuccess = 0;
    p_CommonData->trialSuccess = 0;
    p_CommonData->mistakeCounter = 0;
    qDebug()<<"Finished HME Setup";
}

void haptics_thread::SetDynEnvironMultiMassExp() // Jasmin MultiMass Experiment
{
    // create the visual boxes on the dynamicbox meshes
    cCreateBox(p_CommonData->p_dynamicBox1, boxSize1, boxSize1, boxSize1); // make mesh a box
    cCreateBox(p_CommonData->p_dynamicBox2, boxSize2, boxSize2, boxSize2); // make mesh a box
    cCreateBox(p_CommonData->p_dynamicBox3, boxSize3, boxSize3, boxSize3); // make mesh a box

    //setup collision detectors for the dynamic objects
    p_CommonData->p_dynamicBox1->createAABBCollisionDetector(toolRadius);
    p_CommonData->p_dynamicBox2->createAABBCollisionDetector(toolRadius);
    p_CommonData->p_dynamicBox3->createAABBCollisionDetector(toolRadius);

    //define material properties for box 1
    //chai3d::cMaterial mat1;
    mat1.setRedCrimson();
    mat1.setStiffness(stiffness1);
    //mat1.setLateralStiffness(latStiffness1);
    mat1.setDynamicFriction(dynFriction1);
    mat1.setStaticFriction(friction1);
    mat1.setUseHapticFriction(true);
    p_CommonData->p_dynamicBox1->setMaterial(mat1);
    p_CommonData->p_dynamicBox1->setUseMaterial(true);

    // define material properties for box 2
    //chai3d::cMaterial mat2;
    mat2.setBlueAqua();
    mat2.setStiffness(stiffness2);
    //mat2.setLateralStiffness(latStiffness2);
    mat2.setDynamicFriction(dynFriction2);
    mat2.setStaticFriction(friction2);
    mat2.setUseHapticFriction(true);
    p_CommonData->p_dynamicBox2->setMaterial(mat2);
    p_CommonData->p_dynamicBox2->setUseMaterial(true);

    // define material properties for box 3
    //chai3d::cMaterial mat3;
    mat3.setPurpleBlueViolet();
    mat3.setStiffness(stiffness3);
    //mat3.setLateralStiffness(latStiffness3);
    mat3.setDynamicFriction(dynFriction3);
    mat3.setStaticFriction(friction3);
    mat3.setUseHapticFriction(true);
    p_CommonData->p_dynamicBox3->setMaterial(mat3);
    p_CommonData->p_dynamicBox3->setUseMaterial(true);

    // add mesh to ODE object
    p_CommonData->ODEBody1->setImageModel(p_CommonData->p_dynamicBox1);
    p_CommonData->ODEBody2->setImageModel(p_CommonData->p_dynamicBox2);
    p_CommonData->ODEBody3->setImageModel(p_CommonData->p_dynamicBox3);

    // create a dynamic model of the ODE object
    p_CommonData->ODEBody1->createDynamicBox(boxSize1, boxSize1, boxSize1);
    p_CommonData->ODEBody2->createDynamicBox(boxSize2, boxSize2, boxSize2);
    p_CommonData->ODEBody3->createDynamicBox(boxSize3, boxSize3, boxSize3);

    // set mass of box
    p_CommonData->ODEBody1->setMass(mass1);
    p_CommonData->ODEBody2->setMass(mass2);
    p_CommonData->ODEBody3->setMass(mass3);

    // set position of box
    box2InitPos = chai3d::cVector3d(0.1, 0.090, -0.02);// set middle box 1st to use a reference for other box positions
    p_CommonData->ODEBody1->setLocalPos(box2InitPos.get(0), box2InitPos.get(1)+0.15, box2InitPos.get(2));
    p_CommonData->ODEBody2->setLocalPos(box2InitPos);
    p_CommonData->ODEBody3->setLocalPos(box2InitPos.get(0), box2InitPos.get(1)-0.15, box2InitPos.get(2));

    //Add boxes to the world
    p_CommonData->p_world->addChild(p_CommonData->p_dynamicBox1);
    p_CommonData->p_world->addChild(p_CommonData->p_dynamicBox2);
    p_CommonData->p_world->addChild(p_CommonData->p_dynamicBox3);
}

// general mass demo with adjustable parameters
void haptics_thread::SetDynEnvironAdjust() //susana change other properties here
{
    // create the visual boxes on the dynamicbox meshes
    cCreateBox(p_CommonData->p_dynamicBox1, boxSize1, boxSize1, boxSize1); // make mesh a box
    cCreateBox(p_CommonData->p_dynamicBox2, boxSize2, boxSize2, boxSize2); // make mesh a box
    cCreateBox(p_CommonData->p_dynamicBox3, boxSize3, boxSize3, boxSize3); // make mesh a box

    //setup collision detectors for the dynamic objects
    p_CommonData->p_dynamicBox1->createAABBCollisionDetector(toolRadius);
    p_CommonData->p_dynamicBox2->createAABBCollisionDetector(toolRadius);
    p_CommonData->p_dynamicBox3->createAABBCollisionDetector(toolRadius);

    //define material properties for box 1
    //chai3d::cMaterial mat1;
    mat1.setGreenLawn();
    mat1.setStiffness(stiffness1);
    //mat1.setLateralStiffness(latStiffness1);
    mat1.setDynamicFriction(dynFriction1);
    mat1.setStaticFriction(friction1);
    mat1.setUseHapticFriction(true);
    p_CommonData->p_dynamicBox1->setMaterial(mat1);
    p_CommonData->p_dynamicBox1->setUseMaterial(true);

    // define material properties for box 2
    //chai3d::cMaterial mat2;
    mat2.setBlueRoyal();
    mat2.setStiffness(stiffness2);
    //mat2.setLateralStiffness(latStiffness2);
    mat2.setDynamicFriction(dynFriction2);
    mat2.setStaticFriction(friction2);
    mat2.setUseHapticFriction(true);
    p_CommonData->p_dynamicBox2->setMaterial(mat2);
    p_CommonData->p_dynamicBox2->setUseMaterial(true);

    // define material properties for box 3
    //chai3d::cMaterial mat3;
    mat3.setRedSalmon();
    mat3.setStiffness(stiffness3);
    //mat3.setLateralStiffness(latStiffness3);
    mat3.setDynamicFriction(dynFriction3);
    mat3.setStaticFriction(friction3);
    mat3.setUseHapticFriction(true);
    p_CommonData->p_dynamicBox3->setMaterial(mat3);
    p_CommonData->p_dynamicBox3->setUseMaterial(true);

    // add mesh to ODE object
    p_CommonData->ODEBody1->setImageModel(p_CommonData->p_dynamicBox1);
    p_CommonData->ODEBody2->setImageModel(p_CommonData->p_dynamicBox2);
    p_CommonData->ODEBody3->setImageModel(p_CommonData->p_dynamicBox3);

    // create a dynamic model of the ODE object
    p_CommonData->ODEBody1->createDynamicBox(boxSize1, boxSize1, boxSize1);
    p_CommonData->ODEBody2->createDynamicBox(boxSize2, boxSize2, boxSize2);
    p_CommonData->ODEBody3->createDynamicBox(boxSize3, boxSize3, boxSize3);

    // set mass of box
    p_CommonData->ODEBody1->setMass(mass1);
    p_CommonData->ODEBody2->setMass(mass2);
    p_CommonData->ODEBody3->setMass(mass3);

    // set position of box
    p_CommonData->ODEBody1->setLocalPos(0.15,-0.2,-0.3);
    p_CommonData->ODEBody2->setLocalPos(0.20,-0.1,-0.2);
    p_CommonData->ODEBody3->setLocalPos(0.25,0.0,-0.1);

    // define some material properties for wall
    wall = new chai3d::cMesh();
    chai3d::cMaterial matWall;
    matWall.setBrownSandy();
    wall->setMaterial(matWall);
    //create a plane
    wallHeight = 0.1;
    wallThickness = 0.01;

    chai3d::cCreateBox(wall, 1.75*0.3, wallThickness, wallHeight);
    wall->setLocalPos(0.05, 0.085, -0.05);

    p_CommonData->p_world->addChild(wall);

    double hoopX = 0.35;
    double hoopY = 0.65;
    double hoopZ = -0.35;

    hoop1 =new chai3d::cMesh();
    chai3d::cCreateEllipsoid(hoop1, 0.08, 0.01, 0.08);
    //    hoop1->setLocalPos(hoopX, hoopY, hoopZ);
    hoop1->setLocalPos(hoopX, hoopY, hoopZ);
    //    chai3d::cCreateEllipsoid(hoop1, 0.08, 0.08, 0.08);
    //    hoop1->setLocalPos(0, 0, 0.00);

    chai3d::cMaterial matHoop1;
    matHoop1.setRed();
    hoop1->setMaterial(matHoop1);

    p_CommonData->p_world->addChild(hoop1);

    hoop2 = new chai3d::cMesh();
    chai3d::cCreateEllipsoid(hoop2, 0.07, 0.01, 0.07);
    hoop2->setLocalPos(hoopX, hoopY - 0.0015, hoopZ);
    chai3d::cMaterial matHoop2;
    matHoop2.setWhite();
    hoop2->setMaterial(matHoop2);
    p_CommonData->p_world->addChild(hoop2);

    hoop3 = new chai3d::cMesh();
    chai3d::cCreateEllipsoid(hoop3, 0.06, 0.01, 0.06);
    hoop3->setLocalPos(hoopX, hoopY - 0.0025, hoopZ);
    chai3d::cMaterial matHoop3;
    matHoop3.setRed();
    hoop3->setMaterial(matHoop3);
    p_CommonData->p_world->addChild(hoop3);

    hoop4 = new chai3d::cMesh();
    chai3d::cCreateEllipsoid(hoop4, 0.055, 0.01, 0.055);
    hoop4->setLocalPos(hoopX, hoopY - 0.0028, hoopZ);
    chai3d::cMaterial matHoop4;
    matHoop4.setWhite();
    hoop4->setMaterial(matHoop4);
    p_CommonData->p_world->addChild(hoop4);

    targetRadius = 0.08;

    target3 = new chai3d::cMesh();
    double sc = 0.6;
    chai3d::cCreateEllipsoid(target3, targetRadius*sc, targetRadius*sc, targetRadius*sc);
    target3Pos = chai3d::cVector3d(0.05, 0.18, 0);
    target3->setLocalPos(0,0,0); //target3Pos.x(), target3Pos.y(), target3Pos.z());
    matTarget3.setRed();
    target3->setMaterial(matTarget3);
    target3->setUseCulling(true);
    target3->setUseTransparency(true);
    target3->setTransparencyLevel(0.35, true);
    p_CommonData->p_world->addChild(target3);

    target2 = new chai3d::cMesh();
    chai3d::cCreateEllipsoid(target2, targetRadius*sc, targetRadius*sc, targetRadius*sc);
    target2Pos = chai3d::cVector3d(0.05, 0.18, 0.0);
    target2->setLocalPos(0.1 ,-0.1, 0.0); //target3Pos.x(), target3Pos.y(), target3Pos.z());
    matTarget2.setRed();
    target2->setMaterial(matTarget2);
    target2->setUseCulling(true);
    target2->setUseTransparency(true);
    target2->setTransparencyLevel(0.35, true);
    p_CommonData->p_world->addChild(target2);

    target1 = new chai3d::cMesh();
    chai3d::cCreateEllipsoid(target1, targetRadius*sc, targetRadius*sc, targetRadius*sc);
    target1Pos = chai3d::cVector3d(0.05, 0.18, 0.0);
    target1->setLocalPos(0.0 ,-0.2, 0.0); //target3Pos.x(), target3Pos.y(), target3Pos.z());
    matTarget1.setRed();
    target1->setMaterial(matTarget1);
    target1->setUseCulling(true);
    target1->setUseTransparency(true);
    target1->setTransparencyLevel(0.35, true);
    p_CommonData->p_world->addChild(target1);

    //p_CommonData->target1Complete = false;
    p_CommonData->target2Complete = false;
    p_CommonData->target3Complete = false;

    //add one and two indicators
    //p_CommonData->p_world->addChild(p_CommonData->oneModel);
    //p_CommonData->p_world->addChild(p_CommonData->twoModel);
}

// general mass demo with adjustable parameters
void haptics_thread::SetManualAdjust() //susana change other properties here
{
    // create the visual boxes on the dynamicbox meshes
    cCreateBox(p_CommonData->p_dynamicBox1, boxSize1, boxSize1, boxSize1); // make mesh a box
    //    cCreateBox(p_CommonData->p_dynamicBox2, boxSize1*.9, boxSize1*1.05, boxSize1*.9); // make mesh a box
    //    cCreateBox(p_CommonData->p_dynamicBox2, boxSize2, boxSize2, boxSize2); // make mesh a box
    //    cCreateBox(p_CommonData->p_dynamicBox3, boxSize3, boxSize3, boxSize3); // make mesh a box

    // setup collision detectorsfor the dynamic objects
    p_CommonData->p_dynamicBox1->createAABBCollisionDetector(toolRadius);
    //    p_CommonData->p_dynamicBox2->createAABBCollisionDetector(toolRadius);
    //    p_CommonData->p_dynamicBox3->createAABBCollisionDetector(toolRadius);

    // define material properties for box 1
    //chai3d::cMaterial mat1;
    mat1.setGreenLawn();
    mat1.setStiffness(stiffness1);
    //mat1.setLateralStiffness(latStiffness1);
    mat1.setDynamicFriction(dynFriction1);
    mat1.setStaticFriction(friction1);
    mat1.setUseHapticFriction(true);
    p_CommonData->p_dynamicBox1->setMaterial(mat1);
    p_CommonData->p_dynamicBox1->setUseMaterial(true);

    /*
    // define material properties for box 2
    //chai3d::cMaterial mat2;
    mat2.setBlueRoyal();
    mat2.setStiffness(stiffness2);
    mat2.setLateralStiffness(latStiffness2);
    mat2.setDynamicFriction(dynFriction2);
    mat2.setStaticFriction(friction2);
    mat2.setUseHapticFriction(true);
    p_CommonData->p_dynamicBox2->setMaterial(mat2);
    p_CommonData->p_dynamicBox2->setUseMaterial(true);

    // define material properties for box 3
    //chai3d::cMaterial mat3;
    mat3.setRedSalmon();
    mat3.setStiffness(stiffness3);
    mat3.setLateralStiffness(latStiffness3);
    mat3.setDynamicFriction(dynFriction3);
    mat3.setStaticFriction(friction3);
    mat3.setUseHapticFriction(true);
    p_CommonData->p_dynamicBox3->setMaterial(mat3);
    p_CommonData->p_dynamicBox3->setUseMaterial(true);
    */

    // add mesh to ODE object
    p_CommonData->ODEBody1->setImageModel(p_CommonData->p_dynamicBox1);
    //    p_CommonData->ODEBody2->setImageModel(p_CommonData->p_dynamicBox2);
    //    p_CommonData->ODEBody3->setImageModel(p_CommonData->p_dynamicBox3);

    // create a dynamic model of the ODE object
    p_CommonData->ODEBody1->createDynamicBox(boxSize1, boxSize1, boxSize1);
    //    p_CommonData->ODEBody2->createDynamicBox(boxSize2, boxSize2, boxSize2);
    //    p_CommonData->ODEBody3->createDynamicBox(boxSize3, boxSize3, boxSize3);

    // set mass of box
    p_CommonData->ODEBody1->setMass(mass1);
    //    p_CommonData->ODEBody2->setMass(mass2);
    //    p_CommonData->ODEBody3->setMass(mass3);

    // set position of box
    p_CommonData->ODEBody1->setLocalPos(0.15,-.2,-.3);
    //    p_CommonData->ODEBody2->setLocalPos(0.2,-.1,-.2);
    //    p_CommonData->ODEBody3->setLocalPos(0.25,0,-.1);


    // define some material properties for wall
    wall = new chai3d::cMesh();
    chai3d::cMaterial matWall;
    matWall.setBrownSandy();
    wall->setMaterial(matWall);
    //create a plane
    wallHeight = 0.1;
    wallThickness = 0.01;

    chai3d::cCreateBox(wall, 1.75*0.3, wallThickness, wallHeight);
    wall->setLocalPos(0.05, 0.085, -0.05);

    p_CommonData->p_world->addChild(wall);

    double hoopX = 0.35;
    double hoopY = 0.65;
    double hoopZ = -0.35;

    hoop1 = new chai3d::cMesh();
    chai3d::cCreateEllipsoid(hoop1, 0.08, 0.01, 0.08);
    //    hoop1->setLocalPos(hoopX, hoopY, hoopZ);
    hoop1->setLocalPos(hoopX, hoopY, hoopZ);
    //    chai3d::cCreateEllipsoid(hoop1, 0.08, 0.08, 0.08);
    //    hoop1->setLocalPos(0, 0, 0.00);

    chai3d::cMaterial matHoop1;
    matHoop1.setRed();
    hoop1->setMaterial(matHoop1);

    p_CommonData->p_world->addChild(hoop1);

    hoop2 = new chai3d::cMesh();
    chai3d::cCreateEllipsoid(hoop2, 0.07, 0.01, 0.07);
    hoop2->setLocalPos(hoopX, hoopY - 0.0015, hoopZ);
    chai3d::cMaterial matHoop2;
    matHoop2.setWhite();
    hoop2->setMaterial(matHoop2);
    p_CommonData->p_world->addChild(hoop2);

    hoop3 = new chai3d::cMesh();
    chai3d::cCreateEllipsoid(hoop3, 0.06, 0.01, 0.06);
    hoop3->setLocalPos(hoopX, hoopY - 0.0025, hoopZ);
    chai3d::cMaterial matHoop3;
    matHoop3.setRed();
    hoop3->setMaterial(matHoop3);
    p_CommonData->p_world->addChild(hoop3);

    hoop4 = new chai3d::cMesh();
    chai3d::cCreateEllipsoid(hoop4, 0.055, 0.01, 0.055);
    hoop4->setLocalPos(hoopX, hoopY - 0.0028, hoopZ);
    chai3d::cMaterial matHoop4;
    matHoop4.setWhite();
    hoop4->setMaterial(matHoop4);
    p_CommonData->p_world->addChild(hoop4);

    targetRadius = 0.08;

    target3 = new chai3d::cMesh();
    double sc = 0.6;
    chai3d::cCreateEllipsoid(target3, targetRadius*sc, targetRadius*sc, targetRadius*sc);
    target3Pos = chai3d::cVector3d(0.05, 0.18, 0);
    target3->setLocalPos(0,0,0); //target3Pos.x(), target3Pos.y(), target3Pos.z());
    matTarget3.setRed();
    target3->setMaterial(matTarget3);
    target3->setUseCulling(true);
    target3->setUseTransparency(true);
    target3->setTransparencyLevel(0.35, true);
    p_CommonData->p_world->addChild(target3);

    target2 = new chai3d::cMesh();
    chai3d::cCreateEllipsoid(target2, targetRadius*sc, targetRadius*sc, targetRadius*sc);
    target2Pos = chai3d::cVector3d(0.05, 0.18, 0);
    target2->setLocalPos(0.1,-0.1,0); //target3Pos.x(), target3Pos.y(), target3Pos.z());
    matTarget2.setRed();
    target2->setMaterial(matTarget2);
    target2->setUseCulling(true);
    target2->setUseTransparency(true);
    target2->setTransparencyLevel(0.35, true);
    p_CommonData->p_world->addChild(target2);

    target1 = new chai3d::cMesh();
    chai3d::cCreateEllipsoid(target1, targetRadius*sc, targetRadius*sc, targetRadius*sc);
    target1Pos = chai3d::cVector3d(0.05, 0.18, 0);
    target1->setLocalPos(0,-0.2,0); //target3Pos.x(), target3Pos.y(), target3Pos.z());
    matTarget1.setRed();
    target1->setMaterial(matTarget1);
    target1->setUseCulling(true);
    target1->setUseTransparency(true);
    target1->setTransparencyLevel(0.35, true);
    p_CommonData->p_world->addChild(target1);

    //p_CommonData->target1Complete = false;
    p_CommonData->target2Complete = false;
    p_CommonData->target3Complete = false;

    //add one and two indicators
    p_CommonData->p_world->addChild(p_CommonData->oneModel);
    p_CommonData->p_world->addChild(p_CommonData->twoModel);
}
