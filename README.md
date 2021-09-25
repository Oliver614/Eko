# Eko
Multi-tap FDN reverb 
#
Modelled on the Rounds Reverb by native istruments with some additional features not found on the orginal. 

Eko uses a multi-tap diffusion network topology with LFO driven diffusion timings to try and help maximise the diffusions through the 8 diffusion stages.

Low Diffusion and Size, with high Feedback gives more metalic sounding noises while high Diffusion Feedback and Size give massive lush reverbs. 


# Controls
Pre-Delay - Predelay controls the amount of thime it takes for the imput signal to reach the reverb, giving a greater sense of space. Current max and min times 0 - 2 s.

Pre-Delay Feedback - Feeds back the delayed signal back into the delay buffer. 

Diffusion - Controls where in the diffusion network the output signal is tapped from, giving a less or more diffuse sound. 

Feedback - Controls how much of the reverb is fed back on itself, creating longer tails.

Size - Controls the stereo field of the reverb as well as how long the tails last. 

Center - Moves the diffusion timings together.

Spread - Increases teh spread of the diffusion timings. 

HP/LP - Filters the incoming signa. 

Colour - Filters post Reverb  > 0 is highpass, < 0 lowpass.

Emphasis - sets a slight resonance at the colour level. 

Mix - Controls the wet dry level.

# Known issues

Changing the diffusion timings can cause some clicking noises, interpolation needed to fix this.

UI needs some changes.

