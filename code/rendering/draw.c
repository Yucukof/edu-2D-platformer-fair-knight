//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "draw.h"

#define CALLER "draw"

void drawPlayer(Player* player, colour overlay) {
  // Set QUADS coordinates based on player position and sprite size
  float x = (float) player->coord->x - scrollx
      , y = (float) player->coord->y - scrolly
      , w = (float) player->sprites->width
      , h = (float) player->sprites->height;
  drawSprite(player->sprites,x,y+BOTTOMBANNER,w,h,overlay,player->coord->look);
}

void drawBullet(Bullet* bullet) {
  // Set QUADS coordinates based on monster position and sprite size
  float x = (float) bullet->coord->x - scrollx
      , y = (float) bullet->coord->y - scrolly
      , w = (float) bullet->sprites->width
      , h = (float) bullet->sprites->height;
  drawSprite(bullet->sprites,x,y+BOTTOMBANNER,w,h,WHITE,bullet->coord->look);
}

void drawMonster(Monster* mob, colour overlay) {
  // Set QUADS coordinates based on monster position and sprite size
  float x = (float) mob->coord->x - scrollx
      , y = (float) mob->coord->y - scrolly
      , w = (float) mob->sprites->width
      , h = (float) mob->sprites->height;
  // Draw sprite
  drawSprite(mob->sprites,x,y+BOTTOMBANNER,w,h,overlay,mob->coord->look);
}

void drawItem(Item* obj) {
  // Set QUADS coordinates based on monster position and sprite size
  float x = (float) obj->coord->x - scrollx
      , y = (float) obj->coord->y - scrolly
      , w = (float) obj->sprites->width
      , h = (float) obj->sprites->height;
  drawSprite(obj->sprites,x,y+BOTTOMBANNER,w,h,WHITE,RIGHT);
}

void drawTile(Sprite* s, float x, float y) {
        x = (float)x*UNIT -scrollx;
        y = (float)y*UNIT+BOTTOMBANNER-scrolly;
  float w = (float)s->width
      , h = (float)s->height;
  drawSprite(s,x,y,w,h,WHITE,RIGHT);
}

void drawBackground(Sprite* s,colour overlay) {
  // Make shortcurt
  Level* l = game.context->level;
  // Get level value
  float x = - scrollx
      , y = - scrolly + BOTTOMBANNER
      , w = (float)l->width
      , h = (float)l->height;
  // Get sprites for background
  if (!isValidTexture(s)) {
    loadTexture(s);
  }
  // Check if background was found
  if (isValidTexture(s)) {
    float ratio = 1;
    // Get ratio to fil screen
    if (l->height > l->width) {
      ratio = (float)(s->width/l->width);
    } else
    if (l->height < l->width) {
      ratio = (float)(s->height/l->height);
    }
    // Convert to screen size
    w = (float)(s->width  / ratio * UNIT);
    h = (float)(s->height / ratio * UNIT);
    // Draw background
    drawSprite(s,x,y,w,h,WHITE,RIGHT);
  } else {
    logger(TRACE,CALLER,"unable to draw background!");
  }
}

void drawCenteredSprite(Sprite* s, float w, float h, colour overlay, direction orientation) {
  // Determine x and y position to start drawing, compared to screen specs
  float x = (DISPLAYWIDTH -w)/2;
  float y = (DISPLAYHEIGHT-h)/2;
  // Effectively draw sprite
  drawSprite(s,x,y,w,h,overlay,orientation);
}

// UTILITY

void drawSimpleText(char* text,int x, int y,Font* fonts) {
  // Call drawText with default parameters: 20px size, default font, colour black
  drawText(text
          ,x,y,20,0,BLACK
          ,fonts);
}

