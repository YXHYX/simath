#include "ResourceManager.h"

void ResourceManager::initializeSound()
{

	/* Open and initialize a device */
	soundDevice = NULL;
	
	if (!soundDevice)
		soundDevice = alcOpenDevice(NULL);
	if (!soundDevice)
	{
		throw"Could not open a device!\n";
		return;
	}

	this->soundContext = alcCreateContext(this->soundDevice, NULL);
	if (this->soundContext == NULL || alcMakeContextCurrent(this->soundContext) == ALC_FALSE)
	{
		if (this->soundContext != NULL)
			alcDestroyContext(this->soundContext);
		alcCloseDevice(this->soundDevice);
		throw"Could not set a context!\n";
		return;
	}

	deviceName = NULL;
	if (alcIsExtensionPresent(this->soundDevice, "ALC_ENUMERATE_ALL_EXT"))
		deviceName = alcGetString(this->soundDevice, ALC_ALL_DEVICES_SPECIFIER);
	if (!deviceName || alcGetError(this->soundDevice) != AL_NO_ERROR)
		deviceName = alcGetString(this->soundDevice, ALC_DEVICE_SPECIFIER);

}

void ResourceManager::setSoundListener()
{
	ALenum error;
	ALfloat listenerPos[] = { 0.0,0.0,0.0 };
	ALfloat listenerVel[] = { 0.0,0.0,0.0 };
	ALfloat listenerOri[] = { 0.0,0.0,-1.0, 0.0,1.0,0.0 };
	
	// Position ...
	alListenerfv(AL_POSITION, listenerPos);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		throw"alListenerfv POSITION : ";
		return;
	}
	// Velocity ...
	alListenerfv(AL_VELOCITY, listenerVel);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		throw "alListenerfv VELOCITY : ";
		return;
	}
	// Orientation ...
	alListenerfv(AL_ORIENTATION, listenerOri);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		throw"alListenerfv ORIENTATION : ";
		return;
	}
}

ResourceManager::ResourceManager()
{
	this->initializeSound();
}

ResourceManager::~ResourceManager()
{
	alcMakeContextCurrent(NULL);
	if(this->soundContext)
		alcDestroyContext(this->soundContext);
	alcCloseDevice(this->soundDevice);
}

void ResourceManager::loadTexture(std::string name, std::string path, int dither)
{
	this->textures[name] = std::make_shared<Texture>(path, dither);
	this->textures[name]->load();
}

void ResourceManager::bindTexture(std::string texName, std::string objName)
{
	try
	{
		this->objects.at(objName)->texture = this->textures.at(texName);
		int a = this->textures.at(texName).use_count();
	}
	catch(std::out_of_range e)
	{
		throw"RESOURCE_MANAGER::OUT_OF_RANGE_ERROR";
	}
}

void ResourceManager::loadSound(std::string name, std::string path)
{
	this->sounds[name] = std::make_shared<Sound>();
	this->sounds[name]->loadSound(path);
}

std::shared_ptr<Texture> ResourceManager::getTexture(std::string name)
{
	try
	{
		return this->textures.at(name);
	}
	catch(const std::out_of_range)
	{
		throw"RESOURCE_MANAGER::OUT_OF_RANGE_ERROR";
		return nullptr;
	}
}

std::shared_ptr<Object> ResourceManager::getObject(std::string name)
{
	try
	{
		return this->objects.at(name);
	}
	catch (const std::out_of_range)
	{
		throw"RESOURCE_MANAGER::OUT_OF_RANGE_ERROR";
		return nullptr;
	}
}

std::shared_ptr<Sound> ResourceManager::getSound(std::string name)
{
	try
	{
		return this->sounds.at(name);
	}
	catch (const std::out_of_range)
	{
		throw"RESOURCE_MANAGER::OUT_OF_RANGE_ERROR";
		return nullptr;
	}
}
