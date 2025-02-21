#include "plpch.h"
#include "AudioLayer.h"

#include "Primitivo/Audio/Audio.h"
#include "Primitivo/Core/Layer.h"


namespace Primitivo {
	AudioLayer::AudioLayer() : Layer("AudioLayer") {
		AudioCommand::InitAudioEngine();
	}

	 AudioLayer::~AudioLayer() {
		AudioCommand::DeinitAudioEngine();
	}
}