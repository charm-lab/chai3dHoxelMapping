#include "c3DOFdevice.h"
#include "shared_data.h"
#include <QLatin1Char>


c3DOFDevice::c3DOFDevice(int num)
{
    this->finger = num; // is this 0 (index) or 1 (thumb)
}

c3DOFDevice::~c3DOFDevice()
{   
    delete this->motor_1;
    Sleep(100);

#ifdef SENSORAY826
    S826_SystemClose();
    qDebug()<<"S826_SystemClose()";
#endif
}

int c3DOFDevice::Init3DOFDeviceEnc()
{
#ifdef SENSORAY826
    S826_SystemOpen();  //open connection to the sensoray
    qDebug()<<"S826_SystemOpen()";
#endif

    if(this->finger == 0)
    {
        //SENSORAY
        motor_1 = new cMotorController(CHANNEL_NUM0);
        //motor_1 = new cMotorController(6);
        motor_1->InitEncoder();
        motor_1->InitDACOut();
        //////HOXEL

    }

    else if(this->finger == 1)
    {
        motor_1 = new cMotorController(CHANNEL_NUM1);
        //motor_1 = new cMotorController(7);
        motor_1->InitEncoder();
        motor_1->InitDACOut();

        //////HOXEL
    }

    this->neutralPos = this->GetCartesianPos();
    firstTimeThrough = true;
    return 0;

}

double c3DOFDevice::GetJointAngles() // Eigen::Vector3d
{
    //double offset = 0;
    double jointAngles = motor_1->GetMotorAngle();
    return jointAngles;
}

