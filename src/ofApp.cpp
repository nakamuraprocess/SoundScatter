#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	deltaTime = 1.0f / 60;

	gui.setup();
	guiSize = vec2(1080 / 3, 620);
	panSize = vec2(guiSize.x - 126, 116);

	ofDirectory directory;
	directory.listDir("E:\\Sounds\\Sound Effect\\__SoundScatter__");
	directory.sort();
	for (int i = 0; i < (int)directory.size(); i++) {
		ofFile directoryAsFile = directory.getFile(i);
		if (directoryAsFile.isDirectory()) {
			directoryNames.push_back(directory.getName(i));
			directorys.push_back(directoryAsFile);
		}
	}
	for (int i = 0; i < soundPlayerMaxSize; i++) {
		soundPlayer[i].setupPlayer(panSize);
	}
	ofBackground(200);
}

//--------------------------------------------------------------
void ofApp::update() {
	now += deltaTime;
}

//--------------------------------------------------------------
void ofApp::draw(){
	for (int i = 0; i < soundPlayerMaxSize; i++) {
		soundPlayer[i].draw();
		pixelSoundPlayerID[i] = gui.loadPixels(soundPlayer[i].pixelsSpatiolPan);
	}
	drawImGui();
}

//--------------------------------------------------------------
void ofApp::drawImGui() {
	gui.begin();
	for (int i = 0; i < soundPlayerMaxSize; i++) {
		ImGui::GetStyle().WindowRounding = 0;
		ImGui::SetNextWindowPos(ImVec2(guiSize.x * i, marginTop));
		ImGui::SetNextWindowSize(ImVec2(guiSize));
		string name = ofToString(i);
		bool panelOpen = true;
		ImGui::Begin(name.c_str(), &panelOpen, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		if (ImGui::Button(soundPlayer[i].stringButtonName.c_str(), vec2(guiSize.x, 24))) {
			soundPlayer[i].start();
		}
		ImGui::Separator();
		if (ofxImGui::VectorCombo("Directory", &soundPlayer[i].directoryIndex, directoryNames)) {
			soundPlayer[i].loadSoundFilesFromDirectory(directorys[soundPlayer[i].directoryIndex].getAbsolutePath());
		}
		ImGui::Separator();
		ImGui::Combo("Interval", &soundPlayer[i].playTempoIndex, soundPlayer[i].cIntervalList, IM_ARRAYSIZE(soundPlayer[i].cIntervalList));
		ImGui::Separator();
		ImGui::SliderInt("Position", &soundPlayer[i].rangePos, 0, soundPlayer[i].soundFilesMaxSize);
		ImGui::Separator();
		ImGui::SliderInt("Range", &soundPlayer[i].range, 0, 10);
		ImGui::Separator();
		ImGui::SliderInt("Threshold", &soundPlayer[i].playThreshold, 0, 100);
		ImGui::Separator();
		ImGui::SliderFloat("Pitch", &soundPlayer[i].pitch, 0.5, 2.5);
		ImGui::Separator();
		ImGui::SliderFloat("Volume", &soundPlayer[i].volume, 0.0, 2.8);
		ImGui::Separator();
		ImGui::Combo("Pan", &soundPlayer[i].panIndex, soundPlayer[i].spatialPan.cPanList, IM_ARRAYSIZE(soundPlayer[i].spatialPan.cPanList));
		ImGui::Separator();
		ImGui::Image(GetImTextureID(pixelSoundPlayerID[i]), ImVec2(panSize));
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

