/**
 *  \file RigidBodyNewMover.cpp
 *  \brief A mover that transforms a rigid body
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 *
 */
#include <IMP/membrane/RigidBodyNewMover.h>
#include <IMP/core/XYZ.h>
#include <IMP/algebra/vector_generators.h>
IMPMEMBRANE_BEGIN_NAMESPACE

RigidBodyNewMover::RigidBodyNewMover(core::RigidBody d, Float max_x_translation,
                               Float max_y_translation, Float max_z_translation,
                               Float max_angle) {
  IMP_LOG(VERBOSE,"start RigidBodyNewMover constructor");
  max_x_translation_=max_x_translation;
  max_y_translation_=max_y_translation;
  max_z_translation_=max_z_translation;
  max_angle_ =max_angle;
  d_= d;
  IMP_LOG(VERBOSE,"finish mover construction" << std::endl);
}

void RigidBodyNewMover::propose_move(Float f) {
  IMP_LOG(VERBOSE,"RigidBodyNewMover:: propose move f is  : " << f <<std::endl);
  {
    ::boost::uniform_real<> rand(0,1);
    double fc =rand(random_number_generator);
    if (fc > f) return;
  }
  last_transformation_= d_.get_reference_frame().get_transformation_to();
  algebra::VectorD<3> tr_x
    = algebra::get_random_vector_in(algebra::Sphere3D(d_.get_coordinates(),
                                                      max_x_translation_));
  algebra::VectorD<3> tr_y
    = algebra::get_random_vector_in(algebra::Sphere3D(d_.get_coordinates(),
                                                      max_y_translation_));
  algebra::VectorD<3> tr_z
    = algebra::get_random_vector_in(algebra::Sphere3D(d_.get_coordinates(),
                                                      max_z_translation_));

  algebra::VectorD<3> translation
    = algebra::VectorD<3>(tr_x[0],tr_y[1],tr_z[2]);


  ::boost::uniform_real<> rand(-max_angle_,max_angle_);
  Float angle =rand(random_number_generator);

  algebra::Rotation3D rotz
    = algebra::get_rotation_about_axis(algebra::Vector3D(0,0,1),  angle);

  algebra::Rotation3D tilt
    = algebra::get_rotation_about_axis(algebra::Vector3D(0,1,0),  angle);

  algebra::Rotation3D swing
    = algebra::get_rotation_about_axis(algebra::Vector3D(0,0,1), angle);

  algebra::Rotation3D rc
    = d_.get_reference_frame().get_transformation_to().get_rotation()
      *(swing*(tilt*rotz));

  algebra::Transformation3D t(rc, translation);
  IMP_LOG(VERBOSE,"RigidBodyNewMover:: propose move : " << t << std::endl);
  d_.set_reference_frame(algebra::ReferenceFrame3D(t));
}



void RigidBodyNewMover::reset_move() {
  d_.set_reference_frame(algebra::ReferenceFrame3D(last_transformation_));
  last_transformation_= algebra::Transformation3D();
}


void RigidBodyNewMover::do_show(std::ostream &out) const {
  out << "max x translation: " << max_x_translation_ << "\n";
  out << "max y translation: " << max_y_translation_ << "\n";
  out << "max z translation: " << max_z_translation_ << "\n";
  out << "max angle: " << max_angle_ << "\n";
}
IMPMEMBRANE_END_NAMESPACE
