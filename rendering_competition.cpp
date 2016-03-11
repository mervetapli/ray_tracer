#include <core/image.h>
#include <core/color.h>
#include <rt/world.h>
#include <rt/renderer.h>
#include <rt/integrators/recraytrace.h>
#include <rt/cameras/perspective.h>
#include <rt/cameras/dofperspective.h>
#include <rt/groups/simplegroup.h>
#include <rt/groups/bvh.h>
#include <rt/primmod/instance.h>
#include <rt/loaders/obj.h>

#include <rt/solids/triangle.h>
#include <rt/solids/quad.h>
#include <rt/solids/sphere.h>
#include <rt/solids/disc.h>
#include <rt/solids/aabox.h>
#include <rt/lights/pointlight.h>
#include <rt/lights/arealight.h>

#include <rt/textures/constant.h>
#include <rt/textures/perlin.h>
#include <rt/textures/checkerboard.h>
#include <rt/textures/imagetex.h>
#include <rt/materials/flatmaterial.h>
#include <rt/materials/lambertian.h>
#include <rt/materials/phong.h>
#include <rt/materials/fuzzymirror.h>
#include <rt/materials/mirror.h>
#include <rt/materials/combine.h>
#include <rt/materials/glass.h>
#include <rt/coordmappers/tmapper.h>
#include <rt/primmod/bmap.h>

using namespace rt;

SimpleGroup* snow_globe(Point& pos, float scale)
{
	SimpleGroup* result = new SimpleGroup();

	//textures
	Texture* bluetex = new ConstantTexture(RGBColor(0.f, 0.f, 0.7f));
	Texture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
	Texture* whitetex = new ConstantTexture(RGBColor(130.f,80.f,30.f).clamp());

	//materials
	Material* stand = new FuzzyMirrorMaterial(2.485f, 3.433f, 0.05f);
	GlassMaterial* globe = new GlassMaterial(2.0f);
	Material* base = new LambertianMaterial(blacktex, whitetex);

	result->add(new Sphere(Point(pos.x_, pos.y_ - 25 * scale, pos.z_), 100 * scale, nullptr, stand));
	result->add(new Disc(Point(pos.x_, pos.y_ + 100 * scale, pos.z_), Vector(0, 1, 0), 66 * scale, nullptr, base));
	result->add(new Sphere(Point(pos.x_, pos.y_ + 150 * scale, pos.z_), 100 * scale, nullptr, globe));

	//object
	BVH* snowman = new BVH();
	loadOBJ(snowman, "models/guardian/", "test.obj");
	snowman->rebuildIndex();
	Instance* snowman_ = new Instance(snowman);
	snowman_->scale(20 * scale);
	snowman_->rotate(Vector(0, 1, 0), pi / 2);
	snowman_->translate(Vector(pos.x_ , pos.y_ + 150 * scale, pos.z_ ));

	result->add(snowman_);

	return result;
}

