//
//  fftShapes.cpp
//  basic fft 2
//
//  Created by topshop on 06/04/2016.
//
//

#include "fftShapes.h"


void fftShapes::setup(){
    fft.setup();
    timePos = 0;
    setupGUI();
}

void fftShapes::update(){
    setShapeTypeString();
    
    if(prevFftBins!=fft.getNumFFTbins()){
        reset();
    }
    prevFftBins = fft.getNumFFTbins();
    
    fft.update();
    
    fft.setFFTpercentage(binAmount);
    fft.setNumFFTBins(fftBins);
    fft.setHistorySize(volHistoryLength);
    fft.setNormalize(fftNormalize);
    fft.setVolumeRange(maxVol);
    
    fPeakColor = ofFloatColor(peakColor->r/255, peakColor->g/255, peakColor->b/255);
    fBottomColor = ofFloatColor(bottomColor->r/255, bottomColor->g/255, bottomColor->b/255);
    fQuietColor = ofFloatColor(quietColor->r/255, quietColor->g/255, quietColor->b/255);
    fLoudColor = ofFloatColor(loudColor->r/255, loudColor->g/255, loudColor->b/255);
    
    
    buildMainShape();
}

void fftShapes::draw(){
    glEnable(GL_DEPTH_TEST);
    easyCam.begin();
    ofPushMatrix();
    if(shapeCircular) ofTranslate(meshX, meshY,meshZ);
    if(shapeTypeInt == PLANE)ofTranslate(meshX, meshY,meshZ-(timePos*1.15)); //automover - doesnt work well at the moment
    ofScale(scaleMesh, scaleMesh, scaleMesh);
    ofSetColor(255);
    bShowWireframe ? mesh.drawWireframe() : mesh.draw();
    
    ofPopMatrix();
    easyCam.end();
    glDisable(GL_DEPTH_TEST);
    
    if (bDrawDebug) drawDebug();
}

