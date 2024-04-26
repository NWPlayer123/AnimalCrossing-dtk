extern cKF_Skeleton_R_c cKF_bs_r_int_sugi_torch;
extern cKF_Animation_R_c cKF_ba_r_int_sugi_torch;

static void fSGT_ct(FTR_ACTOR* ftr_actor, u8* data) {
    cKF_SkeletonInfo_R_c* keyframe = &ftr_actor->keyframe;

    cKF_SkeletonInfo_R_ct(keyframe, &cKF_bs_r_int_sugi_torch, &cKF_ba_r_int_sugi_torch, ftr_actor->joint,
                          ftr_actor->morph);
    cKF_SkeletonInfo_R_init_standard_repeat(keyframe, &cKF_ba_r_int_sugi_torch, NULL);
    keyframe->frame_control.speed = 0.5f;
    cKF_SkeletonInfo_R_play(keyframe);
}

static void fSGT_mv(FTR_ACTOR* ftr_actor, ACTOR* my_room_actor, GAME* game, u8* data) {
    cKF_SkeletonInfo_R_c* keyframe = &ftr_actor->keyframe;

    cKF_SkeletonInfo_R_play(keyframe);
    keyframe->frame_control.speed = 0.5f;
    sAdo_OngenPos((u32)ftr_actor, 0x57, &ftr_actor->position);
}

extern Gfx sugi_fireto_model[];

static int fSGT_DwAfter(GAME* game, cKF_SkeletonInfo_R_c* keyframe, int joint_idx, Gfx** joint_shape, u8* joint_flags,
                        void* arg, s_xyz* joint_rot, xyz_t* joint_pos) {
    FTR_ACTOR* ftr_actor = (FTR_ACTOR*)arg;
    GAME_PLAY* play = (GAME_PLAY*)game;
    xyz_t pos;
    xyz_t zero_pos = { 0.0f, 0.0f, 0.0f };

    if (joint_idx == 2) {
        f32 scale_x = ftr_actor->scale.x * 0.01f;
        f32 scale_y = ftr_actor->scale.y * 0.01f;
        f32 scale_z = ftr_actor->scale.z * 0.01f;

        OPEN_DISP(game->graph);

        Matrix_Position(&zero_pos, &pos);
        Matrix_push();
        Matrix_translate(pos.x, pos.y, pos.z, 0);
        Matrix_mult(&play->billboard_matrix, 1);
        Matrix_RotateY(DEG2SHORT_ANGLE(90.0f), 1);
        Matrix_scale(scale_x, scale_y, scale_z, 1);

        gSPMatrix(NEXT_POLY_XLU_DISP, _Matrix_to_Mtx_new(game->graph), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        Matrix_pull();
        gSPDisplayList(NEXT_POLY_XLU_DISP, sugi_fireto_model);

        CLOSE_DISP(game->graph);
    }

    return TRUE;
}

static int fSGT_DwBefore(GAME* game, cKF_SkeletonInfo_R_c* keyframe, int joint_idx, Gfx** joint_shape, u8* joint_flags,
                         void* arg, s_xyz* joint_rot, xyz_t* joint_pos) {
    FTR_ACTOR* ftr_actor = (FTR_ACTOR*)arg;
    GAME_PLAY* play = (GAME_PLAY*)game;

    if (joint_idx == 2) {
        *joint_shape = NULL;
    }

    return TRUE;
}

static Gfx* fSGT_GetTwoTileGfx(int x0, int y0, int x1, int y1, GAME* game) {
    return two_tex_scroll_dolphin(game->graph, 0, x0, y0, 32, 32, 1, x1, y1, 32, 64);
}

static void fSGT_dw(FTR_ACTOR* ftr_actor, ACTOR* my_room_actor, GAME* game, u8* data) {
    GAME_PLAY* play = (GAME_PLAY*)game;
    cKF_SkeletonInfo_R_c* keyframe = &ftr_actor->keyframe;
    Mtx* mtx = ftr_actor->skeleton_mtx[game->frame_counter & 1];
    u32 ctr_ofs = ftr_actor->ctr_type == aFTR_CTR_TYPE_GAME_PLAY ? play->game_frame : game->frame_counter;
    Gfx* scroll_gfx = fSGT_GetTwoTileGfx(0, 0, 0, -ctr_ofs * 8, game);

    if (scroll_gfx != NULL) {
        OPEN_DISP(game->graph);

        gSPMatrix(NEXT_POLY_OPA_DISP, _Matrix_to_Mtx_new(game->graph), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPMatrix(NEXT_POLY_XLU_DISP, _Matrix_to_Mtx_new(game->graph), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPSegment(NEXT_POLY_XLU_DISP, G_MWO_SEGMENT_9, scroll_gfx); // fire scroll microcode commands

        CLOSE_DISP(game->graph);

        cKF_Si3_draw_R_SV(game, keyframe, mtx, &fSGT_DwBefore, &fSGT_DwAfter, ftr_actor);
    }
}

static aFTR_vtable_c fSGT_func = {
    &fSGT_ct, &fSGT_mv, &fSGT_dw, NULL, NULL,
};

aFTR_PROFILE iam_sugi_torch = {
    // clang-format off
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	41.0f,
	0.01f,
	aFTR_SHAPE_TYPEA,
	mCoBG_FTR_TYPEA,
	0,
	0,
	0,
	0,
	&fSGT_func,
    // clang-format on
};
