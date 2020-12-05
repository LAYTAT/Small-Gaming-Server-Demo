include_directories(./inc)
include_directories(./inc/proto)

include_directories(/usr/include/mysql)
include_directories(/usr/local/include/mysql++)
link_directories(
        /usr/lib64/mysql
)


# 设置目标可执行文件的存放位置
aux_source_directory(./src DIR_SRC)
aux_source_directory(./src/network DIR_SRC_NETWORK)
list(APPEND DIR_SRC ${DIR_SRC_NETWORK})
aux_source_directory(./src/manager DIR_SRC_MANAGERS)
list(APPEND DIR_SRC ${DIR_SRC_MANAGERS})
#aux_source_directory(../Common/Systems DIR_SRC_SYSTEMS)
#list(APPEND DIR_SRC ${DIR_SRC_SYSTEMS})
aux_source_directory(../inc/proto DIR_SRC_PROTO)
list(APPEND DIR_SRC ${DIR_SRC_PROTO})
#add_definitions("-Wall -g")



add_executable(db_server  ${DIR_SRC})


target_link_libraries(db_server
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