#include "Precompiled/Precompiled.h"
#include "AppData.h"

#include "RepositoryItemTypes.h"
#include "AssetLoading/JsonFileSystem.h"
#include "Rendering/FrameBuilder.h"
#include "Rendering/Defaults.h"
#include "Rendering/Texture.h"
#include "Rendering/GfxDeviceFactory.h"

#include <filesystem>
#include <cassert>

BEGIN_NAMESPACE1( appdata )

namespace {
	using namespace assets;

	static constexpr const char* DEFAULT_DATA_PATH = "data//";
	static constexpr const char* DEFAULT_MODELS_PATH = "data//models//";
	static constexpr const char* DEFAULT_TEXTURES_PATH = "data//textures//";
	static constexpr const char* DEFAULT_MESHES_PATH = "data//meshes//";
	static constexpr const char* DEFAULT_SHADERS_PATH = "data//shaders//";
	static constexpr const char* DEFAULT_SCENES_PATH = "data//scenes//";
	static constexpr const char* DEFAULT_SKYBOXES_PATH = "data//skyboxes//";

	static constexpr const char* SETTINGS_FILE = "data//Settings.json";
	static constexpr const char* SHADER_SOURCE_FILE = "data//ShaderSources.json";
	static constexpr const char* RENDER_GRAPH_FILE = "data//RenderGraph.json";
	static constexpr const char* FRAME_RESOURCES_FILE = "data//RenderResources.json";
	static constexpr const char* RENDER_PASS_LIST_FILE = "data//RenderPassList.json";
	static constexpr const char* RENDER_FEATURE_LIST_FILE = "data//RenderFeatureLists.json";
	static constexpr const char* SKYBOX_LIST_FILE = "data//skyboxes//SkyboxList.json";

	static constexpr const char* SCENE_LIST_FILE = "data//scenes//scenes.json";

	static constexpr const char* JSON_EXTENSION = ".json";
	static constexpr const char* OBJ_EXTENSION = ".obj";


	// ============== DEFAULTS ============== //

	AppDefaults getAppDefaults() {
		return AppDefaults{
			.folders = {
				DEFAULT_DATA_PATH,
				DEFAULT_MODELS_PATH,
				DEFAULT_MESHES_PATH,
				DEFAULT_TEXTURES_PATH,
				DEFAULT_SHADERS_PATH,
				DEFAULT_SCENES_PATH,
				DEFAULT_SKYBOXES_PATH
			},
			.files = {
				SETTINGS_FILE,
				SHADER_SOURCE_FILE,
				RENDER_GRAPH_FILE,
				FRAME_RESOURCES_FILE,
				RENDER_PASS_LIST_FILE,
				RENDER_FEATURE_LIST_FILE,
				SKYBOX_LIST_FILE
			}
		};
	}

	std::span<std::string> getDefaultModels() {
		static std::vector<std::string> models
		{
			DEFAULT_MODELS_PATH + std::string{ "cube.obj" },
			DEFAULT_MODELS_PATH + std::string{ "sphere.obj" },
			DEFAULT_MODELS_PATH + std::string{ "quad.obj" },
		};

		return { models.data(), models.size() };
	}

	// ! ============== DEFAULTS ============== //

	static std::string fullShaderPath( const std::string& shaderSourceFile ) {
		return DEFAULT_SHADERS_PATH + shaderSourceFile;
	}

	// ============== CACHE ============== //

	std::vector<ShaderSource> _shaderSources;
	std::vector<TextureManifest> _skyBoxManifests;
	std::vector<std::string> _modelPaths;
	std::vector<std::string> _meshPaths;
	std::vector<std::string> _texturePaths;
	std::vector<std::string> _scenePaths;
	RenderGraphJson _renderGraph;
	std::vector<RenderResourcesJson> _renderResources;
}

AppData::AppData() {}

AppData::~AppData() {
	saveUserSettings();
}

