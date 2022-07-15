#pragma once

namespace Greet
{
  struct SerializeComponent
  {
    bool serialize = true;
    SerializeComponent();
    SerializeComponent(bool serialize);
  };
}
