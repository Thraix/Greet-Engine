#include <Greet.h>

#include "keyboardcontrol.h"
#include "tree.h"
#include "Portal.h"
#include "World.h"
#include <random>

using namespace Greet;


class UILayer : public Layer
{
  public:
  UILayer(const Ref<Renderer2D>& renderer, const Mat3& projectionMatrix)
    : Layer{renderer, projectionMatrix}
  {
  }

  virtual void ViewportResize(ViewportResizeEvent& event) override
  {
    SetProjectionMatrix(Mat3::OrthographicViewport());
  }
};

#if 1
class Core : public App
{
  private:
    Ref<BatchRenderer3D> renderer3d;
    Ref<BatchRenderer3D> waterRenderer;
    Ref<Material> modelMaterial;
    Ref<Material> flatMaterial;
    Ref<Material> terrainMaterial;
    Ref<Material> waterMaterial;
    Ref<Material> stallMaterial;
    KeyboardControl* movement;
    KeyboardControl* rotation;

    Ref<EntityModel> stall;
    Ref<EntityModel> dragon;
    Ref<EntityModel> terrain;
    Ref<EntityModel> water;
    Ref<EntityModel> polygon;
    Ref<EntityModel> cube;
    Ref<EntityModel> sphere;
    Ref<EntityModel> tetrahedron;
    std::vector<EntityModel> models;
    Light light{{}, 0x0};

    SceneView* sceneView;


    float progressFloat;

    Ref<UILayer> uilayer;
    Ref<Layer3D> layer3d;
    Ref<Renderable2D> cursor;

    Button* button;
    Label* fps;

  public:
    Core()
      : App("Best Game Ever", 960, 540)
    {
      SetFrameCap(144);
    }

    ~Core()
    {
      delete movement;
      delete rotation;
    }