// calculate the desired actuator stroke for the desired pose
double c3DOFDevice::CalcDesiredStroke(double BoxNo, double LevNo1, double LevNo2, double dir, bool MassExp) // Eigen::Vector3d
{
    double desiredStroke1 = 0;
    //    desiredStroke1 = (desiredPos[2]*10); //<< desiredPos;
    desiredStroke1 = ((desiredPos[0]*10)+(desiredPos[1]*10)+(desiredPos[2]*10)); //<< desiredPos;

    int desiredStroke2 = (int) desiredStroke1;
    double desiredStroke3 = (double) desiredStroke2;
    double desiredStroke = (desiredStroke3/10);
    double limit = 5;

    //set stroke to min value if too small
    if (desiredStroke <= 0.1)
    {
        desiredStroke = 0.1;
    }

    //Determine limits based on experiment
    else if(MassExp){
        qDebug() << "In MASS EXP";
        if(dir == 1) {
            if(BoxNo == 1){
                if(LevNo1 == 1){
                    limit = 3;
                    //qDebug() << "box" << BoxNo << "level" << LevNo1 << "limit" << limit;
                }
                else if(LevNo1 == 2){
                    limit = 3.5;
                    //qDebug() << "box" << BoxNo << "level" << LevNo1 << "limit" << limit;
                }
                else if(LevNo1 == 3){
                    limit = 4;
                    //qDebug() << "box" << BoxNo << "level" << LevNo1 << "limit" << limit;
                }
                else if(LevNo1 == 4){
                    limit = 4.5;
                    //qDebug() << "box" << BoxNo << "level" << LevNo1 << "limit" << limit;
                }
                else if(LevNo1 == 5){
                    limit = 5;
                    //qDebug() << "box" << BoxNo << "level" << LevNo1 << "limit" << limit;
                }
            }

            else if(BoxNo == 2){
                if(LevNo2 == 1){
                    limit = 3;
                    //qDebug() << "box" << BoxNo << "level" << LevNo2 << "limit" << limit;
                }
                else if(LevNo2 == 2){
                    limit = 3.5;
                    //qDebug() << "box" << BoxNo << "level" << LevNo2 << "limit" << limit;
                }
                else if(LevNo2 == 3){
                    limit = 4;
                    //qDebug() << "box" << BoxNo << "level" << LevNo2 << "limit" << limit;
                }
                else if(LevNo2 == 4){
                    limit = 4.5;
                    //qDebug() << "box" << BoxNo << "level" << LevNo2 << "limit" << limit;
                }
                else if(LevNo2 == 5){
                    limit = 5;
                    //qDebug() << "box" << BoxNo << "level" << LevNo2 << "limit" << limit;
                }
            }
        }

        else if(dir == 2){
            if(BoxNo == 1){
                if(LevNo1 == 1){
                    limit = 3;
                    qDebug() << "box" << BoxNo << "level" << LevNo1 << "limit" << limit;
                }
                else if(LevNo1 == 2){
                    limit = 3.5;
                    qDebug() << "box" << BoxNo << "level" << LevNo1 << "limit" << limit;
                }
                else if(LevNo1 == 3){
                    limit = 4;
                    qDebug() << "box" << BoxNo << "level" << LevNo1 << "limit" << limit;
                }
                else if(LevNo1 == 4){
                    limit = 4.5;
                    qDebug() << "box" << BoxNo << "level" << LevNo1 << "limit" << limit;
                }
                else if(LevNo1 == 5){
                    limit = 5;
                    qDebug() << "box" << BoxNo << "level" << LevNo1 << "limit" << limit;
                }
            }

            else if(BoxNo == 2){
                if(LevNo2 == 1){
                    limit = 3;
                    qDebug() << "box" << BoxNo << "level" << LevNo2 << "limit" << limit;
                }
                else if(LevNo2 == 2){
                    limit = 3.5;
                    qDebug() << "box" << BoxNo << "level" << LevNo2 << "limit" << limit;
                }
                else if(LevNo2 == 3){
                    limit = 4;
                    qDebug() << "box" << BoxNo << "level" << LevNo2 << "limit" << limit;
                }
                else if(LevNo2 == 4){
                    limit = 4.5;
                    qDebug() << "box" << BoxNo << "level" << LevNo2 << "limit" << limit;
                }
                else if(LevNo2 == 5){
                    limit = 5;
                    qDebug() << "box" << BoxNo << "level" << LevNo2 << "limit" << limit;
                }
            }

        }
        limit = limit*2.5;
    }

    else
    {
        limit = MAX_TACTOR_EXTENSION;
    }

    //Limit the stroke if calcualtions extend limits
    if (desiredStroke > limit)
    {
       // desiredStroke = limit; // do not limit here, limit in the motorcontrol script after scaledown
    }

    if(isnan(desiredStroke))//||isnan( returnAngles[1])||isnan( returnAngles[2]))
    {
        qDebug()<<"desiredStroke:"<< desiredStroke; //[0]<<" "<< returnAngles[1]<<" "<< returnAngles[2]
    }

    return desiredStroke;
}


Eigen::Vector3d c3DOFDevice::GetCartesianPos()
{
    Eigen::Vector3d pos(3);
    pos << 0,0,0;
    return pos;
}

// Set the desired pos
void c3DOFDevice::SetDesiredPos(Eigen::Vector3d desiredPosArg)
{
    double xRange = 20.0;
    double yRange = 20.0;
    double zRange = 20.0;

    // limit workspace motion (plus or minus)
    double xPosLimit = this->neutralPos[0] + xRange;
    double xNegLimit = this->neutralPos[0] - xRange;
    double yPosLimit = this->neutralPos[1] + yRange;
    double yNegLimit = this->neutralPos[1] - yRange;
    double zPosLimit = this->neutralPos[2] + zRange; //*0.7;
    double zNegLimit = this->neutralPos[2] - zRange;

    if (desiredPosArg[0] > xPosLimit) desiredPosArg[0] = xPosLimit;
    if (desiredPosArg[0] < xNegLimit) desiredPosArg[0] = xNegLimit;
    if (desiredPosArg[1] > yPosLimit) desiredPosArg[1] = yPosLimit;
    if (desiredPosArg[1] < yNegLimit) desiredPosArg[1] = yNegLimit;
    if (desiredPosArg[2] > zPosLimit) desiredPosArg[2] = zPosLimit;
    if (desiredPosArg[2] < zNegLimit) desiredPosArg[2] = zNegLimit;

    if (desiredPosArg[0] < 0) desiredPosArg[0] = abs(desiredPosArg[0]);
    if (desiredPosArg[1] < 0) desiredPosArg[1] = abs(desiredPosArg[1]);
    if (desiredPosArg[2] < 0) desiredPosArg[2] = abs(desiredPosArg[2]);

    this->desiredPos = desiredPosArg;
}

