# 自有的include
include_directories(./inc)
include_directories(./inc/Components)
include_directories(./inc/Containers)

# 公共的include
include_directories(../inc/proto)
include_directories(../inc/Configs)

include_directories(/usr/include/mysql)
include_directories(/usr/local/include/mysql++)
link_directories(
        /usr/lib64/mysql
)


# 设置目标可执行文件的存放位置
aux_source_directory(./src DIR_SRC)
aux_source_directory(./src/network DIR_SRC_NETWORK)
list(APPEND DIR_SRC ${DIR_SRC_NETWORK})
aux_source_directory(./src/Systems DIR_SRC_SYSTEMS)
list(APPEND DIR_SRC ${DIR_SRC_SYSTEMS})
aux_source_directory(./src/Managers DIR_SRC_MANAGERS)
list(APPEND DIR_SRC ${DIR_SRC_MANAGERS})


aux_source_directory(../inc/proto DIR_SRC_PROTO)
list(APPEND DIR_SRC ${DIR_SRC_PROTO})
#add_definitions("-Wall -g")

add_executable(game_server  ${DIR_SRC})


target_link_libraries(game_server
        protobuf
        pthread
        mysqlpp
        #libmysqlclient.so
        mysqlclient
        nsl
        z
        m
        redis++
        hiredis
        )

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)