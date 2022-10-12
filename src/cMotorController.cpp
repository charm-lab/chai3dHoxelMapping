//===========================================================================
/*
    This file controls defines a class "MotorControl" that interfaces
    with the Sensoray 826 to allow control of a motor with encoder.
*/
//===========================================================================
#include "shared_data.h"
#include "mainwindow.h"
//#include <QtSerialPort/QSerialPort>
//#include <QtSerialPort/QSerialPortInfo>

//Servo Variables
uint zeroPosOffTime = 750; //command to set motor angle such that we are at zero position
uint zeroPosOnTime = 20000 - zeroPosOffTime; //command to set motor angle such that we are at zero position
uint maxOffTime = 2250; //maximum safe value to use for offTime
double angleRange; //Safe range of operation for servo rotation in degrees

double pitchRadius = 5.75; // Pinion pitch radius in mm
uint counter;//counter for servos

double currentServo0Stroke = 0.00;
double currentServo1Stroke = 0.00;

// Constructor of motor controller =========================================
cMotorController::cMotorController(int inputMotorID)
{
#ifdef SENSORAY826
    channelNum = inputMotorID; //now just indexing motors directly

    //Setting appropriate counter for the motor active in this loop
    if (channelNum == CHANNEL_NUM0)
    {
        counter = 0; //servo0 - index - default dorsal
    }
    if (channelNum == CHANNEL_NUM1)
    {
        counter = 1; //servo1 - thumb - default ventral
    }
#endif
}

void cMotorController::InitDACOut()
{
#ifdef SENSORAY826
    int fail = S826_DacRangeWrite(PCI_BOARD, channelNum, VOLTRANGE, 0);//int fail = 1;
    if (fail == 0)
        qDebug() << "DAC channel #" << channelNum << "initialized";
#endif
#ifndef SENSORAY826
    qDebug() << "Sensoray disabled";
#endif
}
//Route Counter Output to DIO Pins. See Sensoray826 Manual Sec. 7.4.7 & 7.4.10
int RouteCounterOutput(uint board, uint ctr, uint dio)
{
#ifdef SENSORAY826
    uint data[2]; // dio routing mask
    if ((dio >= S826_NUM_DIO) || (ctr >= S826_NUM_COUNT))
        return S826_ERR_VALUE; // bad channel number
    if ((dio & 7) != ctr)
        return S826_ERR_VALUE; // counter output can't be routed to dio

    // Route counter output to DIO pin:
    S826_SafeWrenWrite(board, S826_SAFEN_SWE); // Enable writes to DIO signal router.
    S826_DioOutputSourceRead(board, data); // Route counter output to DIO
    data[dio > 23] |= (1 << (dio % 24)); // without altering other routes.
    S826_DioOutputSourceWrite(board, data);
    return S826_SafeWrenWrite(board, S826_SAFEN_SWD); // Disable writes to DIO signal router.
#endif
#ifndef SENSORAY826
    return 1;
#endif
}

//Set PWM for PWM Generator. See Sensoray826 Manual Sec. 7.4.10
int SetPWM(uint board, uint ctr, uint ontime, uint offtime)
{
#ifdef SENSORAY826
    S826_CounterPreloadWrite(board, ctr, 0, ontime); // On time in us.
    S826_CounterPreloadWrite(board, ctr, 1, offtime); // Off time in us.
#endif
    return 1; // added to keep return type
}

//Create PWM for PWM Generator. See Sensoray826 Manual Sec. 7.4.10
int CreatePWM(uint board, uint ctr, uint ontime, uint offtime)
{
#ifdef SENSORAY826
    S826_CounterModeWrite(board, ctr, // Configure counter for PWM:
                          S826_CM_K_1MHZ | // clock = internal 1 MHz
                          S826_CM_UD_REVERSE | // count down
                          S826_CM_PX_START | S826_CM_PX_ZERO | // preload @startup and counts==0
                          S826_CM_BP_BOTH | // use both preloads (toggle)
                          S826_CM_OM_PRELOAD); // assert ExtOut during preload0 interval
    SetPWM(board, ctr, ontime, offtime); // Program initial on/off times.
#endif
    return 1; // added to keep return type
}

//Start PWM for PWM Generator.  See Sensoray826 Manual Sec. 7.4.10
int StartPWM(uint board, uint ctr)
{
#ifdef SENSORAY826
    return S826_CounterStateWrite(board, ctr, 1); // Start the PWM generator.
#endif
#ifndef SENSORAY826
    return 1; // Start the PWM generator.
#endif
}

