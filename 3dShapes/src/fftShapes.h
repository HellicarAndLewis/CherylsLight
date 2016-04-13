//
//  fftShapes.h
//  basic fft 2
//
//  Created by topshop on 06/04/2016.
//
//

#pragma once

#include "ofMain.h"
#include "ofxProcessFFT.h"
#include "ofxGui.h"

class fftShapes{
public:
    void setup();
    void setupGUI();
    void update();
    void draw();
    void drawDebug();
    void reset();
    ofParameterGroup getParameters(){ return allParams; };
    
    void buildMainShape();
    void connectIndices();
    void colorVertices(int xIndex, float yHeight, vector<float>* spectrum);
    void buildShapeOnce();
    
    void makeDisc(int xIndex, float yHeight, int zTime, vector<float>* spectrum, float fftSize);
    void makeSpring(int xIndex, float yHeight, int zTime);
    void makePlane(int xIndex, float yHeight, int zTime, vector<float>* spectrum);
    void makeCircular(int xIndex, float yHeight, int zTime);
    void makeDiscFromInside(int xIndex, float yHeight, int zTime, vector<float>* spectrum, float fftSize);
    // To be done:
    void makePlaneFromInside(int xIndex, float yHeight, int zTime, vector<float>* spectrum, float fftSize);
  
    
    void saveToFile();
    void setShowDebug(bool drawIt);
    void setPauseMesh(bool pauseIt);
    void setColorScheme(int scheme);
    
    bool getShowDebug(){return bDrawDebug;};
    bool getPauseMesh(){return pauseMesh;};

    // custom
    void clearSpectrums();
private:
    vector< vector<float> > spectrums;
    
    ofEasyCam easyCam;
    ProcessFFT  fft;
    
    ofMesh      mesh;
    
    void setShapeTypeString();
    
    ofParameterGroup allParams;
    ofParameterGroup soundParams;
    ofParameterGroup colorParams;
    ofParameterGroup shapeParams;
    ofParameterGroup shapeTypes;
    ofParameterGroup camPosParams;
    
    // fftBin -> Resolution
    ofParameter<int> fftBins, maxVol;
    
    // sketch is reset if number of fft bin is changed
    int prevFftBins;
    
    // binAmount -> fft percentage
    // exponent -> exponent curve
    
    // meshSpacingDepth -> Spacing between current set of vertices and last set of vertices
    // more meshSpacingDepth means larger area will be drawn
    ofParameter<float> binAmount, exponent, scaleMesh, spinSpeed = 1, spinRadius, spikeScaleFactor, radToSpike = 0.1, smoosh, meshSpacingDepth;
    ofParameter<int> meshSpacingWidth, maxMeshLength = 20000, thickness = 30, maxSpikeHeight, volHistoryLength, timeStep;
    
    // Camaera stuff including scaleMesh above
    ofParameter<int> meshX, meshY, meshZ;
    
    // toggles
    ofParameter<bool> bDrawDebug, autoClear = false,fftNormalize, autoSize, shapeCircular, shapeSpiral, shapeCurve, shapeSpring, shapeSpire, shapePlane, bShowBars, shapeDisc, shapeWavyDisc, bShowWireframe, pauseMesh;
    ofParameter<int> shapeTypeInt;
    // clear mesh when making a different shape
    int prevShapeTypeInt;
    
    ofParameter<string> shapeName;
    
    // colour settings
    ofParameter<ofColor> peakColor, bottomColor, quietColor, loudColor;
    ofFloatColor fPeakColor, fBottomColor, fQuietColor, fLoudColor;
    
    
    enum drawingNames {DISC, SPRING, PLANE, CIRCULAR, DISCINSIDE, PLANEINSIDE};
    
    
    int timePos;
    int loudestBand;
    float numRevolutions;
};
