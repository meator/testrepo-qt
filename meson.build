project('test-pixmap-scaling', 'cpp', default_options: ['cpp_std=c++17'])

# Documentation: https://mesonbuild.com/Qt6-module.html
qt6 = import('qt6')
qt6dep = dependency('qt6', modules: ['Core', 'Widgets'])

ui_files = [
  'src/mainwindow.ui',
]

moc_headers = [
  'include/mainwindow.hpp',
  'include/CustomWidgets.hpp',
]

ui = qt6.compile_ui(sources: ui_files)
res = qt6.compile_resources(sources: 'resources/image.qrc')
moc = qt6.compile_moc(
  headers: moc_headers,
  dependencies: qt6dep,
)

executable(
  'test-pixmap-scaling',
  'src/main.cpp',
  'src/mainwindow.cpp',
  'src/CustomWidgets.cpp',
  ui,
  res,
  moc,
  include_directories: 'include',
  extra_files: [ui_files, moc_headers],
  dependencies: qt6dep,
)
