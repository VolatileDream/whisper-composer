import ctypes

# for destroying all the C resources we're using
import atexit;

# small internal decorator first...
def LibFunction(getLibCallFunc, restype, argtypes):
	
	def builder(func):
		
		def decorator(self, *params):

			libcall = getLibCallFunc( self );
			libcall.restype = restype
			libcall.argtypes = argtypes;

			result = libcall(*params);
			return func(self, result, *params)

		return decorator;

	return builder;


# Now the public stuff:

class AudioEngine(ctypes.c_void_p):
	"""Wrapper class for the C struct
		This is really just a way to have an opaque pointer"""

	def _setLib(self, whisper):
		self._whisper = whisper;

	def AddSound(self, sound, flagFinished):
		return self._whisper.AddSound(self, ctypes.pointer(sound), ctypes.c_bool(flagFinished) );

	def GetFinishedSound(self):
		return self._whisper.GetFinishedSound(self);

	def Dispose(self):
		if self.value is not None:
			self._whisper.DestroyEngine(self);

		# protect ourselves later
		self.value = None;


class Sound(ctypes.Structure):
	"""Wrapper for the C struct"""

	_fields_ = [('audioData', ctypes.POINTER(ctypes.c_float)),
				('length', ctypes.c_ushort ) ];

	@staticmethod
	def fromFloatList( listOfFloat ):
		array = (ctypes.c_float*len(listOfFloat))(*listOfFloat);
		return Sound(array, len(listOfFloat));


class Whisper(object):
	"""Wrapper for the C library"""

	def __init__(self):
		self._engines = {};
		atexit.register(lambda : self._cleanup());
		self._loadLib();
		
		
	def _loadLib(self):
		try:
			self._lib = ctypes.CDLL("./Whisper.o");
			# setup the library
			self._setupLib();
		except Exception, e:
			raise e
		finally:
			pass

	def _setupLib(self):
		lib = self._lib;
		lib.WC_Allocate.restype = AudioEngine;
		lib.WC_Allocate.argtypes = []; # no args

		lib.WC_Init.restype = ctypes.c_bool;
		lib.WC_Init.argtypes = [AudioEngine, ctypes.c_void_p];

		lib.WC_Dispose.restype = None;
		lib.WC_Dispose.argtypes = [ AudioEngine ];

	def CreateEngine(self, settings):
		engine = self._lib.WC_Allocate();

		if engine and self._lib.WC_Init(engine, ctypes.c_void_p(settings)) :
			self._engines[engine.value] = engine;
			engine._setLib(self);
			return engine;

		# initialization failure
		# C lib takes care of cleanup
		return None;

	@LibFunction(lambda x: x._lib.WC_Dispose, None, [AudioEngine])
	def DestroyEngine(self, result, engine):
		self._engines.pop(engine.value, None);
		engine.value = None;

	@LibFunction(lambda x: x._lib.WC_AddSound, ctypes.c_bool, [AudioEngine, ctypes.POINTER(Sound), ctypes.c_bool])
	def AddSound(self, result, engine, sound, flagFinish ):
		# any extra stuff after the function is called
		return result

	@LibFunction(lambda x: x._lib.WC_GetFinishedSound, ctypes.POINTER(Sound), [AudioEngine])
	def GetFinishedSound(self, result_sound, engine):
		return result_sound;

	# registered with atexit
	def _cleanup(self):
		# destroy all allocated C resources
		for (ptr, engine) in self._engines.items():
			self.DestroyEngine(engine);
