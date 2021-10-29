/*  This file is part of Ficus.

    Ficus is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Ficus is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Ficus.  If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef FICUS_H_

#define FICUS_H_

// Maximum number of active clients
#define SERVER_BACKLOG 10
// Maximum content size
#define TRANSFERABLE_SIZE 51200

// Macro Stringizing
#define xstr(s) str(s)
#define str(s) #s

void on_close_interrupt(int);

#endif
