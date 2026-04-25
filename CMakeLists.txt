cmake_minimum_required(VERSION 3.10)

PROJECT(study_framework)

set(CMAKE_CXX_COMPILER g++)
add_definitions("-Wall -std=c++17")

include_directories(${CMAKE_SOURCE_DIR})

set (framework_v_1_0 
     framework/v_1_0/EventHandler.cpp 
     framework/v_1_0/Reactor.cpp )
     
set (framework_v_1_1_lib
     framework/v_1_1/Acceptor.cpp 
     framework/v_1_1/EventHandler.cpp 
     framework/v_1_1/Reactor.cpp
     framework/v_1_1/Connector.cpp
     framework/v_1_1/ServiceHandler.cpp 
     framework/v_1_1/SockAcceptor.cpp 
     framework/v_1_1/SockConnector.cpp 
     framework/v_1_1/SockStream.cpp )
     
set (lib_framework_reactor_1_0
     framework/reactor/1_0/EventHandler.cpp 
     framework/reactor/1_0/Reactor.cpp )

set (lib_framework_task_1_0
     framework/task/1_0/Task.cpp )

     
set (lib_framework_active_object_1_0
     framework/active_object/1_0/Future.cpp
     framework/active_object/1_0/MethodRequest.cpp 
     framework/active_object/1_0/ActivationQueue.cpp)
     
set (lib_framework_acceptor_connector_1_0
     framework/acceptor_connector/1_0/Acceptor.cpp 
     framework/acceptor_connector/1_0/Connector.cpp
     framework/acceptor_connector/1_0/ServiceHandler.cpp 
     framework/acceptor_connector/1_0/SockAcceptor.cpp 
     framework/acceptor_connector/1_0/SockConnector.cpp 
     framework/acceptor_connector/1_0/SockStream.cpp )

set (lib_framework_proactor_1_0
     framework/proactor/1_0/AsynchPseudoTask.cpp
     framework/proactor/1_0/AsynchReadStream.cpp
     framework/proactor/1_0/AsynchWriteStream.cpp 
     framework/proactor/1_0/Handler.cpp
     framework/proactor/1_0/AsynchResult.cpp
     framework/proactor/1_0/Proactor.cpp 
     framework/proactor/1_0/ServiceHandler.cpp 
     framework/proactor/1_0/NotifyPipeManager.cpp )

set (example_1_client_source 
     applications/example_reactor/MainClient.cpp )
     
set (example_1_server_source 
     applications/example_reactor/MainServer.cpp
     applications/example_reactor/Acceptor.cpp
     applications/example_reactor/ServerEventHandler.cpp )
     
set (ex_acc_con_server_lib
     applications/example_acceptor_connector/MainServer.cpp
     applications/example_acceptor_connector/AcceptorImpl.cpp
     applications/example_acceptor_connector/InputHandler.cpp)

set (ex_acc_con_client_lib
     applications/example_acceptor_connector/MainClient.cpp
     applications/example_acceptor_connector/ConnectorImpl.cpp
     applications/example_acceptor_connector/OutputHandler.cpp 
     applications/example_acceptor_connector/Client.cpp)

set (ex_half_sync_half_async_client_lib 
     applications/example_half-sync_half-async/MainClient.cpp )

set (ex_half_sync_half_async_server_lib
     applications/example_half-sync_half-async/MainServer.cpp
     applications/example_half-sync_half-async/AsyncService.cpp
     applications/example_half-sync_half-async/SyncService.cpp
     applications/example_half-sync_half-async/Acceptor.cpp)
         
set (ex_active_object_client_lib 
     applications/example_active_object/MainClient.cpp )

set (ex_active_object_server_lib
     applications/example_active_object/MainServer.cpp
     applications/example_active_object/ActObjMain.cpp
     applications/example_active_object/ActObjClient.cpp
     applications/example_active_object/ActObjServant.cpp
     applications/example_active_object/ActObjAcceptor.cpp
     applications/example_active_object/ActObjMethodRequests.cpp
     applications/example_active_object/ActObjScheduler.cpp
     applications/example_active_object/ActObjServantProxy.cpp
     applications/example_active_object/ActObjServant.cpp
     applications/example_active_object/ActObjMethodCallback.cpp)

set (ex_proactor_client_lib 
     applications/example_proactor/MainClient.cpp )
     
set (ex_proactor_server_lib 
     applications/example_proactor/MainServer.cpp
     applications/example_proactor/Acceptor.cpp
     applications/example_proactor/ServerEventHandler.cpp )
     

add_executable(example_reactor_client
  ${example_1_client_source}
)

add_executable(example_reactor_server
  ${example_1_server_source}
  ${lib_framework_reactor_1_0}
)
 
add_executable(ex_acc_con_client_1
  ${ex_acc_con_client_lib}
  ${lib_framework_acceptor_connector_1_0}
  ${lib_framework_reactor_1_0}
)

add_executable(ex_acc_con_server_1
  ${ex_acc_con_server_lib}
  ${lib_framework_acceptor_connector_1_0}
  ${lib_framework_reactor_1_0}
)

add_executable(ex_half_sync_half_async_server
  ${ex_half_sync_half_async_server_lib}
  ${lib_framework_task_1_0}
  ${lib_framework_reactor_1_0}
)

add_executable(ex_half_sync_half_async_client
  ${ex_half_sync_half_async_client_lib}
)

add_executable(ex_active_object_server
  ${ex_active_object_server_lib}
  ${lib_framework_task_1_0}
  ${lib_framework_reactor_1_0}
  ${lib_framework_active_object_1_0}
)

add_executable(ex_active_object_client
  ${ex_active_object_client_lib}
)

add_executable(ex_proactor_client
  ${ex_proactor_client_lib}
)

add_executable(ex_proactor_server
  ${ex_proactor_server_lib}
  ${lib_framework_reactor_1_0}
  ${lib_framework_proactor_1_0}
)


 