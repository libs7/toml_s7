load("@obazl_tools_cc//config:BASE.bzl",
     _BASE_COPTS    = "BASE_COPTS",
     _BASE_LINKOPTS = "BASE_LINKOPTS",
     "DSO_EXT",
     _define_module_version = "define_module_version")

define_module_version = _define_module_version

# load("@cc_config//:CONFIG.bzl",
#      _BASE_COPTS    = "BASE_COPTS",
#      # _BASE_DEFINES  = "BASE_DEFINES",
#      _BASE_LINKOPTS = "BASE_LINKOPTS")

# load("@libs7//config/cc:CONFIG.bzl",
#      "LIBS7_SRCS",
#      "LIBS7_DEPS",
#      "LIBS7_INCLUDE_PATHS")

BASE_COPTS    = _BASE_COPTS
BASE_LINKOPTS = _BASE_LINKOPTS

BASE_DEFINES = []

BASE_SRCS = []
BASE_DEPS = []
BASE_INCLUDE_PATHS = []
PROFILE       = ["PROFILE_$(COMPILATION_MODE)"]
