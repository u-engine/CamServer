#include "ofxBox2d.h"

#pragma comment(lib, "Box2d.lib")

// ------------------------------------------------------ 
ofxBox2d::ofxBox2d() {
	
}
// ------------------------------------------------------ init
void ofxBox2d::init() {
	
	//settings
	bHasContactListener = false;
	bCheckBounds		= false;
	bEnableGrabbing		= true;
	bWorldCreated		= false;
	scale				= OFX_BOX2D_SCALE;
	doSleep				= true;
	
	//gravity
	gravity.Set(0, 5.0f);
	setFPS(60.0);
	
	//mouse grabbing
	mouseJoint = NULL;
	ground	   = NULL;
	
	//debug drawer
	debugRender.setScale(scale);
	debugRender.SetFlags(1);
	
	worldAABB.lowerBound.Set(-100.0f, -100.0f);
	worldAABB.upperBound.Set(100.0f, 100.0f);
	
	world = new b2World(worldAABB, b2Vec2(gravity.x, gravity.y), doSleep);
	world->SetDebugDraw(&debugRender);
	
	
	//float bw = ofGetWidth()/scale;
	//ofLog(OF_LOG_NOTICE, "- Box2D Created -\n"); 

	bWorldCreated = true;
	
}


// ------------------------------------------------------ grab shapes 
void ofxBox2d::setContactListener(ofxBox2dContactListener * listener) {
	
	if(world != NULL) {
		bHasContactListener = true;
		world->SetContactListener(listener);
	}
	else {
		printf("--- you need a world ---\n");	
	}
}
 

// ------------------------------------------------------ set gravity
void ofxBox2d::setGravity(float x, float y) {
	world->SetGravity(b2Vec2(x, y));
}
void ofxBox2d::setGravity(b2Vec2 pt) {
	world->SetGravity(b2Vec2(pt.x, pt.y));
}

// ------------------------------------------------------ set bounds
void ofxBox2d::setBounds(b2Vec2 lowBounds, b2Vec2 upBounds) {
	// still need to work on this...
}

// ------------------------------------------------------ create bounds
void ofxBox2d::createFloor(float floorWidth, float bottom) {
	
	if(!bWorldCreated) return;
	
	b2BodyDef bd;
	bd.position.Set(0, 0);
	ground = world->CreateBody(&bd);	
	b2PolygonDef sd;
	sd.filter.groupIndex = 1;
	
	sd.density = 0.0f;
	sd.restitution = 0.0f;
	sd.friction = 0.6;
	float thick = 5/OFX_BOX2D_SCALE;
	
	//bottom
	sd.SetAsBox((floorWidth/OFX_BOX2D_SCALE)/2, thick, 
				b2Vec2((floorWidth/OFX_BOX2D_SCALE)/2, (bottom-thick)/OFX_BOX2D_SCALE), 0.0);
	ground->CreateShape(&sd);
}

// ------------------------------------------------------ create bounds
void ofxBox2d::createBounds(float x, float y, float w, float h) {
	
	if(!bWorldCreated) return;
	
	b2BodyDef bd;
	bd.position.Set(0, 0);
	ground = world->CreateBody(&bd);	
	b2PolygonDef sd;
	sd.filter.groupIndex = 1;
	
	sd.density = 0.0f;
	sd.restitution = 0.0f;
	sd.friction = 0.6;
	float thick = 30/OFX_BOX2D_SCALE;
	// w h x y r 
	//right
	sd.SetAsBox(thick, (h/OFX_BOX2D_SCALE)/2, b2Vec2((w/OFX_BOX2D_SCALE), (h/OFX_BOX2D_SCALE)/2), 0.0);
	ground->CreateShape(&sd);
	//left
	sd.SetAsBox(thick, (h/OFX_BOX2D_SCALE)/2, b2Vec2(0, (h/OFX_BOX2D_SCALE)/2), 0.0);
	ground->CreateShape(&sd);
	//top
	sd.SetAsBox((w/OFX_BOX2D_SCALE)/2, thick, b2Vec2((w/OFX_BOX2D_SCALE)/2, 0), 0.0);
	ground->CreateShape(&sd);
	//bottom
	sd.SetAsBox((w/OFX_BOX2D_SCALE)/2, thick, b2Vec2((w/OFX_BOX2D_SCALE)/2, h/OFX_BOX2D_SCALE), 0.0);
	ground->CreateShape(&sd);
}

