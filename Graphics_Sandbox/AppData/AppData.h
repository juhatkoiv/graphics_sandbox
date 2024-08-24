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

class AppData
{
public:
	AppData();
	~AppData();

	void createDefaults();
	void initialize();

	const Settings& getSettings();
	void saveUserSettings();

	Result<std::string> getLastScenePath() const;
	void setLastSceneName( const std::string& sceneName );

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
	std::unique_ptr<Settings> _settings;
};

END_NAMESPACE1