void drawCenteredText(char* text, int y, uint size, uint index, colour overlay,Font* fonts) {
  // Enable GL bidimensional texturing
  glEnable(GL_TEXTURE_2D);

  // Get future text length;
  Font* font = findFont(fonts,index);
  // Set start point
  float penx    = 1;

  // Check if matching font was found
  if (font != NULL) {

    // Get freetype font face object
    FT_Face face = font->face;
    //
    FT_UInt glyph_index;

    // Set size of text based on pixels (parameter = font,width,height // width being ignored)
    FT_Set_Pixel_Sizes(face, 0, size);

    // Read throught text
    for (int i = 0; text[i] != '\0'; i++) {
      // For each char, get glyphe reference
      glyph_index = FT_Get_Char_Index( face, text[i]);
      // Load glyph element as texturable object
      if (FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER)) {
        logger(WARN,CALLER,"Failed to load glyph.");
        // If successfully loaded, convert glyph to bitmap
      } else if (FT_Render_Glyph( face->glyph,FT_RENDER_MODE_NORMAL)) {
        logger(WARN,CALLER,"failed to render glyph");
      } else {
        penx += (face->glyph->advance.x / 64.0f);
      }
    }
    glDisable(GL_TEXTURE_2D);
    int x = (DISPLAYWIDTH/2)-((int)penx/2);

    // Effectively draw centered text
    drawText(text,x,y,size,index,overlay,fonts);
  }
}

void drawText(char* text,int x,int y,uint size,int index,colour overlay,Font* fonts) {
  // Enable GL bidimensional texturing
  glEnable(GL_TEXTURE_2D);

  // Find fonts in resources
  Font* font = findFont(fonts,index);

  // Check if font was effectively found
  if (font != NULL) {
    // Load FreeType face object
    FT_Face face = font->face;
    // Declare char position variables
    float penx = x
        , peny = y
        , penw = 1
        , penh = 1;

    // Declare FT variables
    FT_UInt glyph_index;
    GLuint charText;

    // Set size of text based on pixels (parameter = font,width,height // width being ignored)
    FT_Set_Pixel_Sizes(face, 0, size);

    // Render eahc char in string
    for (int i = 0; text[i] != '\0' && i < MAXSTRING; i++) {
      // Find glyph object reference
      glyph_index = FT_Get_Char_Index( face, text[i]);

      // Attemp to load glyph as texturable element
      if (FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER)) {
          logger(WARN,CALLER,"Failed to load glyph.");
          // if found, attempt to convert it to bitmap
      } else if (FT_Render_Glyph( face->glyph,FT_RENDER_MODE_NORMAL)) {
          logger(WARN,CALLER,"failed to render glyph");
      } else {
        // Update pen position variables with glyph specifics
        peny = y;
        penw = face->glyph->metrics.width / 64.0f;
        penh = face->glyph->metrics.horiBearingY / 64.0f;

        // Set font colour
        setOverlay(overlay);

        // Initilialise GL texture object
        glGenTextures(1, &charText);
        glBindTexture(GL_TEXTURE_2D, charText);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

        // Push glyph bitmap to texture
        glTexImage2D( GL_TEXTURE_2D,
                      0,
                      GL_ALPHA,
                      face->glyph->bitmap.width,
                      face->glyph->bitmap.rows,
                      0,
                      GL_ALPHA,
                      GL_UNSIGNED_BYTE,
                      face->glyph->bitmap.buffer);

        // Get pen y value (offset of char on line)
        // peny = y+face->glyph->metrics.vertBearingY/FT_ADVANCE;

        // Draw quad relevant to glyph to apply texture to it
        glBegin(GL_QUADS);
          glTexCoord2f(0.0, 1.0);     glVertex2f(penx       , peny       );
          glTexCoord2f(0.0, 0.0);     glVertex2f(penx       , peny + penh);
          glTexCoord2f(1.0, 0.0);     glVertex2f(penx + penw, peny + penh);
          glTexCoord2f(1.0, 1.0);     glVertex2f(penx + penw, peny       );
        glEnd();

        // advance pen position
        penx += (face->glyph->advance.x / 64.0f);
      }
    }
  }
  // Disable Open GL texturing mode
  glDisable(GL_TEXTURE_2D);
}

