#include "../tomb3/pch.h"
#include "triboss.h"
#include "objects.h"
#include "sphere.h"
#include "../3dsystem/phd_math.h"
#include "box.h"
#include "effect2.h"
#include "../specific/game.h"
#include "sound.h"
#include "items.h"
#include "lot.h"
#include "control.h"

static long radii[5] = { 200, 400, 500, 500, 475 };
static long heights[5] = { -1536, -1280, -832, -384, 0 };
static long dradii[5] = { 1600, 5600, 6400, 5600, 1600 };
static long dheights1[5] = { -7680, -4224, -768, 2688, 6144 };
static long dheights2[5] = { -1536, -1152, -768, -384, 0 };
static long death_radii[5];
static long death_heights[5];

static short FindLizardManItemNumber(short room_number)
{
	ITEM_INFO* item;

	for (int i = 0; i < level_items; i++)
	{
		item = &items[i];

		if (item->object_number == LIZARD_MAN && item->room_number == room_number)
			return i;
	}

	return NO_ITEM;
}

static void RotateHeadXAngle(ITEM_INFO* item)
{
	PHD_VECTOR pos;
	PHD_VECTOR lpos;
	long dx, dy, dz;
	short ang;

	lpos.x = 0;
	lpos.y = 0;
	lpos.z = 0;
	GetJointAbsPosition(lara_item, &lpos, HIPS);

	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	GetJointAbsPosition(item, &pos, HIPS);

	dx = abs(pos.x - lpos.x);
	dy = pos.y - lpos.y;
	dz = abs(pos.z - lpos.z);
	ang = (short)phd_atan(phd_sqrt(SQUARE(dx) + SQUARE(dz)), dy);

	if (abs(ang) < 0x2000)
		CreatureJoint(item, 2, ang);
	else
		CreatureJoint(item, 2, 0);
}

static void ExplodeTribeBoss(ITEM_INFO* item)
{
	SHIELD_POINTS* p;
	long x, y, z, rad, angle, r, g, b, m;

	TribeBossShieldOn = 0;

	if (bossdata.explode_count == 1 || bossdata.explode_count == 15 || bossdata.explode_count == 25 ||
		bossdata.explode_count == 35 || bossdata.explode_count == 45 || bossdata.explode_count == 55)
	{
		x = (GetRandomDraw() & 0x3FF) + item->pos.x_pos - 512;
		y = item->pos.y_pos - (GetRandomDraw() & 0x3FF) - 256;
		z = (GetRandomDraw() & 0x3FF) + item->pos.z_pos - 512;
		ExpRings[bossdata.ring_count].x = x;
		ExpRings[bossdata.ring_count].y = y;
		ExpRings[bossdata.ring_count].z = z;
		ExpRings[bossdata.ring_count].on = 1;
		bossdata.ring_count++;
		TriggerExplosionSparks(x, y, z, 3, -2, 2, 0);

		for (int i = 0; i < 2; i++)
			TriggerExplosionSparks(x, y, z, 3, -1, 2, 0);

		SoundEffect(SFX_BLAST_CIRCLE, &item->pos, 0x800000 | SFX_SETPITCH);
	}

	for (int i = 0; i < 5; i++)
	{
		if (bossdata.explode_count < 128)
		{
			death_radii[i] = (dradii[i] >> 4) + ((bossdata.explode_count * dradii[i]) >> 7);
			death_heights[i] = dheights2[i] + ((bossdata.explode_count * (dheights1[i] - dheights2[i])) >> 7);
		}
	}

	p = TribeBossShield;

	for (int i = 0; i < 5; i++)
	{
		y = death_heights[i];
		rad = death_radii[i];
		angle = (wibble & 0x3F) << 3;

		for (int j = 0; j < 8; j++, p++)
		{
			p->x = short((rad * rcossin_tbl[angle << 1]) >> 11);
			p->y = (short)y;
			p->z = short((rad * rcossin_tbl[(angle << 1) + 1]) >> 11);

			if (!i || i == 16 || bossdata.explode_count >= 64)
				p->rgb = 0;
			else
			{
				m = 64 - bossdata.explode_count;
				r = (m * (GetRandomDraw() & 0x1F)) >> 6;
				b = (GetRandomDraw() & 0x3F) + 224;
				g = (m * ((b >> 2) + (GetRandomDraw() & 0x3F))) >> 6;
				b = (m * b) >> 6;
				p->rgb = (b << 16) | (g << 8) | r;	//bgr
			}

			angle = (angle + 512) & 0xFFF;
		}
	}
}

