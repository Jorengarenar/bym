find_package(Doxygen)
if(DOXYGEN_FOUND)

  if(NOT ONLY_DOCS)
    set(DOXYGEN_QUIET YES)
  endif()

  set(DOXYGEN_EXCLUDE_PATTERNS
    */build/*
    */extern/*
    */tests/*
    )

  set(DOXYGEN_IMAGE_PATH "../res/")

  set(DOXYGEN_HTML_EXTRA_STYLESHEET
    "${CMAKE_SOURCE_DIR}/extern/doxygen-awesome-css/doxygen-awesome.css")

  set(DOXYGEN_EXTRACT_ALL     YES)
  set(DOXYGEN_EXTRACT_PRIVATE YES)
  set(DOXYGEN_EXTRACT_STATIC  YES)

  #set(DOXYGEN_DOT_UML_DETAILS       YES)
  #set(DOXYGEN_HIDE_UNDOC_RELATIONS  NO)
  set(DOXYGEN_CLASS_DIAGRAMS        YES)
  set(DOXYGEN_CLASS_GRAPH           YES)
  set(DOXYGEN_COLLABORATION_GRAPH   YES)
  set(DOXYGEN_DOT_GRAPH_MAX_NODES   100)
  set(DOXYGEN_DOT_TRANSPARENT       YES)
  set(DOXYGEN_HAVE_DOT              YES)
  set(DOXYGEN_MAX_DOT_GRAPH_DEPTH   0)
  set(DOXYGEN_TEMPLATE_RELATIONS    YES)
  set(DOXYGEN_UML_LIMIT_NUM_FIELDS  50)
  set(DOXYGEN_UML_LOOK              YES)

  set(DOXYGEN_ALPHABETICAL_INDEX    NO)
  set(DOXYGEN_ALWAYS_DETAILED_SEC   NO)
  set(DOXYGEN_REPEAT_BRIEF          NO)
  set(DOXYGEN_SOURCE_BROWSER        YES)
  set(DOXYGEN_STRIP_FROM_PATH       "${CMAKE_SOURCE_DIR}")

  set(DOXYGEN_LAYOUT_FILE "DoxygenLayout.xml")

  # set(DOXYGEN_PROJECT_LOGO "../res/logo.jpg")

  doxygen_add_docs(docs ${CMAKE_SOURCE_DIR} ALL)

else(DOXYGEN_FOUND)
  message("Doxygen needs to be installed to generate documentation")
endif(DOXYGEN_FOUND)
