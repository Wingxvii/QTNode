TEMPLATE = subdirs
CONFIG   += qwt
SUBDIRS = CuteLogger mvcp src
cache()
src.depends = CuteLogger mvcp