SimpleGroup* table(Point& pos, float scale)
{
	SimpleGroup* result = new SimpleGroup();

	//textures
	Texture* greytex = new ConstantTexture(RGBColor(128.f, 128.f, 128.f).clamp());
	Texture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));

	//materials
	Material* grey_ = new LambertianMaterial(blacktex, greytex);
	Material* mirror_ = new MirrorMaterial(2.485, 3.433);
	CombineMaterial* metal_ = new CombineMaterial();
	metal_->add(grey_, 0.75);
	metal_->add(mirror_, 0.25);

	//top bottom
	result->add(new Quad(Point(pos.x_ - 500.f*scale, pos.y_, pos.z_ - 300.f*scale), Vector(1000.f, 000.f, 000.f)*scale, Vector(000.f, 000.f, 600.f)*scale, nullptr, metal_));
	result->add(new Quad(Point(pos.x_ - 500.f*scale, pos.y_ - 40.f*scale, pos.z_ - 300.f*scale), Vector(1000.f, 000.f, 000.f)*scale, Vector(000.f, 000.f, 600.f)*scale, nullptr, metal_));

	//side
	result->add(new Quad(Point(pos.x_ - 500.f*scale, pos.y_, pos.z_ - 300.f*scale), Vector(1000.f, 000.f, 000.f)*scale, Vector(000.f, -40.f, 000.f)*scale, nullptr, metal_));
	result->add(new Quad(Point(pos.x_ - 500.f*scale, pos.y_, pos.z_ - 300.f*scale), Vector(000.f, -40.f, 000.f)*scale, Vector(000.f, 000.f, 600.f)*scale, nullptr, metal_));
	result->add(new Quad(Point(pos.x_ + 500.f*scale, pos.y_, pos.z_ + 300.f*scale), Vector(1000.f, 000.f, 000.f)*scale, Vector(000.f, -40.f, 000.f)*scale, nullptr, metal_));
	result->add(new Quad(Point(pos.x_ + 500.f*scale, pos.y_, pos.z_ + 300.f*scale), Vector(0000.f, -40.f, 000.f)*scale, Vector(000.f, 000.f, 600.f)*scale, nullptr, metal_));

	//legs
	result->add(new AABox(Point(pos.x_ - 450.f*scale, pos.y_ - 40.f*scale, pos.z_ - 250.f*scale), Point(pos.x_ - 400.f*scale, 0.f, pos.z_ - 200.f*scale), nullptr, metal_));
	result->add(new AABox(Point(pos.x_ + 450.f*scale, pos.y_ - 40.f*scale, pos.z_ + 250.f*scale), Point(pos.x_ + 400.f*scale, 0.f, pos.z_ + 200.f*scale), nullptr, metal_));
	result->add(new AABox(Point(pos.x_ + 450.f*scale, pos.y_ - 40.f*scale, pos.z_ - 250.f*scale), Point(pos.x_ + 400.f*scale, 0.f, pos.z_ - 200.f*scale), nullptr, metal_));
	result->add(new AABox(Point(pos.x_ - 450.f*scale, pos.y_ - 40.f*scale, pos.z_ + 250.f*scale), Point(pos.x_ - 400.f*scale, 0.f, pos.z_ + 200.f*scale), nullptr, metal_));

	return result;
}

SimpleGroup* mirror(Point& pos, float scale)
{
	SimpleGroup* result = new SimpleGroup();

	float z_len_mirror = 300.f*scale;
	float y_len_mirror = 150.f*scale;
	float mirror_edge_len = 20.f*scale;
	Point mirror_s_point(pos.x_ + 0.001f, pos.y_ - (y_len_mirror / 2) , pos.z_ - (z_len_mirror / 2) );
	Material* mirror_material = new MirrorMaterial(2.0f, 3.0f);
	result->add(new Quad(mirror_s_point, Vector(0.0f, 0.0f, z_len_mirror), Vector(0.0f, y_len_mirror, 0.0f), nullptr, mirror_material));
	
	//textures
	Texture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
	PerlinTexture* perlin_texture = new PerlinTexture(RGBColor(117.0f, 72.0f, 49.0f).clamp(), RGBColor(0.0f, 0.0f, 0.0f));
	perlin_texture->addOctave(0.5f, 5.0f);
	perlin_texture->addOctave(0.25f, 10.0f);
	perlin_texture->addOctave(0.125f, 20.0f);
	perlin_texture->addOctave(0.125f, 40.0f);
	
	//materials
	CombineMaterial* mirror_edge_material= new CombineMaterial();
	Material* mirror_edge_mirror = new MirrorMaterial(2.485f, 3.433f);
	Material* mirror_edge_lambertian = new LambertianMaterial(blacktex, perlin_texture);
	Material* mirror_edge_phong = new PhongMaterial(new ConstantTexture(RGBColor::rep(0.2f)), 16.0f);
	mirror_edge_material->add(mirror_edge_mirror, 0.25f);
	mirror_edge_material->add(mirror_edge_phong, 0.25f);
	mirror_edge_material->add(mirror_edge_lambertian, 0.50f);

	//mirror edges
	result->add(new Quad(Point(mirror_s_point.x_ , mirror_s_point.y_, mirror_s_point.z_+z_len_mirror),
		Vector(0.0f, 0.0f, mirror_edge_len), Vector(0.0f, y_len_mirror, 0.0f), nullptr, mirror_edge_material));
	result->add(new Quad(Point(mirror_s_point.x_, mirror_s_point.y_, mirror_s_point.z_),
		Vector(0.0f, 0.0f, - mirror_edge_len), Vector(0.0f, y_len_mirror, 0.0f), nullptr, mirror_edge_material));
	result->add(new Quad(Point(mirror_s_point.x_ , mirror_s_point.y_ + y_len_mirror, mirror_s_point.z_ - mirror_edge_len),
		Vector(0.0f, 0.0f, z_len_mirror + mirror_edge_len * 2), Vector(0.0f, mirror_edge_len, 0.0f), nullptr, mirror_edge_material));
	result->add(new Quad(Point(mirror_s_point.x_ , mirror_s_point.y_, mirror_s_point.z_ - mirror_edge_len),
		Vector(0.0f, 0.0f, z_len_mirror + mirror_edge_len * 2), Vector(0.0f, -mirror_edge_len, 0.0f), nullptr, mirror_edge_material));

	return result;
}

