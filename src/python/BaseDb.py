from sqlalchemy import Column, Integer, String, Sequence, ForeignKey
from sqlalchemy.ext.declarative import declared_attr, declarative_base
Base = declarative_base();


# RegexHookedEngine stuff
import re
from sqlalchemy.event import listens_for

def re_fn(expr, item):
	reg = re.compile(expr, re.I);
	return reg.search(item) is not None;

class RegexHookedEngine(object):

	__REGEX_FUNC = 'regexp';

	def __init__(self, engine=None, **kvargs):
		# setup regex support for sqlalchemy
		# this hooks up the callback in sqlite
		listens_for(engine, "begin")(self.hookup_regex);

	def hookup_regex(self,conn):
		conn.connection.create_function(self.__REGEX_FUNC, 2, re_fn)

	def regex_match(self, field, regex):
		return field.op(self.__REGEX_FUNC)(regex);

# Db table definitions

class NamedTableMixin(object):
	@declared_attr
	def __tablename__(cls):
		return cls.__name__.lower();

class NamedIdentifierMixin(NamedTableMixin):
	@declared_attr
	def id(cls):
		return Column("id", Integer, Sequence(cls.__tablename__ + "_id_seq"), primary_key=True);

	@declared_attr
	def name(cls):
		return Column("name", String);


class Tag(NamedIdentifierMixin, Base):

	def __repr__(self):
		return "<Tag: {} {}>".format(self.id, self.name);

class Track(NamedIdentifierMixin, Base):
	total_weight = Column("total_weight", Integer)

class Sound(NamedIdentifierMixin, Base):
	pass;

# tables that serve only to join 2 other tables

class SoundTags(NamedTableMixin, Base):
	# 2 foreign keys for the Sound + Tag
	sound_id = Column(Integer, ForeignKey("sound.id"), primary_key=True)
	tag_id = Column(Integer, ForeignKey("tag.id"), primary_key=True)


class TrackTags(NamedTableMixin, Base):
	# 2 foreign keys for Track + Tag
	track_id = Column(Integer, ForeignKey("track.id"), primary_key=True)
	tag_id = Column(Integer, ForeignKey("tag.id"), primary_key=True)


class TrackSounds(NamedTableMixin, Base):
	# 2 foreign keys for Track + Sound
	track_id = Column(Integer, ForeignKey("track.id"), primary_key=True)
	sound_id = Column(Integer, ForeignKey("sound.id"), primary_key=True)

