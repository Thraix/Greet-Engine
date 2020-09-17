#pragma once
#include <Greet.h>
#include "Cube.h"

namespace vmc
{

	class GridRenderer3D : public Greet::Renderer3D
	{
	private:
    Greet::Ref<Greet::Shader> simpleShader;

		Greet::EntityModel* emodel;
		Greet::Material* material;
		Greet::Mesh* mesh;


    Greet::Ref<Greet::Shader> lineShader;
    Greet::Ref<Greet::VertexArray> vao;
    Greet::Ref<Greet::VertexBuffer> vbo;
    Greet::Ref<Greet::Buffer> ibo;
		uint* m_indices;
	public:

		GridRenderer3D();

		void Begin(Greet::Camera* camera) override;
		void DrawCube(Greet::Camera* camera, const Greet::Vec3f& pos, const Greet::Vec3f& size, uint color, bool culling);
		virtual void Submit(Greet::Camera* camera, const Cube& cube);
		void DrawLineCube(Greet::Camera* camera, const Greet::Vec3f& pos, const Greet::Vec3f& size, const Greet::Vec4f& color);
		void DrawLine(Greet::Camera* camera, const Greet::Vec3f& start, const Greet::Vec3f& end, const Greet::Vec4f& color);
	};
}
