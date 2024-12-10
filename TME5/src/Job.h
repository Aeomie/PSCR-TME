#pragma once
#include "Color.h"
#include "Scene.h"
#include "Sphere.h"
#include "Rayon.h"
#include "Barrier.h"
#include "Vec3D.h"
#include <vector>
#include <limits>

namespace pr
{
	class Job
	{
	public:
		virtual void run() = 0;
		virtual ~Job() {};
	};

	class DrawJobBarrier : public Job
	{
		pr::Scene scene;
		Color *pixels;				   // Change to pointer for dynamic allocation
		const Scene::screen_t &screen; // Use const reference to avoid copying
		Barrier barrier;
		std::vector<Vec3D> lights;
		int y;
		int x;
		int height;

		Color computeColor(const Sphere &obj, const Rayon &ray, const Vec3D &camera, const std::vector<Vec3D> &lights)
		{
			Color finalColor = obj.getColor();
			Vec3D rayInter = (ray.dest - ray.ori).normalize() * obj.intersects(ray);
			Vec3D intersection = rayInter + camera;
			Vec3D normal = obj.getNormale(intersection);
			double dt = 0;

			for (const auto &light : lights)
			{
				Vec3D toLight = (light - intersection);
				if (obj.intersects(Rayon(light, intersection)) >= toLight.length() - 0.05)
				{
					dt += toLight.normalize() & normal;
				}
			}

			finalColor = finalColor * dt + finalColor * 0.2; // Ambient light contribution
			return finalColor;
		}

		int findClosestInter(const Scene &scene, const Rayon &ray)
		{
			auto minz = std::numeric_limits<float>::max();
			int targetSphere = -1;
			int index = 0;
			for (const auto &obj : scene)
			{
				auto zInter = obj.intersects(ray);
				if (zInter < minz)
				{
					minz = zInter;
					targetSphere = index;
				}
				index++;
			}
			return targetSphere;
		}

	public:
		DrawJobBarrier(pr::Scene scene, const std::vector<Vec3D> &lights, int y, int x, Barrier b)
			: scene(scene), pixels(new Color[scene.getWidth() * scene.getHeight()]), screen(scene.getScreenPoints()), lights(lights), y(y), x(x), barrier(b)
		{
			height = scene.getHeight(); // Store height
		}

		void run() override
		{
			auto &screenPoint = screen[y][x];
			Rayon ray(scene.getCameraPos(), screenPoint);

			int targetSphere = findClosestInter(scene, ray);

			if (targetSphere == -1)
			{
				return; // Use return instead of continue
			}
			else
			{
				const Sphere &obj = *(scene.begin() + targetSphere);
				Color finalColor = computeColor(obj, ray, scene.getCameraPos(), lights);
				Color &pixel = pixels[y * scene.getWidth() + x]; // Corrected indexing
				pixel = finalColor;
			}
			barrier.done();
		}

		~DrawJobBarrier()
		{
			delete[] pixels; // Properly deallocate memory
		}
	};
}
