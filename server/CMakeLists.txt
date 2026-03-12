add_executable(server
    main.c
    files.c
    server.c
    response.c
    request.c
    router.c
)

add_custom_command(TARGET server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/public
    $<TARGET_FILE_DIR:server>/public
)

