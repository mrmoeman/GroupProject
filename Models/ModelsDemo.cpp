/*
    Beginning DirectX 11 Game Programming
    By Allen Sherrod and Wendy Jones

    3D Models Demo - Demonstrates loading a model from an .OBJ file.
*/

//////////////
// INCLUDES //
//////////////
#include "ModelsDemo.h"
#include <xnamath.h>
#include <stdio.h>
#include "objLoader.h"
#include "Bullet.h"
#include "MapLoader.h"
#include "AnimatedGameObject.h"
#include "Animation.h"
#include "ParticleHandler.h"
#include "SmartGameObject.h"
#include "GameAI.h"
#include "GameGrid.h"
#include "GraphicsManager.h"
#include "MyMath.h"
#include "SoundLoader.h"

//////////////////////
// GLOBAL VARIABLES //
//////////////////////
const int MAX_MAP_OBJECTS = 115;
const int MAX_ENEMY_OBJECTS = 6;
MapLoader* Map = NULL;
GameGrid grid;
////////////////////////////////////////////////////////////////// Graphics Manager
GraphicsManager * graphicsManager = NULL;
////////////////////////////////////////////////////////////////// Player
float playerHP = 100.0f;
float playerCooldown = .5f;
float playerAttackTimer = 1.0f;
float playerArmZ = 75.0f;
bool gunorknife = true;
////////////////////////////////////////////////////////////////// SHADERS
Shader* shader2D = NULL;
Shader* shaderVTN = NULL;
Shader* shaderNMAP = NULL;
////////////////////////////////////////////////////////////////// TEXTURES
Texture* objects_texture[MAX_MAP_OBJECTS] = {NULL};
Texture* objects_Ntexture[MAX_MAP_OBJECTS] = {NULL};
Texture* skybox_texture = NULL;
Texture* bullet_texture = NULL;
Texture* text_texture = NULL;
Texture* small_enemy_texture = NULL;
Texture* small_enemy_Ntexture = NULL;
Texture* knife_texture = NULL;
Texture* gun_texture = NULL;
Texture* hp_texture = NULL;
////////////////////////////////////////////////////////////////// MODELS
Model* objects_model[MAX_MAP_OBJECTS] = {NULL};
Model* skybox_model = NULL;
Model* bullet_model = NULL;
Model* text_model = NULL;
Model* small_enemy_model = NULL;
Model* knife_model = NULL;
Model* gun_model = NULL;
////////////////////////////////////////////////////////////////// DISPLAY OBJECTS
DisplayObject* objects_display_object[MAX_MAP_OBJECTS] = {NULL};
DisplayObject* skybox_display_object = NULL;
DisplayObject* bullet_display_object = NULL;
DisplayObject* text_display_object = NULL;
DisplayObject* small_enemy_display_object = NULL;
DisplayObject* knife_display_object = NULL;
DisplayObject* gun_display_object = NULL;
////////////////////////////////////////////////////////////////// GAME OBJECTS
GameObject* objects_game_object[MAX_MAP_OBJECTS] = {NULL};
GameObject* skybox_game_object = NULL;
GameObject* bullet_game_object = NULL;
//MovingGameObject* small_enemy_game_object[MAX_ENEMY_OBJECTS] = {NULL};
//CAT
SmartGameObject* small_enemy_game_object[MAX_ENEMY_OBJECTS] = {NULL};
////////////////////////////////////////////////////////////////// ANIMATION OBJECTS
Texture* small_enemy_run_texture = NULL;
Model* small_enemy_run_models[10];
DisplayObject* small_enemy_run_display_objects[10];
//Animation* small_enemy_run_ani = NULL;
// ALEX OWEN 20/04/15 - Instantiate as an Animated GameObject not Animation
AnimatedGameObject* small_enemy_run_ani = NULL;

////////////////////////////////// NATH SOUND

///////////////////////////////////////////////////////////////// SOUND

SoundLoader * soundLoader = NULL;



////////////////////////CAT ///////////////////////////////////////////////////////////////
//int numberOfSmallEnemies = 2;

///////////////////////////////////////////////////////// BULLET TESTING ////////////////////////
/////////////////// Alex Owen - 30/03/15 bullet testing variables ////////////////////////////
const int numBullets = 10;
Bullet bullets[numBullets];
bool shooting = false;
int shotDelay = 0;
int shotDelayMax = 400;
float smallestManhattan = 999999.0f;
struct TextVertexPos
{
    XMFLOAT3 pos;
    XMFLOAT2 tex0;
};
/////////////////////////
/////////////////////////global func
void HitPlayer(float amount)
{
	playerHP -= amount;
	if(playerHP < 0.0f) { 0.0f; }
}
/////////////////////////

ModelsDemo::ModelsDemo( ) : textureMapVS_( 0 ), textureMapPS_( 0 ),textTextureMapVS_( 0 ), textTextureMapPS_( 0 ),
							inputLayout_( 0 ), textInputLayout_(0),textVertexBuffer_(0),textColorMapSampler_( 0 ),
                            vertexBuffer_( 0 ), colorMap_( 0 ), textColorMap_(0), colorMapSampler_( 0 ),
                            viewCB_( 0 ), projCB_( 0 ), worldCB_( 0 ), camPosCB_( 0 ), totalVerts_( 0 ),
							gameState_(PLAY_INTRO), pauseMenuSelection(RETURN),mainMenuSelection(LEVEL_ONE), displayFPS(false)
{
    ZeroMemory( &controller1State_, sizeof( XINPUT_STATE ) );
    ZeroMemory( &prevController1State_, sizeof( XINPUT_STATE ) );
	ZeroMemory(&mouseCurrState, sizeof(DIMOUSESTATE));
	ZeroMemory(&mousePrevState, sizeof(DIMOUSESTATE));

	m_alphaEnableBlendingState = 0;
	m_alphaDisableBlendingState = 0;
	m_depthStencilState = 0;
	m_depthDisabledStencilState = 0;
	frameTime_=0;
	fps_=0;

	MyWidth = 800;
	MyHeight = 600;


	FullScreenState = false;
	MenuSwitched = false;
	playerWin = false;
	wintimer = 0;
}

ModelsDemo::~ModelsDemo( )
{

}

void ModelsDemo::loadLvlArray(const char* path)
{
	//40 by 40
	FILE * file;
	int c;
	int n = 0;

	int a = 0;
	int b = 0;

	//int testArray[35][35];

	//wchar_t checkChar = NULL;

	//checkChar = fileIn.get();

	file = fopen(path,"r");

	if (file==NULL) 
	{
		perror ("Error opening file");
	}
	else
	{
		do 
		{
			c = fgetc (file);
			if (c != ' ') 
			{
				switch(c)
				{
					case 48:
						c = 0;

						break;
					case 49:
						c = 1;

						break;
					case 50:
						c = 2;
						break;
					default:
						
						break;
				}

				levelArray[a][b] = c;

				a++;

				if (a > widthIndex - 1)
				{
					a = 0;
					b++;
				}
			}		
		} 
		while (c != EOF);
			fclose (file);
	}
}

