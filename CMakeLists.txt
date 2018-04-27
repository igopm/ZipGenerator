cmake_minimum_required(VERSION 3.1)

project(ZipGenerator)

set(CMAKE_CXX_STANDARD 14)
FIND_PACKAGE(ZLIB REQUIRED)
INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR})
##INCLUDE(CheckIncludeFiles)
##INCLUDE(CheckSymbolExists)
##INCLUDE(CheckTypeSize)
##INCLUDE(CheckCSourceRuns)
##INCLUDE(CheckCSourceCompiles)

##SET(PACKAGE "libzip")
##SET(PACKAGE_NAME ${PACKAGE})

##>  SET(PACKAGE_VERSION ${VERSION})
##>  SET(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")
##>
##> -ADD_SUBDIRECTORY(lib)
##> -ADD_SUBDIRECTORY(man)
##> -ADD_SUBDIRECTORY(src)
##> -ADD_SUBDIRECTORY(regress)
##> -
##>  # Checks
##>
##>  CHECK_FUNCTION_EXISTS(fseeko HAVE_FSEEKO)
##> @@ -51,12 +48,20 @@
#>  CHECK_TYPE_SIZE("long long" LONGLONG_LIBZIP)
#>
#>  FIND_PACKAGE(ZLIB REQUIRED)
#> -INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR})
#> -CHECK_SYMBOL_EXISTS(ZEXPORT zlib.h HAVE_ZEXPORT)
#> +INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/lib ${ZLIB_INCLUDE_DIR})
#> +
#> +set(CMAKE_REQUIRED_INCLUDES ${ZLIB_INCLUDE_DIR})
#> +CHECK_C_SOURCE_RUNS("#include <zlib.h>
#> +int main() { return !(ZLIB_VERNUM > 0x1120); }" HAVE_ZEXPORT)
#>  IF(NOT HAVE_ZEXPORT)
#>    MESSAGE(FATAL_ERROR "-- ZLIB version too old, please install at least
#> v1.1.2")
#>  ENDIF(NOT HAVE_ZEXPORT)
#>
#> +ADD_SUBDIRECTORY(lib)
#> +ADD_SUBDIRECTORY(man)
#> +ADD_SUBDIRECTORY(src)
#> +ADD_SUBDIRECTORY(regress)
#> +
#>  # Targets
#>
#>  # XXX: pkgconfig file

#This is probably ok. 2 questions:
#a) why change the INCLUDE_DIRECTORIES?
#b) why do you move the ADD_SUBDIRECTORY lines?

#> diff -r 41521957b3cb lib/CMakeLists.txt
#> --- a/lib/CMakeLists.txt      Thu Dec 10 17:16:13 2009 +0100
#> +++ b/lib/CMakeLists.txt      Fri Dec 18 03:08:51 2009 +0100
#> @@ -142,8 +142,9 @@
#>
#>  ADD_LIBRARY(zip SHARED ${LIBZIP_SOURCES} ${LIBZIP_EXTRA_FILES})
#>  SET_TARGET_PROPERTIES(zip PROPERTIES VERSION 2.0 SOVERSION 2 )
#> -TARGET_LINK_LIBRARIES(zip z)
#> +TARGET_LINK_LIBRARIES(zip ${ZLIB_LIBRARY})
#>  INSTALL(TARGETS zip
#> +     RUNTIME DESTINATION bin
#>       ARCHIVE DESTINATION lib
#>       LIBRARY DESTINATION lib)
#>  #CREATE_LIBTOOL_FILE(zip lib)

#Looks ok.

#> diff -r 5c0bdb3d264b CMakeLists.txt
#> --- a/CMakeLists.txt  Fri Dec 18 03:06:57 2009 +0100
#> +++ b/CMakeLists.txt  Fri Dec 18 03:31:06 2009 +0100
#> @@ -57,6 +57,10 @@
#>    MESSAGE(FATAL_ERROR "-- ZLIB version too old, please install at least
#> v1.1.2")
#>  ENDIF(NOT HAVE_ZEXPORT)
#>
#> +IF(MSVC)
#> +ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
#> +ENDIF(MSVC)
#> +
#>  ADD_SUBDIRECTORY(lib)
#>  ADD_SUBDIRECTORY(man)
#>  ADD_SUBDIRECTORY(src)

#What's this about?

#Who defines MSVC?

