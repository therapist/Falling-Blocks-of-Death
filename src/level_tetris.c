#include "level_tetris.h"
#include "frameratecontroller.h"
#include "graphicsmanager.h"
#include "inputmanager.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "defs.h"
#include <string.h>
#include "vector.h"

// Shape struct declarations
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


// globals
static struct Shape* pShape;

static const int GRID_HEIGHT = 18;
static const int GRID_WIDTH = 10;

static u8 block_grid[18][10];

static vec2_t starting_pos;

static int score;

// I J L O S T Z
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

static const struct ShapeRotation O_0;
static const struct ShapeRotation O_90;
static const struct ShapeRotation O_180;
static const struct ShapeRotation O_270;
static const struct ShapeRotation O_0   = { {{0,0}, {0,1}, {1,1}, {1,0}}, &O_270,  &O_90 };
static const struct ShapeRotation O_90  = { {{0,1}, {1,1}, {1,0}, {0,0}},   &O_0, &O_180 };
static const struct ShapeRotation O_180 = { {{1,1}, {1,0}, {0,0}, {1,0}},  &O_90, &O_270 };
static const struct ShapeRotation O_270 = { {{0,1}, {0,0}, {1,0}, {1,1}}, &O_180,   &O_0 };

static const struct ShapeRotation S_0;
static const struct ShapeRotation S_90;
static const struct ShapeRotation S_180;
static const struct ShapeRotation S_270;
static const struct ShapeRotation S_0   = { {{1,0}, {1,1}, {0,1}, {0,2}}, &S_270,  &S_90 };
static const struct ShapeRotation S_90  = { {{0,0}, {1,0}, {1,1}, {2,1}},   &S_0, &S_180 };
static const struct ShapeRotation S_180 = { {{0,2}, {0,1}, {1,1}, {1,0}},  &S_90, &S_270 };
static const struct ShapeRotation S_270 = { {{2,1}, {1,1}, {1,0}, {0,0}}, &S_180,   &S_0 };

static const struct ShapeRotation T_0;
static const struct ShapeRotation T_90;
static const struct ShapeRotation T_180;
static const struct ShapeRotation T_270;
static const struct ShapeRotation T_0   = { {{0,1}, {1,0}, {1,1}, {1,2}}, &T_270,  &T_90 };
static const struct ShapeRotation T_90  = { {{1,1}, {0,0}, {1,0}, {2,0}},   &T_0, &T_180 };
static const struct ShapeRotation T_180 = { {{1,1}, {0,2}, {0,1}, {0,0}},  &T_90, &T_270 };
static const struct ShapeRotation T_270 = { {{1,0}, {2,1}, {1,1}, {0,1}}, &T_180,   &T_0 };

static const struct ShapeRotation Z_0;
static const struct ShapeRotation Z_90;
static const struct ShapeRotation Z_180;
static const struct ShapeRotation Z_270;
static const struct ShapeRotation Z_0   = { {{0,0}, {0,1}, {1,1}, {1,2}}, &Z_270,  &Z_90 };
static const struct ShapeRotation Z_90  = { {{0,1}, {1,1}, {1,0}, {2,0}},   &Z_0, &Z_180 };
static const struct ShapeRotation Z_180 = { {{1,2}, {1,1}, {0,1}, {0,0}},  &Z_90, &Z_270 };
static const struct ShapeRotation Z_270 = { {{2,0}, {1,0}, {1,1}, {0,1}}, &Z_180,   &Z_0 };


// Shape function declarations
static void ShapeFactory( struct Shape* pShape );
static void AddShapeToGrid( struct Shape* pShape );
static int ShapeCollision( struct Shape * pShape );


static void ShapeFactory( struct Shape* pShape )
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
    Vec2Copy( pShape->pos, starting_pos );
}

static void AddShapeToGrid( struct Shape* pShape )
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

static int RemoveLines( void )
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

static void Load( void )
{
    printf("Load::level_tetris\n");
    Vec2Set( starting_pos, (GRID_WIDTH/2 - 1), (GRID_HEIGHT - 5) );
    pShape = (struct Shape*) malloc( sizeof( struct Shape ) );
}

static void Init( void )
{
    pShape->color = 0x0000ffff;
    Vec2Set( pShape->pos, 4, 4 );
    pShape->pRotation = &Z_0;
    
    srand ( time(NULL) );
    
    // reset score
    score = 0;
    
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
    
    printf("Init::level_tetris\n");
}

static void Update( void )
{    
    int i;
    vec2_t new_pos;
    vec2_t old_pos;
    
    printf("Update::level_tetris\n");
    printf("\tframe_time = %.04f sec\n" \
           "\tlevel_time = %.04f sec\n" \
           "\t     score = %i lines\n",
           frame_time, level_time, score);
    
    // Left
    if( KeyTriggered( KEY_a ) )
    {
        Vec2Copy( old_pos, pShape->pos );
        pShape->pos[0] -= 1;
        if( ShapeCollision( pShape ) )
            Vec2Copy( pShape->pos, old_pos );
    }
    
    // Right
    if( KeyTriggered( KEY_d ) )
    {
        Vec2Copy( old_pos, pShape->pos );
        pShape->pos[0] += 1;
        if( ShapeCollision( pShape ) )
            Vec2Copy( pShape->pos, old_pos );
    }
    
    // Down
    if( KeyTriggered( KEY_s ) )
    {
        Vec2Copy( old_pos, pShape->pos );
        pShape->pos[1] -= 1;
        if( ShapeCollision( pShape ) )
        {
            Vec2Copy( pShape->pos, old_pos );
            AddShapeToGrid( pShape );
            score += RemoveLines();
            ShapeFactory( pShape );
        }
    }
    
    // Up; debugging
    if( KeyTriggered( KEY_w ) )
    {
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
const struct GameState GS_level_tetris = { Load, Init, Update, Draw, Free, Unload };

