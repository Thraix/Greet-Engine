#pragma once

#include <graphics/gui/Component.h>
#include <graphics/gui/Text.h>
#include <graphics/gui/TreeNode.h>
#include <utils/Timer.h>

namespace Greet
{

  class TreeView : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(TreeView);
      friend class TreeNode;
      typedef std::function<void(TreeView* view, TreeNode* node, bool selected)> OnNodeSelected;
      typedef std::function<void(TreeView* view, TreeNode* node)> OnNodeFlowChanged;
    private:
      Timer doubleClickTimer;
      Text text;
      TreeNode* tree;

      ////////////////////
      // Styling variables
      float spacing = 5;

      ////////////////////
      // Item Styling variables
      TLBR itemPadding = TLBR{5};
      TLBR itemBorder = TLBR{0};
      Color itemBackgroundColor = Color{0, 0, 0, 0};
      Color itemBorderColor = Color{0, 0, 0, 0};
      float itemBackgroundRadius = 0;
      float itemBorderRadius = 0;
      int itemBackgroundRoundedPrecision = 3;
      int itemBorderRoundedPrecision = 3;

      TreeNode* hovered = nullptr;
      TreeNode* selected = nullptr;

      OnNodeSelected onNodeSelectedCallback;
      OnNodeFlowChanged onNodeFlowChangedCallback;
    public:
      TreeView(const XMLObject& object, Component* parent);

      void SetTreeNode(TreeNode* node);

      virtual void Update(float timeElapsed) override;
      virtual void Render(GUIRenderer* renderer) const override;

      virtual void OnEvent(Event& event, const Vec2f& componentPos) override;
      virtual void MouseExited() override;

      virtual void LoadFrameStyle(const MetaFile& metaFile) override;

      virtual float GetWrapWidth() const override;
      virtual float GetWrapHeight() const override;

      bool HasSelectedNode() const;
      TreeNode* GetSelectedNode() const;
      TreeNode* GetRootTreeNode() const { return tree; };
      TreeNode* GetTreeNode(const UUID& uuid) const;
      void SelectTreeNode(TreeNode* treeNode);

      void SetOnNodeSelectedCallback(OnNodeSelected callback);
      void SetOnNodeFlowChangedCallback(OnNodeFlowChanged callback);

      void CallOnNodeSelectedCallback(TreeNode* node, bool selected);
      void CallOnNodeFlowChangeCallback(TreeNode* node);
  };
}

