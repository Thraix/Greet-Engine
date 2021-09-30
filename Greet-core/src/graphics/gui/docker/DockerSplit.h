#pragma once

#include <graphics/gui/docker/DockerInterface.h>
#include <utils/xml/XMLObject.h>

#include <vector>

namespace Greet {

  class Docker;

  class DockerSplit : public DockerInterface
  {
    friend class DockerContainer;
    protected:
      bool vertical;
      std::vector<DockerInterface*> children;

      float grabPos;
      float grabSize;
      bool grabbingEdge = false;
      int grabbedEdgeIndex;

    public:
      DockerSplit(const XMLObject& object, Docker* docker, DockerSplit* parent);
      DockerSplit(DockerSplit* split, Docker* docker, DockerSplit* parent, bool vertical);
      DockerSplit(const std::vector<DockerInterface*>& children, Docker* docker, DockerSplit* parent, bool vertical);
      virtual ~DockerSplit();

      void Render(GUIRenderer* renderer) const override;
      void Update(float timeElapsed) override;
      void OnEvent(Event& event, const Vec2f& componentPos) override;

      void HandleDroppedTab(DockerTab* tab, MouseReleaseEvent& event, const Vec2f& componentPos) override;

      void AddDocker(int index, DockerInterface* docker);
      DockerInterface* RemoveDocker(int index);
      void ReplaceDocker(int index, DockerInterface* docker);
      void MergeSimilarSplits();

      int GetDockerIndex(DockerInterface* interface);
      int GetDockerCount() const;
      bool IsVertical() const;

      Component* GetComponentByNameNoCast(const std::string& name) override;
      Vec2f GetMinSize() const override;
      DockerTab* GetTab(const std::string& tabName) const override;

      void LoadFrameStyle(const MetaFile& metaFile) override;

      void SetPosition(const Vec2f& _size) override;
      void SetSize(const Vec2f& avSize, bool abRemeasure) override;
      void SetGUIScene(GUIScene* scene) override;

      void UpdateWeights();
      void NormalizeWeights();

      void DebugPrint(int indent) override;
  };
}
