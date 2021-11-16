#ifndef CONE_H
#define CONE_H

#include "hittable.h"
#include "vec3.h"

class cone : public hittable {
   public:
       cone() {}
       cone(point3 cen, double r, double h, shared_ptr<material> m)
           : center(cen), radius(r), height(h), mat_ptr(m) {};

       virtual bool hit(
           const ray& r, double t_min, double t_max, hit_record& rec) const override;

   public:
       point3 center;
       double radius;
       double height;
       shared_ptr<material> mat_ptr;
};

bool cone::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    double Xe = r.origin()[0];
    double Ye = r.origin()[1];
    double Ze = r.origin()[2];
    double Xd = r.direction()[0];
    double Yd = r.direction()[1];
    double Zd = r.direction()[2];
    double Xc = center[0];
    double Yc = center[1];
    double Zc = center[2];
    double rr = radius;
    double hh = height;

    double tan = (rr / hh) * (rr / hh);

    double A = Xe - Xc;
    double B = Ze - Zc;
    double D = hh - Ye + Yc;

    double a = (Xd*Xd) - (tan*Yd*Yd) + (Zd*Zd);
    double b = (2*A*Xd) + (2*B*Zd) + (2*tan*D*Yd);
    double c = (A*A) + (B*B) - (tan*D*D);

    double delta = b*b - 4*(a*c);
    if(delta < 0.0 || fabs(delta) < 0.001) return false;

    double t1 = (-b - sqrt(delta))/(2*a);
    double t2 = (-b + sqrt(delta))/(2*a);
    double t;
    
    if (t_min < t1 && t1 < t_max) t = t1;
    else if (t_min < t2 && t2 < t_max) t = t2;
    else return false;

    float r_ = Ye + Yd * t;

    if ((r_ > Yc) && (r_ < Yc + height)) {}
    else return false;

    rec.t = t;
    rec.p = r.at(rec.t);

    double k = sqrt((rec.p[0]-Xc)*(rec.p[0]-Xc) + (rec.p[2]-Zc)*(rec.p[2]-Zc));
    vec3 outward_normal = vec3 (rec.p[0]-Xc, k*(radius/height), rec.p[2]-Zc);
    double length = outward_normal.length();
    outward_normal = vec3(outward_normal[0]/length, outward_normal[1]/length, outward_normal[2]/length);

    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

#endif