    void Init() override
    {
      uint noiseS = 100;
      std::vector<float> noise = Noise::GenNoise(noiseS, noiseS, 4, 16, 16, 0.8f);
      ImageData imageData = ImageUtils::CreateHeightmapImage(noise, noiseS, noiseS);

      TextureManager::AddTexture2D("noise", Texture2D::Create(imageData, TextureParams(TextureFilter::NEAREST, TextureWrap::CLAMP_TO_EDGE, TextureInternalFormat::RGBA)));

      FontManager::Add("noto", FontContainer("../res/fonts/NotoSansUI-Regular.ttf"));

      Ref<GUIScene> guiScene = NewRef<GUIScene>();
      Frame* frame = FrameFactory::GetFrame("res/guis/header.xml");
      guiScene->AddFrameQueued(frame);

      //camera = new TPCamera3D(vec3(-3.5, -7.8, 5.5), 18, 0.66, 38.5, 15, 80, 0, 0.8f); // Profile shot

      Ref<Skybox> skybox = NewRef<Skybox>(TextureManager::LoadCubeMap("../res/textures/skybox.meta"));
      renderer3d = NewRef<BatchRenderer3D>();
      waterRenderer = NewRef<BatchRenderer3D>();


      modelMaterial = NewRef<Material>(ShaderFactory::Shader3D(), TextureManager::LoadTexture2D("res/textures/debugtexture.meta"));
      flatMaterial = NewRef<Material>(Shader::FromFile("res/shaders/flat3d.glsl"));
      stallMaterial = NewRef<Material>(ShaderFactory::Shader3D(), TextureManager::LoadTexture2D("../res/textures/viking_room.meta"));
      modelMaterial->SetSpecularStrength(1.0)->SetSpecularExponent(10)->SetDiffuseStrength(0.5);

      {
        terrainMaterial = NewRef<Material>(Shader::FromFile("res/shaders/terrain.glsl"));
        waterMaterial = NewRef<Material>(Shader::FromFile("res/shaders/water.glsl"));
        waterMaterial->SetSpecularExponent(50);
        waterMaterial->SetSpecularStrength(0.4);
        waterMaterial->GetShader()->Enable();
        waterMaterial->GetShader()->SetUniform1f("uWaterLevel", 0.45f * 20.0f);
        waterMaterial->GetShader()->Disable();

        terrainMaterial->SetSpecularStrength(0.5f);
        terrainMaterial->SetSpecularExponent(5.0f);

        int gridWidth = 99;
        int gridLength = 99;
#if 1
        std::vector<float> noise = Noise::GenNoise(gridWidth+1, gridWidth + 1,5,8, 8,0.5f);
        MeshData gridMesh = MeshFactory::GridLowPoly(Vec2i{gridWidth, gridLength}, noise, {0, 0, 0}, Vec3f{gridWidth+1.0f, 1.0f, gridLength+1.0f});
        RecalcGrid(gridMesh, gridWidth, gridLength);
        terrain = NewRef<EntityModel>(NewRef<Mesh>(gridMesh), terrainMaterial, Vec3f(0, -15, 0), Vec3f(1.0f, 1.0f, 1.0f), Vec3f(0.0f, 0.0f, 0.0f));
        gridMesh.RemoveAttribute(MESH_NORMALS_LOCATION);
        gridMesh.RemoveAttribute(MESH_COLORS_LOCATION);
        CalcGridVertexOffset(gridMesh);

        water = NewRef<EntityModel>(NewRef<Mesh>(gridMesh), waterMaterial, Vec3f(0, -15, 0), Vec3f(1.0f, 1.0f, 1.0f), Vec3f(0.0f, 0.0f, 0.0f));
#endif
      }

      MeshData polygonMesh = MeshFactory::Polygon(6, MeshFactory::PolygonSizeFormat::SIDE_LENGTH, {0, 0, 0}, 10);
      polygon = NewRef<EntityModel>(NewRef<Mesh>(polygonMesh), terrainMaterial, Vec3f(0,1,0), Vec3f(1,1,1), Vec3f(0,0,0));


      MeshData cubeMesh = MeshFactory::Cube();
      cube = NewRef<EntityModel>(NewRef<Mesh>(cubeMesh), modelMaterial, Vec3f(1,0,0), Vec3f(10, 10, 10), Vec3f(0, 0, 0));

      MeshData sphereMeshData = MeshFactory::Sphere(20, 20, {0, 0, 0}, 0.5f);
      sphereMeshData.GenerateNormals();
      //sphereMeshData->LowPolify();
      Ref<Mesh> sphereMesh = NewRef<Mesh>(sphereMeshData);
      sphere = NewRef<EntityModel>(sphereMesh, modelMaterial, Vec3f(0,0,0), Vec3f(10, 10, 10), Vec3f(0, 0, 0));

      MeshData tetrahedronMesh = MeshFactory::Tetrahedron({0, 0, 0}, 10);
      tetrahedron = NewRef<EntityModel>(NewRef<Mesh>(tetrahedronMesh), modelMaterial, Vec3f(30, 0, 10), Vec3f(1, 1, 1), Vec3f(0, 0, 0));

      MeshData stallMeshData = OBJUtils::LoadObj("../res/objs/viking_room.obj");
      //
      Ref<Mesh> stallMesh = NewRef<Mesh>(stallMeshData);
      stallMaterial->SetSpecularStrength(0.1)->SetSpecularExponent(1);
      stall = NewRef<EntityModel>(stallMesh, stallMaterial, Vec3f(0.0f, 0.0f, -80), Vec3(30.0f, 30.0f, 30.0f), Vec3f(-M_PI / 2, 0, -M_PI / 2));

      // MEMORY LEAK WITH MESHDATA
      //MeshData data = MeshData::ReadFromFile("res/objs/dragon.gobj");
      MeshData data = OBJUtils::LoadObj("../res/objs/dragon.obj");
      data = *data.LowPolify();
      data.GenerateNormals();
      /* data.WriteToFile("res/objs/dragon.gobj"); */
      Ref<Mesh> dragonMesh = NewRef<Mesh>(data);
      dragon = NewRef<EntityModel>(dragonMesh, flatMaterial, Vec3f(20.0f, 0.0f, -25), Vec3(1.0f, 1.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f));

      //Mesh* gridMesh = MeshFactory::cube(0,0,0,10,10,10);
      //gridMesh->setEnableCulling(false);
      //MaterialModel* gridModelMaterial = new MaterialModel(gridMesh, *modelMaterial);
      //grid = new EntityModel(*gridModelMaterial, vec3(0, 0, 0), vec3(1, 1, 1), vec3(0, 0, 0));

      //for (uint i = 0;i < 2000;i++)
      //{
      //	models.push_back(EntityModel(*modelModelMaterial, vec3(random()*100, random() * 100, random() * 100), vec3(1.0f, 1.0f, 1.0f), vec3(random() * 360, random() * 360, random() * 360)));
      //}

      light = Light(Vec3f(10, 20, 10), 0xffffffff);
      const Ref<Shader>& modelShader = modelMaterial->GetShader();
      modelShader->Enable();
      light.SetToUniform(modelShader, "Light");
      modelShader->Disable();
      const Ref<Shader>& flatShader = flatMaterial->GetShader();
      flatShader->Enable();
      light.SetToUniform(flatShader, "Light");
      flatShader->Disable();

      uilayer = NewRef<UILayer>(NewRef<BatchRenderer>(ShaderFactory::Shader2DUI()), Mat3::OrthographicViewport());
      Vec4 colorPink = ColorUtils::GetMaterialColorAsHSV(300 /360.0f, 3);
      cursor = NewRef<Renderable2D>(Vec2f(0,0), Vec2f(32,32), 0xffffffff, Ref<Sprite>{new Sprite{TextureManager::LoadTexture2D("res/textures/cursor.meta")}});
      uilayer->Add(cursor);
      uilayer->Add(NewRef<Renderable2D>(Vec2f(0,0), Vec2f(noiseS, noiseS), 0xffffffff, Ref<Sprite>{new Sprite{TextureManager::LoadTexture2D("noise")}}));

      renderer3d->Submit(NewRef<Portal>(Vec3f{1.0f,1.0f,1.0f}));
      renderer3d->Submit(stall);
      renderer3d->Submit(dragon);
      renderer3d->Submit(terrain);
      renderer3d->Submit(sphere);
      renderer3d->Submit(cube);
      waterRenderer->Submit(water);
      //renderer3d->Submit(polygon);
      //renderer3d->Submit(tetrahedron);
      //for (uint i = 0;i < 2000;i++)
      //{
      //	renderer3d->submit(&models[i]);
      //}
      movement = new KeyboardControl(GLFW_KEY_D,GLFW_KEY_A,GLFW_KEY_S,GLFW_KEY_W,0.5f);
      rotation = new KeyboardControl(GLFW_KEY_DOWN,GLFW_KEY_UP,GLFW_KEY_RIGHT,GLFW_KEY_LEFT,2);
      //Tree t(renderer3d,0,0,0);
      uint pos = 0;
      //		Log::Info(JSONLoader::isNumber("0.1234s",pos));
      GlobalSceneManager::GetSceneManager().Add2DScene(guiScene, "guiScene");

      Ref<TPCamera3D> camera{new TPCamera3D{90, 0.01f,1000.0f, Vec3f(0, 0, 0), 15, 0, 0, 0, 80, -0.8f, 0.8f}};
      layer3d = NewRef<Layer3D>(camera, skybox);
      layer3d->AddRenderer(renderer3d);
      layer3d->AddRenderer(waterRenderer);
      fps = frame->GetComponentByName<Label>("fpsCounter");
      sceneView = frame->GetComponentByName<SceneView>("scene");
      sceneView->GetSceneManager().Add3DScene(layer3d, "World");
      sceneView->GetSceneManager().Add2DScene(uilayer, "uilayer");
    }

