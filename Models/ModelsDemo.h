/*
    Beginning DirectX 11 Game Programming
    By Allen Sherrod and Wendy Jones

    3D Models Demo - Demonstrates loading a model from an .OBJ file.
*/


#ifndef _MODELS_DEMO_H_
#define _MODELS_DEMO_H_

#include "Dx11DemoBase.h"
#include "ArcCamera.h"
#include "FirstPersonCamera.h"
#include <XInput.h>
#include "GameObject.h"
#include <string>

enum GameStates {PLAY_INTRO, START_MENU, RUN, PAUSED, INTRO_MOVIE_REPLAY, RES_MENU};

enum PauseMenuSelection {RETURN, FPS, PLAY_MOVIE, FULLSCREEN, RESOLUTION_MENU , QUIT};
#define PAUSE_MENU_ITEMS 6

void HitPlayer(float amount);

inline PauseMenuSelection operator++(PauseMenuSelection &eDOW, int)
{
   const PauseMenuSelection ePrev = eDOW;
   const int i = static_cast<int>(eDOW);
   eDOW = static_cast<PauseMenuSelection>((i + 1) % PAUSE_MENU_ITEMS);
   return ePrev;
}
inline PauseMenuSelection operator--(PauseMenuSelection &eDOW, int)
{
   const PauseMenuSelection ePrev = eDOW;
   const int i = static_cast<int>(eDOW);
   if(i > 0)
   {
		eDOW = static_cast<PauseMenuSelection>(i - 1);
   }
   else
   {
	   eDOW = static_cast<PauseMenuSelection>(PAUSE_MENU_ITEMS - 1);
   }
   return ePrev;
}


//GA
enum MainMenuSelection {LEVEL_ONE, LEVEL_TWO, LEVEL_THREE, EXIT};
#define MAIN_MENU_ITEMS 5

inline MainMenuSelection operator++(MainMenuSelection &erDOW, int)
{
   const MainMenuSelection erPrev = erDOW;
   const int i = static_cast<int>(erDOW);
   erDOW = static_cast<MainMenuSelection>((i + 1) % MAIN_MENU_ITEMS);
   return erPrev;
}
inline MainMenuSelection operator--(MainMenuSelection &erDOW, int)
{
   const MainMenuSelection erPrev = erDOW;
   const int i = static_cast<int>(erDOW);
   if(i > 0)
   {
		erDOW = static_cast<MainMenuSelection>(i - 1);
   }
   else
   {
	   erDOW = static_cast<MainMenuSelection>(MAIN_MENU_ITEMS - 1);
   }
   return erPrev;
}

//GA 2
enum ResolutionMenuSelection {BACK, OPTA, OPTB, OPTC, OPTD, OPTE};
#define RESOLUTION_MENU_ITEMS 6

inline ResolutionMenuSelection operator++(ResolutionMenuSelection &erDOW, int)
{
   const ResolutionMenuSelection erPrev = erDOW;
   const int i = static_cast<int>(erDOW);
   erDOW = static_cast<ResolutionMenuSelection>((i + 1) % RESOLUTION_MENU_ITEMS);
   return erPrev;
}
inline ResolutionMenuSelection operator--(ResolutionMenuSelection &erDOW, int)
{
   const ResolutionMenuSelection erPrev = erDOW;
   const int i = static_cast<int>(erDOW);
   if(i > 0)
   {
		erDOW = static_cast<ResolutionMenuSelection>(i - 1);
   }
   else
   {
	   erDOW = static_cast<ResolutionMenuSelection>(RESOLUTION_MENU_ITEMS - 1);
   }
   return erPrev;
}



class ModelsDemo : public Dx11DemoBase
{
    public:
        ModelsDemo( );
        virtual ~ModelsDemo( );

        bool LoadContent( );
        void UnloadContent( );

		void LoadMap(std::wstring);
		void UnloadMap();

		void Reset();

		void TurnZBufferOn();
		void TurnZBufferOff();

		void TurnOnAlphaBlending();
		void TurnOffAlphaBlending();

		void LoadOrgEnemy();
		void LoadUrbEnemy();
		void LoadFutaEnemy();

		bool DrawString( char* message, float startX, float startY );
		bool DrawGameObject( GameObject * display_object );
		bool DrawRect();
		bool RenderPassOne();
		bool RenderPassTwo();
		bool RenderPassThree();
		void UpdateWidthandHeight(int width, int height);
        void Update( float dt );
        void Render( );
		void DrawParticles();
		//ALEX OWEN 20/04/15
		void collisions();
		void DrawUI(float x, float y, float w , float h, Texture* imgtouse);
		// COSMIN / ANDREW / CHRIS //
		void loadLvlArray(const char*);

		void SetGameState(GameStates state);
		GameStates GetGameState();
		bool FullScreenState;

		float DistanceBetweenPoints(XMFLOAT3 _position1, XMFLOAT3 _position2);

    private:
        ID3D11VertexShader* textureMapVS_;
        ID3D11PixelShader* textureMapPS_;
		ID3D11VertexShader* textTextureMapVS_;
        ID3D11PixelShader* textTextureMapPS_;
		ID3D11VertexShader* normalTextureMapVS_;
        ID3D11PixelShader* normalTextureMapPS_;

		ID3D11VertexShader* colourMapVS_;
        ID3D11PixelShader* colourMapPS_;

        ID3D11InputLayout* inputLayout_;
		ID3D11InputLayout* textInputLayout_;
        ID3D11Buffer* vertexBuffer_;
		ID3D11Buffer* textVertexBuffer_;
		ID3D11Buffer* vertexBufferTerrain_;
        int totalVerts_;

        ID3D11ShaderResourceView* colorMap_;
		ID3D11ShaderResourceView* textColorMap_;
        ID3D11SamplerState* colorMapSampler_;
		ID3D11SamplerState* textColorMapSampler_;

        ID3D11Buffer* viewCB_;
        ID3D11Buffer* projCB_;
        ID3D11Buffer* worldCB_;
        ID3D11Buffer* camPosCB_;
        XMMATRIX projMatrix_;

        //ArcCamera camera_;
		FirstPersonCamera camera_;

        XINPUT_STATE controller1State_;
        XINPUT_STATE prevController1State_;

		GameStates gameState_;
		PauseMenuSelection pauseMenuSelection;
		ResolutionMenuSelection resolutionMenuSelection;
		MainMenuSelection mainMenuSelection;
		bool displayFPS;

		int MyWidth;
		int MyHeight;
		bool MenuSwitched;
		bool playerWin;
		int wintimer;

		//////////////time globals///////////////////
public:
		//double countsPerSecond_ = 0.0;
		//__int64 CounterStart_ = 0;

		//int frameCount_ = 0;
		int fps_;

		//__int64 frameTimeOld_ = 0;
		double frameTime_;
};

#endif