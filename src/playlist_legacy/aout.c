/*****************************************************************************
 * aout.c: audio output controls for the VLC playlist
 *****************************************************************************
 * Copyright (C) 2002-2012 VLC authors and VideoLAN
 *
 * Authors: Christophe Massiot <massiot@via.ecp.fr>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <math.h>

#include <vlc_common.h>
#include <vlc_aout.h>
#include <vlc_playlist_legacy.h>

#include "../audio_output/aout_internal.h"
#include "playlist_internal.h"

audio_output_t *playlist_GetAout(playlist_t *pl)
{
    /* NOTE: it is assumed that the input resource exists. In practice,
     * the playlist must have been activated. This is automatic when calling
     * pl_Get(). FIXME: input resources are deleted at deactivation, this can
     * be too early. */
    playlist_private_t *sys = pl_priv(pl);
    return input_resource_HoldAout(sys->p_input_resource);
}

float playlist_VolumeGet (playlist_t *pl)
{
    float volume = -1.f;

    audio_output_t *aout = playlist_GetAout (pl);
    if (aout != NULL)
    {
        volume = aout_VolumeGet (aout);
        aout_Release(aout);
    }
    return volume;
}

int playlist_VolumeSet (playlist_t *pl, float vol)
{
    int ret = -1;

    audio_output_t *aout = playlist_GetAout (pl);
    if (aout != NULL)
    {
        ret = aout_VolumeSet (aout, vol);
        aout_Release(aout);
    }
    return ret;
}

/**
 * Raises the volume.
 * \param value how much to increase (> 0) or decrease (< 0) the volume
 * \param volp if non-NULL, will contain contain the resulting volume
 */
int playlist_VolumeUp (playlist_t *pl, int value, float *volp)
{
    int ret = -1;

    audio_output_t *aout = playlist_GetAout (pl);
    if (aout != NULL)
    {
        ret = aout_VolumeUpdate (aout, value, volp);
        aout_Release(aout);
    }
    return ret;
}

int playlist_MuteGet (playlist_t *pl)
{
    int mute = -1;

    audio_output_t *aout = playlist_GetAout (pl);
    if (aout != NULL)
    {
        mute = aout_MuteGet (aout);
        aout_Release(aout);
    }
    return mute;
}

int playlist_MuteSet (playlist_t *pl, bool mute)
{
    int ret = -1;

    audio_output_t *aout = playlist_GetAout (pl);
    if (aout != NULL)
    {
        ret = aout_MuteSet (aout, mute);
        aout_Release(aout);
    }
    return ret;
}

void playlist_EnableAudioFilter (playlist_t *pl, const char *name, bool add)
{
    audio_output_t *aout = playlist_GetAout (pl);
    if (aout)
    {
        aout_EnableFilter(aout, name, add);
        aout_Release(aout);
    }
}
