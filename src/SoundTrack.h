#pragma once

class SoundTrack{
public:
	ofRectangle rectArea;
	vec2 radius;
	static const int maxSize = 8;
	float radians[maxSize];
	vec2 posInit;
	vec2 posTargets[maxSize];
	float panMap[maxSize];
	vec2 center;
	ofColor colorBackground;
	ofColor colorForeground;

	//--------------------------------------------------------------
	void setup(vec2 center, vec2 areaSize, ofColor colorBackground, ofColor colorForeground) {
		this->center = center;
		rectArea.set(0, 0, areaSize.x, areaSize.y);
		this->colorBackground = colorBackground;
		this->colorForeground = colorForeground;
		radius = vec2(360, -360);
		posInit = vec2(center.x, ofGetHeight() -20);

		panMap[0] = 1.0;
		panMap[1] = 0.8;
		panMap[2] = 0.4;
		panMap[3] = 0.1;
		panMap[4] = -0.1;
		panMap[5] = -0.4;
		panMap[6] = -0.8;
		panMap[7] = -1.0;

		for (int i = 0; i < maxSize; i++) {
			radians[i] = (PI / maxSize) * (i + 0.5);
			float x = cos(radians[i]) * radius.x + posInit.x;
			float y = sin(radians[i]) * radius.y + posInit.y;
			posTargets[i] = vec2(x, y);
		}

		ofSetCircleResolution(128);
	}

	//--------------------------------------------------------------
	void draw() {
		ofPushStyle();
		ofSetColor(colorForeground);
		ofDrawEllipse(posInit, radius.x * 2.0, radius.y * 2.0);
		ofSetColor(colorBackground);
		ofDrawEllipse(posInit, radius.x * 1.95, radius.y * 1.95);
		ofNoFill();
		ofSetColor(colorForeground);
		for (int i = 0; i < 20; i++) {
			ofDrawEllipse(posInit, radius.x * (0.1 * i), radius.y * (0.1 * i));
		}
		for (int i = 0; i < maxSize; i++) {
			ofDrawLine(posInit, posTargets[i]);
		}
		ofPopStyle();
	}

};