void fftShapes::buildShapeOnce(){
    cout<<"Building shape..."<<endl;
    ofVec3f prevPt;
    ofVec3f currentPt;
    
    if(shapeTypeInt == DISC){
        timeStep = 0.5*meshSpacingDepth;
    }else if(shapeTypeInt == DISCINSIDE){
        timeStep = 4*meshSpacingDepth;
    }
    
    if(!pauseMesh){
        ofFile songtxt;
        ofBuffer songBuffer;
        
        // Reading the data saved in song.txt into spectrums vector to be used next
        songtxt.open(ofToDataPath("song.txt"), ofFile::ReadOnly, false);
        songBuffer = songtxt.readToBuffer();
        string inputText = songBuffer.getText();
        vector<string> str = ofSplitString(inputText, "\n");
        vector< vector<string> > strstr;
        spectrums.clear();
        for (int i=0; i<str.size(); i++){
            strstr.push_back(ofSplitString(str[i], " "));
        }
        spectrums.resize(strstr.size());
        for (int i=0; i<strstr.size(); i++){
            for (int j=0; j<strstr[i].size(); j++){
                spectrums[i].push_back(ofToFloat(strstr[i][j]));
            }
        }
        
        
        // Iterate spectrums vector to draw the shape
        for (int i=0; i<spectrums.size(); i++) {
            if(i%timeStep==0){
            
                // Saving the vector into a file - Uncomment to reseed song shape
    //            for (int j=0; j<spectrums[i].size(); j++){
    //                songBuffer.append(to_string(spectrums[i][j]));
    //                if(j != spectrums[i].size()-1){
    //                    songBuffer.append(" ");
    //                }
    //            }
    //            songBuffer.append("\n");
                
                
                if(fft.getNormalized()){
                    maxVol = 1.0; //just 0-1 when normalized
                }
                
                for (int z = (timePos-1)*meshSpacingDepth; z<timePos*meshSpacingDepth; z = z+meshSpacingDepth) {
                    for (int x=0; x<fft.getNumFFTbins()*meshSpacingWidth; x = x+meshSpacingWidth) {
                        // x is the fft bin or the frequency band we are looking at
                        // y is the spike height
                        // z is the current bit of sound the fft is analyzing
                        float y;
                        if(fft.getNormalized()){
                            y = spectrums[i][x/meshSpacingWidth]*maxSpikeHeight;
                        }else{
                            y = ofMap(spectrums[i][x/meshSpacingWidth], 0, maxVol, 0, 1)*maxSpikeHeight;
                        }
                        
                        
                        float angle = ofMap(z, 0, maxMeshLength, 0, TWO_PI); //sets starting point of angle //phi
                        
                        float alpha = ofMap(z, 0, maxMeshLength, 1, 1); //sets alpha value so it fades out, not currently used
                        
                        numRevolutions = angle/TWO_PI;
                        
                        
                        //OUTER RADIUS
                        float wigglyRadius = 2*spinRadius + y*(spikeScaleFactor)*(spinRadius*radToSpike); //diameter + offset from audio
                        float tempZ = (float)wigglyRadius * cos(spinSpeed*angle); //outer radius
                        float tempY = (float)wigglyRadius * sin(spinSpeed*angle);
                        
                        //INNER RADIUS
                        float wigglyRadiusSmall = 2*spinRadius-thickness; //diameter with offset for thickness
                        float tempZsmall = (float)wigglyRadiusSmall * cos(spinSpeed*angle); //inner radius
                        float tempYsmall = (float)wigglyRadiusSmall * sin(spinSpeed*angle);
                        
                        //DRAW SHAPES
                        
                        if (shapeTypeInt==DISC) {
                            makeDisc(x,y,z, &spectrums[i], spectrums.size());
                        }else if(shapeTypeInt==SPRING){
                            makeSpring(x,y,z);
                        }else if(shapeTypeInt==PLANE){
                            makePlane(x,y,z, &spectrums[i]);
                        }else if(shapeTypeInt==CIRCULAR){
                            makeCircular(x,y,z);
                        }else if(shapeTypeInt==DISCINSIDE){
                            makeDiscFromInside(x,y,z, &spectrums[i], spectrums.size());
                        }else if(shapeTypeInt==PLANEINSIDE){
                            makePlaneFromInside(x,y,z, &spectrums[i], spectrums.size());
                        }
                    }
                }
                
                // Connecting the points and incrementing counter
                connectIndices();
                timePos++; //time step
                timePos=timePos%maxMeshLength; //reset after it maxes out on length
                if (timePos==0) {
                    mesh.clear();
                    numRevolutions = 0;
                }
            }
        }
//        bool fileWritten = ofBufferToFile("song.txt", songBuffer);
//        cout<<spectrums.size()<<endl;
    }
}

void fftShapes::buildMainShape(){
    ofVec3f prevPt;
    ofVec3f currentPt;
//    cout<<fft.getSpectrum().size()<<endl;
    if(!pauseMesh){
        if(ofGetFrameNum()%timeStep==0){
            spectrums.push_back(fft.getSpectrum());
        }
    }
    
}

void fftShapes::colorVertices(int xIndex, float yHeight, vector<float>* spectrum){
    //Color via intensity
    if (xIndex/meshSpacingWidth == fft.getLoudBand()) {
        mesh.addColor(fPeakColor);
    }
    else{
        float meshHeight =(*spectrum)[xIndex/meshSpacingWidth];
        mesh.addColor(
              ofColor(
                  ofMap(meshHeight,0,maxVol,quietColor->r,loudColor->r),
                  ofMap(meshHeight,0,maxVol,quietColor->g,loudColor->g),
                  ofMap(meshHeight,0,maxVol,quietColor->b,loudColor->b))
              );
    }
}

