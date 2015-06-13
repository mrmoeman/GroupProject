#include "GraphicsManager.h"

GraphicsManager::GraphicsManager(void)
{
	m_shader2D = NULL;
	m_shaderVTN = NULL;
	m_shaderNMAP = NULL;
	m_device = NULL;

	locked_textures = false;
	locked_models = true;
	locked_display_objects = true;
	locked_animations = true;
}

GraphicsManager::~GraphicsManager(void)
{
}

void GraphicsManager::Init(ID3D11Device* device)
{
	if (m_device != NULL) return;

	m_device = device;

	m_shader2D = new Shader2D();
	m_shader2D->Init( m_device );

	m_shaderVTN = new ShaderVTN();
	m_shaderVTN->Init( m_device );

	m_shaderNMAP = new ShaderNMAP();
	m_shaderNMAP->Init( m_device );
}

void GraphicsManager::Release()
{
	m_animations.clear();
	m_display_objects.clear();

	// loop through the models
	std::unordered_map<unsigned int,Model*>::iterator m_it = m_models.begin();
	while ( m_it != m_models.end() )
	{
		m_it->second->Release();
		delete m_it->second;
		m_it++;
	}
	m_models.clear();

	// loop through the textures
	std::unordered_map<unsigned int,Texture*>::iterator t_it = m_textures.begin();
	while ( t_it != m_textures.end() )
	{
		t_it->second->Release();
		delete t_it->second;
		t_it++;
	}
	m_textures.clear();

	m_shader2D->Release(); m_shader2D = NULL;
	m_shaderVTN->Release(); m_shaderVTN = NULL;
	m_shaderNMAP->Release(); m_shaderNMAP = NULL;
	m_device = NULL;

	locked_textures = true;
	locked_models = true;
	locked_display_objects = true;
	locked_animations = true;
}

void GraphicsManager::LockTextures()
{
	if ( !locked_textures && locked_models && locked_display_objects && locked_animations )
	{
			locked_textures = true;
			locked_models = false;
	}
}

void GraphicsManager::LockModels()
{
	if ( locked_textures && !locked_models && locked_display_objects && locked_animations )
	{
		locked_models = true;
		locked_display_objects = false;
	}
}
	
void GraphicsManager::LockDisplayObjects()
{
	if ( locked_textures && locked_models && !locked_display_objects && locked_animations )
	{
		locked_display_objects = true;
		locked_animations = false;
	}
}

void GraphicsManager::LockAnimations()
{
	if ( locked_textures && locked_models && locked_display_objects && !locked_animations )
	{
		locked_animations = true;
	}
}

bool GraphicsManager::AddTexture( unsigned int texture_id , char * file )
{
	// make sure the asset type is not locked
	if ( locked_textures ) return false;

	// make sure the ID doesn't exist already
	std::unordered_map<unsigned int,Texture*>::iterator found = m_textures.find( texture_id );
	if ( found != m_textures.end() ) return false;

	// make sure the object can be created
	Texture * texture = new Texture();
	if ( !texture->Init( m_device, file ) )
	{
		delete texture;
		return false;
	}

	// move the object to the unordered map
	m_textures.insert ( std::make_pair<unsigned int, Texture*>( texture_id, texture ));
	return true;
}

bool GraphicsManager::AddModel2D( unsigned int model_id , char *file )
{
	// make sure the asset type is not locked
	if ( locked_models ) return false;

	// make sure the ID doesn't exist already
	std::unordered_map<unsigned int,Model*>::iterator found = m_models.find( model_id );
	if ( found != m_models.end() ) return false;

	// make sure the object can be created
	Model * model = new Model2D();
	if ( !model->Init( m_device, file ) )
	{
		delete model;
		return false;
	}

	// save the object to the unordered map
	m_models.insert ( std::make_pair<unsigned int, Model*>( model_id, model ));
	return true;
}

bool GraphicsManager::AddModelVTN( unsigned int model_id , char *file )
{
	// make sure the asset type is not locked
	if ( locked_models ) return false;

	// make sure the ID doesn't exist already
	std::unordered_map<unsigned int,Model*>::iterator found = m_models.find( model_id );
	if ( found != m_models.end() ) return false;

	// make sure the object can be created
	Model * model = new ModelVTN();
	if ( !model->Init( m_device, file ) )
	{
		delete model;
		return false;
	}

	// save the object to the unordered map
	m_models.insert ( std::make_pair<unsigned int, Model*>( model_id, model ));
	return true;
}

bool GraphicsManager::AddModelNMAP( unsigned int model_id , char *file )
{
	// make sure the asset type is not locked
	if ( locked_models ) return false;

	// make sure the ID doesn't exist already
	std::unordered_map<unsigned int,Model*>::iterator found = m_models.find( model_id );
	if ( found != m_models.end() ) return false;

	// make sure the object can be created
	Model * model = new ModelNMAP();
	if ( !model->Init( m_device, file ) )
	{
		delete model;
		return false;
	}

	// save the object to the unordered map
	m_models.insert ( std::make_pair<unsigned int, Model*>( model_id, model ));
	return true;
}

