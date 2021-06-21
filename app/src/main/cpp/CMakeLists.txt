cmake_minimum_required(VERSION 3.10.2)

project("voicechange")

# todo 第一步：导入头文件
include_directories("inc")

#批量导入所有源文件
file(GLOB allCPP *.c *.h *.cpp)

add_library(
             native-lib
             SHARED
            ${allCPP}
)

# CMAKE_CXX_FLAGS 环境变量(库的路径)
# CMAKE_SOURCE_DIR == CMakeLists.txt的路径
# CMAKE_ANDROID_ARCH_ABI ==  当前CPU架构

# todo 第二步：导入库文件
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}")

#find_library(log-lib
#              log
#        )

# todo 第三步：链接具体的库 到libnative-lib.so
target_link_libraries(
        native-lib
        log # 自动寻找
        fmod # 具体的库 链接到libnative-lib.so里面去
        fmodL # 具体的库 链接到libnative-lib.so里面去
)