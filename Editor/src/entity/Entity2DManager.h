#pragma once


#include "LineBatchRenderer.h"

#include <event/Event.h>
#include <ecs/ECSScene.h>
#include <graphics/renderers/BatchRenderer.h>

class EntityManager;

class Entity2DManager
{

  struct EntityHolder
  {
    bool holding = false;
    bool threshold = false;
    Greet::Vec2f pressPos;
    Greet::Vec2f entityPressPos;
  };

  EntityManager* entityManager;
  Greet::ECSScene* scene;
  EntityHolder entityHolder;
  Greet::Ref<Greet::LineBatchRenderer> lineBatchRenderer;


  // Gizmo
  Greet::Ref<Greet::BatchRenderer> gizmoRenderer;
  Greet::Ref<Greet::Texture2D> translationGizmo;

  public:
    Entity2DManager(EntityManager* entityManager, Greet::ECSScene* scene);

    void OnEvent(Greet::Event& event);
    void RenderPre() const;
    void RenderPost() const;
};