void fftShapes::connectIndices(){
    int indexHeight = timePos;
    indexHeight = ofClamp(indexHeight, 0, maxMeshLength);
    int indexWidth = fft.getNumFFTbins();
    indexWidth = ofClamp(indexWidth, 0, fft.getNumFFTbins())+4;
    
    //ADD ALL THEM INDICES, SON!
    if(timePos>0){
        for (int y = timePos-1; y<timePos; y++){ //time
            for (int x=0; x<indexWidth-1; x++){ //eqCols
                
                if(x==0 && y!=0){ //left side and right side connections
                    mesh.addIndex(x+y*indexWidth);				// 0
                    mesh.addIndex((x+1)+y*indexWidth);			// 1
                    mesh.addIndex(x+(y+1)*indexWidth);			// 10
                    
                    mesh.addIndex(x+(y+1)*indexWidth);			// 10
                    mesh.addIndex((x+1)+y*indexWidth);			// 1
                    mesh.addIndex((x+1)+(y+1)*indexWidth);		// 11
                    
                    mesh.addIndex(x+y*indexWidth);				// 0
                    mesh.addIndex((x+fft.getNumFFTbins()+3)+y*indexWidth);	// 1
                    mesh.addIndex(x+(y+1)*indexWidth);			// 10
                    
                    mesh.addIndex((x+fft.getNumFFTbins()+3)+y*indexWidth);	// 1
                    mesh.addIndex((x+fft.getNumFFTbins()+3)+(y+1)*indexWidth);	// 11
                    mesh.addIndex(x+(y+1)*indexWidth);			// 10
                }
                
                
                mesh.addIndex(x+y*indexWidth);				// 0
                mesh.addIndex((x+1)+y*indexWidth);			// 1
                mesh.addIndex(x+(y+1)*indexWidth);			// 10
                
                mesh.addIndex((x+1)+y*indexWidth);			// 1
                mesh.addIndex((x+1)+(y+1)*indexWidth);		// 11
                mesh.addIndex(x+(y+1)*indexWidth);			// 10
            }
        }
    }
}

void fftShapes::reset(){
    timePos = 0;
    mesh.clear();
    numRevolutions = 0;//reset
}


// Using /-----------------------------------------------------------
void fftShapes::makeDisc(int xIndex, float yHeight, int zTime, vector<float>* spectrum, float fftSize){
    //DISCUS DISCUSS
    
    float angle = ofMap(zTime, 0, fftSize, 0,TWO_PI); //sets starting point of angle //phi
    //LEFT SIDE
    
    //OUTER RADIUS Edit Inner Radius here
    float tempRadius = spinSpeed*((fft.getNumFFTbins())*meshSpacingWidth/2)/16;
    
    float ySpike = yHeight*(spikeScaleFactor)*(spinRadius*radToSpike);
    float wigglyRadius = 2*tempRadius + xIndex; //diameter + offset from audio
    float tempZ = (float)wigglyRadius * cos(spinSpeed*angle); //outer radius
    float tempX = (float)wigglyRadius * sin(spinSpeed*angle);
    
    
    float extraOffset = 0;
    float yOffset = 0;
    
    
    if(xIndex==0 && yHeight!= 0){
        mesh.addVertex(ofVec3f(tempX,-thickness+yOffset,tempZ)); //bottom vert
        mesh.addColor(ofColor(200, 200, 200));
        mesh.addVertex(ofVec3f(tempX,ySpike+yOffset,tempZ)); //top vert
        mesh.addColor(ofColor(100, 100, 100));
    }
    
    //MAIN PART
    mesh.addVertex(ofVec3f(tempX,ySpike,tempZ));
    
    
    colorVertices(xIndex, yHeight, spectrum);
    
    //RIGHT SIDE
    if(xIndex==(fft.getNumFFTbins()*meshSpacingWidth)-meshSpacingWidth && yHeight!=0){
        mesh.addVertex(ofVec3f(tempX,ySpike+yOffset,tempZ)); //top right piece
        mesh.addColor(ofColor(100, 100, 100));
        mesh.addVertex(ofVec3f(tempX, -thickness+yOffset,tempZ)); //bottom right piece
        mesh.addColor(ofColor(100, 100, 100));
    }
}