void ModelsDemo::LoadOrgEnemy() // normals
{

	// ZS-->23/03/2015 -> Small Enemy Run Animation
	// first load the texture and objs needed to create the display objects
	// the display objects act as the frames




	small_enemy_run_texture = new Texture();
	small_enemy_run_texture->Init( d3dDevice_, "Obj/Ani/SmallEnemyRunTexture.png");
	small_enemy_Ntexture = new Texture();
	small_enemy_Ntexture->Init( d3dDevice_, "Obj/Ani/SmallEnemyRunTexture_N.png");

	small_enemy_run_models[0] = new ModelNMAP(); 
	small_enemy_run_models[0]->Init(d3dDevice_, "Obj/Ani/SmallEnemy_Run_01.obj");
	small_enemy_run_display_objects[0] = new DisplayObject();
	small_enemy_run_display_objects[0]->Init( small_enemy_run_models[0], small_enemy_run_texture, shaderNMAP, small_enemy_Ntexture );

	small_enemy_run_models[1] = new ModelNMAP(); 
	small_enemy_run_models[1]->Init(d3dDevice_, "Obj/Ani/SmallEnemy_Run_02.obj");
	small_enemy_run_display_objects[1] = new DisplayObject();
	small_enemy_run_display_objects[1]->Init( small_enemy_run_models[1], small_enemy_run_texture, shaderNMAP, small_enemy_Ntexture );

	small_enemy_run_models[2] = new ModelNMAP(); 
	small_enemy_run_models[2]->Init(d3dDevice_, "Obj/Ani/SmallEnemy_Run_03.obj");
	small_enemy_run_display_objects[2] = new DisplayObject();
	small_enemy_run_display_objects[2]->Init( small_enemy_run_models[2], small_enemy_run_texture, shaderNMAP, small_enemy_Ntexture );

	small_enemy_run_models[3] = new ModelNMAP(); 
	small_enemy_run_models[3]->Init(d3dDevice_, "Obj/Ani/SmallEnemy_Run_04.obj");
	small_enemy_run_display_objects[3] = new DisplayObject();
	small_enemy_run_display_objects[3]->Init( small_enemy_run_models[3], small_enemy_run_texture, shaderNMAP, small_enemy_Ntexture );

	small_enemy_run_models[4] = new ModelNMAP(); 
	small_enemy_run_models[4]->Init(d3dDevice_, "Obj/Ani/SmallEnemy_Run_05.obj");
	small_enemy_run_display_objects[4] = new DisplayObject();
	small_enemy_run_display_objects[4]->Init( small_enemy_run_models[4], small_enemy_run_texture, shaderNMAP, small_enemy_Ntexture );
	
	small_enemy_run_models[5] = new ModelNMAP(); 
	small_enemy_run_models[5]->Init(d3dDevice_, "Obj/Ani/SmallEnemy_Run_06.obj");
	small_enemy_run_display_objects[5] = new DisplayObject();
	small_enemy_run_display_objects[5]->Init( small_enemy_run_models[5], small_enemy_run_texture, shaderNMAP, small_enemy_Ntexture );

	small_enemy_run_models[6] = new ModelNMAP(); 
	small_enemy_run_models[6]->Init(d3dDevice_, "Obj/Ani/SmallEnemy_Run_07.obj");
	small_enemy_run_display_objects[6] = new DisplayObject();
	small_enemy_run_display_objects[6]->Init( small_enemy_run_models[6], small_enemy_run_texture, shaderNMAP, small_enemy_Ntexture );

	small_enemy_run_models[7] = new ModelNMAP(); 
	small_enemy_run_models[7]->Init(d3dDevice_, "Obj/Ani/SmallEnemy_Run_08.obj");
	small_enemy_run_display_objects[7] = new DisplayObject();
	small_enemy_run_display_objects[7]->Init( small_enemy_run_models[7], small_enemy_run_texture, shaderNMAP, small_enemy_Ntexture );

	small_enemy_run_models[8] = new ModelNMAP(); 
	small_enemy_run_models[8]->Init(d3dDevice_, "Obj/Ani/SmallEnemy_Run_09.obj");
	small_enemy_run_display_objects[8] = new DisplayObject();
	small_enemy_run_display_objects[8]->Init( small_enemy_run_models[8], small_enemy_run_texture, shaderNMAP, small_enemy_Ntexture );

	small_enemy_run_models[9] = new ModelNMAP(); 
	small_enemy_run_models[9]->Init(d3dDevice_, "Obj/Ani/SmallEnemy_Run_10.obj");
	small_enemy_run_display_objects[9] = new DisplayObject();
	small_enemy_run_display_objects[9]->Init( small_enemy_run_models[9], small_enemy_run_texture, shaderNMAP, small_enemy_Ntexture );



	// ALEX OWEN 16/04/15
	Animation* animator = new Animation();
	animator->SetFramesPerSec(10);
	for ( int i = 0; i<10; ++i )
	{
		animator->AddFrame( small_enemy_run_display_objects[i] );
	}
	//Add parameters for construstor
	small_enemy_run_ani = new AnimatedGameObject(XMFLOAT3(100.0f, 0.0f, 100.f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f),animator);


	// Chris, Andrew, Tyanah // POSITIONS
	XMFLOAT3 pos[] = { XMFLOAT3(0 + ARRAY_WIDTH/2*100, 0, 0 + ARRAY_HEIGHT/2*100), XMFLOAT3(300 + ARRAY_WIDTH/2*100, 0, 300 + ARRAY_HEIGHT/2*100)
						, XMFLOAT3(500 + ARRAY_WIDTH/2*100, 0, 500 + ARRAY_HEIGHT/2*100), XMFLOAT3(100 + ARRAY_WIDTH/2*100, 0, 200 + ARRAY_HEIGHT/2*100)
						, XMFLOAT3(600 + ARRAY_WIDTH/2*100, 0, 500 + ARRAY_HEIGHT/2*100), XMFLOAT3(700 + ARRAY_WIDTH/2*100, 0, 700 + ARRAY_HEIGHT/2*100)};

	for(int i = 0; i < MAX_ENEMY_OBJECTS; i++) //melee
	{
		small_enemy_game_object[i] = new SmartGameObject();
		small_enemy_game_object[i]->setPosition( pos[i] );
		small_enemy_game_object[i]->setRotation( XMFLOAT3(0, 0, 0) );
		small_enemy_game_object[i]->setScale( XMFLOAT3(1, 1, 1) );
		small_enemy_game_object[i]->animator=animator;
		small_enemy_game_object[i]->SetDisplayObject( small_enemy_game_object[i]->animator->GetCurrentFrame() );
		small_enemy_game_object[i]->SetHP(30);
	}

	// Chris, Andrew, Tyanah // RANGED
	//small_enemy_game_object[5]->setScale(XMFLOAT3(2, 2, 2));
	small_enemy_game_object[5]->brain->AIState = small_enemy_game_object[5]->brain->RANGED;


}
void ModelsDemo::LoadUrbEnemy() //normals
{
	small_enemy_run_texture = new Texture();
	small_enemy_run_texture->Init( d3dDevice_, "Obj/Ani/MI_UD_SwarmTex.png");
	small_enemy_Ntexture = new Texture();
	small_enemy_Ntexture->Init( d3dDevice_, "Obj/Ani/MI_UD_SwarmNormal.png");

	small_enemy_run_models[0] = new ModelNMAP(); 
	small_enemy_run_models[0]->Init(d3dDevice_, "Obj/Ani/UDMI_Swarm_Move1.obj");
	small_enemy_run_display_objects[0] = new DisplayObject();
	small_enemy_run_display_objects[0]->Init( small_enemy_run_models[0], small_enemy_run_texture, shaderNMAP, small_enemy_Ntexture );

	small_enemy_run_models[1] = new ModelNMAP(); 
	small_enemy_run_models[1]->Init(d3dDevice_, "Obj/Ani/UDMI_Swarm_Move2.obj");
	small_enemy_run_display_objects[1] = new DisplayObject();
	small_enemy_run_display_objects[1]->Init( small_enemy_run_models[1], small_enemy_run_texture, shaderNMAP, small_enemy_Ntexture );

	small_enemy_run_models[2] = new ModelNMAP(); 
	small_enemy_run_models[2]->Init(d3dDevice_, "Obj/Ani/UDMI_Swarm_Move3.obj");
	small_enemy_run_display_objects[2] = new DisplayObject();
	small_enemy_run_display_objects[2]->Init( small_enemy_run_models[2], small_enemy_run_texture, shaderNMAP, small_enemy_Ntexture );

	small_enemy_run_models[3] = new ModelNMAP(); 
	small_enemy_run_models[3]->Init(d3dDevice_, "Obj/Ani/UDMI_Swarm_Move4.obj");
	small_enemy_run_display_objects[3] = new DisplayObject();
	small_enemy_run_display_objects[3]->Init( small_enemy_run_models[3], small_enemy_run_texture, shaderNMAP, small_enemy_Ntexture );

	small_enemy_run_models[4] = new ModelNMAP(); 
	small_enemy_run_models[4]->Init(d3dDevice_, "Obj/Ani/UDMI_Swarm_Move5.obj");
	small_enemy_run_display_objects[4] = new DisplayObject();
	small_enemy_run_display_objects[4]->Init( small_enemy_run_models[4], small_enemy_run_texture, shaderNMAP, small_enemy_Ntexture );
	
	small_enemy_run_models[5] = new ModelNMAP(); 
	small_enemy_run_models[5]->Init(d3dDevice_, "Obj/Ani/UDMI_Swarm_Move6.obj");
	small_enemy_run_display_objects[5] = new DisplayObject();
	small_enemy_run_display_objects[5]->Init( small_enemy_run_models[5], small_enemy_run_texture, shaderNMAP, small_enemy_Ntexture );

	small_enemy_run_models[6] = new ModelNMAP(); 
	small_enemy_run_models[6]->Init(d3dDevice_, "Obj/Ani/UDMI_Swarm_Move7.obj");
	small_enemy_run_display_objects[6] = new DisplayObject();
	small_enemy_run_display_objects[6]->Init( small_enemy_run_models[6], small_enemy_run_texture, shaderNMAP, small_enemy_Ntexture );

	small_enemy_run_models[7] = new ModelNMAP(); 
	small_enemy_run_models[7]->Init(d3dDevice_, "Obj/Ani/UDMI_Swarm_Move8.obj");
	small_enemy_run_display_objects[7] = new DisplayObject();
	small_enemy_run_display_objects[7]->Init( small_enemy_run_models[7], small_enemy_run_texture, shaderNMAP, small_enemy_Ntexture );

	small_enemy_run_models[8] = new ModelNMAP(); 
	small_enemy_run_models[8]->Init(d3dDevice_, "Obj/Ani/UDMI_Swarm_Move9.obj");
	small_enemy_run_display_objects[8] = new DisplayObject();
	small_enemy_run_display_objects[8]->Init( small_enemy_run_models[8], small_enemy_run_texture, shaderNMAP, small_enemy_Ntexture );

	small_enemy_run_models[9] = new ModelNMAP(); 
	small_enemy_run_models[9]->Init(d3dDevice_, "Obj/Ani/UDMI_Swarm_Move10.obj");
	small_enemy_run_display_objects[9] = new DisplayObject();
	small_enemy_run_display_objects[9]->Init( small_enemy_run_models[9], small_enemy_run_texture, shaderNMAP, small_enemy_Ntexture );



	// ALEX OWEN 16/04/15
	Animation* animator = new Animation();
	animator->SetFramesPerSec(10);
	for ( int i = 0; i<10; ++i )
	{
		animator->AddFrame( small_enemy_run_display_objects[i] );
	}
	//Add parameters for construstor
	small_enemy_run_ani = new AnimatedGameObject(XMFLOAT3(100.0f, 0.0f, 100.f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f),animator);


	// Chris, Andrew, Tyanah // POSITIONS
	XMFLOAT3 pos[] = { XMFLOAT3(0 + ARRAY_WIDTH/2*100, 0, 0 + ARRAY_HEIGHT/2*100), XMFLOAT3(300 + ARRAY_WIDTH/2*100, 0, 300 + ARRAY_HEIGHT/2*100)
						, XMFLOAT3(500 + ARRAY_WIDTH/2*100, 0, 500 + ARRAY_HEIGHT/2*100), XMFLOAT3(100 + ARRAY_WIDTH/2*100, 0, 200 + ARRAY_HEIGHT/2*100)
						, XMFLOAT3(600 + ARRAY_WIDTH/2*100, 0, 500 + ARRAY_HEIGHT/2*100), XMFLOAT3(700 + ARRAY_WIDTH/2*100, 0, 700 + ARRAY_HEIGHT/2*100)};

	for(int i = 0; i < MAX_ENEMY_OBJECTS; i++) //melee
	{
		small_enemy_game_object[i] = new SmartGameObject();
		small_enemy_game_object[i]->setPosition( pos[i] );
		small_enemy_game_object[i]->setRotation( XMFLOAT3(0, 0, 0) );
		small_enemy_game_object[i]->setScale( XMFLOAT3(1, 1, 1) );
		small_enemy_game_object[i]->animator=animator;
		small_enemy_game_object[i]->SetDisplayObject( small_enemy_game_object[i]->animator->GetCurrentFrame() );
		small_enemy_game_object[i]->SetHP(30);
	}

	// Chris, Andrew, Tyanah // RANGED
	//small_enemy_game_object[5]->setScale(XMFLOAT3(2, 2, 2));
	small_enemy_game_object[5]->brain->AIState = small_enemy_game_object[5]->brain->RANGED;

}
void ModelsDemo::LoadFutaEnemy() //no normals
{
}

