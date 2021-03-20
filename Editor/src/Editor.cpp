#include "Editor.h"

#include "CameraController.h"

using namespace Greet;

Editor::Editor()
  : App{"Editor", 1440, 1440 * 9 / 16}
{
  SetFrameCap(60);
}

Editor::~Editor()
{
}

void Editor::Init()
{
  FontManager::Add("noto", FontContainer("res/fonts/noto.ttf"));
  gui = NewRef<GUIScene>();
  frame = FrameFactory::GetFrame("res/guis/Editor.xml");
  gui->AddFrameQueued(frame);
  GlobalSceneManager::GetSceneManager().Add2DScene(gui, "gui");

  shader3d = ShaderFactory::Shader3D();
  shaderSkybox = ShaderFactory::ShaderSkybox();
  cubeMapSkybox = TextureManager::LoadCubeMap("res/textures/skybox.meta");

  SetupGUI(frame);
}

void Editor::SetupGUI(Frame* frame)
{
  settingsContainer = frame->GetComponentByName<Container>("settings");
  settingsContainer->AddComponent(ComponentFactory::GetComponent("res/guis/Transformation3DCompontent.xml", settingsContainer));
  settingsContainer->AddComponent(ComponentFactory::GetComponent("res/guis/Transformation2DCompontent.xml", settingsContainer));
  settingsContainer->LoadFrameStyle(frame->GetStylingFile());
  sceneView = frame->GetComponentByName<SceneView>("sceneView");
  Button* addSceneButton = frame->GetComponentByName<Button>("addSceneButton");
  Button* addEntityButton = frame->GetComponentByName<Button>("addEntityButton");
  sceneTreeView = frame->GetComponentByName<TreeView>("treeView");

  ASSERT(sceneView, "Could not load SceneView");
  ASSERT(addSceneButton, "Could not load Add Scene Button");
  ASSERT(addEntityButton, "Could not load Add Entity Button");
  ASSERT(sceneTreeView, "Could not load Scene TreeView");

  sceneTree = new TreeNode({});
  sceneTreeView->SetTreeNode(sceneTree);

  // Create scene
  std::string name = "Scene " + std::to_string(scenes.size());
  scene = NewRef<EditorScene>();
  Entity camera = scene->AddEntity("Camera3D");
  camera.AddComponent<Camera3DComponent>(Mat4::Identity(), 90, 0.01, 1000, true);
  camera.AddComponent<Environment3DComponent>(shaderSkybox, cubeMapSkybox);
  camera.AddComponent<NativeScriptComponent>(NewRef<NativeScriptHandler>(new CameraControllerScript()));

  scenes.emplace(name, scene);
  sceneTree->AddChildNode(TreeNode{name});
  sceneView->GetSceneManager().Add3DScene(scene, name);

  addEntityButton->SetOnPressCallback([&](Component* component)
  {
    if(sceneTreeView->HasSelectedNode())
    {
      static int entityId = 0;
      TreeNode* selected = sceneTreeView->GetSelectedNode();
      TreeNode* sceneNode = GetRootSceneTreeNode(selected);
      Ref<ECSScene> scene = scenes.find(sceneNode->GetName())->second;
      Entity entity = scene->AddEntity("Entity#" + std::to_string(entityId));
      selected->AddChildNode(TreeNode{"Entity#" + std::to_string(entityId)});
      entity.AddComponent<Transform3DComponent>(Vec3f{entityId * 1.5f, 0, 0}, Vec3f{1,1,1}, Vec3f{0,0,0});
      MeshData data = MeshFactory::Cube({0,0,0}, {1,2,1.5});
      entity.AddComponent<MeshComponent>(NewRef<Mesh>(data));
      entity.AddComponent<MaterialComponent>(NewRef<Material>(ShaderFactory::Shader3D()));
      entityId++;
    }
  });
}

void Editor::Destruct()
{
  GlobalSceneManager::GetSceneManager().Remove2DScene("gui");
}

TreeNode* Editor::GetRootSceneTreeNode(TreeNode* node) const
{
  TreeNode* current = node;
  while(!current->GetParent()->IsRoot())
  {
    current = current->GetParent();
  }
  return current;
}

void Editor::Tick()
{
}

void Editor::Render()
{
}

void Editor::Update(float timeElapsed)
{
}

void Editor::OnEvent(Event& event)
{
}
