#ifndef __TETRIS_H__
#define __TETRIS_H__

#include "defs.h"


extern const int GRID_HEIGHT;
extern const int GRID_WIDTH;

extern u8 block_grid[18][10];


struct Shape;
struct ShapeRotation;

struct Shape
{
    vec2_t pos;
    u32 color;
    struct ShapeRotation const * pRotation;
};

struct ShapeRotation
{
    const vec2_t block[4];
    const struct ShapeRotation const * ccw;
    const struct ShapeRotation const * cw;
};


void ShapeFactory( struct Shape* pShape, const vec2_t start_pos );
void AddShapeToGrid( struct Shape* pShape );
int ShapeCollision( struct Shape * pShape );
int RemoveLines( void );
void ClearGrid( void );


// I J L O S T Z
extern const struct ShapeRotation I_0;
extern const struct ShapeRotation I_90;
extern const struct ShapeRotation I_180;
extern const struct ShapeRotation I_270;

extern const struct ShapeRotation J_0;
extern const struct ShapeRotation J_90;
extern const struct ShapeRotation J_180;
extern const struct ShapeRotation J_270;

extern const struct ShapeRotation L_0;
extern const struct ShapeRotation L_90;
extern const struct ShapeRotation L_180;
extern const struct ShapeRotation L_270;

extern const struct ShapeRotation O_0;
extern const struct ShapeRotation O_90;
extern const struct ShapeRotation O_180;
extern const struct ShapeRotation O_270;

extern const struct ShapeRotation S_0;
extern const struct ShapeRotation S_90;
extern const struct ShapeRotation S_180;
extern const struct ShapeRotation S_270;

extern const struct ShapeRotation T_0;
extern const struct ShapeRotation T_90;
extern const struct ShapeRotation T_180;
extern const struct ShapeRotation T_270;

extern const struct ShapeRotation Z_0;
extern const struct ShapeRotation Z_90;
extern const struct ShapeRotation Z_180;
extern const struct ShapeRotation Z_270;


#endif // __TETRIS_H__
