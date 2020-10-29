find_package(Doxygen)
if(DOXYGEN_FOUND)

  if(NOT ONLY_DOCS)
    set(DOXYGEN_QUIET YES)
  endif()

  set(DOXYGEN_EXCLUDE_PATTERNS
    */docs/*
    */extern/*
  )

  doxygen_add_docs(doc ${CMAKE_SOURCE_DIR} ALL)

else(DOXYGEN_FOUND)
  message("Doxygen needs to be installed to generate the Doxygen documentation")
endif(DOXYGEN_FOUND)
