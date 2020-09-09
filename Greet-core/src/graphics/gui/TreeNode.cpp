#include "TreeNode.h"

#include <graphics/gui/TreeView.h>

#include <algorithm>

namespace Greet
{
  TreeNode::TreeNode(const std::string& name)
    : name{name}
  {}

  TreeNode::TreeNode(const std::string& name, const std::initializer_list<TreeNode>& nodes)
    : name{name}, childNodes{nodes}
  {
    for(auto&& node : childNodes)
      node.parent = this;
  }

  TreeNode::TreeNode(const TreeNode& node)
    : childNodes{node.childNodes}, parent{node.parent}, name{node.name}, open{node.open}, selected{node.selected}, hovered{node.hovered}, dirty{node.dirty}, padding{node.padding}
  {
    for(auto&& node : childNodes)
      node.parent = this;
  }

  TreeNode::TreeNode(TreeNode&& node)
    : childNodes{std::move(node.childNodes)}, parent{node.parent}, name{std::move(node.name)}, open{node.open}, selected{node.selected}, hovered{node.hovered}, dirty{node.dirty}, padding{std::move(node.padding)}
  {
    for(auto&& node : childNodes)
      node.parent = this;
  }

  TreeNode& TreeNode::operator=(const TreeNode& node)
  {
    childNodes = node.childNodes;
    parent = node.parent;
    name = node.name;
    open = node.open;
    selected = node.selected;
    hovered = node.hovered;
    dirty = node.dirty;
    padding = node.padding;

    for(auto&& node : childNodes)
      node.parent = this;

    return *this;
  }

  TreeNode& TreeNode::operator=(TreeNode&& node)
  {
    childNodes = std::move(node.childNodes);
    parent = node.parent;
    name = std::move(node.name);
    open = node.open;
    selected = node.selected;
    hovered = node.hovered;
    dirty = node.dirty;
    padding = std::move(node.padding);

    for(auto&& node : childNodes)
      node.parent = this;

    return *this;
  }

  void TreeNode::Render(GUIRenderer* renderer, const TreeView& view) const
  {
    float offset = 0.0f;
    Render(renderer, offset, 0, view);
  }

  void TreeNode::Render(GUIRenderer* renderer, float& offset, int indent, const TreeView& view) const
  {
    float indentOffset = indent * GetFlowControllerWidth(view);

    if(!IsRoot())
    {
      if(hovered)
        renderer->DrawRect({indentOffset, offset}, {view.GetContentSize().w - indentOffset + padding.GetHeight(), (float)view.text.font.GetSize() + padding.GetWidth()}, Color(0xff626262), false);

      renderer->PushTranslation(Vec2{indentOffset + padding.left, offset + padding.top});
      RenderFlowController(renderer, view);
      float width = view.text.font.GetWidthOfText(name);
      renderer->DrawText(name, {GetFlowControllerWidth(view), (float)view.text.font.GetBaselineOffset()}, view.text.font, view.text.color, false);
      renderer->PopTranslation();
      offset += view.text.font.GetSize() + view.spacing + padding.GetHeight();
      indent++;
    }

    if(IsOpen())
    {
      for(auto&& node : childNodes)
      {
        node.Render(renderer, offset, indent, view);
      }
    }
  }

  void TreeNode::RenderFlowController(GUIRenderer* renderer, const TreeView& view) const
  {
    float posY = view.text.font.GetMedianOffset();
    float size = view.text.font.GetMedianHeight();

    if(!IsLeaf())
    {
      if(IsOpen())
        renderer->DrawTriangle({0, posY}, {size, posY}, {size * 0.5f, posY + size}, view.text.color, false);
      else
        renderer->DrawTriangle({0, posY}, {0, posY + size}, {size, posY + size * 0.5f}, view.text.color, false);
    }
    else
    {
      renderer->DrawLine({0, posY + size * 0.5f}, size, 1, false , view.text.color, false);
    }
  }

