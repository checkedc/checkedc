# CMakeList.txt used by the Checked C version of clang/LLVM.
# This copies the Checked C header files to the system header
# directory for clang so that they can easily be found/used.
#
# Checked C does not require runtime system changes, and it would
# be unnecesarily heavyweight to require people to pick up a new
# library just to get the Checked C include wrpaper files.
# So we place the header files with the compiler.

set(files
  errno_checked.h
  fenv_checked.h
  inttypes_checked.h
  math_checked.h
  signal_checked.h
  stdchecked.h
  stdio_checked.h
  stdlib_checked.h
  string_checked.h
  threads_checked.h
  time_checked.h
  unistd_checked.h
  _builtin_stdio_checked.h
  _builtin_string_checked.h
  _builtin_common.h
  checkedc_extensions.h
  )

# Hack - compute the CLANG version from the LLVM version.  The
# CLANG_VERSION variable is not available at this point during the build.

string(REGEX MATCH "[0-9]+\\.[0-9]+(\\.[0-9]+)?" CHECKEDC_CLANG_VERSION
  ${PACKAGE_VERSION})

set(output_dir ${LLVM_LIBRARY_OUTPUT_INTDIR}/clang/${CHECKEDC_CLANG_VERSION}/include)

set(out_files)
foreach( f ${files} )
  set( src ${CMAKE_CURRENT_SOURCE_DIR}/${f} )
  set( dst ${output_dir}/${f} )
  add_custom_command(OUTPUT ${dst}
    DEPENDS ${src}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${src} ${dst}
    COMMENT "Copying Checked C's ${f}...")
  list(APPEND out_files ${dst})
endforeach( f )

add_custom_target(checkedc-headers ALL DEPENDS ${out_files})
set_target_properties(checkedc-headers PROPERTIES FOLDER "Misc")

install(
  FILES ${files}
  COMPONENT checkedc-headers
  PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
  DESTINATION lib${LLVM_LIBDIR_SUFFIX}/clang/${CHECKEDC_CLANG_VERSION}/include)

if (NOT CMAKE_CONFIGURATION_TYPES) # don't add this for IDE's.
  add_custom_target(install-checkedc-headers
    DEPENDS checkedc-headers
    COMMAND "${CMAKE_COMMAND}"
            -DCMAKE_INSTALL_COMPONENT=checkedc-headers
            -P "${CMAKE_BINARY_DIR}/cmake_install.cmake")
endif()
