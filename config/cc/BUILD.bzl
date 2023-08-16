# load("@cc_config//:CONFIG.bzl",
#      _BASE_COPTS    = "BASE_COPTS",
#      _BASE_DEFINES  = "BASE_DEFINES",
#      _BASE_LINKOPTS = "BASE_LINKOPTS")

load("@libs7//config/cc:CONFIG.bzl",
     "LIBS7_SRCS",
     "LIBS7_DEPS",
     "LIBS7_INCLUDE_PATHS")

BASE_COPTS = [] #_BASE_COPTS
BASE_LINKOPTS = [] # _BASE_LINKOPTS
BASE_DEFINES = [] # _BASE_DEFINES

BASE_SRCS = [] # LIBS7_SRCS
BASE_DEPS = [] #LIBS7_DEPS + ["@liblogc//src:logc"]
BASE_INCLUDE_PATHS = [] # LIBS7_INCLUDE_PATHS + ["-I$(@liblogc)/src"]
