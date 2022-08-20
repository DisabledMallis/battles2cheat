add_subdirectory(balances_hook)
add_subdirectory(encrypt)
add_subdirectory(event)
add_subdirectory(opengl)
add_subdirectory(packet)
add_subdirectory(redshift)
add_subdirectory(game_instance)
add_subdirectory(user_instance)
add_subdirectory(gamehud)
add_subdirectory(gamescreen)
add_subdirectory(lobbyscreen)
add_subdirectory(network_queue)
add_subdirectory(NetworkEvent)
add_subdirectory(NKModuleStorage)
add_subdirectory(module_user)
add_subdirectory(steamapi)
add_subdirectory(unlocks)

target_sources(${CMAKE_PROJECT_NAME}Lib PUBLIC
	"hook.cpp"
	"hook_manager.cpp"
)