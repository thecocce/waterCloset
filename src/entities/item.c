/*
Copyright (C) 2019 Parallel Realities

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "item.h"

static void tick(void);
static void touch(Entity *other);
static void die(void);
static void load(cJSON *root);
static void save(cJSON *root);

void initItem(Entity *e)
{
	Item *i;
	
	i = malloc(sizeof(Item));
	memset(i, 0, sizeof(Item));
	
	e->typeName = "item";
	e->type = ET_ITEM;
	e->data = i;
	e->flags = EF_WEIGHTLESS+EF_NO_ENT_CLIP;
	e->tick = tick;
	e->touch = touch;
	e->die = die;
	
	e->load = load;
	e->save = save;
	
	stage.totalItems++;
}

static void tick(void)
{
	/* nothing */
}

static void touch(Entity *other)
{
	if (self->health > 0 && other != NULL && (other->type == ET_PLAYER || other->type == ET_CLONE))
	{
		self->health = 0;
		
		playPositionalSound(SND_ITEM, CH_ITEM, self->x, self->y, stage.player->x, stage.player->y);
		
		stage.items++;
		
		if (stage.items == stage.totalItems && stage.coins == stage.totalCoins)
		{
			playPositionalSound(SND_FANFARE, CH_ITEM, self->x, self->y, stage.player->x, stage.player->y);
		}
		
		game.stats[STAT_ITEMS]++;
		
		game.currentStageMeta->itemsFound = MAX(stage.items, game.currentStageMeta->itemsFound);
	}
}

static void die(void)
{
	addPowerupParticles(self->x + self->w / 2, self->y + self->h / 2);
}

static void load(cJSON *root)
{
	Item *item;
	
	item = (Item*)self->data;
	
	STRNCPY(item->textureFilename, cJSON_GetObjectItem(root, "textureFilename")->valuestring, MAX_NAME_LENGTH);
	
	self->atlasImage = getAtlasImage(item->textureFilename, 1);
	self->w = self->atlasImage->rect.w;
	self->h = self->atlasImage->rect.h;
}

static void save(cJSON *root)
{
	Item *item;
	
	item = (Item*)self->data;
	
	cJSON_AddStringToObject(root, "textureFilename", item->textureFilename);
}
