# set the flags for swig
set(CMAKE_SWIG_FLAGS -namespace com.neueda.cdr -dllimport CdrCsharp)
set_source_files_properties(../cdr.csharp.i PROPERTIES CPLUSPLUS ON)

# Add swig module
add_compile_options(-fno-strict-aliasing)
swig_add_module(CdrCsharp csharp ../cdr.csharp.i)

# Files to install with csharp
set(CSHARP_INSTALL_FILES
  ${CMAKE_CURRENT_BINARY_DIR}/libCdrCsharp.so)
set(CSHARP_SRC_FILES
  ${CMAKE_CURRENT_BINARY_DIR}/cdr.cs
  ${CMAKE_CURRENT_BINARY_DIR}/CdrPINVOKE.cs
  ${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_std__vectorT_CommonDataRepresentation__cdrItem_const_p_t.cs
  ${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_u_int.cs
  ${CMAKE_CURRENT_BINARY_DIR}/Cdr.cs
  ${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_double.cs
  ${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_std__vectorT_CommonDataRepresentation__cdr_t.cs
  ${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_void.cs
  ${CMAKE_CURRENT_BINARY_DIR}/cdrDateTime.cs
  ${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_p_std__vectorT_CommonDataRepresentation__cdr_t.cs
  ${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_SWIGCDATA.cs
  ${CMAKE_CURRENT_BINARY_DIR}/cdrItem.cs
  ${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_std__pairT_long_long_const_CommonDataRepresentation__cdrItem_t.cs
  ${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_time_t.cs
  ${CMAKE_CURRENT_BINARY_DIR}/cdrItemType.cs
  ${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_std__string.cs
  ${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_tm.cs)

# # Declare install target for csharp
install(FILES ${CSHARP_INSTALL_FILES}
  DESTINATION lib/csharp)
install(FILES ${CSHARP_SRC_FILES}
  DESTINATION lib/csharp/dllsrc)
