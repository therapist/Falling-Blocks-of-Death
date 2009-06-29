#include "level_tetris.h"
#include "frameratecontroller.h"
#include "graphicsmanager.h"
#include "inputmanager.h"
#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include <string.h>
#include "vector.h"

static const int GRID_HEIGHT = 18;
static const int GRID_WIDTH = 10;

static u8 block_grid[18][10];

static vec2_t single_block;
static vec2_t starting_pos;

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

static const struct ShapeRotation I_0;
static const struct ShapeRotation I_90;
static const struct ShapeRotation I_180;
static const struct ShapeRotation I_270;
static const struct ShapeRotation I_0   = { {{0,0}, {0,1}, {0,2}, {0,3}}, &I_270,  &I_90 };
static const struct ShapeRotation I_90  = { {{0,0}, {1,0}, {2,0}, {3,0}},   &I_0, &I_180 };
static const struct ShapeRotation I_180 = { {{0,3}, {0,2}, {0,1}, {0,0}},  &I_90, &I_270 };
static const struct ShapeRotation I_270 = { {{3,0}, {2,0}, {1,0}, {0,0}}, &I_180,   &I_0 };

static const struct ShapeRotation J_0;
static const struct ShapeRotation J_90;
static const struct ShapeRotation J_180;
static const struct ShapeRotation J_270;
static const struct ShapeRotation J_0   = { {{1,2}, {1,1}, {1,0}, {0,0}}, &J_270,  &J_90 };
static const struct ShapeRotation J_90  = { {{2,0}, {1,0}, {0,0}, {0,1}},   &J_0, &J_180 };
static const struct ShapeRotation J_180 = { {{0,0}, {0,1}, {0,2}, {1,2}},  &J_90, &J_270 };
static const struct ShapeRotation J_270 = { {{0,1}, {1,1}, {2,1}, {2,0}}, &J_180,   &J_0 };

static const struct ShapeRotation L_0;
static const struct ShapeRotation L_90;
static const struct ShapeRotation L_180;
static const struct ShapeRotation L_270;
static const struct ShapeRotation L_0   = { {{1,0}, {1,1}, {1,2}, {0,2}}, &L_270,  &L_90 };
static const struct ShapeRotation L_90  = { {{0,0}, {1,0}, {2,0}, {2,1}},   &L_0, &L_180 };
static const struct ShapeRotation L_180 = { {{0,2}, {0,1}, {0,0}, {1,0}},  &L_90, &L_270 };
static const struct ShapeRotation L_270 = { {{0,0}, {0,1}, {1,1}, {2,1}}, &L_180,   &L_0 };

static struct Shape* pShape;

// I J L O S T Z

