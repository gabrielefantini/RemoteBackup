# CMake generated Testfile for 
# Source directory: /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-src/test/cmake_import_minver
# Build directory: /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-build/test/cmake_import_minver
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(cmake_import_minver_configure "/opt/clion-2020.3/bin/cmake/linux/bin/cmake" "-G" "Unix Makefiles" "-A" "" "-Dnlohmann_json_DIR=/home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-build" "/home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-src/test/cmake_import_minver/project")
set_tests_properties(cmake_import_minver_configure PROPERTIES  FIXTURES_SETUP "cmake_import_minver" _BACKTRACE_TRIPLES "/home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-src/test/cmake_import_minver/CMakeLists.txt;1;add_test;/home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-src/test/cmake_import_minver/CMakeLists.txt;0;")
add_test(cmake_import_minver_build "/opt/clion-2020.3/bin/cmake/linux/bin/cmake" "--build" ".")
set_tests_properties(cmake_import_minver_build PROPERTIES  FIXTURES_REQUIRED "cmake_import_minver" _BACKTRACE_TRIPLES "/home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-src/test/cmake_import_minver/CMakeLists.txt;8;add_test;/home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-src/test/cmake_import_minver/CMakeLists.txt;0;")
