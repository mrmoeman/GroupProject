#include "MapLoader.h"
#include<DxErr.h>

MapLoader::MapLoader(void)
{
	totalObj = 0;
}

void MapLoader::LoadMap(std::wstring filename)
{
	wchar_t checkChar = NULL;
	//opens the file
	std::wifstream fileIn (filename.c_str());

	if (fileIn)//checks to see if file is open
    {
        while(fileIn)
        {           
            checkChar = fileIn.get();
 
            switch (checkChar)
            {
            case ' ': //ignores unwanted lines
                checkChar = fileIn.get();
                while(checkChar != '\n')
                    checkChar = fileIn.get();
                break;
 
            case '#': //stores wanted lines
                checkChar = fileIn.get();
                if(checkChar == ' ')    
                {
                    std::wstring on, tn;
 
                    float sx, sy, sz;
                    float rx, ry, rz;
                    float px, py, pz;
                    fileIn >> on >> tn;
					fileIn >> px >> py >> pz >> sx >> sy >> sz >> rx >> ry >> rz;

					std::string son( on.begin(), on.end() );
					bool objDup = false;
					int objToUse = 0 ;
					std::string stn( tn.begin(), tn.end() );
					bool textDup = false;
					int textToUse = 0 ;

					for(int i = 0; i < objName.size(); i++)
					{
						objDup = false;
						if(son == objName[i])
						{
							objToUse = i;
							objDup = true;
							break;
						}
					}
					if(!objDup)
					{
						objName.push_back(son);
					}
                    //objName.push_back(son);

					for(int i = 0; i < textureName.size(); i++)
					{
						textDup = false;
						if(stn == textureName[i])
						{
							textToUse = i;
							textDup = true;
							break;
						}
					}
					if(!textDup)
					{
						textureName.push_back(stn);
					}
					//textureName.push_back(stn);
					ObjectData val;

					if(!textDup && !objDup)
					{
						val.onWhich = totalObj;
						val.whichObj = objName.size() - 1;
						val.whichTexture = textureName.size() - 1;
						setData.push_back(val); 
					}

					if(textDup && !objDup)
					{
						val.onWhich = totalObj;
						val.whichObj = objName.size() - 1;
						val.whichTexture = textToUse;
						setData.push_back(val);
					}

					if(!textDup && objDup)
					{
						val.onWhich = totalObj;
						val.whichObj = objToUse;
						val.whichTexture = textureName.size() - 1;
						setData.push_back(val);
					}

					if(textDup && objDup)
					{
						val.onWhich = totalObj;
						val.whichObj = objToUse;
						val.whichTexture = textToUse;
						setData.push_back(val); 
					}

                    Sca.push_back(XMFLOAT3(sx, sy, sz));
                    Rot.push_back(XMFLOAT3(rx, ry, rz));
                    Pos.push_back(XMFLOAT3(px, py, pz));
					totalObj+=1;
                }
                break;
 
            default:                
                break;
            }
        }
    }else{DXTRACE_MSG( "Failed To Open Map File!" );} //if failed to open file show error

}

void MapLoader::UnloadMap()
{
	objName.clear();
	textureName.clear();
	Sca.clear();
	Rot.clear();
	Pos.clear();
	totalObj = 0;
}

MapLoader::~MapLoader(void)
{
}