    void Destruct() override
    {
      sceneView->GetSceneManager().Remove3DScene("World");
      sceneView->GetSceneManager().Remove2DScene("uilayer");
      GlobalSceneManager::GetSceneManager().Remove2DScene("guiScene");
    }

    void RecalcPositions(Vec3f& vertex)
    {
      float y = vertex.y;
      if (y < 0.45)
      {
        //y = 0.45f + Noise::PRNG(vertex->x, vertex->z)*0.01f;// + 0.03f*(rand() / (float)RAND_MAX - 0.5f);
      }
      else if (y < 0.48)
      {

      }
      else if (y < 0.58)
      {

      }
      else if (y < 0.65)
      {
        y = (pow(y - 0.58, 0.6) + 0.58);
      }
      else
      {
        y = (pow(y - 0.58, 0.6) + 0.58);
      }
      vertex.y = y * 20.0;
    }

    void RecalcColors(const Vec3f& v1, const Vec3f& v2, const Vec3f& v3, uint* color)
    {
      float y = (v1.y + v2.y + v3.y) / 3.0f / 20.0f;
      if (y < 0.45+0.01f)
      {
        uint blue = (uint)(pow(1, 4.0f) * 255);
        blue = blue > 255 ? 255 : blue;
        *color = 0xff000000 | ((blue / 2) << 16) | ((uint)(blue * 0.9) << 8) | blue;
        *color = 0xffF0E5A5;
      }
      else if (y < 0.48)
      {
        *color = 0xffF0E5A5;
      }
      else if (y < 0.58)
      {
        *color = 0xff7CD663;
      }
      else if (y < pow(0.07, 0.6) + 0.58)
      {
        *color = 0xffB5B0A8;
      }
      else
      {
        *color = 0xffDCF2F2;
      }
    }

