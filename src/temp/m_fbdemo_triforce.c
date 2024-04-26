#include "m_fbdemo_triforce.h"

#include "dataobject.h"
#include "gfxalloc.h"
#include "libultra/gu.h"
#include "libultra/libultra.h"
#include "m_rcp.h"

void fbdemo_triforce_startup(fbdemo_triforce* this) {
  this->finished = 0;

  if (this->direction != 0) {
    this->txt = 0;
  } else {
    this->txt = 0x224;
  }

  guPerspective(&this->perspmtx, &this->normal, 60.0f, 4.0f / 3.0f, 10.0f,
                12800.0f, 1.0f);
  guLookAt(&this->lookatmtx, 0.0f, 0.0f, 400.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
           0.0f);
}

fbdemo_triforce* fbdemo_triforce_init(fbdemo_triforce* this) {
  bzero(this, sizeof(fbdemo_triforce));
  return this;
}

void fbdemo_triforce_move(fbdemo_triforce* this, int updaterate) {
  int d;
  f32 t;
  t = (1.0f - (this->txt / 548.0f));
  if ((this->textureno == 1) || (this->textureno == 2)) {
    d = 0.5f * (14.0f + (12.0f * t));
  } else {
    d = 0.5f * (16.0f + (12.0f * t));
  }

  if (this->direction != 0) {
    this->txt += d;
    if (this->txt >= 548) {
      this->txt = 548;
      this->finished = TRUE;
    }
  } else {
    this->txt -= d;
    if (this->txt <= 0) {
      this->txt = 0;
      this->finished = TRUE;
    }
  }
}

void fbdemo_triforce_draw(fbdemo_triforce* this, Gfx** gfxP) {
  Gfx* gfx = *gfxP;
  Gfx* t;
  Gfx* scroll;

  gDPPipeSync(gfx++);
  scroll = gfx_tex_scroll2(&gfx, this->txt, 0, 16, 4);
  gSPSegment(gfx++, 0x9, scroll);
  gSPMatrix(gfx++, &this->perspmtx,
            G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
  gSPPerspNormalize(gfx++, this->normal);
  gSPMatrix(gfx++, &this->lookatmtx, G_MTX_MUL | G_MTX_PROJECTION);
  t = gfxalloc(&gfx, 0x40);
  guScale((Mtx*)t, 0.019f, 0.019f, 1.0f);

  gSPMatrix(gfx++, t, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

  switch (this->textureno) {
    case 0:
      gSPDisplayList(gfx++, ef_wipe1_modelT);
      break;
    case 1:
      gSPDisplayList(gfx++, ef_wipe2_modelT);
      break;
    case 2:
      gSPDisplayList(gfx++, ef_wipe3_modelT);
      break;
  }
  gDPPipeSync(gfx++);
  *gfxP = gfx;
}

s8 fbdemo_triforce_is_finish(fbdemo_triforce* this) { return this->finished; }

void fbdemo_triforce_settype(fbdemo_triforce* this, int type) {
  this->textureno = ((type & 0x80) != 0) ? 1 : ((type & 0x40) != 0 ? 2 : 0);
  this->direction = (type & 0xF) != 1;
}

void fbdemo_triforce_setcolor_rgba8888(void) {}
