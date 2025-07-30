#pragma once

#include "SpatialPan.h"

class SoundPlayer : ofThread{
private:
	vec2 size;
	ofxMotion motionTimer;
	ofSoundPlayer* soundPlayerPtr = nullptr;;
	vector <ofSoundPlayer> soundFiles;
	bool bPlaying = false;
	string buttonName[2] = { "START", "STOP" };
	ofRectangle timerAreaBase;
	ofRectangle timerAreaInner;
	string directoryPath;

public:
	SpatialPan spatialPan;
	int soundFilesMaxSize = 0;
	int directoryIndex = 0;
	string stringButtonName = buttonName[(int)bPlaying];
	int playTempoIndex = 1;
	const char* cTempoList[8] = { "0.08", "0.1", "0.2", "0.5", "1.0", "1.5", "2.0", "5.0" };
	int playThreshold = 50;
	int range = 10;
	int rangePos = 0;
	float volume = 0.0;
	float pitch = 1.0;
	int panIndex = 0;

	//--------------------------------------------------------------
	void setupPlayer(vec2 pos, vec2 size, int marginTop) {
		this->size = size;
		timerAreaInner = ofRectangle(pos.x + 10, pos.y + 6, size.x - 14, marginTop - 10);
		timerAreaBase = ofRectangle(pos.x, pos.y, size.x, marginTop);
		motionTimer.setMotionTransformPtr(new MoveLiner());
		motionTimer.setMotionColorPtr(new DefaultColor());
		motionTimer.setup(ofxMotion::DrawMode::RECT, pos, vec2(1.0, 1.0), 4.0, 8.0, 0.0f, ofColor(200), ofxMotion::AnchorMode::ANCHOR_TOP_LEFT, 0, false);
		spatialPan.setup(vec2(pos.x, size.y), vec2(size.x, size.x), ofColor(200, 200, 200), ofColor(50));
	}

	//--------------------------------------------------------------
	void loadSoundFilesFromDirectory(string path) {
		directoryPath = path;
		startThread();
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
		ofSetColor(117, 131, 142);
		ofDrawRectangle(timerAreaBase);
		ofSetColor(50, 50, 60);
		ofDrawRectangle(timerAreaInner);
		ofPopStyle();
		motionTimer.draw();
		spatialPan.draw(panIndex);
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
				soundPlayerPtr->setPan(spatialPan.getPan(panIndex));
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
				soundFiles[i].setVolume(0.0);
				soundFiles[i].stop();
			}
			stringButtonName = buttonName[(int)bPlaying];
		}
	}

	//--------------------------------------------------------------
	void threadedFunction() {
		while (isThreadRunning()) {
			if (lock()) {
				stop();
				soundFiles.clear();
				ofDirectory directory;
				directory.listDir(directoryPath);
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
				unlock();
				stopThread();
			}
		}
	}

};