bool GraphicsManager::AddDisplayObject2D(  unsigned int display_object_id, unsigned int texture_id , unsigned int model_id )
{
	// make sure the asset type is not locked
	if ( locked_display_objects ) return false;

	// make sure the ID doesn't exist already
	std::unordered_map<unsigned int,DisplayObject*>::iterator found = m_display_objects.find( display_object_id );
	if ( found != m_display_objects.end() ) return false;

	// make sure the texture exists
	Texture * texture = GetTexture( texture_id );
	if ( texture == NULL ) return false;

	// make sure the model exists
	Model * model = GetModel( model_id );
	if ( model == NULL ) return false;

	// make sure the object can be created
	DisplayObject * display_object = new DisplayObject();
	if ( !display_object->Init( model, texture, m_shader2D ) )
	{
		delete display_object;
		return false;
	}

	// save the object to the unordered map
	m_display_objects.insert ( std::make_pair<unsigned int, DisplayObject*>( display_object_id, display_object ));
	return true;
}

bool GraphicsManager::AddDisplayObjectVTN(  unsigned int display_object_id, unsigned int texture_id , unsigned int model_id )
{
	// make sure the asset type is not locked
	if ( locked_display_objects ) return false;

	// make sure the ID doesn't exist already
	std::unordered_map<unsigned int,DisplayObject*>::iterator found = m_display_objects.find( display_object_id );
	if ( found != m_display_objects.end() ) return false;

	// make sure the texture exists
	Texture * texture = GetTexture( texture_id );
	if ( texture == NULL ) return false;

	// make sure the model exists
	Model * model = GetModel( model_id );
	if ( model == NULL ) return false;

	// make sure the object can be created
	DisplayObject * display_object = new DisplayObject();
	if ( !display_object->Init( model, texture, m_shaderVTN ) )
	{
		delete display_object;
		return false;
	}

	// save the object to the unordered map
	m_display_objects.insert ( std::make_pair<unsigned int, DisplayObject*>( display_object_id, display_object ));
	return true;
}

bool GraphicsManager::AddDisplayObjectNMAP(  unsigned int display_object_id, unsigned int texture_id , unsigned int model_id, unsigned int texture_normal_map_id )
{
	// make sure the asset type is not locked
	if ( locked_display_objects ) return false;

	// make sure the ID doesn't exist already
	std::unordered_map<unsigned int,DisplayObject*>::iterator found = m_display_objects.find( display_object_id );
	if ( found != m_display_objects.end() ) return false;

	// make sure the texture exists
	Texture * texture = GetTexture( texture_id );
	if ( texture == NULL ) return false;

	// make sure the model exists
	Model * model = GetModel( model_id );
	if ( model == NULL ) return false;

	// make sure the normal map exists
	Texture * normal_map = GetTexture( texture_normal_map_id );
	if ( normal_map == NULL ) return false;

	// make sure the object can be created
	DisplayObject * display_object = new DisplayObject();
	if ( !display_object->Init( model, texture, m_shaderNMAP, normal_map ) )
	{
		delete display_object;
		return false;
	}

	// save the object to the unordered map
	m_display_objects.insert ( std::make_pair<unsigned int, DisplayObject*>( display_object_id, display_object ));
	return true;
}

bool GraphicsManager::AddAnimation( unsigned int animation_id )
{
	// make sure the asset type is not locked
	if ( locked_animations ) return false;

	// make sure the ID doesn't exist already
	std::unordered_map<unsigned int,Animation*>::iterator found = m_animations.find( animation_id );
	if ( found != m_animations.end() ) return false;

	// make sure the object can be created
	Animation * animation = new Animation();

	// save the object to the unordered map
	m_animations.insert ( std::make_pair<unsigned int, Animation*>( animation_id, animation ));
	return true;
}

Texture * GraphicsManager::GetTexture( unsigned int texture_id )
{
	// check that the id for said object already exists using the key
	std::unordered_map<unsigned int,Texture*>::iterator found = m_textures.find( texture_id );
	if ( found == m_textures.end() ) return NULL;

	// return the object at the key's position
	return found->second;
}

Model * GraphicsManager::GetModel( unsigned int model_id )
{
	// check that the id for said object already exists using the key
	std::unordered_map<unsigned int,Model*>::iterator found = m_models.find( model_id );
	if ( found == m_models.end() ) return NULL;

	// return the object at the key's position
	return found->second;
}

DisplayObject * GraphicsManager::GetDisplayObject( unsigned int display_object_id )
{
	// check that the id for said object already exists using the key
	std::unordered_map<unsigned int,DisplayObject*>::iterator found = m_display_objects.find( display_object_id );
	if ( found == m_display_objects.end() ) return NULL;

	// return the object at the key's position
	return found->second;
}

Animation * GraphicsManager::GetAnimation( unsigned int animation_id )
{
	// check that the id for said object already exists using the key
	std::unordered_map<unsigned int,Animation*>::iterator found = m_animations.find( animation_id );
	if ( found == m_animations.end() ) return NULL;

	// return the object at the key's position
	return found->second;
}