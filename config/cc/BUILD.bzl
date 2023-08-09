load("//:MODULE.bzl",
     "LIBLOGC_VERSION", "LIBS7_VERSION")

BASE_SRCS = ["@libs7//src:libs7.h",
             "@libs7//src:s7.h"]

BASE_DEPS = [
    "@libs7//src:s7",
    "@libs7//config:hdrs",
    "@liblogc//src:logc",
]

BASE_INCLUDE_PATHS = [
    "-Iexternal/libs7~{}/src".format(LIBS7_VERSION),
    "-I$(GENDIR)/external/libs7~{}/src".format(LIBS7_VERSION),
    "-Iexternal/liblogc~{}/src".format(LIBLOGC_VERSION),
]

BASE_COPTS = [
    "-x", "c",
    "-Wall",
    "-Wextra",
] + select({
    "@platforms//os:macos": [
        "-std=c11",
        "-Werror=pedantic", # not needed with -Werror?
        "-Wpedantic", # same as -pedantic, strict ISO C and ISO C++ warnings
        "-pedantic-errors",
        # "-Wno-gnu-statement-expression",
        # "-Werror=pedantic",
        # "-Wno-gnu",
        # "-Wno-format-pedantic",
    ],
    "@platforms//os:linux": [
        "-std=gnu11",
        "-fPIC",
        # GCC:
        "-Werror", # turn all warnings into errors
        "-Wfatal-errors", # stop on first error
        # "-Wl,--no-undefined",
    ],
    "//conditions:default": ["-std=c11"],
# }) + select({
#     "@platforms//cpu:arm64": [
#         # "-target", "arm64-apple-darwin22.5.0"
#     ],
#     "//conditions:default": [],
})

BASE_LINKOPTS = select({
    "@platforms//os:linux": ["-rdynamic", "-ldl"],
    "@platforms//os:macos": [],
    "//conditions:default": []
})

BASE_DEFINES = select({
    "@platforms//os:macos": ["DSO_EXT=\\\".dylib\\\""],
    "@platforms//os:linux": [
        "DSO_EXT=\\\".so\\\"",
        ## "_XOPEN_SOURCE=500", # strdup
        "_POSIX_C_SOURCE=200809L", # strdup, strndup since glibc 2.10
        "_DEFAULT_SOURCE",    # dirent macros
        "_GNU_SOURCE"         # dlsym RTLD macros
    ],
    "//conditions:default":   ["DSO_EXT=\\\".so\\\""]
}) + select({
        "@libs7//config/s7:debug?": ["S7_DEVBUILD"],
        "//conditions:default":   []
}) + select({
    "@libs7//config/profile:dev?": ["DEVBUILD"],
    "//conditions:default": []
}) + select({
    "@libs7//config/debug:trace?": ["TRACING"],
    "//conditions:default": []
}) + select({
    "@libs7//config/clibs/link:shared?": ["-fPIC"],
    "//conditions:default": []
})
