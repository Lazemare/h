#include <include.h>

inline void update_Ep(struct MDSYSTEM *mdsystem)
{
	mdsystem->Ep = -1.0 * mdsystem->k * mdsystem->q * mdsystem->q + \
		mdsystem->q * mdsystem->q * mdsystem->q * mdsystem->q;
}

inline void update_f(struct MDSYSTEM *mdsystem)
{
	mdsystem->f = 2.0 * mdsystem->k * mdsystem->q - \
		4.0 * mdsystem->q * mdsystem->q * mdsystem->q;
}
