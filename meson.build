project('showcase', 'cpp', default_options: ['cpp_std=c++17'])

qt6 = import('qt6')
qt6dep = dependency('qt6', modules: ['Core', 'Widgets'])

moc_files = qt6.compile_moc(
  headers: 'src/mainwindow.h',
  dependencies: qt6dep,
)

ui_files = qt6.compile_ui(
  sources: 'src/mainwindow.ui',
)

adbpicker = dependency('libqtandroidtoolspicker')

executable(
  'showcase',
  'src/main.cpp',
  'src/mainwindow.cpp',
  moc_files,
  ui_files,
  dependencies: [qt6dep, adbpicker],
  extra_files: ['src/mainwindow.h', 'src/mainwindow.ui'],
)