void fftShapes::makeDiscFromInside(int xIndex, float yHeight, int zTime, vector<float>* spectrum, float fftSize){
    // Start from inside
    // draw one set of vertices only in one execution
    
    // xIndex is the fft bin or the frequency band we are looking at
    // yHeight is the spike height
    // zTime is the current bit of sound the fft is analyzing
    //
    // so zTime should map to the radius
    // and xIndex should map to the radius
    // a fraction of xIndex/(fft.getNumFFTbins()*meshSpacingWidth) could be useful to plot out the angle
    // maybe xIndex/(fft.getNumFFTbins()*meshSpacingWidth) * TWO_PI to get the angle in radians
    float maximumX = fft.getNumFFTbins() * meshSpacingWidth;
    float unmappedAngle = xIndex / maximumX;
    
    float angle = ofMap(unmappedAngle, 0, 1, 0, TWO_PI);
    float radius = zTime;
    
    float ySpike = yHeight*(spikeScaleFactor)*(spinRadius*radToSpike);
    
    float tempZ = (float)radius * cos(angle); //outer radius
    float tempX = (float)radius * sin(angle);
    
    float yOffset = 0;
    
    // Beginning the shape
    if(xIndex==0 && yHeight!= 0){
        mesh.addVertex(ofVec3f(tempX,-thickness+yOffset,tempZ)); //bottom vert
        mesh.addColor(ofColor(200, 200, 200));
        mesh.addVertex(ofVec3f(tempX,ySpike+yOffset,tempZ)); //top vert
        mesh.addColor(ofColor(100, 100, 100));
    }
    
    //MAIN PART
    mesh.addVertex(ofVec3f(tempX,ySpike,tempZ));
    
    
    colorVertices(xIndex, yHeight, spectrum);
    
    // Ending the shape
    if(xIndex==(fft.getNumFFTbins()*meshSpacingWidth)-meshSpacingWidth && yHeight!=0){
        angle = TWO_PI;
        tempZ = (float)radius * cos(angle);
        tempX = (float)radius * sin(angle);
        mesh.addVertex(ofVec3f(tempX,ySpike,tempZ));
        mesh.addColor(ofColor(100, 100, 100));
    }
}


void fftShapes::makePlaneFromInside(int xIndex, float yHeight, int zTime, vector<float>* spectrum, float fftSize){
    // Start from inside
    // draw one set of vertices only in one execution
    
    // xIndex is the fft bin or the frequency band we are looking at
    // yHeight is the spike height
    // zTime is the current bit of sound the fft is analyzing
    //
    // so zTime should map to the radius
    // and xIndex should map to half the length of the square
    // The math is radius=zTime/(2*cos angle)
    
    // a fraction of xIndex/(fft.getNumFFTbins()*meshSpacingWidth) could be useful to plot out the angle
    // maybe xIndex/(fft.getNumFFTbins()*meshSpacingWidth) * TWO_PI to get the angle in radians
    float t = fft.getNumFFTbins() * meshSpacingWidth;
    float unmappedAngle = xIndex / t;
    
    float angle = ofMap(unmappedAngle, 0, 1, 0, TWO_PI);
    float radius = zTime/2;
    
    if( angle>0 && angle<=PI/4 ){
        radius = zTime/(2*cos(angle));
    }else if( angle>PI/4 && angle<2*PI/4){
        radius = zTime/(2*sin(angle));
    }else if( angle>2*PI/4 && angle<=3*PI/4 ){
        radius = zTime/(2*sin(angle));
    }else if( angle>3*PI/4 && angle<=PI){
        radius = abs(zTime/(2*cos(angle)));
    }else if( angle>PI && angle<=5*PI/4){
        radius = abs(zTime/(2*cos(angle)));
    }else if( angle>5*PI/4 && angle<=6*PI/4){
        radius = abs(zTime/(2*sin(angle)));
    }else if( angle>6*PI/4 && angle<=7*PI/4){
        radius = abs(zTime/(2*sin(angle)));
    }else if( angle>7*PI/4 && angle<=8*PI/4){
        radius = abs(zTime/(2*cos(angle)));
    }
    
    float ySpike = yHeight*(spikeScaleFactor)*(spinRadius*radToSpike);
    
    if(ySpike > maxSpikeHeight*3){
        ySpike = 2*ySpike/4;
    }
    
    float tempZ = (float)radius * cos(angle); //outer radius
    float tempX = (float)radius * sin(angle);
    
    float yOffset = 0;
    
    
    if(xIndex==0 && yHeight!= 0){
        mesh.addVertex(ofVec3f(tempX,-thickness+yOffset,tempZ)); //bottom vert
        mesh.addColor(ofColor(200, 200, 200));
        mesh.addVertex(ofVec3f(tempX,ySpike+yOffset,tempZ)); //top vert
        mesh.addColor(ofColor(100, 100, 100));
    }
    
    //MAIN PART
    mesh.addVertex(ofVec3f(tempX,ySpike,tempZ));
    
    
    colorVertices(xIndex, yHeight, spectrum);
    
    // Close up the shape
    if(xIndex==(fft.getNumFFTbins()*meshSpacingWidth)-meshSpacingWidth && yHeight!=0){
        angle = TWO_PI;
        tempZ = (float)radius * cos(angle);
        tempX = (float)radius * sin(angle);
        mesh.addVertex(ofVec3f(tempX,ySpike,tempZ));
        mesh.addColor(ofColor(100, 100, 100));
    }
}


