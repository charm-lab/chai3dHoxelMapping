/***********************************************************************************************************************************
COPYRIGHT AND PERMISSION NOTICE
Penn Software The Penn Haptic Texture Toolkit
Copyright (C) 2013 The Trustees of the University of Pennsylvania
All rights reserved.

See copyright and permission notice at the end of this file.

Report bugs to Heather Culbertson (hculb@seas.upenn.edu, +1 215-573-6748) or Katherine J. Kuchenbecker (kuchenbe@seas.upenn.edu, +1 215-573-2786)

This code is based on the original TexturePad haptic rendering system designed by Joseph M. Romano.
************************************************************************************************************************************/

#include "TappingParameters.h"
#include "shared.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "autoGenHashMatrix.h"
#include <qDebug>

#include <iostream>
#include <complex>

using namespace std;

AccSynthHashMatrix generateHashMatrix(char texArray[][50])
{

AccSynthHashMatrix tempmatrix(NUM_TEX);

double allSpeeds[200]; //allocate memory for arrays to store lists of speeds and forces
double allForces[200];
int DT1[200]; //allocate memory for arrays to store lists of Delaunay triangulation vertices
int DT2[200];
int DT3[200];
double modLSF[MAX_COEFF]; //allocate memory for arrays to store AR and MA LSFs
double modMALSF[MAX_MACOEFF];

int numMod;
int numTri;
double maxSpeed;
double maxForce;
double mu;

int numCoeff;
int numMACoeff;
double variance;
double gain;
double modSpeed;
double modForce;

string imFilename;

//Find filepath of executable
//char result[100];
//strcpy(result,"blah");
////GetModuleFileName(NULL,LPWSTR(result),100);
//string::size_type pos1 = string(result).find_last_of( "\\/" );
//string exeFilepath = string(result).substr(0,pos1);
//string::size_type pos2 = exeFilepath.find_last_of( "\\/" );

//Find filepath of XML model files
string baseFilename = "C:\\Users\\Sam\\Desktop\\chai3dFingerMapping\\build\\XML\\Models_";
string myFilename;

/********* Fill array with known texture names ***********/
// divide textures by group

//Paper
 qDebug()<<"here";
 char testtest[50] = {0};
strcpy(&texArray[0][0],"Book");
 qDebug()<<"here";
strcpy(&texArray[1][0],"Bubble Envelope");
strcpy(&texArray[2][0],"Cardboard");
strcpy(&texArray[3][0],"Coffee Filter");
strcpy(&texArray[4][0],"Dot Paper");
strcpy(&texArray[5][0],"Folder");
strcpy(&texArray[6][0],"Gift Box");
strcpy(&texArray[7][0],"Glitter Paper");
strcpy(&texArray[8][0],"Greeting Card");
strcpy(&texArray[9][0],"Masking Tape");
strcpy(&texArray[10][0],"Paper Bag");
strcpy(&texArray[11][0],"Paper Plate 1");
strcpy(&texArray[12][0],"Paper Plate 2");
strcpy(&texArray[13][0],"Playing Card");
strcpy(&texArray[14][0],"Resume Paper");
strcpy(&texArray[15][0],"Sandpaper 100");
strcpy(&texArray[16][0],"Sandpaper 220");
strcpy(&texArray[17][0],"Sandpaper 320");
strcpy(&texArray[18][0],"Sandpaper Aluminum Oxide");
strcpy(&texArray[19][0],"Textured Paper");
strcpy(&texArray[20][0],"Tissue Paper");
strcpy(&texArray[21][0],"Wax Paper");

//Plastic
strcpy(&texArray[22][0],"ABS Plastic");
strcpy(&texArray[23][0],"Binder");
strcpy(&texArray[24][0],"Candle");
strcpy(&texArray[25][0],"File Portfolio");
strcpy(&texArray[26][0],"Frosted Acrylic");
strcpy(&texArray[27][0],"Nitrile Glove");
strcpy(&texArray[28][0],"Plastic Mesh 1");
strcpy(&texArray[29][0],"Plastic Mesh 2");
strcpy(&texArray[30][0],"Tarp");
strcpy(&texArray[31][0],"Wavy Acrylic");

//Fabric
strcpy(&texArray[32][0],"Athletic Shirt");
strcpy(&texArray[33][0],"Blanket");
strcpy(&texArray[34][0],"CD Sleeve");
strcpy(&texArray[35][0],"Canvas 1");
strcpy(&texArray[36][0],"Canvas 2");
strcpy(&texArray[37][0],"Canvas 3");
strcpy(&texArray[38][0],"Cotton");
strcpy(&texArray[39][0],"Denim");
strcpy(&texArray[40][0],"Felt");
strcpy(&texArray[41][0],"Flannel");
strcpy(&texArray[42][0],"Fleece");
strcpy(&texArray[43][0],"Leather 1 Back");
strcpy(&texArray[44][0],"Leather 1 Front");
strcpy(&texArray[45][0],"Leather 2 Back");
strcpy(&texArray[46][0],"Leather 2 Front");
strcpy(&texArray[47][0],"Microfiber Cloth");
strcpy(&texArray[48][0],"Nylon Bag");
strcpy(&texArray[49][0],"Nylon Mesh");
strcpy(&texArray[50][0],"Pleather");
strcpy(&texArray[51][0],"Portfolio Cover");
strcpy(&texArray[52][0],"Silk 1");
strcpy(&texArray[53][0],"Silk 2");
strcpy(&texArray[54][0],"Textured Cloth");
strcpy(&texArray[55][0],"Towel");
strcpy(&texArray[56][0],"Velcro Hooks");
strcpy(&texArray[57][0],"Velcro Loops");
strcpy(&texArray[58][0],"Velvet");
strcpy(&texArray[59][0],"Vinyl 1");
strcpy(&texArray[60][0],"Vinyl 2");
strcpy(&texArray[61][0],"Whiteboard Eraser");

//Tile
strcpy(&texArray[62][0],"Floortile 1");
strcpy(&texArray[63][0],"Floortile 2");
strcpy(&texArray[64][0],"Floortile 3");
strcpy(&texArray[65][0],"Floortile 4");
strcpy(&texArray[66][0],"Floortile 5");
strcpy(&texArray[67][0],"Floortile 6");
strcpy(&texArray[68][0],"Floortile 7");

//Carpet
strcpy(&texArray[69][0],"Artificial Grass");
strcpy(&texArray[70][0],"Carpet 1");
strcpy(&texArray[71][0],"Carpet 2");
strcpy(&texArray[72][0],"Carpet 3");
strcpy(&texArray[73][0],"Carpet 4");

//Foam
strcpy(&texArray[74][0],"EPDM Foam");
strcpy(&texArray[75][0],"Pink Foam");
strcpy(&texArray[76][0],"Polyethylene Foam");
strcpy(&texArray[77][0],"Scouring Pad");
strcpy(&texArray[78][0],"Styrofoam");
strcpy(&texArray[79][0],"Textured Rubber");

//Metal
strcpy(&texArray[80][0],"Aluminum Foil");
strcpy(&texArray[81][0],"Aluminum");
strcpy(&texArray[82][0],"Metal Mesh");
strcpy(&texArray[83][0],"Metal Shelving");
strcpy(&texArray[84][0],"Textured Metal");
strcpy(&texArray[85][0],"Whiteboard");

//Stone
strcpy(&texArray[86][0],"Brick 1");
strcpy(&texArray[87][0],"Brick 2");
strcpy(&texArray[88][0],"Ceramic");
strcpy(&texArray[89][0],"Painted Brick");
strcpy(&texArray[90][0],"Stone Tile 1");
strcpy(&texArray[91][0],"Stone Tile 2");
strcpy(&texArray[92][0],"Terra Cotta");

//Carbon Fiber
strcpy(&texArray[93][0],"Carbon Fiber");
strcpy(&texArray[94][0],"Resin Carbon Fiber");

//Wood
strcpy(&texArray[95][0],"Cork");
strcpy(&texArray[96][0],"MDF");
strcpy(&texArray[97][0],"Painted Wood");
strcpy(&texArray[98][0],"Stained Wood");
strcpy(&texArray[99][0],"Wood");

int countMod;

// Loop through all textures
for (int numSurf=0;numSurf<NUM_TEX;numSurf++)
{
    //qDebug()<<"here";
    qDebug()<<numSurf<<" of "<<NUM_TEX;
    pugi::xml_document doc;

    myFilename = baseFilename + texArray[numSurf] + ".xml"; // get full filename of model file

    pugi::xml_parse_result result = doc.load_file(myFilename.c_str()); // load model file
    //qDebug()<< "Load result: " << result.description();

    pugi::xml_node modelSet = doc.child("modelSet");

    mu = atof(modelSet.child_value("mu")); //friction coefficient

    numMod = atoi(modelSet.child_value("numMod")); //number of models

    numTri = atoi(modelSet.child_value("numTri")); //number of triangles in Delaunay triangulation

    maxSpeed = atof(modelSet.child_value("maxSpeed")); //maximum modeled speed

    maxForce = atof(modelSet.child_value("maxForce")); //maximum modeled force

    //list of all model speeds
    int count = 0;
    for (pugi::xml_node speedList = modelSet.child("speedList").child("value"); speedList; speedList = speedList.next_sibling("value"))
    {
        allSpeeds[count] = atof(speedList.child_value());
        count++;
    }

    //list of all model forces
    count = 0;
    for (pugi::xml_node forceList = modelSet.child("forceList").child("value"); forceList; forceList = forceList.next_sibling("value"))
    {
        allForces[count] = atof(forceList.child_value());
        count++;
    }

    //list of all triangles in Delaunay triangulation
    count = 0;
    for (pugi::xml_node tri = modelSet.child("tri"); tri; tri = tri.next_sibling("tri"))
    {
        pugi::xml_node triChild = tri.child("value");
        DT1[count] = atoi(triChild.child_value());
        DT2[count] = atoi(triChild.next_sibling("value").child_value());
        DT3[count] = atoi(triChild.next_sibling("value").next_sibling("value").child_value());
      count++;
    }


    numCoeff = atoi(modelSet.child_value("numARCoeff")); //number of AR coefficients
    //qDebug()<<numCoeff;

    numMACoeff = atoi(modelSet.child_value("numMACoeff")); //number of MA coefficients
    /*if (numMACoeff == 0)
        isARMA = false;
    else
        isARMA = true;*/

    //create a hash table for this surface
    tempmatrix.AddTable(numSurf,numMod,allSpeeds,allForces);

    countMod = 0;
    //for each model in the file
    for (pugi::xml_node model = modelSet.child("model"); model; model = model.next_sibling("model"))
    {

        //read all AR LSFs
        count = 0;
        pugi::xml_node ARlsf = model.child("ARlsf");
        for (pugi::xml_node ARval = ARlsf.child("value"); ARval; ARval = ARval.next_sibling("value"))
        {
         modLSF[count] = atof(ARval.child_value());
         count++;
        }

       //read all MA LSFs (if needed)
        if(isARMA)
        {
         count = 0;
         pugi::xml_node MAlsf = model.child("MAlsf");
         for (pugi::xml_node MAval = MAlsf.child("value"); MAval; MAval = MAval.next_sibling("value"))
         {
          modMALSF[count] = atof(MAval.child_value());
          count++;
         }
         gain = atof(model.child("gain").child_value());
        }

        variance = atof(model.child("var").child_value()); //model variance
        modSpeed = atof(model.child("speedMod").child_value()); //model speed
        modForce = atof(model.child("forceMod").child_value()); //model force

        countMod++;

        //create a hash entry for each model
        if(isARMA){
        AccSynthHashEntry HashEntry(numSurf,modForce,modSpeed,modLSF,modMALSF,variance,gain,numCoeff,numMACoeff,numTri,numMod,DT1,DT2,DT3,maxSpeed,maxForce,mu);
        tempmatrix.AddEntry(HashEntry,numMod,allSpeeds,allForces);}
        else{
        AccSynthHashEntry HashEntry(numSurf,modForce,modSpeed,modLSF,variance,numCoeff,numTri,numMod,DT1,DT2,DT3,maxSpeed,maxForce,mu);
        tempmatrix.AddEntry(HashEntry,numMod,allSpeeds,allForces);}

    }
//qDebug()<<countMod;
} // end loop through all textures

//qDebug()<<tempmatrix.hashTable[99].hashMap[15].maxSpeed;
qDebug()<<"done";
return tempmatrix;
}


