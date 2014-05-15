


#include "ofxSettings.h"
ofxSettings settings;

ofxSettings::ofxSettings() {
	// default value for settings.
	xmlFilePath = "settings.xml";
}
/**
 * Loads the file.
 */
void ofxSettings::load(string path) {
	xmlFilePath = path;
	data.clear();
	ofxXmlSettings xml;
	if(!xml.loadFile(xmlFilePath)) {
		ofLog(OF_LOG_WARNING, "Can't load xml from %s\n", xmlFilePath.c_str());
		return;
	}

	xml.pushTag("properties");
	int numProps = xml.getNumTags("property");
	for(int i = 0; i < numProps; i++) {
		string key = xml.getAttribute("property", "key", "", i);
		string value = xml.getAttribute("property", "value", "", i);
		data[key] = value;
	}
	// even if the file isn't found, loaded should be true
	loaded = true;
}



/**
 * Saves the file. If you don't pass a parameter, it'll try to
 * save it to the last place it was loaded from.
 */
bool ofxSettings::save(string path) {
	if(path!="") xmlFilePath = path;
	if(xmlFilePath=="") {
		ofLog(OF_LOG_ERROR, "Error, no xml file specified\n");
		return false;
	}

	ofxXmlSettings xml;
	xml.addTag("properties");
	xml.pushTag("properties");


	map<string,string>::iterator it;
	int i = 0;
	for ( it=data.begin() ; it != data.end(); it++ ) {
		xml.addTag("property");
		xml.setAttribute("property", "key", (*it).first, i);
		xml.setAttribute("property", "value", (*it).second, i);
		i++;
	}
	if(xml.saveFile(xmlFilePath)) {
		ofLog(OF_LOG_NOTICE, "Saved to %s\n", ofToDataPath(xmlFilePath).c_str());
		return true;
	} else {
		ofLog(OF_LOG_ERROR, "Couldn't save to %s\n", ofToDataPath(xmlFilePath).c_str());
		return false;
	}
}


void ofxSettings::set(string key, string value) {
	data[key] = value;
	save();
}



string ofxSettings::get(string key, string defaultValue) {
	if(!loaded) load(xmlFilePath);
	if(data.find(key)==data.end()) {
		set(key, defaultValue);
		return defaultValue;
	}
	return data[key];
}


void ofxSettings::set(string key, bool value) {
	set(key, string((value?"true":"false")));
}

void ofxSettings::set(string key, int value) {
	set(key, ofToString(value));
}

void ofxSettings::set(string key, float value) {
	set(key, ofToString(value));
}

void ofxSettings::set(string key, const ofVec3f &value) {
	string val = ofToString(value.x) + "," + ofToString(value.y) + "," + ofToString(value.z);
	set(key, val);
}

void ofxSettings::set(string key, const ofRectangle &value) {
	string val = ofToString(value.x) + "," + ofToString(value.y) + "," + ofToString(value.width) + "," + ofToString(value.height);
	set(key, val);
}



int ofxSettings::getInt(string key, int defaultValue) {
	return ofToInt(get(key, ofToString(defaultValue)));
}

float ofxSettings::getFloat(string key, float defaultValue) {
	return ofToFloat(get(key, ofToString(defaultValue)));
}



bool ofxSettings::getBool(string key, bool defaultValue) {
	string val = get(key, defaultValue?"true":"false");
	return val=="true" || val=="yes" || val=="1" || val=="TRUE" || val=="YES";
}

ofVec3f ofxSettings::getVec3(string key, ofVec3f value) {
	string val = ofToString(value.x) + "," + ofToString(value.y) + "," + ofToString(value.z);
	val = get(key, val);
	
	vector<string> parts = ofSplitString(val, ",");
	if(parts.size()==3) {
		value.x = ofToFloat(parts[0]);
		value.y = ofToFloat(parts[1]);
		value.z = ofToFloat(parts[2]);
		//printf("vals: %f %f %f\n", value.x, value.y, value.z);
	} else {
		printf("Parse error in ofxSettings::getVec3() - value is '%s'\n", val.c_str());
	}
	return value;
}




ofRectangle ofxSettings::getRect(string key, ofRectangle value) {
	
	
	string val = ofToString(value.x) + "," + ofToString(value.y) + "," + ofToString(value.width) + "," + ofToString(value.height);
	val = get(key, val);
	
	vector<string> parts = ofSplitString(val, ",");
	if(parts.size()==4) {
		value.x      = ofToFloat(parts[0]);
		value.y      = ofToFloat(parts[1]);
		value.width  = ofToFloat(parts[2]);
		value.height = ofToFloat(parts[3]);

	} else {
		printf("Parse error in ofxSettings::getRect() - value is '%s'\n", val.c_str());
	}
	return value;
}
