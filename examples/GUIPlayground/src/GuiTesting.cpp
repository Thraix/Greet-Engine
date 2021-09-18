#include <Greet.h>

#include <functional>

#include "TestScene.h"

using namespace Greet;

class Layer2D : public Layer
{
  public:
    Layer2D()
      : Layer{NewRef<BatchRenderer>(ShaderFactory::Shader2D()), Mat3{1}}
    {}

    virtual void ViewportResize(ViewportResizeEvent& event) override
    {
      SetProjectionMatrix(Mat3::OrthographicViewport());
    }
};

class Core : public App
{
  private:
    Ref<GUIScene> guiScene;
    Component* content;
    TestScene* scene;
    float progressBarValue;
    Ref<Layer2D> layer;
    SceneView* editorView;
    Ref<Renderable2D> renderable;
  public:
    Core()
      : App("GUI Testing", 960, 540)
    {
      SetFrameCap(60);
    }

    ~Core()
    {}

    void Init() override
    {
      renderable = NewRef<Renderable2D>(Vec2f{0, 0}, Vec2f{70, 70}, 0xffffffff);
      /* GlobalSceneManager::GetSceneManager().Add2DScene(new TestScene(), "testscene"); */
      /* return; */
      layer = NewRef<Layer2D>();
      layer->Add(renderable);
      progressBarValue = 0;
      FontManager::Add("noto", FontContainer("res/fonts/NotoSansUI-Regular.ttf"));

      guiScene = NewRef<GUIScene>();

      Frame* frame = FrameFactory::GetFrame("res/guis/gui.xml");
      guiScene->AddFrameQueued(frame);

#if 1
      if(frame != nullptr)
      {
        editorView = frame->GetComponentByName<SceneView>("EditorView");
        frame->GetComponentByName<ProgressBar>("progressBar")
          ->AttachValueReference(&progressBarValue);
        frame->GetComponentByName<RadioGroup>("Radio")
          ->SetOnRadioChangeCallback(BIND_MEMBER_FUNC(OnRadioChangeCallback));
        frame->GetComponentByName<Slider>("slider")
          ->SetOnClickCallback(BIND_MEMBER_FUNC(OnClickCallback));
        frame->GetComponentByName<Slider>("slider")
          ->SetOnPressCallback(BIND_MEMBER_FUNC(OnClickCallback));
        frame->GetComponentByName<Slider>("slider")
          ->SetOnReleaseCallback(BIND_MEMBER_FUNC(OnReleaseCallback));
        frame->GetComponentByName<Slider>("slider")
          ->SetOnValueChangeCallback(BIND_MEMBER_FUNC(OnValueChangeCallback));
        frame->GetComponentByName<Slider>("sliderVertical")
          ->SetOnValueChangeCallback(BIND_MEMBER_FUNC(OnValueChangeVerticalCallback));
        frame->GetComponentByName<ProgressBar>("progressBar")
          ->AttachValueReference(&progressBarValue);
        frame->GetComponentByName<ProgressBar>("progressBarVertical")
          ->AttachValueReference(&progressBarValue);
        frame->GetComponentByName<ProgressBar>("progressBarReverse")
          ->AttachValueReference(&progressBarValue);
        frame->GetComponentByName<ProgressBar>("progressBarVerticalReverse")
          ->AttachValueReference(&progressBarValue);
        frame->GetComponentByName<ColorPicker>("backgroundChanger")
          ->SetOnColorChangeCallback(BIND_MEMBER_FUNC(OnColorChangeCallback));
        frame->GetComponentByName<Button>("button")
          ->SetOnClickCallback(BIND_MEMBER_FUNC(OnButtonPressCallback));
        frame->GetComponentByName<TreeView>("treeview")
          ->SetTreeNode(new TreeNode("Root", {
                          TreeNode("Entity1", {
                              TreeNode("Translation", {
                                  TreeNode("X"),
                                  TreeNode("Y"),
                                  }),
                              TreeNode("Tag", {
                                  TreeNode("str")
                                  })
                              }),
                          TreeNode("Entity2", {
                              TreeNode("Translation", {
                                  TreeNode("X"),
                                  TreeNode("Y"),
                                  }),
                              TreeNode("Tag", {
                                  TreeNode("str")
                                  })
                              }),
                          TreeNode("Entity3", {
                              TreeNode("Translation", {
                                  TreeNode("X"),
                                  TreeNode("Y"),
                                  }),
                              TreeNode("Tag", {
                                  TreeNode("str")
                                  })
                              })
                          }));
        frame->GetComponentByName<TreeView>("treeview")
          ->SetOnNodeSelectedCallback(BIND_MEMBER_FUNC(OnTreeNodeSelected));
        frame->GetComponentByName<TreeView>("treeview")
          ->SetOnNodeFlowChangedCallback(BIND_MEMBER_FUNC(OnTreeNodeFlowChanged));

        editorView->Add2DScene(layer, "2dScene");

        Docker* docker = frame->GetComponentByName<Docker>("docker");
        if(docker)
        {
          DockerTab* tab = docker->GetTab("Settings");
          if(tab)
          {
            tab->SetOnTabShowCallback(
                [](Docker* docker, DockerTab* tab)
                {
                  Log::Info("Show");
                });
            tab->SetOnTabHideCallback(
                [](Docker* docker, DockerTab* tab)
                {
                  Log::Info("Hide");
                });
          }
        }
      }
#endif
      GlobalSceneManager::GetSceneManager().Add2DScene(guiScene, "GUIScene");
    }

