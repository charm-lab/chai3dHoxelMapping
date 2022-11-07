#include "Vibrotactile_thread.h"
#define SAMPLING_PERIOD 500 ;        // Sampling period in microseconds (50000 = 20 samples/s).
//#define TSETTLE 49997;  // Compensate for nominal ADC conversion time.
#define TSETTLE (SAMPLING_PERIOD - 3);
#define SAMPLE_RATE (1000);
//-6600 = 9.8
//-8800 = -9.8
//m = 0.00890909
//b = 68.6
class Timer {
public:
    Timer() {
        reset();
    }
    /// reset() makes the timer start over counting from 0.0 seconds.
    void reset() {
        unsigned __int64 pf;
        QueryPerformanceFrequency((LARGE_INTEGER *)&pf);
        freq_ = 1.0 / (double)pf;
        QueryPerformanceCounter((LARGE_INTEGER *)&baseTime_);
    }
    /// seconds() returns the number of seconds (to very high resolution)
    /// elapsed since the timer was last created or reset().
    double seconds() {
        unsigned __int64 val;
        QueryPerformanceCounter((LARGE_INTEGER *)&val);
        return (val - baseTime_) * freq_;
    }

    /// seconds() returns the number of milliseconds (to very high resolution)
    /// elapsed since the timer was last created or reset().
    double milliseconds() {
        return seconds() * 1000.0;
    }
private:
    double freq_;
    unsigned __int64 baseTime_;
};

Timer t;
//double StartTime = 0;
//double CurrentTime = 0;
int channelNum_vib0 = 7;
int channelNum_vib1 = 6;
int channelNum_acc0 = 1;

#ifdef ACC
void Vibrotactile_thread::AdcHandler(short *xyzData)
{
  int errcode;
  int slotval[16];  // buffer must be sized for 16 slots
  while (errcode != S826_ERR_OK) {
    uint slotlist = 1;  // only slot 0 is of interest in this example
    errcode = S826_AdcRead(0, slotval, NULL, &slotlist, S826_WAIT_INFINITE); // wait for IRQ
    if (errcode != S826_ERR_OK)
        qDebug()<<"WTF NOW";
      //break;
    //printf("Raw adc data = %d", slotval[0] & 0xFFFF);
    p_CommonData->Acc=(short)(slotval[0]& 0xFFFF);
    *xyzData = (short) (slotval[0] & 0xFFFF);
    //qDebug()<<(short)slotdata;
  }
}
#endif
Vibrotactile_thread::Vibrotactile_thread(QObject *parent) : QThread(parent)
{

}

Vibrotactile_thread::~Vibrotactile_thread()
{

}

void Vibrotactile_thread::initialize()
{
    qDebug()<<"Starting application.\n";
p_CommonData->VibroTactileThreadActive=true;
p_CommonData->playVib0=false;
p_CommonData->playingVib0=false;
p_CommonData->playVib1=false;
p_CommonData->playingVib1=false;

//    int fail0 = S826_DacRangeWrite(PCI_BOARD, channelNum_vib0, 3, 0);      // program dac output range: -10V to +10V
//    if (fail0 == 0){
//    qDebug() << "DAC" << channelNum_vib0 << "init'ed for vibration";
//    }
//    int fail1 = S826_DacRangeWrite(PCI_BOARD, channelNum_vib1, 3, 0);      // program dac output range: -10V to +10V
//    if (fail1 == 0){
//    qDebug() << "DAC" << channelNum_vib1 << "init'ed for vibration";
//    }

    #ifdef ACC
    // Configure the ADC subsystem and start it running
    S826_AdcSlotConfigWrite(PCI_BOARD, 0, 0, 497, 0); // measuring ain 0 on slot 0
    S826_AdcSlotConfigWrite(PCI_BOARD, 1, 0, 497, 0); // measuring ain 0 on slot 1
    S826_AdcSlotConfigWrite(PCI_BOARD, 2, 0, 497, 0); // measuring ain 0 on slot 2
    S826_AdcSlotlistWrite(PCI_BOARD, 1, S826_BITWRITE);                 // enable slot 0
    S826_AdcTrigModeWrite(PCI_BOARD, 0);                                // trigger mode = continuous
    S826_AdcEnableWrite(PCI_BOARD, 1);
    qDebug()<<"Acceleration Measurements";
    #endif


    // Load texture file
#ifdef VIBRATION
    //menuInterface();
    qDebug()<<"Before Hash Matrix";
    AccSynthHashMatrix thematrix = generateHashMatrix(texArray); //function in "autoGenHashMatrix.h"
    qDebug()<<"After Hash Matrix";

    mymatrix = thematrix;

    qDebug()<<"Before Tap Matrix";
    mytapmatrix = generateTapMatrix(texArray); //function in "autoGenHashMatrix.h"
    qDebug()<<"Texture matrix created successfully!\n";
#endif

#ifdef SIMPLE_VIB

#endif


}

