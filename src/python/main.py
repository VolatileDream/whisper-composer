from Whisper import AudioEngine, Sound, Whisper

import math, time

if __name__ == "__main__":
	
	raw = [];
	for i in range(0,int(math.pow(2,15))):
		raw.append( math.sin( i * 2.0/200.0 * math.pi ));

	sound = Sound.fromFloatList( raw );

	whisper = Whisper();

	engine = whisper.CreateEngine(None);

	engine.AddSound( sound, True );

	time.sleep(10);