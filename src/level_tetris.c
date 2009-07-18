#include "level_tetris.h"
#include "tetris.h"
#include "frameratecontroller.h"
#include "graphicsmanager.h"
#include "inputmanager.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "defs.h"
#include <string.h>
#include "vector.h"


// globals
static struct Shape* pShape;

static vec2_t starting_pos;

static int score;

static float drop_timer;
static float drop_speed;


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
    
    drop_timer = 0.;
    drop_speed = 0.5;
    
    // reset score
    score = 0;
    
    ClearGrid();
    
    printf("Init::level_tetris\n");
}

static void Update( void )
{
    int i;
    vec2_t new_pos;
    vec2_t old_pos;
    int drop = 0;
    
    drop_timer += frame_time;
    while( drop_timer > drop_speed )
    {
        drop += 1;
        drop_timer -= drop_speed;
    }
    
    printf("Update::level_tetris\n");
    printf("\tframe_time = %.04f sec\n" \
           "\tlevel_time = %.04f sec\n" \
           "\t     score = %i lines\n",
           frame_time, level_time, score);
    
    // autodrop
    while( drop-- )
    {
        Vec2Copy( old_pos, pShape->pos );
        pShape->pos[1] -= 1;
        if( ShapeCollision( pShape ) )
        {
            Vec2Copy( pShape->pos, old_pos );
            AddShapeToGrid( pShape );
            score += RemoveLines();
            ShapeFactory( pShape, starting_pos );
        }
    }
    
    
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
            ShapeFactory( pShape, starting_pos );
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