static void TribeBossDie(short item_number)
{
	ITEM_INFO* item;

	item = &items[item_number];
	item->collidable = 0;
	item->hit_points = NO_TARGET;
	KillItem(item_number);
	DisableBaddieAI(item_number);
	item->flags |= IFL_INVISIBLE;
}

void InitialiseTribeBoss(short item_number)
{
	ITEM_INFO* item;
	SHIELD_POINTS* p;
	long r, y, angle;

	item = &items[item_number];
	bossdata.LizmanItem = FindLizardManItemNumber(item->room_number);
	bossdata.LizmanRoom = items[bossdata.LizmanItem].room_number;

	for (int i = 0; i < 3; i++)
		TrigDynamics[i].x = 0;

	bossdata.dropped_icon = 0;
	bossdata.dead = 0;
	bossdata.ring_count = 0;
	bossdata.explode_count = 0;
	bossdata.attack_head_count = 0;
	bossdata.death_count = 0;
	bossdata.attack_flag = 0;
	bossdata.attack_count = 0;
	shield_active = 0;
	TribeBossShieldOn = 0;
	lizard_man_active = 0;

	p = TribeBossShield;

	for (int i = 0; i < 5; i++)
	{
		y = heights[i];
		r = radii[i];
		angle = 0;

		for (int j = 0; j < 8; j++, p++, angle += 1024)
		{
			p->x = short((r * rcossin_tbl[angle]) >> 11);
			p->y = (short)y;
			p->z = short((r * rcossin_tbl[angle + 1]) >> 11);
			p->rgb = 0;
		}
	}
}

void TriggerSummonSmoke(long x, long y, long z)
{
	SPARKS* sptr;

	sptr = &sparks[GetFreeSpark()];
	sptr->On = 1;
	sptr->sR = 16;
	sptr->sG = 64;
	sptr->sB = 0;
	sptr->dR = 8;
	sptr->dG = 32;
	sptr->dB = 0;
	sptr->FadeToBlack = 64;
	sptr->ColFadeSpeed = (GetRandomControl() & 7) + 16;
	sptr->TransType = 2;
	sptr->Life = (GetRandomControl() & 0xF) + 96;
	sptr->sLife = sptr->Life;
	sptr->extras = 0;
	sptr->Dynamic = -1;
	sptr->x = (GetRandomControl() & 0x7F) + x - 64;
	sptr->y = y - (GetRandomControl() & 0x1F);
	sptr->z = (GetRandomControl() & 0x7F) + z - 64;
	sptr->Xvel = (GetRandomControl() & 0xFF) - 128;
	sptr->Yvel = -16 - (GetRandomControl() & 0xF);
	sptr->Zvel = (GetRandomControl() & 0xFF) - 128;
	sptr->Friction = 0;

	if (GetRandomControl() & 1)
	{
		sptr->Flags = 794;
		sptr->RotAng = GetRandomControl() & 0xFFF;

		if (GetRandomControl() & 1)
			sptr->RotAdd = -4 - (GetRandomControl() & 7);
		else
			sptr->RotAdd = (GetRandomControl() & 7) + 4;
	}
	else
		sptr->Flags = 778;

	sptr->Scalar = 3;
	sptr->Def = (uchar)objects[EXPLOSION1].mesh_index;
	sptr->Gravity = -8 - (GetRandomControl() & 7);
	sptr->MaxYvel = -4 - (GetRandomControl() & 7);
	sptr->dWidth = (GetRandomControl() & 0x1F) + 128;
	sptr->sWidth = sptr->dWidth >> 1;
	sptr->Width = sptr->dWidth >> 1;
	sptr->dHeight = sptr->dWidth + (GetRandomControl() & 0x1F) + 32;
	sptr->sHeight = sptr->dHeight >> 1;
	sptr->Height = sptr->dHeight >> 1;
}