#> diff -r 5c0bdb3d264b cmake-zipconf.h.in
#> --- a/cmake-zipconf.h.in      Fri Dec 18 03:06:57 2009 +0100
#> +++ b/cmake-zipconf.h.in      Fri Dec 18 03:31:06 2009 +0100
#> @@ -55,6 +55,8 @@
#>  typedef signed int zip_int32_t;
#>  #elif defined(LONG_LIBZIP) && LONG_LIBZIP == 4
#>  typedef signed long zip_int32_t;
#> +#elif defined(_MSC_VER)
#> +typedef signed long zip_int32_t;
#>  #endif
#>  #if defined(HAVE_UINT32_T_LIBZIP)
#>  typedef uint32_t zip_uint32_t;
#> @@ -62,6 +64,8 @@
#>  typedef unsigned int zip_uint32_t;
#>  #elif defined(LONG_LIBZIP) && LONG_LIBZIP == 4
#>  typedef unsigned long zip_uint32_t;
#> +#elif defined(_MSC_VER)
#> +typedef unsigned long zip_uint32_t;
#>  #endif
#>  #if defined(HAVE_INT64_T_LIBZIP)
#>  typedef int64_t zip_int64_t;
#> @@ -69,6 +73,8 @@
#>  typedef signed long zip_int64_t;
#>  #elif defined(LONGLONG_LIBZIP) && LONGLONG_LIBZIP == 8
#>  typedef signed long long zip_int64_t;
#> +#elif defined(_MSC_VER)
#> +typedef signed long long zip_int64_t;
#>  #endif
#>  #if defined(HAVE_UINT64_T_LIBZIP)
#>  typedef uint64_t zip_uint64_t;
#> @@ -76,6 +82,8 @@
#>  typedef unsigned long zip_uint64_t;
#>  #elif defined(LONGLONG_LIBZIP) && LONGLONG_LIBZIP == 8
#>  typedef unsigned long long zip_uint64_t;
#> +#elif defined(_MSC_VER)
#> +typedef unsigned long long zip_uint64_t;
#>  #endif
#>
#>  #define ZIP_INT8_MIN -0x80

#I don't like this -- why don't the HAVE_* checks work?

#> @@ -92,6 +100,6 @@
#>
#>  #define ZIP_INT64_MIN        -0x8000000000000000LL
#>  #define ZIP_INT64_MAX         0x7fffffffffffffffLL
#> -#define ZIP_UINT64_MAX        0xffffffffffffffffLLU
#> +#define ZIP_UINT64_MAX        0xffffffffffffffffULL
#>
#>  #endif /* zipconf.h */

#This is probably fine.

#> diff -r 5c0bdb3d264b lib/mkstemp.c
#> --- a/lib/mkstemp.c   Fri Dec 18 03:06:57 2009 +0100
#> +++ b/lib/mkstemp.c   Fri Dec 18 03:31:06 2009 +0100
#> @@ -45,6 +45,15 @@
#>  #define O_BINARY 0
#>  #endif
#>
#> +#ifdef _MSC_VER
#> +# ifndef pid_t
#> +typedef int  pid_t;
#> +# endif
#> +# define     _IFMT   0170000 // type of file
#> +# define             _IFDIR  0040000 // directory
#> +# define     S_ISDIR(m)      (((m)&_IFMT) == _IFDIR)
#> +#endif
#> +
#>
#>
#>  int

#This looks very ad-hoc and should probably be tested for in cmake.

#> diff -r 5c0bdb3d264b lib/zip.h
#> --- a/lib/zip.h       Fri Dec 18 03:06:57 2009 +0100
#> +++ b/lib/zip.h       Fri Dec 18 03:31:06 2009 +0100
#> @@ -37,8 +37,12 @@
#>
#>
#>  #ifndef ZIP_EXTERN
#> -#ifdef _MSC_VER
#> -#define ZIP_EXTERN __declspec(dllexport)
#> +#ifdef _WIN32
#> +# ifdef zip_EXPORTS
#> +#  define ZIP_EXTERN __declspec(dllexport)
#> +# else
#> +#  define ZIP_EXTERN __declspec(dllimport)
#> +# endif
#>  #else
#>  #define ZIP_EXTERN
#>  #endif

#What's _WIN32? What's zip_EXPORTS?

