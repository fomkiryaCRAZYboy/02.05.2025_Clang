#include "cipher.h"

/*
    --Шифруются только английские буквы и арабские цифры, но не другие символы--
    
    Если номер буквы в алфавите меньше или равен 13, то она заменяется буквой,
    номер которой на 2 больше текущего номера. Буквы, номера которых больше 13,
    заменяются буквами с номерами на 2 меньше текущего номера.

    Код цифры умножается на три
*/

u8 search_code57(u8 digit);

u8 search_code52(u8 digit);
   

u8* encryption(const u8* msg, u32 msglen)
{
    u8* encrypted_msg = malloc(msglen+1);

    u32 i;
    for(i = 0; i < msglen; ++i)
    {
        if(isalpha(msg[i]))
        {
            /*
                если код символа от 65 до 90 включительно, то это заглавные буквы,
                значит, алгоритм шифрования определяем в соответствии с кодом.

                [65, 77] [78, 90]

                если код символа от 65 до 90 включительно, то это строчные буквы,
                значит, алгоритм шифрования определяем в соответствии с кодом.
                
                [97, 109] [110, 122]
            */

            if((u32)msg[i] <= 77 || 
               (u32)msg[i] >= 97 && 
               (u32)msg[i] <= 109)
            {
                encrypted_msg[i] = (u32)msg[i] + 2;
                continue;
            }
            else
            {
                encrypted_msg[i] = (u32)msg[i] - 3;
                continue;
            }
        }

        if(isdigit(msg[i]))
        {

            /* цифры от 0 до 4 */

            if((u32)msg[i] <= 52)
            {
                encrypted_msg[i] = search_code52(msg[i]);
                continue;
            }
            else 
            {
                encrypted_msg[i] = search_code57(msg[i]);
                continue;
            }
        }

        encrypted_msg[i] = msg[i];
    }

    return encrypted_msg;
}


u8 search_code52(u8 digit)
{
    switch(digit)
    {
        case (u8)48:  return (u8)(48 - 1);
        case (u8)49:  return (u8)(49 - 3);
        case (u8)50:  return (u8)(50 - 5);
        case (u8)51:  return (u8)(51 - 7);
        case (u8)52:  return (u8)(52 - 9);

        default:      return (u8)150;
    }
}

u8 search_code57(u8 digit)
{
    switch(digit)
    {
        case (u8)53:  return (u8)(53 + 9);
        case (u8)54:  return (u8)(54 + 7);
        case (u8)55:  return (u8)(55 + 5);
        case (u8)56:  return (u8)(56 + 3);
        case (u8)57:  return (u8)(57 + 1);

        default:    return (u8)150;
    }
}
