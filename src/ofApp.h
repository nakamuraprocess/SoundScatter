#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ofxAppHelper.h"
#include "ofxMotion.h"
#include "SoundTrack.h"
#include "SoundPlayer.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void drawImGui();
		void keyReleased(int key);

		float now;
		float step;

		ofxImGui::Gui gui;
		vec2 center;
		SoundTrack soundTrack;
		static const int soundPlayerMaxSize = 3;
		SoundPlayer soundPlayer[soundPlayerMaxSize];
};
