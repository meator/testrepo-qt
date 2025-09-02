project('libqtandroidtoolspicker', 'cpp', default_options: ['cpp_std=c++17'])

description = 'Library providing QWidget for picking android-tools source'

#
# Qt6 compilation
#

qt6 = import('qt6')
qt6dep = dependency('qt6', modules: ['Core', 'Widgets'])

subdir('i18n')

inc = include_directories('include')

moc = qt6.compile_moc(
  headers: [
    'include/atp/atp.hpp',
  ],
  dependencies: qt6dep,
)

ui = qt6.compile_ui(
  sources: 'src/gui/main_wizard_widget.ui',
)

#
# Gather info about the target system (host machine)
#
is_glibc = meson.get_compiler('cpp').compiles('''
#if __has_include(<features.h>)
 #include <features.h>
 #ifndef __GLIBC__
  #error "Host machine is not glibc!"
 #endif
#else
 #error "Host machine is not glibc!"
#endif
  ''', name: 'Is host machine glibc?')

target_defs_cfg = configuration_data()
target_defs_cfg.set_quoted('ADBPICKER_INTERNAL_OS', host_machine.system())
target_defs_cfg.set_quoted('ADBPICKER_INTERNAL_CPU_FAM', host_machine.cpu_family())
target_defs_cfg.set10('ADBPICKER_INTERNAL_GLIBC', is_glibc)
target_defs_cfg.set10('ADBPICKER_INTERNAL_IS_LE', host_machine.endian() == 'little')

bundle_translations = get_option('bundle_translations')
target_defs_cfg.set10('ADBPICKER_INTERNAL_EXTERNAL_TRANSLATIONS', bundle_translations)
target_defs_cfg.set_quoted('ADBPICKER_INTERNAL_TRANSLATIONS_DIR', bundle_translations ? '' : translations_dir)

configure_file(
  configuration: target_defs_cfg,
  output: 'internal.hpp',
)

#
# Library compilation
#
sources = [
  'src/gui/main_wizard_widget.cpp',
  'src/gui/initialize_translator.cpp',
  moc,
  ui,
  translations,
]

extra_files = [
  # 'include/qtandroidtoolspicker/adbpicker.hpp',
  # 'src/adbpicker.ui',
]

base_args = {
  'include_directories': [inc, 'src/include'],
  'dependencies': qt6dep,
  'extra_files': extra_files,
}

static_lib = '-DADBPICKER_STATIC_LIB'

assert(meson.project_name().startswith('lib'))
libname = meson.project_name().substring(3)

if meson.is_subproject()
  lib = static_library(
    libname,
    sources,
    cpp_args: static_lib,
    kwargs: base_args,
  )
  dep = declare_dependency(
    link_with: lib,
    include_directories: inc,
    compile_args: static_lib,
  )
  meson.override_dependency(
    meson.project_name(),
    dep
  )
else
  # If Meson >=1.3.0 is available, set static_lib with cpp_static_args. If not,
  # do it the long and complicated way.
  # TODO
  if false and meson.version().version_compare('>=1.3.0')
    lib = library(
      libname,
      sources,
      cpp_static_args: static_lib,
      cpp_args: '-DADBPICKER_INTERNAL_DLL_EXPORTS',
      gnu_symbol_visibility: 'hidden',
      install: true,
      kwargs: base_args,
    )

  elif get_option('default_library') == 'both'
    lib_shared = shared_library(
      libname,
      sources,
      cpp_args: '-DADBPICKER_INTERNAL_DLL_EXPORTS',
      gnu_symbol_visibility: 'hidden',
      install: true,
      kwargs: base_args,
    )

    lib_static = static_library(
      libname,
      sources,
      cpp_args: static_lib,
      install: true,
      kwargs: base_args,
    )

    lib = lib_shared
  else
    if get_option('default_library') == 'static'
      cpp_args = [static_lib]
    else
      cpp_args = ['-DADBPICKER_INTERNAL_DLL_EXPORTS']
    endif
    lib = library(
      libname,
      sources,
      cpp_args: cpp_args,
      gnu_symbol_visibility: 'hidden',
      install: true,
      kwargs: base_args,
    )
  endif

  pkg = import('pkgconfig')
  if get_option('default_library') == 'both'
    pkg.generate(
      lib,
      filebase: meson.project_name(),
      description: description,
      # This dirty hack is taken from
      # https://code.videolan.org/videolan/libplacebo/-/blob/a68b9680e42208350f141822714c3d969369ef95/src/meson.build#L284-286
      extra_cflags: ['\nCflags.private:', static_lib]
    )
  elif get_option('default_library') == 'shared'
    pkg.generate(
      lib,
      filebase: meson.project_name(),
      description: description,
    )
  elif get_option('default_library') == 'static'
    pkg.generate(
      lib,
      filebase: meson.project_name(),
      description: description,
      extra_cflags: static_lib
    )
  else
    assert(false)
  endif

  install_headers(
    'include/atp/atp.hpp',
    'include/atp/dll.hpp',
    'include/atp/spimpl.hpp',
    subdir: 'atp',
  )
endif
