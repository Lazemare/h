#include <include.h>

inline void update_Ep(struct MDSYSTEM *mdsystem)
{
	mdsystem->Ep = 0.5 * mdsystem->k * mdsystem->q * mdsystem->q;
}

inline void update_f(struct MDSYSTEM *mdsystem)
{
	mdsystem->f = -1.0 * mdsystem->k * mdsystem->q;
}