void Vibrotactile_thread::run()
{

        char outFileString[12] = "testing.csv";
        //char outFileString[100];
        FILE *outFilePtr;
        outFilePtr = fopen(outFileString, "w+");


    int Vmin = 0;
    int Vmax =  1.8;
    int iCount0 = 0;
    int iCount1 = 0;
    p_CommonData->texture=5;
    double maxx0=0;
    double maxx1=0;
#ifdef SIMPLE_VIB
    while(p_CommonData->VibroTactileThreadActive){
        A=p_CommonData->SDVibAmp/10; //V
        Omega=p_CommonData->SDVibFreq; //Hz
        Beta=-1*p_CommonData->SDVibBeta;
        if (p_CommonData->playVib0 && p_CommonData->playingVib0 == false)
        { //play vibration
            p_CommonData->playVib0=false;
            p_CommonData->playingVib0=true;
            StartTime0=p_CommonData->overallClock.getCurrentTimeSeconds();
            iMax = 100;
            taps0 = mytapmatrix.HashAndInterp2(p_CommonData->texture, p_CommonData->Velocity0.length()*1000); // interpolates entire 100 ms long tap; function in "TappingParameters.cpp"
            A0=p_CommonData->Velocity0.length()*A;
            A0=A;
            maxx0=0;

        }
        CurrentTime0=p_CommonData->overallClock.getCurrentTimeSeconds()-StartTime0;

        if (p_CommonData->playVib1 && p_CommonData->playingVib1 == false)
        {   //play vibration
            p_CommonData->playVib1=false;
            p_CommonData->playingVib1=true;
            StartTime1=p_CommonData->overallClock.getCurrentTimeSeconds();
            iMax = 100;
            taps1 = mytapmatrix.HashAndInterp2(p_CommonData->texture, p_CommonData->Velocity1.length()*1000); // interpolates entire 100 ms long tap; function in "TappingParameters.cpp"
            A1=p_CommonData->Velocity1.length()*A;
            A1=A;
            maxx1=0;
        }
        CurrentTime1=p_CommonData->overallClock.getCurrentTimeSeconds()-StartTime1;


        if (p_CommonData->playingVib0)
        {   //playing vibration on 0
            if (CurrentTime0>.1)
            { // .3 second
                p_CommonData->playingVib0=false;
                //qDebug()<<"Vibration Done 0. Max: "<<maxx0;

            }
            VibOutPut0=A0*sin(2*pi*Omega*CurrentTime0)*exp(Beta*CurrentTime0)+A0/2;

            if (p_CommonData->Decaying==false)
            {
                out0=taps0[int(CurrentTime0*1000)]*1.8/200;
                VibOutPut0=out0;
            }
            if (maxx0<abs(VibOutPut0))
            {
                maxx0=abs(VibOutPut0);
            }
        }
        else
        {
            VibOutPut0=0;
        }

        if (p_CommonData->playingVib1)
        {  //playing vibration on 1
            if (CurrentTime1>0.1){ // 1 second
                p_CommonData->playingVib1=false;
                 //qDebug()<<"Vibration Done 1. Max: "<<maxx1;
            }
            VibOutPut1=A1*sin(2*pi*Omega*CurrentTime1)*exp(Beta*CurrentTime1)+A1/2;

            if (p_CommonData->Decaying==false)
            {
                out1=taps1[int(CurrentTime1*1000)]*1.8/200;
                VibOutPut1=out1;
            }
            if (maxx1<abs(VibOutPut1))
            {
                maxx1=abs(VibOutPut1);
            }
        }
        else
        {
            VibOutPut1=0;
        }

        if (VibOutPut0>Vmax)
        {
            VibOutPut0=Vmax;
        }
        else if(VibOutPut0<Vmin)
        {
            VibOutPut0=Vmin;
        }

    if (VibOutPut1>Vmax)
    {
        VibOutPut1=Vmax;
    }
    else if(VibOutPut1<Vmin)
    {
        VibOutPut1=Vmin;
    }

    uint setPoint0 = (VibOutPut0-Vmin)/(Vmax-Vmin) * MAXSETPNT_V;
    uint setPoint1 = (VibOutPut1-Vmin)/(Vmax-Vmin) * MAXSETPNT_V;
#ifdef SENSORAY826
//    if (p_CommonData->VT_Feedback){
//    S826_DacDataWrite(PCI_BOARD, channelNum_vib0, setPoint0, 0);
//    S826_DacDataWrite(PCI_BOARD, channelNum_vib1, setPoint1, 0);
//    }

#ifdef ACC
    AdcHandler(&xyzData);   // Handle periodic ADC interrupts (using code from earlier example)
    //m = 0.00890909
    //b = 68.6
    fprintf(outFilePtr, "%f, %f, %i, %i, %i\n", p_CommonData->overallClock.getCurrentTimeSeconds(),p_CommonData->Acc, p_CommonData->playingVib0, p_CommonData->ImpulseVib,p_CommonData->VT_Feedback);
    //p_CommonData->Acc=(short)slotdata;
    //qDebug()<<(short)slotdata;
#endif
#endif
#endif




#ifdef VIBRATION

    //qDebug()<<"Choose signal to play\n";
    while (false)
    {
        while (p_CommonData->playVib1)
        {
            qDebug()<<"playing vib";
            if (playText)
            {
                iMax = tLoop*SAMPLE_RATE;
                mymatrix.HashAndInterp2(textNum, tangentSpeed, normForce, filtCoeff, filtMACoeff); // call when force and speed are updated, doesn't have to be called each loop; function definition in "AccSynthHashMatrix.cpp"
                std::tie(coeffNum, MAcoeffNum, filtVariance, filtGain) = mymatrix.HashAndInterp2(textNum, tangentSpeed, normForce, filtCoeff, filtMACoeff); // call when force and speed are updated, doesn't have to be called each loop; function definition in "AccSynthHashMatrix.cpp"
            }
            else
            {
                qDebug()<<"playing vib";
                iMax = 100;
                taps1 = mytapmatrix.HashAndInterp2(textNum, tapSpeed); // interpolates entire 100 ms long tap; function in "TappingParameters.cpp"
            qDebug()<<"playing vib";
            }
            //for (iLoop = 0; iLoop < tLoop*SAMPLE_RATE; iLoop++)
            for (iLoop = 0; iLoop < iMax; iLoop++)
            {
                //StartTime = t.seconds();
                deltaT = 0.0;

                while (deltaT <= 0.001)
                {
                //    CurrentTime = t.seconds();
                 //   deltaT = CurrentTime - StartTime;
                }

                //CurrentTime = t.seconds();

                if (iLoop == 0) // timing messed up on first time through loop
                    elapsedTime = 0.001;
                else
                    elapsedTime = elapsedTime + deltaT;

                if (playText)
                {
                    out1 = texGain*vibrations(coeffNum, MAcoeffNum, filtVariance, filtGain, filtCoeff, filtMACoeff, outputHist, excitationHist); // needs to be called in loop at 1000 Hz when user is touching the surface; then scale output and play to voicecoil
                }
                else
                {
                    out1 = taps1[iLoop] * 0.05;
                }
                //std::cout << out1 << std::endl;

                // do something
                //p_CommonData->VibrationOutput = (uint)(DAC_VSCALAR + out1 / 5.0*DAC_VSCALAR);
                //S826_DacDataWrite(board, channelNum, (uint)(DAC_VSCALAR + out1 / 5.0*DAC_VSCALAR), 0);

                //CurrentTime = t.seconds();
            }
            playVib = false;
            //qDebug()<<"Choose signal to play\n";
        }
        elapsedTime = 0.0;

        //S826_DacDataWrite(board, channelNum, (uint)(DAC_VSCALAR), 0);
    }
while (false)
{
    // Program all analog outputs to zero volts.
    //S826_DacDataWrite(board, channelNum, (uint)(DAC_VSCALAR), 0);

   // S826_SystemClose();
}

#endif
}
}