void AppData::createDefaults() {
	auto k = getDefaultModels();
	UNUSED_VAR( k );

	AppDefaults appDefaults = getAppDefaults();

	for (const auto& path : appDefaults.folders) {
		if (!std::filesystem::exists( path )) {
			std::filesystem::create_directories( path );
		}
	}

	// Default settings
	{
		Settings defaultSettings
		{
			DEFAULT_MODELS_PATH,
			DEFAULT_TEXTURES_PATH,
			DEFAULT_MESHES_PATH,
			SHADER_SOURCE_FILE,
			DEFAULT_SCENES_PATH,
			""
		};

		JsonFileSystem::SaveAsJson<Settings>( defaultSettings, SETTINGS_FILE );
	}

	// Default shader sources
	{
		std::vector<ShaderSource> shaderSources
		{
			ShaderSource{ shader::LAMBERTIAN,
				fullShaderPath( "model_view_projection_vert.spv" ),
				fullShaderPath( "lambertian_frag.spv" ) },
			ShaderSource{ shader::LAMBERTIAN_TRANSPARENT,
				fullShaderPath( "model_view_projection_vert.spv" ),
				fullShaderPath( "lambertian_frag.spv" ) },
			ShaderSource{ shader::LIGHTS,
				fullShaderPath( "model_view_projection_vert.spv" ),
				fullShaderPath( "single_color_frag.spv" ) },
			ShaderSource{ shader::SINGLE_COLOR_BORDER,
				fullShaderPath( "model_view_projection_scaled_vert.spv" ),
				fullShaderPath( "single_color_frag.spv" ) },
			ShaderSource{ shader::LAMBERTIAN_INSTANCED,
				fullShaderPath( "model_view_projection_instanced_vert.spv" ),
				fullShaderPath( "sprite_default_frag.spv" ) },
			ShaderSource{ shader::FULL_SCREEN,
				fullShaderPath( "full_screen_vert.spv" ),
				fullShaderPath( "full_screen_frag.spv" ) },
			ShaderSource{ shader::FULL_SCREEN_COLOR_INVERTED,
				fullShaderPath( "full_screen_vert.spv" ),
				fullShaderPath( "full_screen_inverted_color_frag.spv" ) },
			ShaderSource{ shader::FULL_SCREEN_BLUR_VERTICAL,
				fullShaderPath( "full_screen_vert.spv" ),
				fullShaderPath( "full_screen_blur_vertical_frag.spv" ) },
			ShaderSource{ shader::FULL_SCREEN_BLUR_HORIZONTAL,
				fullShaderPath( "full_screen_vert.spv" ),
				fullShaderPath( "full_screen_blur_horizontal_frag.spv" ) },
			ShaderSource{ shader::FULL_SCREEN_PIXELATED,
				fullShaderPath( "full_screen_vert.spv" ),
				fullShaderPath( "full_screen_pixelated_frag.spv" ) },
			ShaderSource{ shader::FULL_SCREEN_CHROME_ABERRATION,
				fullShaderPath( "full_screen_vert.spv" ),
				fullShaderPath( "full_screen_chrome_aberrated_frag.spv" ) },
			ShaderSource{ shader::FULL_SCREEN_COLOR_CORRECTION,
				fullShaderPath( "full_screen_vert.spv" ),
				fullShaderPath( "full_screen_color_corrected_frag.spv" ) },
			ShaderSource{ shader::SKYBOX,
				fullShaderPath( "skybox_vert.spv" ),
				fullShaderPath( "skybox_frag.spv" ) },
				// TODO add NULL_SHADER - pink shader
			ShaderSource{ shader::NULL_SHADER,
				fullShaderPath( "model_view_projection_vert.spv" ),
				fullShaderPath( "error_frag.spv" ) },
		};
		JsonFileSystem::SaveAsJson<std::vector<ShaderSource>>( shaderSources, SHADER_SOURCE_FILE );
	}

	// Default render graph and frame resources
	{
		RenderGraphJson queueJsons = rendering::Defaults::buildGraphJson();
		std::vector<RenderResourcesJson> resourcesJson = rendering::Defaults::buildResourcesJson();

		rendering::Defaults::validateBuild();

		assets::JsonFileSystem::SaveAsJson( resourcesJson, FRAME_RESOURCES_FILE );
		assets::JsonFileSystem::SaveAsJson( queueJsons, RENDER_GRAPH_FILE );
	}

	// Default render feature list
	{
		std::span<std::string> renderFeatures = rendering::Defaults::buildFeatureNameList();
		JsonFileSystem::SaveAsJson<std::span<std::string>>( renderFeatures, RENDER_FEATURE_LIST_FILE );
	}

	// Default render pass list
	{
		std::span<std::string> renderPasses = rendering::Defaults::buildPassNameList();
		JsonFileSystem::SaveAsJson<std::span<std::string>>( renderPasses, RENDER_PASS_LIST_FILE );
	}

	// Validate
	for (const auto& path : appDefaults.files) {
		if (!std::filesystem::exists( path )) {
			throw std::runtime_error( "File does not exist: " + path );
		}
	}
}

