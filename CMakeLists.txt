cmake_minimum_required(VERSION 3.16)

project(rbtree-lab-main C)

set(CMAKE_C_STANDARD 11)

# rbtree.h가 src/ 안에 있으므로 include 경로 추가
include_directories(${PROJECT_SOURCE_DIR}/src)

# 🔹 라이브러리: rbtree.c
add_library(rbtree
        src/rbtree.c
)

# 🔹 실행 파일: src/driver.c (메인 함수 포함)
add_executable(rbtree_main_exec
        src/driver.c
)
target_link_libraries(rbtree_main_exec rbtree)

# 🔹 테스트 파일: test/test_rbtree.c
add_executable(rbtree_test
        test/test-rbtree.c
)
target_link_libraries(rbtree_test rbtree)

# 🔹 클린 타겟 추가 (옵션): make clean-all 로 빌드 클린
add_custom_target(clean-all
        COMMAND ${CMAKE_COMMAND} --build . --target clean
        COMMENT "Cleaning all build files"
)

target_compile_definitions(rbtree_test PRIVATE SENTINEL)