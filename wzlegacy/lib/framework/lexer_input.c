/*This code copyrighted (2013) for the Warzone 2100 Legacy Project under the GPLv2.

Warzone 2100 Legacy is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Warzone 2100 Legacy is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Warzone 2100 Legacy; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA*/
#include "frame.h"
#include "lexer_input.h"

int lexer_input(lexerinput_t *input, char *buf, size_t max_size, int nullvalue)
{
    switch (input->type)
    {
        case LEXINPUT_PHYSFS:
            if (PHYSFS_eof(input->input.physfsfile))
            {
                buf[0] = EOF;
                return nullvalue;
            }
            else
            {
                int result = PHYSFS_read(input->input.physfsfile, buf, 1, max_size);
                if (result == -1)
                {
                    buf[0] = EOF;
                    return nullvalue;
                }
                return result;
            }
            break;

        case LEXINPUT_BUFFER:
            if (input->input.buffer.begin != input->input.buffer.end)
            {
                buf[0] = *input->input.buffer.begin++;
                return 1;
            }
            else
            {
                buf[0] = EOF;
                return nullvalue;
            }
            break;
    }

    ASSERT(!"Invalid input type!", "Invalid input type used for lexer (numeric value: %u)", (unsigned int)input->type);
    return nullvalue;
}