//int generateTapMatrix(char (*texArray)[50])
TappingParametersMatrix generateTapMatrix(char (*texArray)[50])
{
    qDebug()<<"begining";
    double * allSpeeds;
    double tapSpeed;
    double aTap[tapL];
    int numTap;

    //Find filepath of executable
//    char result[100];
//    GetModuleFileName(NULL,LPWSTR(result),100);
//    string::size_type pos1 = string(result).find_last_of( "\\/" );
//    string exeFilepath = string(result).substr(0,pos1);
//    string::size_type pos2 = exeFilepath.find_last_of( "\\/" );

    //Find filepath of XML model files
    //string baseFilename = exeFilepath.substr(0,pos2) + "\\build\\XML\\Taps_";
    //string baseFilename = exeFilepath.substr(0,pos2) + "\\build\\TappingOrig\\Taps_";
   // string baseFilename = exeFilepath.substr(0,pos2) + "\\build\\TappingShift\\Taps_";
    string baseFilename = "C:\\Users\\Sam\\Desktop\\chai3dFingerMapping\\build\\TappingShift\\Taps_";

    string myFilename;

    TappingParametersMatrix tempmatrix1(NUM_TEX);

    // Loop through all textures

    for (int numSurf=0;numSurf<NUM_TEX;numSurf++)
    {

        //qDebug()<<numSurf<<" of "<<NUM_TEX;
        pugi::xml_document doc;

        myFilename = baseFilename + texArray[numSurf] + ".xml"; // get full filename of tap file

        pugi::xml_parse_result result = doc.load_file(myFilename.c_str()); // load tap file

        pugi::xml_node tapSet = doc.child("tapSet");

        //number of taps
        numTap = atoi(tapSet.child_value("numTap")); //number of models
        allSpeeds = new double [numTap];

        //list of all model speeds
        int count = 0;
        for (pugi::xml_node speedList = tapSet.child("speedList").child("value"); speedList; speedList = speedList.next_sibling("value"))
        {
            allSpeeds[count] = atof(speedList.child_value());
            count++;
        }

        /*for (int ns = 0; ns<NUM_TAP; ns++)
            allSpeeds[ns] = tapSpeed[ns];*/

        //create a hash table for this surface
        tempmatrix1.AddTable(numSurf,numTap,allSpeeds);

        for (pugi::xml_node tap = tapSet.child("tap"); tap; tap = tap.next_sibling("tap"))
        {
            tapSpeed = atof(tap.child("tapSpeed").child_value()); //tap speed

            //read all tap accelerations
            count = 0;
            pugi::xml_node tapAccel = tap.child("tapAccel");
            for (pugi::xml_node tapval = tapAccel.child("value"); tapval; tapval = tapval.next_sibling("value"))
            {
                aTap[count] = atof(tapval.child_value());
                count++;
            }

            TappingParametersEntry TapEntry(numSurf,numTap,tapSpeed,aTap);
            tempmatrix1.AddEntry(TapEntry,numTap,allSpeeds);

        }

        /*for (int numTap = 0; numTap<NUM_TAP; numTap++)
        {
            speedTap = tapSpeed[numTap];
            for(int count = 0; count<tapL; count++)
                aTap[count] = tapAccel_1[count]; //////////////////////////////////////////////////////////////////////////////

            TappingParametersEntry TapEntry(numSurf,speedTap,aTap);
            tempmatrix.AddEntry(TapEntry,NUM_TAP,allSpeeds);
        }*/

    }// end loop through all textures

qDebug()<<"end";
return tempmatrix1;

}

