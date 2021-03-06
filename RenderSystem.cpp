#include <iostream>
#include <ctime>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "SOIL.h"
#include "skilLib.h"
#include "systems.h"
#include "components.h"
#include "messages.h"
#include <math.h>

extern Engine* theEngine;
const float PI = 3.141592654;

RenderSystem::RenderSystem(){
	id = SYSTEM_RENDER;
}

bool SHOW_VECTORS = true;

int renderMode(Entity* ent){

  /*
    0 - Error, do nothing
    1 - Wireframe
    2 - Color
    3 - Texture
   */

  if(ent->hasComponent(COMPONENT_POSITION) && ent->hasComponent(COMPONENT_DIMENSIONS)){
    if(ent->hasComponent(COMPONENT_COLOR)) return 2;
    if(ent->hasComponent(COMPONENT_TEXTURE)) return 3;
    return 1;
  }
  if(ent->hasComponent(COMPONENT_TEXTMESSAGE)) return 4;
  
  return 0;
}

void RenderSystem::update(float delta){
  for(int iterator = 0; iterator < numEntities(); iterator++){
    
    Entity *ent = entityAt(iterator);
    int mode = renderMode(ent);
    Color* color;
    Dimensions* dim;
    Position* pos;
    Texture* tex;
    
    float x, y, r, w, h;
    
    if(mode > 0){
      if(mode != 4){
        dim = static_cast<Dimensions*>(ent->getComponent(COMPONENT_DIMENSIONS));
          
        w = dim->getWidth();
        h = dim->getHeight();

      }else{

        w = h = 0;
      
      }

      pos = static_cast<Position*>(ent->getComponent(COMPONENT_POSITION));
      x = pos->getX();
      y = pos->getY();
      r = pos->getR();

    }else{

      // Default to zero
      x = y = r = w = h = 0;

    }

    // x and y corners
    float xC[4] = {x-w/2, x+w/2, x+w/2, x-w/2};
    float yC[4] = {y-h/2, y-h/2, y+h/2, y+h/2};

    if(r != 0){
      float l = sqrt(pow(w/2,2) + pow(h/2, 2));
      float thetas[4] = {7.0*PI/4.0, PI/4.0, 3.0*PI/4.0, 5.0*PI/4.0};


      for(int i = 0; i<4; i++){
        xC[i] = x + ( cos(r + thetas[i]) * l );
        yC[i] = y + ( sin(r +thetas[i]) * l );

      }

    }


    // Wireframe Mode
    if(mode == 1){
      glDisable(GL_TEXTURE_2D);
      glColor3f(0.0, 1.0, 0.3);

      for(int i = 0; i < 4; i++){
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex2f(xC[i],yC[i]);
        if(i<3) glVertex2f(xC[i+1], yC[i+1]);
        else glVertex2f(xC[0], yC[0]);
        glEnd(); 

      }

      glBegin(GL_LINES);
      glVertex2f(x,y);
      glVertex2f(x+cos(r)*w/2, y+sin(r)*w/2);
      glEnd();

    }

    // Colored Mode
    if(mode == 2){
      color = static_cast<Color*>(ent->getComponent(COMPONENT_COLOR));
      float r = color->getR();
      float g = color->getG();
      float b = color->getB();
      float a = color->getA();
      
      glDisable(GL_TEXTURE_2D);
      glColor4f(r,g,b,a);
      glBegin(GL_QUADS);
      
      for(int i = 0; i<4; i++){
      
        glVertex2i(xC[i],yC[i]);
      
      }
      
      glEnd();
    }

    // Textured Mode
    if(mode == 3){

      tex = static_cast<Texture*>(ent->getComponent(COMPONENT_TEXTURE));
      float *xMap = tex->tex->x;
      float *yMap = tex->tex->y;
      
      glEnable(GL_TEXTURE_2D);
      glColor3f(1.0, 1.0, 1.0);
      glBindTexture(GL_TEXTURE_2D, tex->tex->getTex());
      glBegin(GL_QUADS);

      for(int i = 0; i<4; i++){
      
        glTexCoord2f(xMap[i], yMap[i]); glVertex2i(xC[i], yC[i]);
      
      }

      glEnd();
      glDisable(GL_TEXTURE_2D);

    }

    if(ent->hasComponent(COMPONENT_TEXTMESSAGE)){

      TextMessage *txt = static_cast<TextMessage*>(ent->getComponent(COMPONENT_TEXTMESSAGE));
      std::string fnt = txt->getFont();
      std::string text = txt->getMessage();
      int padding = txt->getPadding();
      int size = txt->getSize();

      glEnable(GL_TEXTURE_2D);
      for(int i = 0; i < text.size(); i++){
        int ascii = text[i] - 32;
        RegTextureMapped* tmpTex = theEngine->registry.getTextureMapped(fnt + std::to_string(ascii));
        float *xMap = tmpTex->x;
        float *yMap = tmpTex->y;
        
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, tmpTex->getTex());
        glBegin(GL_QUADS);
        glTexCoord2f(xMap[0], yMap[0]); glVertex2i(x+padding+size*i*.5, y);
        glTexCoord2f(xMap[1], yMap[1]); glVertex2i(x+size*.5+padding+size*i*.5, y);
        glTexCoord2f(xMap[2], yMap[2]); glVertex2i(x+size*.5+padding+size*i*.5, y+size);
        glTexCoord2f(xMap[3], yMap[3]); glVertex2i(x+padding+size*i*.5, y+size);
        glEnd();
      }

      glDisable(GL_TEXTURE_2D);
    }

    /*
      If vectors showing for debug purposes
     */
		
    if(ent->hasComponent(COMPONENT_VELOCITY) && SHOW_VECTORS){
      Vector *vel = static_cast<Vector*>(ent->getComponent(COMPONENT_VELOCITY));
      float direction = vel->getDirection();
      
      glDisable(GL_TEXTURE_2D);
      glColor4f(0.0, 0.0, 1.0, 1.0);
      
      float centerx = x+w/2;
      float centery = y+h/2;
      float endx = centerx + vel->getXComponent();
      float endy = centery + vel->getYComponent();
      glBegin(GL_TRIANGLE_FAN);
      glVertex2f(centerx, centery);
      for(int theta = 0; theta < 360; theta += 5){
	       glVertex2f(centerx+sin(theta*3.14/180) * 5, centery + cos(theta*3.14/180) * 5);
      }
      glEnd();
      glLineWidth(2.0);
      
      glBegin(GL_LINES);
      glVertex2f(centerx, centery);
      glVertex2f(endx, endy);
      glEnd();
      
      float offset = 5*3.14/6;
      
      glBegin(GL_LINES);
      glVertex2f(endx, endy);
      glVertex2f(endx+10*cos(direction+offset), endy+10*sin(direction+offset));
      glEnd();
      
      glBegin(GL_LINES);
      glVertex2f(endx, endy);
      glVertex2f(endx+10*cos(direction-offset), endy+10*sin(direction-offset));
      glEnd();
      
    }
    
    glColor3f(1.0, 1.0, 1.0);
    
  }
}
	 