void fftShapes::makePlane(int xIndex, float yHeight, int zTime, vector<float>* spectrum){
    float y = (*spectrum)[xIndex/meshSpacingWidth]*maxSpikeHeight*spikeScaleFactor*100;
    
    //LEFT SIDE
    if(xIndex==0 && y!= 0){
        mesh.addVertex(ofVec3f(xIndex-meshSpacingWidth,0-thickness,zTime)); //bottom vert
        mesh.addColor(ofColor(255, 255, 255));
        mesh.addVertex(ofVec3f(xIndex-meshSpacingWidth,0,zTime)); //top vert
        mesh.addColor(ofColor(255, 255, 255));
    }
    
    mesh.addVertex(ofVec3f(xIndex,y,zTime));
    //Color via intensity
    colorVertices(xIndex, y, spectrum);
    
    //RIGHT SIDE
    if(xIndex==(fft.getNumFFTbins()*meshSpacingWidth)-meshSpacingWidth && y!=0){
        mesh.addVertex(ofVec3f(xIndex+meshSpacingWidth,0,zTime)); //top right piece
        mesh.addColor(ofColor(255, 255, 255));
        mesh.addVertex(ofVec3f(xIndex+meshSpacingWidth, 0-thickness,zTime)); //bottom right piece
        mesh.addColor(ofColor(255, 255, 255));
    }
}
// Using end /-------------------------------------------------------

