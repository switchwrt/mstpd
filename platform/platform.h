/*****************************************************************************
  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc., 59
  Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  The full GNU General Public License is included in this distribution in the
  file called LICENSE.

  Authors: Pavel Šimerda

******************************************************************************/

#include <linux/types.h>

int bridge_port_vlan_configure(unsigned ifindex, __u16 vid, __u16 fid, __u16 mstid);
int bridge_port_tree_set_state(unsigned ifindex, __u16 mstid, __u8 state);
