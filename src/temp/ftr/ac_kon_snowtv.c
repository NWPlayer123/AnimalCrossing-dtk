static void aKonsnowtv_ct(FTR_ACTOR* ftr_actor, u8* data);
static void aKonsnowtv_mv(FTR_ACTOR* ftr_actor, ACTOR* my_room_actor, GAME* game, u8* data);
static void aKonsnowtv_dw(FTR_ACTOR* ftr_actor, ACTOR* my_room_actor, GAME* game, u8* data);
static void aKonsnowtv_dt(FTR_ACTOR* ftr_actor, u8* data);

extern u8 int_kon_snowtv07a_TA_tex_txt[];
extern u8 int_kon_snowtv07b_TA_tex_txt[];
extern u8 int_kon_snowtv07c_TA_tex_txt[];
extern u8 int_kon_snowtv07d_TA_tex_txt[];

extern Gfx int_kon_snowtv_body_model[];
extern Gfx int_kon_snowtv_face_model[];

u8* aKonsnowtv_on_anime[] = {
    int_kon_snowtv07b_TA_tex_txt, int_kon_snowtv07b_TA_tex_txt, int_kon_snowtv07b_TA_tex_txt,
    int_kon_snowtv07b_TA_tex_txt, int_kon_snowtv07b_TA_tex_txt, int_kon_snowtv07b_TA_tex_txt,
    int_kon_snowtv07b_TA_tex_txt, int_kon_snowtv07b_TA_tex_txt, int_kon_snowtv07b_TA_tex_txt,
    int_kon_snowtv07b_TA_tex_txt, int_kon_snowtv07c_TA_tex_txt, int_kon_snowtv07c_TA_tex_txt,
    int_kon_snowtv07c_TA_tex_txt, int_kon_snowtv07c_TA_tex_txt, int_kon_snowtv07c_TA_tex_txt,
    int_kon_snowtv07c_TA_tex_txt, int_kon_snowtv07c_TA_tex_txt, int_kon_snowtv07c_TA_tex_txt,
    int_kon_snowtv07c_TA_tex_txt, int_kon_snowtv07c_TA_tex_txt, int_kon_snowtv07d_TA_tex_txt,
    int_kon_snowtv07d_TA_tex_txt, int_kon_snowtv07d_TA_tex_txt, int_kon_snowtv07d_TA_tex_txt,
    int_kon_snowtv07d_TA_tex_txt, int_kon_snowtv07d_TA_tex_txt, int_kon_snowtv07d_TA_tex_txt,
    int_kon_snowtv07d_TA_tex_txt, int_kon_snowtv07d_TA_tex_txt, int_kon_snowtv07d_TA_tex_txt,
};

static void aKonsnowtv_ct(FTR_ACTOR* ftr_actor, u8* data){
    
}
static void aKonsnowtv_mv(FTR_ACTOR* ftr_actor, ACTOR* my_room_actor, GAME* game, u8* data){

    if(ftr_actor->switch_bit != FALSE){
        if(aFTR_CAN_PLAY_SE(ftr_actor)){
            sAdo_OngenPos((u32) ftr_actor, 0x2B, &ftr_actor->position);
        }
        ftr_actor->tex_animation.frame++;

        if(ftr_actor->tex_animation.frame >= 60 || ftr_actor->tex_animation.frame < 0){
            ftr_actor->tex_animation.frame = 0;
        }
    
    }

    if(ftr_actor->switch_changed_flag != FALSE){
        if(ftr_actor->switch_bit != FALSE){
            sAdo_OngenTrgStart(0x16, &ftr_actor->position);
        }
        else{
            sAdo_OngenTrgStart(0x17, &ftr_actor->position);
        }
    }
    
}
static void aKonsnowtv_dw(FTR_ACTOR* ftr_actor, ACTOR* my_room_actor, GAME* game, u8* data) {

    OPEN_DISP(game->graph);

    gSPMatrix(NEXT_POLY_OPA_DISP,_Matrix_to_Mtx_new(game->graph), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    if(ftr_actor->switch_bit != FALSE){
        u8* anime = aKonsnowtv_on_anime[(s16)(ftr_actor->tex_animation.frame >> 1)];
        gSPSegment(NEXT_POLY_OPA_DISP, G_MWO_SEGMENT_8, anime);
    }
    else{
        gSPSegment(NEXT_POLY_OPA_DISP, G_MWO_SEGMENT_8, int_kon_snowtv07a_TA_tex_txt);
    }

    gSPDisplayList(NEXT_POLY_OPA_DISP,int_kon_snowtv_face_model);
    gSPDisplayList(NEXT_POLY_OPA_DISP,int_kon_snowtv_body_model);

    CLOSE_DISP(game->graph);
}
static void aKonsnowtv_dt(FTR_ACTOR* ftr_actor, u8* data){
    
}


static aFTR_vtable_c aKonsnowtv_func = {
	&aKonsnowtv_ct,
	&aKonsnowtv_mv,
	&aKonsnowtv_dw,
	&aKonsnowtv_dt,
	NULL,
};

aFTR_PROFILE iam_kon_snowtv = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	18.0f,
	0.01f,
	aFTR_SHAPE_TYPEA,
	mCoBG_FTR_TYPEA,
	0,
	0,
	0,
	aFTR_INTERACTION_START_DISABLED,
	&aKonsnowtv_func,
};
