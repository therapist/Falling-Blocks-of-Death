#include "tetris.h"
#include "vector.h"
#include <string.h>

const int GRID_HEIGHT = 18;
const int GRID_WIDTH = 10;

u8 block_grid[18][10];

void ShapeFactory( struct Shape* pShape, const vec2_t start_pos )
{
    switch( rand() % 7 )
    {
        case 0:
            pShape->pRotation = &I_0;
            break;
        case 1:
            pShape->pRotation = &J_0;
            break;
        case 2:
            pShape->pRotation = &L_0;
            break;
        case 3:
            pShape->pRotation = &O_0;
            break;
        case 4:
            pShape->pRotation = &S_0;
            break;
        case 5:
            pShape->pRotation = &T_0;
            break;
        case 6:
            pShape->pRotation = &Z_0;
            break;
    }
    Vec2Copy( pShape->pos, start_pos );
}

void AddShapeToGrid( struct Shape* pShape )
{
    int i;
    vec2_t pos;
    
    // don't add if shape is out of bounds
    if( ShapeCollision( pShape ) )
        return;
    
    for( i=0; i<4; ++i )
    {
        Vec2Add( pos, pShape->pRotation->block[i], pShape->pos );
        block_grid[ (int)pos[1] ][ (int)pos[0] ] = 1;
    }
}

static int collision( vec2_t pos )
{
    int x;
    int y;
    x = pos[0];
    y = pos[1];
    
    if( x < 0 
    || x >= GRID_WIDTH
    || y < 0
    || y >= GRID_HEIGHT
    || ( block_grid[y][x] ) )
        return 1;
        
    return 0;
}

int ShapeCollision( struct Shape * pShape )
{
    int i;
    vec2_t block_pos;
    
    for( i=0; i<4; ++i )
    {
        Vec2Add( block_pos, pShape->pRotation->block[i], pShape->pos );
        if( collision( block_pos ) )
            return 1;
    }
    
    return 0;
}

int RemoveLines( void )
{
    static u8 complete_line[10] = { 1,1,1,1,1,1,1,1,1,1 };
    int i, j;
    int num_lines = 0;
    
    for( i=0; i<GRID_HEIGHT; ++i )
    {
        if( memcmp( block_grid[i], complete_line, sizeof( block_grid[0] ) ) == 0 )
        {
            for( j=i; j<(GRID_HEIGHT - 1); ++j )
            {
                memcpy( block_grid[j], block_grid[j+1], sizeof( block_grid[0] ) );
            }
            ++num_lines;
            --i;    // start checking at the same line
        }
    }
    return num_lines;
}

void ClearGrid( void )
{
    // clear block grid
    memset( block_grid, 0, sizeof( block_grid ) );
    
    // add some starting blocks for debugging
    u8 line3[10] = {0,0,0,0,0,0,0,0,1,0};
    u8 line2[10] = {0,0,0,0,0,1,0,0,1,0};
    u8 line1[10] = {0,0,1,0,1,1,1,1,1,0};
    u8 line0[10] = {0,1,1,1,1,0,1,1,1,0};
    memcpy( block_grid[3], line3, sizeof( line3 ) );
    memcpy( block_grid[2], line2, sizeof( line2 ) );
    memcpy( block_grid[1], line1, sizeof( line1 ) );
    memcpy( block_grid[0], line0, sizeof( line0 ) );
}


// I J L O S T Z
const struct ShapeRotation I_0;
const struct ShapeRotation I_90;
const struct ShapeRotation I_180;
const struct ShapeRotation I_270;
const struct ShapeRotation I_0   = { {{0,0}, {0,1}, {0,2}, {0,3}}, &I_270,  &I_90 };
const struct ShapeRotation I_90  = { {{0,0}, {1,0}, {2,0}, {3,0}},   &I_0, &I_180 };
const struct ShapeRotation I_180 = { {{0,3}, {0,2}, {0,1}, {0,0}},  &I_90, &I_270 };
const struct ShapeRotation I_270 = { {{3,0}, {2,0}, {1,0}, {0,0}}, &I_180,   &I_0 };

