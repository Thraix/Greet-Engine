#pragma once

#include "entity/EntityManager.h"

class Editor : public Greet::App
{
  private:
    Greet::Ref<Greet::GUIScene> gui;
    Greet::Ref<EntityManager> entityManager;
  public:
    Editor();

    ~Editor();

    void Init() override;
    void Destruct() override;

    void Tick() override;
    void Update(float timeElapsed) override;
    void Render() override;
    void OnEvent(Greet::Event& event) override;
};