void cMotorController::InitEncoder()
{
#ifdef SENSORAY826
    /*
        S826_AdcSlotConfigWrite(PCI_BOARD, 0,  6, 100, S826_ADC_GAIN_2);        //S826_AdcSlotConfigWrite(PCI_BOARD, 1, 6, 50, S826_ADC_GAIN_2);
        S826_AdcSlotConfigWrite(PCI_BOARD, 1,  6, 10, S826_ADC_GAIN_2);        //S826_AdcSlotConfigWrite(PCI_BOARD, 1, 6, 50, S826_ADC_GAIN_2);
        S826_AdcSlotConfigWrite(PCI_BOARD, 2,  6, 10, S826_ADC_GAIN_2);        //S826_AdcSlotConfigWrite(PCI_BOARD, 1, 6, 50, S826_ADC_GAIN_2);
        S826_AdcSlotConfigWrite(PCI_BOARD, 3,  6, 10, S826_ADC_GAIN_2);        //S826_AdcSlotConfigWrite(PCI_BOARD, 1, 6, 50, S826_ADC_GAIN_2);
        S826_AdcSlotConfigWrite(PCI_BOARD, 4,  6, 10, S826_ADC_GAIN_2);        //S826_AdcSlotConfigWrite(PCI_BOARD, 1, 6, 50, S826_ADC_GAIN_2);
        S826_AdcSlotConfigWrite(PCI_BOARD, 5,  6, 10, S826_ADC_GAIN_2);        //S826_AdcSlotConfigWrite(PCI_BOARD, 1, 6, 50, S826_ADC_GAIN_2);
        S826_AdcSlotConfigWrite(PCI_BOARD, 6,  6, 10, S826_ADC_GAIN_2);        //S826_AdcSlotConfigWrite(PCI_BOARD, 1, 6, 50, S826_ADC_GAIN_2);
        S826_AdcSlotConfigWrite(PCI_BOARD, 7,  6, 10, S826_ADC_GAIN_2);        //S826_AdcSlotConfigWrite(PCI_BOARD, 1, 6, 50, S826_ADC_GAIN_2);

        S826_AdcSlotConfigWrite(PCI_BOARD, 8,  7, 100, S826_ADC_GAIN_2);        //S826_AdcSlotConfigWrite(PCI_BOARD, 1, 6, 50, S826_ADC_GAIN_2);
        S826_AdcSlotConfigWrite(PCI_BOARD, 9,  7, 10, S826_ADC_GAIN_2);        //S826_AdcSlotConfigWrite(PCI_BOARD, 1, 6, 50, S826_ADC_GAIN_2);
        S826_AdcSlotConfigWrite(PCI_BOARD, 10, 7, 10, S826_ADC_GAIN_2);        //S826_AdcSlotConfigWrite(PCI_BOARD, 1, 6, 50, S826_ADC_GAIN_2);
        S826_AdcSlotConfigWrite(PCI_BOARD, 11, 7, 10, S826_ADC_GAIN_2);        //S826_AdcSlotConfigWrite(PCI_BOARD, 1, 6, 50, S826_ADC_GAIN_2);
        S826_AdcSlotConfigWrite(PCI_BOARD, 12, 7, 10, S826_ADC_GAIN_2);        //S826_AdcSlotConfigWrite(PCI_BOARD, 1, 6, 50, S826_ADC_GAIN_2);
        S826_AdcSlotConfigWrite(PCI_BOARD, 13, 7, 10, S826_ADC_GAIN_2);        //S826_AdcSlotConfigWrite(PCI_BOARD, 1, 6, 50, S826_ADC_GAIN_2);
        S826_AdcSlotConfigWrite(PCI_BOARD, 14, 7, 10, S826_ADC_GAIN_2);        //S826_AdcSlotConfigWrite(PCI_BOARD, 1, 6, 50, S826_ADC_GAIN_2);
        S826_AdcSlotConfigWrite(PCI_BOARD, 15, 7, 10, S826_ADC_GAIN_2);        //S826_AdcSlotConfigWrite(PCI_BOARD, 1, 6, 50, S826_ADC_GAIN_2);
    */

    S826_AdcSlotlistWrite(PCI_BOARD, 0xFFFF, S826_BITWRITE); // Hardware triggered, source = counter 0 ExtOut
    S826_AdcTrigModeWrite(PCI_BOARD, 0);
    S826_AdcEnableWrite(PCI_BOARD, 1); //enable ADC conversions.

    //Initialize range of angles the servo can actuate -- Based on whatever values allow safe operation
    angleRange = (MAX_TACTOR_EXTENSION / pitchRadius) * (180.0/PI);
    qDebug()<<"angleRange"<< angleRange;

    //Create initial onTime and offTime for DutyCycle
    //uint initOffTime = zeroPosOffTime; //minVal = 750; maxVal = 2500;
    //uint initOnTime = 20000 - initOffTime;

    //***Initialize PWM Generator, see Sensoray826 Manual Sec. 7.4.10***
    CreatePWM(PCI_BOARD, counter, zeroPosOnTime, zeroPosOffTime); // Configure counter0 as PWM.
    RouteCounterOutput(PCI_BOARD, counter, channelNum); // Route counter# output to dio# (see Section 7.4.7).
    StartPWM(PCI_BOARD, counter); // Start the PWM running.

    qDebug()<<"initialized PWM"<< channelNum <<" | " << "counter" << counter <<" | " <<"offTime: " <<zeroPosOffTime;

#endif
}

