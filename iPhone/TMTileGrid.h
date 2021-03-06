//
//  TMTileGrid.h
//  iMurphy
//
//  Represents a grid of tiles that form a level, world, etc.
//
//  Is capable of drawing the appropriate portion of that grid into the current murphy viewport.
//  (currently with a direct API, but I think it will implement a common drawing protocol soon.)
//
//  Created by Dave Peck on 4/16/09.
//  Copyright 2009 Code Orange. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TMTileMap.h"

@interface TMTileGrid : NSObject {
	TMTileMap *map;		
	uint16_t width;
	uint16_t height;	
	uint16_t *grid;
	
	GLfloat gridLeft;	  // where in GL space does our grid start?
	GLfloat gridTop;
	GLfloat gridRight;
	GLfloat gridBottom;
	GLfloat pixelWidth;   // these probably don't belong here, but for now...
	GLfloat pixelHeight;
			
	GLfloat *tileCoordinates;
	GLfloat *tileTextureCoordinates;
	GLushort *tileIndexes;
	GLushort numCoordinates;
	GLushort numIndexes;	
}

+(id)tileGridWithMap:(TMTileMap *)map width:(uint16_t)width height:(uint16_t)height;

@property (readonly) TMTileMap *map;
@property (readonly) uint16_t width;
@property (readonly) uint16_t height;
@property (readonly) GLfloat gridLeft;
@property (readonly) GLfloat gridTop;
@property (readonly) GLfloat gridRight;
@property (readonly) GLfloat gridBottom;

-(uint16_t)getTileIdAtX:(uint16_t)x y:(uint16_t)y;
-(void)setTileIdAtX:(uint16_t)x y:(uint16_t)y tileId:(uint16_t)tileId;


// Do these belong here?
-(void)setGridLeft:(GLfloat)myLeft top:(GLfloat)myTop;
-(void)setPixelWidth:(GLfloat)pixelWidth height:(GLfloat)pixelHeight;
-(void)alignViewportToPixel:(GLfloat *)left top:(GLfloat *)top;
-(void)drawInViewportLeft:(GLfloat)left top:(GLfloat)top right:(GLfloat)right bottom:(GLfloat)bottom;

@end
