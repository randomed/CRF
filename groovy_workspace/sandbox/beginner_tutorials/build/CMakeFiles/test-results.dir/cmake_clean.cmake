FILE(REMOVE_RECURSE
  "../msg_gen"
  "../srv_gen"
  "../src/beginner_tutorials/msg"
  "../src/beginner_tutorials/srv"
  "CMakeFiles/test-results"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/test-results.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
