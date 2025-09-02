#pragma once

// https://gcc.gnu.org/wiki/Visibility

// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
  #define ATPICKER_HELPER_DLL_IMPORT __declspec(dllimport)
  #define ATPICKER_HELPER_DLL_EXPORT __declspec(dllexport)
  #define ATPICKER_HELPER_DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define ATPICKER_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
    #define ATPICKER_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
    #define ATPICKER_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define ATPICKER_HELPER_DLL_IMPORT
    #define ATPICKER_HELPER_DLL_EXPORT
    #define ATPICKER_HELPER_DLL_LOCAL
  #endif
#endif

#ifndef ATPICKER_STATIC_LIB
  #ifdef ATPICKER_INTERNAL_DLL_EXPORTS
    #define ATPICKER_API ATPICKER_HELPER_DLL_EXPORT
  #else
    #define ATPICKER_API ATPICKER_HELPER_DLL_IMPORT
  #endif
  #define ATPICKER_LOCAL ATPICKER_HELPER_DLL_LOCAL
#else
  #define ATPICKER_API
  #define ATPICKER_LOCAL
#endif