//Probably not needed for servo motor (there's no pot for the HXT servos)
double cMotorController::GetMotorAngle()
{
    double motorAngle = 0;

    double rawMotorAngle;
#ifdef SENSORAY826

    //AdcHandler(&PotReading_i, &PotReading_t);

    //int errcode;
    int slotval[16];

    //    while (1) {
    //        uint slotlist = 0xFFFF;  // look at slots 0 through 2
    //        errcode = S826_AdcRead(PCI_BOARD, slotval, NULL, &slotlist, 1000); // wait for IRQ

    //        if (errcode == S826_ERR_OK)
    //            break;
    //    }

    //       if (i == 0){
    if (channelNum == 6)
    {

        double PotReading_i1 = (double)(slotval[0]  & 0xFFFF);
        double PotReading_i2 = (double)(slotval[1]  & 0xFFFF);
        double PotReading_i3 = (double)(slotval[2]  & 0xFFFF);
        double PotReading_i4 = (double)(slotval[3]  & 0xFFFF);
        double PotReading_i5 = (double)(slotval[4]  & 0xFFFF);
        double PotReading_i6 = (double)(slotval[5]  & 0xFFFF);
        double PotReading_i7 = (double)(slotval[6]  & 0xFFFF);
        double PotReading_i8 = (double)(slotval[7]  & 0xFFFF);
        double PotReading_i = (PotReading_i1+PotReading_i2+PotReading_i3+PotReading_i4+PotReading_i5+PotReading_i6+PotReading_i7+PotReading_i8)*330/(8*32768); //);//
        int PotFiltered_i1 = (int) PotReading_i;
        double PotFiltered_i2 = (double) PotFiltered_i1;
        rawMotorAngle = abs(21 - (PotFiltered_i2/10))*3/2; //0.7*PotReading_i+0.3*PotReading_iold;
    }
    else if (channelNum == 7)
    {
        double PotReading_t1 = (double)(slotval[8]  & 0xFFFF);
        double PotReading_t2 = (double)(slotval[9]  & 0xFFFF);
        double PotReading_t3 = (double)(slotval[10] & 0xFFFF);
        double PotReading_t4 = (double)(slotval[11] & 0xFFFF);
        double PotReading_t5 = (double)(slotval[12] & 0xFFFF);
        double PotReading_t6 = (double)(slotval[13] & 0xFFFF);
        double PotReading_t7 = (double)(slotval[14] & 0xFFFF);
        double PotReading_t8 = (double)(slotval[15] & 0xFFFF);
        double PotReading_t = (PotReading_t1+PotReading_t2+PotReading_t3+PotReading_t4+PotReading_t5+PotReading_t6+PotReading_t7+PotReading_t8)*330/(8*32768); //);//
        int PotFiltered_t1 = (int) PotReading_t;
        double PotFiltered_t2 = (double) PotFiltered_t1;
        rawMotorAngle = abs(21 - (PotFiltered_t2/10))*3/2; //0.7*PotReading_t+0.3*PotReading_told;
    }
    motorAngle = rawMotorAngle;
#endif
    return motorAngle;
}

// Destructor of motor controller ================================
cMotorController::~cMotorController()
{
#ifndef SENSORAY826
    S826_AdcEnableWrite(PCI_BOARD, 0);
#endif
  close();
}

