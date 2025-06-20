#pragma once

class SpatialPan{
public:
	ofRectangle SpatialPanArea;
	vec2 radius;
	vec2 pos;
	static const int spatialPanMaxSize = 8;
	float radians[spatialPanMaxSize];
	vec2 posTargets[spatialPanMaxSize];
	float panMap[spatialPanMaxSize];
	ofColor colorBackground;
	ofColor colorForeground;

	//--------------------------------------------------------------
	void setup(vec2 pos, vec2 size, ofColor colorBackground, ofColor colorForeground) {
		SpatialPanArea.set(-size.x * 0.5, -size.y * 0.5, size.x, size.y);
		float x = pos.x + size.x * 0.5;
		float y = pos.y + size.y * 0.5;
		this->pos = vec2(x, y);
		this->colorBackground = colorBackground;
		this->colorForeground = colorForeground;
		radius = vec2(size.x * 0.8, -size.x * 0.8);
		
		panMap[0] = 1.0;
		panMap[1] = 0.8;
		panMap[2] = 0.4;
		panMap[3] = 0.1;
		panMap[4] = -0.1;
		panMap[5] = -0.4;
		panMap[6] = -0.8;
		panMap[7] = -1.0;

		for (int i = 0; i < spatialPanMaxSize; i++) {
			radians[i] = (PI / spatialPanMaxSize) * i;
			float x = cos(radians[i]) * radius.x * 0.5;
			float y = sin(radians[i]) * radius.y * 0.5;
			posTargets[i] = vec2(x, y);
		}

		ofSetCircleResolution(128);
	}

	//--------------------------------------------------------------
	void draw() {
		ofPushMatrix();
		ofTranslate(pos);
		ofPushStyle();
		ofSetColor(180, 180, 180);
		ofDrawRectangle(SpatialPanArea);
		ofSetColor(colorForeground);
		ofDrawEllipse(0, 0, radius.x, radius.y);
		ofSetColor(colorBackground);
		ofDrawEllipse(0, 0, radius.x, radius.y);
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