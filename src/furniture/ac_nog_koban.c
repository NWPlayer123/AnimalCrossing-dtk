extern u16 int_nog_kouban_on_pal[] ATTRIBUTE_ALIGN(32) = {
#include "assets/int_nog_kouban_on_pal.inc"
};

extern u16 int_nog_kouban_off_pal[] ATTRIBUTE_ALIGN(32) = {
#include "assets/int_nog_kouban_off_pal.inc"
};

static void fNKN_ct(FTR_ACTOR* ftr_actor, u8* data) {
    ftr_actor->pal_p = (u16*)zelda_malloc_align(16 * sizeof(u16), 32);
    fFTR_MorphHousepaletteCt(ftr_actor->pal_p, int_nog_kouban_off_pal, int_nog_kouban_on_pal, ftr_actor);
}

static void fNKN_mv(FTR_ACTOR* ftr_actor, ACTOR* my_room_actor, GAME* game, u8* data) {
    fFTR_MorphHousePalette(ftr_actor->pal_p, int_nog_kouban_off_pal, int_nog_kouban_on_pal, ftr_actor);
}

static void fNKN_dt(FTR_ACTOR* ftr_actor, u8* data) {
    if (ftr_actor->pal_p != NULL) {
        zelda_free(ftr_actor->pal_p);
    }
}

extern Gfx int_nog_koban_on_model[];
extern Gfx int_nog_koban_onT_model[];
extern Gfx int_nog_koban_offT_model[];

static void fNKN_dw(FTR_ACTOR* ftr_actor, ACTOR* my_room_actor, GAME* game, u8* data) {
    u16* pal_p = ftr_actor->pal_p;

    OPEN_DISP(game->graph);

    gSPMatrix(NEXT_POLY_OPA_DISP, _Matrix_to_Mtx_new(game->graph), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(NEXT_POLY_OPA_DISP, G_MWO_SEGMENT_8, pal_p);
    gSPDisplayList(NEXT_POLY_OPA_DISP, int_nog_koban_on_model);
    gSPDisplayList(NEXT_POLY_OPA_DISP, int_nog_koban_onT_model);
    gSPDisplayList(NEXT_POLY_OPA_DISP, int_nog_koban_offT_model);

    CLOSE_DISP(game->graph);
}

static aFTR_vtable_c fNKN_func = {
    &fNKN_ct, &fNKN_mv, &fNKN_dw, &fNKN_dt, NULL,
};

aFTR_PROFILE iam_nog_koban = {
    // clang-format off
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	15.7f,
	0.01f,
	aFTR_SHAPE_TYPEA,
	mCoBG_FTR_TYPEA,
	0,
	0,
	0,
	aFTR_INTERACTION_TOGGLE,
	&fNKN_func,
    // clang-format on
};
