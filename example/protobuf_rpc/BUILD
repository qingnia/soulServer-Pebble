
gen_rule(
    name = 'gen_rpcMsg_message',
    srcs = [
        'net/rpcMsg.proto',
    ],
    cmd = '$BUILD_DIR/../thirdparty/protobuf/bin/protoc --cpp_out=$BUILD_DIR/ $SRCS',
    deps = [
    ],
    outs = [
        'net/rpcMsg.pb.cc',
        'net/rpcMsg.pb.h',
    ],
)

gen_rule(
    name = 'gen_rpcMsg_rpc',
    srcs = [
        'net/rpcMsg.proto',
    ],
    cmd = '$BUILD_DIR/../thirdparty/protobuf/bin/protoc --plugin=protoc-gen-rpc=$BUILD_DIR/tools/compiler/pb/protobuf_rpc --rpc_out=$BUILD_DIR/ $SRCS',
    deps = [
        '//tools/compiler/pb:protobuf_rpc',
    ],
    outs = [
        'net/rpcMsg.rpc.pb.cc',
        'net/rpcMsg.rpc.pb.h',
        'net/rpcMsg.rpc.pb.inh',
    ],
)

cc_library(
    name = 'rpcMsg',
    srcs = [
        'net/rpcMsg.pb.cc',
        'net/rpcMsg.rpc.pb.cc',
        'net/rpcMsg.cpp',
	'gameEntity/player.cpp',
	'gameEntity/card.cpp',
	'gameEntity/resCard.cpp',
	'gameEntity/issueCard.cpp',
	'gameEntity/roomCard.cpp',
	'gameEntity/action.cpp',
	'gameEntity/position.cpp',
	'gameEntity/examine.cpp',
	'utils/config.cpp',
	'utils/commonFun.cpp',
	'utils/log.cpp',
	'gameEntity/gameMap.cpp',
	'gameEntity/gameMgr.cpp',
    ],
    incs = [
        '../../thirdparty/protobuf/include/',
    ],
    deps = [
        ':gen_rpcMsg_message',
        ':gen_rpcMsg_rpc',
        '//src/framework/:pebble_framework',
    ]
)

cc_library(
    name = 'singleServer',
    srcs = [
        'net/singleServer.cpp', 
    ],
    incs = [
        '../../thirdparty/protobuf/include/',
    ],
    deps = [
        ':rpcMsg',
        '//src/server/:pebble_server',
    ],
)

cc_binary(
    name = 'client',
    srcs = [
        'net/client.cpp',
    ],
    incs = [
        '../../thirdparty/protobuf/include/',
    ],
    deps = [
        ':rpcMsg',
        '//src/client/:pebble_client',
    ],
)

cc_binary(
    name = 'server',
    srcs = [
        'main.cpp', 
    ],
    incs = [
        '../../thirdparty/protobuf/include/',
    ],
    deps = [
        ':rpcMsg',
        ':singleServer',
	'#pthread'
    ],
)
