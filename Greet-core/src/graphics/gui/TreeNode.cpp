#include "TreeNode.h"

#include <graphics/gui/TreeView.h>

#include <algorithm>

namespace Greet
{
  TreeNode::TreeNode(const std::string& name, bool open)
    : TreeNode{name, UUID{}, open}
  {}

  TreeNode::TreeNode(const std::string& name, const std::initializer_list<TreeNode>& nodes, bool open)
    : TreeNode{name, uuid, nodes, open}
  {}

  TreeNode::TreeNode(const std::string& name, const UUID& uuid, bool open)
    : name{name}, open{open}, uuid{uuid}
  {}

  TreeNode::TreeNode(const std::string& name, const UUID& uuid, const std::initializer_list<TreeNode>& nodes, bool open)
    : name{name}, childNodes{nodes}, open{open}, uuid{uuid}
  {
    for(auto&& node : childNodes)
      node.parent = this;
  }

  TreeNode::TreeNode(const TreeNode& node)
    : childNodes{node.childNodes}, parent{node.parent}, name{node.name}, open{node.open}, selected{node.selected}, hovered{node.hovered}, dirty{node.dirty}, styling{node.styling}, uuid{node.uuid}
  {
    for(auto&& node : childNodes)
      node.parent = this;
  }

  TreeNode::TreeNode(TreeNode&& node)
    : childNodes{std::move(node.childNodes)}, parent{node.parent}, name{std::move(node.name)}, open{node.open}, selected{node.selected}, hovered{node.hovered}, dirty{node.dirty}, styling{std::move(node.styling)}, uuid{node.uuid}
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
    styling = node.styling;
    uuid = node.uuid;

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
    styling = std::move(node.styling);
    uuid = node.uuid;

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
      if(styling.borderColor.a > 0)
        renderer->DrawRoundedRect({indentOffset, offset}, {view.GetContentSize().w - indentOffset, (float)view.text.font.GetSize() + styling.padding.GetHeight() + styling.border.GetHeight()}, styling.borderColor, styling.borderRadius, styling.borderRoundedPrecision, false);

      if(styling.backgroundColor.a > 0)
        renderer->DrawRoundedRect({indentOffset + styling.border.left, offset + styling.border.top}, {view.GetContentSize().w - indentOffset - styling.border.GetWidth(), (float)view.text.font.GetSize() + styling.padding.GetHeight()}, styling.backgroundColor, styling.backgroundRadius, styling.backgroundRoundedPrecision, false);

      renderer->PushTranslation(Vec2f{indentOffset + styling.padding.left + styling.border.left, offset + styling.padding.top + styling.border.top});
      RenderFlowController(renderer, view);
      float width = view.text.font.GetWidthOfText(name);
      renderer->DrawText(name, {GetFlowControllerWidth(view), (float)view.text.font.GetBaselineOffset()}, view.text.font, view.text.color, false);
      renderer->PopTranslation();
      offset += view.text.font.GetSize() + view.spacing + styling.padding.GetHeight() + styling.border.GetHeight();
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

  std::pair<TreeNode*, bool> TreeNode::GetTreeNodeAt(const Vec2f& position, const TreeView& view)
  {
    if(!Utils::IsInside(position, {0, 0}, view.GetContentSize()))
      return {nullptr, false};
    Vec2f pos = position;
    return GetTreeNodeAt(pos, 0, view);
  }

  std::pair<TreeNode*, bool> TreeNode::GetTreeNodeAt(Vec2f& position, int indent, const TreeView& view)
  {
    if(!IsRoot())
    {
      if(position.y >= 0 && position.y < view.text.font.GetSize() + styling.padding.GetHeight() + styling.border.GetHeight())
      {
        position.y -= view.spacing + view.text.font.GetSize() + styling.padding.GetHeight() + styling.border.GetHeight();
        if(position.x >= indent * GetFlowControllerWidth(view))
          return {this, position.x < (indent + 1) * GetFlowControllerWidth(view)};
        return {nullptr, false};
      }
      position.y -= view.spacing + view.text.font.GetSize() + styling.padding.GetHeight() + styling.border.GetHeight();
      indent++;
    }

    if(IsOpen())
    {
      for(auto&& node : childNodes)
      {
        std::pair<TreeNode*, bool> at = node.GetTreeNodeAt(position, indent, view);
        if(at.first)
          return at;
      }
    }
    return {nullptr, false};
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
      width = std::max(width, (float)view.text.font.GetWidthOfText(name) + (indent + 1) * GetFlowControllerWidth(view) + styling.padding.GetWidth());
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
      height += view.text.font.GetSize() + styling.padding.GetHeight() + styling.border.GetHeight();
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

  void TreeNode::ToggleOpen(TreeView& view)
  {
    open = !open;
    MarkDirty();
    view.CallOnNodeFlowChangeCallback(this);
  }

  TreeNode* TreeNode::GetChildNode(const std::string& name)
  {
    for(auto& child : childNodes)
    {
      if(child.name == name)
        return &child;
    }
    return nullptr;
  }

  TreeNode* TreeNode::GetChildNode(const UUID& uuid)
  {
    for(auto& child : childNodes)
    {
      if(child.uuid == uuid)
        return &child;
    }
    return nullptr;
  }

  TreeNode* TreeNode::GetChildNode(std::function<bool(const TreeNode& node)> compare)
  {
    for(auto& child : childNodes)
    {
      if(compare(child))
        return &child;
    }
    return nullptr;
  }

  void TreeNode::SetHovered(bool hover, const TreeView& view, bool hoverFlowController)
  {
    this->hoverFlowController = hoverFlowController;
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

  void TreeNode::SetSelected(bool select, TreeView& view)
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
      view.CallOnNodeSelectedCallback(this, select);
    }
  }

  void TreeNode::SetStyling(const std::string& styleName, const TreeView& view)
  {
    const ComponentStyle& style = view.GetStyle(styleName);
    styling.padding = style.GetTLBR("itemPadding");
    styling.border = style.GetTLBR("itemBorder");
    styling.backgroundColor = style.GetColor("itemBackgroundColor");
    styling.borderColor = style.GetColor("itemBorderColor");
    styling.backgroundRadius = style.GetFloat("itemBackgroundRadius");
    styling.borderRadius = style.GetFloat("itemBorderRadius");
    styling.backgroundRoundedPrecision = style.GetInt("itemBackgroundRoundedPrecision");
    styling.borderRoundedPrecision = style.GetInt("itemBorderRoundedPrecision");
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