const struct ShapeRotation J_0;
const struct ShapeRotation J_90;
const struct ShapeRotation J_180;
const struct ShapeRotation J_270;
const struct ShapeRotation J_0   = { {{1,2}, {1,1}, {1,0}, {0,0}}, &J_270,  &J_90 };
const struct ShapeRotation J_90  = { {{2,0}, {1,0}, {0,0}, {0,1}},   &J_0, &J_180 };
const struct ShapeRotation J_180 = { {{0,0}, {0,1}, {0,2}, {1,2}},  &J_90, &J_270 };
const struct ShapeRotation J_270 = { {{0,1}, {1,1}, {2,1}, {2,0}}, &J_180,   &J_0 };

const struct ShapeRotation L_0;
const struct ShapeRotation L_90;
const struct ShapeRotation L_180;
const struct ShapeRotation L_270;
const struct ShapeRotation L_0   = { {{1,0}, {1,1}, {1,2}, {0,2}}, &L_270,  &L_90 };
const struct ShapeRotation L_90  = { {{0,0}, {1,0}, {2,0}, {2,1}},   &L_0, &L_180 };
const struct ShapeRotation L_180 = { {{0,2}, {0,1}, {0,0}, {1,0}},  &L_90, &L_270 };
const struct ShapeRotation L_270 = { {{0,0}, {0,1}, {1,1}, {2,1}}, &L_180,   &L_0 };

const struct ShapeRotation O_0;
const struct ShapeRotation O_90;
const struct ShapeRotation O_180;
const struct ShapeRotation O_270;
const struct ShapeRotation O_0   = { {{0,0}, {0,1}, {1,1}, {1,0}}, &O_270,  &O_90 };
const struct ShapeRotation O_90  = { {{0,1}, {1,1}, {1,0}, {0,0}},   &O_0, &O_180 };
const struct ShapeRotation O_180 = { {{1,1}, {1,0}, {0,0}, {1,0}},  &O_90, &O_270 };
const struct ShapeRotation O_270 = { {{0,1}, {0,0}, {1,0}, {1,1}}, &O_180,   &O_0 };

const struct ShapeRotation S_0;
const struct ShapeRotation S_90;
const struct ShapeRotation S_180;
const struct ShapeRotation S_270;
const struct ShapeRotation S_0   = { {{1,0}, {1,1}, {0,1}, {0,2}}, &S_270,  &S_90 };
const struct ShapeRotation S_90  = { {{0,0}, {1,0}, {1,1}, {2,1}},   &S_0, &S_180 };
const struct ShapeRotation S_180 = { {{0,2}, {0,1}, {1,1}, {1,0}},  &S_90, &S_270 };
const struct ShapeRotation S_270 = { {{2,1}, {1,1}, {1,0}, {0,0}}, &S_180,   &S_0 };

const struct ShapeRotation T_0;
const struct ShapeRotation T_90;
const struct ShapeRotation T_180;
const struct ShapeRotation T_270;
const struct ShapeRotation T_0   = { {{0,1}, {1,0}, {1,1}, {1,2}}, &T_270,  &T_90 };
const struct ShapeRotation T_90  = { {{1,1}, {0,0}, {1,0}, {2,0}},   &T_0, &T_180 };
const struct ShapeRotation T_180 = { {{1,1}, {0,2}, {0,1}, {0,0}},  &T_90, &T_270 };
const struct ShapeRotation T_270 = { {{1,0}, {2,1}, {1,1}, {0,1}}, &T_180,   &T_0 };

const struct ShapeRotation Z_0;
const struct ShapeRotation Z_90;
const struct ShapeRotation Z_180;
const struct ShapeRotation Z_270;
const struct ShapeRotation Z_0   = { {{0,0}, {0,1}, {1,1}, {1,2}}, &Z_270,  &Z_90 };
const struct ShapeRotation Z_90  = { {{0,1}, {1,1}, {1,0}, {2,0}},   &Z_0, &Z_180 };
const struct ShapeRotation Z_180 = { {{1,2}, {1,1}, {0,1}, {0,0}},  &Z_90, &Z_270 };
const struct ShapeRotation Z_270 = { {{2,0}, {1,0}, {1,1}, {0,1}}, &Z_180,   &Z_0 };