void AppData::initialize() {
	AppDefaults appDefaults = getAppDefaults();

	for (const auto& path : appDefaults.folders) {
		if (!std::filesystem::exists( path )) {
			throw std::runtime_error( "Folder does not exist: " + path );
		}
	}

	if (!std::filesystem::exists( SETTINGS_FILE )) {
		createDefaults();
	}

	Settings defaultSettings{};
	JsonFileSystem::LoadFromJson<Settings>( defaultSettings, SETTINGS_FILE );
	_settings = std::make_unique<Settings>( defaultSettings );
	

	auto scenePath = getScenesPath();
	for (const auto& path : std::filesystem::directory_iterator( scenePath )) {
		if (path.path().extension() == JSON_EXTENSION)
			_scenePaths.push_back( path.path().string() );
	}

	auto meshPath = _settings->meshPath;
	for (const auto& path : std::filesystem::directory_iterator( meshPath )) {
		if (path.path().extension() == OBJ_EXTENSION)
			_meshPaths.push_back( path.path().string() );
	}

	auto texturePaths = _settings->texturePath;
	for (const auto& path : std::filesystem::directory_iterator( texturePaths )) {
		if (std::filesystem::is_directory( path ))
			continue;

		_texturePaths.push_back( path.path().string() );
	}

	auto modelPath = _settings->modelPath;
	for (const auto& path : std::filesystem::directory_iterator( modelPath )) {
		if (path.path().extension() == JSON_EXTENSION)
			_modelPaths.push_back( path.path().string() );
	}

	JsonFileSystem::LoadFromJson<std::vector<ShaderSource>>( _shaderSources, SHADER_SOURCE_FILE );

	assets::JsonFileSystem::LoadFromJson( _renderGraph, RENDER_GRAPH_FILE );
	if (_renderGraph.queues.empty())
		throw;

	assets::JsonFileSystem::LoadFromJson( _renderResources, FRAME_RESOURCES_FILE );
	if (_renderResources.empty())
		throw;

	//JsonFileSystem::LoadFromJson<std::vector<TextureManifest>>( _skyBoxManifests, SKYBOX_LIST_FILE );
}

const Settings& AppData::getSettings() {
	assert( _settings != nullptr );
	return *_settings.get();
}

void AppData::saveUserSettings() {
	JsonFileSystem::SaveAsJson<Settings>( *_settings, SETTINGS_FILE );
}

Result<std::string> AppData::getLastScenePath() const
{
	if (_settings->lastScenePath.empty())
		return Result<std::string>::failed();

	return Result<std::string>::ok( _settings->lastScenePath );
}

void AppData::setLastSceneName( const std::string& sceneName )
{
	_settings->lastScenePath = sceneName;
}

const std::string& AppData::getScenesPath() const {
	static std::string path = DEFAULT_SCENES_PATH;
	return path;
}

const std::string& AppData::getSceneListFilePath() const {
	static std::string path = SCENE_LIST_FILE;
	return path;
}

const std::string& AppData::getScenePath( const std::string& sceneName ) const {
	for (const auto& path : _scenePaths) {
		bool found = path.find( sceneName ) != std::string::npos;
		if (found) {
			return path;
		}
	}

	// create new
	std::string scenePath = DEFAULT_SCENES_PATH + sceneName;
	_scenePaths.push_back( scenePath );

	return _scenePaths.back();
}

std::span<std::string> AppData::getScenePaths() const {
	return std::span<std::string>{ _scenePaths.data(), _scenePaths.size() };
}

std::span<std::string> AppData::getMeshPaths() const {
	return std::span<std::string>{ _meshPaths.data(), _meshPaths.size() };
}

std::span<std::string> AppData::getTexturePaths() const {
	return std::span<std::string>{ _texturePaths.data(), _texturePaths.size() };
}

std::span<std::string> AppData::getModelPaths() const {
	return std::span<std::string>{ _modelPaths.data(), _modelPaths.size() };
}

std::span<ShaderSource> AppData::getShaderSources() const {
	return std::span<ShaderSource>{ _shaderSources.data(), _shaderSources.size() };
}

std::span<TextureManifest> AppData::getSkyBoxManifests() const {
	return std::span<TextureManifest>{_skyBoxManifests.data(), _skyBoxManifests.size()};
}

const RenderGraphJson& AppData::getRenderGraph() const {
	return _renderGraph;
}

const std::vector<RenderResourcesJson>& AppData::getRenderResources() const {
	return _renderResources;
}

END_NAMESPACE1