from Whisper import Whisper, AudioEngine

from BaseDb import Base, Tag, Sound, Track, SoundTags, TrackTags, TrackSounds, RegexHookedEngine

LOCATION = "sqlite:///db.sqlite";
DEBUG = True;

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker

# sqlalchemy regex support setup
# this defines the callback for sqlite
	
class ComposerDB(RegexHookedEngine):

	def __init__(self, location=None, echo=False):
		
		if location is None:
			location = LOCATION;

		self._dbEngine = create_engine( location, echo=echo );

		RegexHookedEngine.__init__(self, self._dbEngine)
		
		self.session = sessionmaker(bind=self._dbEngine);
		
		# create all tables
		Base.metadata.create_all(self._dbEngine)


	def with_session(*args, **session_kvargs):

		real_func = [None];

		def wrap(self, *args, **kvargs):
			session = self.session(**session_kvargs);
			try:
				return real_func[0](self, session, *args, **kvargs);
			finally:
				session.close();

		def decorator_constructor(fn):
			real_func[0] = fn;
			return wrap

		if len(args) > 1 or (len(args) is 1 and len(session_kvargs) is not 0):
			raise Exception(
				"Incorrect parameters, with_session accepts " +
				"either a single function, or a set of key " +
				"value parameters, but nothing else.");

		if len(args) is 1 and len(session_kvargs) is 0:
			real_func[0] = args[0];
			return wrap

		return decorator_constructor;


	@with_session(autocommit=False)
	def create_tag(self, session, name):
		"""Creates a new tag object with the specific name, iff it doesn't already exist""";

		# don't insert duplicates
		name_regexp = "^{}$".format(name);
		result = self._get_tags(session, name_regexp ).first();

		if result is not None:
			return result;

		#insert the value
		t = Tag(name=name);
		session.add(t);
		session.commit();
		return Tag(name=name);
	
	# internal use only
	def _get_tags(self, session, search_regex=None):
		query = session.query(Tag);
		return query.filter( self.regex_match(Tag.name, search_regex) );

	@with_session
	def get_tags(self, session, search_regex=None):
		"""Gets all of the tags that match search_regex.""";
		# look at _get_tags for how this actually works.
		return self._get_tags(session, search_regex).all();


c = ComposerDB(LOCATION, DEBUG)
print c.create_tag("hello")
result = c.get_tags("h")
if result is not None:
	for tag in result:
		print tag