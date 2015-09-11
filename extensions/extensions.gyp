{
  'targets': [
    {
      'target_name': 'xwalk_extensions',
      'type': 'static_library',
      'dependencies': [
        '../../base/base.gyp:base',
        '../../ipc/ipc.gyp:ipc',
        '../../url/url.gyp:url_lib',
        '../../v8/tools/gyp/v8.gyp:v8',
        '../../third_party/WebKit/public/blink_headers.gyp:blink_headers',
        'extensions_resources.gyp:xwalk_extensions_resources',
      ],
      'includes': [
        '../../build/filename_rules.gypi',
      ],
      'sources': [
        'browser/xwalk_extension_data.cc',
        'browser/xwalk_extension_data.h',
        'browser/xwalk_extension_function_handler.cc',
        'browser/xwalk_extension_function_handler.h',
        'browser/xwalk_extension_process_host.cc',
        'browser/xwalk_extension_process_host.h',
        'browser/xwalk_extension_service.cc',
        'browser/xwalk_extension_service.h',
        'common/android/xwalk_extension_android.cc',
        'common/android/xwalk_extension_android.h',
        'common/xwalk_extension.cc',
        'common/xwalk_extension.h',
        'common/xwalk_extension_messages.cc',
        'common/xwalk_extension_messages.h',
        'common/xwalk_extension_server.cc',
        'common/xwalk_extension_server.h',
        'common/xwalk_extension_switches.cc',
        'common/xwalk_extension_switches.h',
        'common/xwalk_extension_vector.h',
        'common/xwalk_external_adapter.cc',
        'common/xwalk_external_adapter.h',
        'common/xwalk_external_extension.cc',
        'common/xwalk_external_extension.h',
        'common/xwalk_external_instance.cc',
        'common/xwalk_external_instance.h',
        'common/xwalk_extension_permission_types.h',
        'extension_process/xwalk_extension_process_main.cc',
        'extension_process/xwalk_extension_process_main.h',
        'extension_process/xwalk_extension_process.cc',
        'extension_process/xwalk_extension_process.h',
        'extension_process/xwalk_extension_process_main.cc',
        'extension_process/xwalk_extension_process_main.h',
        'public/XW_Extension.h',
        'public/XW_Extension_Message_2.h',
        'public/XW_Extension_Permissions.h',
        'public/XW_Extension_SyncMessage.h',
        'renderer/xwalk_extension_client.cc',
        'renderer/xwalk_extension_client.h',
        'renderer/xwalk_extension_module.cc',
        'renderer/xwalk_extension_module.h',
        'renderer/xwalk_extension_renderer_controller.cc',
        'renderer/xwalk_extension_renderer_controller.h',
        'renderer/xwalk_internal_api.js',
        'renderer/xwalk_js_module.cc',
        'renderer/xwalk_js_module.h',
        'renderer/xwalk_module_system.cc',
        'renderer/xwalk_module_system.h',
        'renderer/xwalk_v8_utils.cc',
        'renderer/xwalk_v8_utils.h',
        'renderer/xwalk_v8tools_module.cc',
        'renderer/xwalk_v8tools_module.h',
      ],
      'conditions': [
        ['OS=="win"', {
          'dependencies': [
            'xwalk_dotnet_bridge',
          ],
          'sources': [
              'common/win/xwalk_dotnet_extension.cc',
              'common/win/xwalk_dotnet_extension.h',
              'common/win/xwalk_dotnet_instance.cc',
              'common/win/xwalk_dotnet_instance.h',
            ],
        }],
      ],
    },
  ],
  'conditions': [
    ['OS=="win"', {
      # This is needed so that /RTC1 is not added to the
      # compilation command line (it's not compatible with /clr)
      'target_defaults': {
        'variables': {
          'win_debug_RuntimeChecks': '0',
        },
      },
      'targets': [
        {
          'target_name': 'xwalk_dotnet_bridge',
          'type': 'shared_library',
          'sources': [
            'common/win/xwalk_dotnet_bridge.cc',
            'common/win/xwalk_dotnet_bridge.h',
          ],
          'defines': [
            'DOTNET_BRIDGE_IMPLEMENTATION',
          ],
          'dependencies': [
            '../../base/base.gyp:base'
          ],
          'msvs_settings': {
            'VCCLCompilerTool': {
              'RuntimeTypeInfo': 'true',
              'CompileAsManaged':'true',
            }
          },
        },
      ],
    }],  # OS=="win"
  ],
}
