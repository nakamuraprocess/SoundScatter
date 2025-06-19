#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	gui.setup();
	ofColor colorBackground = ofColor(200, 200, 200);
	ofBackground(colorBackground);
	step = 1.0f / 60;

	center = vec2(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
	soundTrack.setup(center, vec2(ofGetWidth(), 200), colorBackground, ofColor(50, 50, 50));

	ofColor color[3];
	color[0] = ofColor(0, 170, 0);
	color[1] = ofColor(0, 0, 255, 100);
	color[2] = ofColor(120, 60, 60);
	
	ofDirectory directory;
	directory.listDir("sounds");
	directory.sort();
	for (int i = 0; i < (int)directory.size(); i++) {
		cout << directory.getName(i) << endl;
		directoryNames.push_back(directory.getName(i));
		directorys.push_back(directory.getFile(i));
	}

	for (int i = 0; i < soundPlayerMaxSize; i++) {
		soundPlayer[i].setupPlayer(soundTrack.posInit, soundTrack.radius, soundTrack.radians, soundTrack.maxSize, soundTrack.panMap, color[i]);
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	now += step;
	for (int i = 0; i < soundPlayerMaxSize; i++) {
		soundPlayer[i].update(now);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	soundTrack.draw();
	for (int i = 0; i < soundPlayerMaxSize; i++) {
		soundPlayer[i].draw();
	}
	drawImGui();
}

//--------------------------------------------------------------
void ofApp::drawImGui() {
	gui.begin();
	{
		for (int i = 0; i < soundPlayerMaxSize; i++) {
			ImGui::GetStyle().WindowRounding = 0;
			ImGui::SetNextWindowPos(ImVec2(0 + (i * 300), 0));
			ImGui::SetNextWindowSize(ImVec2(300, 200));
			string name = ofToString(i);
			bool panelOpen = true;
			ImGui::Begin(name.c_str(), &panelOpen, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
			if (ImGui::Button(soundPlayer[i].stringButtonName.c_str(), vec2(300, 24))) {
				soundPlayer[i].start();
			}
			ImGui::Combo("Tempo", &soundPlayer[i].playTempoIndex, soundPlayer[i].cTempoList, IM_ARRAYSIZE(soundPlayer[i].cTempoList));
			ImGui::Separator();
			ImGui::SliderInt("Threshold", &soundPlayer[i].playThreshold, 0, 100);
			ImGui::Separator();
			ImGui::SliderInt("Range", &soundPlayer[i].range, 0, soundPlayer[i].soundFilesMaxSize);
			ImGui::Separator();
			ImGui::SliderInt("Position", &soundPlayer[i].rangePos, 0, soundPlayer[i].soundFilesMaxSize);
			ImGui::Separator();
			ImGui::SliderFloat("Pitch", &soundPlayer[i].pitch, 0.5, 2.5);
			ImGui::Separator();
			ImGui::SliderFloat("Volume", &soundPlayer[i].volume, 0.0, 2.8);
			ImGui::Separator();
			int directoryIndex = 0;
			if (ofxImGui::VectorCombo("Directory", &directoryIndex, directoryNames)){
				soundPlayer[i].loadSoundFilesFromDirectory(directorys[directoryIndex].getAbsolutePath());
			}
		}
	}
	gui.end();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == ' ') {
		for (int i = 0; i < soundPlayerMaxSize; i++) {
			soundPlayer[i].start();
		}
	}
}

