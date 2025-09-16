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

	int intervalIndex = 4;
	const char* cIntervalList[20] = { "60", "70", "80", "90", "100", "110", "120", "130", "140", "150", "160", "170", "180", "190", "200", "300", "400", "500", "1000", "2000"};
	int groupIndex = 0;
	int groupIndexCounter = 0;
	const char* cGroupList[5] = { "1", "2", "3", "4", "8" };
	int waitTimeIndex = 0;
	const char* cWaitTimeList[7] = { "500", "1000", "1500", "2000", "2500", "3000", "5000" };

	float threshold = 1.0;
	int range = 1;
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
	void drawFbo() {
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
			if (soundPlayerPtr != nullptr) {
				soundPlayerPtr->stop();
			}
			stringButtonName = buttonName[(int)bPlaying];
		}
	}

	//--------------------------------------------------------------
	void threadedFunction() {
		while (isThreadRunning()) {
			if (lock()) {
				if (ofRandom(1.0) <= threshold) {
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

				groupIndexCounter++;
				groupIndexCounter %= ofToInt(cGroupList[groupIndex]);
				int interval = ofToInt(cIntervalList[intervalIndex]);
				if (cGroupList[groupIndex] != "1" && groupIndexCounter == ofToInt(cGroupList[groupIndex]) - 1) {
					interval = ofToInt(cWaitTimeList[waitTimeIndex]);
				}
				sleep(interval);
				unlock();
			}
		}
	}

};