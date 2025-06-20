#pragma once

#include "SpatialPan.h"

class SoundPlayer {
private:
	vec2 size;
	SpatialPan SpatialPan;
	ofxMotion motionTimer;
	ofSoundPlayer* soundPlayerPtr = nullptr;;
	vector <ofSoundPlayer> soundFiles;
	bool bPlaying = false;
	string buttonName[2] = { "START", "STOP" };
	ofRectangle timerAreaBase;
	ofRectangle timerAreaInner;


public:
	int soundFilesMaxSize = 0;
	int directoryIndex = 0;
	string stringButtonName = buttonName[(int)bPlaying];
	int playTempoIndex = 9;
	const char* cTempoList[18] = { "0.1", "0.2", "0.3", "0.4", "0.5", "0.6", "0.7", "0.8", "0.9", "1", "2", "3", "4", "5", "10", "20", "30", "60" };
	int playThreshold = 100;
	int range = 0;
	int rangePos = 0;
	float volume = 1.0;
	float pitch = 1.0;


	//--------------------------------------------------------------
	void setupPlayer(vec2 pos, vec2 size, int marginTop) {
		this->size = size;
		motionTimer.setMotionTransformPtr(new MoveLiner());
		motionTimer.setMotionColorPtr(new DefaultColor());
		motionTimer.setup(ofxMotion::DrawMode::RECT, pos, vec2(1.0, 1.0), 4.0, 8.0, 0.0f, ofColor(200), ofxMotion::AnchorMode::ANCHOR_TOP_LEFT, 0, false);
		SpatialPan.setup(vec2(pos.x, size.y), vec2(size.x, size.x), ofColor(200, 200, 200), ofColor(50));
		timerAreaBase = ofRectangle(pos.x, pos.y, size.x, marginTop);
		timerAreaInner = ofRectangle(pos.x + 10, pos.y + 6 , size.x - 14, marginTop - 10);
	}

	//--------------------------------------------------------------
	void loadSoundFilesFromDirectory(string path) {
		stop();
		soundFiles.clear();
		ofDirectory directory;
		directory.listDir(path);
		directory.sort();
		for (int i = 0; i < (int)directory.size(); i++) {
			ofFile file(directory.getPath(i));
			if (file.exists()) {
				string fileExtension = ofToUpper(file.getExtension());
				if (fileExtension == "MP3" || fileExtension == "WAV") {
					ofSoundPlayer tmpSoundFile;
					tmpSoundFile.load(directory.getPath(i));
					tmpSoundFile.setLoop(false);
					tmpSoundFile.setMultiPlay(true);
					soundFiles.push_back(tmpSoundFile);
				}
			}
		}
		rangePos = 0;
		soundFilesMaxSize = (int)soundFiles.size();
		cout << "Sound File Size: " << soundFilesMaxSize << endl;
	}

	//--------------------------------------------------------------
	void update(float now) {
		motionTimer.update(now);
		if (bPlaying && motionTimer.getMotionTransform()->getState() == MotionTransformBase::DONE) {
			motionTimer.getMotionTransform()->setState(MotionTransformBase::IDLING);
			startTimer();
		}
	}
	//--------------------------------------------------------------
	void draw() {
		ofPushStyle();
		ofSetColor(100, 100, 120);
		ofDrawRectangle(timerAreaBase);
		ofSetColor(50, 50, 60);
		ofDrawRectangle(timerAreaInner);
		ofPopStyle();

		motionTimer.draw();
		SpatialPan.draw();
	}

	//--------------------------------------------------------------
	void startTimer() {
		float velocity = ofToFloat(cTempoList[playTempoIndex]);
		vec2 initPos = motionTimer.getMotionTransform()->getPosInitial();
		motionTimer.getMotionTransform()->startMoveLiner(vec2(initPos.x + 10, 6), vec2(initPos.x + size.x - 10, 6), velocity, 0.0, ofxeasing::linear::easeNone);
		if (ofRandom(0, 100) <= playThreshold) {
			if (rangePos >= soundFilesMaxSize) {
				rangePos = soundFilesMaxSize - 1;
			}
			int rangeMaxCul = rangePos + range;
			if (rangeMaxCul > soundFilesMaxSize) {
				rangeMaxCul = soundFilesMaxSize;
			}
			int index = ofRandom(rangePos, rangeMaxCul);
			soundPlayerPtr = &soundFiles[index];
			if (soundPlayerPtr != nullptr) {
				int panIndex = ofRandom(0, SpatialPan.spatialPanMaxSize);
				soundPlayerPtr->setPan(SpatialPan.panMap[panIndex]);
				soundPlayerPtr->setSpeed(pitch);
				soundPlayerPtr->setVolume(volume);
				soundPlayerPtr->play();
			}
		}
	}

	//--------------------------------------------------------------
	void start() {
		if (!bPlaying) {
			bPlaying = true;
			stringButtonName = buttonName[(int)bPlaying];
			startTimer();
		}
		else if (bPlaying) {
			stop();
		}
	}

	//--------------------------------------------------------------
	void stop() {
		if (bPlaying) {
			bPlaying = false;
			for (int i = 0; i < soundFilesMaxSize; i++) {
				soundPlayerPtr = &soundFiles[i];
				if (soundPlayerPtr != nullptr) {
					soundPlayerPtr->setVolume(0.0);
					soundPlayerPtr->stop();
				}
			}
			stringButtonName = buttonName[(int)bPlaying];
		}
	}

};