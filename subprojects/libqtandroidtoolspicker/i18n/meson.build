translation_files = [
  'de.ts'
]

if get_option('translationdir') == ''
  translations_dir = get_option('prefix') / get_option('datadir') / 'bundle_translations/translations'
else
  translations_dir = get_option('prefix') / get_option('translationdir')
endif

qt6.compile_translations(
  ts_files: translation_files,
  build_by_default: true,
  install: not get_option('bundle_translations'),
  install_dir: translations_dir,
)

if get_option('bundle_translations')
  qrc = configure_file(
    command: ['generate_qrc.py', translation_files],
    output: 'translations.qrc',
    capture: true,
  )

  translations = qt6.compile_resources(sources: qrc)
else
  translations = []
endif