// read the desired pos
Eigen::Vector3d c3DOFDevice::ReadDesiredPos()
{
    return this->desiredPos;
}

//// set motor torques based on desired joint torques
//void c3DOFDevice::SetStrokeOutput(double desiredStroke, bool equal)//(Eigen::Vector3d desJointTorqueOutput)
//{
//    this->motor_1->SetOutputStroke(desiredStroke, bool equal);
//}

double c3DOFDevice::ReadStrokeOutput()
{
    double returnStroke(this->motor_1->strokeOutput); //, this->motor_2->voltageOutput, this->motor_3->voltageOutput);
    return returnStroke;
}

// joint controller that reads device desired joint angles
void c3DOFDevice::SendStroke(bool man, bool equal, bool reverse, bool equalTouch, bool index, bool thumb, double ref, double BoxNo, double LevNo1, double LevNo2, double dir, bool MassExp)
{
    //     qDebug()<< "try2" << BoxNo << LevNo1 << LevNo2;

    double desStr_VR = CalcDesiredStroke(BoxNo, LevNo1, LevNo2, dir, MassExp);

    if(isnan(desStr_VR))
        qDebug()<<"CalcDesiredStroke:"<<desStr_VR;

    double desStr = 0;
    if(man == 1)
        desStr = ref;
    else
        desStr = desStr_VR;

    this->motor_1->SetOutputStroke(desStr, equal, reverse, equalTouch, index, thumb);
}

void c3DOFDevice::TurnOffControl()
{
    this->motor_1->SetOutputStroke(0, false, false, false, false, false);
}

chai3d::cMatrix3d c3DOFDevice::rotX1(double a_angle)
{
    chai3d::cMatrix3d t_tempMatrix;
    t_tempMatrix(0,0) = 1; t_tempMatrix(0,1) = 0; t_tempMatrix(0,2) = 0;
    t_tempMatrix(1,0) = 0; t_tempMatrix(1,1) = cos(a_angle); t_tempMatrix(1,2) = -sin(a_angle);
    t_tempMatrix(2,0) = 0; t_tempMatrix(2,1) = sin(a_angle); t_tempMatrix(2,2) =  cos(a_angle);
    return t_tempMatrix;
}

// =========================================================================================
chai3d::cMatrix3d c3DOFDevice::rotY1(double a_angle)
{
    chai3d::cMatrix3d t_tempMatrix;
    t_tempMatrix(0,0) =  cos(a_angle); t_tempMatrix(0,1) = 0; t_tempMatrix(0,2) =  sin(a_angle);
    t_tempMatrix(1,0) = 0;             t_tempMatrix(1,1) = 1; t_tempMatrix(1,2) =             0;
    t_tempMatrix(2,0) = -sin(a_angle); t_tempMatrix(2,1) = 0; t_tempMatrix(2,2) =  cos(a_angle);
    return t_tempMatrix;
}

// =========================================================================================
chai3d::cMatrix3d c3DOFDevice::rotZ1(double a_angle)
{
    chai3d::cMatrix3d t_tempMatrix;
    t_tempMatrix(0,0) = cos(a_angle); t_tempMatrix(0,1) = -sin(a_angle); t_tempMatrix(0,2) = 0;
    t_tempMatrix(1,0) = sin(a_angle); t_tempMatrix(1,1) =  cos(a_angle); t_tempMatrix(1,2) = 0;
    t_tempMatrix(2,0) = 0           ; t_tempMatrix(2,1) = 0            ; t_tempMatrix(2,2) = 1;
    return t_tempMatrix;
}
