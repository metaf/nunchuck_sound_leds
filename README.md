# nunchuck_sound_leds
Control NeoPixel LED strips with a Wii nunchuck, and eventually a microphone.

NeoPixel LED strips have anywhere from 8 to hundreds of individually addressable full RGB color LEDs.

The idea is to use the LEDs as a music-visualizer.  For now I'm experimenting with
nunchuck control as a way to set patterns on a per-LED basis.  Going forward I'd like
to do some simple DSP stuff to make the LEDs sound-reactive.  I might need to migrate
to a Raspberry Pi at that point since the Arduino is probably underpowered for DSP.

One simple idea would be to split the microphone input into bands (like an equalizer, 60-200hz, 200-1000hz, 1khz-3khz, 
and >3khz for example) and then try to pulse LEDs in rhythm per band with coloring depending on volume/actual frequency 
within the band.  

