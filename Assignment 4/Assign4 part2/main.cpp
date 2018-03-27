#include "main.h"

#include "CS148/Scene.h"

using namespace std;

//****************************************************
// Global Variables
//****************************************************
Viewport * viewport;
mat4 viewToWorld;
World * world;
Film * film;
Scene * scene;

double max(double a, double b) {
	if (a > b)
		return a;
	else
		return b;
}

RGB ambientShading(Primitive *p) {
	double ma = p -> getMaterial().getMA();
    //ambient effect
	return world -> getAmbientLightColor() * ma * p -> getColor();
}

RGB lambertShading(Primitive *p, vec4& point) {
	RGB retcolor(0,0,0);
	vec4 lv;
	double mL = p -> getMaterial(). getML();
	for(vector<Light*>::const_iterator iter = world -> getLightsBeginIterator(); iter != world -> getLightsEndIterator() ; iter++) {
		//getting every light source
		Light* light = *iter;
        //incident vector
		light -> getIncidenceVector(point, lv);
		//effect of lambert
		retcolor += mL * p -> getColor() * light -> getColor() * max(p -> calculateNormal(point) * lv, 0);
	}
	return retcolor;
}

RGB specularShading(Primitive *p, Ray &r, vec4 point) {
	double mS = p->getMaterial().getMS();
	double msm = p->getMaterial().getMSM();
	double msp = p->getMaterial().getMSP();
    vec4 incdnt;
	RGB retColor(0, 0, 0);

	for (vector<Light*>::const_iterator i = world->getLightsBeginIterator(); i != world->getLightsEndIterator(); i++) {
		Light* light = *i;
        //colour from specular effect due to metalness
		RGB S = (msm * p->getColor()) + ((1 - msm) *RGB(1, 1, 1));
        //direction of ray from eye
		vec4 u = r.direction().normalize();
		//incident light vector
		light->getIncidenceVector(point, incdnt);
        //reflectance vector
		vec4 rf = ((incdnt) - 2 * (p->calculateNormal(point) * incdnt) * p->calculateNormal(point)).normalize();
        //colour to return
		retColor += mS * S *light->getColor(point)* pow((double)max(rf*u, 0), msp);
	}
	return retColor;
}

Ray getReflectedRay(Primitive * p, Ray & r, vec4 point) {
    //incident vector
	vec4 incident = r.direction().normalize();
    //start point
    vec4 one = point.normalize();
    //end point
	vec4 two = (incident - (2 * (p->calculateNormal(point) * incident) * p->calculateNormal(point))).normalize();
	//resultat vector
	vec4 end = (one + two).normalize();
    //resultant ray
	return ((vec3)one, (vec3)end, 0.1);
}


RGB traceRay(Ray & ray, int depth) {
    RGB retColor(0,0,0);
	double t;
	Primitive* worldObject = world -> intersect(ray, t);
	vec4 point = ray.start() + ray.direction()*t;
	if(worldObject) {
		retColor += ambientShading(worldObject);
		retColor += lambertShading(worldObject, point);
		retColor += specularShading(worldObject, ray, point);
		
		if(depth < 1) {
            //the reflected ray will give us the effect of different objects in vacanity of the this objects
			Ray reflected = getReflectedRay(worldObject, ray, point);
			retColor = retColor + traceRay(reflected, depth + 1);
		}	
	}
    //check if no colour is out of range
	if(retColor[RED] > 1) retColor[RED] = 1;
	if(retColor[BLUE] > 1) retColor[BLUE] = 1;
	if(retColor[GREEN] > 1) retColor[GREEN] = 1;
    return retColor;
}

//-------------------------------------------------------------------------------
// Here you write your main render function. This calls the raycast method
// on each ray, which you generate using the Sampler and Ray classes.
void renderWithRaycasting() {
    Sample sample;    //This is the point on the viewport being sampled.
    Ray ray;        //This is the ray being traced from the eye through the point.
    RGB c(0,0,0);

    viewport->resetSampler();
    while(viewport->getSample(sample)) {  //just gets a 2d sample position on the film
        c = RGB(0,0,0);
    	ray = viewport->createViewingRay(sample);  //convert the 2d sample position to a 3d ray
        ray.transform(viewToWorld);  //transform this to world space
        c = traceRay(ray, 0);
        film->expose(c, sample);
    }
	film->bakeAndSave();
	cout << "Image saved!" << endl;
}

