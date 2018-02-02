
//
extern void user_abort(void);

void abort(void)
{
	user_abort();
}