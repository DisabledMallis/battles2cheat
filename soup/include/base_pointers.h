#ifndef SRC_BASE_POINTERS
#define SRC_BASE_POINTERS
class base_pointers
{
public:
	char pad_0000[16]; //0x0000
	class CTextureLoader* texture_loader; //0x0010
	class WinInput* win_input; //0x0018
	class game_meta* game_meta; //0x0020
	char pad_0028[8]; //0x0028
	class WinFileIO* win_fileio; //0x0030
	class CScreenManager* screen_manager; //0x0038
	char pad_0040[8]; //0x0040
	class WinSoundManager* sound_manager; //0x0048
	class WinMusicManager* music_manager; //0x0050
	char pad_0058[40]; //0x0058
	class CLegacyNetworkQueue* legacy_network_queue; //0x0080
	char pad_0088[72]; //0x0088
	class CLoc* loc; //0x00D0
}; //Size: 0x00D8
#endif // !SRC_BASE_POINTERS