//-------------------------------------------------------------------------------
// This traverses the read-in scene file's DAG and builds a list of
// primitives, lights and the viewport object. See World.h
void sceneToWorld(SceneInstance *inst, mat4 localToWorld, int time) {
    if (inst == NULL)
        return;

    mat4 nodeXform;
    inst->computeTransform(nodeXform,time);
    localToWorld = localToWorld * nodeXform;

    SceneGroup *g = inst->getChild();
    if (g == NULL) { // for example if the whole scene fails to load
        cout << "ERROR: We arrived at an instance with no child?!" << endl;
        return;
    }

    int ccount = g->getChildCount();
    for (int i = 0; i < ccount; i++) {
        sceneToWorld(g->getChild(i), localToWorld, 0);
    }

    CameraInfo f;
    if (g->computeCamera(f, time)) {
        viewToWorld = localToWorld;

        if (viewport != NULL)
            delete viewport;

        vec4 eye(0.0, 0.0, 0.0, 1.0);
        vec4 LL(f.sides[FRUS_LEFT], f.sides[FRUS_BOTTOM], -f.sides[FRUS_NEAR], 1.0);
        vec4 UL(f.sides[FRUS_LEFT], f.sides[FRUS_TOP], -f.sides[FRUS_NEAR], 1.0);
        vec4 LR(f.sides[FRUS_RIGHT], f.sides[FRUS_BOTTOM], -f.sides[FRUS_NEAR], 1.0);
        vec4 UR(f.sides[FRUS_RIGHT], f.sides[FRUS_TOP], -f.sides[FRUS_NEAR], 1.0);

        viewport = new Viewport(eye, LL, UL, LR, UR, IMAGE_WIDTH, IMAGE_HEIGHT);
    }

    LightInfo l;
    if (g->computeLight(l, time)) {
        if (l.type == LIGHT_AMBIENT) {
            RGB amb = world->getAmbientLightColor();
            world->setAmbientLightColor(amb + l.color);
        }
        else if (l.type == LIGHT_DIRECTIONAL) {
            DirectionalLight *li = new DirectionalLight(l.color);
            vec4 dir(0,0,-1,0);
            li->setDirection(localToWorld*dir);
            world->addLight(li);
        }
        else if (l.type == LIGHT_POINT) {
            PointLight *li = new PointLight(l.color, l.falloff, l.deadDistance);
            vec4 pos(0,0,0,1);
            li->setPosition(localToWorld*pos);
            world->addLight(li);
        }
        else if (l.type == LIGHT_SPOT) {
            throw "oh no";
        }
    }

    double r;
    MaterialInfo m;
    if (g->computeSphere(r, m, time)) {
        Material mat(m.k[0],m.k[1],m.k[2],m.k[3],m.k[4],m.k[MAT_MS],m.k[5],m.k[6]);
        Sphere *sph = new Sphere(r, m.color, mat, localToWorld);
        world->addPrimitive(sph);
    }

    OBJTriangleMesh *t;
    if (g->computeMesh(t, m, time)) {
        Material mat(m.k[0],m.k[1],m.k[2],m.k[3],m.k[4],m.k[MAT_MS],m.k[5],m.k[6]);
        for (vector<OBJTriangle*>::iterator it = t->triangles.begin(); it != t->triangles.end(); ++it) {
            Triangle *tri = new Triangle(
                                t->vertices[ (**it).ind[0] ]->pos,
                                t->vertices[ (**it).ind[1] ]->pos,
                                t->vertices[ (**it).ind[2] ]->pos,
                                m.color, mat, localToWorld);
            world->addPrimitive(tri);
        }
    }

}

//-------------------------------------------------------------------------------
/// Initialize the environment
int main(int argc,char** argv) {
	
	if (argc != 3) {
		cout << "USAGE: raytracer scene.scd output.png" << endl;
		exit(1);
	}
	
	film = new Film(IMAGE_WIDTH, IMAGE_HEIGHT, argv[2]);
    scene = new Scene(argv[1]);
    viewToWorld = identity3D();
    world = new World();
    sceneToWorld(scene->getRoot(), identity3D(), 0);
	cout << "Imported Scene File." << endl;
	world->printStats();
	renderWithRaycasting();
}
