project('md3-wx', 'cpp',
  version: '1.0',
  default_options: [
    'buildtype=debug',
    'cpp_std=c++17'
  ]
)

# Project options
build_examples = get_option('build_examples')

# Dependencies
wxwidgets_dep = dependency('wxwidgets', version: '>=3.2.0', required: false)
if not wxwidgets_dep.found()
  # Create a mock dependency for development without wxWidgets
  wxwidgets_dep = declare_dependency(
    include_directories: include_directories('.'),
    compile_args: ['-DMD3_WITHOUT_WXWIDGETS']
  )
  message('WARNING: wxWidgets not found, building without wxWidgets for development')
endif

# Source files
md3wx_sources = [
  'src/MD3Control.cpp',
  'src/MD3Theme.cpp',
  'src/MD3Animator.cpp',
  'src/MD3Button.cpp',
  'src/MD3Events.cpp',
  'src/MD3Layout.cpp',
  'src/MD3Checkbox.cpp',
  'src/MD3RadioButton.cpp',
  'src/MD3Switch.cpp'
]

# Create library
md3wx_lib = library('md3wx', md3wx_sources,
  dependencies: [wxwidgets_dep],
  include_directories: include_directories('include', '.'),
  install: true
)

# Install headers
headers = [
  'include/wx_md3/core/MD3Theme.h',
  'include/wx_md3/core/MD3Animator.h',
  'include/wx_md3/core/MD3Events.h',
  'include/wx_md3/core/MD3Layout.h',
  'include/wx_md3/core/MD3Control.h',
  'include/wx_md3/components/MD3Button.h',
  'include/wx_md3/components/MD3Checkbox.h',
  'include/wx_md3/components/MD3RadioButton.h',
  'include/wx_md3/components/MD3Switch.h',
]

install_headers(headers, subdir: 'md3')

# Example application
if build_examples
  button_demo = executable('button_demo', 'examples/e_md_button.cpp',
    link_with: [md3wx_lib],
    dependencies: [wxwidgets_dep],
    include_directories: include_directories('include', '.'),
    install: false
  )

  control_demo = executable('control_demo', 'examples/e_md_control.cpp',
    link_with: [md3wx_lib],
    dependencies: [wxwidgets_dep],
    include_directories: include_directories('include', '.'),
    install: false
  )

  event_demo = executable('event_demo', 'examples/e_md_event.cpp',
    link_with: [md3wx_lib],
    dependencies: [wxwidgets_dep],
    include_directories: include_directories('include', '.'),
    install: false
  )

  theme_demo = executable('theme_demo', 'examples/e_md_theme.cpp',
    link_with: [md3wx_lib],
    dependencies: [wxwidgets_dep],
    include_directories: include_directories('include', '.'),
    install: false
  )

  layout_demo = executable('layout_demo', 'examples/e_md_layout.cpp',
    link_with: [md3wx_lib],
    dependencies: [wxwidgets_dep],
    include_directories: include_directories('include', '.'),
    install: false
  )

  animator_demo = executable('animator_demo', 'examples/e_md_animator.cpp',
    link_with: [md3wx_lib],
    dependencies: [wxwidgets_dep],
    include_directories: include_directories('include', '.'),
    install: false
  )

  checkbox_demo = executable('checkbox_demo', 'examples/e_md_checkbox.cpp',
    link_with: [md3wx_lib],
    dependencies: [wxwidgets_dep],
    include_directories: include_directories('include', '.'),
    install: false
  )

  radiobutton_demo = executable('radiobutton_demo', 'examples/e_md_radiobutton.cpp',
    link_with: [md3wx_lib],
    dependencies: [wxwidgets_dep],
    include_directories: include_directories('include', '.'),
    install: false
  )

  switch_demo = executable('switch_demo', 'examples/e_md_switch.cpp',
    link_with: [md3wx_lib],
    dependencies: [wxwidgets_dep],
    include_directories: include_directories('include', '.'),
    install: false
  )

  comparison_demo = executable('comparison_demo', 'examples/e_md_comparison.cpp',
    link_with: [md3wx_lib],
    dependencies: [wxwidgets_dep],
    include_directories: include_directories('include', '.'),
    install: false
  )

  animation_demo = executable('animation_demo', 'examples/e_md_animation_demo.cpp',
    link_with: [md3wx_lib],
    dependencies: [wxwidgets_dep],
    include_directories: include_directories('include', '.'),
    install: false
  )

  animation_test = executable('animation_test', 'examples/e_md_animation_test.cpp',
    link_with: [md3wx_lib],
    dependencies: [wxwidgets_dep],
    include_directories: include_directories('include', '.'),
    install: false
  )
endif