#include <Greet.h>

#include "EditorScene.h"

class Editor : public Greet::App
{
  private:
    Greet::Ref<Greet::GUIScene> gui;
    std::map<std::string, Greet::Ref<Greet::ECSScene>> scenes;

    // GUI references
    Greet::Frame* frame;
    Greet::SceneView* sceneView;
    Greet::TreeNode* sceneTree;
    Greet::TreeView* sceneTreeView;
    Greet::Container* settingsContainer;

    Greet::Ref<EditorScene> scene;
    Greet::Ref<Greet::Shader> shader3d;
    Greet::Ref<Greet::Shader> shaderSkybox;
    Greet::Ref<Greet::CubeMap> cubeMapSkybox;
  public:
    Editor();

    ~Editor();

    void Init() override;
    void Destruct() override;
    void SetupGUI(Greet::Frame* frame);

    void CreateEntity();

    void Tick() override;
    void Update(float timeElapsed) override;
    void Render() override;
    void OnEvent(Greet::Event& event) override;

    Greet::TreeNode* GetRootSceneTreeNode(Greet::TreeNode* node) const;
};
