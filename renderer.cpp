#include <rt/renderer.h>
#include <algorithm>
#include <core/image.h>
#include <rt/ray.h>
#include <rt/cameras/camera.h>
#include <rt/integrators/integrator.h>
#include <core/random.h>

using namespace rt;

RGBColor a1computeColor(uint x, uint y, uint width, uint height);
RGBColor a2computeColor(const Ray& r);

//ctor
Renderer::Renderer(Camera* camera, Integrator* integrator)
	: camera_(camera)
	, integrator_(integrator)
	, samples_(1)
{}

//setSamples
void Renderer::setSamples(uint samples)
{
	samples_ = samples;
}

//render
void Renderer::render(Image& img)
{
	uint width = img.width();
	uint height = img.height();

	for (uint i = 0; i < width; ++i)
	{
		for (uint j = 0; j < height; ++j)
		{
			//a1computeColor returns a clamped([0,1]) rgb value
			//multiply it with 255
			if (samples_ == 1)
			{
				RGBColor color = integrator_->getRadiance(camera_->getPrimaryRay(2 * (i + 0.5f) / width - 1, 2 * (j + 0.5f) / height - 1));
				img(i, j) = RGBColor(std::min(color.r, 1.0f), std::min(color.g, 1.0f), std::min(color.b, 1.0f)) * 255;
			}

			else
			{
				RGBColor color = RGBColor::rep(0.0f);

				for (uint k = 0; k < samples_; ++k)
				{
					color = color +
						integrator_->getRadiance(camera_->getPrimaryRay(2 * (i + random()) / width - 1, 2 * (j + random()) / height - 1));
				}

				color = color / samples_;
				img(i, j) = RGBColor(std::min(color.r, 1.0f), std::min(color.g, 1.0f), std::min(color.b, 1.0f)) * 255;
			}
		}
	}
}

//test_render1
void Renderer::test_render1(Image& img)
{
	/*uint width = img.width();
	uint height = img.height();

	for (uint i = 0; i < width; ++i)
	{
		for (uint j = 0; j < height; ++j)
		{
			//a1computeColor returns a clamped([0,1]) rgb value
			//multiply it with 255
			img(i, j) = a1computeColor(i, j, width, height) * 255;
		}
	}*/
}

//test_render2
void Renderer::test_render2(Image& img)
{
	/*uint width = img.width();
	uint height = img.height();

	for (uint i = 0; i < width; ++i)
	{
		for (uint j = 0; j < height; ++j)
		{
			//a2computeColor returns a clamped([0,1]) rgb value
			//multiply it with 255
			img(i, j) = a2computeColor(cam->getPrimaryRay(2 * (i + 0.5) / width - 1, 2 * (j + 0.5) / height - 1)) * 255;
		}
	}*/
}