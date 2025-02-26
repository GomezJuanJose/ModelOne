#include "mopch.h"
#include "AudioLayer.h"

#include "ModelOne/Audio/Audio.h"
#include "ModelOne/Core/Layer.h"


namespace ModelOne {
	AudioLayer::AudioLayer() : Layer("AudioLayer") {
		AudioCommand::InitAudioEngine();
	}

	 AudioLayer::~AudioLayer() {
		AudioCommand::DeinitAudioEngine();
	}
}