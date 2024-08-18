#pragma once

#include <string>
#include <memory>
#include <span>

DECLARE1( struct, appdata, ShaderSource );
DECLARE1( struct, appdata, Settings );
DECLARE1( struct, appdata, TextureManifest );
DECLARE( struct, RenderGraphJson );
DECLARE( struct, RenderResourcesJson );

BEGIN_NAMESPACE1( appdata )

class ConfigRepository
{
public:
	ConfigRepository();
	~ConfigRepository();

	void createDefaults();
	void initialize();

	const Settings& getUserSettings();
	const Settings& getDefaultSettings();

	void saveUserSettings( const Settings& settings );
	const std::string& getScenesPath() const;
	const std::string& getSceneListFilePath() const;
	const std::string& getScenePath( const std::string& sceneName ) const;
	std::span<std::string> getScenePaths() const;
	std::span<std::string> getMeshPaths() const;
	std::span<std::string> getTexturePaths() const;
	std::span<std::string> getModelPaths() const;
	std::span<ShaderSource> getShaderSources() const;
	std::span<TextureManifest> getSkyBoxManifests() const;

	const RenderGraphJson& getRenderGraph() const;
	const std::vector<RenderResourcesJson>& getRenderResources() const;

private:
	std::unique_ptr<Settings> _userSettings;
	std::unique_ptr<Settings> _defaultSettings;
};

END_NAMESPACE1