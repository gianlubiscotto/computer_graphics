#pragma once
#include "audiere.h"
using namespace audiere;

class Soundclass {
public:
	audiere::AudioDevice* device;
	Soundclass(){
		device = OpenDevice();
		
	}
	void play(char* path) {
		OutputStreamPtr sound(OpenSound(device, path, false));
			sound->play();
	}
};

extern class Soundclass classe_suono;    // IN MAIN.CPP