#include_directories(../inc)
#include_directories(/usr/include/mysql)
#include_directories(/usr/local/include/mysql++)
#
#
#
#link_directories(
#        /usr/lib64/mysql
#)
#
#
#aux_source_directory(. DIR_SRC)
#aux_source_directory(../db DIR_SRC_DB)
#aux_source_directory(../Cache DIR_SRC_CACHE)
#aux_source_directory(../inc/proto DIR_SRC_PROTO)
#
#add_definitions("-Wall -g")
#
#add_executable(server ${DIR_SRC} ${DIR_SRC_DB} ${DIR_SRC_CACHE} ${DIR_SRC_PROTO})
#target_link_libraries(server
#        protobuf
#        pthread
#        mysqlpp
#        #libmysqlclient.so
#        mysqlclient
#        nsl
#        z
#        m
#        redis++
#        hiredis
#        )
#
#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)





# 添加头文件目录
include_directories(./inc)
include_directories(./inc/proto)
include_directories(./inc/Components)
include_directories(/usr/include/mysql)
include_directories(/usr/local/include/mysql++)

include_directories(../inc/Configs)

link_directories(/usr/lib64/mysql)

# 将src目录下的所有源文件存入变量 DIR_SRC
aux_source_directory(./src DIR_SRC_LOGIN_SERVER)

# 设置目标可执行文件的存放位置

aux_source_directory(./src/db DIR_SRC_DB)
aux_source_directory(./src/Cache DIR_SRC_CACHE)
aux_source_directory(./src/Managers DIR_SRC_MANAGERS)
aux_source_directory(./src/Systems DIR_SRC_SYSTEMS)
aux_source_directory(./src/Network DIR_SRC_Network)
aux_source_directory(./inc/proto DIR_SRC_PROTO)
add_definitions("-Wall -g")
#add_compile_options(-std=c++11)
add_executable(login_server ${DIR_SRC_LOGIN_SERVER} ${DIR_SRC_DB} ${DIR_SRC_CACHE} ${DIR_SRC_PROTO} ${DIR_SRC_MANAGERS} ${DIR_SRC_SYSTEMS} ${DIR_SRC_Network})

# 添加到可执行文件

target_link_libraries(login_server
        protobuf
        pthread
        mysqlpp
        mysqlclient
        #        libmysqlclient.so
        redis++
        hiredis
        )