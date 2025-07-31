#pragma once

#include "SpatialPan.h"

class SoundPlayer : ofThread{
private:
	ofSoundPlayer* soundPlayerPtr = nullptr;;
	vector <ofSoundPlayer> soundFiles;
	bool bPlaying = false;
	string buttonName[2] = { "START", "STOP" };
	string directoryPath;

public:
	SpatialPan spatialPan;
	ofFbo fboSpatiolPan;
	ofPixels pixelsSpatiolPan;

	int soundFilesMaxSize = 0;
	int directoryIndex = 0;
	string stringButtonName = buttonName[(int)bPlaying];
	int playTempoIndex = 0;
	const char* cIntervalList[13] = { "100", "110", "120", "130", "140", "150", "200", "300", "400", "500", "1500", "1000", "2000" };
	int playThreshold = 50;
	int range = 10;
	int rangePos = 0;
	float volume = 0.0;
	float pitch = 1.0;
	int panIndex = 0;

	//--------------------------------------------------------------
	void setupPlayer(vec2 size) {
		spatialPan.setup(size, ofColor(200, 200, 200), ofColor(50));
		fboSpatiolPan.allocate(size.x, size.y, GL_RGBA, 3);
		fboSpatiolPan.begin();
		ofClear(255);
		fboSpatiolPan.end();
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
	void draw() {
		fboSpatiolPan.begin();
		ofClear(255);
		spatialPan.draw(panIndex);
		fboSpatiolPan.end();
		fboSpatiolPan.readToPixels(pixelsSpatiolPan);
	}

	//--------------------------------------------------------------
	void start() {
		if (!bPlaying) {
			startThread();
			bPlaying = true;
			stringButtonName = buttonName[(int)bPlaying];
		}
		else if (bPlaying) {
			stop();
		}
	}

	//--------------------------------------------------------------
	void stop() {
		if (bPlaying) {
			stopThread();
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
				sleep(ofToInt(cIntervalList[playTempoIndex]));
				unlock();
			}
		}
	}

};