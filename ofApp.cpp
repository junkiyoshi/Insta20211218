#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(255);
	ofEnableDepthTest();

	int span = 30;
	for (int x = -300; x <= 300; x += span) {

		for (int y = -300; y <= 300; y += span) {

			for (int z = 0; z <= 2400; z += span * 2) {

				this->location_list.push_back(glm::vec3(x, y, z));
				this->rotate_deg_list.push_back(0);
				this->param_list.push_back(0);
				this->word_index_list.push_back(0);
			}
		}
	}

	for (auto& location : this->location_list) {

		vector<int> route_info = vector<int>();
		int index = -1;
		for (auto& other : this->location_list) {

			index++;
			if (location == other) { continue; }

			float distance = glm::distance(location, other);
			if (distance <= span) {

				route_info.push_back(index);
			}
		}

		this->route_info_list.push_back(route_info);
	}

	for (int i = 0; i < 50; i++) {

		this->index_list.push_back((int)ofRandom(this->location_list.size()));
	}

	this->font_size = 23;
	ofTrueTypeFontSettings font_settings("fonts/msgothic.ttc", this->font_size);
	font_settings.antialiased = true;
	font_settings.addRanges(ofAlphabet::Japanese);
	this->font.load(font_settings);

	this->words = {

		u8"‚ ", u8"‚¢", u8"‚¤", u8"‚¦", u8"‚¨",
		u8"‚©", u8"‚«", u8"‚­", u8"‚¯", u8"‚±",
		u8"‚³", u8"‚µ", u8"‚·", u8"‚¹", u8"‚»",
		u8"‚½", u8"‚¿", u8"‚Â", u8"‚×", u8"‚Æ",
		u8"‚È", u8"‚É", u8"‚Ê", u8"‚Ë", u8"‚Ì",
		u8"‚Í", u8"‚Ð", u8"‚Ó", u8"‚Ö", u8"‚Ù",
		u8"‚Ü", u8"‚Ý", u8"‚Þ", u8"‚ß", u8"‚à",
		u8"‚â", u8"‚ä", u8"‚æ",
		u8"‚ç", u8"‚è", u8"‚é", u8"‚ê", u8"‚ë",
		u8"‚í", u8"‚ð", u8"‚ñ",	
	};
}

//--------------------------------------------------------------
void ofApp::update() {

	if (ofGetFrameNum() % 4 != 0) {

		for (int i = 0; i < this->index_list.size(); i++) {

			this->word_index_list[this->index_list[i]] = (int)ofRandom(this->words.size());
		}
		return;
	}

	for (int i = 0; i < this->index_list.size(); i++) {

		int next_index = this->route_info_list[this->index_list[i]][(int)ofRandom(this->route_info_list[this->index_list[i]].size())];
		for (int k = 0; k < this->route_info_list[this->index_list[i]].size(); k++) {

			if (this->param_list[next_index] <= 0) {

				auto loc = this->location_list[this->index_list[i]];
				auto next = this->location_list[next_index];

				auto angle = std::atan2(next.y - loc.y, next.x - loc.x);
				if (angle != 0) {
				
					this->rotate_deg_list[next_index] = angle * RAD_TO_DEG + 90;
				}
				this->param_list[next_index] = 60;
				this->index_list[i] = next_index;
				this->word_index_list[next_index] = (int)ofRandom(this->words.size());
				break;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	for (int i = 0; i < this->location_list.size(); i++) {

		if (this->param_list[i] > 0) {

			int z = (int)(this->location_list[i].z + ofGetFrameNum() * 4) % 1200 - 600;

			ofPushMatrix();
			ofTranslate(this->location_list[i].x, this->location_list[i].y, z);
	
			ofFill();
			int alpha = ofMap(this->param_list[i], 0, 60, 0, 255);
			ofSetColor(ofColor(0, alpha));

			ofRotateZ(this->rotate_deg_list[i]);
			this->font.drawString(this->words[this->word_index_list[i]], -13, -13);

			ofPopMatrix();
		}

		if (this->param_list[i] > 0) { this->param_list[i] -= 2; }
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}