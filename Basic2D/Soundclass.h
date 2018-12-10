#pragma once
#include "audiere.h"
using namespace audiere;

class Soundclass {
public:
	audiere::AudioDevice* device;
	Soundclass(){
		device = OpenDevice();
		
	}
	OutputStreamPtr play(char* path) {
		OutputStreamPtr sound(OpenSound(device, path, false));
		sound->play();
		return sound;
	}
};

extern class Soundclass classe_suono;    // IN MAIN.CPP