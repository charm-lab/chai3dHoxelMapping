#ifndef VIBROTACTILE_THREAD_H
#define VIBROTACTILE_THREAD_H

#include <QObject>
#include <QThread>
#include "shared_data.h"
#include "c3DOFdevice.h"
#include "chai3d.h"
#include <stdio.h>
#include <QDir>
#include <QCoreApplication>
#include "CODE.h"
#include "Win626.h"
#include "windows.h"
#include <qDebug>
#include <string>
#include <QString>
#include <ostream>
#include <istream>
#include <fstream>
#include <iostream>
#include <sstream>
#include "windows.h"
#include <time.h>

#include <conio.h>
#include <tuple>
#include <mutex>
#include "AccSynthHashMatrix.h"
#include "TappingParameters.h"
#include "autoGenHashMatrix.h"
#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>

#include "826api.h"


#define RANGE_10V 0x00 // Range code for ADC ±10V range.
//#define RANGE_5V 0x10 // Range code for ADC ±5V range.
//#define EOPL 0x80 // ADC end-of-poll-list marker.
//#define CHANMASK 0x0F // ADC channel number mask.
#define COUNTER		CNTR_0A		// Counter A (and implicitly, B) channel to use.
#define GATE_TIME	1000		// Gate time in milliseconds.
//#define DAC_VSCALAR 32767		// Binary-to-volts scalar for DAC
#define MAXSETPNT_V 0xFFFF
#define pi 3.14159

#define SAMPLE_RATE (1000)


class Vibrotactile_thread : public QThread
{
    Q_OBJECT


public:

    // Public Functions ============================================

    // CONSTRUCTOR
    explicit Vibrotactile_thread(QObject *parent = 0);

    // DESTRUCTOR
    ~Vibrotactile_thread();

    // METHODS
    void initialize();
    double vibrations(int coeffNum, int MAcoeffNum, double filtVariance, double filtGain, double *filtCoeff, double *filtMACoeff, std::vector <double> &outputHist,std::vector <double> &excitationHist);
    static int AdcReadSlot(uint board, uint slot, int *slotdata);
#ifdef ACC
void AdcHandler(short*);
#endif
    // Public Variables ============================================
    shared_data* p_CommonData; //create a pointer to a shared_data struct
    bool playVib = false;

    int textNum = 2; //this changes the texture that is being displayed
    int playText = false;

    int ao1 = 1; // 0

    char key;
    volatile bool exitKey = false;

    uint board = 0;
    int errcode = S826_ERR_OK;

    AccSynthHashMatrix mymatrix;
    TappingParametersMatrix mytapmatrix;
    char texArray[NUM_TEX][50];

    boost::mt19937 rng;

    int iLoop = 0;

    double out0 = 0.0;
    double out1 = 0.0;

    double a1 = 0.0;
    double a2 = 0.0;

    float tLoop = 3; //5
    float elapsedTime = 0.0;

    float deltaT = 0.0;


    float tapSpeed = 100; //speed of contact (mm/s), for interpolating between taps; value to come from your code
    float tangentSpeed = 150; //speed of dragging across the surface (mm/s); this will come from your code
    float normForce = 1.5; //normal force during dragging (N); change with your code
    double output; //this will be the calculated texture vibration value
    float texGain = 0.05; //change this for your voicecoil; scales m/s^2 to V
    double *taps0;
    double *taps1;
    double filtCoeff[MAX_COEFF];
    double filtMACoeff[MAX_MACOEFF];
    double filtVariance;
    double filtGain;
    int coeffNum;
    int MAcoeffNum;
    int iMax;
    std::vector <double> outputHist;
    std::vector <double> excitationHist;
    float A; //V
    float A0; //V
    float A1; //V
    float Omega; //Hz
    float Beta;
    float VibOutPut0;
    float CurrentTime0;
    float StartTime0;
    float VibOutPut1;
    float CurrentTime1;
    float StartTime1;
    short xyzData;
protected:
    void run();

};
#endif // VIBROTACTILE_THREAD_H