bool ModelsDemo::LoadContent( )
{
	// Initialise the GraphicsManager object pointer for storing all the graphics
	graphicsManager = new GraphicsManager();
	graphicsManager->Init( d3dDevice_ );

	// test variable as a "key"
	unsigned int some_id = 0;
	unsigned int some1_id = 1;

	// Load textures into the GraphicsManager
	graphicsManager->AddTexture( some_id, "Image/arm.png" );
	graphicsManager->AddTexture( some1_id, "Image/gun.png" );
	// lock textures so no new textures can be added to avoid pointer invalidation issues
	graphicsManager->LockTextures();

	// Load models into the GraphicsManager
	graphicsManager->AddModelVTN( some_id, 	"Obj/arm.obj" );
	graphicsManager->AddModelVTN( some1_id, 	"Obj/gun.obj" );

	// lock models so no new models can be added to avoid pointer invalidation issues
	graphicsManager->LockModels();

	// Load display objects into the GraphicsManager
	graphicsManager->AddDisplayObjectVTN( some_id, some_id, some_id );
	graphicsManager->AddDisplayObjectVTN( some1_id, some1_id, some1_id );
	// lock display objects so no new display objects can be added to avoid pointer invalidation issues
	graphicsManager->LockDisplayObjects();

	// Load animation objects into the GraphicsManager
	graphicsManager->AddAnimation( some_id );
	graphicsManager->AddAnimation( some1_id );

	// Retrieving an Animation and DisplayObject from the GraphicsManager using the key to add an animation frame
	graphicsManager->GetAnimation( some_id )->AddFrame( graphicsManager->GetDisplayObject( some_id ) );
	graphicsManager->GetAnimation( some1_id )->AddFrame( graphicsManager->GetDisplayObject( some1_id ) );

	//ALEXOWEN CCTA 27/04/15 - initialise grid
	//grid.createGrid(32,32);

	//TEST EMITTERS GEORGE ANDERSON
	//AddEmitter(XMFLOAT3(0,100,0), XMFLOAT3(0,1,0), d3dDevice_);
	//AddEmitter(XMFLOAT3(300,100,0), XMFLOAT3(0,0,1), d3dDevice_);
	//AddEmitter(XMFLOAT3(-300,100,0), XMFLOAT3(0,0,0), d3dDevice_);
	//AddEmitter(XMFLOAT3(0,100,300), XMFLOAT3(1,0,0), d3dDevice_);
	//AddEmitter(XMFLOAT3(0,100,-300), XMFLOAT3(1,1,1), d3dDevice_);
	//AddEmitter(XMFLOAT3(-300,100,-300), XMFLOAT3(1,1,0), d3dDevice_);
	//AddEmitter(XMFLOAT3(300,100,-300), XMFLOAT3(0,1,1), d3dDevice_);
	//AddEmitter(XMFLOAT3(300,100,300), XMFLOAT3(1,0,1), d3dDevice_);
	//AddEmitter(XMFLOAT3(-300,100,300), XMFLOAT3(0.5,0.5,0.5), d3dDevice_);
	///////////////////////////////////////////////////////////////////////////////////////////////

	///---------LOADING COLLISION DATA---------------
	loadLvlArray("collision.txt");



	///---------LOADING COLLISION DATA---------------

	///////////////////////////////////////////////////////////////////////////////////////PARTICLE SHADER MAN///////////////// GEORGE ANDERSON
	HRESULT d3dResult;
	ID3DBlob* vsBuffer = 0;
	bool compileResult = CompileD3DShader( "ParticleShader.fx", "VS_Main", "vs_4_0", &vsBuffer );
	if( compileResult == false )
    {
        DXTRACE_MSG( "Error compiling the vertex shader!" );
        return false;
    }
    d3dResult = d3dDevice_->CreateVertexShader( vsBuffer->GetBufferPointer( ),vsBuffer->GetBufferSize( ), 0, &colourMapVS_ );
    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating the vertex shader!" );
        if( vsBuffer )
            vsBuffer->Release( );
        return false;
    }
    D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOUR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    unsigned int totalLayoutElements = ARRAYSIZE( solidColorLayout );
    d3dResult = d3dDevice_->CreateInputLayout( solidColorLayout, totalLayoutElements,vsBuffer->GetBufferPointer( ), vsBuffer->GetBufferSize( ), &inputLayout_ );
    //vsBuffer->Release( );
    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating the input layout!" );
        return false;
    }
    ID3DBlob* psBuffer = 0;
    compileResult = CompileD3DShader( "ParticleShader.fx", "PS_Main", "ps_4_0", &psBuffer );
    if( compileResult == false )
    {
        DXTRACE_MSG( "Error compiling pixel shader!" );
        return false;
    }
    d3dResult = d3dDevice_->CreatePixelShader( psBuffer->GetBufferPointer( ), psBuffer->GetBufferSize( ), 0, &colourMapPS_ );
    //psBuffer->Release( );
    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating pixel shader!" );
        return false;
    }
	///////////////////////////////////////////////////////////////////////////////////////////////END OF PARTICLE SHADER MAN /////////////////////





	// ZS-->04/02/2015 -> temporary boolean for testing
	bool z_test = false;

	// SHADER 2D ////////////////////////////////////////////////////////////
	shader2D = new Shader2D();
	z_test = shader2D->Init( d3dDevice_ );

	// SHADER VTN ///////////////////////////////////////////////////////////
	shaderVTN = new ShaderVTN();
	z_test = shaderVTN->Init( d3dDevice_ );

	// SHADER NMAP //////////////////////////////////////////////////////////
	shaderNMAP = new ShaderNMAP();
	z_test = shaderNMAP->Init( d3dDevice_ );

	// TEXT /////////////////////////////////////////////////////////////////

	// ZS-->17/02/2015 -> Create a ModelClass * object from an obj file
	text_model = new Model2D();  
	z_test = text_model->Init( d3dDevice_ );

	// ZS-->17/02/2015 -> Created a TextureClass * object from a texture file
	text_texture = new Texture();
	z_test = text_texture->Init( d3dDevice_, "Image/fontEX.png" );

	hp_texture = new Texture();
	z_test = hp_texture->Init( d3dDevice_, "Image/health.png" );
	

	// ZS-->17/02/2015 -> Created a DisplayObject * object from an obj file
	text_display_object = new DisplayObject();
	z_test = text_display_object->Init( text_model, text_texture, shader2D );

	//////loadmap//////////////////
	//HK
	
	

	

	///////////////////////////////

	//// KNIFE /////////////////////////////////////////////////////////////////

	//// ZS-->04/02/2015 -> Create a ModelClass * object from an obj file
	knife_model = new ModelVTN();  
	z_test = knife_model->Init( d3dDevice_, "Obj/knife_v1.obj" );

	//// ZS-->04/02/2015 -> Created a TextureClass * object from a texture file
	knife_texture = new Texture();
	z_test = knife_texture->Init( d3dDevice_, "Image/AyeShape_color.png" );

	//// ZS-->04/02/2015 -> Created a DisplayObject * object from an obj file
	knife_display_object = new DisplayObject();
	z_test = knife_display_object->Init( knife_model, knife_texture, shaderVTN );

	// MOON /////////////////////////////////////////////////////////////////
	//HK
	// ZS-->09/02/2015 -> Create a ModelClass * object from an obj file
	skybox_model = new ModelVTN(); 
	z_test = skybox_model->Init( d3dDevice_, "Obj/UD_MC_Skydome.obj" );
	// ZS-->09/02/2015 -> Created a TextureClass * object from a texture file
	skybox_texture = new Texture();
	z_test = skybox_texture->Init( d3dDevice_, "Image/UD_MC_Skydome.png" );

	// ZS-->09/02/2015 -> Created a DisplayObject * object from an obj file
	skybox_display_object = new DisplayObject();
	z_test = skybox_display_object->Init( skybox_model, skybox_texture, shaderVTN );

	// ZS-->09/02/2015 -> Created a moon GameObject
	skybox_game_object = new GameObject();
	skybox_game_object->setPosition( -0, -200, -0);
	skybox_game_object->setRotation(0, 0, 0);
	skybox_game_object->setScale( 1.0 );
	skybox_game_object->SetDisplayObject( skybox_display_object );

	//////////////////////////////////////////////////////////////////////
	bullet_model = new ModelVTN(); 
	z_test = bullet_model->Init( d3dDevice_, "Obj/bullet.obj" );
	// ZS-->09/02/2015 -> Created a TextureClass * object from a texture file
	bullet_texture = new Texture();
	z_test = bullet_texture->Init( d3dDevice_, "Image/bullet.png" );

	// ZS-->09/02/2015 -> Created a DisplayObject * object from an obj file
	bullet_display_object = new DisplayObject();
	z_test = bullet_display_object->Init( bullet_model, bullet_texture, shaderVTN );

	// ZS-->09/02/2015 -> Created a moon GameObject
	bullet_game_object = new GameObject();
	bullet_game_object->setPosition( -0, -200, -0);
	bullet_game_object->setRotation(0, 0, 0);
	bullet_game_object->setScale( 1.0 );
	bullet_game_object->SetDisplayObject( bullet_display_object );

	// ZS-->23/03/2015 -> Small Enemy Run Animation
	// first load the texture and objs needed to create the display objects
	// the display objects act as the frames
	
	/////////////////////////////////////////////////////////////////Bullets//////////////////////////////////////////
	// ALEX OWEN 20/04/15 set up bullets (currently using skybox display object)
	for(int i = 0; i < numBullets; i ++)
	{
		bullets[i].setScale(100);
		bullets[i].SetDisplayObject( bullet_display_object);
	}
	
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////// text engine stuff //////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// START

	vsBuffer = 0;

    compileResult = CompileD3DShader( "TextTextureMap.fx", "VS_Main", "vs_4_0", &vsBuffer );

    if( compileResult == false )
    {
        DXTRACE_MSG( "Error compiling the vertex shader!" );
        return false;
    }

    d3dResult = d3dDevice_->CreateVertexShader( vsBuffer->GetBufferPointer( ),
        vsBuffer->GetBufferSize( ), 0, &textTextureMapVS_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating the vertex shader!" );

        if( vsBuffer )
            vsBuffer->Release( );

        return false;
    }

    D3D11_INPUT_ELEMENT_DESC textSolidColorLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    totalLayoutElements = ARRAYSIZE( textSolidColorLayout );

    d3dResult = d3dDevice_->CreateInputLayout( textSolidColorLayout, totalLayoutElements,
        vsBuffer->GetBufferPointer( ), vsBuffer->GetBufferSize( ), &textInputLayout_ );

    vsBuffer->Release( );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating the input layout!" );
        return false;
    }

    psBuffer = 0;

    compileResult = CompileD3DShader( "TextTextureMap.fx", "PS_Main", "ps_4_0", &psBuffer );

    if( compileResult == false )
    {
        DXTRACE_MSG( "Error compiling pixel shader!" );
        return false;
    }

    d3dResult = d3dDevice_->CreatePixelShader( psBuffer->GetBufferPointer( ),
        psBuffer->GetBufferSize( ), 0, &textTextureMapPS_ );

    psBuffer->Release( );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating pixel shader!" );
        return false;
    }

	d3dResult = D3DX11CreateShaderResourceViewFromFile( d3dDevice_, "Image/fontEX.png", 0, 0, &textColorMap_, 0 );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to load the texture image!" );
        return false;
    }

	D3D11_SAMPLER_DESC textColorMapDesc;
    ZeroMemory( &textColorMapDesc, sizeof( textColorMapDesc ) );
    textColorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    textColorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    textColorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    textColorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    textColorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    textColorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;

    d3dResult = d3dDevice_->CreateSamplerState( &textColorMapDesc, &textColorMapSampler_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to create color map sampler state!" );
        return false;
    }

    D3D11_BUFFER_DESC textVertexDesc;
    ZeroMemory( &textVertexDesc, sizeof( textVertexDesc ) );
    textVertexDesc.Usage = D3D11_USAGE_DYNAMIC;
    textVertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    textVertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    const int sizeOfSprite = sizeof( TextVertexPos ) * 6;
    const int maxLetters = 24;

    textVertexDesc.ByteWidth = sizeOfSprite * maxLetters;

    d3dResult = d3dDevice_->CreateBuffer( &textVertexDesc, 0, &textVertexBuffer_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to create vertex buffer!" );
        return false;
    }

	// END
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////// text engine stuff //////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

    D3D11_BUFFER_DESC constDesc;
	ZeroMemory( &constDesc, sizeof( constDesc ) );
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constDesc.ByteWidth = sizeof( XMMATRIX );
	constDesc.Usage = D3D11_USAGE_DEFAULT;

	d3dResult = d3dDevice_->CreateBuffer( &constDesc, 0, &viewCB_ );

	if( FAILED( d3dResult ) )
    {
        return false;
    }

    d3dResult = d3dDevice_->CreateBuffer( &constDesc, 0, &projCB_ );

	if( FAILED( d3dResult ) )
    {
        return false;
    }

    d3dResult = d3dDevice_->CreateBuffer( &constDesc, 0, &worldCB_ );

	if( FAILED( d3dResult ) )
    {
        return false;
    }

	constDesc.ByteWidth = sizeof( XMFLOAT4 );

    d3dResult = d3dDevice_->CreateBuffer( &constDesc, 0, &camPosCB_ );

	if( FAILED( d3dResult ) )
    {
        return false;
    }
	///////////////////////////////////////////////////////////////////////////////////////////////////////
/*		D3D11_BLEND_DESC blendDesc2;
    ZeroMemory( &blendDesc2, sizeof( blendDesc2 ) );
    blendDesc2.RenderTarget[0].BlendEnable = TRUE;
	
	blendDesc2.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc2.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc2.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc2.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc2.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc2.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc2.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;


    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

    d3dDevice_->CreateBlendState( &blendDesc2, &alphaBlendState_ );
	//d3dDevice_->CreateBlendState( &BlendState, &alphaBlendState_ );
    d3dContext_->OMSetBlendState( alphaBlendState_, blendFactor, 0xFFFFFFFF );*/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D11_BLEND_DESC blendStateDescription;
	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	//blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	//blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	d3dResult = d3dDevice_->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState);
	if(FAILED(d3dResult))
	{
		return false;
	}

	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// Create the blend state using the description.
	d3dResult = d3dDevice_->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState);
	if(FAILED(d3dResult))
	{
		return false;
	}

	// MM---2015-02-16 // new maximum rendering distance
    projMatrix_ = XMMatrixPerspectiveFovLH( XM_PIDIV4, 800.0f / 600.0f, 1.0f, 1000000.0 );
    projMatrix_ = XMMatrixTranspose( projMatrix_ );

    camera_.SetDistance( 12.0f, 4.0f, 2000.0f );
	// MM---2015-02-16
	camera_.SetPosition(0.0f + ARRAY_WIDTH/2*100 , 160.0f,0.0f + ARRAY_HEIGHT/4*100);// new camera position

	

	soundLoader = new SoundLoader();
	soundLoader->Init();
    return true;
}

void ModelsDemo::UnloadContent( )
{
    if( colorMapSampler_ ) colorMapSampler_->Release( );
	if( textColorMapSampler_ ) textColorMapSampler_->Release( );
    if( colorMap_ ) colorMap_->Release( );
	if( textColorMap_ ) textColorMap_->Release( );
    if( textureMapVS_ ) textureMapVS_->Release( );
    if( textTextureMapPS_ ) textTextureMapPS_->Release( );
    if( textTextureMapVS_ ) textTextureMapVS_->Release( );
    if( textureMapPS_ ) textureMapPS_->Release( );
    if( inputLayout_ ) inputLayout_->Release( );
	if( textInputLayout_ ) textInputLayout_->Release( );
	
    if( vertexBuffer_ ) vertexBuffer_->Release( );
	if( textVertexBuffer_ ) textVertexBuffer_->Release( );
    if( viewCB_ ) viewCB_->Release( );
    if( projCB_ ) projCB_->Release( );
    if( worldCB_ ) worldCB_->Release( );
	if( camPosCB_ ) camPosCB_->Release( );

	if(m_alphaEnableBlendingState)
	{
		m_alphaEnableBlendingState->Release();
		m_alphaEnableBlendingState = 0;
	}

	if(m_alphaDisableBlendingState)
	{
		m_alphaDisableBlendingState->Release();
		m_alphaDisableBlendingState = 0;
	}

	HammerTime(); //GA

    colorMapSampler_ = 0;
	textColorMapSampler_ = 0;
    colorMap_ = 0;
	textColorMap_ = 0;
    textureMapVS_ = 0;
    textureMapPS_ = 0;
	textTextureMapVS_ = 0;
    textTextureMapPS_ = 0;
    inputLayout_ = 0;
	textInputLayout_ = 0;
	//changed vertexBuffer_ = 0; 
    vertexBuffer_ = 0;
	textVertexBuffer_ = 0;
    viewCB_ = 0;
    projCB_ = 0;
    worldCB_ = 0;

	graphicsManager->Release();

	/*if ( tank_model ) { tank_model->Release(); tank_model = NULL; }
	if ( tank_texture ) { tank_texture->Release(); tank_texture = NULL; }
	if ( tank_display_object ) { delete tank_display_object; tank_display_object = NULL; }*/
	//HK
	if( text_model ) { text_model->Release(); text_model = NULL; }
	if( text_texture ) { text_texture->Release(); text_texture = NULL; }

	if( skybox_model ) { skybox_model->Release(); skybox_model = NULL; }
	if( skybox_texture ) { skybox_texture->Release(); skybox_texture = NULL; }
	if( skybox_display_object ) { delete skybox_display_object; skybox_display_object = NULL; }
	if( skybox_game_object ) { delete skybox_game_object; skybox_game_object = NULL;}

	if( bullet_model ) { bullet_model->Release(); bullet_model = NULL; }
	if( bullet_texture ) { bullet_texture->Release(); bullet_texture = NULL; }
	if( bullet_display_object ) { delete bullet_display_object; bullet_display_object = NULL; }
	if( bullet_game_object ) { delete bullet_game_object; bullet_game_object = NULL;}

	UnloadMap();

	// NATH audio

	
	soundLoader->UnloadSoundEngine();
	
}