void TriggerLizardMan()
{
	ITEM_INFO* item;
	ROOM_INFO* r;
	short room_number;

	item = &items[bossdata.LizmanItem];
	item->object_number = LIZARD_MAN;
	item->room_number = bossdata.LizmanRoom;
	item->pos.x_pos = lizman_summon_coords[bossdata.attack_type - 1][0];
	item->pos.y_pos = lizman_summon_coords[bossdata.attack_type - 1][1];
	item->pos.z_pos = lizman_summon_coords[bossdata.attack_type - 1][2];
	item->anim_number = objects[LIZARD_MAN].anim_index;
	item->frame_number = anims[item->anim_number].frame_base;
	item->current_anim_state = anims[item->anim_number].current_anim_state;
	item->goal_anim_state = anims[item->anim_number].current_anim_state;
	item->required_anim_state = 0;
	item->pos.x_rot = 0;

	if (bossdata.attack_type == 1)
		item->pos.y_rot = -0x8000;
	else
		item->pos.y_rot = 0;

	item->pos.z_rot = 0;
	item->timer = 0;
	item->flags = 0;
	item->data = 0;
	item->mesh_bits = -1;
	item->hit_points = objects[LIZARD_MAN].hit_points;
	item->active = 0;
	item->status = ITEM_ACTIVE;
	item->collidable = 1;
	AddActiveItem(bossdata.LizmanItem);
	EnableBaddieAI(bossdata.LizmanItem, 1);

	room_number = item->room_number;
	GetFloor(item->pos.x_pos, item->pos.y_pos, item->pos.z_pos, &room_number);

	if (item->room_number != room_number)
		ItemNewRoom(bossdata.LizmanItem, room_number);

	lizard_man_active = 1;
	RemoveDrawnItem(bossdata.LizmanItem);

	r = &room[item->room_number];
	item->next_item = r->item_number;
	r->item_number = bossdata.LizmanItem;
}

void TriggerElectricSparks(GAME_VECTOR* pos, long shield)
{
	SPARKS* sptr;
	long dx, dz;

	dx = lara_item->pos.x_pos - pos->x;
	dz = lara_item->pos.z_pos - pos->z;

	if (dx < -0x5000 || dx > 0x5000 || dz < -0x5000 || dz > 0x5000)
		return;

	TrigDynamics[1].x = pos->x;
	TrigDynamics[1].y = pos->y;
	TrigDynamics[1].z = pos->z;
	
	sptr = &sparks[GetFreeSpark()];
	sptr->On = 1;
	sptr->sR = 255;
	sptr->sG = 255;
	sptr->sB = 255;

	if (shield)
	{
		sptr->dR = 255;
		sptr->dG = (GetRandomControl() & 0x7F) + 64;
		sptr->dB = 0;
	}
	else if (bossdata.attack_type)
	{
		sptr->dR = 0;
		sptr->dB = (GetRandomControl() & 0x7F) + 64;
		sptr->dG = (sptr->dB >> 1) + 128;
	}
	else
	{
		sptr->dR = 0;
		sptr->dG = (GetRandomControl() & 0x7F) + 64;
		sptr->dB = (sptr->dG >> 1) + 128;
	}

	sptr->ColFadeSpeed = 3;
	sptr->FadeToBlack = 8;
	sptr->Life = 16;
	sptr->sLife = 16;
	sptr->TransType = 2;
	sptr->Dynamic = -1;
	sptr->x = (GetRandomControl() & 0x1F) + pos->x - 16;
	sptr->y = (GetRandomControl() & 0x1F) + pos->y - 16;
	sptr->z = (GetRandomControl() & 0x1F) + pos->z - 16;
	sptr->Xvel = 4 * (GetRandomControl() & 0x1FF) - 1024;
	sptr->Yvel = 2 * (GetRandomControl() & 0x1FF) - 512;
	sptr->Zvel = 4 * (GetRandomControl() & 0x1FF) - 1024;

	if (shield)
	{
		sptr->Xvel >>= 1;
		sptr->Yvel >>= 1;
		sptr->Zvel >>= 1;
	}

	sptr->Friction = 4;
	sptr->Flags = 2;
	sptr->Scalar = 3;
	sptr->Width = (GetRandomControl() & 1) + 1;
	sptr->sWidth = sptr->Width;
	sptr->dWidth = (GetRandomControl() & 3) + 4;
	sptr->Height = (GetRandomControl() & 1) + 1;
	sptr->sHeight = sptr->Height;
	sptr->dHeight = (GetRandomControl() & 3) + 4;
	sptr->Gravity = 15;
	sptr->MaxYvel = 0;
}