double Vibrotactile_thread::vibrations(int coeffNum, int MAcoeffNum, double filtVariance, double filtGain, double *filtCoeff, double *filtMACoeff, std::vector <double> &outputHist,std::vector <double> &excitationHist)
{
    double output = 0.0;
    double excitation = 0.0;
    double rgen_mean=0.;
    boost::mt19937 generator;

    //generate Gaussian random number with power equal to interpolation model variance
        boost::normal_distribution<> nd(rgen_mean, sqrt(filtVariance));
        boost::variate_generator<boost::mt19937&,
        boost::normal_distribution<> > var_nor(rng, nd);
        excitation = var_nor();
        output = 0.0;

        //if the size of output history is less than the number of AR coefficients, append zeros
        if(outputHist.size()<(unsigned int) MAX_COEFF)
        {
            int subt = MAX_COEFF - outputHist.size();
            for(int j = 0; j < subt ; j++)
            {
                outputHist.push_back(0.0);
            }
        }
        //if the size of excitation history is less than the number of MA coefficients, append zeros
        if(excitationHist.size()<(unsigned int) MAX_MACOEFF)
        {
            int subt = MAX_MACOEFF - excitationHist.size();
            for(int j = 0; j < subt ; j++)
            {
                excitationHist.push_back(0.0);
            }
        }

        //apply AR coefficients to history of output values
        for(int i = 0; i < coeffNum; i++)
        {
            output += outputHist.at(i) * (-filtCoeff[i]);
        }
        //if applicable, also apply MA coefficients to history of excitation values
        if(isARMA)
        {
            output += excitation*filtGain;
            for(int i = 0; i < MAcoeffNum; i++)
            {
                output += excitationHist.at(i) * (filtMACoeff[i])*filtGain;
            }

            } else
        {
            output += excitation;
        }

        //if the size of output history is greater than the number of AR coefficients, make the extra values zero so we're not storing junk
        if(outputHist.size()>(unsigned int) coeffNum)
        {
            for(unsigned int kk = coeffNum; kk < outputHist.size(); kk++)
            outputHist.at(kk) = 0.0;
        }
        //if the size of excitation history is greater than the number of MA coefficients, make the extra values zero so we're not storing junk
        if(excitationHist.size()>(unsigned int) MAcoeffNum)
        {
            for(unsigned int kk = MAcoeffNum; kk < excitationHist.size(); kk++)
            excitationHist.at(kk) = 0.0;
        }

    // remove the last element of our output vector
    outputHist.pop_back();
    excitationHist.pop_back();
    // push our new ouput value onto the front of our vector stack
    outputHist.insert(outputHist.begin(),output);
    excitationHist.insert(excitationHist.begin(),excitation);

    return output; //this is the output vibration value (in m/s^2)
}
