#ifndef _MAP_LOADER_H_
#define _MAP_LOADER_H_

#include<d3d11.h>
#include <xnamath.h>
#include <fstream>
#include <vector>
#include <string>

class MapLoader
{
private:
	typedef struct {
		int onWhich;
		int whichObj;
		int whichTexture;
	}ObjectData;

    int totalObj;
    std::vector<std::string> objName;
    std::vector<std::string> textureName;
	std::vector<XMFLOAT3> Pos;
    std::vector<XMFLOAT3> Sca;
    std::vector<XMFLOAT3> Rot;
	std::vector<ObjectData> setData;

//http://stackoverflow.com/questions/347949/how-to-convert-a-stdstring-to-const-char-or-char/4152881#4152881

public:
	MapLoader(void);
	void LoadMap(std::wstring);
	void UnloadMap();
	std::string getObjName (int i)		const { return objName[i]; }
	std::string getImgName (int i)		const { return textureName[i]; }
	ObjectData getSetData (int i)		const { return setData[i]; }
	XMFLOAT3 getObjPos (int i)			const { return Pos[i]; }
	XMFLOAT3 getObjSca (int i)			const { return Sca[i]; }
	XMFLOAT3 getObjRot (int i)			const { return Rot[i]; }
	int getObjCount()					const { return totalObj; }
	int textureCount()					const { return textureName.size(); }
	int modelCount()					const { return objName.size(); }
	~MapLoader(void);
};
#endif
