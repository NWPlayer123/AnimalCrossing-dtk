extern aFTR_rig_c aTakBarber_sklkey;

extern cKF_Skeleton_R_c cKF_bs_r_int_tak_barber;
extern cKF_Animation_R_c cKF_ba_r_int_tak_barber;

aFTR_rig_c aTakBarber_sklkey = {
    &cKF_bs_r_int_tak_barber,
    &cKF_ba_r_int_tak_barber,
    1.0f,
};

aFTR_PROFILE iam_tak_barber = {
    NULL, NULL, NULL, NULL, NULL, NULL, &aTakBarber_sklkey, NULL, 40.0f, 0.01f, aFTR_SHAPE_TYPEA, mCoBG_FTR_TYPEA,
    0,    2,    0,    0,    NULL,
};