/******************************************************************************
  TappingParametersEntry default ctor
******************************************************************************/
TappingParametersEntry::TappingParametersEntry()
{
    dummy = true;
}
/******************************************************************************
  TappingParametersEntry constructor
******************************************************************************/
TappingParametersEntry::TappingParametersEntry(int mysurfType, int mynumtap, double myspeed, double *myTap)	
{
    tapSpeed = myspeed; //incoming tapping speed
	numTap = mynumtap;
    surfType = mysurfType; //surface number (0-99)

    tapAccel = new double [tapL]; //allocates memory for arrays to store model vertex information for Delaunay triangulation

    //Get the tapping accelerations
    for(int i =0; i< tapL; i++){
		tapAccel[i] = myTap[i];
    }
}

/******************************************************************************
  Interpolation between tapping models
******************************************************************************/
// method to hash some values and change the appropriate shared memory to reflect this query
//void TappingParametersTable::HashAndInterp2(float interpSpeed)
double * TappingParametersTable::HashAndInterp2(double interpSpeed)
{
	double sAbove;
	double sBelow;
	bool foundS = false;
	int numTap = tapMap[0].numTap;
	double * tapSpeeds;
	tapSpeeds = new double [numTap];
	int sCount=0;

	static double tempTap[100];

	for(int kk=0;kk<numTap;kk++)
		tapSpeeds[kk] = tapMap[kk].tapSpeed;

	if(interpSpeed>=tapSpeeds[numTap-1]) //if greater than maximum speed)
	{
		for(int kk=0;kk<tapL;kk++)
			tempTap[kk] = tapMap[numTap-1].tapAccel[kk];
		foundS = true;
	}
	if(interpSpeed<=tapSpeeds[0]) //if less than minimum speed
	{
		for(int kk=0;kk<tapL;kk++)
			tempTap[kk] = (interpSpeed/tapSpeeds[0])*tapMap[0].tapAccel[kk];
		foundS = true;
	}

	while(!foundS)
	{
		if(interpSpeed<tapSpeeds[sCount])
		{
			sAbove = tapSpeeds[sCount];
			sBelow = tapSpeeds[sCount-1];
			for(int kk=0;kk<tapL;kk++){
				tempTap[kk] = tapMap[sCount-1].tapAccel[kk] + (interpSpeed-sBelow)/(sAbove-sBelow)*(tapMap[sCount].tapAccel[kk]-tapMap[sCount-1].tapAccel[kk]);
			}
			foundS = true;
		}
				
		sCount++;
	}

	return tempTap;
}

