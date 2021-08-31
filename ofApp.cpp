#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(0.5);
	ofEnableDepthTest();

	this->size = 5;
	ofColor color;
	for (auto x = -450; x <= 450; x += this->size) {

		for (auto y = -450; y <= 450; y += this->size) {

			this->box_info_list.push_back(make_pair(glm::vec2(x, y), 0.f));

			color.setHsb(ofRandom(255), 130, 255);
			this->box_color_list.push_back(color);
		}
	}

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	this->face.clear();
	this->frame.clear();

	if (ofGetFrameNum() % 20 == 0) {

		auto l = glm::vec2(ofRandom(-250, 250), ofRandom(-250, 250));
		auto f = 5;
		this->wave_list.push_back(make_pair(l, f));
	}

	auto max_height = 150;
	for (auto& wave : this->wave_list) {

		for (auto& box_info : this->box_info_list) {

			auto distance = glm::distance(wave.first, glm::vec2(box_info.first.x, box_info.first.y));
			if (distance > wave.second - 10 && distance < wave.second + 10) {

				box_info.second = (box_info.second >= 1) ? 1 : box_info.second + ofMap(abs(distance - wave.second), 0, 20, 0.125, 0);
			}
			else {

				box_info.second = (box_info.second <= 0) ? 0 : box_info.second - 0.018;
			}		
		}

		wave.second += 3;
	}

	ofColor color;
	for (int i = 0; i < this->box_info_list.size(); i++) {

		auto& box_info = this->box_info_list[i];
		if (box_info.second > 0) {

			auto len = ofMap(ofNoise(box_info.first.x * 0.02, box_info.first.y * 0.02, ofGetFrameNum() * 0.01), 0, 1, 0, max_height * box_info.second);
			this->setBoxToMesh(this->face, this->frame, glm::vec3(box_info.first.x, box_info.first.y, len * 0.5), this->size, this->size, len, this->box_color_list[i]);
		}
		else {

			color.setHsb(ofRandom(255), 130, 255);
			this->box_color_list[i] = color;
		}
	}

	for (int i = this->wave_list.size() - 1; i >= 0; i--) {

		if (this->wave_list[i].second > 450) {

			this->wave_list.erase(this->wave_list.begin() + i);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(295);

	this->face.drawFaces();

	ofSetColor(39);
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setBoxToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float size, ofColor color) {

	this->setBoxToMesh(face_target, frame_target, location, size, size, size, color);
}

//--------------------------------------------------------------
void ofApp::setBoxToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float height, float width, float depth, ofColor color) {

	int index = face_target.getVertices().size();

	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.999, height * 0.5 * 0.999, depth * -0.5 * 0.999));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.999, height * 0.5 * 0.999, depth * -0.5 * 0.999));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.999, height * 0.5 * 0.999, depth * 0.5 * 0.999));
	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.999, height * 0.5 * 0.999, depth * 0.5 * 0.999));

	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.999, height * -0.5 * 0.999, depth * -0.5 * 0.999));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.999, height * -0.5 * 0.999, depth * -0.5 * 0.999));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.999, height * -0.5 * 0.999, depth * 0.5 * 0.999));
	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.999, height * -0.5 * 0.999, depth * 0.5 * 0.999));

	face_target.addIndex(index + 0); face_target.addIndex(index + 1); face_target.addIndex(index + 2);
	face_target.addIndex(index + 0); face_target.addIndex(index + 2); face_target.addIndex(index + 3);

	face_target.addIndex(index + 4); face_target.addIndex(index + 5); face_target.addIndex(index + 6);
	face_target.addIndex(index + 4); face_target.addIndex(index + 6); face_target.addIndex(index + 7);

	face_target.addIndex(index + 0); face_target.addIndex(index + 4); face_target.addIndex(index + 1);
	face_target.addIndex(index + 4); face_target.addIndex(index + 5); face_target.addIndex(index + 1);

	face_target.addIndex(index + 1); face_target.addIndex(index + 5); face_target.addIndex(index + 6);
	face_target.addIndex(index + 6); face_target.addIndex(index + 2); face_target.addIndex(index + 1);

	face_target.addIndex(index + 2); face_target.addIndex(index + 6); face_target.addIndex(index + 7);
	face_target.addIndex(index + 7); face_target.addIndex(index + 3); face_target.addIndex(index + 2);

	face_target.addIndex(index + 3); face_target.addIndex(index + 7); face_target.addIndex(index + 4);
	face_target.addIndex(index + 4); face_target.addIndex(index + 0); face_target.addIndex(index + 3);

	frame_target.addVertex(location + glm::vec3(width * -0.5, height * 0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * 0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * 0.5, depth * 0.5));
	frame_target.addVertex(location + glm::vec3(width * -0.5, height * 0.5, depth * 0.5));

	frame_target.addVertex(location + glm::vec3(width * -0.5, height * -0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * -0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * -0.5, depth * 0.5));
	frame_target.addVertex(location + glm::vec3(width * -0.5, height * -0.5, depth * 0.5));

	frame_target.addIndex(index + 0); frame_target.addIndex(index + 1);
	frame_target.addIndex(index + 1); frame_target.addIndex(index + 2);
	frame_target.addIndex(index + 2); frame_target.addIndex(index + 3);
	frame_target.addIndex(index + 3); frame_target.addIndex(index + 0);

	frame_target.addIndex(index + 4); frame_target.addIndex(index + 5);
	frame_target.addIndex(index + 5); frame_target.addIndex(index + 6);
	frame_target.addIndex(index + 6); frame_target.addIndex(index + 7);
	frame_target.addIndex(index + 7); frame_target.addIndex(index + 4);

	frame_target.addIndex(index + 0); frame_target.addIndex(index + 4);
	frame_target.addIndex(index + 1); frame_target.addIndex(index + 5);
	frame_target.addIndex(index + 2); frame_target.addIndex(index + 6);
	frame_target.addIndex(index + 3); frame_target.addIndex(index + 7);

	for (int i = 0; i < 8; i++) {

		face_target.addColor(ofColor(color));

	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}