void FindClosestShieldPoint(long x, long y, long z, ITEM_INFO* item)
{
	SHIELD_POINTS* p;
	GAME_VECTOR pos;
	long affected[5];
	long point, dist, bestdist, dx, dy, dz, c, n;
	long r, g, b;

	affected[0] = 0;
	affected[1] = -1;
	affected[2] = 1;
	affected[3] = -8;
	affected[4] = 8;

	bestdist = 0x7FFFFFFF;
	point = 0;

	for (int i = 0; i < 40; i++)
	{
		p = &TribeBossShield[i];

		if (i >= 16 && i <= 23)
		{
			dx = p->x + item->pos.x_pos - x;
			dy = p->y + item->pos.y_pos - y;
			dz = p->z + item->pos.z_pos - z;
			dist = SQUARE(dx) + SQUARE(dy) + SQUARE(dz);

			if (dist < bestdist)
			{
				bestdist = dist;
				point = i;
			}
		}
	}

	switch (lara.gun_type)
	{
	case LG_PISTOLS:
	case LG_UZIS:
		c = 144;
		break;

	case LG_MAGNUMS:
	case LG_HARPOON:
		c = 200;
		break;

	case LG_SHOTGUN:
	case LG_M16:
		c = 192;
		break;

	case LG_ROCKET:
	case LG_GRENADE:
		c = 224;
		break;

	default:
		c = 0;
		break;
	}

	for (int i = 0; i < 5; i++)
	{
		n = point + affected[i];

		if ((n & 7) == 7 && affected[i] == -1)
			n += 8;

		if (!(n & 7) && affected[i] == 1)
			n -= 8;

		p = &TribeBossShield[n];

		r = p->rgb & 0xFF;
		g = (p->rgb >> 8) & 0xFF;
		b = (p->rgb >> 16) & 0xFF;

		if (i)
		{
			if (c < 200)
			{
				r += c >> 3;

				if (r > c >> 1)
					r = c >> 1;
			}
			else
				r = c >> 1;
		}
		else if (c < 200)
		{
			r += c >> 2;

			if (r > c)
				r = c;
		}
		else
			r = c;

		if (i)
		{
			if (c < 200)
			{
				g += c >> 3;

				if (g > c >> 1)
					g = c >> 1;
			}
			else
				g = c >> 1;
		}
		else if (c < 200)
		{
			g += c >> 2;

			if (g > c)
				g = c;
		}
		else
			g = c;

		if (i)
		{
			if (c < 200)
			{
				b += c >> 3;

				if (b > c >> 1)
					b = c >> 1;
			}
			else
				b = c >> 1;
		}
		else if (c < 200)
		{
			b += c >> 2;

			if (b > c)
				b = c;
		}
		else
			b = c;

		p->rsub = (GetRandomControl() & 7) + 8;
		p->gsub = (GetRandomControl() & 7) + 8;
		p->bsub = (GetRandomControl() & 7) + 8;

		if (lara.gun_type == LG_ROCKET || lara.gun_type == LG_GRENADE)
		{
			p->rsub >>= 1;
			p->gsub >>= 1;
			p->bsub >>= 1;
		}

		p->rgb = (b << 16) | (g << 8) | r;
	}

	pos.x = x;
	pos.y = y;
	pos.z = z;

	for (int i = 0; i < 7; i++)
		TriggerElectricSparks(&pos, 1);
}

void inject_triboss(bool replace)
{
	INJECT(0x00471FB0, FindLizardManItemNumber, replace);
	INJECT(0x00471570, InitialiseTribeBoss, replace);
	INJECT(0x00471960, RotateHeadXAngle, replace);
	INJECT(0x00471E30, TriggerSummonSmoke, replace);
	INJECT(0x00471BD0, ExplodeTribeBoss, replace);
	INJECT(0x00471520, TribeBossDie, replace);
	INJECT(0x00471A30, TriggerLizardMan, replace);
	INJECT(0x00471350, TriggerElectricSparks, replace);
	INJECT(0x00471680, FindClosestShieldPoint, replace);
}
