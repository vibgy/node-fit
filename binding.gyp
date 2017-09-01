{
  "targets": [
    {
      "target_name": "binding",
      "sources": [
         "src/binding.cc",
         "src/fitparser.cc",
         "libfit/fit_protocol_validator.cpp",
         "libfit/fit.cpp",
         "libfit/fit_accumulated_field.cpp",
         "libfit/fit_accumulator.cpp",
         "libfit/fit_buffer_encode.cpp",
         "libfit/fit_buffered_record_mesg_broadcaster.cpp",
         "libfit/fit_crc.cpp",
         "libfit/fit_date_time.cpp",
         "libfit/fit_decode.cpp",
         "libfit/fit_encode.cpp",
         "libfit/fit_field.cpp",
         "libfit/fit_field_definition.cpp",
         "libfit/fit_mesg.cpp",
         "libfit/fit_mesg_broadcaster.cpp",
         "libfit/fit_mesg_definition.cpp",
         "libfit/fit_mesg_with_event_broadcaster.cpp",
         "libfit/fit_profile.cpp",
         "libfit/fit_unicode.cpp",
         "libfit/fit_developer_field.cpp",
         "libfit/fit_developer_field_definition.cpp",
         "libfit/fit_developer_field_description.cpp",
         "libfit/fit_factory.cpp",
         "libfit/fit_field_base.cpp"
      ],
      "cflags!": [ "-fno-exceptions", "-fno-rtti" ],
      "cflags_cc!": [ "-fno-exceptions", "-fno-rtti" ],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          }
        }]
      ],
      "include_dirs" : ["<!(node -e \"require('nan')\")"]
    }
  ]
}