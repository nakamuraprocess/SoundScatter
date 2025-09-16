#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ofxAppHelper.h"
#include "ofxMotion.h"
#include "SoundPlayer.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyReleased(int key);

		float now;
		float deltaTime;

		ofxImGui::Gui gui;
		vec2 guiSize;
		vec2 panSize;
		int marginTop;

		static const int soundPlayerMaxSize = 3;
		SoundPlayer soundPlayer[soundPlayerMaxSize];
		GLuint pixelSoundPlayerID[soundPlayerMaxSize];
		vector <string> directoryNames;
		vector <ofFile> directorys;
};
