struct continuation;

char* getrbp() asm("_getrbp");
char* getrsp() asm("_getrsp");
int reify(char* start, struct continuation* p, int initial) asm("_reify");
void reinstate(char* start, struct continuation* p, int value) asm("_reinstate");
