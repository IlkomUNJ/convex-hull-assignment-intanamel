# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/komgraf_1_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/komgraf_1_autogen.dir/ParseCache.txt"
  "komgraf_1_autogen"
  )
endif()