/******************************************************************************
  TappingParametersEntry default destructor
******************************************************************************/
TappingParametersEntry::~TappingParametersEntry()
{}

/******************************************************************************
  TappingParametersTable default ctor
******************************************************************************/
TappingParametersTable::TappingParametersTable()
{}


/******************************************************************************
  TappingParametersTable Constructor for creating blank tables with speed array
******************************************************************************/
TappingParametersTable::TappingParametersTable(int surfNum, int numTaps, double speedTap[])
{
    // generate the memory for an appropriately sized table

        tapMap = new TappingParametersEntry[numTaps];

    // populate our table with dummy force and speed entries
    for(int k=0; k<numTaps; k++)
    {
		(tapMap[k]).surfType = surfNum;
		(tapMap[k]).tapSpeed = speedTap[k];
    }

}

/******************************************************************************
  AccSynthHashTable default destructor
******************************************************************************/
TappingParametersTable::~TappingParametersTable()
{
    //NOTE! We don't need to delete the memory used here since we take care of it in the AccSynthHashMatrix destructor.
    //NOTE! It would have been nice to delete it here, but the delete keyword does not handle nested class deletes nicely enough to trust it
}

/******************************************************************************
  Return the force hash value BELOW our selected entry (used for placement of
  models in storage)
******************************************************************************/
int TappingParametersTable::speedTapVal(TappingParametersEntry tapEntry, int numTaps, double speedTap[])
{
	int retval = 0;

	for (int i=0;i<numTaps;i++)
	{
	    if (tapEntry.tapSpeed==speedTap[i])
	    {
	        retval = i;
			break;
	    }
	}
    
    return retval;
}

