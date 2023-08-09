# load("@libs7//config/cc:BUILD.bzl",
#      _BASE_COPTS    = "BASE_COPTS",
#      _BASE_DEFINES  = "BASE_DEFINES",
#      _BASE_LINKOPTS = "BASE_LINKOPTS")

# load("//MODULE.bzl",
#      "LIBS7_VERSION")

# BASE_COPTS          = _BASE_COPTS
# BASE_DEFINES        = _BASE_DEFINES
# BASE_LINKOPTS       = _BASE_LINKOPTS

TOML_S7_VERSION    = "0.1.0"
LIBTOML_CC_VERSION = "0.1.0"
LIBS7_VERSION      = "0.1.0"
GOPT_VERSION       = "10.0.0"
LIBLOGC_VERSION    = "1.0.0"
S7_TOOLS_VERSION   = "1.0.0"
UNITY_VERSION      = "2.5.2"
UTHASH_VERSION     = "2.3.0"

# BASE_SRCS = []
# BASE_DEPS = [
#     "@libs7//src:s7", "@libs7//config:hdrs",
#     "@liblogc//src:logc"
# ]
# BASE_INCLUDE_PATHS = [
#     "-Iexternal/libs7~{}/config".format(LIBS7_VERSION),
#     "-Iexternal/libs7~{}/src".format(LIBS7_VERSION),
#     "-Iexternal/liblogc~{}/src".format(LIBLOGC_VERSION),
# ]