  TreeNode* TreeNode::GetTreeNodeAt(const Vec2& position, const TreeView& view)
  {
    if(!Utils::IsInside(position, {0, 0}, view.GetContentSize()))
      return nullptr;
    Vec2 pos = position;
    return GetTreeNodeAt(pos, 0, view);
  }

  TreeNode* TreeNode::GetTreeNodeAt(Vec2& position, int indent, const TreeView& view)
  {
    if(!IsRoot())
    {
      if(position.y >= 0 && position.y < view.text.font.GetSize() + padding.GetHeight())
      {
        position.y -= view.spacing + view.text.font.GetSize() + padding.GetHeight();
        if(position.x >= indent * GetFlowControllerWidth(view))
          return this;
        return nullptr;
      }
      position.y -= view.spacing + view.text.font.GetSize() + padding.GetHeight();
      indent++;
    }

    if(IsOpen())
    {
      for(auto&& node : childNodes)
      {
        TreeNode* at = node.GetTreeNodeAt(position, indent, view);
        if(at)
          return at;
      }
    }
    return nullptr;
  }

  void TreeNode::AddChildNode(TreeNode&& node)
  {
    childNodes.push_back(node);
    childNodes.back().parent = this;
    MarkDirty();
  }

  float TreeNode::GetWidth(const TreeView& view) const
  {
    return GetWidth(0, view);
  }

  float TreeNode::GetWidth(int indent, const TreeView& view) const
  {
    float width = 0;
    if(!IsRoot())
    {
      width = std::max(width, (float)view.text.font.GetWidthOfText(name) + (indent + 1) * GetFlowControllerWidth(view) + padding.GetWidth());
      indent++;
    }

    if(IsOpen())
    {
      for(auto&& node : childNodes)
      {
        width = std::max(width, node.GetWidth(indent, view));
      }
    }
    return width;
  }

  float TreeNode::GetFlowControllerWidth(const TreeView& view) const
  {
    return view.text.font.GetMedianHeight() + view.text.font.GetWidthOfText(std::string(" "));
  }

  float TreeNode::GetHeight(const TreeView& view) const
  {
    float height = 0.0f;
    if(!IsRoot())
    {
      height += view.text.font.GetSize() + padding.GetHeight();
    }
    else
      height -= view.spacing;

    if(IsOpen())
    {
      for(auto&& node : childNodes)
      {
        height += node.GetHeight(view) + view.spacing;
      }
    }
    return height;
  }


  void TreeNode::MarkDirty()
  {
    if(!IsRoot())
      parent->MarkDirty();
    else
      dirty = true;
  }

  void TreeNode::UnmarkDirty()
  {
    ASSERT(!parent, "Unmarking non root node in TreeNode");
    dirty = false;
  }

  void TreeNode::ToggleOpen(const TreeView& view)
  {
    open = !open;
    MarkDirty();
  }

  void TreeNode::SetHovered(bool hover, const TreeView& view)
  {
    if(hover != hovered)
    {
      hovered = hover;
      if(!selected)
      {
        if(hovered)
          SetStyling("hover", view);
        else
          SetStyling("normal", view);
      }
    }
  }

  void TreeNode::SetSelected(bool select, const TreeView& view)
  {
    if(select != selected)
    {
      selected = select;
      if(selected)
      {
        SetStyling("active", view);
      }
      else
      {
        if(hovered)
          SetStyling("hover", view);
        else
          SetStyling("normal", view);
      }
    }
  }

  void TreeNode::SetStyling(const std::string& styleName, const TreeView& view)
  {
    const ComponentStyle& style = view.GetStyle(styleName);
    padding = style.GetTLBR("itemPadding");
    MarkDirty();
  }

  void TreeNode::UpdateStyling(const TreeView& view)
  {
    std::string styleName = hovered ? "hover" : (selected ? "active" : "normal");
    SetStyling(styleName, view);
    for(auto&& node : childNodes)
    {
      node.UpdateStyling(view);
    }
  }
}