#> diff -r 5c0bdb3d264b lib/zipint.h
#> --- a/lib/zipint.h    Fri Dec 18 03:06:57 2009 +0100
#> +++ b/lib/zipint.h    Fri Dec 18 03:31:06 2009 +0100
#> @@ -36,19 +36,24 @@
#>
#>  #include <zlib.h>
#>
#> -#ifdef _MSC_VER
#> -#define ZIP_EXTERN __declspec(dllimport)
#> -#endif
#> -
#>  #include "zip.h"
#>  #include "config.h"
#>
#> +#ifdef _MSC_VER
#> +# ifndef mode_t
#> +typedef int mode_t;
#> +# endif
#> +# define strcasecmp _strcmpi        // strcasecmp doesn't exist
#> +# define snprintf _snprintf         // remove linker error
#> +# define strdup _strdup             // deprecated?!? by Microsoft
#> +#endif
#> +

#Brrr. Please check for the functions in cmake instead and make simple
#replacements instead.

#>  #ifndef HAVE_MKSTEMP
#>  int _zip_mkstemp(char *);
#>  #define mkstemp _zip_mkstemp
#>  #endif
#>
#> -#ifdef HAVE_MOVEFILEEXA
#> +#ifdef _WIN32
#>  #include <windows.h>
#>  #define _zip_rename(s, t)                                            \
#>       (!MoveFileExA((s), (t),                                         \

#There is an explicit HAVE_MOVEFILEEXA test in cmake -- doesn't it
#work?

#> diff -r 5c0bdb3d264b regress/set_comment_all.c
#> --- a/regress/set_comment_all.c       Fri Dec 18 03:06:57 2009 +0100
#> +++ b/regress/set_comment_all.c       Fri Dec 18 03:31:06 2009 +0100
#> @@ -40,6 +40,11 @@
#>
#>  #include "zip.h"
#>
#> +#ifdef _MSC_VER
#> +#define snprintf _snprintf
#> +#endif
#> +
#> +
#>  const char *prg;
#>  const char *new_archive_comment="This is the new,\r\n"
#>  "multiline archive comment.\r\n"

#Brrr.
#cmake test and replacement function instead, please.

#> diff -r 5c0bdb3d264b regress/set_comment_localonly.c
#> --- a/regress/set_comment_localonly.c Fri Dec 18 03:06:57 2009 +0100
#> +++ b/regress/set_comment_localonly.c Fri Dec 18 03:31:06 2009 +0100
#> @@ -40,6 +40,10 @@
#>
#>  #include "zip.h"
#>
#> +#ifdef _MSC_VER
#> +#define snprintf _snprintf
#> +#endif
#> +
#>  const char *prg;
#>
#>  int
#> diff -r 5c0bdb3d264b regress/set_comment_revert.c
#> --- a/regress/set_comment_revert.c    Fri Dec 18 03:06:57 2009 +0100
#> +++ b/regress/set_comment_revert.c    Fri Dec 18 03:31:06 2009 +0100
#> @@ -40,6 +40,10 @@
#>
#>  #include "zip.h"
#>
#> +#ifdef _MSC_VER
#> +#define snprintf _snprintf
#> +#endif
#> +
#>  const char *prg;
#>  const char *new_archive_comment="This is the new,\r\n"
#>  "multiline archive comment.\r\n"

#For these two: same as the one before.

#> diff -r 5c0bdb3d264b src/CMakeLists.txt
#> --- a/src/CMakeLists.txt      Fri Dec 18 03:06:57 2009 +0100
#> +++ b/src/CMakeLists.txt      Fri Dec 18 03:31:06 2009 +0100
#> @@ -1,14 +1,23 @@
#>  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../lib
#>    ${CMAKE_CURRENT_BINARY_DIR}/..)
#>
#> -ADD_EXECUTABLE(zipcmp zipcmp.c)
#> +set(zipcmp_SRCS zipcmp.c)
#> +if(MSVC)
#> +  set(zipcmp_SRCS ${zipcmp_SRCS} unistd.c)
#> +endif(MSVC)
#> +ADD_EXECUTABLE(zipcmp ${zipcmp_SRCS})
#>  TARGET_LINK_LIBRARIES(zipcmp zip)
#> -INSTALL(TARGETS zipcmp RUNTIME DESTINATION bin)
#>
#> -ADD_EXECUTABLE(zipmerge zipmerge.c)
#> +set(zipmerge_SRCS zipmerge.c)
#> +if(MSVC)
#> +  set(zipmerge_SRCS ${zipmerge_SRCS} unistd.c)
#> +endif(MSVC)
#> +ADD_EXECUTABLE(zipmerge ${zipmerge_SRCS})
#>  TARGET_LINK_LIBRARIES(zipmerge zip)
#> -INSTALL(TARGETS zipmerge RUNTIME DESTINATION bin)
#>
#> -ADD_EXECUTABLE(ziptorrent ziptorrent.c)
#> +set(ziptorrent_SRCS ziptorrent.c)
#> +if(MSVC)
#> +  set(ziptorrent_SRCS ${ziptorrent_SRCS} unistd.c)
#> +endif(MSVC)
#> +ADD_EXECUTABLE(ziptorrent ${ziptorrent_SRCS})
#>  TARGET_LINK_LIBRARIES(ziptorrent zip)
#> -INSTALL(TARGETS ziptorrent RUNTIME DESTINATION bin)