SimpleGroup* build_room(Point& pos, float scale)
{
	SimpleGroup* result = new SimpleGroup();
	
	//textures
	Texture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
	Texture* greytex = new ConstantTexture(RGBColor(128.f, 128.f, 128.f).clamp());
	PerlinTexture* floor_perlin = new PerlinTexture(RGBColor(102.0f, .0f, .0f).clamp(), RGBColor(1.0f, 1.0f, 1.0f));
	floor_perlin->addOctave(0.5f, 5.0f);
	floor_perlin->addOctave(0.25f, 10.0f);
	floor_perlin->addOctave(0.125f, 20.0f);
	floor_perlin->addOctave(0.125f, 40.0f);
	floor_perlin->addOctave(0.0625f, 80.0f);
	CheckerboardTexture* floor_checker = new CheckerboardTexture(RGBColor(0.0f, 0.0f, 0.0f), RGBColor(1.0f, 1.0f, 1.0f));
	ImageTexture* bmap = new ImageTexture("models/06_DISP.jpg");
	PerlinTexture* wall_perlin = new PerlinTexture(RGBColor(160, 160, 160).clamp(), RGBColor(0.0f, 0.0f, 0.0f));
	wall_perlin->addOctave(0.5f, 5.0f);
	wall_perlin->addOctave(0.25f, 10.0f);
	wall_perlin->addOctave(0.125f, 20.0f);
	wall_perlin->addOctave(0.125f, 40.0f);
	wall_perlin->addOctave(0.125f, 80.0f);
	Texture* whitetex = new ConstantTexture(RGBColor::rep(1.0f));
	Texture* bluetex = new ConstantTexture(RGBColor(150.f,255.f,255.f).clamp());

	//materials
	Material* floor_lambertian = new LambertianMaterial(blacktex, floor_checker);
	Material* floor_lambertian2 = new LambertianMaterial(blacktex, floor_perlin);
	LambertianMaterial* wall_lambertian = new LambertianMaterial(blacktex, bluetex);
	LambertianMaterial* wall_lambertian2 = new LambertianMaterial(blacktex, wall_perlin);
	Material* floor_mirror = new MirrorMaterial(0.0f, 0.0f);
	CombineMaterial* floor_combined = new CombineMaterial();
	floor_combined->add(floor_lambertian, 0.55f);
	floor_combined->add(floor_mirror, 0.05f);
	floor_combined->add(floor_lambertian2, 0.40f);
	Material* white = new LambertianMaterial(blacktex, whitetex);
	Material* black = new LambertianMaterial(blacktex, blacktex);
	CombineMaterial* wall = new CombineMaterial();
	wall->add(wall_lambertian, 0.75);
	wall->add(wall_lambertian2, 0.25);

	//mappers
	TriangleMapper* bottomleft = new TriangleMapper(Point(0, 0, 0), Point(3, 0, 0), Point(0, 3, 0));
	TriangleMapper* topright = new TriangleMapper(Point(3, 3, 0), Point(3, 0, 0), Point(0, 3, 0));
	TriangleMapper* bottomleft2 = new TriangleMapper(Point(0, 0, 0), Point(2, 0, 0), Point(0, 2, 0));
	TriangleMapper* topright2 = new TriangleMapper(Point(2, 2, 0), Point(2, 0, 0), Point(0, 2, 0));

	//floor
	result->add(new Triangle(Point(pos.x_ - 750.f*scale, pos.y_, pos.z_ - 500.f*scale), Point(pos.x_ - 750.f*scale, pos.y_, pos.z_ + 500 * scale), 
		Point(pos.x_ + 750.f*scale, pos.y_, pos.z_ - 500.f*scale), bottomleft, floor_combined));
	result->add(new Triangle(Point(pos.x_ + 750.f*scale, pos.y_, pos.z_ + 500.f*scale), Point(pos.x_ + 750.f*scale, pos.y_, pos.z_ - 500.f*scale), 
		Point(pos.x_ - 750.f*scale, pos.y_, pos.z_ + 500.f*scale), topright, floor_combined));

	//ceiling
	result->add(new BumpMapper(new Triangle(Point(pos.x_ - 750.f*scale, pos.y_ + 1000.f*scale, pos.z_ - 500.f*scale), Point(pos.x_ + 750.f*scale, pos.y_ + 1000.f*scale, pos.z_ - 500.f*scale), Point(pos.x_ - 750.f*scale, pos.y_ + 1000.f*scale, pos.z_ + 500*scale), bottomleft2, white),
		bmap, Point(0, 0, 0), Point(2, 0, 0), Point(0, 2, 0), 2.0f));
	result->add(new BumpMapper(new Triangle(Point(pos.x_ + 750.f*scale, pos.y_+1000.f*scale, pos.z_ + 500.f*scale), Point(pos.x_ + 750.f*scale, pos.y_+1000.f*scale, pos.z_ - 500.f*scale), Point(pos.x_ - 750.f*scale, pos.y_+1000.f*scale, pos.z_ + 500.f*scale), topright2, white),
		bmap, Point(2, 2, 0), Point(2, 0, 0), Point(0, 2, 0), 2.0f));

	//back wall
	result->add(new BumpMapper(new Triangle(Point(pos.x_ - 750.f*scale, pos.y_, pos.z_ + 500.f*scale), Point(pos.x_ + 750.f*scale, pos.y_, pos.z_ + 500.f*scale), Point(pos.x_ - 750.f*scale, pos.y_ + 1000.f*scale, pos.z_ + 500.f*scale), bottomleft2, wall),
		bmap, Point(0, 0, 0), Point(2, 0, 0), Point(0, 2, 0), 20.0f));
	result->add(new BumpMapper(new Triangle(Point(pos.x_ + 750.f*scale, pos.y_ + 1000.f*scale, pos.z_ + 500.f*scale), Point(pos.x_ + 750.f*scale, pos.y_, pos.z_ + 500.f*scale), Point(pos.x_ - 750.f*scale, pos.y_ + 1000.f*scale, pos.z_ + 500.f*scale), topright2, wall),
		bmap, Point(2, 2, 0), Point(2, 0, 0), Point(0, 2, 0), 20.0f));

	//right wall
	result->add(new BumpMapper(new Triangle(Point(pos.x_ - 750.f*scale, pos.y_, pos.z_ - 500.f*scale), Point(pos.x_ - 750.f*scale, pos.y_, pos.z_ + 500.f*scale), Point(pos.x_ - 750.f*scale, pos.y_ + 1000.f*scale, pos.z_ - 500.f*scale), bottomleft2, wall),
		bmap, Point(0, 0, 0), Point(2, 0, 0), Point(0, 2, 0), 20.0f));
	result->add(new BumpMapper(new Triangle(Point(pos.x_ - 750.f*scale, pos.y_ + 1000.f*scale, pos.z_ + 500.f*scale), Point(pos.x_ - 750.f*scale, pos.y_, pos.z_ + 500.f*scale), Point(pos.x_ - 750.f*scale, pos.y_ + 1000.f*scale, pos.z_ - 500.f*scale), topright2, wall),
		bmap, Point(2, 2, 0), Point(2, 0, 0), Point(0, 2, 0), 20.0f));

	//left wall
	result->add(new BumpMapper(new Triangle(Point(pos.x_ + 750.f*scale, pos.y_, pos.z_ - 500.f*scale), Point(pos.x_ + 750.f*scale, pos.y_, pos.z_ + 500.f*scale), Point(pos.x_ + 750.f*scale, pos.y_ + 1000.f*scale, pos.z_ - 500.f*scale), bottomleft2, wall),
		bmap, Point(0, 0, 0), Point(2, 0, 0), Point(0, 2, 0), 20.0f));
	result->add(new BumpMapper(new Triangle(Point(pos.x_ + 750.f*scale, pos.y_ + 1000.f*scale, pos.z_ + 500.f*scale), Point(pos.x_ + 750.f*scale, pos.y_, pos.z_ + 500.f*scale), Point(pos.x_ + 750.f*scale, pos.y_ + 1000.f*scale, pos.z_ - 500.f*scale), topright2, wall),
		bmap, Point(2, 2, 0), Point(2, 0, 0), Point(0, 2, 0), 20.0f));

	//front wall
	result->add(new Quad(Point(pos.x_ + 750.f*scale, pos.y_, pos.z_ - 500.f*scale), Vector(-1500.f, 000.f, 000.f)*scale, Vector(000.f, 1000.f, 000.f)*scale, nullptr, black));

	return result;
}