void ModelsDemo::TurnOnAlphaBlending()
{
	float blendFactor[4];
	
	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	
	// Turn on the alpha blending.
	d3dContext_->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);

	return;
}

void ModelsDemo::TurnOffAlphaBlending()
{
	float blendFactor[4];
	
	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	
	// Turn off the alpha blending.
	d3dContext_->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);

	return;
}

void ModelsDemo::TurnZBufferOn()
{
	d3dContext_->OMSetDepthStencilState(m_depthStencilState, 1);
	return;
}

void ModelsDemo::TurnZBufferOff()
{
	d3dContext_->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
	return;
}

void ModelsDemo::SetGameState(GameStates state)
{
	gameState_ = state;
}

GameStates ModelsDemo::GetGameState()
{
	return gameState_;
}

void ModelsDemo::Reset()
{
	playerHP = 100.0f;
	playerCooldown = .5f;
	playerAttackTimer = 1.0f;
	playerArmZ = 75.0f;
}

//ALEX OWEN - 16/02/15 - get the distance between two points
float ModelsDemo::DistanceBetweenPoints(XMFLOAT3 _position1, XMFLOAT3 _position2)
{
	float dx = _position2.x - _position1.x;
	float dy = _position2.y - _position1.y;
	float dz = _position2.z - _position1.z;
	return sqrt(dx * dx + dy * dy + dz * dz);
}

void ModelsDemo::UnloadMap()
{

	for(int c = 0; c< MAX_MAP_OBJECTS; c++)
	{
		if( objects_model[c] ) { objects_model[c]->Release(); objects_model[c] = NULL; }
		if( objects_texture[c] ) { objects_texture[c]->Release(); objects_texture[c] = NULL; }
		if( objects_Ntexture[c] ) { objects_Ntexture[c]->Release(); objects_Ntexture[c] = NULL; }
		if( objects_display_object[c] ) { delete objects_display_object[c]; objects_display_object[c] = NULL; }
		if( objects_game_object[c] ) { delete objects_game_object[c]; objects_game_object[c] = NULL; }
	}

	if( Map ) { Map->UnloadMap(); delete Map; Map = NULL; }

	if(small_enemy_texture)
	{
			small_enemy_texture->Release();
			small_enemy_texture = NULL;
	}
	if(small_enemy_Ntexture)
	{
			small_enemy_Ntexture->Release();
			small_enemy_Ntexture = NULL;
	}
	////////////////////////////////////////////////////////////////// MODELS

	if(small_enemy_model)
	{
		small_enemy_model->Release();
		small_enemy_model = NULL;
	}
	////////////////////////////////////////////////////////////////// DISPLAY OBJECTS
	if(small_enemy_display_object)
	{
		delete small_enemy_display_object;
		small_enemy_display_object = NULL;
	}
	////////////////////////////////////////////////////////////////// GAME OBJECTS
	//cat delete memeory
	for(int c = 0; c< MAX_ENEMY_OBJECTS; c++)
	{
		if(small_enemy_game_object[c])
		{
			delete small_enemy_game_object[c];
			small_enemy_game_object[c] = NULL;
		}
	}
}

void ModelsDemo::LoadMap(std::wstring MapName)
{
	Map = new MapLoader();
	Map->LoadMap(MapName);

	for(int c = 0; c < Map->textureCount(); c++)
	{
		//objects_texture[0] how many textures to make
		objects_texture[c] = new Texture();
		objects_texture[c]->Init( d3dDevice_, &Map->getImgName(c)[0u] );
		//
		std::string temp = Map->getImgName(c);
		temp.insert(temp.end()-4,1,'_');
		temp.insert(temp.end()-4,1,'N');

		bool z_test = false;
		//HK - loading normals texture - must have same name with _N at the end.
		//if image name is == 'hello.png'
		//normal image must be == 'hello_N.png'
		objects_Ntexture[c] = new Texture();
		z_test = objects_Ntexture[c]->Init( d3dDevice_, &temp[0u] );

		if(!z_test) 
		{
			objects_Ntexture[c] = NULL;
		}

	}

	for(int c = 0; c < Map->modelCount(); c++)
	{
		//objects_model[0] how many models to make
		if(objects_Ntexture[c] == NULL)
			objects_model[c] = new ModelVTN();
		else
			objects_model[c] = new ModelNMAP();

		objects_model[c]->Init( d3dDevice_, &Map->getObjName(c)[0u] );
	}

	//update grid CAT (09/03/2015) //
	/*for(int c = 0; c < Map->getObjCount(); c++)
	{
		//check before loading same model and texture
		//objects_model[0]
		objects_model[c] = new ModelVTN(); 
		objects_model[c]->Init( d3dDevice_, &Map->getObjName(c)[0u] );
		//objects_texture[0]
		objects_texture[c] = new Texture();
		objects_texture[c]->Init( d3dDevice_, &Map->getImgName(c)[0u] );
		//objects_display_object[0]
		objects_display_object[c] = new DisplayObject();
		objects_display_object[c]->Init( objects_model[c], objects_texture[c], shaderVTN );
		//objects_game_object[0]
		objects_game_object[c] = new GameObject();
		objects_game_object[c]->SetPos( Map->getObjPos(c) );
		objects_game_object[c]->SetRot( Map->getObjRot(c) );
		objects_game_object[c]->SetScale( Map->getObjSca(c) );
		objects_game_object[c]->SetDisplayObject( objects_display_object[c] );

	}*/

	// Alex Owen, CCTA 27/04/15
	for(int c = 0; c < Map->getObjCount(); c++)
	{
		//objects_display_object[0] set which model texture to use for each object
		objects_display_object[c] = new DisplayObject();
		if(objects_Ntexture[Map->getSetData(c).whichTexture] == NULL)
			objects_display_object[c]->Init( objects_model[Map->getSetData(c).whichObj], objects_texture[Map->getSetData(c).whichTexture], shaderVTN);
		else
			objects_display_object[c]->Init( objects_model[Map->getSetData(c).whichObj], objects_texture[Map->getSetData(c).whichTexture], shaderNMAP, objects_Ntexture[Map->getSetData(c).whichTexture] );
		//objects_game_object[0]
		objects_game_object[c] = new GameObject();
		objects_game_object[c]->setPosition( Map->getObjPos(c).z + ARRAY_WIDTH/2*100, Map->getObjPos(c).y, Map->getObjPos(c).x + ARRAY_HEIGHT/2*100);
		objects_game_object[c]->setRotation( Map->getObjRot(c) );
		objects_game_object[c]->setScale( Map->getObjSca(c) );
		objects_game_object[c]->SetDisplayObject( objects_display_object[c] );

		// Chris, Andrew, Cosmin (16/03/15) //
		int a = (int)objects_game_object[c]->getX()/100;
		int b = (int)objects_game_object[c]->getZ()/100;
		
		//if(c != 0) levelArray[a][b] = 1;

		/*for(int i = 0; i < (objectWidth * 100) / 2; i++)
		{
			for(int j = 0; j < (objectHeight * 100) / 2; j++)
			{
				if(canSeeOver == true)
				{
					levelArray[i][j] = 1;
				}
				else
				{
					levelArray[i][j] = 2;
				}
			}
		}*/
	}



}