#This shouldn't be conditional on MSVC, but on the existence of
#getopt().

#> diff -r 5c0bdb3d264b src/unistd.c
#> --- /dev/null Thu Jan 01 00:00:00 1970 +0000
#> +++ b/src/unistd.c    Fri Dec 18 03:31:06 2009 +0100
#> @@ -0,0 +1,96 @@
#> +#ifdef _MSC_VER
#> +#include <windows.h>

#Ewww. How hard would it to get e.g.
#http://cvsweb.netbsd.org/bsdweb.cgi/src/lib/libc/stdlib/getopt.c?only_with_tag=MAIN
#to compile on Windows?

#> diff -r 5c0bdb3d264b src/zipcmp.c
#> --- a/src/zipcmp.c    Fri Dec 18 03:06:57 2009 +0100
#> +++ b/src/zipcmp.c    Fri Dec 18 03:31:06 2009 +0100
#> @@ -37,12 +37,20 @@
#>  #include <stdio.h>
#>  #include <stdlib.h>
#>  #include <string.h>
#> -#include <unistd.h>
#>  #include <zlib.h>
#>
#>  #include "config.h"
#>  #include "zip.h"
#>
#> +#ifdef HAVE_UNISTD_H
#> +#include <unistd.h>
#> +#endif
#> +

#This one's fine.

#> +#ifdef _MSC_VER
#> +extern int optind;
#> +#define strcasecmp _strcmpi
#> +#endif
#> +
#>  struct entry {
#>      char *name;
#>      unsigned int size;

#This not, see above.

#> diff -r 5c0bdb3d264b src/zipmerge.c
#> --- a/src/zipmerge.c  Fri Dec 18 03:06:57 2009 +0100
#> +++ b/src/zipmerge.c  Fri Dec 18 03:31:06 2009 +0100
#> @@ -45,6 +45,11 @@
#>  #include <unistd.h>
#>  #endif
#>
#> +#ifdef _MSC_VER
#> +extern int optind;
#> +#define strcasecmp _strcmpi
#> +#endif
#> +
#>  #include "zip.h"
#>
#>

#Same as previous.

#> diff -r 5c0bdb3d264b src/ziptorrent.c
#> --- a/src/ziptorrent.c        Fri Dec 18 03:06:57 2009 +0100
#> +++ b/src/ziptorrent.c        Fri Dec 18 03:31:06 2009 +0100
#> @@ -37,12 +37,20 @@
#>  #include <stdio.h>
#>  #include <stdlib.h>
#>  #include <string.h>
#> -#include <unistd.h>
#>  #include <zlib.h>
#>
#>  #include "config.h"
#>  #include "zip.h"
#>
#> +#ifdef HAVE_UNISTD_H
#> +#include <unistd.h>
#> +#endif
#> +
#> +#ifdef _MSC_VER
#> +extern int optind;
#> +#define strcasecmp _strcmpi
#> +#endif
#> +
#>
#>
#>  #define FLAG_DRYRUN  1

#Same as previous.