/******************************************************************************
  Add entry to TappingParametersTable
******************************************************************************/
void TappingParametersTable::AddEntry(TappingParametersEntry tapEntry, int numTaps, double speedTap[])
{
    (tapMap[speedTapVal(tapEntry, numTaps, speedTap)]) = tapEntry;
}

/******************************************************************************
  Constructor to create a blank Hash Matrix full of n HashTables for each n surface
******************************************************************************/
TappingParametersMatrix::TappingParametersMatrix(int numSurfaces)
{
    // store the # of surfaces in our hash table matrix
    numSurf = numSurfaces;

    // initialize an array to hold a hash table for each surface
    tapTable = new TappingParametersTable[numSurfaces];

    fflush(stdin);
}

/******************************************************************************
  Empty constructor
******************************************************************************/
TappingParametersMatrix::TappingParametersMatrix()
{}

/******************************************************************************
  Add table to TappingParametersMatrix
******************************************************************************/
void TappingParametersMatrix::AddTable(int surfNum, int numTaps, double speedTap[])
{
   tapTable[surfNum] = TappingParametersTable(surfNum,numTaps,speedTap);
   fflush(stdin);
}

/******************************************************************************
  TappingParametersMatrix destructor
******************************************************************************/
TappingParametersMatrix::~TappingParametersMatrix()
{

}