    void CalcGridVertexOffset(MeshData& data)
    {
      Pointer<Vec3f>& vertices = data.GetVertices();
      uint vertexCount = data.GetVertexCount();
      uint indexCount = data.GetIndexCount();
      Pointer<uint>& indices = data.GetIndices();

      Pointer<uint8_t> offsets(4 * vertexCount);
      for (int i = 0;i < indexCount;i+=3)
      {
        Vec3f v1 = vertices[indices[i+1]] - vertices[indices[i]];
        Vec3f v2 = vertices[indices[i+2]] - vertices[indices[i]];
        offsets[indices[i]*4] = round(v1.x);
        offsets[indices[i]*4 + 1] = round(v1.z);
        offsets[indices[i]*4 + 2] = round(v2.x);
        offsets[indices[i]*4 + 3] = round(v2.z);
      }
      data.AddAttribute({4, BufferAttributeType::BYTE4}, offsets);
    }

    void RecalcGrid(MeshData& data, uint gridWidth, uint gridLength)
    {
      Pointer<uint> colors(data.GetVertexCount());
      Pointer<Vec3f>& vertices = data.GetVertices();
      uint indexCount = data.GetIndexCount();
      Pointer<uint>& indices = data.GetIndices();
      Pointer<Vec3f> normals = data.GetAttribute(MESH_NORMALS_LOCATION)->second;
      for (int i = 0;i < indexCount;i+=3)
      {
        RecalcPositions(vertices[indices[i]]);
      }
      uint index = MeshFactory::IndexGrid(gridWidth, gridLength - 1, gridWidth, gridLength);
      RecalcPositions(vertices[MeshFactory::IndexGrid(gridWidth,gridLength-1,gridWidth,gridLength)]);
      index = MeshFactory::IndexGrid(0, gridLength, gridWidth, gridLength);
      RecalcPositions(vertices[index]);
      for (int i = 0;i < indexCount;i += 3)
      {
        normals[indices[i]] = MeshFactory::CalculateNormal(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]]);
        RecalcColors(vertices[indices[i]], vertices[indices[i+1]], vertices[indices[i+2]], &colors[indices[i]]);
      }
      data.AddAttribute({MESH_COLORS_LOCATION, BufferAttributeType::UBYTE4, true}, colors);
    }

    float Random()
    {
      return (float)((double)rand() / (RAND_MAX));
    }

    void Tick() override
    {
      std::string s = StringUtils::to_string(GetFPS()) + " fps | " + StringUtils::to_string(GetUPS())+ " ups";
      if(fps)
        fps->SetText(StringUtils::to_string(GetFPS()));
      Window::SetTitle("Best Game Ever | " + s);
    }

    float hue = 0;
    Vec3f velocityPos;
    Vec3f velocityNeg;
    float waterTime = 0.0;

    void Update(float elapsedTime) override
    {
      waterTime += elapsedTime;
      waterMaterial->GetShader()->Enable();
      waterMaterial->GetShader()->SetUniform1f("uTime", waterTime);
      waterMaterial->GetShader()->Disable();

      progressFloat++;
      if (progressFloat > 1000)
        progressFloat = 0;
      hue += elapsedTime / 3.0f;
      while (hue >= 1)
        hue--;
      cursor->m_color = Color(hue, 1, 1, 1.0).ToRGB().AsUInt();
      sphere->SetPosition(light.position);
    }

    void OnEvent(Event& event) override
    {
      if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
      {
        // Temporary add viewport since we are not inside the layer
        RenderCommand::PushViewportStack(sceneView->GetRealPosition(), Vec2f(Window::GetWidth(), Window::GetHeight()) - sceneView->GetRealPosition());
        cursor->SetPosition(~uilayer->GetProjectionMatrix() * Input::GetMousePos());
        RenderCommand::PopViewportStack();
      }
      else if(EVENT_IS_TYPE(event, EventType::KEY_RELEASE))
      {
        KeyReleaseEvent& e = (KeyReleaseEvent&)event;
        Log::Info(e.GetButton());
        movement->onInput(e.GetButton(),false);
        rotation->onInput(e.GetButton(),false);
        if (e.GetButton() == GLFW_KEY_LEFT_SHIFT)
        {
          velocityNeg.y = 0;
        }
        if (e.GetButton() == GLFW_KEY_SPACE)
        {
          velocityPos.y = 0;
        }
      }
      else if(EVENT_IS_TYPE(event, EventType::JOYSTICK_CONNECT))
      {
        Log::Info("Controller %s connected!", ((JoystickConnectEvent&)event).GetJoystick());
      }
      else if(EVENT_IS_TYPE(event, EventType::JOYSTICK_DISCONNECT))
      {
        Log::Info("Controller %s disconnected!", ((JoystickDisconnectEvent&)event).GetJoystick());
      }
      else if(EVENT_IS_TYPE(event, EventType::KEY_PRESS))
      {
        KeyPressEvent& e = (KeyPressEvent&)event;
        if (e.GetButton() == GLFW_KEY_F5)
        {
          modelMaterial->SetShader(Shader::FromFile("res/shaders/3dshader.shader"));
          terrainMaterial->SetShader(Shader::FromFile("res/shaders/terrain.shader"));
          flatMaterial->SetShader(Shader::FromFile("res/shaders/flat3d.shader"));
          modelMaterial->GetShader()->Enable();
          light.SetToUniform(modelMaterial->GetShader(), "Light");
          modelMaterial->GetShader()->Disable();
          flatMaterial->GetShader()->Enable();
          light.SetToUniform(flatMaterial->GetShader(), "Light");
          flatMaterial->GetShader()->Disable();
          //terrainShader->enable();
          //l->setToUniform(terrainShader, "light");
          //terrainShader->disable();
        }

        Ref<TPCamera3D> camera = std::static_pointer_cast<TPCamera3D>(layer3d->GetCamera3D());
        if (e.GetButton() == GLFW_KEY_F1)
        {
          Log::Info("pos=%s height=%s distance=%s rotation=%s", camera->GetPosition(), camera->GetHeight(), camera->GetDistance(), camera->GetRotation());
        }
        if (e.GetButton() == GLFW_KEY_F2)
        {
          camera->SetPosition(Vec3f(-3.5, -7.8, 5.5));
          camera->SetDistance(18);
          camera->SetHeight(0.66);
          camera->SetRotation(38.5);
        }
        if (e.GetButton() == GLFW_KEY_F10)
        {
          Utils::Screenshot(RenderCommand::GetViewportWidth(), RenderCommand::GetViewportHeight());
        }
        if (e.GetButton() == GLFW_KEY_X)
        {
          Vec2f p = Vec2f{camera->GetWorldToScreenCoordinate(Vec3f(0, 0, 0))};
          Window::TransformScreenToWindowPos(p);
          Log::Info(p);
        }
        movement->onInput(e.GetButton(),true);
        rotation->onInput(e.GetButton(),true);
        if (e.GetButton() == GLFW_KEY_LEFT_SHIFT)
        {
          velocityNeg.y = 0.2;
        }
        if (e.GetButton() == GLFW_KEY_SPACE)
        {
          velocityPos.y = 0.2;
        }
      }
      else if(EVENT_IS_TYPE(event, EventType::JOYSTICK_PRESS))
      {
        JoystickPressEvent& e = (JoystickPressEvent&)event;
        Log::Info("Joystick button pressed joy=%s button=%s", e.GetJoystick(), e.GetButton());
      }
    }

    bool screenshot = false;
    void Render() override
    {
    }
};
#endif

int main()
{
  Core game;
  game.Start();
}
