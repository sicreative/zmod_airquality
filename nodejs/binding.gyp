{
  'targets': [
    {
      'target_name': 'airquality-native',
      'sources': [ 'src/airquality.cc','src/zmodhat.cpp','src/zmod4xxx.c','src/sensorasyncworker.cc','src/iaq_2nd_gen.h' ],
      'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")",'src/'],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      "libraries": [ '-lwiringPi','../lib_iaq_2nd_gen.a' ],
      'xcode_settings': {
        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
        'CLANG_CXX_LIBRARY': 'libc++',
        'MACOSX_DEPLOYMENT_TARGET': '10.7'
      },
      'msvs_settings': {
        'VCCLCompilerTool': { 'ExceptionHandling': 1 },
      }
    }
  ]
}