void ModelsDemo::Update( float dt )
{
	MenuSwitched = false;

	if(gameState_ == RUN)
	{
		UpdateParticleHandler(dt);//GA

		// Chris, Andrew, Tyanah //
		static float enemyAttackTimer = 1.0f;
		static float enemyCooldown = 1.0f;
		enemyAttackTimer += dt;
		smallestManhattan = 99999.0f;
		int ID = -1;

		// Chris, Andrew, Tyanah //
		// Sets the Id's dependant on the objects in the level //
		for(int c = 0; c < Map->getObjCount(); c++)
		{
			if(small_enemy_game_object[5]->returnManhattanDistance
				((int)objects_game_object[c]->getPosition().x / 100, 
				(int)objects_game_object[c]->getPosition().z / 100, camera_.GetPosition())
				< smallestManhattan)
			{
				ID = c;
			}	
		}


		

		// Chris, Andrew, Tyanah //
		// Sets the ranged positions to the object positions //
		small_enemy_game_object[5]->brainFreeze.rangedX = (int)objects_game_object[ID]->getPosition().x / 100;
		small_enemy_game_object[5]->brainFreeze.rangedZ = (int)objects_game_object[ID]->getPosition().z / 100;
		smallestManhattan = 99999.0f;

		// Chris, Andrew, Tyanah //
		for(int i = 0; i < MAX_ENEMY_OBJECTS; i++)
		{

			if(small_enemy_game_object[i]->gethp() > 0){
				small_enemy_game_object[i]->SetDisplayObject( small_enemy_game_object[i]->animator->GetCurrentFrame() );
			}

			if(playerHP > 0)
			{

			// If the enemy isn't dead, attacking or a in a ranged state //
			if (small_enemy_game_object[i]->brain->AIState != small_enemy_game_object[i]->brain->DEAD
				&& small_enemy_game_object[i]->brain->AIState != small_enemy_game_object[i]->brain->ATTACK
				&& small_enemy_game_object[i]->brain->AIState != small_enemy_game_object[i]->brain->RANGED)
			{
				small_enemy_game_object[i]->returnManhattanDistance(camera_.GetPosition());

				if (small_enemy_game_object[i]->brainFreeze.manDistPlayer 
					< smallestManhattan && small_enemy_game_object[i]->brain->AIState != small_enemy_game_object[i]->brain->ATTACK)
				{
					small_enemy_game_object[i]->brain->AIState = small_enemy_game_object[i]->brain->LEADER;
					smallestManhattan = small_enemy_game_object[i]->brainFreeze.manDistPlayer;
				}
				else 
				{
					small_enemy_game_object[i]->brain->AIState = small_enemy_game_object[i]->brain->SWARM;
				}
			}

			}

			if(playerHP > 0)
			{
			
			// If the enemy is the leader //
			if(small_enemy_game_object[i]->brain->AIState == small_enemy_game_object[i]->brain->LEADER)
			{
				int newX = small_enemy_game_object[i]->brainFreeze.leaderGridX;
				int newZ = small_enemy_game_object[i]->brainFreeze.leaderGridZ;

				for(int i=0; i < MAX_ENEMY_OBJECTS; i++)
				{
					small_enemy_game_object[i]->brainFreeze.leaderGridX = newX;
					small_enemy_game_object[i]->brainFreeze.leaderGridZ = newZ;
				}
			}

			}
		}

		// Chris, Andrew, Tyanah //
		for(int i=0; i < MAX_ENEMY_OBJECTS; i++)
		{
			if(playerHP > 0)
			{

			// If the enemy is not dead or in a ranged state //
			if(small_enemy_game_object[i]->brainFreeze.manDistPlayer < 2.0f 
				&& small_enemy_game_object[i]->brain->AIState != small_enemy_game_object[i]->brain->DEAD
				&& small_enemy_game_object[i]->brain->AIState != small_enemy_game_object[i]->brain->RANGED)
			{
				small_enemy_game_object[i]->brain->AIState = small_enemy_game_object[i]->brain->ATTACK;
			}

			// If the enemy is in attack state //
			if (small_enemy_game_object[i]->brain->AIState == small_enemy_game_object[i]->brain->ATTACK
				&& small_enemy_game_object[i]->brainFreeze.manDistPlayer > 2.0f
				&& small_enemy_game_object[i]->brain->AIState != small_enemy_game_object[i]->brain->DEAD
				&& small_enemy_game_object[i]->brain->AIState != small_enemy_game_object[i]->brain->RANGED)
			{
				small_enemy_game_object[i]->brain->AIState = small_enemy_game_object[i]->brain->SWARM;

				// Used to check if the area around the enemy is occupied //
				/*for(int j=0; j < 8; j++)
				{
					if(small_enemy_game_object[i]->brainFreeze.attackID[j] == 2)
					{
						small_enemy_game_object[i]->brainFreeze.attackID[j] = 0;

						for(int k = 0; k < MAX_ENEMY_OBJECTS; k++)
						{
							small_enemy_game_object[k]->brainFreeze.attackID[j] = 0;
						}
					}
				}*/
			}

			}

			if(playerHP > 0)
			{
			// Chris, Cosmin, Tyanah, Andrew //
			if(small_enemy_game_object[i]->brain->AIState == small_enemy_game_object[i]->brain->ATTACK)
			{
				if(enemyAttackTimer >= enemyCooldown){
						if(DistanceBetweenPoints(camera_.GetPosition(), small_enemy_game_object[i]->getPosition()) < 350)
						{
							HitPlayer(3.0f);
							enemyAttackTimer = 0.0f;
							AddEmitter(camera_.GetPosition(), XMFLOAT3(1,0,0), d3dDevice_);
							
						}
				}
			}
			}

			

			small_enemy_game_object[i]->update(camera_.GetPosition(), dt);

			// Used to check if the area around the enemy is occupied //
			/*for(int j=0; j < 8; j++)
			{
				if(small_enemy_game_object[i]->brainFreeze.attackID[j] == 2)
				{
					for(int k = 0; k < MAX_ENEMY_OBJECTS; k++)
					{
						small_enemy_game_object[k]->brainFreeze.attackID[j] = 1;
					}
				}
			}*/
		}
		
		small_enemy_game_object[0]->animator->Update(dt);
		// ALEX OWEN 20/04/15 old rotation code
		//small_enemy_game_object[0]->setRotation(small_enemy_game_object[0]->getRX(), small_enemy_game_object[0]->getRY() + 0.0001, small_enemy_game_object[0]->getRZ());

		// ZS-->09/02/2015 -> moon rotation update
		static float moon_rotation = 0;
		moon_rotation += 0.01f * dt;
		if ( moon_rotation > 360.0f ) moon_rotation -= 360.0f;
		skybox_game_object->setRotation( 0, moon_rotation, 0 );
		// HK
		skybox_game_object->setPosition(camera_.GetPosition());
		// MM---2015-02-16
		//measure_game_object->SetRot( 0, 0, 0 );

		//ALEX OWEN - 30/03/15 - update to handle shooting. Will be moved to seperate class
		if(shooting)
		{
			shotDelay ++;
			if(shotDelay > shotDelayMax)
			{
				shooting = false;
				shotDelay = 0;
				//OutputDebugString("shotDelay Max Reached!");
			}
		}

		//camera_.ApplyZoom(.01);
		//camera_.ApplyRotation(0.001,0.001);
	
		////changed to i = 1 so i can use tank[0] for testing
		//for(int i = 1; i < numberOfTanks; i++)	
		//	tank[i].Pos.z+=1*dt;


		
	}

	float moveSpeed=0.0001f;
	// MM---2015-02-16
	float moveSpeed2=500.0f;
	float zoom = 0.0;
	float xRotation=0.0;
	float yRotation=0.0;

	/*if(keystate[DIK_LEFT] & 0x80)
	{
        yRotation += moveSpeed;
	}
    if(keystate[DIK_RIGHT] & 0x80)
	{
        yRotation -= moveSpeed;
	}
	
	if(keystate[DIK_DOWN] & 0x80)
	{
        xRotation += moveSpeed;
	}
    if(keystate[DIK_UP] & 0x80)
	{
        xRotation -= moveSpeed;
	}
	if(keystate[DIK_SUBTRACT] & 0x80)
	{
        zoom += moveSpeed*2;
	}
    if(keystate[DIK_ADD] & 0x80)
	{
        zoom -= moveSpeed*2;
	}*/

	//moveSpeed=0.001f;
	float moveLeftRight =0.0;
	float moveBackForward =0.0;

	/*if(keystate[DIK_LEFT] & 0x80)
	{
        yRotation -= moveSpeed*.1f;
	}
    if(keystate[DIK_RIGHT] & 0x80)
	{
        yRotation += moveSpeed*.1f;
	}*/

	if(gameState_ == START_MENU)
	{

		//if(keystate[DIK_RETURN] & 0x80)
		//{
		//	gameState_ = RUN;
		//	Reset();
		//	LoadMap(L"Org_Objs.txt");
		//}

		if(keystate[DIK_RETURN] & 0x80 && mainMenuSelection == LEVEL_ONE)
		{
			gameState_ = RUN;
			soundLoader->PlaySound(SOUND_ID::RAIN);
			Reset();
			LoadMap(L"Org_Objs.txt");
			LoadOrgEnemy();
		}
		if(keystate[DIK_RETURN] & 0x80 && mainMenuSelection == LEVEL_TWO)
		{
			gameState_ = RUN;
			soundLoader->PlaySound(SOUND_ID::RAIN);
			Reset();
			LoadMap(L"UD_LVL1_Coordinates.txt");
			LoadUrbEnemy();
		}
		if(keystate[DIK_RETURN] & 0x80 && mainMenuSelection == LEVEL_THREE)
		{
			gameState_ = RUN;
			soundLoader->PlaySound(SOUND_ID::RAIN);
			Reset();
			LoadMap(L"Org_Objs.txt");
			LoadOrgEnemy();
		}
		if(keystate[DIK_RETURN] & 0x80 && mainMenuSelection == EXIT)
		{
			PostQuitMessage(0);
		}

		if(
			(!(keystate[DIK_DOWN] & 0x80)&&(keyPrevState[DIK_DOWN] & 0x80))
			||
			(!(keystate[DIK_S] & 0x80)&&(keyPrevState[DIK_S] & 0x80))
			)
		{
			mainMenuSelection++;

		}
		if(
			(!(keystate[DIK_UP] & 0x80)&&(keyPrevState[DIK_UP] & 0x80))
			||
			(!(keystate[DIK_W] & 0x80)&&(keyPrevState[DIK_W] & 0x80))
			)

		{
			mainMenuSelection--;
		}

	}



	//resolution menu GA
	if(gameState_ == RES_MENU)
	{
		if(!(keystate[DIK_ESCAPE] & 0x80)&&(keyPrevState[DIK_ESCAPE] & 0x80))
		{
			//PostQuitMessage(0);
			gameState_ = PAUSED;
		}
		if(!(keystate[DIK_RETURN] & 0x80)&&(resolutionMenuSelection == BACK)&&(keyPrevState[DIK_RETURN] & 0x80) && MenuSwitched == false)
		{
			//PostQuitMessage(0);
			gameState_ = PAUSED;
			//SetWindowPos(hwnd_, hwnd_, 0, 0, 800, 600, SWP_NOACTIVATE | SWP_NOZORDER);
			//UpdateWidthandHeight(800, 600);
			//HRESULT d3dResult;
			//d3dResult = swapChain_->ResizeBuffers(0, 800, 600, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
			MenuSwitched = true;
		}
		if((keystate[DIK_RETURN] & 0x80)&&(resolutionMenuSelection == OPTA)&&(keyPrevState[DIK_RETURN] & 0x80))
		{
			//PostQuitMessage(0);
			//gameState_ = RUN;
			SetWindowPos(hwnd_, hwnd_, 0, 0, 800, 600, SWP_NOACTIVATE | SWP_NOZORDER);
			UpdateWidthandHeight(800, 600);
			HRESULT d3dResult;
			d3dResult = swapChain_->ResizeBuffers(0, 800, 600, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		}
		if((keystate[DIK_RETURN] & 0x80)&&(resolutionMenuSelection == OPTB)&&(keyPrevState[DIK_RETURN] & 0x80))
		{
			//PostQuitMessage(0);
			//gameState_ = RUN;
			SetWindowPos(hwnd_, hwnd_, 0, 0, 1024, 768, SWP_NOACTIVATE | SWP_NOZORDER);
			UpdateWidthandHeight(1024, 768);
			HRESULT d3dResult;
			d3dResult = swapChain_->ResizeBuffers(0, 1024, 768, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		}
		if((keystate[DIK_RETURN] & 0x80)&&(resolutionMenuSelection == OPTC)&&(keyPrevState[DIK_RETURN] & 0x80))
		{
			//PostQuitMessage(0);
			//gameState_ = RUN;
			SetWindowPos(hwnd_, hwnd_, 0, 0, 1280, 960, SWP_NOACTIVATE | SWP_NOZORDER);
			UpdateWidthandHeight(1280, 960);
			HRESULT d3dResult;
			d3dResult = swapChain_->ResizeBuffers(0, 1280, 960, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		}
		if((keystate[DIK_RETURN] & 0x80)&&(resolutionMenuSelection == OPTD)&&(keyPrevState[DIK_RETURN] & 0x80))
		{
			//PostQuitMessage(0);
			//gameState_ = RUN;
			SetWindowPos(hwnd_, hwnd_, 0, 0, 1600, 900, SWP_NOACTIVATE | SWP_NOZORDER);
			UpdateWidthandHeight(1600, 900);
			HRESULT d3dResult;
			d3dResult = swapChain_->ResizeBuffers(0, 1600, 900, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		}
		if((keystate[DIK_RETURN] & 0x80)&&(resolutionMenuSelection == OPTE)&&(keyPrevState[DIK_RETURN] & 0x80))
		{
			//PostQuitMessage(0);
			//gameState_ = RUN;
			SetWindowPos(hwnd_, hwnd_, 0, 0, 1920, 1080, SWP_NOACTIVATE | SWP_NOZORDER);
			UpdateWidthandHeight(1920, 1080);
			HRESULT d3dResult;
			d3dResult = swapChain_->ResizeBuffers(0, 1920, 1080, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		}


		if(
			(!(keystate[DIK_DOWN] & 0x80)&&(keyPrevState[DIK_DOWN] & 0x80))
			||
			(!(keystate[DIK_S] & 0x80)&&(keyPrevState[DIK_S] & 0x80))
			)
		{
			resolutionMenuSelection++;

		}
		if(
			(!(keystate[DIK_UP] & 0x80)&&(keyPrevState[DIK_UP] & 0x80))
			||
			(!(keystate[DIK_W] & 0x80)&&(keyPrevState[DIK_W] & 0x80))
			)

		{
			resolutionMenuSelection--;
		}


	}



	if(gameState_ == PAUSED)
	{
		
		if(!(keystate[DIK_ESCAPE] & 0x80)&&(keyPrevState[DIK_ESCAPE] & 0x80))
		{
			//PostQuitMessage(0);
			gameState_ = RUN;
		}
		if((keystate[DIK_RETURN] & 0x80)&&(pauseMenuSelection == RETURN))
		{
			//PostQuitMessage(0);
			gameState_ = RUN;
		}
		if((keystate[DIK_RETURN] & 0x80)&&(pauseMenuSelection == PLAY_MOVIE))
		{
			//PostQuitMessage(0);
			gameState_ = INTRO_MOVIE_REPLAY;
		}
		
		if((keystate[DIK_RETURN] & 0x80)&&(pauseMenuSelection == QUIT))
		{
			PostQuitMessage(0);
		}

		if(!(keystate[DIK_RETURN] & 0x80)&&(pauseMenuSelection == RESOLUTION_MENU)&&(keyPrevState[DIK_RETURN] & 0x80) && MenuSwitched == false)
		{
			gameState_ = RES_MENU;
			MenuSwitched = true;
		}
		
		if((!(keystate[DIK_RETURN] & 0x80)&&(keyPrevState[DIK_RETURN] & 0x80))
			&&(pauseMenuSelection == FPS))
		{
			displayFPS = !displayFPS;
		}

		if((!(keystate[DIK_RETURN] & 0x80)&&(keyPrevState[DIK_RETURN] & 0x80))
			&&(pauseMenuSelection == FULLSCREEN))
		{
			FullScreenState = !FullScreenState;
			swapChain_->SetFullscreenState(FullScreenState, NULL);
		}
		
		if(
			(!(keystate[DIK_DOWN] & 0x80)&&(keyPrevState[DIK_DOWN] & 0x80))
			||
			(!(keystate[DIK_S] & 0x80)&&(keyPrevState[DIK_S] & 0x80))
			)
		{
			pauseMenuSelection++;

			/*if(pauseMenuSelection == FPS)
			{
				pauseMenuSelection = RETURN;
			}
			else
			{
				pauseMenuSelection = FPS;
			}*/
		}
		if(
			(!(keystate[DIK_UP] & 0x80)&&(keyPrevState[DIK_UP] & 0x80))
			||
			(!(keystate[DIK_W] & 0x80)&&(keyPrevState[DIK_W] & 0x80))
			)

		{
			pauseMenuSelection--;
			//if(pauseMenuSelection == FPS)
			//{
			//	pauseMenuSelection = RETURN;
			//}
			//else
			//{
			//	pauseMenuSelection = FPS;
			//}
		}


	}

	if(gameState_ == RUN)
	{
		if((keystate[DIK_LEFT] & 0x80)||(keystate[DIK_A] & 0x80))
		{
			//moveLeftRight -= moveSpeed;
			moveLeftRight -= moveSpeed2*dt;
		}
		if((keystate[DIK_RIGHT] & 0x80)||(keystate[DIK_D] & 0x80))
		{
			//moveLeftRight += moveSpeed;
			moveLeftRight += moveSpeed2*dt;
		}
	
		if((keystate[DIK_DOWN] & 0x80)||(keystate[DIK_S] & 0x80))
		{
			//moveBackForward  -= moveSpeed;
			moveBackForward  -= dt*moveSpeed2;
		}
		if((keystate[DIK_UP] & 0x80)||(keystate[DIK_W] & 0x80))
		{
			//moveBackForward  += moveSpeed;
			moveBackForward  += dt*moveSpeed2;
		}
		if(keystate[DIK_SUBTRACT] & 0x80)
		{
			zoom += moveSpeed*2;
		}
		if(keystate[DIK_ADD] & 0x80)
		{
			zoom -= moveSpeed*2;
		}
	
		if(!(keystate[DIK_ESCAPE] & 0x80)&&(keyPrevState[DIK_ESCAPE] & 0x80))
		{
			//PostQuitMessage(0);
			gameState_ = PAUSED;
		}

		
		//ALEX OWEN - 20/04/15 - Bullet firing code
		if(mouseCurrState.rgbButtons[0] & 0x80 && !shooting && gunorknife)
		{
			shooting = true;
			shotDelay = 0;
			bool shot = false;
			for(int i = 0; i < numBullets; ++i)
			{
				//if bullet hasn't been fired
				//ALEX OWEN 20/04/15
				if(!bullets[i].fired && !shot)
				{
					XMVECTOR bulletPosition;
					bulletPosition = XMLoadFloat3(&camera_.GetPosition());
					
					bullets[i].setStartPoint(camera_.GetPosition());
					bullets[i].setPositionVec(bulletPosition);
					//XMVECTOR bulletRotation = small_enemy_game_object[0]->getRotationVec();
					XMVECTOR bulletRotation = camera_.GetRotationVec();
					bullets[i].setRotationVec(bulletRotation);
					bullets[i].updateVectors();
					bullet_game_object->setRotation(XMFLOAT3(camera_.xRotation_, camera_.yRotation_ + 1.57, 0));
					
 					XMVECTOR bulletVelocity = XMVector3Normalize(bullets[i].getForwardVector()) * bullets[i].getMaxSpeed();
					bullets[i].setVelocity(bulletVelocity);
					/*
					XMVECTOR Determinant;

					XMMATRIX invertView = XMMatrixInverse(&Determinant, camera_.GetViewMatrix());

					XMMATRIX translationMatrix = XMMatrixTranslation(100.0f,0.0f,100.0f);
			//this needs to be done based on what the gun model looks like, basicly move it foward and to the right for example

					worldMat =tran*invertView;
			//world matrix for the gun
			*/
					bullets[i].fired = true;
					shot = true;
				}
			}
		}
		
		if(mouseCurrState.lZ > 0.1){
			gunorknife = true;
		}
		if(mouseCurrState.lZ < -0.1){
			gunorknife = false;
		}

		playerAttackTimer += dt;
		if(playerAttackTimer >= playerCooldown) playerArmZ = 75.0f;

		if((mouseCurrState.lX != mousePrevState.lX) || (mouseCurrState.lY != mousePrevState.lY))
		{
			yRotation += mousePrevState.lX * 0.005f;
			//yRotation += mousePrevState.lX * dt*2.0f;//mouse movement already based on time

			xRotation += mouseCurrState.lY * 0.005f;
			//xRotation += mouseCurrState.lY * dt*2.0f;/mouse movement already based on time

			if(mouseCurrState.rgbButtons[0] & 0x80 && !gunorknife)
			{
				if(playerAttackTimer >= playerCooldown)
				{
					playerArmZ = 100.0f;
					playerAttackTimer = 0.0f;
					for(int j = 0; j < MAX_ENEMY_OBJECTS; j ++)
					{
						if(DistanceBetweenPoints(camera_.GetPosition(), small_enemy_game_object[j]->getPosition()) < 450)
						{
							small_enemy_game_object[j]->Hit(10.0f);

							/*for(int i = 0; i < MAX_ENEMY_OBJECTS; i++)
						{
							small_enemy_game_object[i]->brain->AIState = small_enemy_game_object[i]->brain->ATTACK;
						}*/
							
							if(small_enemy_game_object[j]->gethp() <= 0.0f) 
							{
								small_enemy_game_object[j]->setRotationZ(XM_PI);
								small_enemy_game_object[j]->setPositionY(100.0f);
								small_enemy_game_object[j]->brain->AIState = small_enemy_game_object[j]->brain->DEAD;
								AddEmitter(small_enemy_game_object[j]->getPosition(), XMFLOAT3(0,0,1), d3dDevice_);
							}
						}
					}
				}
			}
		
			if(xRotation>XM_PI/8)
			{
				xRotation=XM_PI/8;
			}

			if(xRotation<-(XM_PI/8))
			{
				xRotation=-(XM_PI/8);
			}

			mousePrevState = mouseCurrState;

			
		}

		// ZS-->22/03/2015 -> update small enemy animation
			small_enemy_run_ani->update( dt );

			// ALEX OWEN 20/04/15 - update all bullets
			for(int i = 0; i < numBullets; i++)
			{
				if(bullets[i].fired)
				{
					bullets[i].update(dt);
					collisions();
				}
			}
		/*
		// ALEX OWEN 20/04/15 - update all bullets
		for(int i = 0; i < numBullets; i++)
		{
			if(bullets[i].fired)
			{
				bullets[i].update(dt);
			}
		}
		*/
		
		//ALEX OWEN , CCTA, 27/04, before moving camera check if grid space is occupied and reduce
		//velocity.
		//int gridX = ((int)camera_.GetPosition().x + moveLeftRight) / 100;
		//int gridZ = ((int)camera_.GetPosition().z + moveBackForward) / 100;
		/*int gridX = ((int)camera_.GetPosition().x) / 100;
		int gridZ = ((int)camera_.GetPosition().z) / 100;
		if(levelArray[gridX][gridZ+1] > 0)
		{
			if(camera_.GetPosition().z > ((gridZ + 1) * 100) - 5)
				camera_.SetPosition(camera_.GetPosition().x, camera_.GetPosition().y, ((gridZ + 1) * 100) - 5);
		}

		if(levelArray[gridX][gridZ-1] > 0)
		{
			if(camera_.GetPosition().z < ((gridZ) * 100) + 5)
				camera_.SetPosition(camera_.GetPosition().x, camera_.GetPosition().y, ((gridZ) * 100) + 5);
		}

		if(levelArray[gridX + 1][gridZ] > 0)
		{
			if(camera_.GetPosition().x > ((gridX + 1) * 100) - 5)
				camera_.SetPosition(((gridX + 1) * 100) - 5, camera_.GetPosition().y, camera_.GetPosition().z);
		}

		if(levelArray[gridX - 1][gridZ] > 0)
		{
			if(camera_.GetPosition().x < ((gridX) * 100) + 5)
				camera_.SetPosition(((gridX) * 100) + 5, camera_.GetPosition().y, camera_.GetPosition().z);
		}
		
		if(camera_.GetPosition().x < 0)
		{
			camera_.SetPosition(1, camera_.GetPosition().y, camera_.GetPosition().z);
		}

		if(camera_.GetPosition().x > GRID_SIZE * 100)
		{
			camera_.SetPosition(GRID_SIZE * 100, camera_.GetPosition().y, camera_.GetPosition().z);
		}

		if(camera_.GetPosition().z < 0)
		{
			camera_.SetPosition(camera_.GetPosition().x, camera_.GetPosition().y, 1);
		}

		if(camera_.GetPosition().z > GRID_SIZE * 100)
		{
			camera_.SetPosition(camera_.GetPosition().x, camera_.GetPosition().y, GRID_SIZE * 100);
		}*/


		//check if dead
		int kaplan = 0;
		for(int huseyin = 0; huseyin < MAX_ENEMY_OBJECTS; huseyin ++)
		{
			if(small_enemy_game_object[huseyin]->gethp() > 0.0f) 
			{
				kaplan++;
			}
		}
		if(playerWin == true && GetParticleNumber() == 0 && wintimer > 10){
			playerWin = false;
			wintimer = 0;
			UnloadMap();
			gameState_ = START_MENU;
			
		}
		if(playerWin == false && kaplan == 0){
			playerWin = true;
			AddEmitter(XMFLOAT3(camera_.position_.x +300,100,camera_.position_.z), XMFLOAT3(0,0,1), 50, d3dDevice_);
			AddEmitter(XMFLOAT3(camera_.position_.x -300,100,camera_.position_.z), XMFLOAT3(0,0,0), 50, d3dDevice_);
			AddEmitter(XMFLOAT3(camera_.position_.x,100,camera_.position_.z + 300), XMFLOAT3(1,0,0), 50, d3dDevice_);
			AddEmitter(XMFLOAT3(camera_.position_.x,100,camera_.position_.z -300), XMFLOAT3(1,1,1), 50, d3dDevice_);
			AddEmitter(XMFLOAT3(camera_.position_.x -300,100,camera_.position_.z - 300), XMFLOAT3(1,1,0), 50, d3dDevice_);
			AddEmitter(XMFLOAT3(camera_.position_.x + 300,100,camera_.position_.z -300), XMFLOAT3(0,1,1), 50, d3dDevice_);
			AddEmitter(XMFLOAT3(camera_.position_.x + 300,100,camera_.position_.z + 300), XMFLOAT3(1,0,1), 50, d3dDevice_);
			AddEmitter(XMFLOAT3(camera_.position_.x -300,100,camera_.position_.z + 300), XMFLOAT3(0.5,0.5,0.5), 50, d3dDevice_);
		}
		if(playerWin == true){
			wintimer++;
		}

		camera_.Move(moveLeftRight, moveBackForward);

		camera_.ApplyZoom(zoom);
		camera_.ApplyRotation(xRotation,yRotation);

		if(playerHP <= 0.0f)
							{
								UnloadMap();
								gameState_ = START_MENU;
							}

	}

	//camera_.SetPosition(10.0f,3.0f, 10.0f);

	/*
    unsigned long result = XInputGetState( 0, &controller1State_ );

    if( result != ERROR_SUCCESS )
    {
        return;
    }

    // Button press event.
    if( controller1State_.Gamepad.wButtons & XINPUT_GAMEPAD_BACK )
	{ 
		PostQuitMessage( 0 );
	}

    // Button up event.
    if( ( prevController1State_.Gamepad.wButtons & XINPUT_GAMEPAD_B ) &&
        !( controller1State_.Gamepad.wButtons & XINPUT_GAMEPAD_B ) )

	{ 
		camera_.ApplyZoom( -1.0f );
	}

    // Button up event.
    if( ( prevController1State_.Gamepad.wButtons & XINPUT_GAMEPAD_A ) &&
        !( controller1State_.Gamepad.wButtons & XINPUT_GAMEPAD_A ) )

	{ 
		camera_.ApplyZoom( 1.0f );
	}

    float yawDelta = 0.0f;
    float pitchDelta = 0.0f;

    if( controller1State_.Gamepad.sThumbRY < -1000 ) yawDelta = -0.001f;
    else if( controller1State_.Gamepad.sThumbRY > 1000 ) yawDelta = 0.001f;

    if( controller1State_.Gamepad.sThumbRX < -1000 ) pitchDelta = -0.001f;
    else if( controller1State_.Gamepad.sThumbRX > 1000 ) pitchDelta = 0.001f;

    camera_.ApplyRotation( yawDelta, pitchDelta );
	*/

    memcpy( &prevController1State_, &controller1State_, sizeof( XINPUT_STATE ) );
}

// ALEX OWEN 20/04/15 - Function for handling collisions.
void ModelsDemo::collisions()
{
	for(int i = 0; i < numBullets; ++i)
	{
		if(!bullets[i].fired) continue;
		for(int j = 0; j < MAX_ENEMY_OBJECTS; ++j)
		{
			float scaleZ = small_enemy_game_object[j]->getSZ();
			if(DistanceBetweenPoints(bullets->getPosition(), small_enemy_game_object[j]->getPosition()) < 150.0f)
			{
				small_enemy_game_object[j]->Hit(10.0f);
						
				if(small_enemy_game_object[j]->gethp() <= 0.0f) 
				{
					small_enemy_game_object[j]->setRotationZ(XM_PI);
					small_enemy_game_object[j]->setPositionY(100.0f);
					small_enemy_game_object[j]->brain->AIState = small_enemy_game_object[j]->brain->DEAD;
					AddEmitter(small_enemy_game_object[j]->getPosition(), XMFLOAT3(0,0,1), d3dDevice_);
				}
				bullets[i].stopBullet();
				AddEmitter(bullets[i].getPosition(), XMFLOAT3(1,1,0), d3dDevice_);
			}
		}
	}
}

bool ModelsDemo::DrawString( char* message, float startX, float startY )
{
	//float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    //d3dContext_->ClearRenderTargetView( backBufferTarget_, clearColor );


    // Size in bytes for a single sprite.
    const int sizeOfSprite = sizeof( TextVertexPos ) * 6;

    // Demo's dynamic buffer setup for max of 24 letters.
    const int maxLetters = 24;

    int length = strlen( message );

    // Clamp for strings too long.
    if( length > maxLetters )
        length = maxLetters;

    // Char's width on screen.
    float charWidth = 32.0f / 800.0f;

    // Char's height on screen.
    float charHeight = 32.0f / 640.0f;
    
    // Char's texel width.
    //float texelWidth = 32.0f / 864.0f;
	float texelWidth = 32.0f / 3072.0f;

    // verts per-triangle (3) * total triangles (2) = 6.
    const int verticesPerLetter = 6;

    D3D11_MAPPED_SUBRESOURCE mapResource;
    HRESULT d3dResult = d3dContext_->Map( textVertexBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to map resource!" );
        return false;
    }

    // Point to our vertex buffer's internal data.
    TextVertexPos *spritePtr = ( TextVertexPos* )mapResource.pData;

	const int indexSpace = static_cast<char>( ' ' );
    const int indexA = static_cast<char>( 'A' );
    const int indexZ = static_cast<char>( 'Z' );
	const int indexSquare = static_cast<char>( 127 );

    for( int i = 0; i < length; ++i )
    {
        float thisStartX = startX + ( charWidth * static_cast<float>( i ) );
        float thisEndX = thisStartX + charWidth;
        float thisEndY = startY + charHeight;

        spritePtr[0].pos = XMFLOAT3( thisEndX,   thisEndY, 1.0f );
        spritePtr[1].pos = XMFLOAT3( thisEndX,   startY,   1.0f );
        spritePtr[2].pos = XMFLOAT3( thisStartX, startY,   1.0f );
        spritePtr[3].pos = XMFLOAT3( thisStartX, startY,   1.0f );
        spritePtr[4].pos = XMFLOAT3( thisStartX, thisEndY, 1.0f );
        spritePtr[5].pos = XMFLOAT3( thisEndX,   thisEndY, 1.0f );

        int texLookup = 0;
        int letter = static_cast<char>( message[i] );

        //if( letter < indexA || letter > indexZ )
        if( letter < indexSpace || letter > indexSquare )
		{
            // Grab one index past Z, which is a blank space in the texture.
           // texLookup = ( indexZ - indexA ) + 1;
			texLookup = indexSquare;
        }
        else
        {
            // A = 0, B = 1, Z = 25, etc.
            //texLookup = ( letter - indexA );
			texLookup = ( letter - indexSpace ); 
        }

        float tuStart = 0.0f + ( texelWidth * static_cast<float>( texLookup ) );
        float tuEnd = tuStart + texelWidth;

        spritePtr[0].tex0 = XMFLOAT2( tuEnd, 0.0f );
        spritePtr[1].tex0 = XMFLOAT2( tuEnd, 1.0f );
        spritePtr[2].tex0 = XMFLOAT2( tuStart, 1.0f );
        spritePtr[3].tex0 = XMFLOAT2( tuStart, 1.0f );
        spritePtr[4].tex0 = XMFLOAT2( tuStart, 0.0f );
        spritePtr[5].tex0 = XMFLOAT2( tuEnd, 0.0f );

        spritePtr += 6;
    }

    d3dContext_->Unmap( textVertexBuffer_, 0 );
    d3dContext_->Draw( 6 * length, 0 );

    return true;
}

bool ModelsDemo::DrawGameObject( GameObject * game_object )
{
	// if the object is null, return
	if (!game_object) return false;

	// get the display object from the game object
	DisplayObject * display_object = game_object->GetDisplayObject();

	// activate the GameObject's shader on the graphics card
	display_object->GetShader()->Render( d3dContext_ );

	// activate the GameObject's vertex buffer on the graphics card
	display_object->GetModel()->Render( d3dContext_ );

	// activate the GameObject's texture buffer on the graphics card
	display_object->GetTexture()->Render( d3dContext_ );

	// create matrices to create a single world matrix for the GameObject's transform
	XMMATRIX scale_mat = XMMatrixScaling(game_object->getSX(), game_object->getSY(), game_object->getSZ());
	XMMATRIX rotation_mat = XMMatrixRotationRollPitchYaw(game_object->getRX(), game_object->getRY(), game_object->getRZ());
	XMMATRIX position_mat = XMMatrixTranslation(game_object->getX(), game_object->getY(), game_object->getZ());
	
	// 1) scale 
	// 2) rotate 
	// 3) position
	XMMATRIX world_mat = XMMatrixTranspose( scale_mat * rotation_mat * position_mat );

	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

	// THESE OPERATIONS ARE NOT ENCAPSULATED YET !!!

	// update the world matrix on the graphics card
	d3dContext_->UpdateSubresource( worldCB_, 0, 0, &world_mat, 0, 0 );

	// set the world matrix on the vertex shader
	d3dContext_->VSSetConstantBuffers( 0, 1, &worldCB_ );

	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

	d3dContext_->Draw( display_object->GetModel()->GetVertexCount(), 0 );

	return true;
}

void ModelsDemo::Render( )
{
	// if the context has not been initialised, dont try to render anything
    if( d3dContext_ == 0 ) return;

	float clearColor[4] = { 0.7f, 0.8f, 1.0f, 1.0f };
	float redColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f };

	// clear the buffers
    d3dContext_->ClearRenderTargetView( renderTargetViewMap, redColor );
	d3dContext_->ClearRenderTargetView( backBufferTarget_, clearColor );				
    d3dContext_->ClearDepthStencilView( depthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	/// sets render target - NOT WORKING FOR SOME BLOODY REASON!
	d3dContext_->OMSetRenderTargets( 1, &renderTargetViewMap, depthStencilView_ );			

	if((gameState_ == RUN)||(gameState_== PAUSED) || (gameState_ == RES_MENU)) RenderPassOne();

	RenderPassThree();

	if(gameState_ == RUN){
		DrawUI(10, 35, 500 * playerHP/100, 50, hp_texture );
	}

	RenderPassTwo();

    swapChain_->Present( 0, 0 );
}
//George Anderson
/////draws a rectangle to screen
bool ModelsDemo::DrawRect()
{

	//////////////////
    D3D11_MAPPED_SUBRESOURCE mapResource;
    HRESULT d3dResult = d3dContext_->Map( textVertexBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to map resource!" );
        return false;
    }

    // Point to our vertex buffer's internal data.
    TextVertexPos *spritePtr = ( TextVertexPos* )mapResource.pData;



        spritePtr[0].pos = XMFLOAT3( -1.0f,   1.0f, 1.0f );
        spritePtr[1].pos = XMFLOAT3( 1.0f,   1.0f,   1.0f );
        spritePtr[2].pos = XMFLOAT3( -1.0f, -1.0f,   1.0f );
        spritePtr[3].pos = XMFLOAT3( 1.0f, 1.0f,   1.0f );
        spritePtr[4].pos = XMFLOAT3( 1.0f, -1.0f, 1.0f );
        spritePtr[5].pos = XMFLOAT3( -1.0f,   -1.0f, 1.0f );
		

        spritePtr[0].tex0 = XMFLOAT2( 0.0f, 0.0f );
        spritePtr[1].tex0 = XMFLOAT2( 1.0f, 0.0f );
        spritePtr[2].tex0 = XMFLOAT2( 0.0f, 1.0f );
        spritePtr[3].tex0 = XMFLOAT2( 1.0f, 0.0f );
        spritePtr[4].tex0 = XMFLOAT2( 1.0f, 1.0f );
        spritePtr[5].tex0 = XMFLOAT2( 0.0f, 1.0f );
    

    d3dContext_->Unmap( textVertexBuffer_, 0 );
    d3dContext_->Draw( 6, 0 );

    return true;
}

/////Models
bool ModelsDemo::RenderPassOne(){

	TurnOffAlphaBlending();
	TurnZBufferOn();

	XMMATRIX worldMat = XMMatrixIdentity( );
	worldMat = XMMatrixTranspose( worldMat );

	XMMATRIX viewMat = camera_.GetViewMatrix( );
	viewMat = XMMatrixTranspose( viewMat );

	XMFLOAT3 cameraPos = camera_.GetPosition( );

	//d3dContext_->UpdateSubresource( worldCB_, 0, 0, &worldMat, 0, 0 );
	d3dContext_->UpdateSubresource( viewCB_, 0, 0, &viewMat, 0, 0 );
	d3dContext_->UpdateSubresource( projCB_, 0, 0, &projMatrix_, 0, 0 );
	d3dContext_->UpdateSubresource( camPosCB_, 0, 0, &cameraPos, 0, 0 );

	//d3dContext_->VSSetConstantBuffers( 0, 1, &worldCB_ );
	d3dContext_->VSSetConstantBuffers( 1, 1, &viewCB_ );
	d3dContext_->VSSetConstantBuffers( 2, 1, &projCB_ );
	d3dContext_->VSSetConstantBuffers( 3, 1, &camPosCB_ );
	
	// ZS-->16/03/2015 -> draw knife
	//XMFLOAT3 k_pos( 25.0f, -12.5f, 50.0f );
	XMFLOAT3 k_pos( 25.0f, -12.5f, playerArmZ );
	XMFLOAT3 k_rot( 0.0f, -XM_PIDIV2 - (XM_PIDIV4 * 0.5f), 0.0f );
	XMFLOAT3 k_scale( 3.5f, 3.5f, 3.5f );
	//knife_display_object->DrawWeapon( d3dContext_, worldCB_, &camera_, k_pos, k_rot, k_scale );

	unsigned int some_id = 0;
	unsigned int some1_id = 1;
	if(!gunorknife)
		graphicsManager->GetAnimation( some_id )->GetCurrentFrame()->DrawWeapon( d3dContext_, worldCB_, &camera_, k_pos, k_rot, k_scale );
	if(gunorknife)
		graphicsManager->GetAnimation( some1_id )->GetCurrentFrame()->DrawWeapon( d3dContext_, worldCB_, &camera_, k_pos, k_rot, k_scale );
	// ZS-->22/03/2015 -> draw small enemy animation
	//small_enemy_run_ani->GetCurrentFrame()->Draw( d3dContext_, worldCB_, k_pos, k_rot, k_scale );
	// ALEX OWEN 20/04/15 Animated Game Object had it's own draw
	//small_enemy_run_ani->SetDisplayObject( small_enemy_run_ani->animator->GetCurrentFrame());
	//small_enemy_run_ani->Draw(d3dContext_, worldCB_);


	// ZS-->16/03/2015 -> draw moon
	skybox_game_object->Draw( d3dContext_, worldCB_ );

	// ALEX OWEN 20/04/15 - Draw bullets that have been fired
	for(int i = 0; i < numBullets; i ++)
	{
		if(bullets[i].fired)
			bullets[i].Draw(d3dContext_, worldCB_);
	}

	//HK
	for(int c = 0; c < Map->getObjCount(); c++)
		objects_game_object[c]->Draw( d3dContext_, worldCB_ );

	//if(small_enemy_game_object[0]->state == 0) small_enemy_game_object[0]->SetRotZ(XM_PI);

	for(int i = 0; i < MAX_ENEMY_OBJECTS; i++){
		if(small_enemy_game_object[i]->gethp() > 0){
			small_enemy_game_object[i]->setRotation(XMFLOAT3(0, RotateYToFace(small_enemy_game_object[i]->getPosition(), camera_.GetPosition()) + 1.57 ,0));
		}
		small_enemy_game_object[i]->Draw( d3dContext_, worldCB_ );
	}

	//// ZS-->02/03/2015 -> draw terrain
	//DrawGameObject( terrain_game_object );

	//// // MM---2015-02-16 
	//DrawGameObject( measure_game_object );

	DrawParticles();

	/////NORMAL MAPPING

	return true;
}

////Text
bool ModelsDemo::RenderPassTwo(){

	d3dContext_->OMSetRenderTargets( 1, &backBufferTarget_, NULL );
	
	TurnZBufferOff();
	TurnOnAlphaBlending();
	
	unsigned int stride = sizeof( TextVertexPos );
	unsigned int offset = 0;

	d3dContext_->IASetVertexBuffers( 0, 1, &textVertexBuffer_, &stride, &offset );
	d3dContext_->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	text_display_object->GetShader()->Render( d3dContext_ );
	text_display_object->GetTexture()->Render( d3dContext_ );

	if (gameState_ == RUN)
	{
		char output[50];
		sprintf_s(output, "Health:%.2f", playerHP);
		//DrawString( output, .3f, -0.9f);
	}

	if (gameState_ == START_MENU)
	{
		/////////////////////////////////////////////TEXT//////////////////////////////////////////////
		//DrawString( "PRESS ENTER to START", -0.4f, 0.0f );
		////////////////////////////////////////////////////////////////////////////////////////////////
		DrawString( "Main Menu", -0.25f, 0.0f );
		if (mainMenuSelection == LEVEL_ONE)
		{
			DrawString( "->Organic Zone<-", -0.33f, -0.1f );
		}
		else
		{
			DrawString( "Organic Zone", -0.25f, -0.1f );
		}

		if (mainMenuSelection == LEVEL_TWO)
		{
			DrawString( "->Urban Decay Zone<-", -0.33f, -0.2f );
		}
		else
		{
			DrawString( "Urban Decay Zone", -0.25f, -0.2f );
		}

		if (mainMenuSelection == LEVEL_THREE)
		{
			DrawString( "->Futuristic Zone<-", -0.33f, -0.3f );
		}
		else
		{
			DrawString( "Futuristic Zone", -0.25f, -0.3f );
		}

		if (mainMenuSelection == EXIT)
		{
			DrawString( "->Exit<-", -0.33f, -0.4f );
		}
		else
		{
			DrawString( "Exit", -0.25f, -0.4f );
		}


	}

	if((gameState_ == RUN)&&(displayFPS==true))
	{
		/////////////////////////////////////////////TEXT//////////////////////////////////////////////

		char output[50];
		sprintf_s(output, "FPS:%d", fps_);

		DrawString( output, -0.9f, 0.9f);

		sprintf_s(output, "Frame Time:%.6f", frameTime_);

		DrawString( output, -0.9f, 0.8f);
		////////////////////////////////////////////////////////////////////////////////////////////////

		//ALEX OWEN - 16/02/15 - testing screen outputs
		float dist = DistanceBetweenPoints(camera_.GetPosition(),XMFLOAT3(0,0,0));
		//tank[0].setVelocity(XMVectorSet(3,1,2,1));
		//XMFLOAT3 norm = tank[0].getVelocityDirection();
		sprintf_s( output, "dist: %.6f", dist);
		//DrawString(output, -0.9f, 0.7f);

		sprintf_s( output, "x.%f z.%f", camera_.GetPosition().x, camera_.GetPosition().z);
		//DrawString(output, -0.9f, 0.6f);

		sprintf_s( output, "x.%f z.%f", small_enemy_game_object[0]->getX(), small_enemy_game_object[0]->getZ());
		//DrawString(output, -0.9f, 0.5f);

		sprintf_s( output, "%.2f", small_enemy_game_object[0]->gethp());
		//DrawString(output, -0.9f, 0.4f);

	}

	if (gameState_ == PAUSED)
	{
		/////////////////////////////////////////////TEXT//////////////////////////////////////////////

		DrawString( "GAME PAUSED", -0.2f, 0.0f );
		if (pauseMenuSelection == RETURN)
		{
			DrawString( "->Return to Game<-", -0.33f, -0.1f );
		}
		else
		{
			DrawString( "Return to Game", -0.25f, -0.1f );
		}

		if (pauseMenuSelection == FPS)
		{
			if(displayFPS==true)
			{
				DrawString( "->Display FPS: ON<-",-0.35f, -0.2f );
			}
			else
			{
				DrawString( "->Display FPS:OFF<-",-0.35f, -0.2f );
			}
		}
		else
		{
			if(displayFPS==true)
			{
				DrawString( "Display FPS: ON",-0.27f, -0.2f );
			}
			else
			{
				DrawString( "Display FPS:OFF",-0.27f, -0.2f );
			}
		}
		
		if (pauseMenuSelection == PLAY_MOVIE)
		{
			DrawString( "->Play the Movie<-", -0.33f, -0.3f );
		}
		else
		{
			DrawString( "Play the Movie", -0.25f, -0.3f );
		}


		///fullscreen


		if (pauseMenuSelection == FULLSCREEN)
		{
			if(FullScreenState == true){
				DrawString( "->FullScreen: On<-", -0.33f, -0.4f );
			}
			else{
				DrawString( "->FullScreen: Off<-", -0.33f, -0.4f );
			}

		}
		else
		{
			if(FullScreenState == true){
				DrawString( "FullScreen: On", -0.25f, -0.4f );
			}
			else{
				DrawString( "FullScreen: Off", -0.25f, -0.4f );
			}
		}



		if (pauseMenuSelection == RESOLUTION_MENU)
		{
			DrawString( "->Resolution Menu<-", -0.33f, -0.5f );
		}
		else
		{
			DrawString( "Resolution Menu", -0.25f, -0.5f );
		}


		if (pauseMenuSelection == QUIT)
		{
			DrawString( "->Quit the Game<-", -0.33f, -0.6f );
		}
		else
		{
			DrawString( "Quit the Game", -0.25f, -0.6f );
		}

		////////////////////////////////////////////////////////////////////////////////////////////////
	}


	if (gameState_ == RES_MENU)
	{


		if (resolutionMenuSelection == BACK)
		{
			DrawString( "->Back<-", -0.33f, -0.1f );
		}
		else
		{
			DrawString( "Back", -0.25f, -0.1f );
		}


		if (resolutionMenuSelection == OPTA)
		{
			DrawString( "->800 x 600: <-", -0.33f, -0.2f );
		}
		else
		{
			DrawString( "800 x 600", -0.25f, -0.2f );
		}

		if (resolutionMenuSelection == OPTB)
		{
			DrawString( "->1024 x 768: <-", -0.33f, -0.3f );
		}
		else
		{
			DrawString( "1024 x 768", -0.25f, -0.3f );
		}

		if (resolutionMenuSelection == OPTC)
		{
			DrawString( "->1280 x 960: <-", -0.33f, -0.4f );
		}
		else
		{
			DrawString( "1280 x 960", -0.25f, -0.4f );
		}

		if (resolutionMenuSelection == OPTD)
		{
			DrawString( "->1600 x 900: <-", -0.33f, -0.5f );
		}
		else
		{
			DrawString( "1600 x 900", -0.25f, -0.5f );
		}

		if (resolutionMenuSelection == OPTE)
		{
			DrawString( "->1920 x 1080: <-", -0.33f, -0.6f );
		}
		else
		{
			DrawString( "1920 x 1080", -0.25f, -0.6f );
		}


	}


	
	TurnOffAlphaBlending();
	TurnZBufferOn();

	return true;
}

////Rectangle for secondary target view
bool ModelsDemo::RenderPassThree()
{
	d3dContext_->OMSetRenderTargets( 1, &backBufferTarget_, NULL );

	TurnZBufferOff();
	TurnOnAlphaBlending();
	
	unsigned int stride = sizeof( TextVertexPos );
	unsigned int offset = 0;

	d3dContext_->IASetInputLayout( textInputLayout_ );
	d3dContext_->IASetVertexBuffers( 0, 1, &textVertexBuffer_, &stride, &offset );
	d3dContext_->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	d3dContext_->VSSetShader( textTextureMapVS_, 0, 0 );
	d3dContext_->PSSetShader( textTextureMapPS_, 0, 0 );

	//d3dContext_->ClearRenderTargetView( renderTargetViewMap, redColor );	/////
	d3dContext_->PSSetShaderResources( 0, 1, &shaderResourceViewMap  );
    d3dContext_->PSSetSamplers( 0, 1, &textColorMapSampler_ );
	d3dContext_->IASetVertexBuffers( 0, 1, &textVertexBuffer_, &stride, &offset );
	//d3dContext_->IASetVertexBuffers( 0, 1, &vertexBufferRect_, &stride, &offset );
	//d3dContext_->Draw( 6, 0 );
	DrawRect();

	return true;
}

void ModelsDemo::DrawParticles(){//GEORGE ANDERSON

	d3dContext_->IASetInputLayout( inputLayout_ );
	d3dContext_->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	d3dContext_->VSSetShader( colourMapVS_, 0, 0 );
	d3dContext_->PSSetShader( colourMapPS_, 0, 0 );

	DrawAllParticles(d3dContext_, camera_.GetPosition());
}

void ModelsDemo::UpdateWidthandHeight(int width, int height){ //GA
	MyWidth = width;
	MyHeight = height;

	projMatrix_ = XMMatrixPerspectiveFovLH( XM_PIDIV4, MyWidth / MyHeight, 1.0f, 1000000.0 );
    projMatrix_ = XMMatrixTranspose( projMatrix_ );

}

void ModelsDemo::DrawUI(float x, float y, float w , float h, Texture* imgtouse)
{
	float screenX = ((2 * x) / 800)-1.0;
	float screenY = ((2 * y) / 600)-1.0;
	float screenW = ((2 * x+w) / 800)-1.0;
	float screenH = ((2 * y-h) / 600)-1.0;

	d3dContext_->OMSetRenderTargets( 1, &backBufferTarget_, NULL );

	TurnZBufferOff();
	TurnOnAlphaBlending();
	
	unsigned int stride = sizeof( TextVertexPos );
	unsigned int offset = 0;

	d3dContext_->IASetInputLayout( textInputLayout_ );
	d3dContext_->IASetVertexBuffers( 0, 1, &textVertexBuffer_, &stride, &offset );
	d3dContext_->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	d3dContext_->VSSetShader( textTextureMapVS_, 0, 0 );
	d3dContext_->PSSetShader( textTextureMapPS_, 0, 0 );
	imgtouse->Render(d3dContext_);
    d3dContext_->PSSetSamplers( 0, 1, &textColorMapSampler_ );
	d3dContext_->IASetVertexBuffers( 0, 1, &textVertexBuffer_, &stride, &offset );

	//////////////////
    D3D11_MAPPED_SUBRESOURCE mapResource;
    HRESULT d3dResult = d3dContext_->Map( textVertexBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource );

    // Point to our vertex buffer's internal data.
    TextVertexPos *spritePtr = ( TextVertexPos* )mapResource.pData;

    spritePtr[0].pos = XMFLOAT3( screenX, screenY, 1.0f );
    spritePtr[1].pos = XMFLOAT3( screenW, screenY, 1.0f );
    spritePtr[2].pos = XMFLOAT3( screenX, screenH, 1.0f );
    spritePtr[3].pos = XMFLOAT3( screenW, screenY, 1.0f );
    spritePtr[4].pos = XMFLOAT3( screenW, screenH, 1.0f );
    spritePtr[5].pos = XMFLOAT3( screenX, screenH, 1.0f );
	
    spritePtr[0].tex0 = XMFLOAT2( 0.0f, 0.0f );
    spritePtr[1].tex0 = XMFLOAT2( 1.0f, 0.0f );
    spritePtr[2].tex0 = XMFLOAT2( 0.0f, 1.0f );
    spritePtr[3].tex0 = XMFLOAT2( 1.0f, 0.0f );
    spritePtr[4].tex0 = XMFLOAT2( 1.0f, 1.0f );
    spritePtr[5].tex0 = XMFLOAT2( 0.0f, 1.0f );

    d3dContext_->Unmap( textVertexBuffer_, 0 );
    d3dContext_->Draw( 6, 0 );

}

