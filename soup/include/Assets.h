#ifndef SRC_ASSETS
#define SRC_ASSETS

namespace Assets {
	class Asset
	{
	public:
		char pad_0000[96]; //0x0000
	}; //Size: 0x0060

	class SceneAsset : public Asset
	{
	public:
	}; //Size: 0x0060
};

#endif // !SRC_ASSETS