void fftShapes::makeSpring(int xIndex, float yHeight, int zTime){
    
    //    float angle = ofMap(zTime, 0,maxMeshLength, 0,TWO_PI); //sets starting point of angle //phi
    //    float wigglyRadius = 2*spinRadius + yHeight*(spikeScaleFactor)*(spinRadius*radToSpike); //diameter + offset from audio
    //    float wigglyRadiusSmall = 2*spinRadius-thickness; //diameter with offset for thickness
    //    float tempY = (float)wigglyRadius * sin(spinSpeed*angle);
    //    float tempZ = (float)wigglyRadius * cos(spinSpeed*angle); //outer radius
    //    float tempYsmall = (float)wigglyRadiusSmall * sin(spinSpeed*angle);
    //    float tempZsmall = (float)wigglyRadiusSmall * cos(spinSpeed*angle); //inner radius
    //
    //    float extraOffset = 1;
    //    //LEFT SIDE
    //    if(xIndex==0 && yHeight!= 0){ //left side for the rest
    //        mesh.addVertex(ofVec3f(xIndex+(numRevolutions*((fft.getNumFFTbins()+extraOffset)*meshSpacingWidth+extraOffset))-meshSpacingWidth,tempYsmall,tempZsmall)); //bottom vert
    //        mesh.addColor(ofColor(255, 255, 255));
    //        mesh.addVertex(ofVec3f(xIndex+(numRevolutions*((fft.getNumFFTbins()+extraOffset)*meshSpacingWidth+extraOffset))-meshSpacingWidth, tempY,tempZ)); //top vert
    //        mesh.addColor(ofColor(255, 255, 255));
    //    }
    //
    //    //MAIN PART
    //    mesh.addVertex(ofVec3f(xIndex+(numRevolutions*((fft.getNumFFTbins()+extraOffset)*meshSpacingWidth+extraOffset)),tempY,tempZ));
    //
    //    colorVertices(xIndex, yHeight);
    //
    //    //RIGHT SIDE
    //    if(xIndex==(fft.getNumFFTbins()*meshSpacingWidth)-meshSpacingWidth && yHeight!=0){ //right side for the rest
    //        mesh.addVertex(ofVec3f(xIndex+(numRevolutions*(fft.getNumFFTbins()+extraOffset)*meshSpacingWidth)+meshSpacingWidth,tempY,tempZ)); //top right piece
    //        mesh.addColor(ofColor(255, 255, 255));
    //        mesh.addVertex(ofVec3f(xIndex+(numRevolutions*(fft.getNumFFTbins()+extraOffset)*meshSpacingWidth)+meshSpacingWidth, tempYsmall,tempZsmall)); //bottom right piece
    //        mesh.addColor(ofColor(255, 255, 255));
    //    }
}

void fftShapes::makeCircular(int xIndex, float yHeight, int zTime){
    
//    float angle = ofMap(zTime, 0,maxMeshLength, 0,TWO_PI); //sets starting point of angle //phi
//    float wigglyRadius = 2*spinRadius + yHeight*(spikeScaleFactor)*(spinRadius*radToSpike); //diameter + offset from audio
//    float tempZ = (float)wigglyRadius * cos(spinSpeed*angle); //outer radius
//    float tempY = (float)wigglyRadius * sin(spinSpeed*angle);
//    
//    //INNER RADIUS
//    float wigglyRadiusSmall = 2*spinRadius-thickness; //diameter with offset for thickness
//    float tempZsmall = (float)wigglyRadiusSmall * cos(spinSpeed*angle); //inner radius
//    float tempYsmall = (float)wigglyRadiusSmall * sin(spinSpeed*angle);
//    
//    
//    //LEFT SIDE
//    if(xIndex==0 && yHeight!= 0){
//        mesh.addVertex(ofVec3f(xIndex-meshSpacingWidth,tempYsmall,tempZsmall)); //bottom vert
//        mesh.addColor(ofColor(255, 255, 255));
//        mesh.addVertex(ofVec3f(xIndex-meshSpacingWidth, tempY,tempZ)); //top vert
//        mesh.addColor(ofColor(255, 255, 255));
//    }
//    
//    //MAIN PART
//    mesh.addVertex(ofVec3f(xIndex, tempY,tempZ)); //draw as a plane
//    
//    //Color adding via intensity
//    colorVertices(xIndex, yHeight);
//    
//    //RIGHT SIDE
//    if(xIndex==(fft.getNumFFTbins()*meshSpacingWidth)-meshSpacingWidth && yHeight!=0){ //right side for the rest
//        mesh.addVertex(ofVec3f(xIndex+meshSpacingWidth,tempY,tempZ)); //top right piece
//        mesh.addColor(ofColor(255, 255, 255));
//        mesh.addVertex(ofVec3f(xIndex+meshSpacingWidth, tempYsmall,tempZsmall)); //bottom right piece
//        mesh.addColor(ofColor(255, 255, 255));
//    }
}