/******************************************************************************
  Add entry to AccSynthHashMatrix
******************************************************************************/
void TappingParametersMatrix::AddEntry(TappingParametersEntry tapEntry, int numTaps, double speedTap[])
{
    tapTable[tapEntry.surfType].AddEntry(tapEntry, numTaps, speedTap);
}

/******************************************************************************
  Handles interpolate between models
******************************************************************************/
double * TappingParametersMatrix::HashAndInterp2(int interpSurf, double interpSpeed)
{
	double *taps;

    taps = tapTable[interpSurf].HashAndInterp2(interpSpeed);

	return taps;
}

/***********************************************************************************************************************************
COPYRIGHT AND PERMISSION NOTICE
Penn Software The Penn Haptic Texture Toolkit
Copyright (C) 2013 The Trustees of the University of Pennsylvania
All rights reserved.

The Trustees of the University of Pennsylvania (“Penn”) and Heather Culbertson, Juan Jose Lopez Delgado, and Katherine J. Kuchenbecker, the developer (“Developer”) of Penn Software The Penn Haptic Texture Toolkit (“Software”) give recipient (“Recipient”) and Recipient’s Institution (“Institution”) permission to use, copy, and modify the software in source and binary forms, with or without modification for non-profit research purposes only provided that the following conditions are met:

1)	All copies of Software in binary form and/or source code, related documentation and/or other materials provided with the Software must reproduce and retain the above copyright notice, this list of conditions and the following disclaimer.

2)	Recipient shall have the right to create modifications of the Software (“Modifications”) for their internal research and academic purposes only. 

3)	All copies of Modifications in binary form and/or source code and related documentation must reproduce and retain the above copyright notice, this list of conditions and the following disclaimer.

4)	Recipient and Institution shall not distribute Software or Modifications to any third parties without the prior written approval of Penn.

5)	Recipient will provide the Developer with feedback on the use of the Software and Modifications, if any, in their research.  The Developers and Penn are permitted to use any information Recipient provides in making changes to the Software. All feedback, bug reports and technical questions shall be sent to: 

Heather Culbertson, hculb@seas.upenn.edu, +1 215-573-6748
Katherine J. Kuchenbecker, kuchenbe@seas.upenn.edu, +1 215-573-2786

6)	Recipient acknowledges that the Developers, Penn and its licensees may develop modifications to Software that may be substantially similar to Recipient’s modifications of Software, and that the Developers, Penn and its licensees shall not be constrained in any way by Recipient in Penn’s or its licensees’ use or management of such modifications. Recipient acknowledges the right of the Developers and Penn to prepare and publish modifications to Software that may be substantially similar or functionally equivalent to your modifications and improvements, and if Recipient or Institution obtains patent protection for any modification or improvement to Software, Recipient and Institution agree not to allege or enjoin infringement of their patent by the Developers, Penn or any of Penn’s licensees obtaining modifications or improvements to Software from the Penn or the Developers.

7)	Recipient and Developer will acknowledge in their respective publications the contributions made to each other’s research involving or based on the Software. The current citations for Software are:

Heather Culbertson, Juan Jose Lopez Delgado, and Katherine J. Kuchenbecker. One Hundred Data-Driven Haptic Texture Models and Open-Source Methods for Rendering on 3D Objects. In Proc. IEEE Haptics Symposium, February 2014.

8)	Any party desiring a license to use the Software and/or Modifications for commercial purposes shall contact The Center for Technology Transfer at Penn at 215-898-9591.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS, CONTRIBUTORS, AND THE TRUSTEES OF THE UNIVERSITY OF PENNSYLVANIA "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER, CONTRIBUTORS OR THE TRUSTEES OF THE UNIVERSITY OF PENNSYLVANIA BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

************************************************************************************************************************************/