#> diff -r 4be4b5f147aa lib/mkstemp.c
#> --- a/lib/mkstemp.c   Fri Dec 18 03:14:27 2009 +0100
#> +++ b/lib/mkstemp.c   Fri Dec 18 03:27:16 2009 +0100
#> @@ -54,6 +54,11 @@
#>  # define     S_ISDIR(m)      (((m)&_IFMT) == _IFDIR)
#>  #endif
#>
#> +#ifdef _WIN32
#> +# define PATH_SEPARATOR '\\'
#> +#else
#> +# define PATH_SEPARATOR '/'
#> +#endif
#>

#All these are just wrong, sorry.
#The zip spec explicitly says that the path separator inside zip files
#is '/'.

#(rest elided)

#> diff -r 95d0bdc53374 CMakeLists.txt
#> --- a/CMakeLists.txt  Fri Dec 18 03:32:52 2009 +0100
#> +++ b/CMakeLists.txt  Fri Dec 18 03:47:59 2009 +0100
#> @@ -123,4 +123,8 @@
#>  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/cmake-zipconf.h.in
#>    ${CMAKE_CURRENT_BINARY_DIR}/zipconf.h)
#>
#> -INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/zipconf.h DESTINATION
#> lib/libzip/include)
#> +IF(NOT WIN32)
#> +    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/zipconf.h DESTINATION
#> lib/libzip/include)
#> +ELSE(NOT WIN32)
#> +    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/zipconf.h DESTINATION include)
#> +ENDIF(NOT WIN32)

#Why this one?

#> diff -r 701fbf217819 lib/mkstemp.c
#> --- a/lib/mkstemp.c   Fri Dec 18 03:47:21 2009 +0100
#> +++ b/lib/mkstemp.c   Fri Dec 18 03:54:05 2009 +0100
#> @@ -128,7 +128,11 @@
#>       }
#>
#>       for (;;) {
#> -             if ((fd=open(path, O_CREAT|O_EXCL|O_RDWR|O_BINARY, 0600)) >= 0)
#> +#ifdef _WIN32
#> +        if ((fd=_open(path, O_CREAT|O_EXCL|O_RDWR|O_BINARY)) >= 0)
#> +#else
#> +        if ((fd=open(path, O_CREAT|O_EXCL|O_RDWR|O_BINARY, 0600)) >= 0)
#> +#endif
#>                       return (fd);
#>               if (errno != EEXIST)
#>                       return (0);


#find_package(PkgConfig)
#pkg_check_modules(PC_LIBZIP QUIET libzip)

#find_path(LIBZIP_INCLUDE_DIR_ZIP
#    NAMES zip.h
#    HINTS ${PC_LIBZIP_INCLUDE_DIRS})

#find_path(LIBZIP_INCLUDE_DIR_ZIPCONF
#    NAMES zipconf.h
#    HINTS ${PC_LIBZIP_INCLUDE_DIRS})

#find_library(LIBZIP_LIBRARY
#    NAMES zip)
#message("PC_LIBZIP = ${PC_LIBZIP_LIBRARIES}")
#message("LIBZIP_LIBRARY = ${LIBZIP_LIBRARY}")
#message("PC_LIBZIP = ${PC_LIBZIP_LIBRARIES}")
#message("PC_LIBZIP = ${PC_LIBZIP_LIBRARIES}")
#find_library(LIBZIP_LIBRARY
#    NAMES zip)
#message("PC_LIBZIzipP = zip")
##include(FindPackageHandleStandardArgs)
##FIND_PACKAGE_HANDLE_STANDARD_ARGS(
##    LIBZIP DEFAULT_MSG
##    LIBZIP_LIBRARY LIBZIP_INCLUDE_DIR_ZIP LIBZIP_INCLUDE_DIR_ZIPCONF)

##set(LIBZIP_VERSION 0)

##if (LIBZIP_INCLUDE_DIR_ZIPCONF)
##  FILE(READ "${LIBZIP_INCLUDE_DIR_ZIPCONF}/zipconf.h" _LIBZIP_VERSION_CONTENTS)
##  if (_LIBZIP_VERSION_CONTENTS)
##    STRING(REGEX REPLACE ".*#define LIBZIP_VERSION \"([0-9.]+)\".*" "\\1" LIBZIP_VERSION "${_LIBZIP_VERSION_CONTENTS}")
##  endif ()
##endif ()

#set(LIBZIP_VERSION ${LIBZIP_VERSION} CACHE STRING "Version number of libzip")

## The MyZipHelper library
#add_subdirectory(MyZipHelper)
#include_directories(MyZipHelper)

add_subdirectory(Example_1)
