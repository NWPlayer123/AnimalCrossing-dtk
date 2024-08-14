#include "ac_t_rei2.h"

#include "m_name_table.h"
#include "sys_matrix.h"
#include "m_lib.h"
#include "m_rcp.h"

static void aTRI2_actor_ct(ACTOR* actor, GAME* game);
static void aTRI2_actor_draw(ACTOR* actor, GAME* game);
static void aTRI2_actor_move(ACTOR* actor, GAME* game);
static void aTRI2_setupAction(ACTOR* actor, int action);

ACTOR_PROFILE T_Rei2_Profile = {
    mAc_PROFILE_T_REI2,
    ACTOR_PART_BG,
    ACTOR_STATE_NO_DRAW_WHILE_CULLED | ACTOR_STATE_NO_MOVE_WHILE_CULLED,
    EMPTY_NO,
    ACTOR_OBJ_BANK_TOOLS,
    sizeof(REI2_ACTOR),
    &aTRI2_actor_ct,
    NONE_ACTOR_PROC,
    &aTRI2_actor_move,
    &aTRI2_actor_draw,
    NULL
};

extern Gfx crw_rei2_body_model[];

static void aTRI2_actor_ct(ACTOR* actor, GAME* game){
    aTRI2_setupAction(actor, 4);
}

static void aTRI2_destruct(ACTOR* actor){
    Actor_delete(actor);
}

static void aTRI2_setupAction(ACTOR* actor, int action){
    REI2_ACTOR* rei2 = (REI2_ACTOR*)actor;
    static REI2_PROC process[] = {
        (REI2_PROC)none_proc1, (REI2_PROC)none_proc1, (REI2_PROC)none_proc1, aTRI2_destruct,(REI2_PROC)none_proc1, NULL
    };
    
    rei2->proc = process[action];
    rei2->current_id = action;
    rei2->tools_class.work0 = action;
}

static void aTRI2_actor_move(ACTOR* actor, GAME* game){
    REI2_ACTOR* rei2 = (REI2_ACTOR*)actor;
    
    if(rei2->tools_class.work0 != rei2->current_id){
        aTRI2_setupAction(actor, rei2->tools_class.work0);
    }

    rei2->proc(actor);
}

static void aTRI2_actor_draw(ACTOR* actor, GAME* game){
    REI2_ACTOR* rei2 = (REI2_ACTOR*)actor;

    GRAPH* graph;
    Gfx* gfxp;

    if(rei2->tools_class.init_matrix == 1){
        graph = game->graph;
        
        OPEN_DISP(graph);
        
        Matrix_put(&rei2->tools_class.matrix_work);
        Matrix_Position_Zero(&rei2->tools_class.actor_class.world.position);
        
        rei2->tools_class.init_matrix = 0;
    
        _texture_z_light_fog_prim_npc(graph); 

        gfxp = NOW_POLY_OPA_DISP;
        gSPMatrix(gfxp++, _Matrix_to_Mtx_new(graph),G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gfxp++, crw_rei2_body_model);
        SET_POLY_OPA_DISP(gfxp);
        
        CLOSE_DISP(graph);
    }
}
