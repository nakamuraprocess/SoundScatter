#pragma once

class SpatialPan{
private:
	ofRectangle SpatialPanArea;
	ofColor colorBackground;
	ofColor colorForeground;
	vec2 radius;
	vec2 pos;
	static const int spatialPanMaxSize = 7;
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
		
		panMap[0] = 1.0;
		panMap[1] = 0.8;
		panMap[2] = 0.4;
		panMap[3] = 0.0;
		panMap[4] = -0.4;
		panMap[5] = -0.8;
		panMap[6] = -1.0;

		cPanList[0] = "R3";
		cPanList[1] = "R2";
		cPanList[2] = "R1";
		cPanList[3] = "C";
		cPanList[4] = "L1";
		cPanList[5] = "L2";
		cPanList[6] = "L3";
		cPanList[7] = "LR(Random)";
		cPanList[8] = "LR(All)";

		for (int i = 0; i < spatialPanMaxSize; i++) {
			radians[i] = (PI / spatialPanMaxSize) * i + (PI / spatialPanMaxSize) * 0.5;
			float x = cos(radians[i]) * radius.x * 0.5;
			float y = sin(radians[i]) * radius.y * 0.5;
			posTargets[i] = vec2(x, y);
		}

		ofSetCircleResolution(128);
	}

	//--------------------------------------------------------------
	float getPan(int index) const {
		float pan = 0.0f;
		if (index == 7) {
			pan = ofSign(ofRandomf());
		}
		else if (index == 8) {
			int randomIndex = ofRandom(0, spatialPanMaxSize);
			pan = panMap[randomIndex];
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
		for (int i = 0; i < spatialPanMaxSize; i++) {
			ofDrawLine(0, 0, posTargets[i].x, posTargets[i].y);
		}
		ofPopStyle();
		ofPopMatrix();
	}

};