# Generates a .cpp that embeds a shader file's source as a std::string.
# Invoked as a build step (see CMakeLists.txt) so the generated file is
# always rebuilt from the current .frag/.vert contents - never edited by hand.
#
# Usage: cmake -DIN_FILE=<path> -DVAR_NAME=<identifier> -DOUT_FILE=<path> -P EmbedShader.cmake

if(NOT IN_FILE OR NOT VAR_NAME OR NOT OUT_FILE)
    message(FATAL_ERROR "EmbedShader.cmake requires IN_FILE, VAR_NAME, and OUT_FILE")
endif()

file(READ "${IN_FILE}" SHADER_SOURCE)

set(CONTENT "// Generated from ${IN_FILE} by EmbedShader.cmake - do not edit by hand.\n")
string(APPEND CONTENT "#include <string>\n\n")
string(APPEND CONTENT "std::string ${VAR_NAME} = R\"GLSL(${SHADER_SOURCE})GLSL\";\n")

file(WRITE "${OUT_FILE}" "${CONTENT}")
