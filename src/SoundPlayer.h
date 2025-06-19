#pragma once

class SoundPlayer {
private:
	ofxMotion motionPlay;
	ofxMotion motionTimer;
	ofSoundPlayer* soundPlayerPtr = nullptr;;
	vector <ofSoundPlayer> soundFiles;
	vec2 trackPosInit;
	int trackIndex;
	vec2 trackRadius;
	float* trackRadians;
	int trackMaxSize;
	float* panMap;
	bool bPlaying = false;
	string buttonName[2] = { "START", "STOP" };

public:
	int soundFilesMaxSize = 0;
	int playTempoIndex = 5;
	const char* cTempoList[14] = { "0.05", "0.1", "0.2", "0.5", "0.8", "1", "2", "3", "4", "5", "10", "20", "30", "60" };
	int playThreshold = 100;
	int range = 0;
	int rangePos = 0;
	float volume = 1.0;
	float pitch = 1.0;


	string stringButtonName = buttonName[(int)bPlaying];

	//--------------------------------------------------------------
	void setupPlayer(vec2 trackPosInit, vec2 trackRadius, float* trackRadians, int trackMaxSize, float* panMap, ofColor color) {
		this->trackPosInit = trackPosInit;
		this->trackRadius = trackRadius;
		this->trackRadians = trackRadians;
		this->trackMaxSize = trackMaxSize;
		this->panMap = panMap;
		motionPlay.setMotionTransformPtr(new MoveRadial());
		motionPlay.setMotionColorPtr(new DefaultColor());
		motionPlay.setup(ofxMotion::DrawMode::CIRCLE, this->trackPosInit, vec2(1.0, 1.0), 12, 12, 0.0f, color, ofxMotion::AnchorMode::ANCHOR_CENTER, 0, false);
		motionTimer.setMotionTransformPtr(new MoveLiner());
		motionTimer.setMotionColorPtr(new DefaultColor());
		motionTimer.setup(ofxMotion::DrawMode::RECT, vec2(0.0, 0.0), vec2(1.0, 1.0), 1.0, 1.0, 0.0f, color, ofxMotion::AnchorMode::ANCHOR_CENTER, 0, true);
		loadSoundFiles("sounds1000");
	}

	void loadSoundFiles(string path) {
		ofDirectory directory;
		directory.listDir(path);
		directory.allowExt("mp3");
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
		soundFilesMaxSize = (int)soundFiles.size();
		cout << "Sound File Size: " << soundFilesMaxSize << endl;
	}

	//--------------------------------------------------------------
	void update(float now) {
		motionPlay.update(now);
		motionTimer.update(now);

		if (rangePos >= soundFilesMaxSize) {
			rangePos = soundFilesMaxSize - 1;
		}
		int rangeMaxCul = rangePos + range;
		if (rangeMaxCul > soundFilesMaxSize) {
			rangeMaxCul = soundFilesMaxSize;
		}
		if (bPlaying && motionPlay.getMotionTransform()->getState() == MotionTransformBase::DONE) {
			motionPlay.getMotionTransform()->setState(MotionTransformBase::IDLING);
			int index = ofRandom(rangePos, rangeMaxCul);
			soundPlayerPtr = &soundFiles[index];
			if (soundPlayerPtr != nullptr) {
				soundPlayerPtr->setPan(panMap[trackIndex]);
				soundPlayerPtr->setSpeed(pitch);
				soundPlayerPtr->setVolume(volume);
				soundPlayerPtr->play();
			}
		}
		if (bPlaying && motionTimer.getMotionTransform()->getState() == MotionTransformBase::DONE) {
			motionTimer.getMotionTransform()->setState(MotionTransformBase::IDLING);
			startTimer();
		}
	}

	//--------------------------------------------------------------
	void draw() {
		if (bPlaying) {
			motionPlay.draw();
		}
	}

	//--------------------------------------------------------------
	void startTimer() {
		float velocity = ofToFloat(cTempoList[playTempoIndex]);
		motionTimer.getMotionTransform()->startMoveLiner(vec2(0, 0), vec2(100, 0), velocity, 0.0, ofxeasing::linear::easeNone);
		if (ofRandom(0, 100) <= playThreshold) {
			trackIndex = ofRandom(0, trackMaxSize);
			motionPlay.getMotionTransform()->startMoveRadial(trackRadians[trackIndex], vec2(0, 0), trackRadius, velocity, 0.0, ofxeasing::linear::easeNone);
		}
		else {
			motionPlay.getMotionTransform()->setPos(trackPosInit);
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
			bPlaying = false;
			stringButtonName = buttonName[(int)bPlaying];
		}
	}

};