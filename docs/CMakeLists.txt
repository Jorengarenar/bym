find_package(Doxygen)
if(DOXYGEN_FOUND)

  if(NOT ONLY_DOCS)
    set(DOXYGEN_QUIET YES)
  endif()

  set(DOXYGEN_EXCLUDE_PATTERNS
    */build/*
    */extern/*
    )

  set(DOXYGEN_IMAGE_PATH "../res/")

  set(DOXYGEN_EXTRACT_ALL YES)
  set(DOXYGEN_EXTRACT_PRIVATE YES)

  set(DOXYGEN_LAYOUT_FILE "DoxygenLayout.xml")

  # set(DOXYGEN_PROJECT_LOGO "../res/logo.jpg")

  doxygen_add_docs(docs ${CMAKE_SOURCE_DIR} ALL)

else(DOXYGEN_FOUND)
  message("Doxygen needs to be installed to generate the Doxygen documentation")
endif(DOXYGEN_FOUND)
