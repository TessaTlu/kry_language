cmake_minimum_required(VERSION 3.20)
project(KryaLanguage)

set(CMAKE_CXX_STANDARD 23)
set(BOOST_ROOT "/opt/homebrew/Cellar/boost")
find_package(Boost 1.76.0 COMPONENTS regex REQUIRED)

if (Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})
endif ()
add_executable(KryaLanguage main.cpp internal/krya/language/token/token.cpp internal/krya/language/token/token.h internal/krya/language/grammar/grammar.h internal/krya/language/grammar/operator/operator.h internal/krya/language/grammar/rvalue/rvalue.h internal/krya/language/grammar/lvalue/lvalue.h internal/krya/language/grammar/non_terminal/non_terminal.h internal/krya/language/grammar/lang/lang.cpp internal/krya/language/grammar/lang/lang.h internal/krya/language/grammar/data_type/data_type.cpp internal/krya/language/grammar/data_type/data_type.h internal/krya/language/grammar/var_declaration/var_declaration.h internal/krya/language/grammar/assignable/assignable.h internal/krya/language/grammar/expressions/expressions.h)
if (Boost_FOUND)
    target_link_libraries(KryaLanguage ${Boost_LIBRARIES})
endif ()
message("DONE")