// Write all outputs to 0 and then close 826 board =========================
int cMotorController::close()
{
#ifdef SENSORAY826
    //Set Servo to zero pos
    uint closeOnTime = 20000 - zeroPosOffTime;
    SetPWM(PCI_BOARD, CHANNEL_NUM0, closeOnTime, zeroPosOffTime); //reset servo0
    SetPWM(PCI_BOARD, CHANNEL_NUM1, closeOnTime, zeroPosOffTime); //reset servo1

    qDebug()<<"close offTime: "<<zeroPosOffTime;

#endif
    return 0;
}

//***Essentially SetPWM(), after deciding how much stroke is needed, use to update the PWM as needed
void cMotorController::SetOutputStroke(double desiredStroke, bool equal, bool reverse, bool equalTouch, bool index, bool thumb)
{

    if (std::isnan(desiredStroke))
    {
        //qDebug()<<"desiredStroke:"<<desiredStroke;
        desiredStroke=0;
    }
    //qDebug()<<"scale:"<<scale;
    //double scaleForce = scale;
    double convertToCM = 0.1;
    double convertToMM = 10;

    //bool dors = true;
    //bool lat = true;

    double Vmax = 5; //10//range of DAC
    double Vmin = 0; //10//range of DAC

    double StrokeOut = 0.00;
    double StrokeOutAvg = 0.00; //only for Mapping 4
    double StrokeOut_r = 0.00;

    //double MaxStroke = 20; //10//MAX_AMPS*AMPS_TO_VOLTS; //0.96 //we need to limit the max voltage output to a corresponding max amperage;
    double MaxStroke = MAX_TACTOR_EXTENSION; //in mm

    //desiredStroke = MaxStroke; // Send constant desiredStroke
    //StrokeOut = (desiredStroke)*convertToCM; // desiredStroke [mm] turned into StrokeOut [cm] -- Not sure why, this was from Mine


    //STROKE STUFF HEREERERERERERER !!!!!

    // StrokeOut = 5.0;
    StrokeOut = desiredStroke;     // no longer scaled Down - Jasmin's was scaled by 0.4

    if (std::isnan(StrokeOut))
    {
        StrokeOut = 0;
    }
    if (StrokeOut > MaxStroke)
    {
        StrokeOut = MaxStroke;
        StrokeOut_r = MaxStroke;
    }
    else if (StrokeOut < 0.1)
    {
        StrokeOut = StrokeOut;
        StrokeOut_r = 0.1;
    }

    //Pull mapping value from mainwindow.cpp script
    extern int mappingVal;

    double angleOut;
    uint offTime;
    uint onTime;

    #ifdef SENSORAY826
    //qDebug()<<channelNum;
    //Setting appropriate counter for the motor active in this loop
    if (channelNum == CHANNEL_NUM0)
    {
        currentServo0Stroke = StrokeOut;
    }
    if (channelNum == CHANNEL_NUM1)
    {
        currentServo1Stroke = StrokeOut;
    }
    //qDebug()<<"currentServo0Stroke: "<<currentServo0Stroke<<"currentServo1Stroke: "<<currentServo1Stroke;

    //for averaging and summing mappings
    if (mappingVal == 4)
    {
        //qDebug()<<"Mapping4 -- averaging";
        //uint setPoint = ((StrokeOut-Vmin)/(Vmax-Vmin)) * MAXSETPNT;
        StrokeOutAvg = 0.50 * (currentServo0Stroke + currentServo1Stroke);
        angleOut = (StrokeOutAvg/pitchRadius) * (180.00/PI);

        offTime = (uint)(angleOut * ((maxOffTime - zeroPosOffTime)/angleRange)) + zeroPosOffTime; //added zeropos offset
        onTime = 20000 - offTime;// 20 ms - onTime for 50 Hz PWM frequency
    }
    //Default calculations
    else
    {
        //qDebug()<<"Any other Mapping";
        //uint setPoint = ((StrokeOut-Vmin)/(Vmax-Vmin)) * MAXSETPNT;
        angleOut = (StrokeOut/pitchRadius) * (180.00/PI);

        offTime = (uint)(angleOut * ((maxOffTime - zeroPosOffTime)/angleRange)) + zeroPosOffTime; //added zeropos offset
        onTime = 20000 - offTime;// 20 ms - onTime for 50 Hz PWM frequency
    }

    //qDebug()<<"mappingVal"<<mappingVal;
    //Normal Mapping
    if(mappingVal == 1)
    {
        //***use different counter for each motor
        if (channelNum == CHANNEL_NUM0)
        {
            counter = 0; //servo0 - index - default dorsal
        }
        if (channelNum == CHANNEL_NUM1)
        {
            counter = 1; //servo1 - thumb - default ventral
        }
        SetPWM(PCI_BOARD, counter, onTime, offTime);
    }
    //Reverse Mapping
    else if(mappingVal == 2)
    {
        if (channelNum == CHANNEL_NUM0)
        {
            counter = 1; //servo1 - index - ventral
        }
        if (channelNum == CHANNEL_NUM1)
        {
            counter = 0; //servo1 - thumb - dorsal
        }
        SetPWM(PCI_BOARD, counter, onTime, offTime);
    }
    //Single Mapping
    if(mappingVal == 3)
    {
        //***use different counter for each motor
        if (channelNum == CHANNEL_NUM0)
        {
            counter = 0; //servo0 - index - default dorsal
            SetPWM(PCI_BOARD, counter, onTime, offTime);
        }
        if (channelNum == CHANNEL_NUM1)
        {
            counter = 1; //servo1 - thumb - default ventral
            SetPWM(PCI_BOARD, counter, zeroPosOnTime, zeroPosOffTime);
        }
    }
    //Averaged
    if(mappingVal == 4)
    {
        //only send to dorsal servo
        counter = 0;
        SetPWM(PCI_BOARD, counter, onTime, offTime);
    }
    //Control - No Haptic Feedback
    if(mappingVal == 5)
    {
        //***use different counter for each motor
        if (channelNum == CHANNEL_NUM0)
        {
            counter = 0; //servo0 - index - default dorsal
        }
        if (channelNum == CHANNEL_NUM1)
        {
            counter = 1; //servo1 - thumb - default ventral
        }
        SetPWM(PCI_BOARD, counter, zeroPosOnTime, zeroPosOffTime);
    }
    else
    {
        //        //***use different counter for each motor
        //        if (channelNum == CHANNEL_NUM0)
        //        {
        //            counter = 0; //servo0 - index - default dorsal
        //        }
        //        if (channelNum == CHANNEL_NUM1)
        //        {
        //            counter = 1; //servo1 - thumb - default ventral
        //        }
        //        SetPWM(PCI_BOARD, counter, onTime, offTime);

    }

    //SetPWM(PCI_BOARD, counter, onTime, offTime);
    //qDebug()<< channelNum <<"|" << counter <<"|" <<"StrokeOut:"<<StrokeOut<<"| angleOut:"<<angleOut<<"| set offTime:"<<offTime<<"| desiredStroke:"<<desiredStroke;

    /*
//    if(equalTouch){
//        if (index == true && thumb == true){
//            if(channelNum==6){
////                qDebug()<<"equal selected, reverse NOT";
//                //S826_DacDataWrite(PCI_BOARD, 6, setPoint, 0); // Stroke is sent to the actuator in the cm format
//                //S826_DacDataWrite(PCI_BOARD, 7, setPoint, 0); // Stroke is sent to the actuator in the cm format

//            }
//        }
//   }


//        if(equal){
//            if(channelNum==6){
////                qDebug()<<"equal selected, reverse NOT";
//                S826_DacDataWrite(PCI_BOARD, 6, setPoint, 0); // Stroke is sent to the actuator in the cm format
//                S826_DacDataWrite(PCI_BOARD, 7, setPoint, 0); // Stroke is sent to the actuator in the cm format
//            }
//        }

//        else if(reverse){
//                if(channelNum==6){
//    //                qDebug()<<"reverse selected, equal NOT";
//                    S826_DacDataWrite(PCI_BOARD, 7, setPoint, 0); // Stroke is sent to the actuator in the cm format
//                }
//                else if(channelNum ==7){
//                    S826_DacDataWrite(PCI_BOARD, 6, setPoint, 0); // Stroke is sent to the actuator in the cm format
//                }
//            }
//        else{
//                if(channelNum==6){
//    //                qDebug()<<"reverse and equal NOT selected";
//                    S826_DacDataWrite(PCI_BOARD, 6, setPoint, 0); // Stroke is sent to the actuator in the cm format
//                }
//                else if(channelNum ==7){
//                    S826_DacDataWrite(PCI_BOARD, 7, setPoint, 0); // Stroke is sent to the actuator in the cm format
//                }
//            }
*/

#endif

    this->strokeOutput = StrokeOut; // I want to see the Stroke in the mm format

    //qDebug()<<strokeOutput;
}
