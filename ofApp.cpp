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

		u8"あ", u8"い", u8"う", u8"え", u8"お",
		u8"か", u8"き", u8"く", u8"け", u8"こ",
		u8"さ", u8"し", u8"す", u8"せ", u8"そ",
		u8"た", u8"ち", u8"つ", u8"べ", u8"と",
		u8"な", u8"に", u8"ぬ", u8"ね", u8"の",
		u8"は", u8"ひ", u8"ふ", u8"へ", u8"ほ",
		u8"ま", u8"み", u8"む", u8"め", u8"も",
		u8"や", u8"ゆ", u8"よ",
		u8"ら", u8"り", u8"る", u8"れ", u8"ろ",
		u8"わ", u8"を", u8"ん",	
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