SimpleGroup* light(Point& pos, float scale)
{
	SimpleGroup* result = new SimpleGroup();

	Texture* greytex = new ConstantTexture(RGBColor(128.f, 128.f, 128.f).clamp());
	Texture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));

	Material* grey_ = new LambertianMaterial(blacktex, greytex);
	Material* mirror_ = new MirrorMaterial(2.485, 3.433);
	CombineMaterial* metal_ = new CombineMaterial();
	metal_->add(grey_, 0.75);
	metal_->add(mirror_, 0.25);

	result->add(new Quad(Point(pos.x_ - 250.f*scale, pos.y_, pos.z_ + 50.f*scale), Vector(0.0f, -50.f, 0.0f)*scale, Vector(250.0f, 0.0f, 0.0f)*scale, nullptr, metal_));
	result->add(new Quad(Point(pos.x_ - 250.f*scale, pos.y_, pos.z_ + 50.f*scale), Vector(0.0f, -50.f, 0.0f)*scale, Vector(0.0f, 0.0f, -100.0f)*scale, nullptr, metal_));
	result->add(new Quad(Point(pos.x_ + 250.f*scale, pos.y_, pos.z_ - 50.f*scale), Vector(0.0f, -50.f, 0.0f)*scale, Vector(-250.0f, 0.0f, 0.0f)*scale, nullptr, metal_));
	result->add(new Quad(Point(pos.x_ + 250.f*scale, pos.y_, pos.z_ - 50.f*scale), Vector(0.0f, -50.f, 0.0f)*scale, Vector(0.0f, 0.0f, 100.0f)*scale, nullptr, metal_));

	return result;
}

