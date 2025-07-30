#pragma once

class SpatialPan{
private:
	ofRectangle SpatialPanArea;
	ofColor colorBackground;
	ofColor colorForeground;
	vec2 radius;
	vec2 pos;
	static const int spatialPanMaxSize = 9;
	float radians[spatialPanMaxSize];
	vec2 posTargets[spatialPanMaxSize];
	float panMap[spatialPanMaxSize];

public:
	const char* cPanList[9];

	//--------------------------------------------------------------
	void setup(vec2 pos, vec2 size, ofColor colorBackground, ofColor colorForeground) {
		float x = pos.x + (size.x * 0.5);
		float y = pos.y + (size.y * 0.5) + 18;
		this->pos = vec2(x, y);

		this->colorBackground = colorBackground;
		this->colorForeground = colorForeground;
		SpatialPanArea.set(-size.x * 0.5, -size.y * 0.5, size.x, size.y);
		radius = vec2(size.x * 0.8, -size.x * 0.8);
		
		panMap[0] = 0.0;
		panMap[1] = 0.0;
		panMap[2] = 0.0;
		panMap[3] = 1.0;
		panMap[4] = 0.8;
		panMap[5] = 0.4;
		panMap[6] = -0.4;
		panMap[7] = -0.8;
		panMap[8] = -1.0;

		cPanList[0] = "RANDOM";
		cPanList[1] = "RANDOM LR";
		cPanList[2] = "CENTER";
		cPanList[3] = "R1";
		cPanList[4] = "R2";
		cPanList[5] = "R3";
		cPanList[6] = "L3";
		cPanList[7] = "L2";
		cPanList[8] = "L1";


		for (int i = 0; i < 7; i++) {
			radians[i] = (PI / 7) * i + (PI / 7) * 0.5;
			float x = cos(radians[i]) * radius.x * 0.5;
			float y = sin(radians[i]) * radius.y * 0.5;
			posTargets[i] = vec2(x, y);
		}

		ofSetCircleResolution(128);
	}

	//--------------------------------------------------------------
	float getPan(int index) const {
		float pan = 0.0f;
		if (cPanList[index] == "RANDOM") {
			int randomIndex = ofRandom(0, spatialPanMaxSize);
			pan = panMap[randomIndex];
		}
		else if (cPanList[index] == "RANDOM LR") {
			pan = ofSign(ofRandomf());
		}
		else {
			pan = panMap[index];
		}
		return pan;
	}

	//--------------------------------------------------------------
	void draw(int index) const {
		ofPushMatrix();
		ofTranslate(pos);
		ofPushStyle();
		ofSetColor(117, 131, 142);
		ofDrawRectangle(SpatialPanArea);
		ofSetColor(colorForeground);
		ofDrawEllipse(0, 0, radius.x, radius.y);
		ofSetColor(colorBackground);
		ofDrawEllipse(0, 0, radius.x * 0.95, radius.y * 0.95);
		ofNoFill();
		ofSetColor(colorForeground);
		for (int i = 0; i < 9; i++) {
			ofDrawEllipse(0, 0, radius.x * (0.1 * i), radius.y * (0.1 * i));
		}
		if (cPanList[index] == "RANDOM") {
			for (int i = 0; i < spatialPanMaxSize; i++) {
				ofDrawLine(0, 0, posTargets[i].x, posTargets[i].y);
			}
		}
		else if (cPanList[index] == "RANDOM LR") {
			ofDrawLine(0, 0, posTargets[0].x, posTargets[0].y);
			ofDrawLine(0, 0, posTargets[6].x, posTargets[6].y);
		}
		else if (cPanList[index] == "CENTER") {
			ofDrawLine(0, 0, posTargets[3].x, posTargets[3].y);
		}
		else if (cPanList[index] == "R1") {
			ofDrawLine(0, 0, posTargets[0].x, posTargets[0].y);
		}
		else if (cPanList[index] == "R2") {
			ofDrawLine(0, 0, posTargets[1].x, posTargets[1].y);
		}
		else if (cPanList[index] == "R3") {
			ofDrawLine(0, 0, posTargets[2].x, posTargets[2].y);
		}
		else if (cPanList[index] == "L3") {
			ofDrawLine(0, 0, posTargets[4].x, posTargets[4].y);
		}
		else if (cPanList[index] == "L2") {
			ofDrawLine(0, 0, posTargets[5].x, posTargets[5].y);
		}
		else if (cPanList[index] == "L1") {
			ofDrawLine(0, 0, posTargets[6].x, posTargets[6].y);
		}
		ofPopStyle();
		ofPopMatrix();
	}
};