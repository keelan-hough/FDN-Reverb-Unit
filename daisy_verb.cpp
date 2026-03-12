#include <string.h>
#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;

DaisySeed hw;
daisysp::Oscillator osc;

void AudioCallback(AudioHandle::InputBuffer in,
				   AudioHandle::OutputBuffer out,
				   size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		float sig = osc.Process();
		out[0][i] = sig;
		out[1][i] = sig;
	}
}

int main(void)
{
	// initialising the hardware
	hw.Configure();
	hw.Init();
	// configuring DS for audio
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	// oscillator config
	osc.Init(hw.AudioSampleRate()); // set the osc sr to the same as the board
	osc.SetFreq(1000);
	osc.SetAmp(1);
	// start audio callback
	hw.StartAudio(AudioCallback);
	// getting the bastard LED blinking
	bool led_state;
	led_state = true;
	for(;;)
	{
		hw.SetLed(led_state);
		led_state = !led_state;
		System::Delay(500);
	}
	while(1){}
}
