#ifndef PRAC3_3_4_HANDLERS_H
#define PRAC3_3_4_HANDLERS_H


/* SCREEN handlers */
void
deficit_money(int sig);

void
enough_money(int sig);

void
add_money(int sig);


/* MECH handlers: */
void put_item(int sig);


/* NOTECHANGER handlers: */
void
receive_money(int sig);


/* BUTTON handlers: */
void
click(int sig);



#endif //PRAC3_3_4_HANDLERS_H