void drawSprite(Sprite* sprite, float x, float y, float w, float h, colour overlay, direction orientation) {

  // Enable Open GL texturing mode
  glEnable(GL_TEXTURE_2D);

  // Check if Sprite texture is loaded
  if (!isValidTexture(sprite)) {
  loadTexture(sprite);
}

  // Check if Sprite is effectively loaded
  if (isValidTexture(sprite)) {
    // Apply texture and set its colour
    glBindTexture(GL_TEXTURE_2D,sprite->texture);
    setOverlay(overlay);
  } else {
    logger(INFO,CALLER,"cannot draw unloaded sprite!");
    // Highlight missing texture with red fill and disable openGl texture mode
    glDisable(GL_TEXTURE_2D);
    setOverlay(RED);
  }

  // draw quand
  if (orientation == LEFT) {
    // Draw reversed quad
    glBegin(GL_QUADS);
      glTexCoord2f(1.0, 1.0);     glVertex2f(x    , y    );
      glTexCoord2f(1.0, 0.0);     glVertex2f(x    , y + h);
      glTexCoord2f(0.0, 0.0);     glVertex2f(x + w, y + h);
      glTexCoord2f(0.0, 1.0);     glVertex2f(x + w, y    );
    glEnd();
  } else
  if (orientation == INVERT) {
    // Draw inverted quad
    glBegin(GL_QUADS);
      glTexCoord2f(0.0, 0.0);     glVertex2f(x    , y    );
      glTexCoord2f(0.0, 1.0);     glVertex2f(x    , y + h);
      glTexCoord2f(1.0, 1.0);     glVertex2f(x + w, y + h);
      glTexCoord2f(1.0, 0.0);     glVertex2f(x + w, y    );
    glEnd();
  } else {
    // Draw normal quad
    glBegin(GL_QUADS);
      glTexCoord2f(0.0, 1.0);     glVertex2f(x    , y    );
      glTexCoord2f(0.0, 0.0);     glVertex2f(x    , y + h);
      glTexCoord2f(1.0, 0.0);     glVertex2f(x + w, y + h);
      glTexCoord2f(1.0, 1.0);     glVertex2f(x + w, y    );
    glEnd();
  }
  // Disable GL texturing mode if not done already
  glDisable(GL_TEXTURE_2D);
}

void drawRectangle(float x, float y, float w, float h,colour overlay) {
  // Disable GL texturing mode if needed
  glDisable(GL_TEXTURE_2D);
  // Set rectangle colour
  setOverlay(overlay);
  // Draw quad
  glBegin(GL_QUADS);
    glVertex2f(x,y);
    glVertex2f(x,h);
    glVertex2f(w,h);
    glVertex2f(w,y);
  glEnd();
}

void setOverlay(colour overlay) {
  // Set open GL current colour
  switch (overlay) {

    case WHITE:
      glColor3f( 1.f, 1.f, 1.f );
      break;

    case RED:
      glColor3f( 1.f, 0.f, 0.f );
      break;

    case BLUE:
      glColor3f( 0.f, 0.f, 1.f );
      break;

    case GREEN:
      glColor3f( 0.f, 1.f, 0.f );
      break;

    case ORANGE:
      glColor3f( 1.f, 1.f, 1.f );
      break;

    case GREY:
      glColor3f( .1f, .1f, .1f );
      break;

    case YELLOW:
      glColor3f( 1.f, 1.f, 1.f );
      break;

    case BLACK:
      glColor3f( .0f, .0f, .0f );
      break;

    case BLUR:
      glColor4f( .1f, .1f, 0.1f,.5f );
      break;

    case UNCHANGED:
      // do nothing
      break;

    case MAXCOLOUR:
      logger(ERR,CALLER,"invalid overlay requested!");
      break;
  }
}
