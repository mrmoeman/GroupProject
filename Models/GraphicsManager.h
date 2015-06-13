#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include "DisplayObject.h"
#include "Animation.h"

#include <unordered_map>

/*

	The GraphicsManager handles safe initialization and creation of assets implementing a time-locking mechanic. 
	It stores its internal data in std::unordered_map containers that use unsigned ints as keys.
	A GraphicsManager works best with an organised asset system that uses an unsigned ints in an enum.

	IMPORTANT: 
	
	1) ->	Always initialise a GraphicsManager object before adding assets to it
	2) ->	Add assets in the following order only: Textures, Models, DisplayObjects, Animations
	3) ->	Before loading a different type of asset, make sure to lock the previous type of asset loaded
			to avoid invalidated pointers

	for example...

		// load all textures

		// lock textures ( textures can be used safely )

		// load all models

		// lock models ( models can be used safely )

		// load all display objects

		// lock display objects ( display objects can be used safely )

		// load all animations

		// lock animations ( animations can be used safely )

	4) ->	Free all the memeory in a GraphicsManager using Release() before it gets destroyed

	NOTE: Currently the shaders are hardcoded into the class due to the nature of the Shader classes

*/

class GraphicsManager
{
	bool locked_textures, locked_models, locked_display_objects, locked_animations;

	Shader2D		*m_shader2D;
	ShaderVTN		*m_shaderVTN;
	ShaderNMAP		*m_shaderNMAP;
	ID3D11Device	*m_device;

	std::unordered_map<unsigned int,Texture*>			m_textures;
	std::unordered_map<unsigned int,Model*>				m_models;
	std::unordered_map<unsigned int,DisplayObject*>		m_display_objects;
	std::unordered_map<unsigned int,Animation*>			m_animations;

public:

	void Init(ID3D11Device* device);

	GraphicsManager(void);
	~GraphicsManager(void);

	void Release();

	// setters

	void LockTextures();
	void LockModels();
	void LockDisplayObjects();
	void LockAnimations();

	bool AddTexture( unsigned int texture_id , char *_file );

	bool AddModel2D( unsigned int model_id , char *_file );
	bool AddModelVTN( unsigned int model_id , char *_file );
	bool AddModelNMAP( unsigned int model_id , char *_file );

	bool AddDisplayObject2D(  unsigned int display_object_id, unsigned int texture_id , unsigned int model_id );
	bool AddDisplayObjectVTN(  unsigned int display_object_id, unsigned int texture_id , unsigned int model_id );
	bool AddDisplayObjectNMAP(  unsigned int display_object_id, unsigned int texture_id , unsigned int model_id, unsigned int texture_normal_map_id );
	bool AddAnimation( unsigned int animation_id );

	// getters

	Texture * GetTexture( unsigned int texture_id );
	Model * GetModel( unsigned int model_id );
	DisplayObject * GetDisplayObject( unsigned int display_object_id );
	Animation * GetAnimation( unsigned int animation_id );

};