    void Destruct() override
    {
      GlobalSceneManager::GetSceneManager().Remove2DScene("GUIScene");
    }

    void OnTreeNodeSelected(TreeView* view, TreeNode* node, bool selected)
    {
      if(selected)
        Log::Info("TreeView node selected ", node->GetName());
      else
        Log::Info("TreeView node unselected ", node->GetName());
    }

    void OnTreeNodeFlowChanged(TreeView* view, TreeNode* node)
    {
      Log::Info("TreeView node flow changed %s %s", node->GetName(), node->IsOpen() ? "true" : "false");
    }

    void OnRadioChangeCallback(RadioButton* button)
    {
      Log::Info("Radio changed to ", button->GetName());
    }

    void OnButtonPressCallback(Component* button)
    {
      renderable->m_color = 0xff000000 | ((rand() % 255) << 16) | ((rand() % 255) << 8) | (rand() % 255);
    }

    void OnValueChangeCallback(Slider* slider, float oldValue, float newValue)
    {
      renderable->m_position.x = editorView->GetWidth()/2 + newValue - (slider->GetMaxValue() - slider->GetMinValue()) / 2 - renderable->m_size.w / 2;
    }

    void OnValueChangeVerticalCallback(Slider* slider, float oldValue, float newValue)
    {
      renderable->m_position.y = editorView->GetHeight()/2 + newValue - (slider->GetMaxValue() - slider->GetMinValue()) / 2 - renderable->m_size.h / 2;
    }

    void OnColorChangeCallback(Component* component, const Color& oldValue, const Color& current)
    {
      RenderCommand::SetClearColor(Color(current.r,current.g,current.b, 1));
      editorView->LoadStyle("normal", Styling{.colors={{"backgroundColor", current}}});
    }

    void OnClickCallback(Component* component)
    {
      Log::Info("On click");
    }

    void OnPressCallback(Component* component)
    {
      Log::Info("On Press");
    }

    void OnReleaseCallback(Component* component)
    {
      Log::Info("On Release");
    }

    void Tick() override
    {
      std::string s = StringUtils::to_string(GetFPS()) + " fps | " + StringUtils::to_string(GetUPS()) + " ups";
      Window::SetTitle("GUI Testing | " + s);
    }

    void Update(float elapsedTime) override
    {
      progressBarValue += elapsedTime * 0.5;
      if(progressBarValue >= 1.5)
        progressBarValue -= 1.5;
    }

    void Render() override
    {
    }
};

int main()
{
  {
    //XMLObject obj = XML::FromFile("projtest.xml");
    Core game;
    game.Start();


  }
}
