message( "TINYRENDERER::docs >>> configuring build process for the docs" )

################################################################################
#                           Configuring Doxygen                                #
################################################################################

find_package( Doxygen )

if ( DOXYGEN_FOUND )
    # find all tinyrenderer-related headers for the documentation
    get_property( TINYRENDERER_PUBLIC_INCS_DIR GLOBAL PROPERTY TINYRENDERER_INCS_DIR )
    file( GLOB_RECURSE TINYRENDERER_PUBLIC_INCS_FILES "${TINYRENDERER_PUBLIC_INCS_DIR}/*.h" )
    # configure doxygen
    set( DOXYGEN_INPUT_DIR "${TINYRENDERER_PUBLIC_INCS_DIR}" )
    set( DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/doxygen" )
    set( DOXYGEN_INDEX_FILE "${DOXYGEN_OUTPUT_DIR}/html/index.html" )
    set( DOXYFILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in" )
    set( DOXYFILE_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" )
    # replace variables inside @@ with the current values
    configure_file( ${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY )
    # create the directory where to send the documentation
    file( MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR} )
    # configure doxygen external command
    add_custom_command( OUTPUT ${DOXYGEN_INDEX_FILE}
                        DEPENDS ${TINYRENDERER_PUBLIC_INCS_FILES}
                        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
                        MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
                        COMMENT "TINYRENDERER::docs >>> Generating docs (doxygen) ..." )
    add_custom_target( tinyrenderer_cpp_docs_doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE} )
    message( "TINYRENDERER::docs >>> successfully setup the build process for the docs!" )
else()
    message( "TINYRENDERER::docs >>> couldn't find Doxygen; skipping docs generation" )
endif()

################################################################################
#                           Configuring Sphinx                                 #
################################################################################

find_package( Sphinx REQUIRED )

if ( SPHINX_FOUND AND DOXYGEN_FOUND )
    # configure sphinx
    set( SPHINX_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}" )
    set( SPHINX_BUILD "${CMAKE_CURRENT_BINARY_DIR}/sphinx" )
    set( SPHINX_INDEX_FILE "${SPHINX_BUILD}/index.html" )
    # configure sphinx extenrla command
    add_custom_command( OUTPUT ${SPHINX_INDEX_FILE}
                        COMMAND 
                            ${SPHINX_EXECUTABLE} -b html
                            # Tell breathe where to find the Doxygen output
                            -Dbreathe_projects.TinyRenderer=${DOXYGEN_OUTPUT_DIR}/xml
                        ${SPHINX_SOURCE} ${SPHINX_BUILD}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                        DEPENDS
                            ${CMAKE_CURRENT_SOURCE_DIR}/index.rst
                            ${DOXYGEN_INDEX_FILE}
                        MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
                        COMMENT "TINYRENDERER::docs >>> Generating docs (sphinx) ..." )
    add_custom_target( tinyrenderer_cpp_docs_sphinx ALL DEPENDS ${SPHINX_INDEX_FILE} )

else()
    message( "TINYRENDERER::docs >>> couldn't find Sphinx; skipping docs generation" )
endif()