static void ShapeFactory( struct Shape* pShape, char type )
{
    switch( type )
    {
        case 'I':
            break;
        case 'J':
            break;
        case 'L':
            break;
        case 'O':
            break;
        case 'S':
            break;
        case 'T':
            break;
        case 'Z':
            break;
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

static int ShapeCollision( struct Shape * pShape )
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

static void Load( void )
{
    printf("Load::level_tetris\n");
    Vec2Set( starting_pos, (GRID_WIDTH/2 - 1), (GRID_HEIGHT - 1) );
    pShape = (struct Shape*) malloc( sizeof(struct Shape) );
}

static void Init( void )
{
    pShape->color = 0x0000ffff;
    Vec2Set( pShape->pos, 4, 4 );
    pShape->pRotation = &L_0;
    
    // clear block grid
    memset( block_grid, 0, sizeof(block_grid) );
    
    // set single_block at the starting position
    Vec2Copy( single_block, starting_pos );
    
    // add some starting blocks for debugging
    u8 line3[10] = {0,0,0,0,0,0,0,0,1,0};
    u8 line2[10] = {0,0,0,0,0,1,0,0,1,0};
    u8 line1[10] = {0,0,1,0,1,1,1,1,1,0};
    u8 line0[10] = {0,1,1,1,1,0,1,1,1,0};
    memcpy( block_grid[3], line3, sizeof(line3) );
    memcpy( block_grid[2], line2, sizeof(line2) );
    memcpy( block_grid[1], line1, sizeof(line1) );
    memcpy( block_grid[0], line0, sizeof(line0) );
    
    printf("Init::level_tetris\n");
}

static void Update( void )
{    
    int i;
    vec2_t new_pos;
    vec2_t old_pos;
    
    printf("Update::level_tetris\n");
    printf("\tframe_time = %.04f sec\n\tlevel_time = %.04f sec\n", frame_time, level_time);
    
    // Left
    if( KeyTriggered( KEY_a ) )
    {
        Vec2Copy( new_pos, single_block );
        new_pos[0] -= 1;
        if( !collision( new_pos ) )
            Vec2Copy( single_block, new_pos );
        
        Vec2Copy( old_pos, pShape->pos );
        pShape->pos[0] -= 1;
        if( ShapeCollision( pShape ) )
            Vec2Copy( pShape->pos, old_pos );
    }
    
    // Right
    if( KeyTriggered( KEY_d ) )
    {
        Vec2Copy( new_pos, single_block );
        new_pos[0] += 1;
        if( !collision( new_pos ) )
            Vec2Copy( single_block, new_pos );
        
        Vec2Copy( old_pos, pShape->pos );
        pShape->pos[0] += 1;
        if( ShapeCollision( pShape ) )
            Vec2Copy( pShape->pos, old_pos );
    }
    
    // Down
    if( KeyTriggered( KEY_s ) )
    {
        Vec2Copy( new_pos, single_block );
        new_pos[1] -= 1;
        if( !collision( new_pos ) )
            Vec2Copy( single_block, new_pos );
        
        Vec2Copy( old_pos, pShape->pos );
        pShape->pos[1] -= 1;
        if( ShapeCollision( pShape ) )
            Vec2Copy( pShape->pos, old_pos );
    }
    
    // Up; debugging
    if( KeyTriggered( KEY_w ) )
    {
        Vec2Copy( new_pos, single_block );
        new_pos[1] += 1;
        if( !collision( new_pos ) )
            Vec2Copy( single_block, new_pos );
        
        Vec2Copy( old_pos, pShape->pos );
        pShape->pos[1] += 1;
        if( ShapeCollision( pShape ) )
            Vec2Copy( pShape->pos, old_pos );
    }
    
    // Rotate
    if( KeyTriggered( KEY_q ) )
    {
        struct ShapeRotation const * old_rot = pShape->pRotation;
        pShape->pRotation = pShape->pRotation->ccw;
        
        if( ShapeCollision( pShape ) )
            pShape->pRotation = old_rot;
    }
    
    if( KeyTriggered( KEY_e ) )
    {
        struct ShapeRotation const * old_rot = pShape->pRotation;
        pShape->pRotation = pShape->pRotation->cw;
        
        if( ShapeCollision( pShape ) )
            pShape->pRotation = old_rot;
    }
    
    if( KeyTriggered( KEY_r ) )
        GS_next = GS_RESTART;
    
    // Quit if escape is pressed
    if( KeyTriggered( KEY_ESCAPE ) )
        GS_next = GS_QUIT;
}

static void Draw( void )
{
    int i;
    int x, y;
    vec2_t pos;
    
    printf("Draw::level_tetris\n\n");
    
    // draw pShape
    Graphics_SetColor( pShape->color );
    for( i=0; i<4; ++i )
    {
        Vec2Copy( pos, pShape->pRotation->block[i] );
        Vec2Add( pos, pos, pShape->pos );
        Graphics_DrawBlock( pos );
    }
    
    // draw single block
    Graphics_SetColor( 0xff0000ff );
    Graphics_DrawBlock( single_block );
    
    // draw set blocks
    Graphics_SetColor( 0xff00ffff );
    for( y=0; y<GRID_HEIGHT; ++y )
        for( x=0; x<GRID_WIDTH; ++x )
            if( block_grid[y][x] )
            {
                Vec2Set( pos, x, y );
                Graphics_DrawBlock( pos );
            }
}

static void Free( void )
{
    printf("Free::level_tetris\n");
}

static void Unload( void )
{
    printf("Unload::level_tetris\n");
    free(pShape);
}

// global accessor
const struct GameState GS_level_tetris = {Load, Init, Update, Draw, Free, Unload};