void fftShapes::drawDebug(){
    
    
    ofPushMatrix();
    ofTranslate(100, 0);
    fft.drawDebug();
    ofDrawBitmapStringHighlight("Num Mesh Vertices: " + ofToString(mesh.getNumVertices()), 250, 240);
    
    ofPopMatrix();
    if(bShowBars){
        fft.drawBars();
    }
    ofNoFill();
    ofRectangle(1080, 0, 200, 200);
    ofRectangle(1080, 200, 200, 200);
    ofRectangle(1080, 400, 200, 200);
    ofRectangle(1080, 600, 200, 200);
    
    fft.drawHistoryGraph(ofPoint(1080,0), LOW);
    fft.drawHistoryGraph(ofPoint(1080,200),MID );
    fft.drawHistoryGraph(ofPoint(1080,400),HIGH );
    fft.drawHistoryGraph(ofPoint(1080,600),MAXSOUND );
    ofDrawBitmapString("LOW",1100,20);
    ofDrawBitmapString("HIGH",1100,420);
    ofDrawBitmapString("MID",1100,220);
    ofDrawBitmapString("MAX VOLUME",1100,620);
    
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofFill();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255,127);
    ofRectangle(0,ofGetHeight()-80, ofMap(timePos, 0, maxMeshLength, 0,ofGetWidth()), 30); //draw progress bar
    ofSetColor(80);
    int meshLengthHolder = maxMeshLength;
    ofDrawBitmapString("Time Pos: " + ofToString(timePos) + "/" + ofToString(meshLengthHolder) + "  Time Elapsed: " + ofToString(((float)timePos/1800)*60,2) + "sec", 20, ofGetHeight()-60);
    
    ofDrawBitmapString("Press 's' to save, 'r' to reset\nClick/drag to move, right click and drag to zoom, \nDouble click to reset view", 400, ofGetHeight()-80);
    
    
    
}

void fftShapes::setupGUI(){
    
    soundParams.setName("Sound Params");
    soundParams.add(fftNormalize.set("Normalize over history", true));
    soundParams.add(maxVol.set("Max Volume", 80,1,500));
    soundParams.add(fftBins.set("Num FFT Bins", 32, 8, 96));
    soundParams.add(binAmount.set("Percentage of FFT", 0.14, 0.01, 0.5));
    soundParams.add(exponent.set("Exponent Curve", 1.6, 1, 3.0 ));
    soundParams.add(volHistoryLength.set("Norm. Vol. History Length", 600, 5, 800));
    soundParams.add(timeStep.set("Sample frame skip", 2, 1, 30)); //how often should we sample the sound input? Every frame, or less often? App runs at 60fps, so we just sample every other frame
    
    shapeParams.setName("Shape Controls");
    shapeParams.add(meshSpacingDepth.set("Space btwn time pts",10, 1, 50)); //this is how far apart the mesh points are when spaced by time
    shapeParams.add(meshSpacingWidth.set("Mesh Spacing Width", 4, 1, 300)); //this is how far apart the mesh points are in width for each time step, like if there are 16 FFT bands 10 units apart, it would be 160 units wide
    shapeParams.add(spikeScaleFactor.set("Spike Scale Factor", 1,-3,3));
    shapeParams.add(spinRadius.set("Spin Radius", 700,0,5000)); //radius of the circle that the shape spins around(depends on drawing mode)     //spin radius would be (circumference/pi)/2
    shapeParams.add(spinSpeed.set("Spin Speed", 1, -10, 10)); //1 to revolve normally, higher or lower to love slower or faster around a circle (depends on drawing type)
    shapeParams.add(maxSpikeHeight.set("Max Spike Size", 20,1,100)); //another spike scaling factor...
    
    
    shapeTypes.setName("Select a shape");
    shapeTypes.add(shapeTypeInt.set("Select Type", 0,0,7)); //this really needs to be a dropdown menu...
    shapeTypes.add(shapeName.set("Type", ""));
    
    
    camPosParams.setName("Cam Position Controls");
    camPosParams.add(scaleMesh.set("Scale Mesh", 0.4, 0.01, 0.4));
    camPosParams.add(meshX.set("Mesh X", 0, -ofGetWidth(), ofGetWidth()));
    camPosParams.add(meshY.set("Mesh Y", 0, -ofGetHeight(), ofGetHeight()));
    camPosParams.add(meshZ.set("Mesh Z", 0, -ofGetWidth(), 4000));
    
    colorParams.setName("Color Controls");
    colorParams.add(peakColor.set("Peak Color", ofColor::red,ofColor(0,0), ofColor(255)));
    colorParams.add(quietColor.set("Quiet Color", ofColor::black,ofColor(0,0), ofColor(255)));
    colorParams.add(loudColor.set("Loud Color", ofColor::white,ofColor(0,0), ofColor(255)));
    
    colorParams.add(bottomColor.set("Bottom Color", ofColor::whiteSmoke,ofColor(0,0), ofColor(255)));
    
    allParams.add(bShowBars.set("Draw Bars", true));
    allParams.add(bDrawDebug.set("Show Debug", true));
    allParams.add(bShowWireframe.set("Show Wireframe", false));
    allParams.add(pauseMesh.set("Pause Mesh", false));
    
    allParams.add(shapeTypes);
    allParams.add(soundParams);
    allParams.add(shapeParams);
    allParams.add(camPosParams);
    allParams.add(colorParams);
    
}

