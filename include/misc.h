#pragma once

// Helper stuff
#define PI 3.1415
#define degreesToRadians(deg) deg*(PI/180.0f)
// movement increment in meters (blocks) per second
#define VELOCITY_INCREMENT 200.0f

// TODO solve better (added this to mark where to enable inverted mouse)
#define INVERT_MOUSE_MODIFIER 1

/**
 * TODO move to PhysicsHandler?
 */
void updatePhysics(GameContext* context);
