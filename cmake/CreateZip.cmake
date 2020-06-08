function(create_zip output_file input_files working_dir)
    add_custom_command(
        COMMAND ${CMAKE_COMMAND} -E tar "cf" "${output_file}" --format=zip -- ${input_files}
        WORKING_DIRECTORY "${working_dir}"
        OUTPUT  "${output_file}"
        DEPENDS ${input_files}
        COMMENT "Zipping to ${output_file}."
    )
endfunction()