// ------------------------------------------------------ check if shapes are out of bounds
void ofxBox2d::checkBounds(bool b) {
	bCheckBounds = b;
}

// ------------------------------------------------------ 
void ofxBox2d::update() {
	
	// destroy the object if we are out of the bounds
	if(bCheckBounds) {
		/*
		 float top = 0;
		 float bottom = ofGetHeight();
		 float right = ofGetWidth();
		 float left = 0;
		 
		 b2Body* node = world->GetBodyList();
		 while(node) {
		 b2Body* b = node;
		 node = node->GetNext();
		 b2Vec2 p = b->GetPosition();
		 ofxBox2dBaseShape* base = (ofxBox2dBaseShape*)b->GetUserData();
		 if(base) {
		 //printf("dead:%i\n", base->dead);
		 
		 if(p.y*OFX_BOX2D_SCALE > bottom) {
		 base->dead = true;
		 world->DestroyBody(b);
		 }
		 if(p.y*OFX_BOX2D_SCALE < top) {
		 base->dead = true;
		 world->DestroyBody(b);
		 }
		 if(p.x*OFX_BOX2D_SCALE > right) {
		 base->dead = true;
		 world->DestroyBody(b);
		 }
		 if(p.x*OFX_BOX2D_SCALE < left) {
		 base->dead = true;
		 world->DestroyBody(b);
		 }
		 */
		
		
	}
	
	float	timeStep		   = (1.0f / fps);
	int		velocityIterations = 40;
	int		positionIterations = 20;
	
	world->Step(timeStep, velocityIterations, positionIterations);
	world->Validate();	
	
}
//
//// ------------------------------------------------------ 
//void ofxBox2d::draw() {
//	
//	if(mouseJoint) {
//		b2Body* mbody = mouseJoint->GetBody2();
//		b2Vec2 p1 = mbody->GetWorldPoint(mouseJoint->m_localAnchor);
//		b2Vec2 p2 = mouseJoint->m_target;
//		
//		p1 *= OFX_BOX2D_SCALE;
//		p2 *= OFX_BOX2D_SCALE;
//		
//		//draw a line from touched shape
//		ofEnableAlphaBlending();
//		ofSetLineWidth(2.0);
//		ofSetColor(200, 200, 200, 200);
//		ofLine(p1.x, p1.y, p2.x, p2.y);
//		ofNoFill();
//		ofSetLineWidth(1.0);
//		ofCircle(p1.x, p1.y, 2);
//		ofCircle(p2.x, p2.y, 5);
//		ofDisableAlphaBlending();
//	}
//	
//	//draw the ground
//	if(ground) {
//		for(b2Shape* s=ground->GetShapeList(); s; s=s->GetNext()) {
//			
//			const b2XForm& xf = ground->GetXForm();		
//			b2PolygonShape* poly = (b2PolygonShape*)s;
//			int count = poly->GetVertexCount();
//			const b2Vec2* verts = poly->GetVertices();
//			ofEnableAlphaBlending();
//			ofFill();
//			ofSetColor(90, 90, 90, 100);
//			ofBeginShape();
//			for(int j=0; j<count; j++) {
//				
//				b2Vec2 pt = b2Mul(xf, verts[j]);
//				
//				ofVertex(pt.x*OFX_BOX2D_SCALE, pt.y*OFX_BOX2D_SCALE);
//			}
//			ofEndShape();
//			ofDisableAlphaBlending();
//		}
//	}
//}
