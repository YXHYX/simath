#pragma once
#include "AL/al.h"
#include "AL/alc.h"

#include "AL/alext.h"
#include "sndfile.h"
#include <string>
#include "tools.h"
//TODO add sound implementation 

class Sound
{
private:
	ALuint buffer;
	ALuint source;

	ALfloat offset;
	ALenum state;

	//source propreties
	mathT::vec3d sourcePos;
	mathT::vec3d sourceVel;
	mathT::vec3d sourceDir;

	bool sLooping;
	float sGain;
	float sPitch;
public:
	void loadSound(std::string path);
	Sound();
	~Sound();
	//setters/getters
	void setSourcePosition(mathT::vec3d position);
	void setSourceVelocity(mathT::vec3d velocity);
	void setSourceDirection(mathT::vec3d direction);
	void setPitch(float pitch);
	void setGain(float gain);

	
	
	
	void playSound();


};

