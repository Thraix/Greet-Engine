#include "TreeView.h"

#include <input/InputDefines.h>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(TreeView);

  TreeView::TreeView(const XMLObject& object, Component* parent)
    : Component{object, parent}, text{""}, tree{new TreeNode{""}}
  {
    AddStyle("active", "normal");
    AddStyleVariables(StylingVariables{
        .colors={
          {"textColor", &text.color},
          {"itemBackgroundColor", &itemBackgroundColor},
          {"itemBorderColor", &itemBorderColor}
        },
        .tlbrs={
          {"itemPadding", &itemPadding},
          {"itemBorder", &itemBorder}
        },
        .floats={
          {"spacing", &spacing},
          {"itemBackgroundRadius", &itemBackgroundRadius},
          {"itemBorderRadius", &itemBackgroundRadius}
        },
        .ints={
          {"itemBackgroundRoundedPrecision", &itemBackgroundRoundedPrecision},
          {"itemBorderRoundedPrecision", &itemBorderRoundedPrecision},
        },
        .fonts={{"font", &text.font}}
        });
    LoadStyles(object);
  }

  void TreeView::SetTreeNode(TreeNode* node)
  {
    delete tree;
    tree = node;
    tree->UpdateStyling(*this);
    Remeasure();
  }

  void TreeView::Update(float timeElapsed)
  {
    if(tree->dirty)
    {
      tree->UpdateStyling(*this);
      Remeasure();
      tree->UnmarkDirty();
    }
  }

  void TreeView::Render(GUIRenderer* renderer) const
  {
    renderer->PushTranslation(GetTotalPadding());
    renderer->PushViewport({0, 0}, GetContentSize(), false);
    tree->Render(renderer, *this);
    renderer->PopTranslation();
    renderer->PopViewport();
  }

  void TreeView::OnEvent(Event& event, const Vec2f& componentPos)
  {
    if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      MouseMoveEvent& e = static_cast<MouseMoveEvent&>(event);
      std::pair<TreeNode*, bool> ret = tree->GetTreeNodeAt(e.GetPosition() - componentPos - GetTotalPadding(), *this);

      if(hovered)
        hovered->SetHovered(false, *this, false);
      hovered = ret.first;
      if(hovered)
        hovered->SetHovered(true, *this, ret.second);
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      if(e.GetButton() == GREET_MOUSE_1)
      {
        if(doubleClickTimer.Elapsed() < 0.5)
        {
          if(selected && hovered == selected)
          {
            if(!selected->hoverFlowController)
            {
              selected->ToggleOpen(*this);
            }
          }
        }
        doubleClickTimer.Reset();

        if(hovered)
        {
          if(hovered->hoverFlowController)
          {
            hovered->ToggleOpen(*this);
          }
          else
          {
            if(selected != hovered)
            {
              SelectTreeNode(hovered);
            }
          }
        }
        else
        {
          if(selected)
            selected->SetSelected(false, *this);
          selected = nullptr;
        }
      }
    }
  }

  void TreeView::MouseExited()
  {
    if(hovered)
    {
      hovered->SetHovered(false, *this, false);
      hovered = nullptr;
    }
  }

  void TreeView::LoadFrameStyle(const MetaFile& metaFile)
  {
    Component::LoadFrameStyle(metaFile);
    tree->UpdateStyling(*this);
  }

  float TreeView::GetWrapWidth() const
  {
    return tree->GetWidth(*this);
  }

  float TreeView::GetWrapHeight() const
  {
    return tree->GetHeight(*this);
  }

  bool TreeView::HasSelectedNode() const
  {
    return selected != nullptr;
  }

  TreeNode* TreeView::GetSelectedNode() const
  {
    return selected;
  }

  TreeNode* TreeView::GetTreeNode(const UUID& uuid) const
  {
    return tree->GetChildNode(uuid);
  }

  void TreeView::SelectTreeNode(TreeNode* treeNode)
  {
    if(selected)
      selected->SetSelected(false, *this);
    selected = treeNode;
    if(selected)
      selected->SetSelected(true, *this);
  }

  void TreeView::SetOnNodeSelectedCallback(OnNodeSelected callback)
  {
    onNodeSelectedCallback = callback;
    if(selected)
      CallOnNodeSelectedCallback(selected, true);
  }

  void TreeView::SetOnNodeFlowChangedCallback(OnNodeFlowChanged callback)
  {
    onNodeFlowChangedCallback = callback;
  }

  void TreeView::CallOnNodeSelectedCallback(TreeNode* node, bool selected)
  {
    if(onNodeSelectedCallback)
      onNodeSelectedCallback(this, node, selected);
  }

  void TreeView::CallOnNodeFlowChangeCallback(TreeNode* node)
  {
    if(onNodeFlowChangedCallback)
      onNodeFlowChangedCallback(this, node);
  }
}