void rendering_competition()
{
	static const float scale = 0.001f;
	Image img(1280, 1280);
	SimpleGroup *scene = new SimpleGroup();

	//build
	scene->add(build_room(Point(0, 0, 0), 2*scale));

	Instance* table_ = new Instance(table(Point(0.f, 0.f, 0.f), scale));
	table_->scale(1.5f);
	table_->rotate(Vector(0.f, 1.f, 0.f), pi / 8);
	table_->translate(Vector(800.f, 400.f, -100.f)*scale);

	scene->add(table_);
	scene->add(light(Point(0.0f, 2.0f, 0.0f), scale));
	scene->add(snow_globe(Point(0.4, 0.4, -0.2), scale));
	scene->add(mirror(Point(-1.5, 0.9, -0.2), 2*scale));

	World world;
	world.scene = scene;

	//objects
	BVH* dead = new BVH();
	loadOBJ(dead, "models/dead_man/", "dead_man.obj");
	dead->rebuildIndex();
	Instance* dead_ = new Instance(dead);
	dead_->scale(150*scale);
	dead_->rotate(Vector(0.0f, 1.0f, 0.0f), - 3*pi / 4);
	dead_->translate(Vector(1.f, 0.0f, 0.5f));

	scene->add(dead_);

	//lights
	Texture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
	ConstantTexture* lightsrctex = new ConstantTexture(RGBColor::rep(35.0f));
	Material* lightsource = new LambertianMaterial(lightsrctex, blacktex);

	Quad* light = new Quad(Point(-250.f * scale, 1900.f*scale, -50.f * scale), Vector(500 * scale, 0, 0), Vector(0, 0, 100 * scale), nullptr, lightsource);
	AreaLight als(light);
	world.light.push_back(&als);

	//camera
	DOFPerspectiveCamera* cam = new DOFPerspectiveCamera(Point(1.49f, 1.0f, -0.99f), Vector(-1.7, -0.6, 0.75).normalize(), Vector(0, 1, 0), 0.686f, 0.686f, 1.35f, 0.030f);

	//integrator
	RecursiveRayTracingIntegrator integrator(&world);

	//render image
	Renderer engine(cam, &integrator);
	engine.setSamples(1);
	engine.render(img);
	img.writePNG("final.png");
}
