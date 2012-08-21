#include "ACOpenAL.h"

#pragma region Delegate implementation
//methodo externo a classe q cria o adevice
HRESULT CreateAudioDevice(HINSTANCE hDLL, ACAudioDevice **pDevice)
{
	if (!*pDevice)
	{
		*pDevice = new ACOpenAL(hDLL);
		return AC_OK;
	}

	return AC_FAIL;
};

//metodo externo q libera o adevice
HRESULT ReleaseAudioDevice(ACAudioDevice **pDevice)
{
	if (!*pDevice)
	{
		return AC_FAIL;
	}

	SAFE_DELETE(*pDevice);

	return AC_OK;
};
#pragma endregion

#pragma region Contructor and Destructor
ACOpenAL::ACOpenAL(HINSTANCE hDLL)
{
	mhDLL = hDLL;
	mpLOG = nullptr;
};

ACOpenAL::~ACOpenAL()
{
	Release();
};

void ACOpenAL::Release()
{
	alcDestroyContext(mpContext);
	alcCloseDevice(mpDevice);

	if (mpLOG!=nullptr)
	{
		fclose(mpLOG);
		mpLOG = nullptr;
	}
};
#pragma endregion

#pragma region Initialization
HRESULT ACOpenAL::Init(HWND hWnd, bool log)
{
	HRESULT hr = NULL;

	if (log)
		mpLOG = fopen("ACOpenAL.txt", "w");

	Log("Begin Init");

	// Initialization
	mpDevice = alcOpenDevice(nullptr); // select the "preferred device"
	if (mpDevice)
	{
		mpContext=alcCreateContext(mpDevice,nullptr);
		alcMakeContextCurrent(mpContext);
	}

	// Check for EAX 2.0 support
	EnableEAX = alIsExtensionPresent("EAX2.0");

	// Generate Buffers
	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		Log("[ERROR] Creating device or context sound file: %x.", error);
		return AC_FAIL;
	}

	Log("Success Init");

	return hr;
};
#pragma endregion

#pragma region LOG
void ACOpenAL::Log(char* message, ...)
{
	if (mpLOG!=nullptr)
	{
	   char ch[256];
	   char *pArgs;

	   pArgs = (char*) &message + sizeof(message);
	   vsprintf(ch, message, pArgs);
	   fprintf(mpLOG, ch);
	   fprintf(mpLOG, "\n");
	   fflush(mpLOG);
	}
};
#pragma endregion

#pragma region LoadSound
HRESULT ACOpenAL::LoadSound(const std::string &name, ACSound** ppOutSoundPtr)
{
	HRESULT hr;

	std::string currentPath = ACGlobals::GetPathSounds();
	currentPath.append(name);

	*ppOutSoundPtr = new ACSound();

	//dados de retorno do decoder ogg
	std::vector<char> buffer;
	ALenum format;
	ALsizei freq;

	alGenBuffers(NUM_BUFFERS, &(*ppOutSoundPtr)->SoundID);

	//carrega o ogg do arquivo e armazena no buffer
	OGGLoader::DecodeOGG(currentPath.c_str(), buffer, format, freq);

	//armazena no buffer do OpenAL
	alBufferData((*ppOutSoundPtr)->SoundID, format, &buffer[0], static_cast<ALsizei>(buffer.size()), freq);

	//limpa o buffer pq ele ja ta dentro do dispositivo de audio
	buffer.clear();

	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		Log("[ERROR] Loading sound file: %x.", error);
		return AC_FAIL;
	}

	return AC_OK;
};

void ACOpenAL::ReleaseSound(const ACSound* sound)
{
	alDeleteBuffers(1, &sound->SoundID);
	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		Log("[ERROR] Unloading sound file: %x.", error);
	}
};

#pragma endregion

#pragma region Sound Manipulation

// play a specific sound
void ACOpenAL::PlaySound(UINT sourceID)
{
	alSourcePlay(sourceID);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		Log("[ERROR] PlaySound : %x.", error);
	}
};

// stop a specific sound
void ACOpenAL::StopSound(UINT sourceID)
{
	alSourceStop(sourceID);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		Log("[ERROR] StopSound : %x.", error);
	}
};

// pause a specific sound
void ACOpenAL::PauseSound(UINT sourceID)
{
	alSourcePause(sourceID);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		Log("[ERROR] PauseSound : %x.", error);
	}
};

#pragma endregion

#pragma region Listener Methods
// set listener parameters
void ACOpenAL::SetListener(const Vector3& position, const Vector3& direction, const Vector3& up, const Vector3& velocity)
{
	alListener3f(AL_POSITION, position.X, position.Y, position.Z);
	alListener3f(AL_VELOCITY, velocity.X,velocity.Y,velocity.Z);

	ALfloat* v = new ALfloat[6];
	v[0] = direction.X;
	v[1] = direction.Y;
	v[2] = direction.Z;
	v[4] = up.X;
	v[5] = up.Y;
	v[6] = up.Z;
	alListenerfv(AL_ORIENTATION, v);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		Log("[ERROR] SetListener : %x.", error);
	}
};

#pragma endregion

#pragma region Source Methods
void ACOpenAL::AddSource(UINT* outSourceID)
{
	alGenSources(1, outSourceID);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		Log("[ERROR] AddSource : %x.", error);
	}
};

void ACOpenAL::RemoveSource(const UINT& sourceID)
{
	alDeleteSources(1, &sourceID);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		Log("[ERROR] RemoveSource : %x.", error);
	}
};

void ACOpenAL::BindSourceToBuffer(const UINT& sourceID, const UINT& bufferID)
{
	alSourcei(sourceID, AL_BUFFER, bufferID);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		Log("[ERROR] BindSourceToBuffer : %x.", error);
	}
};

void ACOpenAL::UnbindSourceBuffer(const UINT& sourceID, const UINT& bufferID)
{
	alSourcei(sourceID, AL_BUFFER, NULL);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		Log("[ERROR] BindSourceToBuffer : %x.", error);
	}
};

void ACOpenAL::SetSourceLoop(const UINT& sourceID, const bool& enableLoop)
{
	alSourcei(sourceID, AL_LOOPING, enableLoop);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		Log("[ERROR] SetSourceLoop : %x.", error);
	}
};

void ACOpenAL::SetSourcePosition(const UINT& sourceID, const Vector3* position)
{
	alSource3f(sourceID, AL_POSITION, position->X, position->Y, position->Z);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		Log("[ERROR] SetSourcePosition : %x.", error);
	}
};

void ACOpenAL::SetSourceDirection(const UINT& sourceID, const Vector3* direction)
{
	alSource3f(sourceID, AL_DIRECTION, direction->X, direction->Y, direction->Z);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		Log("[ERROR] SetSourceDirection : %x.", error);
	}
};

void ACOpenAL::SetSourceVelocity(const UINT& sourceID, const Vector3* velocity)
{
	alSource3f(sourceID, AL_VELOCITY, velocity->X, velocity->Y, velocity->Z);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		Log("[ERROR] SetSourceVelocity : %x.", error);
	}
};

void ACOpenAL::SetSourcePitch(const UINT& sourceID, const float& pitch)
{
	alSourcef(sourceID, AL_PITCH, pitch);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		Log("[ERROR] SetSourcePitch : %x.", error);
	}
};

void ACOpenAL::SetSourceGain(const UINT& sourceID, const float& gain)
{
	alSourcef(sourceID, AL_GAIN, gain);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		Log("[ERROR] SetSourceGain : %x.", error);
	}
};

#pragma endregion