void fftShapes::saveToFile(){
    string songTitle = ofSystemTextBoxDialog("Please Enter the name of the song");
    if (songTitle=="") {
        mesh.save("mesh_stuff_"+ofToString(ofGetSystemTime())+".ply");
    }else{
        mesh.save(songTitle+ " "+ofToString(ofGetSystemTime())+".ply");
    }
}

void fftShapes::setShowDebug(bool drawIt){
    bDrawDebug = drawIt;
}

void fftShapes::setPauseMesh(bool pauseIt){
    pauseMesh = pauseIt;
}


void fftShapes::setShapeTypeString(){
    
    if (shapeTypeInt!=prevShapeTypeInt) {
        reset(); //clear mesh when making a different shape..
    }
    
    prevShapeTypeInt = shapeTypeInt;
    
    if (shapeTypeInt==DISC) {
        shapeName = "Disc";
    }else if(shapeTypeInt==SPRING){
        shapeName="Spring";
    }else if(shapeTypeInt==PLANE){
        shapeName="Plane";
    }else if(shapeTypeInt==CIRCULAR){
        shapeName="Circular";
    }else if(shapeTypeInt==DISCINSIDE){
        shapeName = "Disc from Inside";
    }else if(shapeTypeInt==PLANEINSIDE){
        shapeName = "Plane from Inside";
    }
}

void fftShapes::setColorScheme(int scheme){
    
    //Todo: have these make some kind of aesthetic sense...
    if (scheme==0) {
        peakColor = ofColor::red;
        quietColor = ofColor::black;
        loudColor = ofColor::white;
    }else if(scheme==1){
        peakColor = ofColor::yellow;
        quietColor = ofColor::black;
        loudColor = ofColor::green;
    }else if(scheme==2){
        peakColor = ofColor(255,148,112);
        quietColor = ofColor(0,61,245);
        loudColor = ofColor(245,184,0);
    }else if(scheme==3){
        peakColor = ofColor(255,165,0);
        quietColor = ofColor(116,9,178);
        loudColor = ofColor(25,255,132);
    }else if(scheme==4){
        peakColor = ofColor::orange;
        quietColor = ofColor(0,0,0);
        loudColor = ofColor::orangeRed;
    }else if(scheme==5){
        peakColor = ofColor(200,150,70);
        quietColor = ofColor(200,150,70);
        loudColor = ofColor::yellow;
    }
}

void fftShapes::clearSpectrums(){
    spectrums.clear();
}