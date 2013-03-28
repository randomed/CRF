FILE(REMOVE_RECURSE
  "../lib/libpng.pdb"
  "../lib/libpng.so"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/png.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
