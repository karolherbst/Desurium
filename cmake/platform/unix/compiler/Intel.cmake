add_compiler_flags(-fPIC -pipe -fvisibility=hidden -finline-functions -gcc)
add_compiler_flags(DEBUG -fno-omit-frame-pointer -g3)
add_compiler_flags(RELEASE -O2)
add_compiler_flags(CXX -fpermissive -std=c++0x)

add_linker_flags(-Bsymbolic-functions -lpthread)
if(DEBUG)
  add_linker_flags(-rdynamic)
endif()

#ignore some warnings
add_compiler_flags(-wd2928,10120)
set(ICC TRUE)
